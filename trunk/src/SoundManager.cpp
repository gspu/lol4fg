

#include "SoundManager.h"
#include "alc.h"
//#include "alut.h"
#include "functions.h"
#include "Ogre.h"
#include "Listener.h"
#include "Source.h"
#include "vorbis/vorbisfile.h"
//using namespace Ogre;
using namespace SimpleSound;
//singleton
template<> SoundManager* Ogre::Singleton<SoundManager>::ms_Singleton = 0;

Ogre::String debugGetSourceFlags(int flags)
{
	Ogre::String res = "stMusic=";
	if(flags & stMusic)
		res += "true ";
	else
		res += "false ";

	res += "stSpeech=";
	if(flags & stSpeech)
		res += "true ";
	else
		res += "false ";
	
	res += "stSfxLevel=";
	if(flags & stSfxLevel)
		res += "true ";
	else
		res += "false ";
	
	res += "stSfxPlayer=";
	if(flags & stSfxPlayer)
		res += "true ";
	else
		res += "false ";

	res += "stOwnerIsLevel=";
	if(flags & stOwnerIsLevel)
		res += "true ";
	else
		res += "false ";
	
	res += "stGlobal=";
	if(flags & stGlobal)
		res += "true ";
	else
		res += "false ";
	return res;
	
}


/*OGG callback functions BEGIN */
size_t VorbisRead(void *ptr,           // ptr to the data that the vorbis files need
                  size_t byteSize,     // how big a byte is
                  size_t sizeToRead,   // How much we can read
                  void *datasource)    /* this is a pointer to the data we passed
                                          into ov_open_callbacks */
{ 
	Ogre::DataStreamPtr ogreFile = *reinterpret_cast<Ogre::DataStreamPtr*>(datasource);      
	return ogreFile->read(ptr,byteSize);
}
int VorbisSeek(void *datasource,   // this is a pointer to the data we passed into ov_open_callbacks 
               ogg_int64_t offset, // offset from the point we wish to seek to
               int whence)         // where we want to seek to
{
	Ogre::DataStreamPtr ogreFile = *reinterpret_cast<Ogre::DataStreamPtr*>(datasource);    
	switch(whence)
	{
	case SEEK_SET:
		ogreFile->seek(offset);
		break;
	case SEEK_END:
		ogreFile->seek(ogreFile->size()+offset);
		break;
	case SEEK_CUR:
		ogreFile->skip(offset);
		break;
	}
	return 0;
   
}
int VorbisClose(void *datasource) // this is a pointer to the data we passed into ov_open_callbacks
{
	return 0;   
}
long VorbisTell(void *datasource) // this is a pointer to the data we passed into ov_open_callbacks
{
	Ogre::DataStreamPtr ogreFile = *reinterpret_cast<Ogre::DataStreamPtr*>(datasource);    
	return ogreFile->tell();
    
}
/*OGG callback functions END */


