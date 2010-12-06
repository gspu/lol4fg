#ifndef __hud
#define __hud

#include <InventoryDisplayWindow.h>
#include "gamedata.h"

/*
Class for handling the HUD
*/
class HUD: public GameWindow
{
public:
	HUD();

	~HUD();



	void updateStatBars(Ogre::Real hp, Ogre::Real mp, int maxHP, int maxMP);


	
	void setHotkey(unsigned int index, gamedata_hotkeyable *item = NULL);

    gamedata_hotkeyable *getHotkey(unsigned int index)
	{
		assert(index < 10);
		return hotkey[index];
	}

	//triggers the apropriate hotkey
	//index has to be between 0 and 9 and represent the key pressed
	//meaning, 0 will trigger the tenth hotkey
    bool useHotkey(unsigned int index);

	

	void createCompass();

	void setCompassOrientation(Ogre::Quaternion orient);

	void setDisplayedWeapon(gamedata_item* item);

	void setDisplayedSpell(gamedata_spell* spell);

	void setDebugText(Ogre::String str,bool add=false);

	//void showScreenText(String str);

private:

	CEGUI::UDim hpBarWidth;
	CEGUI::UDim manaBarWidth;

	CEGUI::Window *healthBar;
	CEGUI::Window *manaBar;

	CEGUI::Window *healthBarContainer;
	CEGUI::Window *manaBarContainer;

	CEGUI::Window *currentSpell;
	CEGUI::Window *currentWeapon;
	//exact key mapping, hotkey[0] corresponds to the zero key
	//these are the hotkey windows
	CEGUI::Window *hotkeyWindow[10];
	//these are the actual hotkeyed items
    gamedata_hotkeyable* hotkey[10];

	CEGUI::Window *textDisplayWindow;

	

	CEGUI::Window *compass;

	Ogre::SceneManager *compSceneMgr;
	Ogre::SceneNode *compNeedleNode;
	Ogre::Camera *compassCam;

	gamedata_item* curDisplayedWeapon;
	gamedata_spell* curDisplayedSpell;

protected:
	
	bool eventHotkeyClicked(const CEGUI::EventArgs& e);
	bool eventWeaponClicked(const CEGUI::EventArgs& e);
	bool eventSpellClicked(const CEGUI::EventArgs& e);
	
	
};


#endif