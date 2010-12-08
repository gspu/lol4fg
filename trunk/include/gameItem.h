#ifndef __praGameItem
#define __praGameItem
#include "GameObject.h"
#include "Ogre.h"
#include "OgreNewt.h"
#include "StandardApplication.h"



class GameItem: public GameObject
{
public:
	GameItem(TiXmlElement *XMLNode,Level *lvl, Ogre::Vector3 positionOffset);
	GameItem(gamedata_item *data, Level *lvl, Ogre::Vector3 pos, Ogre::Quaternion ornt=Ogre::Quaternion::IDENTITY, Ogre::Vector3 _scale = Ogre::Vector3::UNIT_SCALE)
	{
		init(data,lvl,pos,ornt,_scale);
	}
	GameItem(Ogre::String id, Level *lvl, Ogre::Vector3 pos, Ogre::Quaternion ornt=Ogre::Quaternion::IDENTITY, Ogre::Vector3 _scale = Ogre::Vector3::UNIT_SCALE)
	{
		gamedata_item *myData = StandardApplication::getSingletonPtr()->getItemDataPtr(id);
		if(!myData)
		{
			throw etInvalidId;
		}
		init(myData,lvl,pos,ornt,_scale);
	}
	GameItem(GameItem *src,Level *targetlvl);
	
	~GameItem();
	GameObject* clone(Level *targetLvl = NULL)
	{
		return new GameItem(this,targetLvl);
	}
	Ogre::String getDisplayName()
	{
		return ID;
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
	Ogre::String getName();
	void onActivate(GameChar *user,bool cursorMode=false);
	/*inline gamedata_item *getData()
	{
		return myData;
	}*/


protected:
	void init(gamedata_item *data, Level *lvl, Ogre::Vector3 pos, Ogre::Quaternion ornt=Ogre::Quaternion::IDENTITY, Ogre::Vector3 _scale = Ogre::Vector3::UNIT_SCALE,int SaveGameID = 0);
	//gamedata_obj_static *myData;
	//OgreNewt::Body *mBody;
	//Ogre::SceneNode *mNode;
	//ObjType type; //<- später evtl als enum
	//Ogre::String mID;
	//Ogre::String uName;
	//Level *mLevel;
};

#endif