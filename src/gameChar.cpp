/*
class global_class;
class objCont;*/
#include "FwDec.h"
#include "GameChar.h"


#include "defines.h"
#include "Level.h"

#include "functions.h"
#include "ExplosionObject.h"
#include "MissileObject.h"
#include "SoundManager.h"
#include "Listener.h"
#include "WaterPlane.h"
#include "contactCallback.h"
#include "BeamObject.h"

using namespace Ogre;
using namespace OgreNewt;





GameChar::CharAnimation GameChar::getRandomAnimState(AnimationList animList)
{
	if(animList.empty())
	{
		
		//Ogre::LogManager::getSingleton().logMessage("PRA: StrArray.size()==0");
		//mLog("leere liste");
		return CharAnimation();//"";
	}
	else if(animList.size() == 1)
	{
		//wozu random wenns bloß einer ist?
		return animList[0];
	}
	else
	{
		//Ogre::LogManager::getSingleton().logMessage("PRA: StrArray.size()>1");
		Ogre::Real rnd = Ogre::Math::UnitRandom();
		
		size_t cnt = animList.size();		
		int index = Ogre::Math::IFloor(rnd*cnt);
		//Ogre::LogManager::getSingleton().logMessage("PRA: StrArray["+Ogre::StringConverter::toString(index)+"]");
		return animList[index];
	}
}

bool GameChar::hasItem(gamedata_item *item)
{
    if(inv->getCount(item) > 0)
        return true;
    //nun schaun obs equippt ist
    for(int i=0;i<EQUIP_CNT;i++)
    {
        if(equipped[i] == item)
            return true;
    }
    return false;
}

void GameChar::collisionWithWater(WaterPlane *water)
{
    //sich um das add/removeColliding des Wassers kümmern
    if(mWater != water)
    {
        if(mWater)
            mWater->removeColliding(this);
        if(water)
            water->addColliding(this);
    }
    mWater = water;
    if(water)
        swimming = true;
    else
        swimming = false;

}
//GameChar::GameChar(Ogre::SceneManager* _mSceneMgr,OgreNewt::World* _mWorld, OgreNewt::MaterialID *myMaterial, GameApp *_app, Ogre::Vector3 pos,Ogre::Quaternion orient)
GameChar::GameChar(GameChar *src, Level *_lvl)
{
	curMeleeObj = NULL;
	missileToSpawn = NULL;
	missileToSpawnRepeat = 0;
	currentAnim.anim = NULL;
	mNode = NULL;
	mArt = NULL;
	spellcasting = false;
	cooldown = 0;
	playJumpAnim = false;
	dead = false;
//	spellCooldown = 0;
	Level *lvl;
	if(_lvl)
	{
		lvl = _lvl;
	}
	else
	{
		lvl = src->mLevel;
	}

	Ogre::Quaternion pOrnt;
	Ogre::Vector3 pPos;
	src->getPositionOrientation(pPos,pOrnt);
	init(lvl,static_cast<gamedata_char*>(src->mData),pPos,pOrnt);
	

	/*playerControlled = false;
	mAI = 0;*/
	charName = src->charName;
	mSize = src->mSize;	
	
	mData = src->mData;

	mMass = src->mMass;

	/*setJumpHeight(1.75);
	jumping = false;
	jumpTime = 0;
	jumpLandedTime = 0;
	jump = false;
	jumpOnce = false;
	jumpStartFinished = false;*/


	/*y_rotation_cont = 0;
	y_limit_a = 90;
	y_limit_b = -90;*/
	
	/*onEarth = false;
	attacking = false;
	action = false;
	run = false;
	type = otChar;

	mvUp = false;
	mvDown = false;
	mvRight = false;
	mvLeft = false;*/
	baseScale=src->baseScale;
	/*ent = NULL;
	inv = NULL;
	mSb = NULL;*/

//	maxSpeed = src->maxSpeed;
	maxAccel = src->maxAccel;
	noMovement = true;
	animType = AT_NONE;
	


	

	/*Ogre::SceneManager *mSceneMgr=mLevel->getSceneManager();
	OgreNewt::World *mWorld=mLevel->getWorld();

	mNode = cloneSceneNode(mSceneMgr,src->mNode,0,Any((GameObject*)this));

	camNode = (Ogre::SceneNode*)mNode->getChild(0);*/

	
	
	//meshNode = (Ogre::SceneNode*)mNode->getChild(1);
	//uName = meshNode->getName(); <== FALSCH, ich werde jetzt die most parent node holen, und da nach name...
	//entity stuff



	/*if(meshNode->getAttachedObject(0))
	{
		ent = (Ogre::Entity*)meshNode->getAttachedObject(0);
		fillAnimLists();
		setRandomAnim(AT_IDLE);
	}
		*/
	
	
	
	//Ogre::Vector3 bodyInert;
	//mColData = src->mColData;
	//const Collision* bodyCol = createCollision(mColData,bodyInert);//src->mBody->getCollision();
	//
	//mBody= new OgreNewt::Body( mWorld, bodyCol );  // create the body
	//delete bodyCol; // now the collision can be deleted

	//mBody->attachToNode(mNode); //

	//src->mBody->getMassMatrix(mMass, bodyInert);//OgreNewt::MomentOfInertia::CalcEllipsoidSolid(mMass,mSize); // calculate the inertia ov the body
	//mBody->setMassMatrix( mMass, bodyInert);

	//mBody->setCustomForceAndTorqueCallback<GameChar>(&GameChar::forceCallback,this);  // add the previous defined callback function as the body custom force and torque callback
	//OgreNewt::BasicJoints::UpVector* uv2 = new OgreNewt::BasicJoints::UpVector(mWorld,mBody,Ogre::Vector3::UNIT_Y); // create an upvector. This forces the body to rotate just in the X and Z axes.
	//Ogre::Vector3 pos;
	//Ogre::Quaternion orient;
	//src->mBody->getPositionOrientation(pos,orient);
	//mBody->setPositionOrientation(pos,orient);
	//
	//mBody->setAutoFreeze(0);
	//mBody->setUserData(this);
	//mBody->setType(otChar);
	//mBody->setMaterialGroupID(mLevel->charMaterial);

	//
	// 
	//
	//




	ID = src->ID;
//#ifndef __editor
//	//KI
//	if(ID != "player")
//	{
//		mAI = new AI(this);
//		//später: soll in den data stehen
//	}
//
//	
//#endif
	//mLevel->registerObject(this);

	/*
	stats: später aus xml lesen
	
	*/
	mStats = src->mStats;
	for(int i = 0;i<EQUIP_CNT;i++)
	{
		equipped[i] = src->equipped[i];
	}
	//eqBag = src->eqBag;
	eqSpell = src->eqSpell;
	


	if(src->inv)
		inv = new Inventory(src->inv);
	else
		inv = NULL;
	
	mSb = new Spellbook(src->mSb);

	/*inventory copyconstructor adden*/


}
//GameChar::GameChar(Level *lvl,Ogre::String gcId, Ogre::Vector3 pos,Ogre::Quaternion orient)
//{
//	
//	//prüfe, ob die angegebene ID stimmt
//	gamedata_char *myData = StandardApplication::getSingletonPtr()->getCharDataPtr(gcId);//app->getGameCharDataPtr(gcId);
//	//wenn nicht, ...
//	if(!myData)//->id == "")
//	{
//		//hier exception throwen
//		throw etInvalidId;
//		//return;
//	}
//	init(lvl,myData,pos,orient);
//}
void GameChar::init(Level *lvl,gamedata_char *myData,Ogre::Vector3 pos,Ogre::Quaternion orient,int SaveGameID)
{
	floorColliding = false;
	isInLevelFile = false;
	SGID = SaveGameID;
	curMeleeObj = NULL;
	doOnce = true;
	resetLag = 5; //erstmal ne sekunde
	startingPos = pos;
	startingOrnt = orient;

	mData = myData;
	missileToSpawn = NULL;
	missileToSpawnRepeat = 0;
	currentAnim.anim = NULL;
	mNode = NULL;
	mArt = NULL;
	spellcasting = false;
	playJumpAnim = false;
	jumpLanded = true;
	dead = false;
	cooldown = 0;
	inv = NULL;
	mSb = NULL;

	walkSoundBuffer = NULL;
	jumpSoundBuffer = NULL;
	landSoundBuffer = NULL;
	moveSoundSource = NULL;	
	moveSoundPassed = myData->stepSoundWaitWalk;
	attackSoundSource = NULL;
	equipped[eqBag] = NULL;
	for(int i=0;i<EQUIP_CNT;i++)
	{
		equipped[i] = NULL;	
	}
	
	
	eqSpell = NULL;
//	spellCooldown = 0;
	
//	GameApp *app = dynamic_cast<GameApp*>(StandardApplication::getSingletonPtr());//_app;	
	
	mLevel = lvl;
	ID = myData->id;
	playerControlled = false;


	//KI
	mAI = 0;


	
	charName = myData->name;

	//init stuff

	mSize = myData->cData.collistionParams;//(myData->size)/2;//Ogre::Vector3(0.5,1.5,0.5); //myData.mSize;//#
	
//	////app->log(Ogre::StringConverter::toString(myData->mSize));
	
	charName = myData->name;//"muhkuh"; //fürs erste
	//uName = myData->id;
	mMass = myData->cData.mass;
	//jumpPower = 75;
	setJumpHeight(1.75);
	jumping = false;
	jumpTime = 0;
	jumpLandedTime = 0;
	jump = false;
	jumpOnce = false;
	jumpStartFinished = true;


	y_rotation_cont = 0;
	y_limit_a = 90;
	y_limit_b = -90;
	
	onEarth = false;
	attacking = false;
	action = false;
	run = false;
	type = otChar;
//	mTorque = 100;	//damit wird die geschw. erhöht
//	movement=Ogre::Vector3::ZERO;
	mvUp = false;
	mvDown = false;
	mvRight = false;
	mvLeft = false;
//	baseScale=myData->mesh_scale;
	ent = NULL;


	//maxSpeed = 5;
	maxAccel = myData->movAccel;//120;
	noMovement = true;
	animType = AT_NONE;
	

	//creating the nodes
	//main node
	int i = 0;



	Ogre::SceneManager *mSceneMgr=lvl->getSceneManager();
	OgreNewt::World *mWorld=lvl->getWorld();


	baseScale = myData->wData.scale;
	mNode = mLevel->getSceneManager()->getRootSceneNode()->createChildSceneNode();
	meshNode = mNode->createChildSceneNode();
	try
	{
		mArt = new WorldArt(this,myData->wData,meshNode);
	}
	catch(...)
	{
		//dann halt nicht
		mArt = NULL;
	}
	//	createWorldArt(myData->wData,meshNode);
	//cam node
	camNode = mNode->createChildSceneNode(myData->head_position);
	currentAnim.anim = NULL;
	if(mArt)
	{
		fillAnimLists();
		setRandomAnim(AT_IDLE);
	}


	mColData = myData->cData;
	createBody(myData->cData);
	//mBody->setCustomForceAndTorqueCallback<GameChar>(&GameChar::forceCallback,this);  
	mBody->setCustomForceAndTorqueCallback( boost::bind( &GameChar::forceCallback, this, _1 ) );
	mBody->setPositionOrientation(pos,orient);
	mBody->setMaterialGroupID(mLevel->charMaterial);
	mBody->setAutoSleep(0);// setAutoFreeze(0);
	
	



	if(ID == "player")
	{
		//SGID = -1;
	}
	else
	{
#ifndef __editor
	//KI	
		mAI = new AI(this);
		//später: soll in den data stehen
#endif
	}
	mLevel->registerObject(this);


	baseStats = myData->stats;
	addStats = multStats = mStats = baseStats;


	if(myData->defaultSpellbook)
		mSb = new Spellbook(myData->defaultSpellbook);
	else
		mSb = new Spellbook();	
	if(myData->defaultBag && myData->defaultInventory)
	{
		inv = new Inventory(myData->defaultInventory);
        inv->setMaxSize(myData->defaultBag->Container->defaultSize);
		equipped[eqBag] = myData->defaultBag;
	}
	equipped[eqArmor] = myData->defaultArmor;
	equipped[eqElse1] = myData->defaultElse1;
	equipped[eqElse2] = myData->defaultElse2;
	equipped[eqElse3] = myData->defaultElse3;
	equipped[eqElse4] = myData->defaultElse4;
	equipped[eqMelee] = myData->defaultMelee;
	equipped[eqRanged] = myData->defaultRanged;
	equipped[eqShield] = myData->defaultShield;

	updateStats();
#ifndef __editor
	//sound
	SimpleSound::SoundManager *soundMgr = SimpleSound::SoundManager::getSingletonPtr();
	walkSoundBuffer = mLevel->getBuffer(myData->stepSound);
	
	jumpSoundBuffer = mLevel->getBuffer(myData->jumpSound);
	
	landSoundBuffer = mLevel->getBuffer(myData->landSound);
	

	moveSoundSource = mLevel->createSource(walkSoundBuffer,stSfxLevel,false,false);
	
	moveSoundSource->setPosition(pos);
	//myData->jumpSound
	//jumpLandSound= StandardApplication::getSingletonPtr()->createSound(myData->j,stSfxLevel,false,false);
	//	moveSound->play();
//	moveSound->pause();
//	mNode->attachObject(moveSound);
#endif

}

