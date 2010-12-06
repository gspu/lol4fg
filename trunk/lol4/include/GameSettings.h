#ifndef _gameSettings
#define _gameSettings

#include <Ogre.h>
#include <defines.h>
#include <UniKey.h>
#include "tinyxml.h"
#include "SoundManager.h"
/*
	Class for storing settings for the game. Made it as a class and not a struct to make it
	easier adding load/save functions, should I decide to add them here
*/

#define NUM_KEYS 25
//names for the keys
enum KeyName
{
	keyNone			= 0,
	keyMoveForward	= 1,
	keyMoveBack		= 2,
	keyMoveRight	= 3,
	keyMoveLeft		= 4,
	keyJump			= 5,
	keyAttack		= 6,
	keySpellcast	= 7,
	keyRun			= 8,
	keyAction		= 9,
	keyInventory	= 10,
	keyInvTakeOne	= 11,
	keyInvTakeAll	= 12,
	keyInvUse		= 13,
	keySpellbook	= 14,
	keyScreenshot	= 15,
	keyMainmenu		= 16,
	keyLookUp		= 17,
	keyLookDown		= 18,
	keyLookLeft		= 19,
	keyLookRight	= 20,
	keyToggleMouselooking=21,
	//this is for debugging
	keyDebug3rdp	= 22,
	keyDebugshowlines=23,
	//more keys
	keyDragItem		= 24


};

//simple struct to hold 2 keys
struct KeyPair
{
	KeyPair(UniKey mainKey,UniKey altKey)
	{
		main = mainKey;
		alt = altKey;
	}
	KeyPair()
	{
		//nothing oO
	}
	//the main key
	UniKey main;
	//the alternative key
	UniKey alt;
	/*KeyPair operator = (KeyPair other)
	{
		main = other.main;
		alt = other.alt;
		
		return *this;
	}*/
};


class GameSettings
{
public:
	//use mouse to look around (FPS-like)?
	bool mouselooking;
	//sensibilities for x and y when looking with the mouse
	Ogre::Real sensibility_x_mouse;
	Ogre::Real sensibility_y_mouse;
	//sensibilities for x and y when looking using the keyboard
	Ogre::Real sensibility_x_keyboard;
	Ogre::Real sensibility_y_keyboard;
	//General gain
	Ogre::Real gain_all;
	//Gain for sound effects
	Ogre::Real gain_sfx;
	//Gain for speech
	Ogre::Real gain_speech;
	//Gain for music
	Ogre::Real gain_music;
	//Gain for player sound effects, like inventory click sound
	Ogre::Real gain_player_sfx;
	//OpenAL settings
    Ogre::Real rolloff_factor;
    Ogre::Real ref_distance;
    Ogre::Real max_distance;
	//Distance Model for OpenAL
	SimpleSound::SoundManager::DistanceModel distance_model;
    //Filename of the starting level
	Ogre::String starting_level;
	//Name of the entrance in staring_level where the player should spawn
    Ogre::String starting_entrance;
	//default music, if none specified in the level
    Ogre::String default_music;
	Ogre::String default_combat_music;
	//physics FPS (for Newton)
	Ogre::Real physics_fps;

	//nr of keys in mainKeys and altKeys, for looping
	static const int _num_keys = 24;
	//key definitions
	KeyPair keys[_num_keys];
	////main key definitions
	//UniKey mainKeys[_num_keys];
	////alternative key definitions
	//UniKey altKeys[_num_keys];
	//unsigned int keycnt;
   
	GameSettings():		
		mouselooking(true),
		sensibility_x_mouse(0.15f),
		sensibility_y_mouse(0.15f),
		sensibility_x_keyboard(1),
		sensibility_y_keyboard(1),
		gain_all(1),
		gain_sfx(1),
		gain_speech(1),
		gain_music(1),
		gain_player_sfx(1),
		rolloff_factor(1),
		ref_distance(150),
		max_distance(3400),
		distance_model(SimpleSound::SoundManager::INVERSE_DISTANCE_CLAMPED),
		starting_level("start.zip"),//+ZLEVELEXT),
		starting_entrance("entr0"),
		default_music("default.ogg"),
		default_combat_music("default.ogg")
		
	{
		keys[keyMoveForward]	= KeyPair(OIS::KC_W,		UniKey()); 
		keys[keyMoveBack]		= KeyPair(OIS::KC_S,		UniKey()); 
		keys[keyMoveRight]		= KeyPair(OIS::KC_D,		UniKey()); 
		keys[keyMoveLeft]		= KeyPair(OIS::KC_A,		UniKey());
		keys[keyJump]			= KeyPair(OIS::KC_E,		UniKey()); 
		keys[keyAttack]			= KeyPair(OIS::MB_Left,		UniKey());
		keys[keySpellcast]		= KeyPair(OIS::MB_Right,	UniKey()); 
		keys[keyRun]			= KeyPair(OIS::KC_LSHIFT,	UniKey()); 
		keys[keyAction]			= KeyPair(OIS::KC_SPACE,	UniKey()); 
		keys[keyInventory]		= KeyPair(OIS::KC_I,		OIS::KC_TAB); 
		keys[keySpellbook]		= KeyPair(OIS::KC_M,		UniKey()); 
		keys[keyInvTakeOne]     = KeyPair(OIS::MB_Left,		UniKey()); 
		keys[keyInvTakeAll]		= KeyPair(OIS::MB_Middle,	UniKey()); 
		keys[keyInvUse]         = KeyPair(OIS::MB_Right,	UniKey()); 
		keys[keyScreenshot]		= KeyPair(OIS::KC_SYSRQ,	UniKey()); 
		keys[keyMainmenu]		= KeyPair(OIS::KC_ESCAPE,	UniKey()); 
		keys[keyLookUp]			= KeyPair(OIS::KC_PGUP,		UniKey()); 
		keys[keyLookDown]		= KeyPair(OIS::KC_PGDOWN,	UniKey()); 
		keys[keyLookLeft]		= KeyPair(OIS::KC_LEFT,		UniKey()); 
		keys[keyLookRight]		= KeyPair(OIS::KC_RIGHT,	UniKey()); 
		keys[keyToggleMouselooking] = KeyPair(OIS::KC_X,	UniKey());
		keys[keyDebug3rdp]		= KeyPair(OIS::KC_F1,		UniKey());
		keys[keyDebugshowlines] = KeyPair(OIS::KC_F2,		UniKey());
		keys[keyDragItem]		= KeyPair(OIS::KC_Z,		UniKey());
	}

	//load settings from a xml element
	//requires the <settings> element, with <options>, <keys_main> and <keys_alt>
	void loadSettings(TiXmlElement *elem);
	//save settings to a xml element. the element needs to be appended somewhere then
	TiXmlElement *getAsXml();
private:
	//function for processing keys_main and keys_alt xml element. 
	//void processKeyElem(TiXmlElement *elem);

};

#endif