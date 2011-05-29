#include "LevelPageManager.h"
#include "LevelPagedWorld.h"

template<> LevelPageManager* Ogre::Singleton<LevelPageManager>::ms_Singleton = 0;

LevelPageManager::LevelPageManager()
{
}

LevelPageManager::~LevelPageManager()
{
}

LevelPagedWorld* LevelPageManager::createLevelWorld(Level *lvl, const Ogre::String& name)
{
	Ogre::String theName = name;
	if (theName.empty())
	{
		do 
		{
			theName = mWorldNameGenerator.generate();
		} while (mWorlds.find(theName) != mWorlds.end());
	}
	else if(mWorlds.find(theName) != mWorlds.end())
	{
		OGRE_EXCEPT(Ogre::Exception::ERR_DUPLICATE_ITEM, 
			"World named '" + theName + "' already exists!",
			"PageManager::createWorld");
	}

	LevelPagedWorld* ret = OGRE_NEW LevelPagedWorld(lvl, theName);
	//PagedWorld* ret = OGRE_NEW PagedWorld(theName, this);
	mWorlds[theName] = static_cast<Ogre::PagedWorld*>(ret);

	return ret;

}

LevelPagedWorld* LevelPageManager::loadLevelWorld(Level *lvl, const Ogre::String& name)
{
	LevelPagedWorld* ret = createLevelWorld(lvl,name);

	ret->load();
	/*StreamSerialiser* ser = _readWorldStream(filename);
	ret->load(*ser);
	OGRE_DELETE ser;*/

	return ret;
}

//packing functions. even better if this becomes a singleton
void LevelPageManager::unpackIndex(Ogre::uint32 key, long *x, long *y)
{
	// inverse of packIndex
	// unsigned versions
	Ogre::uint16 y16 = static_cast<Ogre::uint16>(key & 0xFFFF);
	Ogre::uint16 x16 = static_cast<Ogre::uint16>((key >> 16) & 0xFFFF);

	*x = static_cast<Ogre::int16>(x16);
	*y = static_cast<Ogre::int16>(y16);
}

Ogre::uint32 LevelPageManager::packIndex(long x, long y)
{
	// Convert to signed 16-bit so sign bit is in bit 15
	Ogre::int16 xs16 = static_cast<Ogre::int16>(x);
	Ogre::int16 ys16 = static_cast<Ogre::int16>(y);

	// convert to unsigned because we do not want to propagate sign bit to 32-bits
	Ogre::uint16 x16 = static_cast<Ogre::uint16>(xs16);
	Ogre::uint16 y16 = static_cast<Ogre::uint16>(ys16);

	Ogre::uint32 key = 0;
	key = (x16 << 16) | y16;

	return key;
}