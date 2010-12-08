#include "ExplosionObject.h"
#include "Level.h"
#include "functions.h"
#include "Source.h"
#include <TypeConverter.h>

using namespace TypeConverter;

ExplosionObject::ExplosionObject(TiXmlElement *XMLNode,Level *lvl,Ogre::Vector3 positionOffset)
{
	//!TODO!
	/*aufbau:
	<object pos="1 1 1" orient="1 1 1 1" lifetime="5">
		<explosion>
			<worldArt type="billboard" material="test/spark" />
			<sound value="ESPARK1.wav" />
			<lifespan value="1.5" />
		</explosion>
	</object>*/
	if(!XMLNode->Attribute("pos") || !XMLNode->FirstChildElement("explosion"))
		throw etCorruptXml;

	Ogre::Vector3 pos = Ogre::StringConverter::parseVector3(XMLNode->Attribute("pos"));
	ExplosionData data = StandardApplication::getSingletonPtr()->parseExplosionData(XMLNode->FirstChildElement("explosion"));
	
	Ogre::Quaternion ornt;
	Ogre::Vector3 scale;
	int SaveGameID = 0;
	

	if(XMLNode->Attribute("orient"))
	{
		ornt = Ogre::StringConverter::parseQuaternion(XMLNode->Attribute("orient"));
	}
	if(XMLNode->Attribute("scale"))
	{
		scale = Ogre::StringConverter::parseVector3(XMLNode->Attribute("orient"));
	}
	if(XMLNode->Attribute("SGID"))
	{
		SaveGameID = Ogre::StringConverter::parseInt(XMLNode->Attribute("SGID"));
	}
	Ogre::Real _lifetime = 0;
	if(XMLNode->Attribute("lifetime"))
	{
		lifetime = Ogre::StringConverter::parseReal(XMLNode->Attribute("lifetime"));
	}
	bool _nolifetime = false;
	if(XMLNode->Attribute("noLifeTime"))
	{
		_nolifetime = Ogre::StringConverter::parseBool(XMLNode->Attribute("noLifeTime"));
	}
	bool _effectonly = false;
	if(XMLNode->Attribute("effectOnly"))
	{
		_effectonly = Ogre::StringConverter::parseBool(XMLNode->Attribute("effectOnly"));
	}

	gamedata_damagedealer *castitem = NULL;
	if(XMLNode->Attribute("castingItem") && XMLNode->Attribute("castingItemType"))
	{
		Ogre::String strType = XMLNode->Attribute("castingItemType");
		Ogre::String itemID = XMLNode->Attribute("castingItem");
		StandardApplication *app = StandardApplication::getSingletonPtr();
		if(strType == "item")
		{
			castitem = app->getItemDataPtr(itemID);
		}
		else
		{
			castitem = app->getSpellDataPtr(itemID);
		}
	}

	GameObject * caster = NULL;
	if(XMLNode->Attribute("caster"))
	{
		int casterID = Ogre::StringConverter::parseInt(XMLNode->Attribute("caster"));
		caster = lvl->getObjectBySGID( casterID );
	}
	GameObject * target = NULL;
	if(XMLNode->Attribute("target"))
	{
		int casterID = Ogre::StringConverter::parseInt(XMLNode->Attribute("target"));
		target = lvl->getObjectBySGID( casterID );
	}

	init(caster,target,castitem,lvl,pos,_effectonly,_nolifetime,ornt,scale,SaveGameID);
	lifetime = _lifetime;
	if(mSnd)
	{
		mSnd->setMsecOffset(lifetime);
	}
	if(XMLNode->Attribute("inlevelfile"))
	{
		isInLevelFile = Ogre::StringConverter::parseBool(XMLNode->Attribute("inlevelfile"));
	}

}
void ExplosionObject::init(GameObject * caster,GameObject * target,gamedata_damagedealer *castingItem,Level *lvl,Ogre::Vector3 pos,bool effect_only, bool liveForever,Ogre::Quaternion ornt,Ogre::Vector3 scale,int SaveGameID)
{
	if(!castingItem->Explosion)
	{
		mLog("Error: Could not construct Explosion, because item/spell \""+castingItem->id+"\" contains no ExplosionData");
		throw EX_CONSTR_FAILED;
	}
	effectOnly = effect_only;
	noLifeTime = liveForever;
	mCaster = caster;
	mCastingItem = castingItem;
	isInLevelFile = false;
	SGID = SaveGameID;
	lifetime = 0;
	mNode = NULL;
	mArt = NULL;
	mLevel = lvl;
	mSnd = NULL;
	//myData = data;
	type = otExplosion;
	ID="effect";
	Ogre::SceneManager *mSceneMgr = mLevel->getSceneManager();
	//mNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	mBody = NULL;
	//mNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	try
	{
		mArt = new WorldArt(this,mCastingItem->Explosion->wData);
		mArt->modScale(scale);
	}
	catch(ExceptionType)
	{
		//dann gibts kein worldart. macht nix
	}
	//mNode = mArt->getSceneNode();
	if(mArt)
        mNode = mArt->getSceneNode();
    else
        mNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	mNode->setPosition(pos);
	mNode->setOrientation(ornt);
    if(target && target->getType() == gtCharacter)
	    mTarget = target;
    else
        mTarget = NULL;
	//jetzt der eigentliche effekt
    if(target && !effectOnly)
    {
		//nun schauen. spell?
		gamedata_spell *spell = NULL;
		if(mCastingItem->type == gtSpell)
			spell = static_cast<gamedata_spell*>(mCastingItem);
		else
		{
			gamedata_item *item = static_cast<gamedata_item*>(mCastingItem);
			//if(item->rangedSpell)
			spell = item->rangedSpell;
			//hier auch damage
            target->onDamage(item->rangedDamage,mCaster);
		}
	    if(spell)
            target->AddSpell(spell,mCaster);
	    
    }

	//jetzt sound
	if(mCastingItem->Explosion->sound != "")
	{
		
		mSnd = mLevel->createSource(NULL,stSfxLevel,mCastingItem->Explosion->loopSound);//StandardApplication::getSingletonPtr()->createSound(myData.sound,stSfxLevel,myData.loopSound);
		if(mSnd)
		{
			mSnd->setBuffer(mLevel->getBuffer(mCastingItem->Explosion->sound));
			mSnd->setPosition(pos);
			//mSnd->rewind();
			mSnd->play();
		}
		//		mNode->attachObject(mSnd);
//		mSnd->play();
//		mLog("erstelle sound im ExplosionObject konstruktor:");
//		static_cast<TestSound*>(mSnd)->printLog();

	}
	mLevel->registerObject(this);
}
ExplosionObject::~ExplosionObject()
{
//	//mLog("specialeffect destruktor begin");
//	mLevel->unregisterObject(this);
//	//mLog("ExplObject destructor: this="+ptrToString(this));
//	if(mArt)
//		delete mArt;
	if(mSnd)
	{
		
		mSnd->stop();
		mLevel->destroySource(mSnd);
//		OgreAL::SoundManager::getSingletonPtr()->destroySound(mSnd);
	}
//	
//	destroyNode(mLevel->getSceneManager(),mNode);
}

