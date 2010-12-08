#ifndef __mSoundSrc
#define __mSoundSrc
#include "alc.h"
#include "al.h"
#include "Ogre.h"
#include "Sound.h"
#include "Buffer.h"

namespace SimpleSound
{
	class Source
	{
		friend class SoundManager;
	public:
		enum SourceState
		{
			Null    = 0,
			Initial = 1,
			Playing = 2,
			Paused  = 3,
			Stopped = 4
		};
		inline void setPosition(Ogre::Vector3 pos)
		{
			alSource3f(alSource, AL_POSITION,pos.x,pos.y,pos.z);
		}
        inline Ogre::Vector3 getPosition()
		{
            Ogre::Vector3 pos;
			alGetSource3f(alSource, AL_POSITION,&pos.x,&pos.y,&pos.z);
            return pos;
		}
		inline void setOrientation(Ogre::Quaternion ornt)
		{
			Ogre::Vector3 fore = ornt*Ogre::Vector3::NEGATIVE_UNIT_Z;
			/*Ogre::Vector3 up   = ornt*Ogre::Vector3::UNIT_Y;
			float vec[6];
			vec[0] = fore.x;
			vec[1] = fore.y;
			vec[2] = fore.z;
			vec[3] = up.x;
			vec[4] = up.y;
			vec[5] = up.z;*/
			alSource3f(alSource, AL_DIRECTION,fore.x,fore.y,fore.z);
		}
        inline Ogre::Quaternion getOrientation()
		{
			Ogre::Vector3 fore;/* = ornt*Ogre::Vector3::NEGATIVE_UNIT_Z;*/
			/*Ogre::Vector3 up   = ornt*Ogre::Vector3::UNIT_Y;
			float vec[6];
			vec[0] = fore.x;
			vec[1] = fore.y;
			vec[2] = fore.z;
			vec[3] = up.x;
			vec[4] = up.y;
			vec[5] = up.z;*/
			alGetSource3f(alSource, AL_DIRECTION,&fore.x,&fore.y,&fore.z);
            return Ogre::Vector3::NEGATIVE_UNIT_Z.getRotationTo(fore);
		}
		inline void setVelocity(Ogre::Vector3 vel)
		{
			alSource3f(alSource, AL_VELOCITY,vel.x,vel.y,vel.z);
		}
        inline Ogre::Vector3 getVelocity()
		{
            Ogre::Vector3 vel;
			alGetSource3f(alSource, AL_VELOCITY,&vel.x,&vel.y,&vel.z);
            return vel;
		}
		void setBuffer(Buffer *buffer);

		inline Buffer *getBuffer()
		{
			return mBuffer;
		}
		

		/* Play, replay, or resume a Source */
		inline void play()
		{
			if(!mBuffer)
				return;
            if(!alSource)
            {
                if(bak && bak->relToListener)
                    retrieveALSource(2);
                else
                    retrieveALSource(1);
            }
			alGetError();
			alSourcePlay( alSource );
			if(alGetError())
			{
				Ogre::LogManager::getSingletonPtr()->logMessage("play error");
			}
		}
		/* Pause a Source */
		inline void pause()
		{
			if(!mBuffer)
				return;
			alGetError();
			alSourcePause( alSource );
		}
		/* Stop a Source */
		inline void stop()
		{
			if(!mBuffer)
				return;
			alGetError();
			alSourceStop( alSource );
		}
		/* Rewind a Source (set playback postiton to beginning) */
		inline void rewind()
		{
			if(!mBuffer)
				return;
			alSourceRewind( alSource );
		}
		inline bool isPlaying()
		{
			return getSourceState() == Playing;
		}
		inline bool isStopped()
		{
			return getSourceState() == Stopped;
		}
		inline bool isPaused()
		{
			return getSourceState() == Paused;
		}
		inline SourceState getSourceState()
		{
			ALint temp;
			alGetSourcei( alSource,  AL_SOURCE_STATE, &temp );
			switch(temp)
			{
			case AL_INITIAL:
				return Initial;
			case AL_PLAYING:
				return Playing;
			case AL_PAUSED:
				return Paused;
			case AL_STOPPED:
				return Stopped;
			}
			return Null;
		}
		
		inline void setGain(Ogre::Real gain)
		{
			alGetError();
			alSourcef(alSource,AL_GAIN,gain);
			if(alGetError())
			{
				Ogre::LogManager::getSingletonPtr()->logMessage("could not set gain. WTF?");
			}
		}
		inline void setMinGain(Ogre::Real gain)
		{
			alSourcef(alSource,AL_MIN_GAIN,gain);
		}
		inline void setMaxGain(Ogre::Real gain)
		{
			alSourcef(alSource,AL_MAX_GAIN,gain);
		}
		inline void setRelativeToListener(bool set)
		{
			alSourcei(alSource,AL_SOURCE_RELATIVE,set?AL_TRUE:AL_FALSE);
		}
		inline void setReferenceDistance(Ogre::Real dist)
		{
			alSourcef(alSource,AL_REFERENCE_DISTANCE,dist);
		}
		inline void setMaxDistance(Ogre::Real dist)
		{
			alSourcef(alSource,AL_MAX_DISTANCE,dist);
		}
		inline void setRolloffFactor(Ogre::Real factor)
		{
			alSourcef(alSource,AL_ROLLOFF_FACTOR,factor);
		}
		inline void setConeInnerAngle(Ogre::Real angle)
		{
			alSourcef(alSource,AL_CONE_INNER_ANGLE,angle);
		}
		inline void setConeOuterAngle(Ogre::Real angle)
		{
			alSourcef(alSource,AL_CONE_OUTER_ANGLE,angle);
		}
		inline void setConeOuterGain(Ogre::Real gain)
		{
			alSourcef(alSource,AL_CONE_OUTER_GAIN,gain);
		}
		inline void setPitch(Ogre::Real pitch)
		{
			alSourcef(alSource,AL_PITCH,pitch);
		}
		inline void setLoop(bool set)
		{
			alSourcei(alSource,AL_LOOPING,set?AL_TRUE:AL_FALSE);
		}
		/*THis is called Millisec. Offset in the OpenAL documentation, 
		but the variable is called SEC_OFFSET, so no idea what exactly this is*/
		inline void setMsecOffset(Ogre::Real offset)
		{
			alSourcef(alSource,AL_SEC_OFFSET,offset);
		}
		inline void setByteOffset(Ogre::Real offset)
		{
			alSourcef(alSource,AL_BYTE_OFFSET,offset);
		}
		inline void setSampleOffset(Ogre::Real offset)
		{
			alSourcef(alSource,AL_SAMPLE_OFFSET,offset);
		}

