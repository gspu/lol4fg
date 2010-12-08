#include "waterplane.h"
#include "level.h"
#include "functions.h"
#include "contactCallback.h"
#include <TypeConverter.h>

using namespace TypeConverter;


WaterPlane::WaterPlane(TiXmlElement *XMLNode,Level *lvl, Ogre::Vector3 positionOffset)
{	
	Ogre::Vector3 pos = Ogre::StringConverter::parseVector3(XMLNode->Attribute("pos"))+positionOffset;
	Ogre::Vector3 scale = Ogre::Vector3::UNIT_SCALE;
	Ogre::Quaternion ornt = Ogre::Quaternion::IDENTITY;
    FluidData data;
		
	
	if(XMLNode->Attribute("orient"))
	{
		ornt = Ogre::StringConverter::parseQuaternion(XMLNode->Attribute("orient"));
	}	
	
	if(XMLNode->Attribute("scale"))
	{
		scale = Ogre::StringConverter::parseVector3(XMLNode->Attribute("scale"));
	}
    if(XMLNode->Attribute("material"))
	{
		data.material = XMLNode->Attribute("material");
	}    
	if(XMLNode->Attribute("sound"))
	{
		data.sound = XMLNode->Attribute("sound");
	}    
	if(XMLNode->Attribute("density"))
	{
		data.density = Ogre::StringConverter::parseReal(XMLNode->Attribute("density"));
	}    
	if(XMLNode->Attribute("linearViscosity"))
	{
		data.linearViscosity= Ogre::StringConverter::parseReal(XMLNode->Attribute("linearViscosity"));
	}    
	if(XMLNode->Attribute("angularViscosity"))
	{
		data.angularViscosity= Ogre::StringConverter::parseReal(XMLNode->Attribute("angularViscosity"));
	}    
	if(XMLNode->FirstChildElement("plane"))
	{
		data.plane= StandardApplication::getSingletonPtr()->parsePlaneData(XMLNode->FirstChildElement("plane"));
	}    

	
	init(lvl,pos,ornt,scale,data);
}


TiXmlElement* WaterPlane::getXmlElem(Ogre::Vector3 startPosition,bool savegame)
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
    if(scale != Ogre::Vector3::UNIT_SCALE)
	{
		xmlElem->SetAttribute("scale",Ogre::StringConverter::toString(scale).c_str());
	}
	
	xmlElem->SetAttribute("material",mFluidData.material);
	xmlElem->SetAttribute("sound",mFluidData.sound);
	xmlElem->SetAttribute("density",ogre_str(mFluidData.density));
	xmlElem->SetAttribute("linearViscosity",ogre_str(mFluidData.linearViscosity));
	xmlElem->SetAttribute("angularViscosity",ogre_str(mFluidData.angularViscosity));
	
	//planeElem->SetAttribute("normal",ogre_str(wData.pData.normal).c_str());
	TiXmlElement* planeElem = new TiXmlElement("plane");
	//planeElem->SetAttribute("constant",ogre_str(mFluidData.plane.constant).c_str());
	planeElem->SetAttribute("width",ogre_str(mFluidData.plane.width).c_str());
	planeElem->SetAttribute("height",ogre_str(mFluidData.plane.height).c_str());
	planeElem->SetAttribute("xSeg",ogre_str(mFluidData.plane.xSeg).c_str());
	planeElem->SetAttribute("ySeg",ogre_str(mFluidData.plane.ySeg).c_str());
	planeElem->SetAttribute("numTexCoordSets",ogre_str(mFluidData.plane.numTexCoordSets).c_str());
	planeElem->SetAttribute("uTile",ogre_str(mFluidData.plane.uTile).c_str());
	planeElem->SetAttribute("vTile",ogre_str(mFluidData.plane.vTile).c_str());
	planeElem->SetAttribute("upVector",ogre_str(mFluidData.plane.up).c_str());
	xmlElem->LinkEndChild(planeElem);
	
	
	
	//xmlElem->SetValue("water");
	return xmlElem;
	
}