GameChar::GameChar(TiXmlElement *XMLNode,Level *lvl,Ogre::Vector3 positionOffset,bool savegame)
{
	
	Ogre::String id = "";
	if(XMLNode->Attribute("id"))
	{
		id = XMLNode->Attribute("id");
	}
	gamedata_char *myCharData = StandardApplication::getSingletonPtr()->getCharDataPtr(id);
	if(!myCharData)
		throw etInvalidId;
	Ogre::Vector3 pos = Ogre::StringConverter::parseVector3(XMLNode->Attribute("pos"))+positionOffset;
	Ogre::Vector3 scale = Ogre::Vector3::UNIT_SCALE;
	Ogre::Quaternion ornt = Ogre::Quaternion::IDENTITY;
	SGID = 0;
	GameObject *resultObject = NULL;
	/*Ogre::String filename = XMLNode->Attribute("filename");
	Ogre::String material = "";*/
	bool hasCollision = true;			
	
	if(XMLNode->Attribute("orient"))
	{
		ornt = Ogre::StringConverter::parseQuaternion(XMLNode->Attribute("orient"));
	}
	
	
	
	
	if(XMLNode->Attribute("SGID"))
	{
		SGID = Ogre::StringConverter::parseInt(XMLNode->Attribute("SGID"));
	}
	init(lvl,myCharData,pos,ornt,SGID);
	
	
	if(XMLNode->Attribute("inlevelfile"))
	{
		isInLevelFile = Ogre::StringConverter::parseBool(XMLNode->Attribute("inlevelfile"));
	}
	
		
	gamedata_item *melee = myCharData->defaultMelee;
	gamedata_item *ranged = myCharData->defaultRanged;
	gamedata_item *armor = myCharData->defaultArmor;
	gamedata_item *shield = myCharData->defaultShield;
	gamedata_item *else1 = myCharData->defaultElse1;
	gamedata_item *else2 = myCharData->defaultElse2;
	gamedata_item *else3 = myCharData->defaultElse3;
	gamedata_item *else4 = myCharData->defaultElse4;	
	gamedata_item *bag=myCharData->defaultBag;
	gamedata_spell *spell=NULL;
	StandardApplication *app = StandardApplication::getSingletonPtr();
	TiXmlElement *eq = XMLNode->FirstChildElement("equipment");
	if(eq)
	{								
		if(eq->Attribute("melee"))
		{
			melee = app->getItemDataPtr(eq->Attribute("melee"));
		}				
		if(eq->Attribute("ranged"))
		{
			ranged = app->getItemDataPtr(eq->Attribute("ranged"));
		}				
		if(eq->Attribute("armor"))
		{
			armor = app->getItemDataPtr(eq->Attribute("armor"));
		}				
		if(eq->Attribute("shield"))
		{
			shield = app->getItemDataPtr(eq->Attribute("shield"));
		}				
		if(eq->Attribute("else1"))
		{
			else1 = app->getItemDataPtr(eq->Attribute("else1"));
		}				
		if(eq->Attribute("else2"))
		{
			else2 = app->getItemDataPtr(eq->Attribute("else2"));
		}				
		if(eq->Attribute("else3"))
		{
			else3 = app->getItemDataPtr(eq->Attribute("else3"));
		}				
		if(eq->Attribute("else4"))
		{
			else4 = app->getItemDataPtr(eq->Attribute("else4"));
		}
		if(eq->Attribute("spell"))
		{
			spell = app->getSpellDataPtr(eq->Attribute("spell"));
		}	
		if(eq->Attribute("bag"))
		{
            bag = app->getItemDataPtr(Ogre::String("container_")+eq->Attribute("bag"));
		}

	}
	equipped[eqMelee] = melee;
	equipped[eqArmor] = armor;
	equipped[eqRanged] = ranged;
	equipped[eqShield] = shield;
	equipped[eqElse1] = else1;
	equipped[eqElse2] = else2;
	equipped[eqElse3] = else3;
	equipped[eqElse4] = else4;
	equipped[eqBag] = NULL;
	eqSpell = spell;
	updateStats();
	
	
	if(XMLNode->FirstChildElement("inventory"))
	{
		if(setBag(bag) && bag)
		{
			Inventory *inv = getInventory();
			app->parseInventory(XMLNode->FirstChildElement("inventory"),inv);
		}
	}
	else
	{
		Inventory *inv = new Inventory(myCharData->defaultInventory);
		if(!setBag(bag,inv))
		{
			delete inv;
		}
	}

	if(XMLNode->FirstChildElement("spellbook"))
	{
		if(mSb)
			delete mSb;
		mSb = new Spellbook();
		app->parseSpellbook(XMLNode->FirstChildElement("spellbook"),mSb);		
	}

	if(XMLNode->FirstChildElement("basestats"))
	{
		baseStats = app->parseStats(XMLNode->FirstChildElement("basestats")->FirstChildElement());
		updateStats();
	}

	if(XMLNode->FirstChildElement( "activespells" ))
	{
		TiXmlElement *elem = XMLNode->FirstChildElement( "activespells" )->FirstChildElement();
		while(elem)
		{
			Ogre::String id = elem->Attribute("id");
			gamedata_spell *spell = app->getSpellDataPtr(id);
			if(!spell)
				continue;
			GameObject *caster = NULL;
			GameObject *target = NULL;
			int caster_id = 0;
			int target_id = 0;
			if(elem->Attribute("caster"))
			{
				caster_id = Ogre::StringConverter::parseInt(elem->Attribute("caster"));
				if(caster_id == SGID)
					caster = this;
				else
					caster = mLevel->getObjectBySGID(caster_id);
			}
			if(elem->Attribute("target"))
			{
				target_id = Ogre::StringConverter::parseInt(elem->Attribute("target"));
				if(target_id == SGID)
					target = this;
				else
					target = mLevel->getObjectBySGID(target_id);
			}
			int lifetime = Ogre::StringConverter::parseReal(elem->Attribute("lifetime"));
			SpellObject *cur = NULL;
			try
			{
				cur = new SpellObject(spell,caster,target,savegame,lifetime);
			}
			catch(ExceptionType)
			{
				cur = NULL;
			}
			if(cur)
			{
				if(!caster && caster_id!=0)
					cur->caster_sgid = caster_id;
				if(!target && target_id!=0)
					cur->target_sgid = target_id;
				cur->lvl = mLevel;
				activeSpells.push_back(cur);
			}
			elem = elem->NextSiblingElement();
		}
	}
	

	if(mBody)
	{
		Ogre::Vector3 omega = Ogre::Vector3::ZERO;
		Ogre::Vector3 velocity = Ogre::Vector3::ZERO;
		if(XMLNode->Attribute("omega"))
		{
			omega = Ogre::StringConverter::parseVector3(XMLNode->Attribute("omega"));
		}
		if(XMLNode->Attribute("velocity"))
		{
			velocity = Ogre::StringConverter::parseVector3(XMLNode->Attribute("velocity"));
		}
		mBody->setVelocity(velocity);
		mBody->setOmega(omega);
	}
	
}

TiXmlElement* GameChar::getXmlElem(Ogre::Vector3 startPosition,bool savegame)
{
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
	if(SGID)
	{
		xmlElem->SetAttribute("SGID",Ogre::StringConverter::toString(SGID).c_str());
	}
	
	xmlElem->SetAttribute("id",ID.c_str());
	
	TiXmlElement *eqnode = new TiXmlElement( "equipment" );
	eqnode->SetAttribute("melee",getMeleeId().c_str());
	eqnode->SetAttribute("armor",getArmorId().c_str());
	eqnode->SetAttribute("ranged",getRangedId().c_str());
	eqnode->SetAttribute("shield",getShieldId().c_str());
	eqnode->SetAttribute("else1",getElse1Id().c_str());
	eqnode->SetAttribute("else2",getElse2Id().c_str());
	eqnode->SetAttribute("else3",getElse3Id().c_str());
	eqnode->SetAttribute("else4",getElse4Id().c_str());
	eqnode->SetAttribute("spell",getSpellId().c_str());
	eqnode->SetAttribute("bag",getBagId().c_str());
	xmlElem->LinkEndChild(eqnode);
	StandardApplication *app = StandardApplication::getSingletonPtr();
	if(inv)
	{
		TiXmlElement *invnode = new TiXmlElement( "inventory" );
		app->inventoryToXml(invnode,getInventory());
		xmlElem->LinkEndChild(invnode);
	}
	if(mSb)
	{
		TiXmlElement *node = new TiXmlElement( "spellbook" );
		app->spellbookToXml(node,mSb);
		xmlElem->LinkEndChild(node);
	}
	TiXmlElement *statnode = new TiXmlElement( "basestats" );
	app->statsToXml(statnode,baseStats);
	xmlElem->LinkEndChild(statnode);

	TiXmlElement *effectnode = new TiXmlElement( "activespells" );
	app->spellListToXml(effectnode,activeSpells);
	xmlElem->LinkEndChild(effectnode);
	/*for(SpellList::iterator itr=activeSpells.begin();itr!=activeSpells.end();itr++)
	{
		SpellObject* cur = *itr;
		cur->mEffects
	}*/

	/*todo: effekte speichern*/
	xmlElem->SetAttribute("inlevelfile",Ogre::StringConverter::toString(isInLevelFile).c_str());
	if(savegame && mBody && !mBody->isFreezed())//mBody->getSleepingState())
	{
		Ogre::Vector3 omega = mBody->getOmega();
		Ogre::Vector3 velocity = mBody->getVelocity();
		if(omega != Ogre::Vector3::ZERO)
		{
			xmlElem->SetAttribute("omega",Ogre::StringConverter::toString(omega).c_str());
		}
		if(velocity != Ogre::Vector3::ZERO)
		{
			xmlElem->SetAttribute("velocity",Ogre::StringConverter::toString(velocity).c_str());
		}
	}


	return xmlElem;
}



GameChar::~GameChar()
{
#ifndef __editor
	if(mAI)
		delete mAI;
#endif
	if(inv)
		delete inv;
	//mLevel->unregisterObject(this);

	if(moveSoundSource)
	{
		mLevel->destroySource(moveSoundSource);
	}
	if(attackSoundSource)
	{
		mLevel->destroySource(attackSoundSource);
	}
	//if(moveSound)
	//	StandardApplication::getSingletonPtr()->destroySound(moveSound);
	//if(attackSound)
	//	StandardApplication::getSingletonPtr()->destroySound(attackSound);

	//Ogre::SceneNode *tireNode = getFirstTire()->getOgreNode();
	//Ogre::LogManager::getSingletonPtr()->logMessage("vor destroy");
	//destroy();
	//delete mBody;
	////Ogre::SceneNode *tireNode = mTire->getOgreNode();
	////delete mTire;
	//if(mArt)
	//	delete mArt;
	//mNode->removeAndDestroyAllChildren();
	//mLevel->getSceneManager()->destroySceneNode(mNode->getName());


}

bool GameChar::isOnWaterSurface()
{
    if(!mWater)
    {
        
        return false;
    }
    if(Ogre::Math::Abs(mWater->getPlane().getDistance(getPosition())) > mSize.y/2)
    {
        //onWaterSurface = false;
        return false;
    }
    //onWaterSurface = true;
    return true;
}

void GameChar::onCollision(GameObject *with,ContCallback *cb)
{
	OgreNewt::ContactJoint *contacts = cb->getContactJoint();
	Ogre::Vector3 cPos, cNorm;
	for(Contact contact = contacts->getFirstContact(); contact; contact = contact.getNext())
	{
		contact.getPositionAndNormal(cPos,cNorm);
		cPos -= getPosition();
		if(cPos.y < -(3*mSize.y/4) )//wenn der kontakt im unteren viertel des ellipsoids stattfindet
		{
			//schauen ob es nicht der falsche objekttyp ist
			if(with)
			{
				if(with->getType() != otMissile && with->getType() != otWater)
				{
					floorColliding = true;
				}			
			}
			else
				floorColliding = true;
		}
		if(!isMoving())
		{
			contact.setFrictionCoef(1,0,0);
			contact.setFrictionCoef(1,1,0);
			contact.setFrictionState(0,0);
			contact.setFrictionState(1,0);

			/*cb->setContactStaticFrictionCoef(1,0);
			cb->setContactStaticFrictionCoef(1,1);
			cb->setContactKineticFrictionCoef(0.1,0);
			cb->setContactKineticFrictionCoef(0.1,1);
			cb->setContactFrictionState(1,0);
			cb->setContactFrictionState(1,1);*/
	//		mDebugText("setze reibung");
		}
	}

	
	//cb->getContactPositionAndNormal(cPos,cNorm);
	
	
	//floorColliding wird am ende von forceCallback() auf false gesetzt
	//mDebugText(str(cPos.y));
	
	//mLog("onCollision");
}