		inline Ogre::Real getGain()
		{
			Ogre::Real gain = 0;
			alGetSourcef(alSource,AL_GAIN,&gain);
			return gain;
		}


		inline Ogre::Real getMinGain()
		{
			Ogre::Real gain = 0;
			alGetSourcef(alSource,AL_MIN_GAIN,&gain);
			return gain;
		}


		inline Ogre::Real getMaxGain()
		{
			Ogre::Real gain = 0;
			alGetSourcef(alSource,AL_MAX_GAIN,&gain);
			return gain;
		}


		inline bool getRelativeToListener()
		{
			int set = 0;		
			alGetSourcei(alSource,AL_SOURCE_RELATIVE,&set);
			return set==AL_TRUE?true:false;
		}


		inline Ogre::Real getReferenceDistance()
		{
			Ogre::Real dist = 0;
			alGetSourcef(alSource,AL_REFERENCE_DISTANCE,&dist);
			return dist;
		}


		inline Ogre::Real getMaxDistance()
		{
			Ogre::Real dist = 0;
			alGetSourcef(alSource,AL_MAX_DISTANCE,&dist);
			return dist;
		}


		inline Ogre::Real getRolloffFactor()
		{
			Ogre::Real factor = 0;
			alGetSourcef(alSource,AL_ROLLOFF_FACTOR,&factor);
			return factor;
		}


		inline Ogre::Real getConeInnerAngle()
		{
			Ogre::Real angle = 0;
			alGetSourcef(alSource,AL_CONE_INNER_ANGLE,&angle);
			return angle;
		}


		inline Ogre::Real getConeOuterAngle()
		{
			Ogre::Real angle = 0;
			alGetSourcef(alSource,AL_CONE_OUTER_ANGLE,&angle);
			return angle;
		}


		inline Ogre::Real getConeOuterGain()
		{
			Ogre::Real gain = 0;
			alGetSourcef(alSource,AL_CONE_OUTER_GAIN,&gain);
			return gain;
		}


		inline Ogre::Real getPitch()
		{
			Ogre::Real pitch = 0;
			alGetSourcef(alSource,AL_PITCH,&pitch);
			return pitch;
		}


		inline bool getLoop()
		{
			int set = 0;				
			alGetSourcei(alSource,AL_LOOPING,&set);
			return set==AL_TRUE?true:false;
		}


		inline Ogre::Real getMsecOffset()
		{
			Ogre::Real offset = 0;
			alGetSourcef(alSource,AL_SEC_OFFSET,&offset);
			return offset;
		}


		inline Ogre::Real getByteOffset()
		{
			Ogre::Real offset = 0;
			alGetSourcef(alSource,AL_BYTE_OFFSET,&offset);
			return offset;
		}


		inline Ogre::Real getSampleOffset()
		{
			Ogre::Real offset = 0;
			alGetSourcef(alSource,AL_SAMPLE_OFFSET,&offset);
			return offset;
		}

		//sets a custom "type" for this source
		//can be used to selectively set gain
		//via SoundManager::setTypeGain
		inline void setType(int set)
		{
			type = set;
		}
		inline int getType()
		{
			return type;
		}
        //versucht, sich eine neue AL-Source zu holen, falls die alte fehlt
		void retrieveALSource(int priority=0);
		/*

	 * Attached Buffer                   AL_BUFFER               ALint
	 * State (Query only)                AL_SOURCE_STATE         ALint
	 * Buffers Queued (Query only)       AL_BUFFERS_QUEUED       ALint
	 * Buffers Processed (Query only)    AL_BUFFERS_PROCESSED    ALint
	 */
        

	private:
        struct SourceBackup
        {
            //diese Werte sind Backup, für den fall das die ALSource gestohlen wird
            Ogre::Vector3 mPos;
            Ogre::Vector3 mVel;
            Ogre::Quaternion mOrient;
            Ogre::Real mGain;
            Ogre::Real mMinGain;
            Ogre::Real mMaxGain;
            bool relToListener;
            Ogre::Real coneInnerAngle;
            Ogre::Real coneOuterAngle;
            Ogre::Real coneOuterGain;
            Ogre::Real pitch;
            bool loop;
        };
        SourceBackup *bak;
		Source(ALuint src);
        ~Source();
        //klaut die source, sichert aber vorher ihre stats
        ALuint stealALSource();
        void setALSource(ALuint src);
		ALuint alSource;
		Buffer *mBuffer;
		int type;
		//this is for SoundManager::pauseAllSounds
		//if this is set to true, then resumeAllSounds will make this sound play
		bool pausedBySoundMgr;
	};
}
#endif