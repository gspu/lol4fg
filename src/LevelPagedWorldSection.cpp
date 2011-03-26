#include <LevelPagedWorldSection.h>
#include "LevelPagedWorld.h"
#include "LevelPageManager.h"
#include "LevelTerrainGroup.h"
#include "OgreTerrainGroup.h"
#include "Level.h"


LevelPagedWorldSection::LevelPagedWorldSection(const Ogre::String& name, LevelPagedWorld* parent, Ogre::SceneManager* sm)
:Ogre::TerrainPagedWorldSection(name,parent,sm)
{
}

LevelPagedWorldSection::~LevelPagedWorldSection()
{
}


void LevelPagedWorldSection::unloadPage(Ogre::PageID pageID, bool forceSynchronous)
{
	//HIER ANSATZPUNKT FÜR UNLOAD!
	TerrainPagedWorldSection::unloadPage(pageID,forceSynchronous);
}

void LevelPagedWorldSection::loadPage(Ogre::PageID pageID, bool forceSynchronous)
{
	if (!mParent->getManager()->getPagingOperationsEnabled())
		return;

	LevelPageManager *mgr = static_cast<LevelPageManager*>(mParent->getManager());
	LevelTerrainGroup *exGroup = static_cast<LevelTerrainGroup *>(mTerrainGroup);

	
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