void GameChar::forceCallback(OgreNewt::Body* me)
{
#ifndef __editor
	GameApp *app = (GameApp*)StandardApplication::getSingletonPtr();
	if(app->getPaused() || app->getZeroTime())
		return;
#endif
	Ogre::Real gravity = mLevel->getGravity();
	OgreNewt::World *mWorld = mLevel->getWorld();

	Ogre::Vector3 gravAccel(0,gravity,0);
	Ogre::Vector3 movAccel = Ogre::Vector3::ZERO;
	Ogre::Vector3 movDirAbs(0,0,0);
	////for test
	//Ogre::Vector3 pos;
	//Ogre::Quaternion ornt;
	//mBody->getPositionOrientation(pos,ornt);
	//StandardApplication::getSingletonPtr()->setDebugText("pos.y="+str(pos.y));


	
#ifndef __editor

	//mDebugText("reset");
    if(isInWater())
    {
		//mDebugText("im wasser");
        gravAccel=Ogre::Vector3::ZERO;
        
        movDirAbs = getMovementVector(true);

        currentVel = mBody->getVelocity();
	    

	    //StandardApplication::getSingletonPtr()->setDebugText("movDirAbs\nx="+Ogre::StringConverter::toString(movDirAbs.x)+
	    //													"\ny="+Ogre::StringConverter::toString(movDirAbs.y)+
	    //													"\nz="+Ogre::StringConverter::toString(movDirAbs.z));
	    // ****************************** /
	    //   A = (V1 - V2) / DT 
	    // ****************************** /

			
	    Ogre::Real curMaxSpeed = mStats.mStats[st_walkSpeed];//maxSpeed;
	    if(run)
		    curMaxSpeed = mStats.mStats[st_runSpeed];
	    //movAccel = (-currentVel + movDirAbs*curMaxSpeed) / mWorld->getTimeStep();
		movAccel = (-currentVel + movDirAbs*curMaxSpeed) * mWorld->getUpdateFPS(); 

		
		

	    if(movAccel.squaredLength() > maxAccel*maxAccel)
	    {
		    movAccel.normalise();
		    movAccel *= maxAccel;
	    }

		//mDebugText("movAccel="+str(movAccel));

        //nachdem alles durch ist: 
        //onWaterSurface = false;
		 mBody->addForce((movAccel) * mMass);
    }
    else
    {
	    //if(onEarth)
	    //{
			//mDebugText("auf der erde");
		    //keinen Sinn, bewegung zu berechnen, wenn ich über dem boden bin

    		
		    currentVel = mBody->getVelocity();
		    //currentSpeed = currentVel.length();
		    //now: the desired velocity should have the length of maxSpeed and the direction given by the 4 booleans
		    //perhaps i should use Ogre::Vector3 movement; again

		    movDirAbs = getMovementVector(false);

		    //StandardApplication::getSingletonPtr()->setDebugText("movDirAbs\nx="+Ogre::StringConverter::toString(movDirAbs.x)+
		    //													"\ny="+Ogre::StringConverter::toString(movDirAbs.y)+
		    //													"\nz="+Ogre::StringConverter::toString(movDirAbs.z));
		    

    			
		    Ogre::Real curMaxSpeed = mStats.mStats[st_walkSpeed];//maxSpeed;
		    if(run)
			    curMaxSpeed = mStats.mStats[st_runSpeed];
			//if( !(movDirAbs == Ogre::Vector3::ZERO && !onEarth) )//wenn in der luft und keine bewegungsbefehle, dann einfach weiterfliegen
			//{
				//movAccel = (-currentVel + movDirAbs*curMaxSpeed) / mWorld->getTimeStep();
				movAccel = (-currentVel + movDirAbs*curMaxSpeed) * mWorld->getUpdateFPS(); 
				if(!onEarth)
					movAccel.y = 0;//sonst wird die Fallgeschwindigkeit mitberücksichtigt

	    		


				if(movAccel.squaredLength() > maxAccel*maxAccel)
				{
					movAccel.normalise();
					movAccel *= maxAccel;
				}
			//}

    		
	    //}//if(onEarth)

	    //JUMPING BEGIN
    	
	    if(jumping && !jump)
	    {
		    jumpStartFinished = true;
	    }

	    if(jump && !jumping && onEarth)//beginn des Sprungvorganges
	    {
    		
		    //Ogre::LogManager::getSingleton().logMessage("jump accel added");
		    //sprung vorbereiten
		    jumpLanded = false;
		    jumping  = true;
		    jumpTime = 0;
		    jumpLandedTime = 0;
		    jumpStartFinished = false;

		    moveSoundSource->stop();
		    if(jumpSoundBuffer)
		    {
			    moveSoundSource->setBuffer(jumpSoundBuffer);
			    moveSoundSource->play();
		    }
		    //Absprung-Sound vorbereiten
    //		StandardApplication::getSingletonPtr()->destroySound(jumpLandSound);
    //		jumpLandSound = StandardApplication::getSingletonPtr()->createSound(static_cast<gamedata_char*>(mData)->jumpSound,stSfxLevel);
    //		if(jumpLandSound)
    //		{
    //			mNode->attachObject(jumpLandSound);
    //			jumpLandSound->play();
    //		}

    		

	    }
    	
		Ogre::Real timeStep = 1.0f / mWorld->getUpdateFPS(); //mWorld->getTimeStep();
	    if(jumping)//mitten im sprung
	    {
    		
		    jumpTime += timeStep;
    		
		    if(jumpTime <= MAX_JUMPTIME)
		    {
			    if(!jumpStartFinished)
			    {
				    playJumpAnim = true;
    				
				    Ogre::Real curJumpPower = jumpPower;// * timeStep / MAX_JUMPTIME;
    				
    				
				    //if(movDirAbs != Ogre::Vector3::ZERO)
				    //{
					   // Ogre::Vector3 jumpDir = Ogre::Vector3::UNIT_Y + movDirAbs;
    				//	
					   // jumpDir.normalise();
					   // //mDebugText(str(jumpDir.x)+"\n"+str(jumpDir.y)+"\n"+str(jumpDir.z));
					   // movAccel = (jumpDir*curJumpPower);
    				//	
				    //}
				    //else
				    //{
					    movAccel.y = curJumpPower;
						//mDebugText("adde jumppower="+str(curJumpPower));
				    //}
    				
			    }
		    }
		    else
		    {
			    if(!onEarth)
				    jumpStartFinished = true;
    			
		    }

		    //if(Ogre::Math::Abs(currentVel.y) <= MIN_JUMP_VEL && jumpStartFinished)
		    if(onEarth && jumpStartFinished && jumping)//dh ich bin gelandet, aber zeit noch nicht vorbei  //&& jumpTime >= 4*MAX_JUMPTIME)
		    {
			    if(!jumpLanded)
			    {
				    //landesnd
				    //StandardApplication::getSingletonPtr()->destroySound(jumpLandSound);
				    //jumpLandSound = StandardApplication::getSingletonPtr()->createSound(static_cast<gamedata_char*>(mData)->landSound,stSfxLevel);
				    //if(jumpLandSound)
				    //{
				    //	mNode->attachObject(jumpLandSound);
				    //	jumpLandSound->play();
				    //}
    				
				    moveSoundSource->stop();
				    if(landSoundBuffer)
				    {
					    moveSoundSource->setBuffer(landSoundBuffer);
					    moveSoundSource->play();
				    }
				    jumpLanded = true;
			    }
			    jumpLandedTime += timeStep;
    			
			    //StandardApplication::getSingletonPtr()->setDebugText("jumpLandedTime="+str(jumpLandedTime));
			    //jumping = false;
			    playJumpAnim = false;
			    if(jumpLandedTime >= MIN_JUMPPAUSE)
			    {
				    //alles resetten
				    jumping = false;
				    //jumpTime = 0;
				    if(jumpOnce)
				    {
					    jumpOnce = false;
					    jump = false;
				    }
			    }
		    }
	    }


    	

    	

    

    	
	//	mDebugText("adde "+str(movAccel+gravAccel));
	    mBody->addForce((movAccel+gravAccel) * mMass);
    	

    }//if(isInWater()){..}else
#else
    mBody->addForce((movAccel+gravAccel) * mMass);
#endif		
    


	mBody->setOmega(Ogre::Vector3(0,rotation_x*2,0));  //rotieren
	rotation_x = 0;

	if(resetLag >= 0)
	{
		setPositionOrientation(startingPos,startingOrnt);
		mBody->setVelocity(Ogre::Vector3::ZERO);
		mBody->setOmega(Ogre::Vector3::ZERO);
		resetLag-=1;
	}


//    onWaterSurface = false;
	onWaterSurfaceState = 0;
	floorColliding = false; //reset, es wird ggf. im onCollision wieder gesetzt
	
}

Ogre::Vector3 GameChar::getMovementVector(bool inWater)
{
    Ogre::Vector3 movDir = Ogre::Vector3::ZERO; //Movement Direction, relative to char's orientation
    if(mvUp)
	    movDir.z -= 1;
    if(mvDown)
	    movDir.z += 1;
    if(mvLeft)
	    movDir.x -= 1;
    if(mvRight)
	    movDir.x += 1;
    if(movDir == Ogre::Vector3::ZERO)
        return Ogre::Vector3::ZERO;

    if(movDir.squaredLength() != 1)
        movDir.normalise();
    //StandardApplication::getSingletonPtr()->setDebugText(str(movDir,true)+"\n");
    Ogre::Vector3 movDirAbs;
    Ogre::Quaternion currentOrient;
    if(inWater)
    {
		currentOrient = camNode->_getDerivedOrientation();
        movDirAbs = currentOrient * movDir;
    }
    else
    {
        currentOrient = getOrientation();
        movDirAbs = currentOrient * movDir;    
        movDirAbs.y = 0;
    }
    return movDirAbs;
    

}

//Ogre::Quaternion GameChar::get_body_orientation()
//{
//   
//   Ogre::Quaternion orient;
//   Ogre::Vector3 pos;
//
//   m_chassis->getPositionOrientation(pos, orient);
//
//   return orient;
//
//}

//Ogre::Vector3 GameChar::get_body_position()
//{
//   
//   Ogre::Quaternion orient;
//   Ogre::Vector3 pos;
//
//   m_chassis->getPositionOrientation(pos, orient);
//
//   return pos;
//   
//}
Ogre::Ray GameChar::getLookingRay()
{
	
	if(playerControlled)
	{
		//if(isMouseOverGui())
		Ogre::Real mouseX = 0.5;
		Ogre::Real mouseY = 0.5;
		/*CEGUI-stuff:*/
#ifndef __editor
		StandardApplication *app = StandardApplication::getSingletonPtr();
		GameApp *ga = (GameApp*)app;
		if(ga->getCursorMode())
		{
			CEGUI::Point pt = CEGUI::MouseCursor::getSingletonPtr()->getPosition();
			mouseX = pt.d_x/app->getRenderWindow()->getWidth();
			mouseY = pt.d_y/app->getRenderWindow()->getHeight();
//			mDebugText("x="+str(mouseX)+" y="+str(mouseY));
		}
#endif
		Ogre::Ray mRay = mLevel->getMainCam()->getCameraToViewportRay(mouseX,mouseY);	
		return mRay;
		//rayStart = mRay.getOrigin();
		//rayEnd   = mRay.getPoint(maxRange);
	
	}
	else
	{
		Ogre::Vector3 pos = camNode->_getDerivedPosition();
		Ogre::Quaternion ornt = camNode->_getDerivedOrientation();
		
		//getPositionOrientation(pos,ornt);
		Ogre::Ray mRay(pos,ornt*Ogre::Vector3::NEGATIVE_UNIT_Z);
		return mRay;
		//rayStart = mRay.getOrigin();
		//rayEnd   = mRay.getPoint(maxRange);
	}
	

}
GameChar::LookAtResult GameChar::getLookingAt(Ogre::Real maxRange, Ogre::Real minRange)
{
	LookAtResult res;
	res.obj = NULL;
	res.hitPoint = Ogre::Vector3::ZERO;
	res.objDist = maxRange;
	
	Ogre::Vector3 rayStart;
	Ogre::Vector3 rayEnd;
	
	Ogre::Ray mRay = getLookingRay();
	rayStart = mRay.getOrigin();
	rayEnd   = mRay.getPoint(maxRange);
	
	//Ogre::Ray mRay = mLevel->getMainCam()->getCameraToViewportRay(0.5,0.5);		
	
	OgreNewt::BasicRaycast lookingAt(mLevel->getWorld(),rayStart,rayEnd,true);
	OgreNewt::Body *bod = NULL;//lookingAt.getFirstHit().mBody;
	if(lookingAt.getHitCount() > 0)
	{
		
		//res.objDist = lookingAt.getFirstHit().mDistance*maxRange;
		//res.hitPoint = mRay.getPoint(res.objDist);
		
		//kümmert sich darum, wenn der ray das eigene body trifft
		//if(bod == mBody)
		//{
			//bod = NULL;
			for(int i = 0;i<lookingAt.getHitCount();i++)
			{
				OgreNewt::BasicRaycast::BasicRaycastInfo info = lookingAt.getInfoAt(i);
				Ogre::Real dist = info.mDistance*maxRange;
				if(info.mBody != mBody && dist > minRange && dist <= maxRange)
				{
					
					if(res.objDist > dist)
					{
						bod = info.mBody;
						res.objDist = dist;//lookingAt.getInfoAt(i).mDistance*maxRange;
						res.hitPoint = mRay.getPoint(dist);
					}
					//break;
				}
			}
		//}
		
		//else
		//{//von if(body
		//	ga->setDisplayedWeapon(equipped[eqRanged]);
		//	if(attacking && cooldown <= 0)
		//	{
		//		attackRanged();
		//	}
		//	//app->setDebugText("NULL",true);
		//	//dh obj = NULL
		//	ga->hideInfoText();
		//}
	}//if(hitcount > 0
	if(bod)
	{			
		//pointer auf das angeschaute objekt
		res.obj = Ogre::any_cast<GameObject*>(bod->getUserData());//mLevel->getObject(nod);			
	}
	else
	{
		res.hitPoint = mRay.getPoint(maxRange);
		res.objDist = maxRange;
	}
	/*if(res.obj)
		mDebugText(res.obj->getID());
	else
		mDebugText("NULL");*/
	return res;
}

