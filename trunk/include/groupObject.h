#ifndef __praGroupObject
#define __praGroupObject
#include "GameObject.h"
#include "Ogre.h"
#include "OgreNewt.h"



class GroupObject: public GameObject
{
public:
	//man kann hier gleich ein inventar übergeben, das dann verwendet wird. wenn mans nicht macht, wird eines autoerzeugt
	GroupObject(Level *lvl,GameObject *first);

	void addObject(GameObject *add);
	//GroupObject(GroupObject *src,Level *targetLvl = NULL){};
	GameObject* clone(Level *targetLvl = NULL)
	{
		std::vector<GameObject*>::const_iterator itr;
		for(itr = groupedObjects.begin();itr != groupedObjects.end();itr++)
		{
			GameObject *cur = *itr;
			cur->clone(targetLvl);
		}
		return NULL;//static_cast<GroupObject*>(new GroupObject(this,targetLvl));
	}
	~GroupObject();
	//gruppe auflösen
	void dissolveGroup();
	void removeObject(GameObject *obj);
	//die funktion ist für den leveldestruktor gedacht
	void levelDestructor();
	inline GameObject *getObject(unsigned int nr)
	{
		return groupedObjects[nr];
	}
	Ogre::String getDisplayName()
	{
		return "Group ("+Ogre::StringConverter::toString(getCount())+")";
	}
	void setPosition(Ogre::Vector3 pos);
	//sets orientation
	void setOrientation(Ogre::Quaternion ornt);
	//sets both
	void setPositionOrientation(Ogre::Vector3 pos,Ogre::Quaternion ornt);
	//retrieves body position
	Ogre::Vector3 getPosition();
	//retr. body orient
	Ogre::Quaternion getOrientation();
	//retr. both (false if fails)
	bool getPositionOrientation( Ogre::Vector3& pos, Ogre::Quaternion& orient );

	bool hasObject(GameObject *obj)
	{
		std::vector<GameObject*>::const_iterator itr;	
		
		for(itr = groupedObjects.begin();itr != groupedObjects.end();itr++)
		{
			if(obj == *itr)
				return true;
		}
		return false;
	}

	void setScale(Ogre::Vector3 scale);
	void modScale(Ogre::Vector3 scale);
	Ogre::Vector3 getScale();

	OgreNewt::Body *getBody()
	{
		return groupedObjects[0]->getBody();
	}
	virtual Ogre::SceneNode *getSceneNode()
	{
		return mNode;//groupedObjects[0]->getSceneNode();
	}
	//pointer to the level this obj is currently in
	virtual Level *getLevel()
	{
		return mLevel;
	}
	
	virtual WorldArt *getWorldArt()
	{
		return groupedObjects[0]->getWorldArt();
	}
	virtual Ogre::String getID()
	{
		return "";
	}
	virtual Ogre::String getName()
	{
		return "";
	}

	void updateBody();
	void markSelected(bool mark);

	void rotate(const Ogre::Quaternion& q, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_LOCAL);
	void rotate(const Ogre::Vector3& axis, const Ogre::Radian& angle, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_LOCAL);

	size_t getCount()
	{
		return groupedObjects.size();
	}
	std::vector<GameObject*> groupedObjects;

private:
	//EditFrameListener *fl;
	QtOgre::EditGameLogic *gl;
	bool deleteAllGrouped;
	bool isSelected;
	bool dissolveCalled;

	Ogre::Vector3 rootPosition; //wird einfach der schwerpunkt
	Ogre::Vector3 rootScale; //1 ist so, wie es zusammengestellt wurde
	Ogre::Quaternion rootOrientation; //identity, wie es ausgewählt wurde
	//Ogre::SceneNode rootNode;
	//void updateRootValues();
	//void updateChild(GameObject *child);
	/*Ogre::Vector3 oldPos;
	Ogre::Vector3 oldScale;
	Ogre::Quaternion oldOrnt;*/
};
#endif