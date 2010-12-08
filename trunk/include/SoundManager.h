#ifndef __mSoundMgr
#define __mSoundMgr

#include "alc.h"
#include "al.h"
#include "Ogre.h"
#include "Sound.h"
#include "SoundManager.h"
#include "Listener.h"
#include "Buffer.h"
#include "Source.h"
#include "OgreSingleton.h"

namespace SimpleSound
{
/*this class should be the only one to construct sources or buffers
sound should save if it should be playing*/
	class SoundManager: public Ogre::Singleton<SoundManager>
	{
        friend class Source;
	public:
		SoundManager();
		~SoundManager();
        //erstellt ein neues source-objekt
        //optionale Parameter sind nur für den Fall von Bedeutung, wenn Sources gestohlen werden müssen
        //recieverDistance ist die Entfernung des Empfängers zum Listener 
        //(damit kommt nur ein weiter entfernter Spender in Frage)
        //priority:
        //0 - nur gestoppte kommen als Spender in Frage
        //1 - gestoppte und pausierte
        //2 - auch spielende (werden gestoppt)
        //3 - auch relativeToListener
		Source *createSource(Ogre::Real recieverDistance=0, int priority = 0);
		Buffer *createBuffer(Ogre::String filename);
		void destroySource(Source *src);
		void destroyBuffer(Buffer *buff);
		inline Listener *getListener()
		{
			return listener;
		}
		





	/**
	 * Distance models
	 *
	 * used in conjunction with DistanceModel
	 *
	 * implicit: NONE, which disances distance attenuation.
	 */
		enum DistanceModel
		{
			INVERSE_DISTANCE=                       0xD001,
			INVERSE_DISTANCE_CLAMPED=               0xD002,
			LINEAR_DISTANCE=                        0xD003,
			LINEAR_DISTANCE_CLAMPED=                0xD004,
			EXPONENT_DISTANCE=                      0xD005,
			EXPONENT_DISTANCE_CLAMPED=              0xD006
		};

		/**
		 * Doppler scale.  Default 1.0
		 */
		inline void setDopplerFactor(Ogre::Real value)
		{
			alDopplerFactor( value );
		}
		inline void setDopplerVelocity(Ogre::Real value)
		{
			alDopplerVelocity( value );
		}

		inline void setSpeedOfSound(Ogre::Real value)
		{
			alSpeedOfSound( value );
		}
		inline void setDistanceModel(DistanceModel mod)
		{
			alDistanceModel( ALenum(mod) );
		}

	/**
		 * Sets the reference distance.
		 * @param refDistance The reference distance used in attenuation calculations.
		 * @note This is the distance under which the volume for the
		 *     source would normally drop by half
		 * @note Negative values will be ignored
		 */
		void setDefaultRefDistance(Ogre::Real dist,bool applyToAll = true);
	/**
		 * Sets the rolloff factor.
		 * @param rolloffFactor The rolloff rate for the source 
		 * @note Used for distance attenuation
		 * @note Negative values will be ignored
		 */
		void setDefaultRolloffFactor(Ogre::Real dist,bool applyToAll = true);
	/**
		 * Sets the Max Distance.
		 * @param maxDistance The max distance used in the Inverse Clamped Distance Model
		 * @note This is the distance where there will be no further attenuation of the source
		 * @note Negative values will be ignored
		 */
		void setDefaultMaxDist(Ogre::Real dist,bool applyToAll = true);

		//get an existing buffer
		//or create one, if none found and createIfNotFound is set to true
		Buffer *getBuffer(Ogre::String filename,bool createIfNotFound = true,int addType = 0,bool bitwiseAnd = true);

		//sets the gain for the specified type of sources.
		//the type is a custom attribute that can be set
		//through Source::setType
		/*if param bitwiseAnd is set to false, the sound type will
		be compared with param type using "==", otherwise using "&"*/
		void setTypeGain(Ogre::Real gain, int type,bool bitwiseAnd = true);

		/*destroys only Sources with a specific "type"
		if param bitwiseAnd is set to false, the sound type will
		be compared with param type using "==", otherwise using "&"
		*/
		void destroySourcesByType(int type,bool bitwiseAnd = true);
		void playByType(int type,bool bitwiseAnd = true);
		void pauseByType(int type,bool bitwiseAnd = true);
		void stopByType(int type,bool bitwiseAnd = true);
		//primarily, for setting the buffer of a certain source type to NULL
		//in order to be able to delete this buffers
		//this function calls setBuffer, which automatically stops the source
		void setBufferByType(Buffer *buff,int type,bool bitwiseAnd = true);

		/*destroys only Buffers with a specific "type"
		if param bitwiseAnd is set to false, the sound type will
		be compared with param type using "==", otherwise using "&"
		*/
		void destroyBuffersByType(int type,bool bitwiseAnd = true);

		static DistanceModel SoundManager::intToDistanceModel(int val);
		
		void pauseAllSources();
		void resumeAllSources();
		
		typedef std::list<Source*> SourceList;
		typedef std::list<Buffer*> BufferList;
	private:
		ALCdevice *device;
		ALCcontext *context;
		Listener *listener;
		
		SourceList mSources;
		//SourceList mPausedSources;
		BufferList mBuffers;
		Ogre::Real rollofFactor;
		Ogre::Real refDistance;
		Ogre::Real maxDistance;

        
	protected:
		//destroys the AL object
		bool destroyAlSource(Source *src);
		bool destroyAlBuffer(Buffer *buff);
        //creates or steals an al source
        ALuint createALSource(Ogre::Real recieverSqDistance=0, int priority = 0);
        
        
		//steals an ALSource from an alerady existant object
		//the one furthest away is used
        ALuint stealALSource(Ogre::Real recieverSqDistance=0, int priority = 0);
		
	};
}
#endif
