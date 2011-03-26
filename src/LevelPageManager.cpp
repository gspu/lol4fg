#include "LevelPageManager.h"
#include "LevelPagedWorld.h"

LevelPagedWorld* LevelPageManager::createLevelPagedWorld(const Ogre::String& name)
{
	using namespace Ogre;
	String theName = name;
	if (theName.empty())
	{
		do 
		{
			theName = mWorldNameGenerator.generate();
		} while (mWorlds.find(theName) != mWorlds.end());
	}
	else if(mWorlds.find(theName) != mWorlds.end())
	{
		OGRE_EXCEPT(Ogre::Exception::ERR_DUPLICATE_ITEM, 
			"World named '" + theName + "' already exists!",
			"PageManager::createWorld");
	}

	LevelPagedWorld* ret = OGRE_NEW LevelPagedWorld(theName, this);
	mWorlds[theName] = ret;

	return ret;
}