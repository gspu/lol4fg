#ifndef __mSoundListener
#define __mSoundListener
#include "alc.h"
#include "al.h"
#include "Ogre.h"
#include "Sound.h"
#include "OgreSingleton.h"


namespace SimpleSound
{

	class Listener: public Ogre::Singleton<Listener>
	{
		friend class SoundManager;
	public:
		inline void setPosition(Ogre::Vector3 pos)
		{
			alGetError();
			alListener3f(AL_POSITION,pos.x,pos.y,pos.z);
			if(alGetError())
			{
				Ogre::LogManager::getSingletonPtr()->logMessage("WAHH ListenerSetPos problem!");
			}
		}
		inline void setVelocity(Ogre::Vector3 vel)
		{
			alListener3f(AL_VELOCITY,vel.x,vel.y,vel.z);
		}
		inline void setOrientation(Ogre::Quaternion ornt)
		{
			alGetError();
			Ogre::Vector3 fore = ornt*Ogre::Vector3::NEGATIVE_UNIT_Z;
			Ogre::Vector3 up   = ornt*Ogre::Vector3::UNIT_Y;
			float vec[6];
			vec[0] = fore.x;
			vec[1] = fore.y;
			vec[2] = fore.z;
			vec[3] = up.x;
			vec[4] = up.y;
			vec[5] = up.z;
			alListenerfv(AL_ORIENTATION, vec);
			if(alGetError())
			{
				Ogre::LogManager::getSingletonPtr()->logMessage("WAHH ListenerSetOrnt problem!");
			}
		}

        inline Ogre::Vector3 getPosition()
		{
			alGetError();
            Ogre::Vector3 pos;
			alGetListener3f(AL_POSITION,&pos.x,&pos.y,&pos.z);
			return pos;
		}
		inline Ogre::Vector3 getVelocity()
		{
            Ogre::Vector3 vel;
			alListener3f(AL_VELOCITY,vel.x,vel.y,vel.z);
            return vel;
		}
		inline Ogre::Quaternion getOrientation()
		{
			alGetError();
			
			float vec[6];
			
			alGetListenerfv(AL_ORIENTATION, vec);
            Ogre::Vector3 fore;
			Ogre::Vector3 up;
            fore.x = vec[0];
			fore.y = vec[1];
			fore.z = vec[2];
			up.x   = vec[3];
			up.y   = vec[4];
			up.z   = vec[5];
            fore.normalise();
            up.normalise();
            Ogre::Vector3 xAxis = up.crossProduct(-fore);
            xAxis.normalise();

            return Ogre::Quaternion(xAxis,up,-fore);
			
		}
		//sets the gain for the Listener. This is something like the global gain
		inline void setGain(Ogre::Real gain)
		{
			alListenerf( AL_GAIN, gain );
		}
	private:
		Listener(){};
	};
}
#endif