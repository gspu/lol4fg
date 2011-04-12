#include <LevelPagedWorldSection.h>
#include "LevelPagedWorld.h"
#include "LevelPageManager.h"
#include "LevelTerrainGroup.h"
#include "OgreTerrainGroup.h"
#include "Level.h"
#include "LevelPagingListener.h"
#include "OgrePage.h"

LevelPagedWorldSection::LevelPagedWorldSection(const Ogre::String& name, LevelPagedWorld* parent, Ogre::SceneManager* sm)
:
	Ogre::TerrainPagedWorldSection(name,parent,sm),
	mPageListener(NULL)
{
}

LevelPagedWorldSection::~LevelPagedWorldSection()
{
}

void LevelPagedWorldSection::unloadPage(Ogre::Page* p, bool forceSynchronous)
{
	PagedWorldSection::unloadPage(p,forceSynchronous);
}
void LevelPagedWorldSection::unloadPage(Ogre::PageID pageID, bool forceSynchronous)
{
	//HIER ANSATZPUNKT FÜR UNLOAD!
	TerrainPagedWorldSection::unloadPage(pageID,forceSynchronous);
}

bool LevelPagedWorldSection::_unloadProceduralPage(Ogre::Page* page)
{
	return TerrainPagedWorldSection::_unloadProceduralPage(page);
}

bool LevelPagedWorldSection::_unprepareProceduralPage(Ogre::Page* page)
{
	return TerrainPagedWorldSection::_unprepareProceduralPage(page);
}

void LevelPagedWorldSection::holdPage(Ogre::PageID pageID)
{
	TerrainPagedWorldSection::holdPage(pageID);
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
		//at this point, I know for sure that a page load has been requested
		

		// trigger terrain load
		long x, y;
		// pageID is the same as a packed index
		mTerrainGroup->unpackIndex(pageID, &x, &y);

		//notify the listener that there's a new page being requested
		if(mPageListener)
			mPageListener->pageLoadingBegin(x,y);

		//mLog("LevelPagedWorldSection::loadPage: page"+TypeConverter::ogre_str(x)+"/"+TypeConverter::ogre_str(y));
		
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

/** Give a section the opportunity to prepare page content procedurally. 
@remarks
You should not call this method directly. This call may well happen in 
a separate thread so it should not access GPU resources, use _loadProceduralPage
for that
@returns true if the page was populated, false otherwise
*/
bool LevelPagedWorldSection::_prepareProceduralPage(Ogre::Page* page)
{
	//es sollte eigentlich okay sein, hier schon das ..loaded zu callen, denn das dürfte synchron ablaufen.
	//bzw, eventuell erst im nächsten frame nach diesem
	if(mPageListener)
	{
		long x, y;
		unpackTerrainIndex(page->getID(),&x,&y);
		mPageListener->pageLoadingPrepare(x,y,page);
		mPageListener->pageLoadingEnd(x,y);
	}
	return PagedWorldSection::_prepareProceduralPage(page);
}

/** Give a section the opportunity to prepare page content procedurally. 
@remarks
You should not call this method directly. This call will happen in 
the main render thread so it can access GPU resources. Use _prepareProceduralPage
for background preparation.
@returns true if the page was populated, false otherwise
*/
bool LevelPagedWorldSection::_loadProceduralPage(Ogre::Page* page)
{
	if(mPageListener)
	{
		long x, y;
		unpackTerrainIndex(page->getID(),&x,&y);
		mPageListener->pageLoadingProcess(x,y);
	}
	return PagedWorldSection::_loadProceduralPage(page);
}