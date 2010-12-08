#ifndef __loadingThread
#define __loadingThread
#include "windows.h"
#include "Ogre.h"
#include "ZipSaveFile.h"



class LoadingThread
{
public:
    LoadingThread()
    {
        mThread = 0;
        
    }

    void start();

    void stop();

    bool isFinished()
    {
        if(tryLock())
        {
            bool res = finished;
            unlock();
            return res;
        }
        return false;
    }
    int getProgress()
    {
        if(tryLock())
        {
            int res = progress;
            unlock();
            return res;
        }
        return -1;
    }
    int getMaxProgress()
    {
        if(tryLock())
        {
            int res = maxProgress;
            unlock();
            return res;
        }
        return -1;
    }
    //code comes here
    virtual bool run() = 0;

private:
    
protected:
    //is loading finished?
    bool finished;
    //progress. can be whatever
    int progress;
    //when this is reaced, finished is true
    int maxProgress;

    //tries to lock the mutex. returns true on success, false on failure
    bool tryLock()
    {
        if(WaitForSingleObject(mMutex,0) == WAIT_OBJECT_0)
            return true;
        return false;
    }
    //unlocks my mutex
    void unlock()
    {
        ReleaseMutex(mMutex);
    }
    //locks the mutex, with the thread waiting until it's available if it is not ATM
    void lock()
    {
        WaitForSingleObject(mMutex,INFINITE);
    }
    static unsigned long __stdcall threadProc(void* ptr)
    {
        //if(tryLock
        //if(!finished)
        //{
        ((LoadingThread*)ptr)->run();
        
        //}
        
        return 0;
    }

    /*Thread *_threadObj;*/
    HANDLE mThread;
    HANDLE mMutex;
};

class LevelLoadingThread: public LoadingThread
{
public:
    LevelLoadingThread(Ogre::String filename,ZipSaveFile *sav = NULL);
};


#endif;