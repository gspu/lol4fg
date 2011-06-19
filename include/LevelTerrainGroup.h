#ifndef __levelTerrainGroup
#define __levelTerrainGroup

#include "Ogre.h"
#include "OgreTerrainGroup.h"
#include <LevelTerrainPrereqs.h>

class LevelTerrainGroup: public Ogre::TerrainGroup
{
	LevelTerrainGroup(Level *lvl, Ogre::uint16 terrainSize, Ogre::Real terrainWorldSize);
};

#endif