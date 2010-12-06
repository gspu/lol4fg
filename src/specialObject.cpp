#include "SpecialObject.h"
#include "StandardApplication.h"
#include "functions.h"
#include "Level.h"

SpecialObject::SpecialObject(SpecialObject *src, Level *lvl)
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
    Vector3 pos;
	Quaternion ornt;
	src->mBody->getPositionOrientation(pos,ornt);
    init(static_cast<gamedata_special*>(src->mData),L,pos,ornt,src->getScale());
}

SpecialObject::SpecialObject(TiXmlElement *XMLNode,Level *lvl, Vector3 positionOffset )
{
	
    String id = "";
	Vector3 pos = StringConverter::parseVector3(XMLNode->Attribute("pos"))+positionOffset;
	Vector3 scale = Vector3::UNIT_SCALE;
	Quaternion ornt = Quaternion::IDENTITY;
	int SGID = 0;
		
	
	if(XMLNode->Attribute("orient"))
	{
		ornt = StringConverter::parseQuaternion(XMLNode->Attribute("orient"));
	}
	
	if(XMLNode->Attribute("id"))
	{
		id = XMLNode->Attribute("id");
	}
	if(XMLNode->Attribute("scale"))
	{
		scale = StringConverter::parseVector3(XMLNode->Attribute("scale"));
	}
	gamedata_special *mSpData = StandardApplication::getSingletonPtr()->getSpecialDataPtr(id);
	if(!mSpData)
		throw etInvalidId;
	if(XMLNode->Attribute("SGID"))
	{
		SGID = StringConverter::parseInt(XMLNode->Attribute("SGID"));
	}
	init(mSpData,lvl,pos,ornt,scale,SGID);
	if(XMLNode->Attribute("inlevelfile"))
	{
		isInLevelFile = StringConverter::parseBool(XMLNode->Attribute("inlevelfile"));
	}
	if(mBody)
	{
		Vector3 omega = Vector3::ZERO;
		Vector3 velocity = Vector3::ZERO;
		if(XMLNode->Attribute("omega"))
		{
			omega = StringConverter::parseVector3(XMLNode->Attribute("omega"));
		}
		if(XMLNode->Attribute("velocity"))
		{
			velocity = StringConverter::parseVector3(XMLNode->Attribute("velocity"));
		}
		mBody->setVelocity(velocity);
		mBody->setOmega(omega);
	}
	
}

TiXmlElement* SpecialObject::getXmlElem(Vector3 startPosition,bool savegame)
{
	String strtype = objectTypeToString(type);
	if(strtype == "")
		return NULL;
	TiXmlElement* xmlElem = new TiXmlElement(strtype.c_str());
	Vector3 pos, scale;
	Quaternion ornt;
		
	scale = getScale();
	getPositionOrientation(pos,ornt);
		
	pos -= startPosition;
	xmlElem->SetAttribute("pos",StringConverter::toString(pos).c_str());
	if(ornt != Quaternion::IDENTITY)
	{
		xmlElem->SetAttribute("orient",StringConverter::toString(ornt).c_str());
	}
	if(SGID)
	{
		xmlElem->SetAttribute("SGID",StringConverter::toString(SGID).c_str());
	}
	if(scale != Vector3::UNIT_SCALE)
	{
		xmlElem->SetAttribute("scale",StringConverter::toString(scale).c_str());
	}
	xmlElem->SetAttribute("inlevelfile",StringConverter::toString(isInLevelFile).c_str());
	if(savegame && mBody && mBody->getSleepingState())
	{
		Vector3 omega = mBody->getOmega();
		Vector3 velocity = mBody->getVelocity();
		if(omega != Vector3::ZERO)
		{
			xmlElem->SetAttribute("omega",StringConverter::toString(omega).c_str());
		}
		if(velocity != Vector3::ZERO)
		{
			xmlElem->SetAttribute("velocity",StringConverter::toString(velocity).c_str());
		}
	}
	xmlElem->SetAttribute("id",ID.c_str());
	return xmlElem;
	
}

void SpecialObject::init(gamedata_special *data, Level *lvl, Ogre::Vector3 pos, Ogre::Quaternion ornt, Ogre::Vector3 _scale,int SaveGameID)
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

	
	type = otSpecial;

	//SceneManager *mSceneMgr = mLevel->getSceneManager();

	/*mNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();*/
	/*String mesh = myData.mesh;
	baseScale = myData.scale;*/

	ID = data->id;
	/*Vector3 scale = baseScale*_scale;
	createNodeAndEntity(mesh,myData.material);

	mNode->setScale(scale);
	colType = myData.collisionType;
	collisionParams = myData.collisionParams;
	mBody = makeBody(mLevel->getWorld(),myData.collisionType,myData.mass,myData.collisionParams,mNode);*/
