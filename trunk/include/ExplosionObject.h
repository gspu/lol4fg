#ifndef __praSpecialEffect
#define __praSpecialEffect

#include "Ogre.h"

#include "FwDec.h"
////#include "OgreAL.h"
#include "GameObject.h"
#include "StandardApplication.h"
#include "Sound.h"
#include "SmallClasses.h"


//ExplosionObject kann nun 2 Dinge tun: entweder direkt als Spell wirken, oder nur einen Effekt 
//(zB eines Missile oder Beam) darstellen. abhängig davon ist effectOnly true oder false
//der Pointer mCastingItem muss aber gesetzt sein, da es sonst keine ExplosionData kriegt
class ExplosionObject: public GameObject
{
public:
	ExplosionObject(GameObject *caster,GameObject *target,gamedata_damagedealer *castingItem,Level *lvl,Ogre::Vector3 pos,bool effect_only = false, bool liveForever = false,Ogre::Quaternion ornt = Ogre::Quaternion::IDENTITY,Ogre::Vector3 scale = Ogre::Vector3::UNIT_SCALE,int SaveGameID = 0)
	{
		init(ObjectPtr(caster),target,castingItem,lvl,pos,effect_only,liveForever,ornt,scale);
	}
	ExplosionObject(TiXmlElement *XMLNode,Level *lvl,Ogre::Vector3 positionOffset = Ogre::Vector3::ZERO);
	~ExplosionObject();
	GameObject* clone(Level *targetLvl = NULL){return NULL;};
	void update(Ogre::Real time);
	gamedata *getData()
	{
		return NULL;
	}
	virtual TiXmlElement* getXmlElem(Ogre::Vector3 startPosition = Ogre::Vector3::ZERO,bool savegame=false);
	Ogre::String getDisplayName()
	{
		return "(ex)"+ID;
	}
private:
	//ExplosionData myData;
	Ogre::Real lifetime;
	SimpleSound::Source *mSnd;
	void init(ObjectPtr caster,ObjectPtr target,gamedata_damagedealer *castingItem,Level *lvl,Ogre::Vector3 pos,bool effect_only = false, bool liveForever = false,Ogre::Quaternion ornt = Ogre::Quaternion::IDENTITY,Ogre::Vector3 scale = Ogre::Vector3::UNIT_SCALE,int SaveGameID = 0);
	//das Ding, was diese Explosion spawnt.
	gamedata_damagedealer *mCastingItem;
	//das objekt, welches es castet
	ObjectPtr mCaster;
	//das Target. kann auch NULL sein
	ObjectPtr mTarget;
    //ob es nur den Effekt darstellt
	bool effectOnly;
	//ob es ewig lebt, dh ob das erstellende objekt dafür zuständig ist, es zu löschen (beam zB)
	bool noLifeTime;
	
};

#endif