SoundManager::SoundManager()
{
	// Initialize Open AL
	device = alcOpenDevice(NULL); // open default device
	if (device != NULL) 
	{
		context=alcCreateContext(device,NULL); // create context
		if (context != NULL) 
		{
			alcMakeContextCurrent(context); // set active context
		}
	}

	rollofFactor= 1;
	refDistance = 150;
	maxDistance = 3400;

	////init ALUT
	//if(!alutInitWithoutContext (NULL, NULL))
	//{
	//	throw Ogre::Exception(0,"could not initialise ALUT!","SoundManager constructor");
	//}

	//create listener
	listener = new Listener();
	listener->setPosition(Ogre::Vector3::ZERO);
/*
	ALfloat listenerPos[]={0.0,0.0,4.0};
	ALfloat listenerVel[]={0.0,0.0,0.0};
	ALfloat listenerOri[]={0.0,0.0,1.0, 0.0,1.0,0.0};*/

//	ALfloat source0Pos[]={ -2.0, 0.0, 0.0};
//	ALfloat source0Vel[]={ 0.0, 0.0, 0.0};
//
//	ALuint  buffer[1];
//	ALuint  source[1];
//	ALuint  environment[1];
//
//	ALsizei size,freq;
//	ALenum  format;
//	ALvoid  *data;
//
//	//alListenerfv(AL_POSITION,listenerPos);
//	//alListenerfv(AL_VELOCITY,listenerVel);
//	//alListenerfv(AL_ORIENTATION,listenerOri);
//
//	alGetError(); // clear any error messages
//
//	// Generate buffers, or else no sound will happen!
//	alGenBuffers(1, buffer);
//
//	if(alGetError() != AL_NO_ERROR) 
//	{
//    	mLog("- Error creating buffers !!");
//		throw etInvalidId;
//    	//exit(1);
//	}
//	else
//	{
//    	//mLog("init() - No errors yet.");
//	}
//	/*
//	Operating System   	Source Code
//	Linux 	alutLoadWAVFile((Albyte *) "c.wav", &format, &data, size,&freq, &al_bool);
//	Macintosh 	alutLoadWAVFile("c.wav", &format, &data, &size, &freq);
//	Windows 	alutLoadWAVFile("c.wav", &format, &data, &size, &freq, &al_bool);
//	*/
//	
//	//OGRE FORUM TEST BEGIN
//	Ogre::DataStreamPtr pDSWaveFile;
//	try
//	{
//	   pDSWaveFile = Ogre::ResourceGroupManager::getSingleton().openResource("arrow_shoot.wav", "General");
//
//	}
//	catch(Ogre::Exception e)
//	{
//
//	   // File not found
//	   //std::clog << e.getFullDescription() << endl;
//	   throw e;
//
//	}
//
//	      
//	// Read the file into a buffer.
//	
//	size_t DataSize = pDSWaveFile->size();
//
//
////	char* pFileBuffer = new char[DataSize];//malloc(DataSize);
//	char* pFileBuffer = static_cast<char*>(malloc(DataSize));
//
//	pDSWaveFile->read(pFileBuffer, DataSize);
//	pDSWaveFile->close();
//
//	// Load the wav data from memory
//	buffer[0] = alutCreateBufferFromFileImage (pFileBuffer, DataSize);
//	//alutLoadWAVMemory(pFileBuffer, &format, &data, &size, &freq, AL_FALSE);
//	
//
//	ALenum err = alutGetError();
//	Ogre::String test = alutGetErrorString (err);
//	if(err != AL_NO_ERROR) 
//	{
//    	mLog("- Error creating sources !!\n");
//		throw etInvalidId;
//    	//exit(2);
//	}
//	
//	//alBufferData(buffer[0],format,data,size,freq);
//	//free(pFileBuffer);
//	delete pFileBuffer;
//
//	// Create the buffer
////	alBufferData(Buffer, format, data, size, freq);
////	alutUnloadWAV(format, data, size, freq); 
//	//OGRE FORUM TEST END
//
//
////	alutLoadWAVFile("test.wav",&format,&data,&size,&freq,AL_FALSE);
//	
////	alutUnloadWAV(format,data,size,freq);
//	//alutUnloadWAV(format, data, size, freq); 
//
//	alGetError(); /* clear error */
//	alGenSources(1, source);
//
//	if(alGetError() != AL_NO_ERROR) 
//	{
//    	mLog("- Error creating sources !!\n");
//		throw etInvalidId;
//    	//exit(2);
//	}
//	else
//	{
//    	mLog("init - no errors after alGenSources\n");
//	}
//	alSourcef(source[0], AL_PITCH, 1.0f);
//	alSourcef(source[0], AL_GAIN, 1.0f);
//	alSourcefv(source[0], AL_POSITION, source0Pos);
//	alSourcefv(source[0], AL_VELOCITY, source0Vel);
//	alSourcei(source[0], AL_BUFFER,buffer[0]);
//	alSourcei(source[0], AL_LOOPING, AL_TRUE);
//	alSourcePlay( source[0] );
	//loadWAVFile("test.wav",&format,&data,&size,&freq,&loop); 

	

}

void SoundManager::setDefaultRefDistance(Ogre::Real dist,bool applyToAll)
{
	refDistance = dist;
	if(!applyToAll)
		return;
	for(SourceList::iterator itr = mSources.begin();itr!=mSources.end();itr++)
	{
		(*itr)->setReferenceDistance(refDistance);		
	}
}
void SoundManager::setDefaultRolloffFactor(Ogre::Real dist,bool applyToAll)
{
	rollofFactor = dist;
	if(!applyToAll)
		return;
	for(SourceList::iterator itr = mSources.begin();itr!=mSources.end();itr++)
	{
		(*itr)->setRolloffFactor(rollofFactor);	
	}
}


