#include "DynamicObject.h"
#include "StandardApplication.h"
#include "functions.h"
#include "Level.h"

DynamicObject::DynamicObject(DynamicObject *src, Level *lvl)
{
    Level *L;
	if(lvl)
	{
		L = lvl;
	}
	else
	{
		L = src->mLevel;
	}
    Ogre::Vector3 pos;
	Ogre::Quaternion ornt;
	src->mBody->getPositionOrientation(pos,ornt);
    init(static_cast<gamedata_dynamic*>(src->mData),L,pos,ornt,src->getScale());
}

DynamicObject::DynamicObject(TiXmlElement *XMLNode,Level *lvl, Ogre::Vector3 positionOffset )
{
	
    Ogre::String id = "";
	Ogre::Vector3 pos = Ogre::StringConverter::parseVector3(XMLNode->Attribute("pos"))+positionOffset;
	Ogre::Vector3 scale = Ogre::Vector3::UNIT_SCALE;
	Ogre::Quaternion ornt = Ogre::Quaternion::IDENTITY;
	int SGID = 0;
		
	
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
	gamedata_dynamic *mSpData = StandardApplication::getSingletonPtr()->getDynamicDataPtr(id);
	if(!mSpData)
		throw etInvalidId;
	if(XMLNode->Attribute("SGID"))
	{
		SGID = Ogre::StringConverter::parseInt(XMLNode->Attribute("SGID"));
	}
	init(mSpData,lvl,pos,ornt,scale,SGID);
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

TiXmlElement* DynamicObject::getXmlElem(Ogre::Vector3 startPosition,bool savegame)
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
	if(scale != Ogre::Vector3::UNIT_SCALE)
	{
		xmlElem->SetAttribute("scale",Ogre::StringConverter::toString(scale).c_str());
	}
	xmlElem->SetAttribute("inlevelfile",Ogre::StringConverter::toString(isInLevelFile).c_str());
	if(savegame && mBody && !mBody->isFreezed())
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
	xmlElem->SetAttribute("id",ID.c_str());
	return xmlElem;
	
}

void DynamicObject::init(gamedata_dynamic *data, Level *lvl, Ogre::Vector3 pos, Ogre::Quaternion ornt, Ogre::Vector3 _scale,int SaveGameID)
{
	isInLevelFile = false;
	SGID = SaveGameID;
	mNode = NULL;
	mArt = NULL;
	mLevel = lvl;
	//SubObjectList sTest(data->subObjects.size());
	/*subObjectList = new SubObjectList();
	jointList = new JointList();*/
	//typedef std::vector<OgreNewt::Joint*> JointList;

	
	type = otDynamic;
    mData = data;
	//Ogre::SceneManager *mSceneMgr = mLevel->getSceneManager();

	/*mNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();*/
	/*Ogre::String mesh = myData.mesh;
	baseScale = myData.scale;*/

	ID = data->id;
	/*Ogre::Vector3 scale = baseScale*_scale;
	createNodeAndEntity(mesh,myData.material);

	mNode->setScale(scale);
	colType = myData.collisionType;
	collisionParams = myData.collisionParams;
	mBody = makeBody(mLevel->getWorld(),myData.collisionType,myData.mass,myData.collisionParams,mNode);*/
//	createWorldArt(myData.wData);
	baseScale = data->wData.scale;
	

	mArt = new WorldArt(this,data->wData,NULL,_scale);
	mColData = data->cData;
	createBody(data->cData,false);
	if(mBody)
    {
        if(data->cData.colType != CT_TREECOLLISION && data->cData.colType != CT_EXTERNAL)
		    mBody->setCustomForceAndTorqueCallback( boost::bind( &GameObject::forceCallback, this, _1 ) );  // add the previous defined callback function as the body custom force and torque callback
	    //mBody->setUserData(this);
	    mBody->setPositionOrientation(pos,ornt);
        //jetzt die sub.objekte!
	    std::vector<gamedata_dynamic>::const_iterator itr;
	    for(itr = data->subObjects.begin();itr != data->subObjects.end();itr++)
	    {
		    processSubObject(*itr,mBody,_scale);
	    }
    }
	

	mLevel->registerObject(this);
}

