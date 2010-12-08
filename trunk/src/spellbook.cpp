#include "spellbook.h"

Spellbook::Spellbook(Spellbook *sp)
{
	spellcnt = sp->spellcnt;
	std::vector<gamedata_spell*>::const_iterator itr;
	for(itr = sp->spellVector.begin();itr != sp->spellVector.end();itr++)
	{
		gamedata_spell* cur = *itr;
		spellVector.push_back(cur);
	}
}

void Spellbook::resyncCount()
{
	std::vector<gamedata_spell*>::const_iterator itr;
	spellcnt = 0;
	for(itr = spellVector.begin();itr != spellVector.end();itr++)
	{
		gamedata_spell* cur = *itr;
		if(cur)
			spellcnt++;
		
	}
}

bool Spellbook::hasSpell(Ogre::String spellID)
{
	std::vector<gamedata_spell*>::const_iterator itr;
	for( itr = spellVector.begin(); itr != spellVector.end(); itr++)
	{
		gamedata_spell* cur = *itr;
		if(cur->id == spellID)
			return true;
	}
	return false;

}
bool Spellbook::hasSpell(gamedata_spell* spell)
{
	std::vector<gamedata_spell*>::const_iterator itr;
	for( itr = spellVector.begin(); itr != spellVector.end(); itr++)
	{
		gamedata_spell* cur = *itr;
		if(cur == spell)
			return true;
	}
	return false;

}

bool Spellbook::addSpell(Ogre::String spellID)
{
	if(hasSpell(spellID))
		return false;
	else
	{
		spellcnt++;
		gamedata_spell *sp = StandardApplication::getSingletonPtr()->getSpellDataPtr(spellID);
		spellVector.push_back(sp);
		return true;
	}

}
bool Spellbook::addSpell(gamedata_spell *spell)
{
	if(hasSpell(spell))
		return false;
	else
	{
		spellcnt++;
		spellVector.push_back(spell);
		return true;
	}

}
//gibt false zurück, wenn spell nicht vorhanden
bool Spellbook::removeSpell(Ogre::String spellID, bool erase)
{
	std::vector<gamedata_spell*>::iterator itr;
	for( itr = spellVector.begin(); itr != spellVector.end(); itr++)
	{
		gamedata_spell *sp = *itr;
		if(sp->id == spellID)
		{
			spellcnt--;
			if(erase)
				spellVector.erase(itr);
			else
				*itr = NULL;
			return true;
		}

	}
	return false;
	
}
bool Spellbook::removeSpell(gamedata_spell* spell, bool erase)
{
	std::vector<gamedata_spell*>::iterator itr;
	for( itr = spellVector.begin(); itr != spellVector.end(); itr++)
	{
		if(*itr == spell)
		{
			spellcnt--;
			if(erase)
				spellVector.erase(itr);
			else
				*itr = NULL;
			return true;
		}

	}
	return false;
	
}
//setzt den spell ohne wenn und aber
void Spellbook::setAt(size_t pos,Ogre::String spellID)
{
	while(pos >= spellVector.size())
	{
		spellVector.push_back(NULL);
	}
	if(pos > spellcnt)
		spellcnt = pos;
	gamedata_spell *sp = StandardApplication::getSingletonPtr()->getSpellDataPtr(spellID);
	spellVector[pos] = sp;
}
void Spellbook::setAt(size_t pos,gamedata_spell *spell)
{
	while(pos >= spellVector.size())
	{
		spellVector.push_back(NULL);
	}
	if(pos > spellcnt)
		spellcnt = pos;
	spellVector[pos] = spell;
}
//gibt spellID an best. Position, oder "" wenn da nix ist
Ogre::String Spellbook::getAtId(size_t pos)
{
	if(pos >= spellVector.size())
	{
		return "";
	}
	else
		return spellVector[pos]->id;
}
gamedata_spell* Spellbook::getAt(size_t pos)
{
	if(pos >= spellVector.size())
	{
		return NULL;
	}
	else
		return spellVector[pos];
}