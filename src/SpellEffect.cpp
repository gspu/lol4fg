#include "FwDec.h"
#include "SpellEffect.h"
#include "Level.h"

SpellEffect* SpellObject::createSpellEffect(SpellEffectData data)
{
	SpellEffect *res = NULL;
	switch(data.type)
	{
	case seAbsorb:
		res = NULL;//new SpellEffect_Absorb(data,target,caster);
		break;
	case seModStats:
		res = new SpellEffect_ModStats();
		break;
	case seDamage:
		res = new SpellEffect_Damage();
		break;
	case seScript:
		res = NULL;//new SpellEffect_Script(data,target,caster);
		break;
	default:
		res = NULL;
	}
	if(res)
	{
		res->mData = data;
		res->parent = this;
	}
	return res;
}

SpellObject::SpellObject(gamedata_spell *data, GameObject *_caster, GameObject *_target,bool savegame,Ogre::Real _lifetime)
{
	if(!data)
	{
		throw etInvalidId;
	}
	mData = data;
	caster = _caster;
	target = _target;
	lifetime = _lifetime;
	delay = 1;
	lastDelayTime = 0;
	lvl = NULL;
	caster_sgid = 0;
	target_sgid = 0;
	
	for(EffectDataList::const_iterator itr = mData->effects.begin();itr != mData->effects.end();itr++)
	{
		SpellEffect *spell = createSpellEffect(*itr);
		if(spell)
		{
			if(savegame)
				spell->savegame_init(caster,target);
			else
				spell->init(caster,target);
			if(mData->duration == 0)
			{
				//es wird sofort wieder gelöscht
				spell->deInit(caster,target);
				delete spell;
			}
			else
			{
				mEffects.push_back(spell);
			}
		}
	}
	if(mData->duration == 0)
	{
		throw etInvalidId; //AddSpell muss es fangen
		//target->RemoveSpell(this);
	}
}

SpellObject::~SpellObject()
{
	for(EffectList::const_iterator itr = mEffects.begin();itr != mEffects.end();itr++)
	{
		SpellEffect *spell = *itr;
		if(spell)
		{
			spell->deInit(caster,target);
			delete spell;
		}
	}
}

void SpellObject::update(Ogre::Real time)
{
	if(caster_sgid && !caster)
	{
		//dh der eigentliche caster konnte nicht geholt werden
		caster = lvl->getObjectBySGID(caster_sgid);
	}
	if(target_sgid && !target)
	{
		//dh der eigentliche target konnte nicht geholt werden
		target = lvl->getObjectBySGID(target_sgid);
	}
	lifetime += time;
	if(lifetime >= mData->duration)
	{
		lastDelayTime -= (lifetime - mData->duration);
		target->RemoveSpell(this);
	}
	lastDelayTime += time;
	while(lastDelayTime > delay)
	{
		for(EffectList::const_iterator itr = mEffects.begin();itr != mEffects.end();itr++)
		{
			(*itr)->affect(caster,target);
		}
		lastDelayTime -= delay;
	}


}

void SpellEffect_ModStats::init(GameObject *caster, GameObject *target)
{
	if(target->getType() != otChar)
		return;
	GameChar *ch = (GameChar*)target;
	ch->addStatList(mData.stats);
}

void SpellEffect_ModStats::deInit(GameObject *caster, GameObject *target)
{
	if(target->getType() != otChar)
		return;
	GameChar *ch = (GameChar*)target;
	ch->removeStatList(mData.stats);
}

void SpellEffect_Damage::affect(GameObject *caster, GameObject *target)
{
	target->onDamage(mData.dmg,caster);
}
//
//void SpellEffect_Damage::update(Ogre::Real time)
//{
//	lifetime += time;
//	if(lifetime >= mData.duration)
//	{
//		lastDelayTime -= (lifetime - mData.duration);
//		target->RemoveEffect(this);
//	}
//	lastDelayTime += time;
//	while(lastDelayTime > delay)
//	{
//		effect();
//		lastDelayTime -= delay;
//	}
//
//}

//SpellEffect_ModStats::SpellEffect_ModStats()
//{
//
//}	

//SpellEffect_ModStats::~SpellEffect_ModStats()
//{
//	//entfernen
//}

//void SpellEffect_ModStats::update(Ogre::Real time)
//{
//	lifetime += time;
//	if(lifetime >= mData.duration)
//	{
//		//removestats?
//		target->RemoveEffect(this);
//	}
//}
//
//void SpellEffect::update(Ogre::Real time)
//{
//	lifetime += time;
//	if(lifetime >= mData.duration)
//		target->RemoveEffect(this);
//}