#ifndef praMissileObject
#define praMissileObject

#include "Ogre.h"

#include "FwDec.h"
////#include "OgreAL.h"
#include "GameObject.h"
#include "StandardApplication.h"
#include "Sound.h"
//#include "OgreNewt.h"
#include "SmallClasses.h"



class MissileObject: public GameObject
{
public:
	//das v0 ist die Geschwindigkeit des casters
	/*MissileObject(GameObject *_caster,MissileData data, Level *lvl, Ogre::Vector3 pos, Ogre::Quaternion ornt, Ogre::Vector3 v0 = Ogre::Vector3::ZERO)
	{
		SpecificMissileData d;
		d.missile = data;
		d.caster = _caster;
		init(d,lvl,pos,ornt,v0);
	}*/
	MissileObject(TiXmlElement *XMLNode, Level *lvl,Ogre::Vector3 positionOffset = Ogre::Vector3::ZERO);
	MissileObject(GameObject *caster, gamedata_damagedealer *source, Level *lvl, Ogre::Vector3 pos, Ogre::Quaternion ornt, Ogre::Vector3 v0 = Ogre::Vector3::ZERO)
	{
		init(caster,source,lvl,pos,ornt,v0);
	}
	
	//es hat kein sinn, feuerbälle zu klonen
	GameObject* clone(Level *targetLvl = NULL){return NULL;}

	~MissileObject();

	void onCollision(GameObject *with,ContCallback *callback);

	Ogre::String getDisplayName()
	{
		return ID;
	}

	/*Damage dmg;
	EffectDataList *effects;
	ExplosionData *explosion;*/
	//void setData(Damage d,GameObject *_caster = NULL,gamedata_spell *spell = NULL,SharedPtr<ExplosionData> ex=(SharedPtr<ExplosionData>)((ExplosionData*)NULL))
	//{
	//	mData.dmg = d;
	//	mData.spell = spell;
	//	//if(!ex.isNull())
	//	mData.expl = ex;
	//	mData.caster = _caster;
	//}
	//SpellEffectData effect;
	//GameObject *caster;
	void update(Ogre::Real time);
	gamedata *getData()
	{
		return mCastingItem;
	}
	virtual TiXmlElement* getXmlElem(Ogre::Vector3 startPosition = Ogre::Vector3::ZERO,bool savegame=false);
	//Ogre::BillboardSet *mBBset;
private:
	//begrenzte lebensdauer für missiles
	Ogre::Real lifetime;
	SimpleSound::Source *mSnd;
	//OgreAL::Sound *mSnd;
	//MissileData mData;
	//das Ding, was diese missile spawnt. spell oder item
	gamedata_damagedealer *mCastingItem;
	//das objekt, welches es castet
	//ObjectPtr mCaster;
	GameObject *mCaster;
	//SpecificMissileData mData;
	bool hasCollided; //damit onCollision nur 1x passiert
	void init(GameObject * caster, gamedata_damagedealer *source, Level *lvl, Ogre::Vector3 pos, Ogre::Quaternion ornt, Ogre::Vector3 v0 = Ogre::Vector3::ZERO,int SaveGameID = 0);
};

#endif