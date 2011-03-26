#ifndef __levelTerrainGroup
#define __levelTerrainGroup

#include <OgreTerrainGroup.h>
#include <Ogre.h>
#include <OgreStreamSerialiser.h>

#include <LevelTerrainPrerequisites.h>

class LevelTerrainGroup: public Ogre::TerrainGroup
{
public:
	LevelTerrainGroup(Ogre::SceneManager* sm, Ogre::Terrain::Alignment align, Ogre::uint16 terrainSize, 
		Ogre::Real terrainWorldSize):
		TerrainGroup(sm, align, terrainSize, terrainWorldSize)
		{}

	struct TerrainSlotDefinitionAddon
	{
		TerrainSlotDefinitionAddon():
			used(false)				
			{}
		//Ogre::TerrainGroup::TerrainSlotDefinition realDef;
		
		//stream. i hope this won't cause any memory leaks...
		//Ogre::StreamSerialiser stream;
		Ogre::DataStreamPtr stream;
		
		//whenever this thing is used instead of the default one
		bool used;



	};


	/** Slot for a terrain instance, together with its definition. */
	struct  LevelSlot: public TerrainSlot
	{
		/// Definition used to load the terrain
		TerrainSlotDefinitionAddon newDef;
		
		LevelSlot(long _x, long _y):
			TerrainSlot(_x,_y)
		{}

	};

	virtual void defineTerrain(long x, long y);

	virtual void defineTerrain(long x, long y, Ogre::DataStreamPtr _stream);

	virtual void defineTerrain(long x, long y, float constantHeight);

	

	/// WorkQueue::RequestHandler override
	Ogre::WorkQueue::Response* handleRequest(const Ogre::WorkQueue::Request* req, const Ogre::WorkQueue* srcQ);
	
	/// WorkQueue::ResponseHandler override
	//void handleResponse(const Ogre::WorkQueue::Response* res, const Ogre::WorkQueue* srcQ);
private:
	
	//this method of the parent should never be called.
	//i really HATE it that I can't overwrite it
	//only defineTerrain's call it with true, so it should be okay
	//TerrainSlot* getTerrainSlot(long x, long y, bool createIfMissing);

	//similar to getTerrainSlot, but creates my extended class
	LevelSlot* getLevelSlot(long x, long y, bool createIfMissing);
};



#endif