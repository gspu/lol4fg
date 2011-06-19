#include "LevelTerrainGroup.h"
#include "Level.h"

LevelTerrainGroup::LevelTerrainGroup(Level *lvl, Ogre::uint16 terrainSize, Ogre::Real terrainWorldSize)
		: mSceneManager(lvl->getSceneManager())
		, mAlignment(Terrain::ALIGN_X_Z)//default
		//the world/worldsection should set the next two
		, mTerrainSize(terrainSize)//probably the size of the heightmap
		, mTerrainWorldSize(terrainWorldSize)//probably the size of the actual cell
		, mOrigin(Vector3::ZERO)
		//not relevant, since I won't load stuff the ogre way
		, mFilenamePrefix("terrain")//not relevant
		, mFilenameExtension("dat")//not relevant
		, mResourceGroup(ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME)
	{
		mDefaultImportData.terrainAlign = align;
		mDefaultImportData.terrainSize = terrainSize;
		mDefaultImportData.worldSize = terrainWorldSize;
		// by default we delete input data because we copy it, unless user
		// passes us an ImportData where they explicitly don't want it copied
		mDefaultImportData.deleteInputData = true;

		WorkQueue* wq = Root::getSingleton().getWorkQueue();
		mWorkQueueChannel = wq->getChannel("Ogre/TerrainGroup");
		wq->addRequestHandler(mWorkQueueChannel, this);
		wq->addResponseHandler(mWorkQueueChannel, this);

}