void GameChar::updateLookingAt(Ogre::Real time)
{
#ifndef __editor
	StandardApplication *app = StandardApplication::getSingletonPtr();
	GameApp *ga = (GameApp*)app;
	
	curMeleeObj = getLookingAt(PICKUP_RANGE).obj;
	
	if(!curMeleeObj)
	{		
		if(attacking && cooldown <= 0)
		{
			attackRanged();
		}
		if(playerControlled)
		{
			ga->setDisplayedWeapon(equipped[eqRanged]);
			ga->hideInfoText();
		}
	}
	else
	{
		//wir sind in meleeweite und können etwas tun
		if(attacking && cooldown <= 0)
		{			
			attackMelee(curMeleeObj);
		}
		if(action)
		{
			curMeleeObj->onActivate(this);
			//mLevel->deleteObject(curMeleeObj);
			action = false;
		}
		if(playerControlled)
		{
			ga->setDisplayedWeapon(equipped[eqMelee]);
			if(curMeleeObj->getName() != "")
			{
				if(!ga->getCursorMode() || !isMouseOverGui())
				{//!ga->getCursorMode() )
					ga->showInfoText(curMeleeObj->getName(),0.3f,0.3f);
				}
				else
				{
					ga->hideInfoText();
				}
			}
			else
			{
				ga->hideInfoText();
			}
		}		
	}

	if(spellcasting && cooldown <= 0)
	{
		castSpell();
	}

	//always reset it
	action = false;
#endif
	
}

void GameChar::updateAnims(Ogre::Real time)
{
	//===========
	/*
	folgendes:
	-sobald jump==true muss sprunganim gespielt werden, ohne wenn und aber
	-sobald die Bewegung in Y-richtung negativ ist (als Fall) und der Char gerade
	 in der Luft ist, muss Fall-Anim gespielt werden
	-currentVel hat die geschwindigkeit als Vektor
	-variable isJumping?
	*/
	//===========
	//check what anim we need
	//StandardApplication::getSingletonPtr()->setDebugText(Ogre::StringConverter::toString(currentState->getTimePosition()));
	
	if(!dead)
	{
		if(onEarth)
		{
			//Ogre::LogManager::getSingletonPtr()->logMessage("onEarth");
			//we can do lots of stuff while standing on solid ground
			//mDebugText("playJumpAnim="+str(playJumpAnim));	
			if(playJumpAnim)
			{
				//hüpfen
				//geht nur wenn man auf dem Boden steht
				if(animType != AT_JUMP)
				{
					setRandomAnim(AT_JUMP);
					animType = AT_JUMP;
				}

			}
			else if(isMoving())
			{
				//Ogre::LogManager::getSingletonPtr()->logMessage("moving"+Ogre::StringConverter::toString(mvDown)+"wtf");
				/*StandardApplication::getSingletonPtr()->setDebugText(Ogre::StringConverter::toString(mvDown)+"wtf");*/
				if(!run)
				{
					//go somewhere i.e.
					if(animType != AT_WALK)
					{
						setRandomAnim(AT_WALK,true);
						animType = AT_WALK;

						//StandardApplication::getSingletonPtr()->setDebugText(Ogre::StringConverter::toString(mvDown));
						if(mvDown)
						{
							currentAnim.anim->setTimePosition(currentAnim.anim->getLength());
							/*StandardApplication::getSingletonPtr()->setDebugText("beginne rückwärts");*/
						}
					}
				}
				else if(run)
				{
					//or run...
					if(animType != AT_RUN)
					{
						setRandomAnim(AT_RUN);
						animType = AT_RUN;
						if(mvDown && currentAnim.anim)
						{
							currentAnim.anim->setTimePosition(currentAnim.anim->getLength());
						}
					}
				}
			}
			else if(!attacking && !spellcasting)
			{
				//or just stand still
				if(animType != AT_IDLE)
				{
					setRandomAnim(AT_IDLE);
					animType = AT_IDLE;
				}
			}

		}
		else//if(onEarth)
		{
			//jump will take care itself if its time to start fall anim
			//if(!jump)
			//{
			//	if(animType != AT_FALL)
			//	{
			//		setRandomAnim(AT_FALL);
			//		animType = AT_FALL;
			//	}
			//}
			if(currentVel.y < 0)
			{
				if(animType != AT_FALL)
				{
					setRandomAnim(AT_FALL);
					animType = AT_FALL;
				}
			}
		}
		if(attacking)
		{
			//we can attack even while falling
			if(curMeleeObj)
			{
				if(animType != AT_ATKMELEE)
				{
					setRandomAnim(AT_ATKMELEE);
					animType = AT_ATKMELEE;
				}
			}
			else
			{
				if(animType != AT_ATKRANGED)
				{
					setRandomAnim(AT_ATKRANGED);
					animType = AT_ATKRANGED;
				}
			}
		}
		else if(spellcasting)
		{
			if(animType != AT_CAST)
			{
				setRandomAnim(AT_CAST);
				animType = AT_CAST;
			}

		}
	}//if(!dead)
	else
	{
		if(animType != AT_DIE)
		{
			setRandomAnim(AT_DIE);
		}
	}

	//jetzt die animationen animieren
	if(currentAnim.anim)
	{
		Ogre::Vector3 moveSpeed;
		Ogre::Real moveFactor = currentAnim.factor;
		
		switch(animType)
		{
		case AT_WALK:
		case AT_RUN:
			if(mvDown)
			{
				//rückwärts
				if(currentAnim.anim->getTimePosition() <= 0)//currentState->getLength())
				{
					//start the next random anim
					setRandomAnim(animType);
					currentAnim.anim->setTimePosition(currentAnim.anim->getLength());
				}
				moveSpeed = currentVel;
				moveSpeed.y = 0;
				moveFactor *= -moveSpeed.length();
			}
			else
			{			
				//mDebugText("\nvorwärts=",true);
				if(currentAnim.anim->hasEnded())//currentAnim.anim->getTimePosition() >= currentAnim.anim->getLength())
				{
					
					//start the next random anim
					//mDebugText("\nneue anim=",true);
					
					setRandomAnim(animType);
				}
				moveSpeed = currentVel;
				moveSpeed.y = 0;
				moveFactor *= moveSpeed.length();

			}
			
			break;
	
		case AT_IDLE:	
		case AT_ATKMELEE:	
		case AT_ATKRANGED:
		case AT_CAST:
		case AT_FALL:
			if(currentAnim.anim->hasEnded())
			{
				//start the next random anim
				setRandomAnim(animType);
			}
			//currentState->addTime(time);
			break;
		case AT_DIE:
			if(currentAnim.anim->hasEnded())
			{
				//it's over, no need to update it more
				//just disable it
				//currentAnim.anim->setEnabled(false);
			//	moveFactor = 0;
			}
			
			break;
		case AT_JUMP:
			if(currentAnim.anim->hasEnded())
			{
				//switch to the fall anim at the end
				setRandomAnim(AT_FALL,true);
				animType = AT_FALL;
			}
			//currentState->addTime(time);
			break;
		

			
		}
		if(moveFactor != 0)
		{
			//Ogre::Real curtime = currentAnim.anim->getTimePosition();
			//Ogre::Real length = currentAnim.anim->getLength();
			//if(curtime+time*moveFactor > length)
			//{
			//	if(ID != "player")
			//		mDebugText("jezt passierts: curtime="+str(curtime)+"length="+str(length)+"\ntime="+str(time));
			//	//time = (length-curtime)/moveFactor;
			//}
			currentAnim.anim->addTime(time*moveFactor);
		}
	}


}

void GameChar::update(Ogre::Real time)
{
	
	
	//mDebugText(str(getPosition())+"+"+str(resetLag)+"\n"+str(startingPos));
	//nothing of this in the editor, erstmal
#ifndef __editor
	
	//updating anims:
	if(mArt)
		updateAnims(time);
	
	//REGENERIEREN
	
	if(!dead)
	{
		if(mStats.hitpoints <= 0)
			Die();
		if(mStats.mana < mStats.mStats[st_maxMP])
		{
			mStats.mana += time*mStats.mStats[st_manaRegenRate];
			if(mStats.mana > mStats.mStats[st_maxMP])
				mStats.mana = mStats.mStats[st_maxMP];
		}
		
		if(mStats.hitpoints < mStats.mStats[st_maxHP])
		{
			mStats.hitpoints += time*mStats.mStats[st_hpRegenRate];
			if(mStats.hitpoints > mStats.mStats[st_maxHP])
				mStats.hitpoints = mStats.mStats[st_maxHP];
			
		}
	
	
		//REGENERIEREN ENDE
		

		//update the AI
		if(!playerControlled)
		{
			if(mAI)
				mAI->updateGameChar();
		}

		//der teil aktualisiert wo man hinschaut
		updateLookingAt(time);


		
		
			//(mLevel->getWorld(),getPosition(),Ogre::Vector3::NEGATIVE_UNIT_Y * 10);  // throw a ray from the body to 2000 units below to calculate the                                                            // height of the body regard to the floor.
																						//nein, 10 sollte reichen
		onEarth = isOnEarth();

		


	//cooldown updaten
	   if(cooldown > 0)
	   {
		   cooldown -= time;
	   }
	   else if(cooldown <= 0)
	   {
		   cooldown = 0;
	   }
		
	   //missiles abfeuern, wenn noch welche kommen sollen
	   if(missileToSpawnRepeat > 0)
	   {
		   timeSinceLastMissile += time;
		   if(timeSinceLastMissile >= missileToSpawn->Missile->multimissile_delay)
			   spawnMissile(missileToSpawn);
	   }

	   updateMoveSound(time);
	   updateSpells(time);

	}//if(!dead)

   if(playerControlled)
	{
		//anzeige updaten
		static_cast<GameApp*>(StandardApplication::getSingletonPtr())->updateStatBars(mStats.hitpoints,mStats.mana,mStats.mStats[st_maxHP],mStats.mStats[st_maxMP]);
		static_cast<GameApp*>(StandardApplication::getSingletonPtr())->updateCompass();
	}
#endif
  
  

}

bool GameChar::isOnEarth()
{
	if(floorColliding)
		return true;
    //der Teil prüft entfernung zum Boden
	Ogre::Vector3 myPos = getPosition();

	
	OgreNewt::BasicRaycast floorRay(mLevel->getWorld(),myPos,myPos+Ogre::Vector3::NEGATIVE_UNIT_Y * 10,true);

    if (floorRay.getHitCount() > 0)
	{           
        OgreNewt::BasicRaycast::BasicRaycastInfo hit;
        hit.mBody = NULL;
		hit.mDistance = 500;
        for(int i=0;i<floorRay.getHitCount();i++)
        {
            OgreNewt::BasicRaycast::BasicRaycastInfo found = floorRay.getInfoAt(i);
			if(found.mBody != mBody && found.mBody->getType() != otWater && found.mDistance < hit.mDistance)
            {
                hit = found;
                //break;
            }
        }
        if(!hit.mBody)
            return false;
		distToFloor = hit.mDistance * 10; // calculale the distance to the floor
		distToFloor -= mSize.y;// remove char's height; 
		//app->setDebugText(str(distToFloor));
		//mDebugText("distToFloor="+str(distToFloor));
		if(distToFloor > 0.05) //muss noch verkleinert werden, ich weiß
		{
			return false;
		}
		else
		{
			return true;
		}
	} 
	else
	{
		return false;
	}
	
}

void GameChar::updateMoveSound(Ogre::Real time)
{
	
	Ogre::Vector3 pos;
	Ogre::Quaternion ornt;
	getPositionOrientation(pos,ornt);
		
	//test des openal-wrappers:
	SimpleSound::Listener *listener = SimpleSound::Listener::getSingletonPtr();
	listener->setPosition(pos);
	listener->setOrientation(ornt);
	listener->setVelocity(mBody->getVelocity());
	

	/*if(moveSound)
	{
		mLog("gamechar update. moveSound filename="+moveSound->getFileName());
	}*/
	//schritt-sound
	if(moveSoundSource)
	{
		if(!jumping)
		{
			if(moveSoundSource->getBuffer() != walkSoundBuffer)
				moveSoundSource->setBuffer(walkSoundBuffer);
			Ogre::Real waitTime = static_cast<gamedata_char*>(mData)->stepSoundWaitWalk;
			if(run)
				waitTime = static_cast<gamedata_char*>(mData)->stepSoundWaitRun;

			if(isMoving())
			{
				moveSoundPassed += time;
				if(!moveSoundSource->isPlaying() && moveSoundPassed >= waitTime)
				{
					moveSoundPassed = 0;
					moveSoundSource->play();
				}
				

			}
			else
			{
				if(moveSoundSource->isPlaying())
					moveSoundSource->stop();
			}
		}
		moveSoundSource->setPosition(pos);
	}
	if(attackSoundSource)
		attackSoundSource->setPosition(pos);
}

void GameChar::playAttackSound(Ogre::String filename)
{
	Buffer *attackBuff = mLevel->getBuffer(filename);
	if(!attackSoundSource)
	{		
		attackSoundSource = mLevel->createSource(attackBuff,stSfxLevel+stOwnerIsLevel);
	}
	else
	{
		attackSoundSource->stop();
		attackSoundSource->setBuffer(attackBuff);
	}
	if(attackSoundSource)
	{
		attackSoundSource->setPosition(getPosition());
		attackSoundSource->play();
	}
}

