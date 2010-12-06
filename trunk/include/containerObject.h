#ifndef __praContainerItem
#define __praContainerItem
#include "GameObject.h"
#include "Ogre.h"
#include "OgreNewt.h"
#include "StandardApplication.h"
#include "inventory.h"



class ContainerObject: public GameObject
{
public:
	//man kann hier gleich ein inventar übergeben, das dann verwendet wird. wenn mans nicht macht, wird eines autoerzeugt
	ContainerObject(Ogre::String id, Level *lvl, Ogre::Vector3 pos, Ogre::Quaternion ornt=Ogre::Quaternion::IDENTITY, Ogre::Vector3 _scale = Ogre::Vector3::UNIT_SCALE, Inventory *inv = NULL)
	{
		gamedata_container* myData = StandardApplication::getSingletonPtr()->getContainerDataPtr(id);
		if(myData->id == "")
		{
			throw etInvalidId;
		}
		init(myData,lvl,pos,ornt,_scale,inv);
	}
	ContainerObject(gamedata_container *data, Level *lvl, Ogre::Vector3 pos, Ogre::Quaternion ornt=Ogre::Quaternion::IDENTITY, Ogre::Vector3 _scale = Ogre::Vector3::UNIT_SCALE, Inventory *inv = NULL)
	{
		init(data,lvl,pos,ornt,_scale,inv);
	}
	ContainerObject(TiXmlElement *node, Level *lvl, Ogre::Vector3 positionOffset = Ogre::Vector3::ZERO);
	
	ContainerObject(ContainerObject *src,Level *targetLvl = NULL);
	GameObject* clone(Level *targetLvl = NULL)
	{
		return static_cast<ContainerObject*>(new ContainerObject(this,targetLvl));
	}
	~ContainerObject();
	Inventory *mInv;	
	inline Inventory *getInventory()
	{
		return mInv;
	}
	Ogre::String getName()
	{
		return name;
	}
	void onActivate(GameChar *user,bool cursorMode = false);
	virtual TiXmlElement* getXmlElem(Ogre::Vector3 startPosition = Ogre::Vector3::ZERO,bool savegame=false);
	/*inline gamedata_container* getData()
	{
		return myData;
	}*/
	Ogre::String getDisplayName()
	{
		return "(co)"+ID;
	}
private:
	Ogre::String name;
	//fake konstruktor
	void init(gamedata_container *data, Level *lvl, Ogre::Vector3 pos, Ogre::Quaternion ornt=Ogre::Quaternion::IDENTITY, Ogre::Vector3 _scale = Ogre::Vector3::UNIT_SCALE, Inventory *inv = NULL,int SGID = 0);
	//gamedata_container* myData;
};
#endif