//	createWorldArt(myData.wData);
	baseScale = data->wData.scale;
	

	mArt = new WorldArt(this,data->wData,NULL,_scale);
	mColData = data->cData;
	createBody(data->cData);
	if(data->cData.colType != CT_TREECOLLISION && data->cData.colType != CT_EXTERNAL)
		mBody->setCustomForceAndTorqueCallback<GameObject>(&GameObject::forceCallback,this);  // add the previous defined callback function as the body custom force and torque callback
	//mBody->setUserData(this);
	mBody->setPositionOrientation(pos,ornt);

	//jetzt die sub.objekte!
	std::vector<gamedata_special>::const_iterator itr;
	for(itr = data->subObjects.begin();itr != data->subObjects.end();itr++)
	{
		processSubObject(*itr,mBody,_scale);
	}

	mLevel->registerObject(this);
}

void SpecialObject::processSubObject(gamedata_special data, OgreNewt::Body *parent,Vector3 scale)
{
	SceneNode* nod = mLevel->getSceneManager()->getRootSceneNode()->createChildSceneNode();
	
	WorldArt *art = new WorldArt(this,data.wData,nod,scale);
	//art->setScale(baseScale/scale);

	OgreNewt::World *mWorld = mLevel->getWorld();
	OgreNewt::Collision *col;
	Vector3 inertia;
	Real mass = data.cData.mass;
	CollisionData newCData = data.cData;
	newCData.collistionParams;// *= baseScale/scale;
	col = createCollision(newCData,inertia);
	OgreNewt::Body *bod = new OgreNewt::Body(mWorld,col,type);
	if(mass != 0)
	{
		bod->setMassMatrix(mass,inertia);
		if(data.cData.upVector != Vector3::ZERO)
		{
			OgreNewt::BasicJoints::UpVector* uv2 = new OgreNewt::BasicJoints::UpVector(mWorld,bod,data.cData.upVector); // create an upvector. This forces the body to rotate just in the X and Z axes.
	
		}
	}
	bod->setUserData(this);
	bod->attachToNode(nod);
	delete col;
	if(data.cData.colType != CT_TREECOLLISION && data.cData.colType != CT_EXTERNAL)
		mBody->setCustomForceAndTorqueCallback<GameObject>(&GameObject::forceCallback,this);  // add the previous defined callback function as the body custom force and torque callback
	//mBody->setUserData(this);
	Vector3 pos;
	Quaternion ornt;
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

	std::vector<gamedata_special>::const_iterator itr;
	for(itr = data.subObjects.begin();itr != data.subObjects.end();itr++)
	{
		processSubObject(*itr,bod,scale);
	}
}

SpecialObject::~SpecialObject()
{
	
	mLevel->unregisterObject(this);
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
	
	delete mBody;
	
	if(mArt)
		delete mArt;
	destroyNode(mLevel->getSceneManager(),mNode);
//	mSceneMgr->destroySceneNode(mNode->getName());
}
/*
OgreNewt::Body *SpecialObject::getBody()
{
	return mBody;
}

String SpecialObject::getID()
{
	return mID;
}
Level *SpecialObject::getLevel()
{
	return mLevel;
}
*/


void SpecialObject::setOrientation(Quaternion ornt)
{
	//das erste objekt wird als parent angesehen
	Quaternion oldOrnt;
	Vector3 oldPos;
	mBody->getPositionOrientation(oldPos,oldOrnt);
	//Vector3 offset = pos - oldPos;

	


	
	if(!subObjectList.empty())
	{
		Quaternion rotBy = ornt * oldOrnt.Inverse();
		SubObjectList::iterator itr;
		for(itr = subObjectList.begin();itr!=subObjectList.end();itr++)
		{
			OgreNewt::Body *bod = itr->mBody;
			Quaternion sOrnt;
			Vector3 sPos;
			bod->getPositionOrientation(sPos,sOrnt);
			

			
			Quaternion newOrnt = rotBy * sOrnt;
			
			Vector3 relPos = sPos-oldPos;
			relPos = rotBy * relPos;
			bod->setPositionOrientation(relPos+oldPos,newOrnt);
		}
		
	}
	mBody->setPositionOrientation(oldPos,ornt);


}


void SpecialObject::rotate(const Vector3& axis, const Radian& angle, Node::TransformSpace relativeTo)
{
	Quaternion q;
	q.FromAngleAxis(angle,axis);
	
	rotate(q, relativeTo);
	
	
}

void SpecialObject::setPositionOrientation(Ogre::Vector3 pos, Ogre::Quaternion ornt)
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