void GameChar::castSpell()
{
#ifndef __editor
	GameApp* app = (GameApp*)StandardApplication::getSingletonPtr();
	
	if(!eqSpell)
		return;
	if(cooldown > 0)
		return;

	//jetzt mana
	if(mStats.mana < eqSpell->manaCost)
	{
		app->showScreenText("Not enough mana!");
		return;
	}
	//jetzt ahnenmagie
	if(eqSpell->ancient)
	{
		if(mStats.ancientMagic == 0)
		{
			app->showScreenText("No ancient magic!");
			return;
		}
		mStats.ancientMagic--;
	}
	mStats.mana -= eqSpell->manaCost;
	
	cooldown = eqSpell->cooldown;
	if(eqSpell->sound != "")
	{
		playAttackSound(eqSpell->sound);
	/*	if(attackSound)
		{
			StandardApplication::getSingletonPtr()->destroySound(attackSound);
		}*/
//		attackSound = StandardApplication::getSingletonPtr()->createSound(eqSpell->sound,stSfxLevel);
//		mNode->attachObject(attackSound);
//		attackSound->play();
	}
	switch(eqSpell->spellHitType)
	{
	case shMissile:
		spawnMissile(eqSpell,true);
		break;
	case shExplosion:
		spawnExplosion(eqSpell);
		break;
	case shOnSelf:
		////erstmal test
		//if(eqSpell->effectData.type == seDamage)
		//{
		//	onDamage(eqSpell->effectData.dmg);
		//}
		AddSpell(eqSpell,this);
//		AddEffects(&eqSpell->effects,this);
		break;
	case shBeam:
		spawnBeam(eqSpell);
		break;
	}
#endif
				
}

void GameChar::attackMelee(GameObject *target)
{
	
	if(cooldown > 0)
		return;
	if(!equipped[eqMelee])
		return;
	//mLog("attackiere melee: "+equipped[eqMelee]->id);
	if(equipped[eqMelee]->meleeSound != "")
	{
		playAttackSound(equipped[eqMelee]->meleeSound);
		//StandardApplication::getSingletonPtr()->destroySound(attackSound);
		//attackSound = StandardApplication::getSingletonPtr()->createSound(equipped[eqMelee]->meleeSound,stSfxLevel);
		//mNode->attachObject(attackSound);
		//attackSound->play();
	}
	
	//pos += ornt * Ogre::Vector3(0,0,-1);
	cooldown = equipped[eqMelee]->meleeCooldown;
	//SpellEffectData *melee = equipped[eqMelee]->meleeEffect;
	target->onDamage(equipped[eqMelee]->meleeDamage,this);
	/*if(melee && melee->type == seDamage)
	{
		target->onDamage(melee->dmg);
	}*/
}

void GameChar::attackRanged()
{
	
	if(cooldown > 0)
		return;
	if(!equipped[eqRanged])
		return;
	//mLog("attackiere ranged: "+equipped[eqRanged]->id);
	//cooldown = TEST_COOLDOWN;
	
	if(equipped[eqRanged]->rangedSound != "")
	{
		playAttackSound(equipped[eqRanged]->rangedSound);
		//StandardApplication::getSingletonPtr()->destroySound(attackSound);
		//attackSound = StandardApplication::getSingletonPtr()->createSound(equipped[eqRanged]->rangedSound,stSfxLevel);
		//mNode->attachObject(attackSound);
		//attackSound->play();
	}

	//pos += ornt * Ogre::Vector3(0,0,-1);
	cooldown = equipped[eqRanged]->rangedCooldown;
//	SpecificMissileData d;
	
	//d.missile = *equipped[eqRanged]->Missile;
	//d.dmg = equipped[eqRanged]->rangedDamage;
//	d.effects = &equipped[eqRanged]->rangedEffects;
	//d.expl = equipped[eqRanged]->Explosion;
	//d.caster = this;
	spawnMissile(equipped[eqRanged],true);
}


void GameChar::attachCam(Ogre::Camera *cam)
{
	
	
	if(cam && cam->getParentNode())
	{
		//====MARKER==== den anderen char die controlle entziehen!
		cam->getParentSceneNode()->detachObject(cam);
	}

	camNode->attachObject(cam);
	//playerControlled = true;
	//thirdpersonNode->attachObject(playerCam);
}

void GameChar::detachCam(Ogre::Camera *cam)
{
	if(camNode->getAttachedObject(cam->getName()))
	{
		camNode->detachObject(cam);
	}

}

void GameChar::attachCam(Ogre::String name)
{
	
	Ogre::Camera *playerCam = mLevel->getSceneManager()->getCamera(name);
	if(playerCam->getParentNode())
	{
		//====MARKER==== den anderen char die controlle entziehen!
		playerCam->getParentSceneNode()->detachObject(playerCam);
	}

	camNode->attachObject(playerCam);
	//playerControlled = true;
	//thirdpersonNode->attachObject(playerCam);
}

void GameChar::detachCam(Ogre::String name)
{
	if(camNode->getAttachedObject(name))
	{
		camNode->detachObject(name);
	}

}
void GameChar::modLookingDirection(Ogre::Real x, Ogre::Real y)
{
	//Ogre::Real ls = 75;//app->conf_lookspeed;
//	app->settext(Ogre::StringConverter::toString(ls));

	//for the callback:
	rotation_x = x;
	
	//*ls;// * x_sensibility;// * global->conf_lookspeed;
	//rotation_y = y;//*ls;// * y_sensibility;// * global->conf_lookspeed;

	y_rotation_cont += y;
	if (y_rotation_cont > y_limit_a || y_rotation_cont < y_limit_b)  // if the total is bigger or smallest than the limits it will be reseted to it's previous value
	{
		y_rotation_cont -= y;
	}
	else
	{
		camNode->pitch(Ogre::Degree(y));
	}
	//y_rotation_cont += rotation_y;

	//if (y_rotation_cont > y_limit_a || y_rotation_cont < y_limit_b)  // if the total is bigger or smallest than the limits it will be reseted to it's previous value
	//{
	//	camNode->pitch(Ogre::Degree(-rotation_y ));
	//	//thirdpersonNode->pitch(Ogre::Degree(-rotation_y ));
	//	y_rotation_cont -= rotation_y;
	//} 
	
}





void GameChar::setLookingDirection(Ogre::Vector3 dir)
{
	Ogre::Quaternion curOrnt = getOrientation();
	Ogre::Vector3 curLookingDir = curOrnt*Ogre::Vector3::NEGATIVE_UNIT_Z;
	Ogre::Vector3 target_noY = dir;
	target_noY.y = 0;
	Ogre::Vector3 curLookingDir_noY = curLookingDir;
	curLookingDir_noY.y=0;
	if(!target_noY.positionEquals(Ogre::Vector3::ZERO))
	{
		Ogre::Quaternion q = curLookingDir_noY.getRotationTo(target_noY);
		rotate(q);
	}
	camNode->setDirection(dir,Ogre::Node::TS_WORLD);
	//jetzt hoch/runter
	/*Ogre::Quaternion headornt = camNode->getOrientation();
	Ogre::Vector3 headDir = headornt*Ogre::Vector3::NEGATIVE_UNIT_Z;
	headDir.x = 0;
	Ogre::Vector3 headTarget = curOrnt*dir;
	headTarget.x = 0;
	if(!headTarget.positionEquals(Ogre::Vector3::ZERO))
	{
		Ogre::Quaternion q = headDir.getRotationTo(headTarget);
		camNode->rotate(q);
		
	}*/

}

void GameChar::lookAt(Ogre::Vector3 target)
{
	Ogre::Vector3 lookDir = target - getPosition();
	setLookingDirection(lookDir);
	
	//if(target.directionEquals
}

void GameChar::setRandomAnim(AnimationType _animType,bool loop)
{
	//lol: bool lood 
	//     bood doob
	//     qoop qoop
	//     dbqp ppqdbdp


	

	animType = _animType;
	CharAnimation newAnim;
	switch(animType)
	{
	case AT_IDLE:
		newAnim = getRandomAnimState(mAnims.idle);
		//newAnimName = getRandomString(myData->anim_idle);
		break;
	case AT_WALK:
		newAnim = getRandomAnimState(mAnims.walk);

		break;
	case AT_ATKMELEE:
		newAnim = getRandomAnimState(mAnims.attackmelee);

		break;
	case AT_ATKRANGED:
		newAnim = getRandomAnimState(mAnims.attackranged);

		break;
	case AT_DIE:	
		newAnim = getRandomAnimState(mAnims.death);

		break;
	case AT_JUMP:
		newAnim = getRandomAnimState(mAnims.jump);

		break;
	case AT_RUN:
		newAnim = getRandomAnimState(mAnims.run);

		break;
	case AT_FALL:
		newAnim = getRandomAnimState(mAnims.fall);

		break;
	case AT_CAST:
		newAnim = getRandomAnimState(mAnims.cast);

		break;
	}
	if(currentAnim.anim)
	{
		
		
		currentAnim.anim->setTimePosition(currentAnim.anim->getLength());
		if(currentAnim.anim->getEnabled())
			currentAnim.anim->setEnabled(false);
		
		
		
	}
	
	if(newAnim.anim)
	{
		
		
		//Ogre::LogManager::getSingletonPtr()->logMessage("newAnimName="+newState->getAnimationName());
		//mAnimState=ent->getAnimationState(newAnimName);		
		newAnim.anim->setTimePosition(0);
		if(!newAnim.anim->getEnabled())
			newAnim.anim->setEnabled(true);
		newAnim.anim->setLoop(loop);
		currentAnim = newAnim;
		//oldState = newState;
	}
	else
	{
		//Ogre::LogManager::getSingletonPtr()->logMessage("WTF?!");
		//just freeze it and do nothing
		//mAnimState->setTimePosition(0);
	}



}

void GameChar::fillAnimLists()
{
	Ogre::AnimationStateSet *as = mArt->getAllAnimStates();
	

	std::vector<AnimData>::iterator itr;
	gamedata_char* myData = static_cast<gamedata_char*>(mData);
	for(itr = myData->anim_attackmelee.begin();itr != myData->anim_attackmelee.end();itr++)
	{
		AnimData curData = *itr;
		CharAnimation curAnim;
		curAnim.anim = mArt->getAnimState(curData.animName);
		curAnim.factor = curData.factor;		
		mAnims.attackmelee.push_back(curAnim);
	}
	for(itr = myData->anim_attackranged.begin();itr != myData->anim_attackranged.end();itr++)
	{
		AnimData curData = *itr;
		CharAnimation curAnim;
		curAnim.anim = mArt->getAnimState(curData.animName);
		curAnim.factor = curData.factor;		
		mAnims.attackranged.push_back(curAnim);
	}
	for(itr = myData->anim_idle.begin();itr != myData->anim_idle.end();itr++)
	{
		
		
		AnimData curData = *itr;
		CharAnimation curAnim;
		curAnim.anim = mArt->getAnimState(curData.animName);
		curAnim.factor = curData.factor;		
		mAnims.idle.push_back(curAnim);
	}
	for(itr = myData->anim_walk.begin();itr != myData->anim_walk.end();itr++)
	{
		
		
		AnimData curData = *itr;
		CharAnimation curAnim;
		curAnim.anim = mArt->getAnimState(curData.animName);
		curAnim.factor = curData.factor;		
		mAnims.walk.push_back(curAnim);
	}
	for(itr = myData->anim_death.begin();itr != myData->anim_death.end();itr++)
	{
		
		
		AnimData curData = *itr;
		CharAnimation curAnim;
		curAnim.anim = mArt->getAnimState(curData.animName);
		curAnim.factor = curData.factor;		
		mAnims.death.push_back(curAnim);
	}
	for(itr = myData->anim_jump.begin();itr != myData->anim_jump.end();itr++)
	{
		
		
		AnimData curData = *itr;
		CharAnimation curAnim;
		curAnim.anim = mArt->getAnimState(curData.animName);
		curAnim.factor = curData.factor;		
		mAnims.jump.push_back(curAnim);
	}
	for(itr = myData->anim_run.begin();itr != myData->anim_run.end();itr++)
	{
		
		
		AnimData curData = *itr;
		CharAnimation curAnim;
		curAnim.anim = mArt->getAnimState(curData.animName);
		curAnim.factor = curData.factor;		
		mAnims.run.push_back(curAnim);
	}
	for(itr = myData->anim_fall.begin();itr != myData->anim_fall.end();itr++)
	{
		
		
		AnimData curData = *itr;
		CharAnimation curAnim;
		curAnim.anim = mArt->getAnimState(curData.animName);
		curAnim.factor = curData.factor;		
		mAnims.fall.push_back(curAnim);
	}
	for(itr = myData->anim_cast.begin();itr != myData->anim_cast.end();itr++)
	{
		
		
		AnimData curData = *itr;
		CharAnimation curAnim;
		curAnim.anim = mArt->getAnimState(curData.animName);
		curAnim.factor = curData.factor;		
		mAnims.cast.push_back(curAnim);
	}
	

}

void GameChar::setJumpHeight(Ogre::Real height)
{
	/*Ogre::Real grav = mLevel->getGravity();
	if(grav < 0)
		grav *= -1;*/
	//mLog("height="+str(height));
	jumpPower = Ogre::Math::Sqrt ( - height * (2*GRAVITY) ) / MAX_JUMPTIME;
	//mLog("jumpPower="+str(jumpPower));

	/*Ogre::Real squaredJP = -2*GRAVITY*height;
	jumpPower = Ogre::Math::Sqrt(squaredJP);*/
	/*Ogre::LogManager::getSingleton().logMessage("JUMPHEIGHT");
	Ogre::LogManager::getSingleton().logMessage(Ogre::StringConverter::toString(squaredJP));
	Ogre::LogManager::getSingleton().logMessage(Ogre::StringConverter::toString(jumpPower));*/
}