void WaterPlane::init(Level *lvl,Ogre::Vector3 position, Ogre::Quaternion orientation, Ogre::Vector3 scale, FluidData data)
{
    destructorCalled = false;
    //material = _material;
	mFluidData = data;
    isInLevelFile = true;
	mLevel = lvl;
	SGID = 0;
	type = otWater;

	Ogre::SceneManager *mSceneMgr = mLevel->getSceneManager();
	baseScale = Ogre::Vector3::UNIT_SCALE;
    //baseScale.y = 0;


	mBody = NULL;
	mNode = NULL;

    WorldArtData wData;
    wData.type = WT_PLANE;
	wData.material=data.material;
	wData.pData = data.plane;
    wData.pData.normal = Ogre::Vector3::UNIT_Y;
	wData.pData.constant = 0;
	wData.pData.up = Ogre::Vector3::UNIT_Z;

	mArt = new WorldArt(this,wData,NULL,scale);
	
    mNode = mArt->getSceneNode();//mLevel->getSceneManager()->getRootSceneNode()->createChildSceneNode();
    //waterMesh = new WaterMesh(ptrAsStr()+"_watermesh",scale,64);
//    mEnt = mLevel->getSceneManager()->createEntity(ptrAsStr()+"_entity",waterMesh->getMeshName());
 //   mEnt->setMaterialName(data.material);
    //Ogre::SceneNode *offsetNode = mNode->createChildSceneNode(-Ogre::Vector3(scale.x,0,scale.z)/2);

   // mNode->attachObject(mEnt);
   // mEnt->MovableObject::setUserAny(Any((GameObject*)this));

    mColData.colType = CT_BOX;
    mColData.collistionParams = scale;
    mColData.collistionParams.y=0.1;
    mColData.positionOffset.y=-0.05;
		
    OgreNewt::CollisionPtr col = OgreNewt::CollisionPtr(new OgreNewt::CollisionPrimitives::Box(lvl->getWorld(),mColData.collistionParams,0,Ogre::Quaternion::IDENTITY,mColData.positionOffset));
    mBody = new OgreNewt::Body(lvl->getWorld(),col,type);
    mBody->setMassMatrix(0,Ogre::Vector3::UNIT_SCALE);
	mBody->setUserData(Ogre::Any((GameObject*)this));
	//createBody(mColData);
	
    mBody->attachNode(mNode);
	mBody->setPositionOrientation(position,orientation);
	updateBody();
	mLevel->registerObject(this);
   // waterMesh->push(5,5,20);


}



WaterPlane::~WaterPlane()
{

    destructorCalled = true;
//    delete waterMesh;
    ObjectList::iterator itr;
	//omfg... durchgehen und suchen
    //size_t zomg = collidingWithMe.size();
    //if(collidingWithMe.size() > 0)
    //{
    for(itr = collidingWithMe.begin();itr != collidingWithMe.end();itr++)
    {
	    GameObject *cur = *itr;
		
        if(cur->getCollidingWater() == this)
            cur->collisionWithWater(NULL);
    }
    //}
}

void WaterPlane::update(Ogre::Real time)
{
//    waterMesh->updateMesh(time);
}

void WaterPlane::updateBody()
{
    Ogre::Vector3 pos;
    Ogre::Quaternion ornt;
    getPositionOrientation(pos,ornt);
    Ogre::Vector3 normal= ornt*Ogre::Vector3::UNIT_Y;
    mPlane = Ogre::Plane(normal,pos);
}

void WaterPlane::onCollision(GameObject *with,ContCallback *callback)
{
 //   Ogre::Vector3 pos, norm;
 //   callback->getContactPositionAndNormal(pos,norm);
 //       
 //   Ogre::Vector3 myPos = getPosition();
 //   pos -= myPos;
 //   pos -= getScale()/2;
 //   float x = pos.x / getScale().x * 64 ;
	//float z = pos.z / getScale().z * 64 ;
 //   mDebugText("x="+ogre_str(x)+", z="+ogre_str(z));
    
}



void WaterPlane::addColliding(GameObject *obj)
{
    if(destructorCalled)
        return;
    ObjectList::iterator itr;
	//omfg... durchgehen und suchen
	for(itr = collidingWithMe.begin();itr != collidingWithMe.end();itr++)
	{
		GameObject *cur = *itr;
		
		if(cur == obj)
		{			
			return;
		}
	}
    collidingWithMe.push_back(obj);
}

void WaterPlane::removeColliding(GameObject *obj)
{
    if(destructorCalled)
        return;
    ObjectList::iterator itr;
	//omfg... durchgehen und suchen
	for(itr = collidingWithMe.begin();itr != collidingWithMe.end();itr++)
	{
		GameObject *cur = *itr;
		
		if(cur == obj)
		{
			collidingWithMe.erase(itr);
			return;
		}
	}
}

void WaterPlane::setFluidData(FluidData newData)
{
	mFluidData = newData;
	//re-creating worldart
	Ogre::Vector3 scale = mArt->getScale();
	delete mArt;//destroy it
	WorldArtData wData;
    wData.type = WT_PLANE;
	wData.material=mFluidData.material;
	wData.pData = mFluidData.plane;
    wData.pData.normal = Ogre::Vector3::UNIT_Y;
	wData.pData.constant = 0;
	wData.pData.up = Ogre::Vector3::UNIT_Z;

	mArt = new WorldArt(this,wData,NULL,scale);
	
    mNode = mArt->getSceneNode();

}