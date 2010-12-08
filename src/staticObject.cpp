#include "StaticObject.h"
#include "StandardApplication.h"
#include "functions.h"
#include "Level.h"
//#include ""
using namespace Ogre;
void StaticObject::init(Level *lvl, WorldArtData wData, Ogre::Vector3 pos, Ogre::Quaternion ornt, Ogre::Vector3 scale,  bool _collision)
{
	isInLevelFile = true;
	mLevel = lvl;
	SGID = 0;
	type = otStatic;

	Ogre::SceneManager *mSceneMgr = mLevel->getSceneManager();
	baseScale = Ogre::Vector3::UNIT_SCALE;


	mBody = NULL; 
	mNode = NULL;


	mArt = new WorldArt(this,wData);
	//createNodeAndEntity(ID,materialName);

	//mNode->setScale(scale);
	collision = _collision;
	if(collision)
	{		
		mColData.colType = CT_TREECOLLISION;		
		createBody(mColData);			
	}
	if(mBody)
		mBody->setPositionOrientation(pos,ornt);
	else
	{
		mColData.colType = CT_NOCOLLISION;
		mNode->setPosition(pos);
		mNode->setOrientation(ornt);
	}
	mLevel->registerObject(this);
}

void StaticObject::setWorldArtData(WorldArtData wData)
{
	Ogre::Vector3 pos, scale;
	Ogre::Quaternion ornt;
	getPositionOrientation(pos,ornt);
	scale = getScale();
	delete mArt;
	mArt = new WorldArt(this,wData);
	//regenerate collision
	if(collision)
	{		
		delete mBody;
		mColData.colType = CT_TREECOLLISION;		
		createBody(mColData);			
	}
	//if(mBody)
	//	mBody->setPositionOrientation(pos,ornt);
	//else
	//{
	//	mColData.colType = CT_NOCOLLISION;
	//	/*mNode->setPosition(pos);
	//	mNode->setOrientation(ornt);*/
	//}
	setPositionOrientation(pos,ornt);
	setScale(scale);
}


TiXmlElement* StaticObject::getXmlElem(Ogre::Vector3 startPosition,bool savegame)
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
	StandardApplication::getSingletonPtr()->worldArtToXml(mArt->getData(),xmlElem);//->SetValue(type.c_str());
	if(scale != Ogre::Vector3::UNIT_SCALE)
	{
		xmlElem->SetAttribute("scale",Ogre::StringConverter::toString(scale).c_str());
	}
	xmlElem->SetValue("static");
	return xmlElem;
	
}

//
//StaticObject::StaticObject(Level *lvl,TiXmlElement *elem)
//{
//	mLevel = lvl;
//	type = otStatic;
//
//	Ogre::SceneManager *mSceneMgr = mLevel->getSceneManager();
//
//	mBody = NULL;
//	mNode = NULL;
//
//
////	mArt = new WorldArt(this);
//	if(s_type == "dotscene")
//	{
//		mArt = new WorldArt(this,filename,scale);
//		//mArt->loadDotScene(filename,scale);
//	}
//	else if(s_type == "mesh")
//	{
//		mArt = new WorldArt(this,filename,material,scale);
//	}
//	else if(s_type == "billboard")
//	{
//		mArt = new WorldArt(this,material,scale.x,scale.y);
//
//	}
//	else if(s_type == "plane")
//	{
//		mArt = new WorldArt(material,this,1,1);
//	}
//	//createNodeAndEntity(ID,materialName);
//
//	//mNode->setScale(scale);
//	collision = _collision;
//	if(collision)
//	{
//		CollisionData d;
//		d.colType = CT_TREECOLLISION;
//		if(s_type == "dotscene")
//			d.colType = CT_TREECOLLISIONSCENE;
//		createBody(d);
////
//////		colType = CT_TREECOLLISION;
////		OgreNewt::Collision *col = new OgreNewt::CollisionPrimitives::TreeCollision(mLevel->getWorld(),mNode,false);
////		mBody = new OgreNewt::Body(mLevel->getWorld(),col);		
////		//mBody->setCollision(col);
////		delete col;
////		mBody->setUserData(this);
////		mBody->attachToNode(mNode);
//		mBody->setPositionOrientation(pos,ornt);
//		
//	}
//	else
//	{
//		mNode->setPosition(pos);
//		mNode->setOrientation(ornt);
//	}
//	mLevel->registerObject(this);
//
//}

StaticObject::~StaticObject()
{
	//mLevel->unregisterObject(this);
	//if(mBody)
	//	delete mBody;
	//
	//if(mArt)
	//	delete mArt;
	//destroyNode(mLevel->getSceneManager(),mNode);
	////mSceneMgr->destroySceneNode(mNode->getName());
}

