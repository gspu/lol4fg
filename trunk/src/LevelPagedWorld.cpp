#include "LevelPagedWorld.h"
#include "LevelPagedWorldSection.h"

void LevelPagedWorld::notifyCamera(Ogre::Camera* cam)
{
	if(autoLoadingEnabled)
		PagedWorld::notifyCamera(cam);
}

LevelPagedWorldSection* LevelPagedWorld::createLevelWorldSection(Ogre::SceneManager* sceneMgr,
			const Ogre::String& typeName,
			const Ogre::String& sectionName)
{
	using namespace Ogre;
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
		OGRE_EXCEPT(Ogre::Exception::ERR_DUPLICATE_ITEM, 
			"World section named '" + theName + "' already exists!",
			"PagedWorld::createSection");
	}

	LevelPagedWorldSection* ret = 0;
	/*if (typeName == "General")*/
		ret = OGRE_NEW LevelPagedWorldSection(theName, this, sceneMgr);
	/*else
	{
		PagedWorldSectionFactory* fact = getManager()->getWorldSectionFactory(typeName);
		if (!fact)
		{
			OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
				"World section type '" + typeName + "' does not exist!",
				"PagedWorld::createSection");
		}

		ret = fact->createInstance(theName, this, sceneMgr);

	}*/
	mSections[theName] = ret;

	return ret;
}