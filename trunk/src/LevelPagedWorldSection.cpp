#include "LevelPagedWorldSection.h"
#include "OgrePagingPrerequisites.h"
#include "Level.h"
#include "LevelPageManager.h"
#include "LevelPagedWorld.h"
#include "functions.h"
#include "TypeConverter.h"
#include "LevelPage.h"
#include "OgreGrid2DPageStrategy.h"

using namespace TypeConverter;
using namespace Ogre;

LevelPagedWorldSection::LevelPagedWorldSection(Ogre::String name, Level *level):
Ogre::PagedWorldSection(name,static_cast<Ogre::PagedWorld*>(level->getPagedWorld()),level->getSceneManager()),
	mLevel(level),
	autoPagingEnabled(true)
{
	// we always use a grid strategy
	//there isn't even any other one, unless someone codes it
	
	setStrategy(LevelPageManager::getSingletonPtr()->getStrategy("Grid2D"));

	Grid2DPageStrategyData* stratData = static_cast<Grid2DPageStrategyData*>(getStrategyData());
	stratData->setOrigin(Ogre::Vector3::ZERO);
	stratData->setCellRange(-10,-10,10,10);//-32768 to +32767

	Ogre::Real size  = 513;
	Ogre::Real border= 100;

	stratData->setMode(G2D_X_Z);
	stratData->setCellSize(size);//should be 2^n+1
	stratData->setLoadRadius(size-border);
	stratData->setHoldRadius(size);//i think hold isn't actually implemented.

	

	
}

LevelPagedWorldSection::~LevelPagedWorldSection()
{
}

//i need to overwrite those. they will recieve a stream I got from the zip
/// Load this section from a stream (returns true if successful)
bool LevelPagedWorldSection::load(Ogre::StreamSerialiser& stream)
{
	return true;
}
/// Save this section to a stream
void LevelPagedWorldSection::save(Ogre::StreamSerialiser& stream)
{
	//return true;
}

void LevelPagedWorldSection::notifyCamera(Ogre::Camera* cam)
{
	if(autoPagingEnabled)
		Ogre::PagedWorldSection::notifyCamera(cam);
}

Ogre::Page* LevelPagedWorldSection::loadOrCreatePage(const Ogre::Vector3& worldPos)
{
	//hmm. this SEEMS just to do bitpacking. i should change it later to the packIndex function, though
	Ogre::PageID id = getPageID(worldPos);
	// this will create a Page instance no matter what, even if load fails
	// we force the load attempt to happen immediately (forceSynchronous)
	loadPage(id, true);
	return getPage(id);
}

void LevelPagedWorldSection::loadPage(Ogre::PageID pageID, bool forceSynchronous)
{
	using namespace Ogre;
	
	
	//mLog("Loading Page: "+ogre_str(x)+", "+ogre_str(y));
	//offenbar wird das JEDES FUCKING FRAME gecalled. ich muss selber prüfen ob irgendwas nötig ist

	//loadPage versucht die page zu finden

	if (!mParent->getManager()->getPagingOperationsEnabled())
			return;

		PageMap::iterator i = mPages.find(pageID);
		if (i == mPages.end())
		{
			long x, y;
			LevelPageManager::unpackIndex(pageID,&x,&y);

			LevelPage* lPage = OGRE_NEW LevelPage(x,y, this);
			Ogre::Page* page = static_cast<Ogre::Page*>(lPage);
			// try to insert
			std::pair<PageMap::iterator, bool> ret = mPages.insert(
				PageMap::value_type(page->getID(), page));

			if (!ret.second)
			{
				// page with this ID already in map
				if (ret.first->second != page)
				{
					// replacing a page, delete the old one
					OGRE_DELETE ret.first->second;
					ret.first->second = page;
				}
			}
			page->load(forceSynchronous);
		}
		else
			i->second->touch();
}

void LevelPagedWorldSection::unloadPage(Ogre::PageID pageID, bool forceSynchronous)
{
	PagedWorldSection::unloadPage(pageID, forceSynchronous);
}

void LevelPagedWorldSection::unloadPage(Ogre::Page* p, bool forceSynchronous)
{
	PagedWorldSection::unloadPage(p, forceSynchronous);
}

bool LevelPagedWorldSection::_prepareProceduralPage(Ogre::Page* page)
{
	return true;
}

bool LevelPagedWorldSection::_loadProceduralPage(Ogre::Page* page)
{
	return true;
}

bool LevelPagedWorldSection::_unloadProceduralPage(Ogre::Page* page)
{
	return true;
}

bool LevelPagedWorldSection::_unprepareProceduralPage(Ogre::Page* page)
{
	return true;
}

void LevelPagedWorldSection::holdPage(Ogre::PageID pageID)
{

}

Ogre::StreamSerialiser* LevelPagedWorldSection::_readPageStream(Ogre::PageID pageID)
{
	return NULL;
}

Ogre::StreamSerialiser* LevelPagedWorldSection::_writePageStream(Ogre::PageID pageID)
{
	return NULL;
}

const Ogre::String& LevelPagedWorldSection::getType()
{
	static const Ogre::String stype("Level");
	return stype;
}