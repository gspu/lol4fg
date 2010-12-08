#include "GameItem.h"
#include "StandardApplication.h"
#include "functions.h"
#include "defines.h"
#include "Level.h"
#include "inventory.h"
#include "GameChar.h"
#ifndef __editor
#include "GameApp.h"
#endif

void GameItem::init(gamedata_item *data, Level *lvl, Ogre::Vector3 pos, Ogre::Quaternion ornt, Ogre::Vector3 _scale,int SaveGameID)
{
	isInLevelFile = false;
	SGID = SaveGameID;
	mNode = NULL;
	mArt = NULL;
	mData = data;
	
	
	mLevel = lvl;
	type = otItem;
	Ogre::SceneManager *mSceneMgr = mLevel->getSceneManager();

	ID = data->id;
	
	baseScale = data->wData.scale;
	mArt = new WorldArt(this,data->wData,NULL,_scale);
	//	createWorldArt(myData->wData);
	mColData = data->cData;
	createBody(data->cData);

	
	mBody->setCustomForceAndTorqueCallback( boost::bind( &GameObject::forceCallback, this, _1 ) );  // add the previous defined callback function as the body custom force and torque callback
	//mBody->setUserData(this);
	mBody->setPositionOrientation(pos,ornt);
//	uName = mNode->getName();
	mLevel->registerObject(this);
	

}

GameItem::GameItem(TiXmlElement *XMLNode,Level *lvl, Ogre::Vector3 positionOffset)
{
	Ogre::String id = "";
	Ogre::Vector3 pos = Ogre::StringConverter::parseVector3(XMLNode->Attribute("pos"))+positionOffset;
	Ogre::Vector3 scale = Ogre::Vector3::UNIT_SCALE;
	Ogre::Quaternion ornt = Ogre::Quaternion::IDENTITY;
	/*Ogre::String filename = XMLNode->Attribute("filename");
	Ogre::String material = "";*/
	bool hasCollision = true;			
	
	if(XMLNode->Attribute("orient"))
	{
		ornt = Ogre::StringConverter::parseQuaternion(XMLNode->Attribute("orient"));
	}
	
	if(XMLNode->Attribute("id"))
	{
		id = XMLNode->Attribute("id");
	}
	if(XMLNode->Attribute("scale"))
	{
		scale = Ogre::StringConverter::parseVector3(XMLNode->Attribute("scale"));
	}
	if(XMLNode->Attribute("SGID"))
	{
		SGID = Ogre::StringConverter::parseInt(XMLNode->Attribute("SGID"));
	}
	Ogre::Vector3 velocity;
	Ogre::Vector3 omega;
	
	gamedata_item *mItemData = StandardApplication::getSingletonPtr()->getItemDataPtr(id);
	if(!mItemData)
		throw etInvalidId;
	init(mItemData,lvl,pos,ornt,scale,SGID);

	if(XMLNode->Attribute("inlevelfile"))
	{
		isInLevelFile = Ogre::StringConverter::parseBool(XMLNode->Attribute("inlevelfile"));
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

TiXmlElement* GameItem::getXmlElem(Ogre::Vector3 startPosition,bool savegame)
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
	xmlElem->SetAttribute("inlevelfile",Ogre::StringConverter::toString(isInLevelFile).c_str());

	if(scale != Ogre::Vector3::UNIT_SCALE)
	{
		xmlElem->SetAttribute("scale",Ogre::StringConverter::toString(scale).c_str());
	}
	xmlElem->SetAttribute("id",ID.c_str());

	
	return xmlElem;
}


GameItem::GameItem(GameItem *src,Level *lvl)
{
	Level *mLvl;
	if(lvl)
	{
		mLvl = lvl;
	}
	else
	{
		mLvl = src->mLevel;
	}
	
	/*mNode = NULL;
	mArt = NULL;
	
	baseScale = src->baseScale;

	StandardApplication *app = StandardApplication::getSingletonPtr();	
	mData = src->mData;*/
	
	//type = otItem;
	//Ogre::SceneManager *mSceneMgr = mLevel->getSceneManager();

	//ID = src->ID;
	//
	//mArt = new WorldArt(src->mArt,this);
	//mNode = mArt->getSceneNode();

	//mColData = src->mColData;
	//createBody(mColData);

	//
	//mBody->setCustomForceAndTorqueCallback<Level>(&Level::LevelForceCallback,mLevel);  // add the previous defined callback function as the body custom force and torque callback
	////mBody->setUserData(this);
	Ogre::Vector3 pos;
	Ogre::Quaternion ornt;
	Ogre::Vector3 _scale = src->getScale();
	src->mBody->getPositionOrientation(pos,ornt);
	init(static_cast<gamedata_item*>(src->mData),mLvl,pos,ornt,_scale);
	//mBody->setPositionOrientation(pos,ornt);
//	uName = mNode->getName();
	//mLevel->registerObject(this);
	

}

GameItem::~GameItem()
{
	//mLevel->unregisterObject(this);
	//delete mBody;
	////destroyNode(mLevel->getSceneManager(),mNode);
	//if(mArt)
	//	delete mArt;
	//mLevel->getSceneManager()->destroySceneNode(mNode->getName());
}

Ogre::String GameItem::getName()
{
	return static_cast<gamedata_item*>(mData)->name;
}
void GameItem::onActivate(GameChar *user,bool cursorMode)
{
#ifndef __editor
	if(!user)
		return;
	if(!user->getInventory())
		return;
	if(cursorMode)
	{
		Inventory::invDataItem cur;
		cur.count = 1;
		cur.item = static_cast<gamedata_item*>(mData);
		static_cast<GameApp*>(StandardApplication::getSingletonPtr())->setHoldingItem(cur);
		//app->setholdingitem(cur);
		mLevel->deleteObject(this);
		//delete obj;
	}
	else
	{
		if(user->getInventory()->hasSpace())
		{
			user->getInventory()->addItem(ID);
			mLevel->deleteObject(this);
		}
	}
#endif
}