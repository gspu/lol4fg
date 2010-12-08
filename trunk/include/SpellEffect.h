#ifndef __spelleffect
#define __spelleffect

#include "data.h"
#include "gameChar.h"


class SpellObject;

class SpellEffect
{
public:
	virtual SpellEffectType getType()
	{
		return mData.type;
	}
	//standard initialisierung. wird aufgerufen, wenn der effekt etwas trifft
	virtual void init(GameObject *caster, GameObject *target){};
	//initialisierung, wenn aus spielstand geladen
	virtual void savegame_init(GameObject *caster, GameObject *target)
	{
		init(caster, target);
	}
	//de-initialisierung, wenn effekt zu ende
	virtual void deInit(GameObject *caster, GameObject *target){};
	//wird 1x pro update aufgerufen
	virtual void affect(GameObject *caster, GameObject *target){};
	


	SpellEffectData mData;
	SpellObject *parent;
	

};


//fügt schaden hinzu. wenn duration = 0, dann nur einmal, sonst immer wieder im Abstand von delay (ANM: soll constante sein)
//bis duration vorbei ist
class SpellEffect_Damage: public SpellEffect
{
public:
	SpellEffect_Damage(){};
	~SpellEffect_Damage(){};
	void init(GameObject *caster, GameObject *target)
	{
		affect(caster,target);
	}
	void affect(GameObject *caster, GameObject *target);
	void savegame_init(GameObject *caster, GameObject *target)
	{
		init(caster, target);
	}

	/*void init(SpellEffectData data,GameChar *target,GameChar *caster);*/
};


//ändert für die wirkungsdauer die Stats von Target
class SpellEffect_ModStats: public SpellEffect
{
public:
	SpellEffect_ModStats(){};
	~SpellEffect_ModStats(){};
	virtual void init(GameObject *caster, GameObject *target);
	virtual void deInit(GameObject *caster, GameObject *target);
	void savegame_init(GameObject *caster, GameObject *target)
	{
		init(caster, target);
	}
private:
	/*void effect();
	void init(SpellEffectData data,GameChar *target,GameChar *caster);*/
};

class SpellEffect_Absorb: public SpellEffect
{
public:
	void affect(GameObject *caster, GameObject *target);

};

class SpellEffect_Script: public SpellEffect
{
public:
	void init(GameObject *caster, GameObject *target){};
	void deInit(GameObject *caster, GameObject *target){};
	void affect(GameObject *caster, GameObject *target){};

};


class SpellObject
{
	friend class StandardApplication;
public:
	SpellObject(gamedata_spell *data,GameObject *_caster, GameObject *_target,bool savegame=false,Ogre::Real lifetime=0);
	~SpellObject();
	void update(Ogre::Real time);
	typedef std::list<SpellEffect*> EffectList;
	inline gamedata_spell *getData()
	{
		return mData;
	}
	//diese vars sind für das aus dem SG laden
	int caster_sgid;
	int target_sgid;
	//diese levelvar ist für den alleinigen zweck, um bei bedarf die echten pointer holen zu können
	Level *lvl;
private:
	SpellEffect *createSpellEffect(SpellEffectData data);
	

	EffectList mEffects;

	gamedata_spell *mData;
	Ogre::Real lifetime;
	GameObject *target;
	GameObject *caster;
	Ogre::Real delay;
	Ogre::Real lastDelayTime;
	

};


#endif