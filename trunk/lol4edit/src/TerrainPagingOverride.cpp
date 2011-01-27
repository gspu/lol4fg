#include "TerrainPagingOverride.h"
#include "OgreTerrainGroup.h"
#include "Level.h"
#include "ExtendedTerrainGroup.h"


//void ExtendedPagedWorldSection::unloadPage(PageID pageID, bool forceSynchronous)
//{
//}

void ExtendedPagedWorldSection::loadPage(Ogre::PageID pageID, bool forceSynchronous)
{
	if (!mParent->getManager()->getPagingOperationsEnabled())
		return;

	ExtendedPageManager *mgr = static_cast<ExtendedPageManager*>(mParent->getManager());
	ExtendedTerrainGroup *exGroup = static_cast<ExtendedTerrainGroup *>(mTerrainGroup);

	
	PageMap::iterator i = mPages.find(pageID);
	if (i == mPages.end())
	{
		// trigger terrain load
		long x, y;
		// pageID is the same as a packed index
		mTerrainGroup->unpackIndex(pageID, &x, &y);
		
		if(x >= t_minX && x <= t_maxX && y >= t_minY && y <= t_maxY)
		{		
			Ogre::DataStreamPtr stream = mgr->getLevel()->getTerrainForPage(x,y);
			if(stream.isNull())
			{
				
				exGroup->defineTerrain(x,y,(float)0.0f);
			}
			else
			{
				exGroup->defineTerrain(x,y,stream);
			}
			//mTerrainGroup->defineTerrain(x, y);


			exGroup->loadTerrain(x, y, forceSynchronous);
			mLog("try to load page: "+TypeConverter::ogre_str(x)+" / "+TypeConverter::ogre_str(y));
		}
	}

	PagedWorldSection::loadPage(pageID, forceSynchronous);
}


ExtendedPagedWorldSection* ExtendedPaging::createTestWorldSection(ExtendedWorld* world, Ogre::TerrainGroup* terrainGroup, 
			Ogre::Real loadRadius, Ogre::Real holdRadius, 
			Ogre::int32 minX, Ogre::int32 minY, Ogre::int32 maxX, Ogre::int32 maxY, 
			const Ogre::String& sectionName)		
{
	ExtendedPagedWorldSection* ret = static_cast<ExtendedPagedWorldSection*>(
		world->createTestSection(terrainGroup->getSceneManager(), ExtendedSectionFactory::FACTORY_NAME, sectionName));

	ret->init(terrainGroup);
	ret->setLoadRadius(loadRadius);
	ret->setHoldRadius(holdRadius);
	ret->setPageRange(minX, minY, maxX, maxY);

	ret->t_minX = minX;
	ret->t_minY = minY;
	ret->t_maxX = maxX;
	ret->t_maxY = maxY;

	return ret;
}


ExtendedPagedWorldSection* ExtendedPaging::ExtendedSectionFactory::createInstance(const Ogre::String& name, Ogre::PagedWorld* parent, Ogre::SceneManager* sm)
{
	return OGRE_NEW ExtendedPagedWorldSection(name, parent, sm);
}



ExtendedPagedWorldSection* ExtendedWorld::createTestSection(Ogre::SceneManager* sceneMgr,
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

		ExtendedPagedWorldSection* ret = 0;
		/*if (typeName == "General")*/
			ret = OGRE_NEW ExtendedPagedWorldSection(theName, this, sceneMgr);
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


ExtendedWorld* ExtendedPageManager::createTestWorld(const Ogre::String& name)
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

	ExtendedWorld* ret = OGRE_NEW ExtendedWorld(theName, this);
	mWorlds[theName] = ret;

	return ret;
}

void ExtendedWorld::notifyCamera(Ogre::Camera* cam)
{
	if(autoLoadingEnabled)
		PagedWorld::notifyCamera(cam);
}