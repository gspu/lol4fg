#include "FakeLogManager.h"
#include "Ogre.h"

void FakeLog::logMessage(const QString &message,QtOgre::LogLevel logLevel)
{/*
    switch(LL_DEBUG = 1024,
		LL_INFO = 2048,
		LL_WARNING = 4096,
		LL_ERROR = 8192*/
    Ogre::LogManager::getSingletonPtr()->logMessage(message.toStdString());
}