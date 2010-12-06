#include "loadingthread.h"

void LoadingThread::start()
{

    DWORD threadID;
    mThread = CreateThread(0, 0, threadProc, this, 0, &threadID);

    mMutex = CreateMutex(NULL,false,NULL);
    progress = 0;
    maxProgress = 0;
    finished = false;
}

void LoadingThread::stop()
{
    if (mThread)
        TerminateThread (mThread, 0);
    mThread = 0;
    finished = true;
    unlock();
    CloseHandle(mMutex);
}

