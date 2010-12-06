#include "DoorObject.h"
#include "GameObject.h"
#include "StandardApplication.h"
#include "GameChar.h"
#ifndef __editor
#include "GameApp.h"
#include <OgreNewt_Collision.h>
#endif


#include "functions.h"
DoorObject::DoorObject(DoorObject *src, Level *targetLvl)
{
	SGID = 0;
	mNode = NULL;
	mArt = NULL;
	mDoorData = src->mDoorData;
	mData = src->mData;
	type = otDoor;
	if(targetLvl)
	{
		mLevel = targetLvl;
	}
	else
	{
		mLevel = src->mLevel;
	}
	mName = src->mName;
	baseScale = src->baseScale;
	ID = src->ID;
	mArt = new WorldArt(src->mArt,this);
	mNode = mArt->getSceneNode();
	baseScale = src->baseScale;
	mColData = src->mColData;
	Ogre::Real mass;
	Ogre::Vector3 inertia;
	
	const OgreNewt::CollisionPtr col = createCollision(mColData,inertia);//src->mBody->getCollision();

	mBody = new OgreNewt::Body(mLevel->getWorld(),col);
	
//	delete col;
	
	src->mBody->getMassMatrix(mass,inertia);
	mBody->setMassMatrix(mass, inertia);

	if(mass != 0)
		//mBody->setCustomForceAndTorqueCallback( boost::bind( &GameObject::forceCallback, this, _1 ) );  // add the previous defined callback function as the body custom force and torque callback
		mBody->setCustomForceAndTorqueCallback( boost::bind( &GameObject::forceCallback, this, _1 ) );
	mBody->setUserData(Ogre::Any((GameObject*)this));
	Ogre::Vector3 pos;
	Ogre::Quaternion ornt;
	src->mBody->getPositionOrientation(pos,ornt);
	mBody->setPositionOrientation(pos,ornt);
	
	mBody->attachNode(mNode);

	useOnCol = src->useOnCol;

	mLevel->registerObject(this);
	collided = NULL;

}

//DoorObject::DoorObject(Ogre::String id, Level *lvl, /*DoorData addData,*/ Ogre::Vector3 pos, 
//					 Ogre::Quaternion ornt, Ogre::Vector3 _scale)
//{
//	gamedata_door *myData = app->getDoorDataPtr(id);
//	if(myData.id == "")
//	{
//		throw etInvalidId;
//	}
//	init(myData,lvl,pos,ornt,_scale);
//}

TiXmlElement* DoorObject::getXmlElem(Ogre::Vector3 startPosition,bool savegame)
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
	xmlElem->SetAttribute("id",ID.c_str());
	if(scale != Ogre::Vector3::UNIT_SCALE)
	{
		xmlElem->SetAttribute("scale",Ogre::StringConverter::toString(scale).c_str());
	}
	

	
	
	xmlElem->SetAttribute("targetLevel",mDoorData.targetLevel.c_str());
	xmlElem->SetAttribute("targetEntrance",mDoorData.targetEntrance.c_str());

		
	return xmlElem;
}

DoorObject::DoorObject(TiXmlElement *XMLNode, Level *lvl,Ogre::Vector3 positionOffset)
{
	Ogre::String id = "";
	if(XMLNode->Attribute("id"))
	{
		id = XMLNode->Attribute("id");
	}
	
	gamedata_door *mData = StandardApplication::getSingletonPtr()->getDoorDataPtr(id);
	if(!mData)
	{
		throw etInvalidId;
	}
	Ogre::Vector3 pos = Ogre::StringConverter::parseVector3(XMLNode->Attribute("pos"))+positionOffset;
	Ogre::Vector3 scale = Ogre::Vector3::UNIT_SCALE;
	Ogre::Quaternion ornt = Ogre::Quaternion::IDENTITY;
	int SGID = 0;
	bool hasCollision = true;			
	
	if(XMLNode->Attribute("orient"))
	{
		ornt = Ogre::StringConverter::parseQuaternion(XMLNode->Attribute("orient"));
	}
	if(XMLNode->Attribute("SGID"))
	{
		SGID = Ogre::StringConverter::parseInt(XMLNode->Attribute("SGID"));
	}
	
	if(XMLNode->Attribute("scale"))
	{
		scale = Ogre::StringConverter::parseVector3(XMLNode->Attribute("scale"));
	}
	init(mData,lvl,pos,ornt,scale,SGID);
	
	mDoorData.targetEntrance = "";
	mDoorData.targetLevel = "";
	if(XMLNode->Attribute("targetEntrance"))
	{
		mDoorData.targetEntrance = XMLNode->Attribute("targetEntrance");
	}
	
	if(XMLNode->Attribute("targetLevel"))
	{
		mDoorData.targetLevel = XMLNode->Attribute("targetLevel");
	}
	
	
	
}