void SoundManager::setDefaultMaxDist(Ogre::Real dist,bool applyToAll)
{
	maxDistance = dist;
	if(!applyToAll)
		return;
	for(SourceList::iterator itr = mSources.begin();itr!=mSources.end();itr++)
	{
		(*itr)->setMaxDistance(maxDistance);	
	}
}

ALuint SoundManager::stealALSource(Ogre::Real recieverSqDistance, int priority)
{
    //optimal ist eine gestoppte weit weg vom player
    //am allerschlechtesten ist eine spielende mit relativetolistener
    Source *bestStopped = NULL;
    Source *bestPaused = NULL;
    Source *bestPlaying = NULL;
    Source *bestRelToListener = NULL;
    //Source::SourceState relToListenerState = Source::Playing;
    Ogre::Vector3 listenerPos = listener->getPosition();
    for(SourceList::iterator itr=mSources.begin();itr!=mSources.end();itr++)
    {
        Source *cur = *itr;
        Source::SourceState curState = cur->getSourceState();
        Ogre::Real curDist = listenerPos.squaredDistance(cur->getPosition());
        if(cur->alSource == 0 || curDist > recieverSqDistance)
            continue;
        if(!cur->getRelativeToListener())
        {
            switch(curState)
            {
            case Source::Playing:
                if(!bestPlaying)
                    bestPlaying = cur;
                else
                {
                    Ogre::Real dist = listenerPos.squaredDistance(bestPlaying->getPosition());
                    if(dist < curDist)//das aktuelle ist weiter entfernt
                        bestPlaying = cur;
                }
                break;
            case Source::Paused:
                if(!bestPaused)
                    bestPaused = cur;
                else
                {
                    Ogre::Real dist = listenerPos.squaredDistance(bestPaused->getPosition());
                    if(dist < curDist)//das aktuelle ist weiter entfernt
                        bestPaused = cur;
                }
                break;
            case Source::Stopped:
                if(!bestStopped)
                    bestStopped = cur;
                else
                {
                    Ogre::Real dist = listenerPos.squaredDistance(bestStopped->getPosition());
                    if(dist < curDist)//das aktuelle ist weiter entfernt
                        bestStopped = cur;
                }
                break;

            };//switch
        }
        else
        {//relToListener
            if(!bestRelToListener || bestRelToListener->getSourceState() < cur->getSourceState())
                bestRelToListener = cur;
        }
    }//for
    Source *result = NULL;
    switch(priority)
    {
    case 3:
        if(bestRelToListener)
            result = bestRelToListener;
        //absichtlich kein break, damit es die nachfolgenden mit geringerer priorität auch überprüft
    case 2:
        if(bestPlaying)
            result = bestPlaying;
    case 1:
        if(bestPaused)
            result = bestPaused; 
    case 0:
        if(bestStopped)
            result = bestStopped;    
    }
    if(!result)
        return 0;
    return result->stealALSource();
    
    
}


ALuint SoundManager::createALSource(Ogre::Real recieverSqDistance, int priority)
{
    ALuint  source[1];
	alGetError(); /* clear error */
	alGenSources(1, source);
    //neue herangehensweise:
    //auch wenn das Erstellen der OpenAL source failt, wird
    //das objekt erstellt, nur halt ohne source
    //das versucht bei jedem play() an eine source zu kommen
    bool failed = false;
	if(alGetError() != AL_NO_ERROR) 
	{
    	//mLog("- Error creating source!!\n");
        failed = true;
        source[0] = 0;
		//return NULL;
    	//exit(2);
	}
	else
	{
    	//mLog("init - no errors after alGenSources\n");
	}
	if(source[0] == NULL)
	{
        failed = true;
		//mLog("WAHH Source=NULL");
	}
    if(failed)
    {
        //jetzt versuchen, source zu klauen
        source[0] = stealALSource(recieverSqDistance,priority);
        
    }
    return source[0];
}

Source *SoundManager::createSource(Ogre::Real recieverDistance, int priority)
{	
    ALuint source = createALSource();
	Source *res = new Source(source);
	res->setReferenceDistance(refDistance);
	res->setMaxDistance(maxDistance);
	res->setRolloffFactor(rollofFactor);
	mSources.push_back(res);
	if(alGetError() != AL_NO_ERROR) 
	{
    	mLog("- Error while creating source!!\n");
    	//exit(2);
	}
	
	return res;	
}