void DynamicObject::processSubObject(gamedata_dynamic data, OgreNewt::Body *parent,Ogre::Vector3 scale)
{
	Ogre::SceneNode* nod = mLevel->getSceneManager()->getRootSceneNode()->createChildSceneNode();
	
	WorldArt *art = new WorldArt(this,data.wData,nod,scale);
	//art->setScale(baseScale/scale);

	OgreNewt::World *mWorld = mLevel->getWorld();
	OgreNewt::CollisionPtr col;
	Ogre::Vector3 inertia;
	Ogre::Real mass = data.cData.mass;
	CollisionData newCData = data.cData;
	newCData.collistionParams;// *= baseScale/scale;
	col = createCollision(newCData,inertia);
	OgreNewt::Body *bod = new OgreNewt::Body(mWorld,col,type);
	
	if(mass != 0)
	{
		bod->setMassMatrix(mass,inertia);
		if(data.cData.upVector != Ogre::Vector3::ZERO)
		{
			OgreNewt::UpVector* uv2 = new OgreNewt::UpVector(bod,data.cData.upVector); // create an upvector. This forces the body to rotate just in the X and Z axes.
		}
	}
	bod->setUserData(Ogre::Any((GameObject*)this));
	bod->attachNode(nod);
	
	//delete col;
	if(data.cData.colType != CT_TREECOLLISION && data.cData.colType != CT_EXTERNAL)
		mBody->setCustomForceAndTorqueCallback( boost::bind( &GameObject::forceCallback, this, _1 ) );  // add the previous defined callback function as the body custom force and torque callback
	//mBody->setUserData(this);
	Ogre::Vector3 pos;
	Ogre::Quaternion ornt;
	parent->getPositionOrientation(pos,ornt);

	bod->setPositionOrientation(pos+(ornt*data.cData.positionOffset)*scale,ornt*data.cData.orientationOffset);

	OgreNewt::Joint *j = createJoint(parent,bod,data.jointData);
	
	SubObject sub;
	sub.mArt = art;
	sub.mBody = bod;
	sub.colData = data.cData;
	sub.jData = data.jointData;
	sub.parent = parent;
	subObjectList.push_back(sub);
	jointList.push_back(j);
	
	//OgreNewt::BasicJoints::BallAndSocket

	std::vector<gamedata_dynamic>::const_iterator itr;
	for(itr = data.subObjects.begin();itr != data.subObjects.end();itr++)
	{
		processSubObject(*itr,bod,scale);
	}
}

DynamicObject::~DynamicObject()
{
	
	//mLevel->unregisterObject(this);
	for(JointList::iterator itr = jointList.begin();itr!=jointList.end();itr++)
	{
		OgreNewt::Joint* cur = *itr;
		delete cur;
	}
	for(SubObjectList::iterator itr = subObjectList.begin();itr!=subObjectList.end();itr++)
	{
		delete itr->mBody;
		delete itr->mArt;
	}
	//delete subObjectList;
	//delete jointList;
	/*
	delete mBody;
	
	if(mArt)
		delete mArt;
	destroyNode(mLevel->getSceneManager(),mNode);*/
//	mSceneMgr->destroySceneNode(mNode->getName());
}
/*
OgreNewt::Body *DynamicObject::getBody()
{
	return mBody;
}

Ogre::String DynamicObject::getID()
{
	return mID;
}
Level *DynamicObject::getLevel()
{
	return mLevel;
}
*/


void DynamicObject::setOrientation(Ogre::Quaternion ornt)
{
	//das erste objekt wird als parent angesehen
	Ogre::Quaternion oldOrnt;
	Ogre::Vector3 oldPos;
	mBody->getPositionOrientation(oldPos,oldOrnt);
	//Ogre::Vector3 offset = pos - oldPos;

	


	
	if(!subObjectList.empty())
	{
		Ogre::Quaternion rotBy = ornt * oldOrnt.Inverse();
		SubObjectList::iterator itr;
		for(itr = subObjectList.begin();itr!=subObjectList.end();itr++)
		{
			OgreNewt::Body *bod = itr->mBody;
			Ogre::Quaternion sOrnt;
			Ogre::Vector3 sPos;
			bod->getPositionOrientation(sPos,sOrnt);
			

			
			Ogre::Quaternion newOrnt = rotBy * sOrnt;
			
			Ogre::Vector3 relPos = sPos-oldPos;
			relPos = rotBy * relPos;
			bod->setPositionOrientation(relPos+oldPos,newOrnt);
		}
		
	}
	mBody->setPositionOrientation(oldPos,ornt);


}