void DoorObject::init(gamedata_door *myData,Level *lvl,Ogre::Vector3 pos, Ogre::Quaternion ornt,Ogre::Vector3 _scale,int sgid)
{
	isInLevelFile = true;
	SGID = sgid;
	//mAddData = addData;
	mDoorData.targetEntrance = "";
	mDoorData.targetLevel = "";
	type = otDoor;
	mNode = NULL;
	mArt = NULL;
	mScale = _scale;
	mData = myData;
	
	
	/*if(app->datamap_door.find(id) == app->datamap_door.end())
	{
		throw etInvalidId;
	}
	gamedata_door myData = app->datamap_door[id];*/
	
	
	mLevel = lvl;

	mName = myData->name;
	baseScale = myData->wData.scale;



	Ogre::SceneManager *mSceneMgr = mLevel->getSceneManager();

	
//	Ogre::String mesh = myData.mesh;

	ID = myData->id;
	
	/*if(_scale != Ogre::Vector3::UNIT_SCALE)
	{
		scale = _scale;
	}*/
	//createNodeAndEntity(mesh,myData.material,QM_ACTION);
	try
	{
		mArt = new WorldArt(this,myData->wData,NULL,_scale);
	}
	catch(ExceptionType)
	{
		mArt = NULL;
	}
	mColData = myData->cData;
	//createWorldArt(myData.wData);
	createBody(myData->cData);

/*
	mNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	//===========================================
	//das evtl noch beim objekte einlesen machen
	if(Ogre::StringUtil::startsWith(mesh,"!plane"))
	{
		if(!createPlaneMesh(mesh))
		{
			throw Ogre::Exception(0,"could not create mesh "+mesh,"DoorObject constructor");
		}

	}
	//===========================================
	Ogre::String entName = "ent_"+id;//+"_"+Ogre::StringConverter::toString(i);
	for(int i = 0;mSceneMgr->hasEntity(entName);++i)
	{
		entName = "ent_"+id+Ogre::StringConverter::toString(i);
	}
	Ogre::Entity *ent = mSceneMgr->createEntity(entName,mesh);
	ent->setUserAny(Any((GameObject*)this));
	ent->setQueryFlags(QM_ACTION);
	if(myData.material != "")
	{
		ent->setMaterialName(myData.material);
		
	}
	mNode->attachObject(ent);
	*/
	//mNode->setScale(scale);
	//colType = myData.collisionType;
	//collisionParams = myData.collisionParams;
	////////app->log("scalewtf: "+Ogre::StringConverter::toString(scale));
	//mBody = makeBody(mLevel->getWorld(),myData.collisionType,myData.mass,myData.collisionParams,mNode);
	//if(myData.collisionType != CT_TREECOLLISION && myData.collisionType != CT_TREECOLLISIONSCENE)
	//	mBody->setCustomForceAndTorqueCallback<Level>(&Level::LevelForceCallback,mLevel);  // add the previous defined callback function as the body custom force and torque callback
	//mBody->setUserData(this);
	mBody->setPositionOrientation(pos,ornt);
//	uName = mNode->getName();
	mLevel->registerObject(this);
	useOnCol = myData->useOnCollision;
	collided = NULL;
}

DoorObject::~DoorObject()
{
	//mLevel->unregisterObject(this);
	//delete mBody;
	////destroyNode(mLevel->getSceneManager(),mNode);
	//if(mArt)
	//	delete mArt;
	//mLevel->getSceneManager()->destroySceneNode(mNode->getName());
	//erstmal nix
}

void DoorObject::onActivate(GameChar *user,bool cursorMode)
{
#ifndef __editor

	GameApp *app = (GameApp*)StandardApplication::getSingletonPtr();//->setDebugText("activate()=>\n"+mAddData.targetLevel+
		//"\n"+mAddData.targetEntrance);
	//erstmal nur beamen
	app->setLoadLevel(mDoorData.targetLevel,mDoorData.targetEntrance);
#endif
}

void DoorObject::onCollision(GameObject *with,ContCallback *cb)
{
#ifndef __editor
	if(!useOnCol || with->getType() != otChar)
		return;

	GameChar *user = static_cast<GameChar*>(with);
	if(user == collided)
		return;
	

	GameApp *app = (GameApp*)StandardApplication::getSingletonPtr();//->setDebugText("activate()=>\n"+mAddData.targetLevel+
	//ERSTMAL SO, DANN ANDERS!!1
	if(app->getCurrentLevel()->getPlayer() != user)
		return;
	collided = user;
	//erstmal nur beamen
	app->setLoadLevel(mDoorData.targetLevel,mDoorData.targetEntrance);
	
#endif
}

