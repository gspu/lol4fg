#include "LevelPagedWorld.h"
#include "LevelPageManager.h"
#include "LevelPagedWorldSection.h"

using namespace Ogre;

LevelPagedWorld::LevelPagedWorld(Level *lvl, Ogre::String name):
	Ogre::PagedWorld(name,LevelPageManager::getSingletonPtr())
{
}

LevelPagedWorld::~LevelPagedWorld()
{
}


void LevelPagedWorld::notifyCamera(Ogre::Camera* cam)
{
	Ogre::PagedWorld::notifyCamera(cam);
}


void LevelPagedWorld::load()
{
	//loading from level zip file here
}

LevelPagedWorldSection* LevelPagedWorld::createLevelSection(Level *lvl, const Ogre::String& sectionName)
{
	String theName = sectionName;
	if (theName.empty())
	{
		do 
		{
			theName = mSectionNameGenerator.generate();
		} while (mSections.find(theName) != mSections.end());
	}
	else if(mSections.find(theName) != mSections.end())
	{
		OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM, 
			"World section named '" + theName + "' already exists!",
			"PagedWorld::createSection");
	}

	LevelPagedWorldSection* ret = 0;
	
	ret = OGRE_NEW LevelPagedWorldSection(lvl);
	
	mSections[theName] = ret;

	return ret;
}