void DynamicObject::rotate(const Ogre::Vector3& axis, const Ogre::Radian& angle, Ogre::Node::TransformSpace relativeTo)
{
	Ogre::Quaternion q;
	q.FromAngleAxis(angle,axis);
	
	rotate(q, relativeTo);
	
	
}

void DynamicObject::setPositionOrientation(Ogre::Vector3 pos, Ogre::Quaternion ornt)
{
	if(!subObjectList.empty())
	{
		//einzeln
		setPosition(pos);
		setOrientation(ornt);
	}
	else if(mBody)
	{
		mBody->setPositionOrientation(pos,ornt);
	}
	else
	{
		mNode->setPosition(pos);
		mNode->setOrientation(ornt);
	}
}


void DynamicObject::setScale(Ogre::Vector3 scale)
{
	Ogre::Vector3 oldScale = getScale();
	mArt->setScale(baseScale*scale);	
	if(!subObjectList.empty())
	{
		Ogre::Quaternion oldOrnt;
		Ogre::Vector3 oldPos;
		getPositionOrientation(oldPos,oldOrnt);
		Ogre::Vector3 scaleBy = scale/oldScale;
		Ogre::Quaternion temp;
		SubObjectList::iterator itr;
		for(itr = subObjectList.begin();itr!=subObjectList.end();itr++)
		{
			SubObject cur = *itr;
			cur.mArt->modScale(scaleBy);
			Ogre::Vector3 relPos;
			
			cur.mBody->getPositionOrientation(relPos,temp);
			relPos -= oldPos;
			relPos = oldOrnt.Inverse()*relPos;
			relPos *= scaleBy;
			relPos = oldOrnt*relPos;
			cur.mBody->setPositionOrientation(relPos+oldPos,temp);
		}
	}
	
	/*std::vector<GameObject*>::const_iterator itr;
	
	

	
	

	for(itr = groupedObjects.begin();itr != groupedObjects.end();itr++)
	{
		GameObject *cur = *itr;
		if(cur != parent)
		{
			Ogre::Vector3 relPos = cur->getPosition()-oldPos;
			cur->modScale(scaleBy);
			relPos *= scaleBy;
			cur->setPosition(relPos+oldPos);
		}
	}*/

}
void DynamicObject::modScale(Ogre::Vector3 scale)
{
	Ogre::Vector3 oldScale = getScale();
	mArt->modScale(scale);	
	if(!subObjectList.empty())
	{
		Ogre::Quaternion oldOrnt;
		Ogre::Vector3 oldPos;
		getPositionOrientation(oldPos,oldOrnt);
		
		Ogre::Quaternion temp;
		SubObjectList::iterator itr;
		for(itr = subObjectList.begin();itr!=subObjectList.end();itr++)
		{
			SubObject cur = *itr;
			cur.mArt->modScale(scale);
			Ogre::Vector3 relPos;
			
			cur.mBody->getPositionOrientation(relPos,temp);
			relPos -= oldPos;
			relPos = oldOrnt.Inverse()*relPos;
			relPos *= scale;
			relPos = oldOrnt*relPos;
			cur.mBody->setPositionOrientation(relPos+oldPos,temp);
		}
	}
	
/*
	GameObject *parent = groupedObjects[0];
	std::vector<GameObject*>::const_iterator itr;
	
	Ogre::Vector3 oldPos = parent->getPosition();
	

	parent->modScale(scale);	
	

	for(itr = groupedObjects.begin();itr != groupedObjects.end();itr++)
	{
		GameObject *cur = *itr;
		if(cur != parent)
		{
			Ogre::Vector3 relPos = cur->getPosition()-oldPos;
			cur->modScale(scale);
			relPos *= scale;
			cur->setPosition(relPos+oldPos);
		}
	}*/

}