Buffer *SoundManager::createBuffer(Ogre::String filename)
{
	alGetError();
	Buffer *res = NULL;
	ALuint buffer;
	//@TODO: wavs wieder re-implementieren!!1
	//es soll angeblich nicht soo schwer sein
	//in den examples stehen funktionen, wo das gemacht wird
	//man muss es aber testen, und dafür habe ich jetzt atm keine zeit
	/*
	if(Ogre::StringUtil::endsWith(filename,"wav"))
	{
		Ogre::DataStreamPtr pDSWaveFile;
		try
		{
		   pDSWaveFile = Ogre::ResourceGroupManager::getSingleton().openResource(filename, "General");

		}
		catch(Ogre::Exception e)
		{
			mLog(e.getFullDescription());
			return NULL;
		   // File not found
		   //std::clog << e.getFullDescription() << endl;
		   //throw e;

		}

		      
		// Read the file into a buffer.
		
		size_t DataSize = pDSWaveFile->size();


	//	char* pFileBuffer = new char[DataSize];//malloc(DataSize);
		char* pFileBuffer = new char[DataSize];//static_cast<char*>(malloc(DataSize));

		pDSWaveFile->read(pFileBuffer, DataSize);
		

		// Load the wav data from memory
		buffer = alutCreateBufferFromFileImage (pFileBuffer, DataSize);
		//alutLoadWAVMemory(pFileBuffer, &format, &data, &size, &freq, AL_FALSE);
		

		ALenum err = alutGetError();
		//Ogre::String test = ;
		if(err != AL_NO_ERROR) 
		{
			Ogre::LogManager::getSingletonPtr()->logMessage("ERROR: Failed to create buffer for "+filename);
			return NULL;
    		//exit(2);
		}
		
		//alBufferData(buffer[0],format,data,size,freq);
		//free(pFileBuffer);
		//pDSWaveFile->close();
		pDSWaveFile.setNull();
		delete pFileBuffer;
		
		
	}
	else */ 
		if(Ogre::StringUtil::endsWith(filename,"ogg"))
	{
		Ogre::DataStreamPtr ogreFile;
		
		try
		{
		   ogreFile = Ogre::ResourceGroupManager::getSingleton().openResource(filename, "General");

		}
		catch(Ogre::Exception e)
		{
			mLog(e.getFullDescription());
			return NULL;
		   // File not found
		   //std::clog << e.getFullDescription() << endl;
		   //throw e;

		}

	

		ov_callbacks callbacks;
		callbacks.close_func = VorbisClose;
		callbacks.read_func  = VorbisRead;
		callbacks.seek_func  = VorbisSeek;
		callbacks.tell_func  = VorbisTell;
		//now load the ogg from memory:
		vorbis_info *psVorbisInfo;
		OggVorbis_File oggFile;
		unsigned long ulFrequency = 0;
		unsigned long ulChannels = 0;
		unsigned long ulBufferSize =0;
		unsigned long ulFormat = 0;
		if(ov_open_callbacks(&ogreFile, &oggFile, NULL, 0, callbacks)>=0)
		{
			psVorbisInfo = ov_info(&oggFile, -1);
			ulFrequency = psVorbisInfo->rate;
			ulChannels = psVorbisInfo->channels;
			if (psVorbisInfo->channels == 1)
			{
				ulFormat = AL_FORMAT_MONO16;
				// Set BufferSize to 250ms (Frequency * 2 (16bit) divided by 4 (quarter of a second))
				ulBufferSize = ulFrequency >> 1;
				// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
				ulBufferSize -= (ulBufferSize % 2);
			}
			else if (psVorbisInfo->channels == 2)
			{
				ulFormat = AL_FORMAT_STEREO16;
				// Set BufferSize to 250ms (Frequency * 4 (16bit stereo) divided by 4 (quarter of a second))
				ulBufferSize = ulFrequency;
				// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
				ulBufferSize -= (ulBufferSize % 4);
			}
			else if (psVorbisInfo->channels == 4)
			{
				ulFormat = alGetEnumValue("AL_FORMAT_QUAD16");
				// Set BufferSize to 250ms (Frequency * 8 (16bit 4-channel) divided by 4 (quarter of a second))
				ulBufferSize = ulFrequency * 2;
				// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
				ulBufferSize -= (ulBufferSize % 8);
			}
			else if (psVorbisInfo->channels == 6)
			{
				ulFormat = alGetEnumValue("AL_FORMAT_51CHN16");
				// Set BufferSize to 250ms (Frequency * 12 (16bit 6-channel) divided by 4 (quarter of a second))
				ulBufferSize = ulFrequency * 3;
				// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
				ulBufferSize -= (ulBufferSize % 12);
			}
		}
		else
		{
			Ogre::LogManager::getSingletonPtr()->logMessage("ERROR: Failed to create buffer for "+filename);
			return NULL;
		}

		//OgreAL theft BEGIN

		std::vector<char> vBuffer;
		//Buffer buffer;
		char *data = new char[ulBufferSize];
		int section, sizeRead = 0;
		//int size = 0;//ulBufferSize; //normalerweise ist das so,
								//nur bei streaming wird size = 0?
		do
		{
			sizeRead = ov_read(&oggFile, data, ulBufferSize, 0, 2, 1, &section);
			vBuffer.insert(vBuffer.end(), data, data + sizeRead);
		}while(sizeRead > 0);
		/*
		if(size == 0)
		{
			// Read the rest of the file
			do
			{
				sizeRead = ov_read(&oggFile, data, ulBufferSize, 0, 2, 1, &section);
				vBuffer.insert(vBuffer.end(), data, data + sizeRead);
			}while(sizeRead > 0);
		}
		else
		{
			// Read only what was asked for
			while(vBuffer.size() < size)
			{
				sizeRead = ov_read(&oggFile, data, ulBufferSize, 0, 2, 1, &section);
				if(sizeRead == 0) break;
				vBuffer.insert(vBuffer.end(), data, data + sizeRead);
			}
		}
		*/
		delete[] data;
		//return buffer;
		ALuint bf[1];
		alGenBuffers( 1, bf );
		//unsigned long ulBytesWritten = DecodeOggVorbis(&oggFile, decodedOggBuffer, ulBufferSize, ulChannels);
		
		
		//Buffer buffer = bufferData(&mOggStream, mStream ? mBufferSize : 0);
		//alBufferData(bf[0], ulFormat, decodedOggBuffer, ulBytesWritten, ulFrequency);
		alBufferData(bf[0], ulFormat, &vBuffer[0], static_cast<ALsizei>(vBuffer.size()), ulFrequency);
		//CheckError(alGetError(), "Could not load buffer data");
		//OgreAL theft END

		//char *decodedOggBuffer = (char*)malloc(ulBufferSize);             
		//if(!decodedOggBuffer)
		//{
		//	//fail
		//	Ogre::LogManager::getSingletonPtr()->logMessage("ERROR: Failed to create buffer for "+filename);
		//	free(decodedOggBuffer);
		//	return NULL;
		//}
		//
		////write OgreAL buffers
		///*
		//original code:
		//for (iLoop = 0; iLoop < NUMBUFFERS; iLoop++)
		//	{
		//		ulBytesWritten = DecodeOggVorbis(&sOggVorbisFile, pDecodeBuffer, ulBufferSize, ulChannels);
		//		if (ulBytesWritten)
		//		{
		//			alBufferData(uiBuffers[iLoop], ulFormat, pDecodeBuffer, ulBytesWritten, ulFrequency);
		//			alSourceQueueBuffers(uiSource, 1, &uiBuffers[iLoop]);
		//		}
		//	}*/
		

		//free(decodedOggBuffer);
		buffer = bf[0];
		
		

		ALenum err = alGetError();
		//Ogre::String test = ;
		if(err != AL_NO_ERROR) 
		{
			Ogre::LogManager::getSingletonPtr()->logMessage("ERROR: Failed to create buffer for "+filename);		
			return NULL;
    		//exit(2);
		}
		//ogreFile->close();
		ogreFile.setNull();
		//alBufferData(buffer[0],format,data,size,freq);
		//free(pFileBuffer);
//		delete pFileBuffer;
		/*res = new Buffer(filename,buffer);
		mBuffers.push_back(res);
		return res;*/
	}
	if(buffer)
	{
		res = new Buffer(filename,buffer);
		mBuffers.push_back(res);
		return res;
	}
	return NULL;
}

