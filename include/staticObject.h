#ifndef __praStaticObject
#define __praStaticObject

#include "Ogre.h"

//#include "FwDec.h"

#include "GameObject.h"
#include "StandardApplication.h"



class StaticObject: public GameObject
{
public:
	StaticObject(TiXmlElement *XMLNode,Level *lvl,Ogre::Vector3 positionOffset = Ogre::Vector3::ZERO);
	StaticObject(Level *lvl, WorldArtData wData, Ogre::Vector3 pos, Ogre::Quaternion ornt=Ogre::Quaternion::IDENTITY, Ogre::Vector3 scale = Ogre::Vector3::UNIT_SCALE, bool collision = true)
	{
		init(lvl,wData,pos,ornt,scale,collision);
	}
	//default constructor. "type" nimmt "dotscene", "mesh" oder "billboard", wenn leer, versucht es selber rauszufinden
	//StaticObject(Ogre::String filename, Level *lvl, Ogre::Vector3 pos, Ogre::Quaternion ornt=Ogre::Quaternion::IDENTITY, Ogre::Vector3 scale = Ogre::Vector3::UNIT_SCALE, Ogre::String material = "", bool collision = true, Ogre::String type = "");
	//copy constructor
	StaticObject(StaticObject *src, Level *targetLvl = NULL);
	//alternativer, der direkt ein XML-element nimmt. damit sollten im grunde statics geladen werden
	//StaticObject(Level *lvl,TiXmlElement *elem);
	//if lvl = NULL, same level as src will be used
	//StaticObject(DynamicObject *src, Level *lvl = NULL);
	GameObject* clone(Level *targetLvl = NULL)
	{
		return static_cast<GameObject*>(new StaticObject(this,targetLvl));
		//return NULL;
	}
	virtual ~StaticObject();
	
	inline bool hasCollision()
	{
		return collision;
	}
	
	inline Ogre::String getMesh()
	{
		return ID;
	}
	inline Ogre::String getMaterial()
	{
		return materialName;
	}
	virtual TiXmlElement* getXmlElem(Ogre::Vector3 startPosition = Ogre::Vector3::ZERO,bool savegame=false);
	void setMesh(Ogre::String newMesh);
	void setMaterial(Ogre::String newMaterial);
	void setWorldArtData(WorldArtData wData);
	void setCollision(bool enable);
	bool getCollision()
	{
		return collision;
	}
	gamedata *getData()
	{
		return NULL;
	}

	Ogre::String getDisplayName()
	{
		Ogre::String name = "";//+st->getWorldArt()->getData().filename;
		WorldArtData wd = getWorldArt()->getData();
		switch(wd.type)
		{
		case WT_BILLBOARD:
			name += ("billboard:"+wd.material).c_str();
			break;
		case WT_DOTSCENE:
			name += ("dotscene:"+wd.filename).c_str();
			break;
		case WT_MESH:
			name += ("mesh:"+wd.filename).c_str();
			break;
		case WT_PLANE:
			name += ("plane:"+wd.material).c_str();
			break;
		case WT_PARTICLESYSTEM:
			name += ("particlesys:"+wd.material).c_str();
			break;
		}
		return name;
	}
	
	//void updateBody();
	
    //static braucht diese funktionen nicht->mit leer überschreiben
    void update(Ogre::Real time){};
    void playContactSound(Ogre::String filename){};
protected:
	//Ogre::String meshName;
	Ogre::String materialName;
	bool collision;
	void init(Level *lvl, WorldArtData wData, Ogre::Vector3 pos, Ogre::Quaternion ornt=Ogre::Quaternion::IDENTITY, Ogre::Vector3 scale = Ogre::Vector3::UNIT_SCALE, bool collision = true);

};

#endif