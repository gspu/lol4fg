#ifndef __praLvlPaging
#define __praLvlPaging

#include "Ogre.h"
#include "LevelTerrainPrereqs.h"
#include "FwDec.h"

/*
It seems there has to be only one PageManager. Probably I should turn it into a singleton
*/
class LevelPageManager: public Ogre::PageManager, public Ogre::Singleton<LevelPageManager>
{
public:
	class LevelPageProvider
	{
	public:
		LevelPageProvider() {}
		virtual ~LevelPageProvider() {}

		/** Give a provider the opportunity to prepare page content procedurally. 
		@remarks
		This call may well happen in a separate thread so it should not access 
		GPU resources, use loadProceduralPage for that
		@returns true if the page was populated, false otherwise
		*/
		virtual bool prepareProceduralPage(Ogre::Page* page, Ogre::PagedWorldSection* section);
		/** Give a provider the opportunity to load page content procedurally. 
		@remarks
		This call will happen in the main render thread so it can access GPU resources. 
		Use prepareProceduralPage for background preparation.
		@returns true if the page was populated, false otherwise
		*/
		virtual bool loadProceduralPage(Ogre::Page* page, Ogre::PagedWorldSection* section);
		/** Give a provider the opportunity to unload page content procedurally. 
		@remarks
		You should not call this method directly. This call will happen in 
		the main render thread so it can access GPU resources. Use _unprepareProceduralPage
		for background preparation.
		@returns true if the page was populated, false otherwise
		*/
		virtual bool unloadProceduralPage(Ogre::Page* page, Ogre::PagedWorldSection* section);
		/** Give a provider the opportunity to unprepare page content procedurally. 
		@remarks
		You should not call this method directly. This call may well happen in 
		a separate thread so it should not access GPU resources, use _unloadProceduralPage
		for that
		@returns true if the page was unpopulated, false otherwise
		*/
		virtual bool unprepareProceduralPage(Ogre::Page* page, Ogre::PagedWorldSection* section);

		/** Get a serialiser set up to read PagedWorld data for the given world filename. 
		The filename should refer to a file in the level zip
		@remarks
		The StreamSerialiser returned is the responsibility of the caller to
		delete. 
		*/
		virtual Ogre::StreamSerialiser* readWorldStream(const Ogre::String& filename);
		/** Get a serialiser set up to write PagedWorld data for the given world filename. 
		The filename should refer to a file in the level zip
		@remarks
		The StreamSerialiser returned is the responsibility of the caller to
		delete. 
		*/
		virtual Ogre::StreamSerialiser* writeWorldStream(const Ogre::String& filename);
		/** Get a serialiser set up to read Page data for the given PageID, 
			or null if this provider cannot supply one. 
			I will use packed IDs, like in the Ogre TerrainPaging exaple
		@remarks
			The StreamSerialiser returned is the responsibility of the caller to
			delete. 
		@param pageID The ID of the page being requested
		@param section The parent section to which this page will belong
		*/
		virtual Ogre::StreamSerialiser* readPageStream(Ogre::PageID pageID, Ogre::PagedWorldSection* section);

		/** Get a serialiser set up to write Page data for the given PageID, 
		or null if this provider cannot supply one. 
		I will use packed IDs, like in the Ogre TerrainPaging exaple
		@remarks
		The StreamSerialiser returned is the responsibility of the caller to
		delete. 
		@param pageID The ID of the page being requested
		@param section The parent section to which this page will belong
		*/
		virtual Ogre::StreamSerialiser* writePageStream(Ogre::PageID pageID, Ogre::PagedWorldSection* section);
	};


	LevelPageManager();

	LevelPageManager(Level *mLevel);

	~LevelPageManager();

	/** Destroy a world.
	The World seems to be the important, level-dependent stuff*/
	void destroyWorld(LevelPagedWorld* world);

	//packing functions. even better if this becomes a singleton
	static void unpackIndex(Ogre::uint32 key, long *x, long *y);

	static Ogre::uint32 packIndex(long x, long y);
protected:
	Level *mLevel;
};

#endif