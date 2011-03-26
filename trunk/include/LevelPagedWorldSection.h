#ifndef __levelPagedWorldSection
#define __levelPagedWorldSection

#include "Ogre.h"
#include "OgreTerrainPagedWorldSection.h"
#include "OgrePagingPrerequisites.h"

#include "OgrePagedWorldSection.h"
#include "OgrePageManager.h"
#include "OgrePagedWorld.h"
#include "TypeConverter.h"
#include "OgreTerrainPaging.h"

#include <LevelTerrainPrerequisites.h>

//Paged World Section--------------------------------------------------------------------------------------------------
class LevelPagedWorldSection: public Ogre::TerrainPagedWorldSection
{
public:
	LevelPagedWorldSection(const Ogre::String& name, LevelPagedWorld* parent, Ogre::SceneManager* sm);

	~LevelPagedWorldSection();

	/** Ask for a page to be loaded with the given (section-relative) PageID
	@remarks
		You would not normally call this manually, the PageStrategy is in 
		charge of it usually.
		If this page is already loaded, this request will not load it again.
		If the page needs loading, then it may be an asynchronous process depending
		on whether threading is enabled.
	@param pageID The page ID to load
	@param forceSynchronous If true, the page will always be loaded synchronously
	*/
	virtual void loadPage(Ogre::PageID pageID, bool forceSynchronous = false);

	/** Ask for a page to be unloaded with the given (section-relative) PageID
	@remarks
		You would not normally call this manually, the PageStrategy is in 
		charge of it usually.
	@param pageID The page ID to unload
	@param forceSynchronous If true, the page will always be unloaded synchronously
	*/
	virtual void unloadPage(Ogre::PageID pageID, bool forceSynchronous = false);
	///** Ask for a page to be unloaded with the given (section-relative) PageID
	//@remarks
	//You would not normally call this manually, the PageStrategy is in 
	//charge of it usually.
	//@param p The Page to unload
	//@param forceSynchronous If true, the page will always be unloaded synchronously
	//*/
	//virtual void unloadPage(Page* p, bool forceSynchronous = false);
	///** Give a section the opportunity to prepare page content procedurally. 
	//@remarks
	//You should not call this method directly. This call may well happen in 
	//a separate thread so it should not access GPU resources, use _loadProceduralPage
	//for that
	//@returns true if the page was populated, false otherwise
	//*/
	//virtual bool _prepareProceduralPage(Page* page);
	///** Give a section the opportunity to prepare page content procedurally. 
	//@remarks
	//You should not call this method directly. This call will happen in 
	//the main render thread so it can access GPU resources. Use _prepareProceduralPage
	//for background preparation.
	//@returns true if the page was populated, false otherwise
	//*/
	//virtual bool _loadProceduralPage(Page* page);
	///** Give a section  the opportunity to unload page content procedurally. 
	//@remarks
	//You should not call this method directly. This call will happen in 
	//the main render thread so it can access GPU resources. Use _unprepareProceduralPage
	//for background preparation.
	//@returns true if the page was populated, false otherwise
	//*/
	//virtual bool _unloadProceduralPage(Page* page);
	///** Give a section  the opportunity to unprepare page content procedurally. 
	//@remarks
	//You should not call this method directly. This call may well happen in 
	//a separate thread so it should not access GPU resources, use _unloadProceduralPage
	//for that
	//@returns true if the page was unpopulated, false otherwise
	//*/
	//virtual bool _unprepareProceduralPage(Page* page);

	///** Ask for a page to be kept in memory if it's loaded.
	//@remarks
	//	This method indicates that a page should be retained if it's already
	//	in memory, but if it's not then it won't trigger a load. This is useful
	//	for retaining pages that have just gone out of range, but which you
	//	don't want to unload just yet because it's quite possible they may come
	//	back into the active set again very quickly / easily. But at the same
	//	time, if they've already been purged you don't want to force them to load. 
	//	This is the 'maybe' region of pages. 
	//@par
	//	Any Page that is neither requested nor held in a frame will be
	//	deemed a candidate for unloading.
	//*/
	//virtual void holdPage(PageID pageID);

	//void setRealPageProvider(Ogre::PageProvider *prov)
	//{
	//	mRealPageProvider = prov;
	//}

	Ogre::int32 t_minX;
	Ogre::int32 t_minY;
	Ogre::int32 t_maxX;
	Ogre::int32 t_maxY;
protected:
	Ogre::PageProvider *mRealPageProvider;//because in this point, OGRE SUCKS!
};

#endif