#ifndef praBeamObject
#define praBeamObject

#include "Ogre.h"

#include "FwDec.h"
////#include "OgreAL.h"
#include "GameObject.h"
#include "StandardApplication.h"
#include "Sound.h"
//#include "OgreNewt.h"
#include "smallclasses.h"


class BeamObject: public GameObject
{
public:
	BeamObject(GameObject *_caster, gamedata_damagedealer *_source, Level *lvl,int SaveGameID = 0)
	{
		init(_caster,_source,lvl,SaveGameID);
	}
	GameObject *clone(Level *targetLevel = NULL)
	{
		return NULL;
	}
	void update(Ogre::Real time);
	void setScale(Ogre::Vector3 scale);
	void setLength(Ogre::Real length);
	~BeamObject();

	Ogre::String getDisplayName()
	{
		return ID;
	}
private:
	void init(GameObject *_caster, gamedata_damagedealer *_source, Level *lvl,int SaveGameID = 0);
	gamedata_damagedealer *source;
	GameObject * caster;
	Ogre::Real lastLength;
	//Ogre::MaterialPtr myMat;
	Ogre::ManualObject *manObj;
	int currentNumSections;
    Ogre::BillboardChain *bbChain;
	SimpleSound::Source *mSnd;
	ExplosionObject *mEffect;
	Ogre::Real lifetime;
};

#endif