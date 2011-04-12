#include "LevelTerrainGroup.h"

#include "OgreTerrain.h"

#include "OgreTerrainGroup.h"

#include "LevelTerrain.h"

using namespace Ogre;

void LevelTerrainGroup::defineTerrain(long x, long y, float constantHeight)
{
	LevelSlot* slot = getLevelSlot(x, y, true);

	slot->def.useImportData();
	slot->newDef.used = false;
	// Copy all settings, but make sure our primary settings are immutable
	*slot->def.importData = mDefaultImportData;
	slot->def.importData->constantHeight = constantHeight;
	slot->def.importData->terrainAlign = mAlignment;
	slot->def.importData->terrainSize = mTerrainSize;
	slot->def.importData->worldSize = mTerrainWorldSize;
}

void LevelTerrainGroup::defineTerrain(long x, long y)
{
	defineTerrain(x,y,0.0f);
	//Ogre::TerrainGroup::defineTerrain(x,y);
}

void LevelTerrainGroup::defineTerrain(long x, long y, Ogre::DataStreamPtr _stream)
{
	
	LevelSlot* slot = getLevelSlot(x, y, true);

	slot->newDef.used = true;
	slot->newDef.stream = _stream;
}

void LevelTerrainGroup::defineTerrain(long x, long y, const Terrain::ImportData* importData)
{
	LevelSlot* slot = getLevelSlot(x, y, true);

	slot->newDef.used = false;

	slot->def.useImportData();

	// Copy all settings, but make sure our primary settings are immutable
	*slot->def.importData = *importData;
	slot->def.importData->terrainAlign = mAlignment;
	slot->def.importData->terrainSize = mTerrainSize;
	slot->def.importData->worldSize = mTerrainWorldSize;

}


LevelTerrainGroup::LevelSlot* LevelTerrainGroup::getLevelSlot(long x, long y, bool createIfMissing)
{
	uint32 key = packIndex(x, y);
	TerrainSlotMap::iterator i = mTerrainSlots.find(key);
	if (i != mTerrainSlots.end())
		return static_cast<LevelTerrainGroup::LevelSlot*>(i->second);
	else if (createIfMissing)
	{
		LevelSlot* slot = OGRE_NEW LevelSlot(x, y);
		mTerrainSlots[key] = static_cast<TerrainSlot*>(slot);
		return slot;
	}
	return 0;
}

//void ExtendedTerrainGroup::handleResponse(const WorkQueue::Response* res, const WorkQueue* srcQ)
//	{
//		// No response data, just request
//		LoadRequest lreq = any_cast<LoadRequest>(res->getRequest()->getData());
//
//		if (res->succeeded())
//		{
//			TerrainSlot* slot = lreq.slot;
//			Terrain* terrain = slot->instance;
//			if (terrain)
//			{
//				// do final load now we've prepared in the background
//				// we must set the position
//				terrain->setPosition(getTerrainSlotPosition(slot->x, slot->y));
//
//				terrain->load();
//
//				// hook up with neighbours
//				for (int i = -1; i <= 1; ++i)
//				{
//					for (int j = -1; j <= 1; ++j)
//					{
//						if (i != 0 || j != 0)
//							connectNeighbour(slot, i, j);
//					}
//
//				}
//			}
//		}
//		else
//		{
//			// oh dear
//			LogManager::getSingleton().stream(LML_CRITICAL) <<
//				"We failed to prepare the terrain at (" << lreq.slot->x << ", " <<
//				lreq.slot->y <<") with the error '" << res->getMessages() << "'";
//			lreq.slot->freeInstance();
//		}
//
//	}
//
//---------------------------------------------------------------------
WorkQueue::Response* LevelTerrainGroup::handleRequest(const WorkQueue::Request* req, const WorkQueue* srcQ)
{
	LoadRequest lreq = any_cast<LoadRequest>(req->getData());
	//ExtendedSlot *slot = dynamic_cast<ExtendedSlot*>(lreq.slot);
	LevelSlot *slot = static_cast<LevelSlot*>(lreq.slot);
	
	//if(!slot)
	
	TerrainSlotDefinition& def = slot->def;
	TerrainSlotDefinitionAddon& newDef = slot->newDef;
	Terrain* t = slot->instance;
	assert(t && "Terrain instance should have been constructed in the main thread");
	WorkQueue::Response* response = 0;
	try
	{
		if(newDef.used)
		{
			StreamSerialiser ser(newDef.stream);
			t->prepare(ser);
		}
		else if (!def.filename.empty())
			t->prepare(def.filename);
		else
		{
			assert(def.importData && "No import data or file name");
			t->prepare(*def.importData);
			// if this worked, we can destroy the input data to save space
			def.freeImportData();
		}
		response = OGRE_NEW WorkQueue::Response(req, true, Any());
	}
	catch (Exception& e)
	{
		// oops
		response = OGRE_NEW WorkQueue::Response(req, false, Any(), 
			e.getFullDescription());
	}

	return response;


}

LevelTerrainGroup::~LevelTerrainGroup()
{
	//Ogre::TerrainGroup::~TerrainGroup();
}



void LevelTerrainGroup::loadTerrain(long x, long y, bool synchronous)
{
	TerrainSlot* slot = getTerrainSlot(x, y, false);
	if (slot)
	{
		loadLevelTerrainImpl(slot, synchronous);
	}
}

void LevelTerrainGroup::loadAllTerrains(bool synchronous)
{
	// Just a straight iteration - for the numbers involved not worth 
	// keeping a loaded / unloaded list
	for (TerrainSlotMap::iterator i = mTerrainSlots.begin(); i != mTerrainSlots.end(); ++i)
	{
		TerrainSlot* slot = i->second;
		loadLevelTerrainImpl(slot, synchronous);
	}
}

void LevelTerrainGroup::loadLevelTerrainImpl(TerrainSlot* slot, bool synchronous)
{
	if (!slot->instance && 
		(!slot->def.filename.empty() || slot->def.importData))
	{
		// Allocate in main thread so no race conditions
		slot->instance = OGRE_NEW LevelTerrain(mSceneManager);
		slot->instance->setResourceGroup(mResourceGroup);
		// Use shared pool of buffers
		slot->instance->setGpuBufferAllocator(&mBufferAllocator);

		LoadRequest req;
		req.slot = slot;
		req.origin = this;
		Root::getSingleton().getWorkQueue()->addRequest(
			mWorkQueueChannel, WORKQUEUE_LOAD_REQUEST, 
			Any(req), 0, synchronous);

	}
}