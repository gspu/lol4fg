#include "ExtendedTerrainGroup.h"
#include <OgreTerrain.h>

using namespace Ogre;

void ExtendedTerrainGroup::defineTerrain(long x, long y, float constantHeight)
{
	ExtendedSlot* slot = getExtendedSlot(x, y, true);

	slot->def.useImportData();
	slot->newDef.used = false;
	// Copy all settings, but make sure our primary settings are immutable
	*slot->def.importData = mDefaultImportData;
	slot->def.importData->constantHeight = constantHeight;
	slot->def.importData->terrainAlign = mAlignment;
	slot->def.importData->terrainSize = mTerrainSize;
	slot->def.importData->worldSize = mTerrainWorldSize;
}


void ExtendedTerrainGroup::defineTerrain(long x, long y, Ogre::DataStreamPtr _stream)
{
	
	ExtendedSlot* slot = getExtendedSlot(x, y, true);

	slot->newDef.used = true;
	slot->newDef.stream = _stream;
}

//void ExtendedTerrainGroup::ExtendedTerrainSlotDefinition::useStreamData()
//{
//	realDef.freeImportData();
//	realDef.filename.clear();
//}



ExtendedTerrainGroup::ExtendedSlot* ExtendedTerrainGroup::getExtendedSlot(long x, long y, bool createIfMissing)
{
	uint32 key = packIndex(x, y);
	TerrainSlotMap::iterator i = mTerrainSlots.find(key);
	if (i != mTerrainSlots.end())
		return static_cast<ExtendedTerrainGroup::ExtendedSlot*>(i->second);
	else if (createIfMissing)
	{
		ExtendedSlot* slot = OGRE_NEW ExtendedSlot(x, y);
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
WorkQueue::Response* ExtendedTerrainGroup::handleRequest(const WorkQueue::Request* req, const WorkQueue* srcQ)
{
	LoadRequest lreq = any_cast<LoadRequest>(req->getData());
	//ExtendedSlot *slot = dynamic_cast<ExtendedSlot*>(lreq.slot);
	ExtendedSlot *slot = static_cast<ExtendedSlot*>(lreq.slot);
	
	//if(!slot)
	
	TerrainSlotDefinition& def = slot->def;
	ExtendedTerrainSlotDefinition& newDef = slot->newDef;
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
