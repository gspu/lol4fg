#ifndef __praEntranceObj
#define __praEntranceObj

#include "GameObject.h"
#include "Ogre.h"

#include "FwDec.h"


#include "Level.h"

class entranceObject: public GameObject
{
public:
	entranceObject(entranceObject *src,Level *targetLvl = NULL);
	entranceObject(Ogre::String name, Level *lvl, Ogre::Vector3 pos, Ogre::Quaternion ornt=Ogre::Quaternion::IDENTITY)
	{
		init(name,lvl,pos,ornt);
	}
	entranceObject(TiXmlElement *XMLNode,Level *lvl, Ogre::Vector3 positionOffset = Ogre::Vector3::ZERO);
		//: DynamicObject(Ogre::String id, Level *lvl, Ogre::Vector3 pos, Ogre::Quaternion ornt, Ogre::Vector3 _scale);

	~entranceObject();
	void setScale(Ogre::Vector3 scale){}
	void modScale(Ogre::Vector3 scale){}
	Ogre::Vector3 getScale();
	virtual TiXmlElement* getXmlElem(Ogre::Vector3 startPosition = Ogre::Vector3::ZERO,bool savegame=false);
	GameObject* clone(Level *targetLvl = NULL){return NULL;}

	Ogre::String getName();
	void setName(Ogre::String str);
	Ogre::String getDisplayName()
	{
		return ID;
	}
private:
	//Ogre::String mName;
	void init(Ogre::String name, Level *lvl, Ogre::Vector3 pos, Ogre::Quaternion ornt=Ogre::Quaternion::IDENTITY);

};


#endif