void SoundManager::destroySource(Source *src)
{
	if(destroyAlSource(src))
	{
		for(SourceList::iterator itr = mSources.begin();itr!=mSources.end();itr++)
		{
			if(*itr == src)
			{
				
				delete src;
				mSources.erase(itr);
				return;
			}
		}
	}

}
bool SoundManager::destroyAlSource(Source *src)
{
	alGetError();
	
	src->stop();
	src->setBuffer(NULL);
	ALuint sources[1];
	sources[0] = src->alSource;
	alDeleteSources(1, sources);
	if(alGetError())
	{
		//fehler
		Ogre::LogManager::getSingletonPtr()->logMessage("Warning: Could not delete Source");
		return false;
	}
	return true;
}
bool SoundManager::destroyAlBuffer(Buffer *buff)
{
	alGetError();
	ALuint buffers[1];
	buffers[0] = buff->buffer;
	alDeleteBuffers(1,buffers);
	
	if(alGetError())
	{
		//fehler
		Ogre::LogManager::getSingletonPtr()->logMessage("Warning: Could not delete Buffer");
		return false;
	}
	return true;
}

void SoundManager::destroyBuffer(Buffer *buff)
{
	if(destroyAlBuffer(buff))
	{
		for(BufferList::iterator itr = mBuffers.begin();itr!=mBuffers.end();itr++)
		{
			if(*itr == buff)
			{
				delete buff;
				mBuffers.erase(itr);
				return;
			}
		}
	}

}