void DynamicObject::updateBody()
{
	Ogre::Vector3 inertia;
	Ogre::Real mass;
	
	if(mBody)
	{
		
		mBody->getMassMatrix(mass,inertia);
		OgreNewt::CollisionPtr col = createCollision(mColData,inertia);
		mBody->setCollision(col);
		mBody->setMassMatrix(mass,inertia);
	}
	if(!subObjectList.empty())
	{
		//neu processen
		for(JointList::iterator itr=jointList.begin();itr!=jointList.end();itr++)
		{
			delete *itr;
		}
		jointList.clear();
		SubObjectList::iterator itr;
		for(itr = subObjectList.begin();itr!=subObjectList.end();itr++)
		{
			SubObject cur = *itr;
			cur.mBody->getMassMatrix(mass,inertia);
			OgreNewt::CollisionPtr col = createCollision(cur.colData,inertia);
			cur.mBody->setCollision(col);			
			cur.mBody->setMassMatrix(mass,inertia);
			//joint neu anlegen
			OgreNewt::Joint *j = createJoint(cur.parent,cur.mBody,cur.jData);
			
			jointList.push_back(j);
		}
	}
}

void DynamicObject::rotate(const Ogre::Quaternion& q, Ogre::Node::TransformSpace relativeTo)
{
	
	Ogre::Quaternion oldOrnt = getOrientation();
	
	Ogre::Quaternion newOrnt;
	
	switch(relativeTo)
	{
	case Ogre::Node::TS_WORLD:
	case Ogre::Node::TS_PARENT:
		// Rotations are normally relative to local axes, transform up
		newOrnt = q * oldOrnt;
		break;

	case Ogre::Node::TS_LOCAL:
		// Note the order of the mult, i.e. q comes after
		newOrnt = oldOrnt * q;
		break;
	}

	setOrientation(newOrnt);
	

}



void DynamicObject::setPosition(Ogre::Vector3 pos)
{
	Ogre::Quaternion ornt;
	Ogre::Vector3 oldPos;
	mBody->getPositionOrientation(oldPos,ornt);
	Ogre::Vector3 offset = pos - oldPos;
	if(!subObjectList.empty())
	{
		SubObjectList::iterator itr;
		for(itr = subObjectList.begin();itr!=subObjectList.end();itr++)
		{
			OgreNewt::Body *bod = itr->mBody;
			Ogre::Quaternion sOrnt;
			Ogre::Vector3 sPos;
			bod->getPositionOrientation(sPos,sOrnt);
			bod->setPositionOrientation(sPos+offset,sOrnt);
		}
		/*JointList::iterator itr2;
		for(itr2 = jointList.begin();itr2!=jointList.end();itr2++)
		{
			OgreNewt::Joint *j = *itr2;
			j->set
			Ogre::Quaternion sOrnt;
			Ogre::Vector3 sPos;
			bod->getPositionOrientation(sPos,sOrnt);
			bod->setPositionOrientation(sPos+offset,sOrnt);
		}*/
	}
	mBody->setPositionOrientation(pos,ornt);
}
/*
void DynamicObject::setOrientation(Ogre::Quaternion ornt)
{
	Ogre::Quaternion oldOrnt;
	Ogre::Vector3 pos;
	mBody->getPositionOrientation(pos,oldOrnt);
	mBody->setPositionOrientation(pos,ornt);
}

Ogre::Vector3 DynamicObject::getPosition()
{
	Ogre::Quaternion ornt;
	Ogre::Vector3 pos;
	mBody->getPositionOrientation(pos,ornt);
	return pos;
}


Ogre::Quaternion DynamicObject::getOrientation()
{
	Ogre::Quaternion ornt;
	Ogre::Vector3 pos;
	mBody->getPositionOrientation(pos,ornt);
	return ornt;
}
Ogre::Vector3 DynamicObject::getScale()
{
	return mNode->getScale();
}
Ogre::SceneNode *DynamicObject::getSceneNode()
{
	return mNode;
}
ObjType DynamicObject::getType()
{
	return type;
}
Ogre::String DynamicObject::getUName()
{
	return uName;
}
void DynamicObject::modScale(Ogre::Vector3 scale)
{
	mNode->scale(scale);

}*/
//void DynamicObject::update(Ogre::Real time)
//{
//	//nix
//}
//
//
//
//void DynamicObject::setScale(Ogre::Vector3 scale)
//{
//	mNode->setScale(scale);
//}

//Ogre::String DynamicObject::getName()
//{
//	//there is no name
//	return "";
//}
