#ifndef __levelTerrainGroup
#define __levelTerrainGroup

#include "Ogre.h"
#include "OgreTerrainGroup.h"
#include <LevelTerrainPrereqs.h>
#include "FwDec.h"

class LevelTerrainGroup: public Ogre::TerrainGroup
{
public:
	LevelTerrainGroup(Level *lvl, Ogre::uint16 terrainSize, Ogre::Real terrainWorldSize);

	//overwriting ALL variations of defineTerrain since I need to overwrite getTerrainSlot,
	//which I can't
	
	//this one generates a filename and calls the one what takes x, y and a string. 
	//actually, no need to overwrite
	//virtual void defineTerrain(long x, long y);
	
	//constant height
	virtual void defineTerrain(long x, long y, float constantHeight);	
	//tweaking of the importdata might be necessary
	virtual void defineTerrain(long x, long y, const Terrain::ImportData* importData);	
	//image and LayerInstanceList. LayerInstanceList seems to be the list of textures or so
	virtual void defineTerrain(long x, long y, const Image* img, const Terrain::LayerInstanceList* layers = 0);	
	//a lot of floats and a LayerInstanceList
	virtual void defineTerrain(long x, long y, const float* pFloat, const Terrain::LayerInstanceList* layers = 0);
	//from filename. this will be redirected to load from level file instead
	virtual void defineTerrain(long x, long y, const String& filename);
	
};

#endif