Ogre::Real GameChar::getJumpHeight()
{
	return - Ogre::Math::Sqr(jumpPower * MAX_JUMPTIME) / (2*GRAVITY);
	
	
}


void GameChar::onDamage(Damage dmg,GameObject *attacker)
{
	if(dead)
		return;
//	mDebugText("dmg");
	Ogre::Real hp = mStats.hitpoints;
	hp -= (dmg.hitBlunt * mStats.mStats[st_resistBlunt]);
	hp -= (dmg.hitCut * mStats.mStats[st_resistCut]);
	hp -= (dmg.hitPierce * mStats.mStats[st_resistPierce]);
	hp -= (dmg.fire * mStats.mStats[st_resistFire]);
	hp -= (dmg.ice * mStats.mStats[st_resistIce]);
	hp -= (dmg.spark * mStats.mStats[st_resistSpark]);
	hp -= (dmg.poison * mStats.mStats[st_resistPoison]);
	hp -= (dmg.dark * mStats.mStats[st_resistDark]);
	hp -= (dmg.light * mStats.mStats[st_resistLight]);
	if(hp > mStats.mStats[st_maxHP])
		hp = mStats.mStats[st_maxHP];
	if(hp <= 0)
	{
		hp = 0;
		Die();
		//sterben
	}
#ifndef __editor
	if(!dead && mAI && mStats.hitpoints > hp)
	{
		mAI->gotHurt(attacker);

	}
#endif
	mStats.hitpoints = hp;
}

void GameChar::onActivate(GameChar *user,bool cursorMode)
{
#ifndef __editor
	GameApp *app = static_cast<GameApp*>(StandardApplication::getSingletonPtr());
	if(dead)
	{

		app->showOtherInventory(inv,this);
	}
#endif
}
void GameChar::updateStats()
{	
	Ogre::Real curhp = mStats.hitpoints;
	Ogre::Real curmana=mStats.mana;
	int curanc = mStats.ancientMagic;
	addStats = baseStats;	
	
	//addStats:
	for(int i=0;i<EQUIP_CNT;i++)
	{
		if(!equipped[i])
			continue;

		gamedata_item *curData = equipped[i];
		if(curData->equipStats.empty())
			continue;
		for(StatChangeList::const_iterator itr=curData->equipStats.begin();itr!=curData->equipStats.end();itr++)
		{
			ChStat curStat = *itr;
			if(curStat.op == opAdd)
			{
				addStats.mStats[curStat.type] += curStat.value;
			}
		}

	}


	multStats = addStats;
	//multStats:
	for(int i=0;i<EQUIP_CNT;i++)
	{
		if(!equipped[i])
			continue;

		gamedata_item *curData = equipped[i];
		if(curData->equipStats.empty())
			continue;
		for(StatChangeList::const_iterator itr=curData->equipStats.begin();itr!=curData->equipStats.end();itr++)
		{
			ChStat curStat = *itr;
			if(curStat.op == opMultiply)
			{
				multStats.mStats[curStat.type] *= curStat.value;
			}
		}

	}

	//und jetzt die finalen
	mStats = multStats;
	//setzen:
	for(int i=0;i<EQUIP_CNT;i++)
	{
		if(!equipped[i])
			continue;

		gamedata_item *curData = equipped[i];
		if(curData->equipStats.empty())
			continue;
		for(StatChangeList::const_iterator itr=curData->equipStats.begin();itr!=curData->equipStats.end();itr++)
		{
			ChStat curStat = *itr;
			if(curStat.op == opSet)
			{
				mStats.mStats[curStat.type] = curStat.value;
			}
		}

	}
	mStats.hitpoints = curhp;
	mStats.mana=curmana;
	mStats.ancientMagic=curanc;

	////StandardApplication *app = StandardApplication::getSingletonPtr();
	//unsigned int hp = mStats.hitpoints;
	//unsigned int mana = mStats.mana;
	//mStats = baseStats;	
	//////erstmal setzen
	////for(int i=0;i<EQUIP_CNT;i++)
	////{
	////	if(!equipped[i])
	////		continue;

	////	gamedata_item *curData = equipped[i];
	////	//Stats_Item itS = eqMeleeData->equipStats;	
	////	if(curData && curData->equipEffect)
	////	{
	////		Stats_Item its = curData->equipEffect->stat;
	////		if(its.m_strength == Stats_Item::Set)
	////			mStats.strength = its.strength;
	////		if(its.m_agility == Stats_Item::Set)
	////			mStats.agility = its.agility;
	////		if(its.m_magic == Stats_Item::Set)
	////			mStats.magic = its.magic;
	////		if(its.m_maxHP == Stats_Item::Set)
	////			mStats.maxHP = its.maxHP;
	////		if(its.m_maxMP == Stats_Item::Set)
	////			mStats.maxMP = its.maxMP;
	////		
	////		if(its.m_resistBlunt == Stats_Item::Set)
	////			mStats.resistBlunt = its.resistBlunt;
	////		if(its.m_resistCut == Stats_Item::Set)
	////			mStats.resistCut = its.resistCut;
	////		if(its.m_resistPierce == Stats_Item::Set)
	////			mStats.resistPierce = its.resistPierce;
	////		if(its.m_resistFire == Stats_Item::Set)
	////			mStats.resistFire = its.resistFire;
	////		if(its.m_resistIce == Stats_Item::Set)
	////			mStats.resistIce = its.resistIce;
	////		if(its.m_resistSpark == Stats_Item::Set)
	////			mStats.resistSpark = its.resistSpark;
	////		if(its.m_resistPoison == Stats_Item::Set)
	////			mStats.resistPoison = its.resistPoison;
	////		if(its.m_resistDark == Stats_Item::Set)
	////			mStats.resistDark = its.resistDark;
	////		if(its.m_resistLight == Stats_Item::Set)
	////			mStats.resistLight = its.resistLight;
	////	}
	////	
	////}
	////jetzt addieren
	//for(int i=0;i<EQUIP_CNT;i++)
	//{
	//	if(!equipped[i])
	//		continue;

	//	gamedata_item *curData = equipped[i];
	//	//Stats_Item itS = eqMeleeData->equipStats;	
	//	if(curData && curData->equipEffect)
	//	{
	//		Stats_Item its = curData->equipEffect->stat;
	//		if(its.m_strength == Stats_Item::Add)
	//			mStats.strength += its.strength;
	//		if(its.m_agility == Stats_Item::Add)
	//			mStats.agility += its.agility;
	//		if(its.m_magic == Stats_Item::Add)
	//			mStats.magic += its.magic;
	//		if(its.m_maxHP == Stats_Item::Add)
	//			mStats.maxHP += its.maxHP;
	//		if(its.m_maxMP == Stats_Item::Add)
	//			mStats.maxMP += its.maxMP;
	//		
	//		if(its.m_resistBlunt == Stats_Item::Add)
	//			mStats.resistBlunt += its.resistBlunt;
	//		if(its.m_resistCut == Stats_Item::Add)
	//			mStats.resistCut += its.resistCut;
	//		if(its.m_resistPierce == Stats_Item::Add)
	//			mStats.resistPierce += its.resistPierce;
	//		if(its.m_resistFire == Stats_Item::Add)
	//			mStats.resistFire += its.resistFire;
	//		if(its.m_resistIce == Stats_Item::Add)
	//			mStats.resistIce += its.resistIce;
	//		if(its.m_resistSpark == Stats_Item::Add)
	//			mStats.resistSpark += its.resistSpark;
	//		if(its.m_resistPoison == Stats_Item::Add)
	//			mStats.resistPoison += its.resistPoison;
	//		if(its.m_resistDark == Stats_Item::Add)
	//			mStats.resistDark += its.resistDark;
	//		if(its.m_resistLight == Stats_Item::Add)
	//			mStats.resistLight += its.resistLight;
	//	}
	//	
	//}
	////und finally multiplizieren
	//for(int i=0;i<EQUIP_CNT;i++)
	//{
	//	if(!equipped[i])
	//		continue;

	//	gamedata_item *curData = equipped[i];
	//	//Stats_Item itS = eqMeleeData->equipStats;	
	//	if(curData && curData->equipEffect)
	//	{
	//		Stats_Item its = curData->equipEffect->stat;
	//		if(its.m_strength == Stats_Item::Multiply)
	//			mStats.strength *= its.strength;
	//		if(its.m_agility == Stats_Item::Multiply)
	//			mStats.agility *= its.agility;
	//		if(its.m_magic == Stats_Item::Multiply)
	//			mStats.magic *= its.magic;
	//		if(its.m_maxHP == Stats_Item::Multiply)
	//			mStats.maxHP *= its.maxHP;
	//		if(its.m_maxMP == Stats_Item::Multiply)
	//			mStats.maxMP *= its.maxMP;
	//		
	//		if(its.m_resistBlunt == Stats_Item::Multiply)
	//			mStats.resistBlunt *= its.resistBlunt;
	//		if(its.m_resistCut == Stats_Item::Multiply)
	//			mStats.resistCut *= its.resistCut;
	//		if(its.m_resistPierce == Stats_Item::Multiply)
	//			mStats.resistPierce *= its.resistPierce;
	//		if(its.m_resistFire == Stats_Item::Multiply)
	//			mStats.resistFire *= its.resistFire;
	//		if(its.m_resistIce == Stats_Item::Multiply)
	//			mStats.resistIce *= its.resistIce;
	//		if(its.m_resistSpark == Stats_Item::Multiply)
	//			mStats.resistSpark *= its.resistSpark;
	//		if(its.m_resistPoison == Stats_Item::Multiply)
	//			mStats.resistPoison *= its.resistPoison;
	//		if(its.m_resistDark == Stats_Item::Multiply)
	//			mStats.resistDark *= its.resistDark;
	//		if(its.m_resistLight == Stats_Item::Multiply)
	//			mStats.resistLight *= its.resistLight;
	//	}
	//	
	//}
	//
	//StandardApplication *app = StandardApplication::getSingletonPtr();
	//unsigned int hp = mStats.hitpoints;
	//unsigned int mana = mStats.mana;
	//mStats = baseStats;	
	////erstmal setzen
	////nahkampf-waffe:
	//gamedata_item *eqMeleeData = app->getItemDataPtr(eqMelee);
	////Stats_Item itS = eqMeleeData->equipStats;	
	//if(eqMeleeData && eqMeleeData->equipStats)
	//{
	//	Stats_Item its = eqMeleeData->equipStats;
	//	if(its.m_strength == Stats_Item::Set)
	//		mStats.strength = its.strength;
	//	if(its.m_agility == Stats_Item::Set)
	//		mStats.agility = its.agility;
	//	if(its.m_magic == Stats_Item::Set)
	//		mStats.magic = its.magic;
	//	if(its.m_maxHP == Stats_Item::Set)
	//		mStats.maxHP = its.maxHP;
	//	if(its.m_maxMP == Stats_Item::Set)
	//		mStats.maxMP = its.maxMP;
	//	
	//	if(its.m_resistBlunt == Stats_Item::Set)
	//		mStats.resistBlunt = its.resistBlunt;
	//	if(its.m_resistCut == Stats_Item::Set)
	//		mStats.resistCut = its.resistCut;
	//	if(its.m_resistPierce == Stats_Item::Set)
	//		mStats.resistPierce = its.resistPierce;
	//	if(its.m_resistFire == Stats_Item::Set)
	//		mStats.resistFire = its.resistFire;
	//	if(its.m_resistIce == Stats_Item::Set)
	//		mStats.resistIce = its.resistIce;
	//	if(its.m_resistSpark == Stats_Item::Set)
	//		mStats.resistSpark = its.resistSpark;
	//	if(its.m_resistPoison == Stats_Item::Set)
	//		mStats.resistPoison = its.resistPoison;
	//	if(its.m_resistDark == Stats_Item::Set)
	//		mStats.resistDark = its.resistDark;
	//	if(its.m_resistLight == Stats_Item::Set)
	//		mStats.resistLight = its.resistLight;

	//	//mStats += *eqMeleeData->equipStats;
	//}
	////panzerung...
	//gamedata_item *eqArmorData = app->getItemDataPtr(eqArmor);
	//if(eqArmorData && eqArmorData->equipStats)
	//{
	//	mStats += *eqArmorData->equipStats;
	//}
	//gamedata_item *eqRangedData = app->getItemDataPtr(eqRanged);
	//if(eqRangedData && eqRangedData->equipStats)
	//{
	//	mStats += *eqRangedData->equipStats;
	//}
	//gamedata_item *eqShieldData = app->getItemDataPtr(eqShield);
	//if(eqShieldData && eqShieldData->equipStats)
	//{
	//	mStats += *eqShieldData->equipStats;
	//}

	//gamedata_item *eqElse1Data = app->getItemDataPtr(eqElse1);
	//if(eqElse1Data && eqElse1Data->equipStats)
	//{
	//	mStats += *eqElse1Data->equipStats;
	//}
	//gamedata_item *eqElse2Data = app->getItemDataPtr(eqElse2);
	//if(eqElse2Data && eqElse2Data->equipStats)
	//{
	//	mStats += *eqElse2Data->equipStats;
	//}
	//gamedata_item *eqElse3Data = app->getItemDataPtr(eqElse3);
	//if(eqElse3Data && eqElse3Data->equipStats)
	//{
	//	mStats += *eqElse3Data->equipStats;
	//}
	//gamedata_item *eqElse4Data = app->getItemDataPtr(eqElse4);
	//if(eqElse4Data && eqElse4Data->equipStats)
	//{
	//	mStats += *eqElse4Data->equipStats;
	//}




	//Ogre::String eqMelee;
	//Ogre::String eqArmor;
	//Ogre::String eqRanged;
	//Ogre::String eqShield;

	//Ogre::String eqElse1;
	//Ogre::String eqElse2;
	//Ogre::String eqElse3;
	//Ogre::String eqElse4;
}