Buffer *SoundManager::getBuffer(Ogre::String filename,bool createIfNotFound,int addType,bool bitwiseAnd)
{
	for(BufferList::iterator itr = mBuffers.begin();itr!=mBuffers.end();itr++)
	{
		Buffer *b = *itr;
		if(b->mFileName == filename)
		{
			if(addType)
			{
				if(bitwiseAnd)
				{
					if(!(b->getType() & addType))
					{
						b->setType(b->getType()+addType);
					}
				}
				else
				{
					if(b->getType() != addType)
						b->setType(addType);
				}
			}
			return b;				
		}
	}
	if(createIfNotFound)
	{
		return createBuffer(filename);
	}
	return NULL;
}

SoundManager::~SoundManager()
{
	delete listener;
	//todo: delete sources and buffers
	//delete sources
	for(SourceList::iterator itr = mSources.begin();itr!=mSources.end();itr++)
	{
		Source *src = *itr;
		src->stop();
		ALuint sources[1];
		sources[0] = src->alSource;
		alDeleteSources(1, sources);
		delete src;
	}
	//delete buffers
	
	
	
	
	for(BufferList::iterator itr = mBuffers.begin();itr!=mBuffers.end();itr++)
	{
		Buffer *buff = *itr;
		ALuint buffers[1];
		buffers[0] = buff->buffer;
		alDeleteBuffers(1,buffers);
		delete buff;		
	}

	
	////close ALUT
	//alutExit ();
	//Disable context
	alcMakeContextCurrent(NULL);
	//Release context(s)
	alcDestroyContext(context);
	//Close device
	alcCloseDevice(device);
}


void SoundManager::playByType(int type,bool bitwiseAnd)
{
	for(SourceList::iterator itr = mSources.begin();itr!=mSources.end();itr++)
	{
		Source *src = *itr;
		bool found = false;
		if(bitwiseAnd)
			found = ((src->getType() & type) != 0);
		else
			found = src->getType() == type;
		if(found)
		{
			src->play();
		}
	}
}
void SoundManager::pauseByType(int type,bool bitwiseAnd)
{
	for(SourceList::iterator itr = mSources.begin();itr!=mSources.end();itr++)
	{
		Source *src = *itr;
		bool found = false;
		if(bitwiseAnd)
			found = ((src->getType() & type) != 0);
		else
			found = src->getType() == type;
		if(found)
		{
			src->pause();
		}
	}
}
void SoundManager::stopByType(int type,bool bitwiseAnd)
{
	for(SourceList::iterator itr = mSources.begin();itr!=mSources.end();itr++)
	{
		Source *src = *itr;
		bool found = false;
		if(bitwiseAnd)
			found = ((src->getType() & type) != 0);
		else
			found = src->getType() == type;
		if(found)
		{
			src->stop();
		}
	}
}
//primarily, for setting the buffer of a certain source type to NULL
//in order to be able to delete this buffers
void SoundManager::setBufferByType(Buffer *buff,int type,bool bitwiseAnd)
{
	for(SourceList::iterator itr = mSources.begin();itr!=mSources.end();itr++)
	{
		Source *src = *itr;
		bool found = false;
		if(bitwiseAnd)
			found = ((src->getType() & type) != 0);
		else
			found = src->getType() == type;
		if(found)
		{
			src->setBuffer(buff);
		}
	}
}

