#include "LevelTerrainGroup.h"
#include "OgreTerrainGroup.h"
#include "Level.h"
using namespace Ogre;
LevelTerrainGroup::LevelTerrainGroup(Level *lvl, Ogre::uint16 terrainSize, Ogre::Real terrainWorldSize):
	Ogre::TerrainGroup(lvl->getSceneManager())
	{
		//it seems, i CANNOT define the parent's members up there
		
		//the world/worldsection should set the next two
		mTerrainSize = terrainSize;//probably the size of the heightmap
		mTerrainWorldSize = terrainWorldSize;//probably the size of the actual cell
		mOrigin = Ogre::Vector3::ZERO;
		
		

		mDefaultImportData.terrainAlign = mAlignment;//align;
		mDefaultImportData.terrainSize = mTerrainSize;
		mDefaultImportData.worldSize = mTerrainWorldSize;
		// by default we delete input data because we copy it, unless user
		// passes us an ImportData where they explicitly don't want it copied
		mDefaultImportData.deleteInputData = true;

		WorkQueue* wq = Root::getSingleton().getWorkQueue();
		mWorkQueueChannel = wq->getChannel("Ogre/TerrainGroup");
		wq->addRequestHandler(mWorkQueueChannel, this);
		wq->addResponseHandler(mWorkQueueChannel, this);

}