#ifndef __lvlPagedWorldSection
#define __lvlPagedWorldSection

#include <Ogre.h>
#include "LevelTerrainPrereqs.h"
#include "FwDec.h"

class LevelPagedWorldSection: public Ogre::PagedWorldSection
{
public:
	/** Construct a new instance, specifying the parent level. */
	LevelPagedWorldSection(Level *level);
	virtual ~LevelPagedWorldSection();


	//i need to overwrite those. they will recieve a stream I got from the zip
	/// Load this section from a stream (returns true if successful)
	virtual bool load(Ogre::StreamSerialiser& stream);
	/// Save this section to a stream
	virtual void save(Ogre::StreamSerialiser& stream);

	//might need those, too
	/// Called when the frame starts
	virtual void frameStart(Ogre::Real timeSinceLastFrame);
	/// Called when the frame ends
	virtual void frameEnd(Ogre::Real timeElapsed);
	/// Notify a section of the current camera
	virtual void notifyCamera(Ogre::Camera* cam);

	/** Load or create a page against this section covering the given world 
		space position. 
	@remarks
		This method is designed mainly for editors - it will try to load
		an existing page if there is one, otherwise it will create a new one
		synchronously.

		now THIS I need to overwrite. it will create my own Page objects
	*/
	virtual Ogre::Page* loadOrCreatePage(const Ogre::Vector3& worldPos);



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
	/** Ask for a page to be unloaded with the given (section-relative) PageID
	@remarks
	You would not normally call this manually, the PageStrategy is in 
	charge of it usually.
	@param p The Page to unload
	@param forceSynchronous If true, the page will always be unloaded synchronously
	*/
	virtual void unloadPage(Ogre::Page* p, bool forceSynchronous = false);
	/** Give a section the opportunity to prepare page content procedurally. 
	@remarks
	You should not call this method directly. This call may well happen in 
	a separate thread so it should not access GPU resources, use _loadProceduralPage
	for that
	@returns true if the page was populated, false otherwise
	*/
	virtual bool _prepareProceduralPage(Ogre::Page* page);
	/** Give a section the opportunity to prepare page content procedurally. 
	@remarks
	You should not call this method directly. This call will happen in 
	the main render thread so it can access GPU resources. Use _prepareProceduralPage
	for background preparation.
	@returns true if the page was populated, false otherwise
	*/
	virtual bool _loadProceduralPage(Ogre::Page* page);
	/** Give a section  the opportunity to unload page content procedurally. 
	@remarks
	You should not call this method directly. This call will happen in 
	the main render thread so it can access GPU resources. Use _unprepareProceduralPage
	for background preparation.
	@returns true if the page was populated, false otherwise
	*/
	virtual bool _unloadProceduralPage(Ogre::Page* page);
	/** Give a section  the opportunity to unprepare page content procedurally. 
	@remarks
	You should not call this method directly. This call may well happen in 
	a separate thread so it should not access GPU resources, use _unloadProceduralPage
	for that
	@returns true if the page was unpopulated, false otherwise
	*/
	virtual bool _unprepareProceduralPage(Ogre::Page* page);

	/** Ask for a page to be kept in memory if it's loaded.
	@remarks
		This method indicates that a page should be retained if it's already
		in memory, but if it's not then it won't trigger a load. This is useful
		for retaining pages that have just gone out of range, but which you
		don't want to unload just yet because it's quite possible they may come
		back into the active set again very quickly / easily. But at the same
		time, if they've already been purged you don't want to force them to load. 
		This is the 'maybe' region of pages. 
	@par
		Any Page that is neither requested nor held in a frame will be
		deemed a candidate for unloading.
	*/
	virtual void holdPage(Ogre::PageID pageID);



	/** Get a serialiser set up to read Page data for the given PageID. 
	@param pageID The ID of the page being requested
	@remarks
	The StreamSerialiser returned is the responsibility of the caller to
	delete. 
	*/
	virtual Ogre::StreamSerialiser* _readPageStream(Ogre::PageID pageID);

	/** Get a serialiser set up to write Page data for the given PageID. 
	@param pageID The ID of the page being requested
	@remarks
	The StreamSerialiser returned is the responsibility of the caller to
	delete. 
	*/
	virtual Ogre::StreamSerialiser* _writePageStream(Ogre::PageID pageID);

	/** Get the type name of this section. */
	virtual const Ogre::String& getType();

	virtual Level *getLevel(){return mLevel;}
protected:
	Level *mLevel;
};

#endif