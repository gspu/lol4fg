#ifndef __lvlPagedWorld
#define __lvlPagedWorld
#include <Ogre.h>
#include "LevelTerrainPrereqs.h"
#include "FwDec.h"


class LevelPagedWorld: public Ogre::PagedWorld
{
public:
	/** Constructor. name will be generated automatically 
	(if even needed, stuff will be retrieved from the level file), and PageManager will 
	be a singleton
	@param lvl The Level to which this "world" belongs
	*/
	LevelPagedWorld(Level *lvl);
	virtual ~LevelPagedWorld();

	// it might be indeed a good idea to move some of the loading stuff into the relevant classes.
	// no filename or so, I know where to find the data
	void load();

	//just save to level file
	void save();
	
	// non-virtual stuff, AGAIN... let's hope this functions will never be called.
	// i'll try to overwrite them anyway. maybe it'll help
	// they might be unnecessary, though. the TerrainPaging example does not overwrite PagedWorld, but PagedWorldSection
	void load(const String& filename);
	void load(const DataStreamPtr& stream);
	bool load(StreamSerialiser& stream);
	void save(const String& filename);
	void save(const DataStreamPtr& stream);
	void save(StreamSerialiser& stream);

	PagedWorldSection* createSection(SceneManager* sceneMgr,
		const String& typeName,
		const String& sectionName = StringUtil::BLANK);
	PagedWorldSection* createSection(const String& strategyName, SceneManager* sceneMgr,
		const String& sectionName = StringUtil::BLANK);
	PagedWorldSection* createSection(PageStrategy* strategy, SceneManager* sceneMgr, 
		const String& sectionName = StringUtil::BLANK);



	typedef map<String, PagedWorldSection*>::type SectionMap;
	/// Retrieve a const reference to all the sections in this world
	const SectionMap& getSections() const { return mSections; }

	
	//finally, virtual stuff

	/** Give a world  the opportunity to prepare page content procedurally. 
	@remarks
	You should not call this method directly. This call may well happen in 
	a separate thread so it should not access GPU resources, use _loadProceduralPage
	for that
	@returns true if the page was populated, false otherwise
	*/
	virtual bool _prepareProceduralPage(Page* page, PagedWorldSection* section);
	/** Give a world  the opportunity to prepare page content procedurally. 
	@remarks
	You should not call this method directly. This call will happen in 
	the main render thread so it can access GPU resources. Use _prepareProceduralPage
	for background preparation.
	@returns true if the page was populated, false otherwise
	*/
	virtual bool _loadProceduralPage(Page* page, PagedWorldSection* section);
	/** Give a world  the opportunity to unload page content procedurally. 
	@remarks
	You should not call this method directly. This call will happen in 
	the main render thread so it can access GPU resources. Use _unprepareProceduralPage
	for background preparation.
	@returns true if the page was populated, false otherwise
	*/
	virtual bool _unloadProceduralPage(Page* page, PagedWorldSection* section);
	/** Give a world  the opportunity to unprepare page content procedurally. 
	@remarks
	You should not call this method directly. This call may well happen in 
	a separate thread so it should not access GPU resources, use _unloadProceduralPage
	for that
	@returns true if the page was unpopulated, false otherwise
	*/
	virtual bool _unprepareProceduralPage(Page* page, PagedWorldSection* section);
	


	/// Called when the frame starts
	virtual void frameStart(Real timeSinceLastFrame);
	/// Called when the frame ends
	virtual void frameEnd(Real timeElapsed);
	/// Notify a world of the current camera
	virtual void notifyCamera(Camera* cam);

};

#endif