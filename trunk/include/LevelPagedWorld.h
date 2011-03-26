#ifndef __levelPagedWorld
#define __levelPagedWorld

#include "Ogre.h"

#include "functions.h"

#include <OgreTerrainPagedWorldSection.h>
#include "OgrePagingPrerequisites.h"
#include "OgrePagedWorldSection.h"
#include "OgrePageManager.h"
#include "OgrePagedWorld.h"
#include "OgreTerrainPaging.h"

#include "TypeConverter.h"

#include <LevelTerrainPrerequisites.h>

class LevelPagedWorld: public Ogre::PagedWorld
{
public:
	LevelPagedWorld(const Ogre::String& name, Ogre::PageManager* manager):
	  Ogre::PagedWorld(name,manager),
	  autoLoadingEnabled(true)
	  {}
		  

	LevelPagedWorldSection* createLevelWorldSection(Ogre::SceneManager* sceneMgr,
			const Ogre::String& typeName,
			const Ogre::String& sectionName = Ogre::StringUtil::BLANK);

	void notifyCamera(Ogre::Camera* cam);


	bool autoLoadingEnabled;
};

#endif