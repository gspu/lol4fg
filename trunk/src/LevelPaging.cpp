#include "LevelPaging.h"
#include "LevelPagedWorld.h"
#include "LevelPagedWorldSection.h"

const Ogre::String LevelPaging::LevelSectionFactory::FACTORY_NAME("LevelTerrain");

LevelPagedWorldSection* LevelPaging::createLevelWorldSection(LevelPagedWorld* world, Ogre::TerrainGroup* terrainGroup, 
			Ogre::Real loadRadius, Ogre::Real holdRadius, 
			Ogre::int32 minX, Ogre::int32 minY, Ogre::int32 maxX, Ogre::int32 maxY, 
			const Ogre::String& sectionName)
{
	
	LevelPagedWorldSection* ret = world->createLevelWorldSection(
		terrainGroup->getSceneManager(), LevelSectionFactory::FACTORY_NAME, sectionName);

	ret->init(terrainGroup);
	ret->setLoadRadius(loadRadius);
	ret->setHoldRadius(holdRadius);
	ret->setPageRange(minX, minY, maxX, maxY);

	ret->t_minX = minX;
	ret->t_minY = minY;
	ret->t_maxX = maxX;
	ret->t_maxY = maxY;

	ret->setPageListener(mPagingListener);

	return ret;
}

LevelPaging::~LevelPaging()
{
	mLog("nix");
}