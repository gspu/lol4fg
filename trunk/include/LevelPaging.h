#ifndef __levelPaging
#define __levelPaging

#include "Ogre.h"
#include <OgreTerrainPagedWorldSection.h>
#include "functions.h"
#include "OgrePagingPrerequisites.h"

#include "OgrePagedWorldSection.h"
#include "OgrePageManager.h"
#include "OgrePagedWorld.h"
#include "TypeConverter.h"
#include "OgreTerrainPaging.h"

#include <LevelTerrainPrerequisites.h>
class LevelPaging: public Ogre::TerrainPaging
{
public:
	LevelPaging(Ogre::PageManager* pageMgr):
		TerrainPaging(pageMgr)
		{}

	//again, the necessary method is not virtual
	LevelPagedWorldSection* createLevelWorldSection(LevelPagedWorld* world, Ogre::TerrainGroup* terrainGroup, 
			Ogre::Real loadRadius, Ogre::Real holdRadius, 
			Ogre::int32 minX = -10, Ogre::int32 minY = -10, Ogre::int32 maxX = 10, Ogre::int32 maxY = 10, 
			const Ogre::String& sectionName = Ogre::StringUtil::BLANK);

	

protected:

	class LevelSectionFactory : public Ogre::PagedWorldSectionFactory
	{
	public:
		static const Ogre::String FACTORY_NAME;
		const Ogre::String& getName() const;
		Ogre::PagedWorldSection* createInstance(const Ogre::String& name, Ogre::PagedWorld* parent, Ogre::SceneManager* sm);
		void destroyInstance(Ogre::PagedWorldSection*);

	};
	
	

	//SectionFactory mSectionFactory;
};

#endif