//destroys only Sources with a specific "type"
void SoundManager::destroySourcesByType(int type,bool bitwiseAnd )
{
	for(SourceList::iterator itr = mSources.begin();itr!=mSources.end();itr++)
	{
		Source *src = *itr;
		bool found = false;
		if(bitwiseAnd)
			found = ((src->getType() & type) != 0);
		else
			found = src->getType() == type;
		if(found)
		{
			if(destroyAlSource(src))
			{
				src->stop();
				mSources.erase(itr);
				delete src;
			}
		}
	}
}

//destroys only Buffers with a specific "type"
void SoundManager::destroyBuffersByType(int type,bool bitwiseAnd)
{
	for(BufferList::iterator itr = mBuffers.begin();itr!=mBuffers.end();itr++)
	{
		Buffer *buff = *itr;
		bool found = false;
		if(bitwiseAnd)
			found = ((buff->getType() & type) != 0);
		else
			found = buff->getType() == type;
		if(found)
		{
			if(destroyAlBuffer(buff))
			{
				BufferList::iterator temp = itr;
				temp--;
				mBuffers.erase(itr);
				itr = temp;
				delete buff;
			}
		}
	}
}

SoundManager::DistanceModel SoundManager::intToDistanceModel(int val)
{
	switch(val)
	{
	case INVERSE_DISTANCE: return INVERSE_DISTANCE;
	case INVERSE_DISTANCE_CLAMPED: return INVERSE_DISTANCE_CLAMPED;
	case LINEAR_DISTANCE: return LINEAR_DISTANCE;
	case LINEAR_DISTANCE_CLAMPED: return LINEAR_DISTANCE_CLAMPED;
	case EXPONENT_DISTANCE: return EXPONENT_DISTANCE;
	case EXPONENT_DISTANCE_CLAMPED: return EXPONENT_DISTANCE_CLAMPED;
	}
	return INVERSE_DISTANCE_CLAMPED;
}

void SoundManager::setTypeGain(Ogre::Real gain, int type,bool bitwiseAnd)
{
	for(SourceList::iterator itr = mSources.begin();itr!=mSources.end();itr++)
	{
		Source *src = *itr;
		bool found = false;
		if(bitwiseAnd)
			found = ((src->getType() & type) != 0);
		else
			found = src->getType() == type;
		if(found)
		{
			//Ogre::Real off = src->getMsecOffset();
			//src->stop();
			/*mLog("Setting gain of source "+ptrToString(src));
			if(src->getBuffer())
				mLog("buffer filename="+src->getBuffer()->getFileName());
			else
				mLog("buffer is NULL");*/

			/*mLog("-new gain="+str(gain));
			mLog("-flags: "+str(src->getType())+"="+debugGetSourceFlags(src->getType()));
			mLog("-match flags: type="+str(type)+"="+debugGetSourceFlags(type));
			mLog("-found="+str(src->getType() & type)+"="+debugGetSourceFlags(src->getType() & type));*/
			src->setGain(gain);
			//src->setMsecOffset(off);
			//src->play();
		}
	}
}

void SoundManager::pauseAllSources()
{
	// && 
	for(SourceList::iterator itr = mSources.begin();itr!=mSources.end();itr++)
	{
		Source *src = *itr;
		if(src->getSourceState() == Source::Playing)
		{
			src->pause();
			src->pausedBySoundMgr = true;
			//mPausedSources.push_back(src);
		}
	}
}
void SoundManager::resumeAllSources()
{
	for(SourceList::iterator itr = mSources.begin();itr!=mSources.end();itr++)
	{
		Source *src = *itr;
		if(src->pausedBySoundMgr)
		{
			src->play();
			src->pausedBySoundMgr = false;
			//mPausedSources.push_back(src);
		}
	}
}