StaticObject::StaticObject(TiXmlElement *XMLNode,Level *lvl,Ogre::Vector3 positionOffset)
{
	
   
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


	
	bool hasCollision = true;
							
	if(XMLNode->Attribute("collision"))
	{
		hasCollision = Ogre::StringConverter::parseBool(XMLNode->Attribute("collision"));
	}
	
	WorldArtData wData = StandardApplication::getSingletonPtr()->parseWorldArtData(XMLNode);
	init(lvl,wData,pos,ornt,scale,hasCollision);
}

StaticObject::StaticObject(StaticObject *src, Level *lvl)
{
	SGID = 0;
	baseScale = Ogre::Vector3::UNIT_SCALE;
	if(lvl)
	{
		mLevel = lvl;
	}
	else
	{
		mLevel = src->mLevel;
	}
	mNode = NULL;
	mArt = NULL;
	

	type = otStatic;

	Ogre::SceneManager *mSceneMgr = mLevel->getSceneManager();

	ID = src->ID;
	materialName = src->materialName;

	baseScale = Ogre::Vector3::UNIT_SCALE;
	//collisionParams = Ogre::Vector3::UNIT_SCALE;
	//colType = 0;

	mBody = NULL;
	mNode = NULL;


	
	//createNodeAndEntity(ID,materialName);
	mArt = new WorldArt(src->mArt,this);
	mNode = mArt->getSceneNode();
	//mNode = cloneSceneNode(mSceneMgr,src->mNode,0,Any((GameObject*)this));

	//mNode->setScale(src->mNode->getScale());
	collision = src->collision;
	Ogre::Vector3 pos; 
	Ogre::Quaternion ornt;
	src->getPositionOrientation(pos,ornt);
	mColData = src->mColData;
	if(collision)
	{
		OgreNewt::CollisionPtr col = mArt->getTreeCollision();
//		colType = CT_TREECOLLISION;
		//OgreNewt::Collision *col = new OgreNewt::CollisionPrimitives::TreeCollision(mLevel->getWorld(),mNode,false);
		mBody = new OgreNewt::Body(mLevel->getWorld(),col);		
		mBody->setUserData(Ogre::Any((GameObject*)this));
		
		mBody->attachNode(mNode);
		mBody->setPositionOrientation(pos,ornt);
		//delete col;
	}
	else
	{
		mNode->setPosition(pos);
		mNode->setOrientation(ornt);
	}
	mLevel->registerObject(this);
}
void StaticObject::setMesh(Ogre::String newMesh)
{
	Ogre::Entity *ent = static_cast<Ogre::Entity*>(mNode->getAttachedObject(0));
	mNode->detachObject(ent);
	Ogre::SceneManager *mgr = mLevel->getSceneManager();
	mgr->destroyEntity(ent);
	Ogre::String entName = getUniqueEntityName(mgr,"ent_"+ID);
	ent = mgr->createEntity(entName,newMesh);
	mNode->attachObject(ent);
	ent->setUserAny(Any((GameObject*)this));
	ID = newMesh;
	if(materialName != "")
	{
		ent->setMaterialName(materialName);
	}
	updateBody();
}
void StaticObject::setMaterial(Ogre::String newMaterial)
{
	materialName = newMaterial;
	if(materialName != "")
	{
		Ogre::Entity *ent = static_cast<Ogre::Entity*>(mNode->getAttachedObject(0));
		ent->setMaterialName(materialName);
	}
	else
	{
		setMesh(ID);
	}
}
void StaticObject::setCollision(bool enable)
{
	collision = enable;
	updateBody();

}
//void StaticObject::updateBody()
//{
////	Ogre::Vector3 pos; 
////	Ogre::Quaternion ornt;
////	getPositionOrientation(pos,ornt);
////	if(!mBody)
////	{
////		if(collision)
////		{
////			//kein Body da, sollte aber sein
//////			colType = CT_TREECOLLISION;
////			OgreNewt::Collision *col = new OgreNewt::CollisionPrimitives::TreeCollision(mLevel->getWorld(),mNode,false);
////			mBody = new OgreNewt::Body(mLevel->getWorld(),col);		
////			delete col;
////			mBody->setUserData(this);
////			mBody->attachToNode(mNode);
////			mBody->setPositionOrientation(pos,ornt);			
////		}
////	}
////	else
////	{
////		if(!collision)
////		{
////			//body da, soll aber nicht sein
//////			colType = 0;
////			mBody->attachToNode(NULL);
////			delete mBody;
////			mNode->setPosition(pos);
////			mNode->setOrientation(ornt);
////		}
////		else
////		{
////			//body da, soll auch sein, einfach updaten
////			OgreNewt::Collision *col = new OgreNewt::CollisionPrimitives::TreeCollision(mLevel->getWorld(),mNode,false);
////			mBody->setCollision(col);	
////			delete col;
////		}
////	}
//}