void GameChar::addStat(ChStat st)
{
	Ogre::Real temp, temp2;
	switch(st.op)
	{
	case opAdd:
		//den addStats-wert sichern
		temp = addStats.mStats[st.type];
		//den multStats auch
		temp2= multStats.mStats[st.type];
		addStats.mStats[st.type] += st.value;		
		multStats.mStats[st.type] = (temp2/temp)*addStats.mStats[st.type];
		if(temp2 == mStats.mStats[st.type])
			mStats.mStats[st.type] = multStats.mStats[st.type];
		break;
	case opMultiply:
		//auf multStats anwenden und bei bedarf auf mStats anwenden
		temp = multStats.mStats[st.type];
		multStats.mStats[st.type] *= st.value;
		if(temp == mStats.mStats[st.type])
			mStats.mStats[st.type] = multStats.mStats[st.type];
		break;
	case opSet:
		//am einfachsten, nur das mStats ändern
		mStats.mStats[st.type] = st.value;
		break;
	};
}

void GameChar::removeStat(ChStat st)
{
	if(st.op == opSet)
	{
		mStats.mStats[st.type] = multStats.mStats[st.type];
	}
	else
	{
		addStat(st.inverse());
	}
}

bool GameChar::equipItem(gamedata_item *item,unsigned int type,Inventory *_inv)
{
	switch(type)
	{
		case eqMelee: 
			return setMelee(item);
		case eqRanged: 
			return setRanged(item);
		case eqArmor: 
			return setArmor(item);
		case eqShield: 
			return setShield(item);
		case eqElse1: 
			return setElse1(item);	
		case eqElse2: 
			return setElse2(item);
		case eqElse3: 
			return setElse3(item);
		case eqElse4: 
			return setElse4(item);
		case eqBag: 
			return setBag(item,_inv);
	}
	return false;
}

bool GameChar::setMelee(gamedata_item *item)
{	
	if((item && (item->equipFlags & EF_MELEE)) || !item)
	{
       
		equipped[eqMelee] = item;
		updateStats();
		return true;
	}
	else
	{
		return false;
	}
}
bool GameChar::setMelee(Ogre::String itemID)
{
	gamedata_item *idata = NULL;
	if(itemID != "")
	{
		StandardApplication *app = StandardApplication::getSingletonPtr();
		idata = app->getItemDataPtr(itemID);
	}
	return setMelee(idata);
}
bool GameChar::setRanged(gamedata_item *item)
{
	if((item && (item->equipFlags & EF_RANGED)) || !item)
	{
        
		equipped[eqRanged] = item;
		updateStats();
		return true;
	}
	else
	{
		return false;
	}
}
bool GameChar::setRanged(Ogre::String itemID)
{
	gamedata_item *idata = NULL;
	if(itemID != "")
	{
		StandardApplication *app = StandardApplication::getSingletonPtr();
		idata = app->getItemDataPtr(itemID);
	}
	return setRanged(idata);
}
bool GameChar::setShield(gamedata_item *item)
{
	if((item && (item->equipFlags & EF_SHIELD)) || !item)
	{
        
		equipped[eqShield] = item;
		updateStats();
		return true;
	}
	else
	{
		return false;
	}
}
bool GameChar::setShield(Ogre::String itemID)
{
	gamedata_item *idata = NULL;
	if(itemID != "")
	{
		StandardApplication *app = StandardApplication::getSingletonPtr();
		idata = app->getItemDataPtr(itemID);
	}
	return setShield(idata);
}
bool GameChar::setArmor(gamedata_item *item)
{
	if((item && (item->equipFlags & EF_ARMOR)) || !item)
	{
        
		equipped[eqArmor] = item;
		updateStats();
		return true;
	}
	else
	{
		return false;
	}
}
bool GameChar::setArmor(Ogre::String itemID)
{
	gamedata_item *idata = NULL;
	if(itemID != "")
	{
		StandardApplication *app = StandardApplication::getSingletonPtr();
		idata = app->getItemDataPtr(itemID);
	}
	return setArmor(idata);
}
bool GameChar::setElse1(gamedata_item *item)
{
	if((item && (item->equipFlags & EF_ELSE)) || !item)
	{
        
		equipped[eqElse1] = item;
		updateStats();
		return true;
	}
	else
	{
		return false;
	}
}
bool GameChar::setElse1(Ogre::String itemID)
{
	gamedata_item *idata = NULL;
	if(itemID != "")
	{
		StandardApplication *app = StandardApplication::getSingletonPtr();
		idata = app->getItemDataPtr(itemID);
	}
	return setElse1(idata);
}
bool GameChar::setElse2(gamedata_item *item)
{
	if((item && (item->equipFlags & EF_ELSE)) || !item)
	{
        
		equipped[eqElse2] = item;
		updateStats();
		return true;
	}
	else
	{
		return false;
	}
}
bool GameChar::setElse2(Ogre::String itemID)
{
	gamedata_item *idata = NULL;
	if(itemID != "")
	{
		StandardApplication *app = StandardApplication::getSingletonPtr();
		idata = app->getItemDataPtr(itemID);
	}
	return setElse2(idata);
}
bool GameChar::setElse3(gamedata_item *item)
{
	if((item && (item->equipFlags & EF_ELSE)) || !item)
	{
        
		equipped[eqElse3] = item;
		updateStats();
		return true;
	}
	else
	{
		return false;
	}
}
bool GameChar::setElse3(Ogre::String itemID)
{
	gamedata_item *idata = NULL;
	if(itemID != "")
	{
		StandardApplication *app = StandardApplication::getSingletonPtr();
		idata = app->getItemDataPtr(itemID);
	}
	return setElse3(idata);
}
bool GameChar::setElse4(gamedata_item *item)
{
	if((item && (item->equipFlags & EF_ELSE)) || !item)
	{
        
		equipped[eqElse4] = item;
		updateStats();
		return true;
	}
	else
	{
		return false;
	}
}
bool GameChar::setElse4(Ogre::String itemID)
{
	gamedata_item *idata = NULL;
	if(itemID != "")
	{
		StandardApplication *app = StandardApplication::getSingletonPtr();
		idata = app->getItemDataPtr(itemID);
	}
	return setElse4(idata);
}


bool GameChar::useItem(gamedata_item *item)
{
	return item->onUse(this);
    /*if(item->useSpell)
    {
        AddSpell(item->useSpell,this);
        if(item->propertyFlags & PF_LOSE_ON_USE)
            inv->removeItem(item);
        return true;
    }
    return false;*/
}

bool GameChar::equipItem(gamedata_item *item)
{
    if(!item || item->equipFlags == EF_NULL)
        return false;
    if(item->equipFlags & EF_MELEE)
    {
        if(getMelee())
        {
            if(!inv->addItem(getMelee()))
                return false;
        }
        if(!inv->removeItem(item))
            return false;
        if(setMelee(item))
            return true;
    }
    if(item->equipFlags & EF_RANGED)
    {
        if(getRanged())
        {
            if(!inv->addItem(getRanged()))
                return false;
        }
        if(!inv->removeItem(item))
            return false;
        if(setRanged(item))
            return true;
    }
    if(item->equipFlags & EF_ARMOR)
    {
        if(getArmor())
        {
            if(!inv->addItem(getArmor()))
                return false;
        }
        if(!inv->removeItem(item))
            return false;
        if(setArmor(item))
            return true;
    }
    if(item->equipFlags & EF_SHIELD)
    {
        if(getShield())
        {
            if(!inv->addItem(getShield()))
                return false;
        }
        if(!inv->removeItem(item))
            return false;
        if(setShield(item))
            return true;
    }
    if(item->equipFlags & EF_ELSE)
    {
        //hm. erstmal schauen ob eines frei ist
        if(!getElse1())
        {
            if(getElse1())
            {
                if(!inv->addItem(getElse1()))
                    return false;
            }
            if(!inv->removeItem(item))
                return false;
            if(setElse1(item))
                return true;
        }
        if(!getElse2())
        {
            if(getElse2())
            {
                if(!inv->addItem(getElse2()))
                    return false;
            }
            if(!inv->removeItem(item))
                return false;
            if(setElse2(item))
                return true;
        }
        if(!getElse3())
        {
            if(getElse3())
            {
                if(!inv->addItem(getElse3()))
                    return false;
            }
            if(!inv->removeItem(item))
                return false;
            if(setElse3(item))
                return true;
        }
        if(!getElse4())
        {
            if(getElse4())
            {
                if(!inv->addItem(getElse4()))
                    return false;
            }
            if(!inv->removeItem(item))
                return false;
            if(setElse4(item))
                return true;
        }
        //und hier jetzt einfach auf platz 1 setzen
        if(getElse1())
        {
            if(!inv->addItem(getElse1()))
                return false;
        }
        if(!inv->removeItem(item))
            return false;
        if(setElse1(item))
            return true;
    }
    if(item->equipFlags & EF_BAG)
    {
        if(getBag())
        {
            if(inv || !inv->addItem(getBag()))
                return false;
        }
        if(!inv->removeItem(item))
            return false;
        if(setBag(item))
            return true;
    }
    return false;
}

bool GameChar::setBag(gamedata_item* container, Inventory *_inv)
{    
	if(container)
	{
        if(container->Container)
		{
			equipped[eqBag] = container;
			if(_inv)
				inv = _inv;
			else
                inv = new Inventory(container->Container->defaultSize);
			return true;
		}
		else
			return false;
	}
	else
	{
		equipped[eqBag] = NULL;
		inv = NULL;
		return true;
	}
	
}
bool GameChar::setBag(Ogre::String containerID,Inventory *_inv)
{
	gamedata_item *idata = NULL;
	if(containerID != "")
	{
		
		//updateStats();
		//ich gehe davon aus, dass das inventar woanders behandelt wird
		StandardApplication *app = StandardApplication::getSingletonPtr();
		idata = app->getItemDataPtr("container_"+containerID);
	}
	return setBag(idata,_inv);
}
bool GameChar::setSpell(Ogre::String spellID)
{
	if(spellID == "")
		return false;
	
	eqSpell = StandardApplication::getSingletonPtr()->getSpellDataPtr(spellID);
	return true;
}
bool GameChar::setSpell(gamedata_spell* spell)
{	
#ifndef __editor
    static_cast<GameApp*>(StandardApplication::getSingletonPtr())->setDisplayedSpell(spell);
#endif
	eqSpell = spell;
	return true;
}

void GameChar::spawnExplosion(gamedata_spell *sp)// ExplosionData data, EffectDataList *eff,Ogre::Real range)
{
	LookAtResult res= getLookingAt(sp->range);
	GameObject *meleeObj = res.obj;
	Ogre::Vector3 spawnPoint = res.hitPoint;
	

	//if(meleeObj)
	//{

	//	meleeObj->AddSpell(sp,this);
	//	
	//}
	//Ogre::Vector3 spawnPoint = meleeObj->getPosition();
	new ExplosionObject(this,meleeObj,sp,mLevel,spawnPoint,false,false,getOrientation());
	
	
}

void GameChar::spawnBeam(gamedata_spell *sp)// ExplosionData data, EffectDataList *eff,Ogre::Real range)
{
	/*LookAtResult res= getLookingAt(sp->range);
	GameObject *meleeObj = res.obj;
	Ogre::Vector3 spawnPoint = res.hitPoint;
	

	if(meleeObj)
	{

		meleeObj->AddSpell(sp,this);
		
	}*/
	//Ogre::Vector3 spawnPoint = meleeObj->getPosition();
	new BeamObject(this,sp,mLevel);
	
	
}


Ogre::Quaternion GameChar::getFiringOrientation()
{
	//Ogre::Quaternion ornt=/*getFiringOrientation();*/camNode->_getDerivedOrientation();
	////mBody->getPositionOrientation(pos,ornt);
	////ornt = 
	////Ogre::Vector3 v0 = mBody->getVelocity();
	//
	//Ogre::Vector3 pos = getPosition();
	//if(playerControlled)// && false)
	//{
	//	//Ogre::Ray mRay = getLookingRay();
	//	//Ogre::Quaternion q =Ogre::Vector3::NEGATIVE_UNIT_Z.getRotationTo(mRay.getDirection());// dirHeadTp.getRotationTo(dirCenterTp);
	//	
	//	Ogre::Vector3	posHead = camNode->_getDerivedPosition();
	//	Ogre::Ray mRay = getLookingRay();
	//	RaySceneQuery *	mRaySceneQuery = mLevel->getSceneManager()->createRayQuery(mRay);
	//	mRaySceneQuery->setSortByDistance(true);
	//	RaySceneQueryResult &result = mRaySceneQuery->execute();

	//	Ogre::Vector3 targetpoint;

	//	targetpoint = mRay.getPoint(MAX_RANGED_RAYCAST);
	//	Ogre::Real targetDist = MAX_RANGED_RAYCAST;
	//	
	//	if(!result.empty())
	//	{
	//		for(RaySceneQueryResult::iterator itr = result.begin();itr != result.end();itr++)
	//		{
	//			if(itr->distance >= PICKUP_RANGE)
	//			{
	//				targetpoint = mRay.getPoint(itr->distance);
	//				targetDist = itr->distance;
	//				break;
	//			}
	//		}	
	//	}
	//	
	//	Ogre::Vector3 dirCenterTp = targetpoint - pos;
	//	Ogre::Vector3 dirHeadTp = targetpoint - posHead;
	//	Ogre::Vector3 relativeHeadPos = posHead - pos;
	//	//Ogre::Vector3 defaultDirection = relativeHeadPos+(Ogre::Vector3::NEGATIVE_UNIT_Z*targetDist);
	//	//mLevel->debugShowLine(pos,defaultDirection+pos);
	//	//mDebugText("relativeHeadPos="+str(relativeHeadPos));
	//	
	//	//defaultDirection.normalise();
	//	//mLevel->debugShowPoint(defaultDirection+pos);
	//	//dieser quaternion beschreibt nun nur die drehung von -Z in die richtige Richtung
	//	Ogre::Quaternion q_y = getOrientation();// dirHeadTp.getRotationTo(dirCenterTp);
	//	//und dieser nur die Neigung
	//	Ogre::Vector3 playerDir = q_y*Ogre::Vector3::NEGATIVE_UNIT_Z;
	//	Ogre::Quaternion q = playerDir.getRotationTo(dirCenterTp);//defaultDirection.getRotationTo(dirCenterTp);// dirHeadTp.getRotationTo(dirCenterTp);
	//	ornt = q*q_y;//*ornt;
	//	
	//}
	Ogre::Real targetDist;
	Ogre::Vector3 targetPoint;
	Ogre::Quaternion firingOrnt;
	getTargetData(firingOrnt, targetPoint, targetDist);
	return firingOrnt;
}


