#ifndef __levelPageManager
#define __levelPageManager

#include "Ogre.h"
#include <OgreTerrainPagedWorldSection.h>
#include "functions.h"
#include "OgrePagingPrerequisites.h"

#include "OgrePagedWorldSection.h"
#include "OgrePageManager.h"
#include "OgrePagedWorld.h"
#include "TypeConverter.h"
#include "OgreTerrainPaging.h"

#include <LevelTerrainPrerequisites.h>

class LevelPageManager: public Ogre::PageManager
{
public:
	//DAS DING BENUTZT DEN PageProvider
	LevelPageManager(Level *lvl):
		Ogre::PageManager(),
		mLevel(lvl)
	  {
	  }

	//because I cannot overwrite createWorld
	LevelPagedWorld* createLevelPagedWorld(const Ogre::String& name = Ogre::StringUtil::BLANK);

	inline Level *getLevel()
	{
		return mLevel;
	}
private:
	Level *mLevel;
};

#endif