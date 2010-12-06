#ifndef __praDoorObject
#define __praDoorObject

#include "Ogre.h"

//#include "FwDec.h"

#include "GameObject.h"
#include "Level.h"







class DoorObject: public GameObject
{
public:
	//copy constructor
	DoorObject(DoorObject *src, Level *targetLvl = NULL);
	//XML konstruktor
	DoorObject(TiXmlElement *XMLElement, Level *lvl,Ogre::Vector3 positionOffset = Ogre::Vector3::ZERO);
	//default constructor
	DoorObject(gamedata_door *myData, Level *lvl, Ogre::Vector3 pos, Ogre::Quaternion ornt=Ogre::Quaternion::IDENTITY, Ogre::Vector3 _scale = Ogre::Vector3::UNIT_SCALE)
	{
		init(myData,lvl,pos,ornt,_scale);
	}

	DoorObject(Ogre::String id, Level *lvl, Ogre::Vector3 pos, Ogre::Quaternion ornt=Ogre::Quaternion::IDENTITY, Ogre::Vector3 _scale = Ogre::Vector3::UNIT_SCALE)
	{
			
		gamedata_door *myData = StandardApplication::getSingletonPtr()->getDoorDataPtr(id);
		if(!myData)
		{
			throw etInvalidId;
		}
		init(myData,lvl,pos,ornt,_scale);
	}
		//: DynamicObject(Ogre::String id, Level *lvl, Ogre::Vector3 pos, Ogre::Quaternion ornt, Ogre::Vector3 _scale);

	~DoorObject();
	GameObject* clone(Level *targetLvl = NULL)
	{
		return new DoorObject(this,targetLvl);	
	}

	Ogre::String getDisplayName()
	{
		return "(do)"+ID;
	}
	
	virtual TiXmlElement* getXmlElem(Ogre::Vector3 startPosition = Ogre::Vector3::ZERO,bool savegame=false);
	//void update(Ogre::Real time);
	////sets position
	//void setPosition(Ogre::Vector3 pos);
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
	////pointer to the level this obj is currently in
	//Level *getLevel();
	//ObjType getType();
	//Ogre::String getID();
	//Ogre::String getUName();
	Ogre::String getName()
	{
		return mName;
	}
	//if player wants to use it
	void onActivate(GameChar *user,bool cursorMode = false);

	void onCollision(GameObject *with,ContCallback *cb);

	

	DoorData getDoorData()
	{
		return mDoorData;
	}
	void setDoorData(DoorData dt)
	{
		mDoorData = dt;
	}
	void update(Ogre::Real time)
	{
		//einfach das collided resetten
		collided = NULL;
	}
	inline Ogre::Vector3 getScale()
	{
		if(mArt)
			return mArt->getScale()/baseScale;
		else
			return mScale;
	}
	inline void setScale(Ogre::Vector3 scale)
	{
		if(mArt)
			mArt->setScale(baseScale*scale);
		else
			mScale = scale;
		updateBody();
	}
private:
	DoorData mDoorData;
	Ogre::Vector3 mScale;//das ist für das game wichtig, weil sonst ist scale immer (1,1,1)
	Ogre::String mName;
	bool useOnCol;
	GameChar *collided;//das ist nur so ne art buffer. das ist ziemlich egal.
	void init(gamedata_door *myData, Level *lvl, Ogre::Vector3 pos, Ogre::Quaternion ornt=Ogre::Quaternion::IDENTITY, Ogre::Vector3 _scale = Ogre::Vector3::UNIT_SCALE,int sgid = 0);
	//OgreNewt::Body *mBody;
	//Ogre::SceneNode *mNode;
	//ObjType type; //<- später evtl als enum
	//Ogre::String mID;
	//Ogre::String uName;
	//Level *mLevel;
};

#endif