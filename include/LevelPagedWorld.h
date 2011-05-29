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
	LevelPagedWorld(Level *lvl, Ogre::String name);
	virtual ~LevelPagedWorld();

	// it might be indeed a good idea to move some of the loading stuff into the relevant classes.
	// no filename or so, I know where to find the data
	void load();

	//just save to level file
	void save();
	
	// non-virtual stuff, AGAIN... let's hope this functions will never be called.
	// i'll try to overwrite them anyway. maybe it'll help
	// they might be unnecessary, though. the TerrainPaging example does not overwrite PagedWorld, but PagedWorldSection
	/*void load(const Ogre::String& filename);
	void load(const Ogre::DataStreamPtr& stream);
	bool load(Ogre::StreamSerialiser& stream);
	void save(const Ogre::String& filename);
	void save(const Ogre::DataStreamPtr& stream);
	void save(Ogre::StreamSerialiser& stream);*/

	/*Ogre::PagedWorldSection* createSection(Ogre::SceneManager* sceneMgr,
		const Ogre::String& typeName,
		const Ogre::String& sectionName = Ogre::StringUtil::BLANK);
	Ogre::PagedWorldSection* createSection(const Ogre::String& strategyName, Ogre::SceneManager* sceneMgr,
		const Ogre::String& sectionName = Ogre::StringUtil::BLANK);
	Ogre::PagedWorldSection* createSection(Ogre::PageStrategy* strategy, Ogre::SceneManager* sceneMgr, 
		const Ogre::String& sectionName = Ogre::StringUtil::BLANK);*/


	LevelPagedWorldSection* createLevelSection(Level *lvl,
		const Ogre::String& sectionName = Ogre::StringUtil::BLANK);

	//typedef map<String, PagedWorldSection*>::type SectionMap;
	/// Retrieve a const reference to all the sections in this world
	//const SectionMap& getSections() const { return mSections; }

	
	//finally, virtual stuff

	/*
	i do not actually need to overwrite these, it just calls the function of the same name
	on the pageprovider if any, and if it doesn't work, it is called on the PageManager
			bool generated = false;
		if (mPageProvider)
			generated = mPageProvider->unprepareProceduralPage(page, section);
		if (!generated)
			generated = mManager->_unprepareProceduralPage(page, section);
		return generated;
	virtual bool _prepareProceduralPage(Ogre::Page* page, Ogre::PagedWorldSection* section);	
	virtual bool _loadProceduralPage(Ogre::Page* page, Ogre::PagedWorldSection* section);	
	virtual bool _unloadProceduralPage(Ogre::Page* page, Ogre::PagedWorldSection* section);	
	virtual bool _unprepareProceduralPage(Ogre::Page* page, Ogre::PagedWorldSection* section);
	*/
	


	/*don't need those either, they just call frameStart/End on all sections
	/// Called when the frame starts
	virtual void frameStart(Ogre::Real timeSinceLastFrame);
	/// Called when the frame ends
	virtual void frameEnd(Ogre::Real timeElapsed);*/

	/// Notify a world of the current camera
	// i overwrite this to be able to "freeze" the paging state
	virtual void notifyCamera(Ogre::Camera* cam);

};

#endif