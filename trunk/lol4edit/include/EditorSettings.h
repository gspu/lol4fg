#ifndef _editSettings
#define _editSettings

#include <Ogre.h>
#include <defines.h>
#include <UniKey.h>
#include "tinyxml.h"
#include "SoundManager.h"


class EditorSettings
{
public:	
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
    
   
	EditorSettings():			
		gain_all(1),
		gain_sfx(1),
		gain_speech(1),
		gain_music(1),
		gain_player_sfx(1),
		rolloff_factor(1),
		ref_distance(150),
		max_distance(3400),
		distance_model(SimpleSound::SoundManager::INVERSE_DISTANCE_CLAMPED)	
		
	{
		
	}

private:
	

};


#endif