void SpecialObject::setScale(Vector3 scale)
{
	Vector3 oldScale = getScale();
	mArt->setScale(baseScale*scale);	
	if(!subObjectList.empty())
	{
		Quaternion oldOrnt;
		Vector3 oldPos;
		getPositionOrientation(oldPos,oldOrnt);
		Vector3 scaleBy = scale/oldScale;
		Quaternion temp;
		SubObjectList::iterator itr;
		for(itr = subObjectList.begin();itr!=subObjectList.end();itr++)
		{
			SubObject cur = *itr;
			cur.mArt->modScale(scaleBy);
			Vector3 relPos;
			
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
			Vector3 relPos = cur->getPosition()-oldPos;
			cur->modScale(scaleBy);
			relPos *= scaleBy;
			cur->setPosition(relPos+oldPos);
		}
	}*/

}
void SpecialObject::modScale(Vector3 scale)
{
	Vector3 oldScale = getScale();
	mArt->modScale(scale);	
	if(!subObjectList.empty())
	{
		Quaternion oldOrnt;
		Vector3 oldPos;
		getPositionOrientation(oldPos,oldOrnt);
		
		Quaternion temp;
		SubObjectList::iterator itr;
		for(itr = subObjectList.begin();itr!=subObjectList.end();itr++)
		{
			SubObject cur = *itr;
			cur.mArt->modScale(scale);
			Vector3 relPos;
			
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
	
	Vector3 oldPos = parent->getPosition();
	

	parent->modScale(scale);	
	

	for(itr = groupedObjects.begin();itr != groupedObjects.end();itr++)
	{
		GameObject *cur = *itr;
		if(cur != parent)
		{
			Vector3 relPos = cur->getPosition()-oldPos;
			cur->modScale(scale);
			relPos *= scale;
			cur->setPosition(relPos+oldPos);
		}
	}*/

}

void SpecialObject::updateBody()
{
	Vector3 inertia;
	Real mass;
	
	if(mBody)
	{
		
		mBody->getMassMatrix(mass,inertia);
		OgreNewt::Collision *col = createCollision(mColData,inertia);
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
			OgreNewt::Collision *col = createCollision(cur.colData,inertia);
			cur.mBody->setCollision(col);
			cur.mBody->setMassMatrix(mass,inertia);
			//joint neu anlegen
			OgreNewt::Joint *j = createJoint(cur.parent,cur.mBody,cur.jData);
			
			jointList.push_back(j);
		}
	}
}

void SpecialObject::rotate(const Quaternion& q, Node::TransformSpace relativeTo)
{
	
	Quaternion oldOrnt = getOrientation();
	
	Quaternion newOrnt;
	
	switch(relativeTo)
	{
	case Node::TS_WORLD:
	case Node::TS_PARENT:
		// Rotations are normally relative to local axes, transform up
		newOrnt = q * oldOrnt;
		break;

	case Node::TS_LOCAL:
		// Note the order of the mult, i.e. q comes after
		newOrnt = oldOrnt * q;
		break;
	}

	setOrientation(newOrnt);
	

}



void SpecialObject::setPosition(Vector3 pos)
{
	Quaternion ornt;
	Vector3 oldPos;
	mBody->getPositionOrientation(oldPos,ornt);
	Vector3 offset = pos - oldPos;
	if(!subObjectList.empty())
	{
		SubObjectList::iterator itr;
		for(itr = subObjectList.begin();itr!=subObjectList.end();itr++)
		{
			OgreNewt::Body *bod = itr->mBody;
			Quaternion sOrnt;
			Vector3 sPos;
			bod->getPositionOrientation(sPos,sOrnt);
			bod->setPositionOrientation(sPos+offset,sOrnt);
		}
		/*JointList::iterator itr2;
		for(itr2 = jointList.begin();itr2!=jointList.end();itr2++)
		{
			OgreNewt::Joint *j = *itr2;
			j->set
			Quaternion sOrnt;
			Vector3 sPos;
			bod->getPositionOrientation(sPos,sOrnt);
			bod->setPositionOrientation(sPos+offset,sOrnt);
		}*/
	}
	mBody->setPositionOrientation(pos,ornt);
}
/*
void SpecialObject::setOrientation(Quaternion ornt)
{
	Quaternion oldOrnt;
	Vector3 pos;
	mBody->getPositionOrientation(pos,oldOrnt);
	mBody->setPositionOrientation(pos,ornt);
}

Vector3 SpecialObject::getPosition()
{
	Quaternion ornt;
	Vector3 pos;
	mBody->getPositionOrientation(pos,ornt);
	return pos;
}


Quaternion SpecialObject::getOrientation()
{
	Quaternion ornt;
	Vector3 pos;
	mBody->getPositionOrientation(pos,ornt);
	return ornt;
}
Vector3 SpecialObject::getScale()
{
	return mNode->getScale();
}
SceneNode *SpecialObject::getSceneNode()
{
	return mNode;
}
ObjType SpecialObject::getType()
{
	return type;
}
String SpecialObject::getUName()
{
	return uName;
}
void SpecialObject::modScale(Vector3 scale)
{
	mNode->scale(scale);

}*/
//void SpecialObject::update(Real time)
//{
//	//nix
//}
//
//
//
//void SpecialObject::setScale(Vector3 scale)
//{
//	mNode->setScale(scale);
//}

//String SpecialObject::getName()
//{
//	//there is no name
//	return "";
//}
