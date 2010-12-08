#ifndef __praSpecialObject
#define __praSpecialObject

#include "Ogre.h"

#include "FwDec.h"

#include "GameObject.h"
#include "StandardApplication.h"



class DynamicObject: public GameObject
{
public:
	//default constructor
	DynamicObject(TiXmlElement *XMLNode,Level *lvl, Ogre::Vector3 positionOffset = Ogre::Vector3::ZERO);
	DynamicObject(gamedata_dynamic *data, Level *lvl, Ogre::Vector3 pos, Ogre::Quaternion ornt=Ogre::Quaternion::IDENTITY, Ogre::Vector3 _scale = Ogre::Vector3::UNIT_SCALE)
	{
		init(data,lvl,pos,ornt,_scale);
	}
	DynamicObject(Ogre::String id, Level *lvl, Ogre::Vector3 pos, Ogre::Quaternion ornt=Ogre::Quaternion::IDENTITY, Ogre::Vector3 _scale = Ogre::Vector3::UNIT_SCALE)
	{		
		gamedata_dynamic *myData = StandardApplication::getSingletonPtr()->getDynamicDataPtr(id);
		if(!myData || myData->id == "")
		{
			throw etInvalidId;
		}
		init(myData,lvl,pos,ornt,_scale);
	}
	//copy constructor
	//if lvl = NULL, same level as src will be used
	DynamicObject(DynamicObject *src, Level *lvl = NULL);
	GameObject* clone(Level *targetLvl = NULL)
	{
		return static_cast<GameObject*>(new DynamicObject(this,targetLvl));
	}
	virtual ~DynamicObject();

	/*void update(Ogre::Real time);*/
	//sets position
	void setPosition(Ogre::Vector3 pos);
	void setOrientation(Ogre::Quaternion ornt);
	void setPositionOrientation(Ogre::Vector3 pos,Ogre::Quaternion ornt);

	void setScale(Ogre::Vector3 scale);
	void modScale(Ogre::Vector3 scale);	
	virtual TiXmlElement* getXmlElem(Ogre::Vector3 startPosition = Ogre::Vector3::ZERO,bool savegame=false);
	void rotate(const Ogre::Quaternion& q, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_LOCAL);
	void rotate(const Ogre::Vector3& axis, const Ogre::Radian& angle, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_LOCAL);
	////sets orientation
	//void setOrientation(Ogre::Quaternion ornt);
	////retrieves body position
	//Ogre::Vector3 getPosition();
	////retr. body orient
	//Ogre::Quaternion getOrientation();
	//void setScale(Ogre::Vector3 scale);
	//void modScale(Ogre::Vector3 scale);
	//Ogre::Vector3 getScale();
	//OgreNewt::Body *getBody();
	//Ogre::SceneNode *getSceneNode();
	//pointer to the level this obj is currently in
	/*Level *getLevel();
	ObjType getType();
	Ogre::String getID();
	Ogre::String getUName();*/
	/*Ogre::String getName();*/
	void updateBody();
	Ogre::String getDisplayName()
	{
		return ID;
	}


protected:
	void init(gamedata_dynamic *data, Level *lvl, Ogre::Vector3 pos, Ogre::Quaternion ornt=Ogre::Quaternion::IDENTITY, Ogre::Vector3 _scale = Ogre::Vector3::UNIT_SCALE,int SaveGameID = 0);
	void processSubObject(gamedata_dynamic data, OgreNewt::Body *parent,Ogre::Vector3 scale = Ogre::Vector3::UNIT_SCALE);
	//gamedata_dynamic *myData;
	struct SubObject
	{
		OgreNewt::Body* mBody;
		WorldArt* mArt;
		CollisionData colData;
		//ein pointer auf einen eintrag im "subObjects" vector seines parents
		joint_data jData;
		OgreNewt::Body *parent;
	};
	//typedef std::list<OgreNewt::Body*> BodyList;
	typedef std::vector<SubObject> SubObjectList;
	typedef std::vector<OgreNewt::Joint*> JointList;
	SubObjectList subObjectList;
	//BodyList subBodyList;
	JointList jointList;
	
	//gamedata_obj_static *myData;
	//OgreNewt::Body *mBody;
	//Ogre::SceneNode *mNode;
	//ObjType type; //<- später evtl als enum
	//Ogre::String mID;
	//Ogre::String uName;
	//Level *mLevel;

};

#endif