TiXmlElement* ExplosionObject::getXmlElem(Ogre::Vector3 startPosition,bool savegame)
{
	//in diesem Fall ist es egal, auf was savegame steht
	//ObjType type = obj->getType();
	Ogre::String strtype = objectTypeToString(type);
	if(strtype == "")
		return NULL;
	TiXmlElement* xmlElem = new TiXmlElement(strtype.c_str());
	Ogre::Vector3 pos, scale;
	Ogre::Quaternion ornt;
//	Ogre::String id = obj->getID(); = ID
		
	scale = getScale();
	getPositionOrientation(pos,ornt);
		
	pos -= startPosition;
	xmlElem->SetAttribute("pos",Ogre::StringConverter::toString(pos).c_str());
	if(ornt != Ogre::Quaternion::IDENTITY)
	{
		xmlElem->SetAttribute("orient",Ogre::StringConverter::toString(ornt).c_str());
	}
	if(scale != Ogre::Vector3::UNIT_SCALE)
	{
		xmlElem->SetAttribute("scale",Ogre::StringConverter::toString(scale).c_str());
	}
	if(SGID)
	{
		xmlElem->SetAttribute("SGID",Ogre::StringConverter::toString(SGID).c_str());
	}
	xmlElem->SetAttribute("lifetime",Ogre::StringConverter::toString(lifetime).c_str());
	xmlElem->SetAttribute("effectOnly",Ogre::StringConverter::toString(effectOnly).c_str());
	xmlElem->SetAttribute("noLifeTime",Ogre::StringConverter::toString(noLifeTime).c_str());
    if(mCaster)
		xmlElem->SetAttribute("caster",ogre_str(mCaster->getSGID()));
    if(mTarget)
		xmlElem->SetAttribute("target",ogre_str(mTarget->getSGID()));
	
	xmlElem->SetAttribute("inlevelfile",Ogre::StringConverter::toString(isInLevelFile).c_str());
	if(mCastingItem->type == gtItem)
	{
		xmlElem->SetAttribute("castingItemType","item");
	}
	else
	{
		xmlElem->SetAttribute("castingItemType","spell");
	}
	xmlElem->SetAttribute("castingItem",mCastingItem->id);
/*
	TiXmlElement *explData = new TiXmlElement("explosion");
	StandardApplication *app = StandardApplication::getSingletonPtr();
	app->explosionDataToXml(explData,myData);
	xmlElem->LinkEndChild(explData);*/

	xmlElem->SetAttribute("lifetime",ogre_str(lifetime).c_str());
	/*TiXmlElement *effData = new TiXmlElement("effect");
	app->spellEffectDataToXml*/
	
	
	return xmlElem;
}



void ExplosionObject::update(Ogre::Real time)
{
    if(mTarget)
	{
		//am target bleiben
		Ogre::Vector3 targetPos = mTarget->getPosition();
		setPosition(targetPos);
		mSnd->setPosition(targetPos);
	}
	if(!noLifeTime)
	{
		bool finished = false;
		if(mCastingItem->Explosion->lifespan == 0)// && mSnd)
		{
			/*if(mSnd->isStopped())
				finished = true;*/
		}
		else
		{
			lifetime += time;
			if (lifetime >= mCastingItem->Explosion->lifespan )
				finished = true;
		}
		if(finished)
		{
		/*	if(mSnd)
			{
				mLog("specialeffect löscht sound: name="+mSnd->getName()+", file="+mSnd->getFileName());
				mSnd->stop();
				mNode->detachObject(mSnd);
				OgreAL::SoundManager::getSingletonPtr()->destroySound(mSnd);
				mSnd = NULL;
				
			}*/
			mLevel->deleteObject(this);
			
		}
	}
}