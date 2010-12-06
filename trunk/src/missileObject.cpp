#include "MissileObject.h"
#include "Level.h"
#include "functions.h"
#include "GameObject.h"
#include "ExplosionObject.h"
#include "Source.h"

//MissileObject::MissileObject(GameObject *_caster,MissileData data, Level *lvl, Ogre::Vector3 pos, Ogre::Quaternion ornt, Ogre::Vector3 v0)
//{
//}
MissileObject::MissileObject(TiXmlElement *XMLNode, Level *lvl,Ogre::Vector3 positionOffset)
{
	//SpecificMissileData data;
	
	Ogre::Vector3 velocity  = Ogre::StringConverter::parseVector3(XMLNode->Attribute("velocity"));
	Ogre::Vector3 omega = Ogre::Vector3::ZERO;
	int SaveGameID = 0;

	Ogre::Vector3 pos = Ogre::StringConverter::parseVector3(XMLNode->Attribute("pos"))+positionOffset;
	Ogre::Vector3 scale = Ogre::Vector3::UNIT_SCALE;
	Ogre::Quaternion ornt = Ogre::Quaternion::IDENTITY;			
	
	if(XMLNode->Attribute("orient"))
	{
		ornt = Ogre::StringConverter::parseQuaternion(XMLNode->Attribute("orient"));
	}
	if(XMLNode->Attribute("scale"))
	{
		scale = Ogre::StringConverter::parseVector3(XMLNode->Attribute("scale"));
	}
	if(XMLNode->Attribute("omega"))
	{
		omega = Ogre::StringConverter::parseVector3(XMLNode->Attribute("omega"));
	}
	if(XMLNode->Attribute("SGID"))
	{
		SGID = Ogre::StringConverter::parseInt(XMLNode->Attribute("SGID"));
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

	/*StandardApplication *app = StandardApplication::getSingletonPtr();
	
	data.missile = app->parseMissileData(XMLNode->FirstChildElement("missileData"));
	if(XMLNode->FirstChildElement("damage"))
	{
		data.dmg = app->parseDamageData(XMLNode->FirstChildElement("damage"));
	}
	if(XMLNode->FirstChildElement("explosion"))
	{
		ExplosionData *d = new ExplosionData();
		*d = app->parseExplosionData(XMLNode->FirstChildElement("explosion"));
		data.expl.bind(d);
	}
	if(XMLNode->Attribute("spell"))
	{
		data.spell = app->getSpellDataPtr(XMLNode->Attribute("spell"));
	}*/
	ObjectPtr caster;
	if(XMLNode->Attribute("caster"))
	{
		int casterID = Ogre::StringConverter::parseInt(XMLNode->Attribute("caster"));
		caster = ObjectPtr(lvl,casterID);
	}
	
	init(caster,castitem,lvl,pos,ornt,Ogre::Vector3::ZERO,SGID);

	if(mSnd)
	{
		mSnd->setMsecOffset(lifetime);
	}

	if(XMLNode->Attribute("inlevelfile"))
	{
		isInLevelFile = Ogre::StringConverter::parseBool(XMLNode->Attribute("inlevelfile"));
	}
	mBody->setVelocity(velocity);
	mBody->setOmega(omega);
	
}

void MissileObject::init(ObjectPtr caster, gamedata_damagedealer *source, Level *lvl, Ogre::Vector3 pos, Ogre::Quaternion ornt, Ogre::Vector3 v0,int SaveGameID)
{
	if(!source->Missile)
	{
		//zomg! keine MissileData!
		mLog("Error: Could not construct Missile, because item/spell \""+source->id+"\" contains no MissileData");
		throw EX_CONSTR_FAILED;
	}
	isInLevelFile = false;
	SGID = SaveGameID;
	lifetime = 0;
	mNode = NULL;
	mArt = NULL;
	mLevel = lvl;
	mSnd = NULL;
	type = otMissile;
	mCaster = caster;
	mCastingItem = source;
	//mData = data;
	Ogre::SceneManager *mSceneMgr = mLevel->getSceneManager();
	/*Ogre::String mesh = mData.mesh;
	Ogre::String material = mData.material;*/

	hasCollided = false;
	ID = "missile";
	mNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();


	OgreNewt::World *mWorld = mLevel->getWorld();
//	Ogre::SceneManager *mSceneMgr = mLevel->getSceneManager();

	
	
	//Ogre::Vector3 inert=OgreNewt::MomentOfInertia::CalcEllipsoidSolid(mass,Ogre::Vector3(1,1,1));
	mColData = mCastingItem->Missile->cData;
	createBody(mColData);
	mBody->setMaterialGroupID(mLevel->projMaterial);
	//mBody = new OgreNewt::Body(mWorld,test);
	//mNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	//ent = mSceneMgr->createEntity(getUniqueEntityName(mSceneMgr),"ellipsoid.mesh");
	//mNode->attachObject(ent);
	mArt = new WorldArt(this,mCastingItem->Missile->wData);
	mNode = mArt->getSceneNode();
	//mBody->attachToNode(mNode);

	mBody->setLinearDamping(0);

	mBody->setPositionOrientation(pos,ornt);
		
	//mBody->setMassMatrix(mass,inert);
	Ogre::Vector3 vel = ornt*Ogre::Vector3(0,0,-mCastingItem->Missile->velocity) + v0;
	mBody->setVelocity(vel);

	//jetzt sound
	if(mCastingItem->Missile->sound != "")
	{
		mSnd = mLevel->createSource(NULL,stSfxLevel,true);
		if(mSnd)
		{
			mSnd->setBuffer(mLevel->getBuffer(mCastingItem->Missile->sound));
			mSnd->setPosition(pos);
			mSnd->setVelocity(v0);
			mSnd->play();
		}

	}
	mLog("Created Missile "+ptrAsStr());
	mLevel->registerObject(this);
}

MissileObject::~MissileObject()
{
	mLog("About to delete Missile "+ptrAsStr());
	//void *test = this;
	//long muhkuh = reinterpret_cast<long>(test);
	////test2 << test;
	//char buffer[33];
	//int base = 16;
	//ltoa(muhkuh,buffer,base);
	//mLevel->unregisterObject(this);
	//delete mBody;
	//if(mArt)
	//	delete mArt;

	if(mSnd)
	{
		mSnd->stop();
		mLevel->destroySource(mSnd);
		//mNode->detachObject(mSnd);
		//OgreAL::SoundManager::getSingletonPtr()->destroySound(mSnd);
	}

	////mSceneMgr->destroySceneNode(mNode->getName());
	//destroyNode(mLevel->getSceneManager(),mNode);
}

TiXmlElement* MissileObject::getXmlElem(Ogre::Vector3 startPosition,bool savegame)
{
	Ogre::String strtype = objectTypeToString(type);
	if(strtype == "")
		return NULL;
	TiXmlElement* xmlElem = new TiXmlElement(strtype.c_str());
	Ogre::Vector3 pos, scale;
	Ogre::Quaternion ornt;
		
	scale = getScale();
	getPositionOrientation(pos,ornt);
		
	pos -= startPosition;
	xmlElem->SetAttribute("pos",Ogre::StringConverter::toString(pos).c_str());
	if(ornt != Ogre::Quaternion::IDENTITY)
	{
		xmlElem->SetAttribute("orient",Ogre::StringConverter::toString(ornt).c_str());
	}
	if(SGID)
	{
		xmlElem->SetAttribute("SGID",Ogre::StringConverter::toString(SGID).c_str());
	}
	//nun das casting-teil
	if(mCastingItem->type == gtItem)
	{
		xmlElem->SetAttribute("castingItemType","item");
	}
	else
	{
		xmlElem->SetAttribute("castingItemType","spell");
	}
	xmlElem->SetAttribute("castingItem",mCastingItem->id);
	StandardApplication *app = StandardApplication::getSingletonPtr();
	/*TiXmlElement *missileElem = new TiXmlElement("missileData");
	app->missileDataToXml(missileElem,mData.missile);
	xmlElem->LinkEndChild(missileElem);

	TiXmlElement *dmgElem = new TiXmlElement("damage");
	app->damageDataToXml(dmgElem,mData.dmg);
	xmlElem->LinkEndChild(dmgElem);

	if(!mData.expl.isNull())
	{
		TiXmlElement *exElem = new TiXmlElement("explosion");
		app->explosionDataToXml(exElem,*mData.expl.getPointer());
		xmlElem->LinkEndChild(exElem);
	}*/
    if(!mCaster.isNull())
		xmlElem->SetAttribute("caster",str(mCaster->getSGID()));

	/*if(mData.spell)
		xmlElem->SetAttribute("spell",mData.spell->id);*/

	
	Ogre::Vector3 omega = mBody->getOmega();
	Ogre::Vector3 velocity = mBody->getVelocity();
	if(omega != Ogre::Vector3::ZERO)
	{
		xmlElem->SetAttribute("omega",Ogre::StringConverter::toString(omega).c_str());
	}	
	xmlElem->SetAttribute("velocity",Ogre::StringConverter::toString(velocity).c_str());

	xmlElem->SetAttribute("inlevelfile",Ogre::StringConverter::toString(isInLevelFile).c_str());
	

	return xmlElem;
	
}

void MissileObject::onCollision(GameObject *with,ContCallback *callback)
{
    
	if(hasCollided)
		return;
	hasCollided = true;
	//mLog("missile kollidiert");


	//MACHEN SO:
//	with->AddEffects(mData.effects,caster);
	//with->addEffect(effect);
    if(with)
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
            with->onDamage(item->rangedDamage,mCaster.obj);
		}
	    if(spell)
            with->AddSpell(spell,mCaster.obj);

	    
    }
	//jetzt einen effect erstellen. der effect kriegt nun nur die ExplosionData, keinen Pointer auf den Spell oder Item

	if(mCastingItem->Explosion)//eData.isEmpty())
	{
        Ogre::Vector3 pos;
        Ogre::Quaternion ornt;
        
        getPositionOrientation(pos,ornt);
		//mDebugText(str(pos));
        new ExplosionObject(mCaster.obj,with,mCastingItem,mLevel,pos,true,false,ornt);
	}

	/*if(mSnd)
		mSnd->stop();*/
	mLevel->deleteObject(this);
	
}

void MissileObject::update(Ogre::Real time)
{
	
	mLog("updating "+getDebugInfo());
	/*if(mSnd)
	{
		mSnd->setPosition(getPosition());
	}
	lifetime += time;
	if (lifetime >= MISSILE_LIFESPAN )
		mLevel->deleteObject(this);*/
}