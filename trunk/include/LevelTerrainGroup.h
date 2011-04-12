#ifndef __levelTerrainGroup
#define __levelTerrainGroup

#include <OgreTerrainGroup.h>
#include <Ogre.h>
#include <OgreStreamSerialiser.h>

#include <LevelTerrainPrerequisites.h>

class LevelTerrainGroup: public Ogre::TerrainGroup
{
public:
	LevelTerrainGroup(LevelPagingListener *pagingListener, Ogre::SceneManager* sm, Ogre::Terrain::Alignment align, Ogre::uint16 terrainSize, 
		Ogre::Real terrainWorldSize):
		TerrainGroup(sm, align, terrainSize, terrainWorldSize),
		mPagingListener(pagingListener)
		{}

	~LevelTerrainGroup();

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


	//creates a LevelTerrain instead of an Ogre::Terrain
	/** Load any terrain instances that have been defined but not loaded yet. 
	@param synchronous Whether we should force this to happen entirely in the
		primary thread (default false, operations are threaded if possible)
	*/
	virtual void loadAllTerrains(bool synchronous = false);
	
	/** Load a specific terrain slot based on the definition that has already 
		been supplied.
	@param x, y The coordinates of the terrain slot relative to the centre slot (signed).
	@param synchronous Whether we should force this to happen entirely in the
		primary thread (default false, operations are threaded if possible)
	*/
	virtual void loadTerrain(long x, long y, bool synchronous = false);


	/** Slot for a terrain instance, together with its definition. */
	struct  LevelSlot: public TerrainSlot
	{
		/// Definition used to load the terrain
		TerrainSlotDefinitionAddon newDef;
		
		LevelSlot(long _x, long _y):
			TerrainSlot(_x,_y)
		{}

	};

	//I overwrite all of them, because they use the non-virtual getTerrainSlot

	virtual void defineTerrain(long x, long y);

	virtual void defineTerrain(long x, long y, Ogre::DataStreamPtr _stream);

	virtual void defineTerrain(long x, long y, float constantHeight);

	virtual void defineTerrain(long x, long y, const Terrain::ImportData* importData);

	virtual void defineTerrain(long x, long y, const Image* img, const Terrain::LayerInstanceList* layers = 0);

	virtual void defineTerrain(long x, long y, const float* pFloat, const Terrain::LayerInstanceList* layers = 0);

	virtual void defineTerrain(long x, long y, const String& filename);

	

	/// WorkQueue::RequestHandler override
	Ogre::WorkQueue::Response* handleRequest(const Ogre::WorkQueue::Request* req, const Ogre::WorkQueue* srcQ);
	
	/// WorkQueue::ResponseHandler override
	//void handleResponse(const Ogre::WorkQueue::Response* res, const Ogre::WorkQueue* srcQ);
private:
	void loadLevelTerrainImpl(TerrainSlot* slot, bool synchronous);
	
	//this method of the parent should never be called.
	//i really HATE it that I can't overwrite it
	//only defineTerrain's call it with true, so it should be okay
	//TerrainSlot* getTerrainSlot(long x, long y, bool createIfMissing);

	//similar to getTerrainSlot, but creates my extended class
	LevelSlot* getLevelSlot(long x, long y, bool createIfMissing);

	LevelPagingListener *mPagingListener;
};



#endif