#ifndef __praTerrainOverride
#define __praTerrainOverride
#include "Ogre.h"
#include <OgreTerrainPagedWorldSection.h>
#include "functions.h"
#include "OgrePagingPrerequisites.h"

#include "OgrePagedWorldSection.h"
#include "OgrePageManager.h"
#include "OgrePagedWorld.h"
#include "TypeConverter.h"
#include "OgreTerrainPaging.h"

//Paged World Section--------------------------------------------------------------------------------------------------
class ExtendedPagedWorldSection: public Ogre::TerrainPagedWorldSection
{
public:
	ExtendedPagedWorldSection(const Ogre::String& name, Ogre::PagedWorld* parent, Ogre::SceneManager* sm):
		Ogre::TerrainPagedWorldSection(name,parent,sm)
	{
	}

	void loadPage(Ogre::PageID pageID, bool forceSynchronous = false);

	//void unloadPage(Ogre::PageID pageID, bool forceSynchronous = false);


	Ogre::int32 t_minX;
	Ogre::int32 t_minY;
	Ogre::int32 t_maxX;
	Ogre::int32 t_maxY;
};

//Paged World----------------------------------------------------------------------------------------------------------
class ExtendedWorld: public Ogre::PagedWorld
{
public:
	ExtendedWorld(const Ogre::String& name, Ogre::PageManager* manager):
	  Ogre::PagedWorld(name,manager),
	  autoLoadingEnabled(true)
	  {}
		  

	ExtendedPagedWorldSection* createTestSection(Ogre::SceneManager* sceneMgr,
			const Ogre::String& typeName,
			const Ogre::String& sectionName = Ogre::StringUtil::BLANK);

	void notifyCamera(Ogre::Camera* cam);


	bool autoLoadingEnabled;
};

//Terrain Paging-------------------------------------------------------------------------------------------------------
class ExtendedPaging: public Ogre::TerrainPaging
{
public:
	ExtendedPaging(Ogre::PageManager* pageMgr):
		TerrainPaging(pageMgr)
		{}

	ExtendedPagedWorldSection* createTestWorldSection(ExtendedWorld* world, Ogre::TerrainGroup* terrainGroup, 
			Ogre::Real loadRadius, Ogre::Real holdRadius, 
			Ogre::int32 minX = -10, Ogre::int32 minY = -10, Ogre::int32 maxX = 10, Ogre::int32 maxY = 10, 
			const Ogre::String& sectionName = Ogre::StringUtil::BLANK);

	

protected:
	
	class ExtendedSectionFactory : public Ogre::TerrainPaging::SectionFactory
	{
	public:
		
		ExtendedPagedWorldSection* createInstance(const Ogre::String& name, Ogre::PagedWorld* parent, Ogre::SceneManager* sm);
		

	};

	SectionFactory mSectionFactory;
};

class ExtendedPageManager: public Ogre::PageManager
{
public:
	ExtendedPageManager(Level *lvl):
		Ogre::PageManager(),
		mLevel(lvl)
	  {
	  }

	//because I cannot overwrite createWorld
	ExtendedWorld* createTestWorld(const Ogre::String& name = Ogre::StringUtil::BLANK);

	inline Level *getLevel()
	{
		return mLevel;
	}
private:
	Level *mLevel;
};

#endif
