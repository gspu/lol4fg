#ifndef __mSoundBuffer
#define __mSoundBuffer

#include "alc.h"
#include "al.h"
#include "Ogre.h"


namespace SimpleSound
{
	/*Class to store buffers.

	*/
	class Buffer
	{
		friend class SoundManager;
	public:
		
		
		inline ALuint getBuffer()
		{
			return buffer;
		}
		inline Ogre::String getFileName()
		{
			return mFileName;
		}
		inline int getFrequency()
		{
			int freq = 0;
			alGetBufferi( buffer, AL_FREQUENCY, &freq );
			return freq;
		}
		inline int getNumChannels()
		{
			int val = 0;
			alGetBufferi( buffer, AL_CHANNELS, &val );
			return val;
		}
		inline int getBits()
		{
			int val = 0;
			alGetBufferi( buffer, AL_BITS, &val );
			return val;
		}
		inline int getSize()
		{
			int val = 0;
			alGetBufferi( buffer, AL_SIZE, &val );
			return val;
		}
		
		//sets a custom "type" for this buffer
		inline void setType(int set)
		{
			type = set;
		}
		inline int getType()
		{
			return type;
		}

		
	private:
		Buffer(Ogre::String filename,ALint buf)
		{
			mFileName = filename;
			buffer = buf;
			type = 0;
		}
		Ogre::String mFileName;
		ALuint buffer;
		int type;
		
		//unsigned char numBuffers;
		
	};

}
#endif