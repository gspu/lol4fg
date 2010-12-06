#ifndef __praSpellbook
#define __praSpellbook
#include "Ogre.h"
#include "StandardApplication.h"
/*Idee ist die: 
-man hat so viel felder wie man spells hat. bis zu MAXINT
-man kann jeden spell nur 1x haben
-man kanns spells nicht ablegen
-spells löschen? wenn (1) gilt, nicht nötig
-das spellbook resized sich automatisch auf anz.spells +1
*/


class Spellbook
{
public:
	typedef std::vector<gamedata_spell*> SpellVector;
	
	Spellbook(){spellcnt = 0;}//erstmal nix
	Spellbook(Spellbook *sp);//zum kopieren
	~Spellbook(){}//auch nix
	
	bool hasSpell(Ogre::String spellID);
	bool hasSpell(gamedata_spell *spell);
	//gibt false zurück, wenn der spell schon vorhanden
	bool addSpell(Ogre::String  spellID);
	bool addSpell(gamedata_spell *spell);
	//gibt false zurück, wenn spell nicht vorhanden
	//wenn bool erase auf true gesetzt wird, wird die stelle komplett gelöscht,
	//dh sodass die danach nachrücken. sonst wird es einfach auf "" gesetzt
	bool removeSpell(Ogre::String spellID, bool erase = false);
	bool removeSpell(gamedata_spell *spell, bool erase = false);
	//setzt den spell ohne wenn und aber
	void setAt(size_t pos,Ogre::String spellID);
	void setAt(size_t pos,gamedata_spell *spell);
	//gibt spellID an best. Position, oder "" wenn da nix ist
	Ogre::String getAtId(size_t pos);
	gamedata_spell *getAt(size_t pos);
	void resyncCount();
	//gesamtanzahl der Zauber
	inline size_t getSpellCnt()
	{
		//return spellVector.size();
		resyncCount();
		return spellcnt;
	}
	//löscht alle spells
	void clear()
	{
		spellVector.clear();
		spellcnt = 0;
	}
	SpellVector spellVector;
protected:
	size_t spellcnt;
};

#endif
