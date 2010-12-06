#include "Source.h"
#include "Listener.h"
#include "SoundManager.h"

using namespace SimpleSound;

Source::Source(ALuint src)
{
    bak = NULL;
	alSource = src;
	type = 0;
	mBuffer = NULL;
	pausedBySoundMgr = false;
	//mBuffer <- der wird eh default constructed
/*
	alSourcef(source[0], AL_PITCH, 1.0f);
	alSourcef(source[0], AL_GAIN, 1.0f);
	alSourcefv(source[0], AL_POSITION, source0Pos);
	alSourcefv(source[0], AL_VELOCITY, source0Vel);
	alSourcei(source[0], AL_BUFFER,buffer[0]);
	alSourcei(source[0], AL_LOOPING, AL_TRUE);
	alSourcePlay( source[0] );*/
}

ALuint Source::stealALSource()
{
    if(!alSource)
        return 0;
    if(bak)
        delete bak;
    bak = new SourceBackup;
    bak->mPos = getPosition();
    bak->mOrient = getOrientation();
    bak->mVel = getVelocity();
    bak->mGain = getGain();
    bak->mMinGain = getMinGain();
    bak->mMaxGain = getMaxGain();
    bak->relToListener=getRelativeToListener();
    bak->coneInnerAngle=getConeInnerAngle();
    bak->coneOuterAngle=getConeOuterAngle();
    bak->coneOuterGain=getConeOuterGain();
    bak->pitch=getPitch();
    bak->loop=getLoop();
    ALuint src = alSource;
    stop();
    alSource = 0;
    return src;
}

void Source::setALSource(ALuint src)
{
    alSource = src;
    if(bak && alSource)
    {
        setPosition(bak->mPos);
        
        setOrientation(bak->mOrient);
        setVelocity(bak->mVel);
        setGain(bak->mGain);
        setMinGain(bak->mMinGain);
        setMaxGain(bak->mMaxGain);
        setRelativeToListener(bak->relToListener);
        setConeInnerAngle(bak->coneInnerAngle);
        setConeOuterAngle(bak->coneOuterAngle);
        setConeOuterGain(bak->coneOuterGain);
        setPitch(bak->pitch);
        setLoop(bak->loop);
        delete bak;
        bak = NULL;
    }   
}

void Source::retrieveALSource(int priority)
{
    if(alSource != 0)
        return;
    Ogre::Real dist = 0;
    if(bak)
        dist = Listener::getSingletonPtr()->getPosition().squaredDistance(bak->mPos);
    alSource = SoundManager::getSingletonPtr()->stealALSource(0,priority);
}

Source::~Source()
{
    if(bak)
        delete bak;
}

void Source::setBuffer(SimpleSound::Buffer *buffer)
{
	if(mBuffer && !isStopped())
		stop();
	alGetError();
	mBuffer = buffer;
	if(buffer)
		alSourcei(alSource, AL_BUFFER,buffer->getBuffer());
	else
		alSourcei(alSource, AL_BUFFER,NULL);
	ALenum err = alGetError();
	if(err)
	{
		Ogre::LogManager::getSingletonPtr()->logMessage("setBuffer error: "+Ogre::String(alGetString(err)));
	}
}