void GameChar::getTargetData(Ogre::Quaternion &firingOrnt, Ogre::Vector3 &targetPoint, Ogre::Real &targetDist)
{
	firingOrnt=/*getFiringOrientation();*/camNode->_getDerivedOrientation();
	//mBody->getPositionOrientation(pos,ornt);
	//ornt = 
	//Ogre::Vector3 v0 = mBody->getVelocity();
	
	Ogre::Vector3 pos = getPosition();
	if(playerControlled)// && false)
	{
		//Ogre::Ray mRay = getLookingRay();
		//Ogre::Quaternion q =Ogre::Vector3::NEGATIVE_UNIT_Z.getRotationTo(mRay.getDirection());// dirHeadTp.getRotationTo(dirCenterTp);
		
		Ogre::Vector3	posHead = camNode->_getDerivedPosition();
		//Ogre::Ray mRay = getLookingRay();
		//RaySceneQuery *	mRaySceneQuery = mLevel->getSceneManager()->createRayQuery(mRay);
		//mRaySceneQuery->setSortByDistance(true);
		//RaySceneQueryResult &result = mRaySceneQuery->execute();

		////Ogre::Vector3 targetpoint;

		//targetPoint = mRay.getPoint(MAX_RANGED_RAYCAST);
		//targetDist = MAX_RANGED_RAYCAST;
		//
		//if(!result.empty())
		//{
		//	for(RaySceneQueryResult::iterator itr = result.begin();itr != result.end();itr++)
		//	{
		//		if(itr->distance >= PICKUP_RANGE)
		//		{
		//			targetPoint = mRay.getPoint(itr->distance);
		//			targetDist = itr->distance;
		//			break;
		//		}
		//	}	
		//}
		//
		LookAtResult res = getLookingAt(MAX_RANGED_RAYCAST);
		targetPoint = res.hitPoint;
		targetDist = res.objDist;

		Ogre::Vector3 dirCenterTp = targetPoint - pos;
		Ogre::Vector3 dirHeadTp = targetPoint - posHead;
		Ogre::Vector3 relativeHeadPos = posHead - pos;
		//Ogre::Vector3 defaultDirection = relativeHeadPos+(Ogre::Vector3::NEGATIVE_UNIT_Z*targetDist);
		//mLevel->debugShowLine(pos,defaultDirection+pos);
		//mDebugText("relativeHeadPos="+str(relativeHeadPos));
		
		//defaultDirection.normalise();
		//mLevel->debugShowPoint(defaultDirection+pos);
		//dieser quaternion beschreibt nun nur die drehung von -Z in die richtige Richtung
		Ogre::Quaternion q_y = getOrientation();// dirHeadTp.getRotationTo(dirCenterTp);
		//und dieser nur die Neigung
		Ogre::Vector3 playerDir = q_y*Ogre::Vector3::NEGATIVE_UNIT_Z;
		Ogre::Quaternion q = playerDir.getRotationTo(dirCenterTp);//defaultDirection.getRotationTo(dirCenterTp);// dirHeadTp.getRotationTo(dirCenterTp);
		firingOrnt = q*q_y;//*ornt;
		
	}
}

void GameChar::spawnMissile(gamedata_damagedealer *what, bool resetRepeat)
{
	
	//na gut
	//ich raycaste also bis zu einem best. MAX_RANGED,
	//wenn ich was finde, berechne ich die Neigung, mit der ich das missile vom unteren Bildrand aus ab-
	//feuern muss damit es sich beim obj trifft, und wenn nicht, wirds halt parallel gespawned...
	/*
	ist aber eigentlich ne rein kosmetische sache. NPCs können direkt von der headnode oder körpermitte
	aus schießen
	/¯\
	| | |------------------------------------------->
	| |_____________------------------¯¯¯¯¯¯¯¯¯¯¯¯¯¯
	| |
	\_/
	
	*/
	Ogre::Vector3 pos=pos = getPosition();//camNode->_getDerivedPosition();
	Ogre::Quaternion ornt=/*getFiringOrientation();*/camNode->_getDerivedOrientation();
	//mBody->getPositionOrientation(pos,ornt);
	//ornt = 
	//Ogre::Vector3 v0 = mBody->getVelocity();
	//

	//if(playerControlled)// && false)
	//{
	//	//Ogre::Ray mRay = getLookingRay();
	//	//Ogre::Quaternion q =Ogre::Vector3::NEGATIVE_UNIT_Z.getRotationTo(mRay.getDirection());// dirHeadTp.getRotationTo(dirCenterTp);
	//	pos = getPosition();
	//	Ogre::Vector3	posHead = camNode->_getDerivedPosition();
	//	Ogre::Ray mRay = getLookingRay();
	//	RaySceneQuery *	mRaySceneQuery = mLevel->getSceneManager()->createRayQuery(mRay);
	//	mRaySceneQuery->setSortByDistance(true);
	//	RaySceneQueryResult &result = mRaySceneQuery->execute();

	//	Ogre::Vector3 targetpoint;

	//	targetpoint = mRay.getPoint(MAX_RANGED_RAYCAST);
	//	Ogre::Real targetDist = MAX_RANGED_RAYCAST;
	//	
	//	if(!result.empty())
	//	{
	//		for(RaySceneQueryResult::iterator itr = result.begin();itr != result.end();itr++)
	//		{
	//			if(itr->distance >= PICKUP_RANGE)
	//			{
	//				targetpoint = mRay.getPoint(itr->distance);
	//				targetDist = itr->distance;
	//				break;
	//			}
	//		}	
	//	}
	//	
	//	Ogre::Vector3 dirCenterTp = targetpoint - pos;
	//	Ogre::Vector3 dirHeadTp = targetpoint - posHead;
	//	Ogre::Vector3 relativeHeadPos = posHead - pos;
	//	Ogre::Vector3 defaultDirection = relativeHeadPos+(Ogre::Vector3::NEGATIVE_UNIT_Z*targetDist);
	//	//mLevel->debugShowLine(pos,defaultDirection+pos);
	//	//mDebugText("relativeHeadPos="+str(relativeHeadPos));
	//	
	//	//defaultDirection.normalise();
	//	//mLevel->debugShowPoint(defaultDirection+pos);
	//	//dieser quaternion beschreibt nun nur die drehung von -Z in die richtige Richtung
	//	Ogre::Quaternion q_y = getOrientation();// dirHeadTp.getRotationTo(dirCenterTp);
	//	//und dieser nur die Neigung
	//	Ogre::Vector3 playerDir = q_y*Ogre::Vector3::NEGATIVE_UNIT_Z;
	//	Ogre::Quaternion q = playerDir.getRotationTo(dirCenterTp);//defaultDirection.getRotationTo(dirCenterTp);// dirHeadTp.getRotationTo(dirCenterTp);
	//	ornt = q*q_y;//*ornt;
	//	
	//}

	ornt = getFiringOrientation();
	if(what->Missile->multimissile_delay > 0)
	{
		//dh hier verzögerte missiles
		timeSinceLastMissile = 0;

		missileToSpawn = what;
		if(resetRepeat)
		{
			//dh wir resetten den counter
			missileToSpawnRepeat = what->Missile->repeat;
		}
		else
		{
//		effectsToSpawn = eff;
//		dmgToSpawn = dmg;
			missileToSpawnRepeat--;
		}
	}
	if(what->Missile->count > 1)
	{
		if(what->Missile->multimissile_angle == Ogre::Degree(0))
		{
			//erstmal nur 1 abfeuern
			pos += ornt * Ogre::Vector3(0,0,static_cast<gamedata_char*>(mData)->cData.collistionParams.z*-3);
			MissileObject *missile = new MissileObject(this,what,mLevel,pos,ornt);
			//missile->effects = eff;
			//missile->dmg = dmg;
			

		}
		else
		{
			//das ist der startwinkel. gesamtwinkel/2
			Ogre::Degree curAngle = (what->Missile->count-1)*what->Missile->multimissile_angle/2;
			Ogre::Vector3 mPos = pos;
			//der aktuelle winkel
			
			
			for(unsigned int i=0;i<what->Missile->count;i++)
			{
				Ogre::Quaternion q(-curAngle+what->Missile->multimissile_angle*i,Ogre::Vector3::UNIT_Y);
				mPos = pos + ornt*q * Ogre::Vector3(0,0,static_cast<gamedata_char*>(mData)->cData.collistionParams.z*-3);
				MissileObject *missile = new MissileObject(this,what,mLevel,mPos,ornt*q);
				//missile->effects = eff;
				//missile->dmg = dmg;
				//Ogre::Quaternion q2(startAngle*i,Ogre::Vector3::UNIT_Y);
				//q = q * q2;
			}
			

		}
	}
	else
	{
		pos += ornt * Ogre::Vector3(0,0,static_cast<gamedata_char*>(mData)->cData.collistionParams.z*-3);
		MissileObject *missile = new MissileObject(this,what,mLevel,pos,ornt);
		//missile->effects = eff;
		//missile->dmg = dmg;
	}
}

void GameChar::updateSpells(Ogre::Real time)
{
	for(SpellList::const_iterator itr = spellsToDelete.begin();itr!=spellsToDelete.end();itr++)
	{
		SpellObject *cur = *itr;
		RemoveSpellFromActive(cur);
		delete (*itr);
	}
	spellsToDelete.clear();
	for(SpellList::iterator itr = activeSpells.begin();itr!=activeSpells.end();itr++)
	{
		(*itr)->update(time);
	}
	//alte löschen
//	for(EffectList::iterator itr = effectsToDelete.begin();itr!=effectsToDelete.end();itr++)
//	{
//		SpellEffect *cur = *itr;
//		delete cur;
//	}
//	effectsToDelete.clear();
//	for(EffectList::iterator itr = activeEffects.begin();itr!=activeEffects.end();itr++)
//	{
//		SpellEffect *cur = *itr;
////		cur->update(time);
//	}
}


void GameChar::Die()
{
#ifndef __editor
//	mDebugText("TOT!!!!");
	dead = true;
	mStats.hitpoints = 0;
	missileToSpawnRepeat = 0;
	missileToSpawn = NULL;
	setMoveJump(false);
	action = false;
	moveStop();
	attacking = false;
	spellcasting = false;
	setRandomAnim(AT_DIE);
	GameApp *app = static_cast<GameApp*>(StandardApplication::getSingletonPtr());
	if(mAI)
	{
		mAI->Die();
	}
	if(this == mLevel->getPlayer())
	{
		
		app->gameOver();
	}
#endif
}

void GameChar::AddSpell(gamedata_spell *spell,GameObject *caster)
{
	if(!spell)
		return;
	SpellObject *sp = NULL;
	try
	{
		sp = new SpellObject(spell,caster,this);
	}
	catch(ExceptionType)
	{
		sp = NULL;
	}
	if(sp)
	{
		activeSpells.push_back(sp);
	}
	//activeSpells
}

void GameChar::RemoveSpell(gamedata_spell *spell)
{
	SpellObject *so = NULL;
	for(SpellList::iterator itr = activeSpells.begin();itr != activeSpells.end();itr++)
	{
		if((*itr)->getData() == spell)
		{
			so = *itr;
			//activeSpells.erase(itr);
			break;
		}
	}
	if(so)
		spellsToDelete.push_back(so);
}
void GameChar::RemoveSpell(SpellObject *spell)
{
	/*for(SpellList::iterator itr = activeSpells.begin();itr != activeSpells.end();itr++)
	{
		if((*itr) == spell)
		{
			activeSpells.erase(itr);
			break;
		}
	}*/
	spellsToDelete.push_back(spell);
}

void GameChar::RemoveSpellFromActive(gamedata_spell *spell)
{
	for(SpellList::iterator itr = activeSpells.begin();itr != activeSpells.end();itr++)
	{
		if((*itr)->getData() == spell)
		{
			activeSpells.erase(itr);
			break;
		}
	}
}

void GameChar::RemoveSpellFromActive(SpellObject *spell)
{
	for(SpellList::iterator itr = activeSpells.begin();itr != activeSpells.end();itr++)
	{
		if((*itr) == spell)
		{
			activeSpells.erase(itr);
			break;
		}
	}
}