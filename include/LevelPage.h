#ifndef __lvlPage
#define __lvlPage


#include <Ogre.h>
#include "LevelTerrainPrereqs.h"
#include "FwDec.h"
#include <OgreWorkQueue.h>

class DynamicLines;

class LevelPage: public Ogre::Page
{
public:

	//i will calculate the id when necessary
	LevelPage(long x, long y, LevelPagedWorldSection* parent);
	~LevelPage();


	/** Load this page. 
	@param synchronous Whether to force this to happen synchronously.
	*/
	virtual void load(bool synchronous);
	/** Unload this page. 
	*/
	virtual void unload();


	//omfg yay, virtual stuff!
	/// Save page data to an automatically generated file name
	virtual void save();
	/// Save page data to a file
	virtual void save(const Ogre::String& filename);
	/// Save page data to a serialiser 
	virtual void save(Ogre::StreamSerialiser& stream);

	/// Called when the frame starts
	virtual void frameStart(Ogre::Real timeSinceLastFrame);
	/// Called when the frame ends
	virtual void frameEnd(Ogre::Real timeElapsed);
	/// Notify a section of the current camera
	virtual void notifyCamera(Ogre::Camera* cam);

	//absolutely no idea how this stuff is supposed to work or if i need it
	///** Create a new PageContentCollection within this page.
	//This is equivalent to calling PageManager::createContentCollection and 
	//then attachContentCollection.
	//@param typeName The name of the type of content collection (see PageManager::getContentCollectionFactories)
	//*/
	//virtual PageContentCollection* createContentCollection(const String& typeName);

	///** Destroy a PageContentCollection within this page.
	//This is equivalent to calling detachContentCollection and 
	//	PageManager::destroyContentCollection.
	//*/
	//virtual void destroyContentCollection(PageContentCollection* coll);
	///** Destroy all PageContentCollections within this page.
	//*/
	//virtual void destroyAllContentCollections();
	///// Get the number of content collections
	//virtual size_t getContentCollectionCount() const;
	///// Get a content collection
	//virtual PageContentCollection* getContentCollection(size_t index);
	///// Get the list of content collections
	//const ContentCollectionList& getContentCollectionList() const;

	//those should be virtual. not sure if i actually need to overwrite them, though
	///// WorkQueue::RequestHandler override
	//bool canHandleRequest(const Ogre::WorkQueue::Request* req, const Ogre::WorkQueue* srcQ);
	///// WorkQueue::RequestHandler override
	//Ogre::WorkQueue::Response* handleRequest(const Ogre::WorkQueue::Request* req, const Ogre::WorkQueue* srcQ);
	///// WorkQueue::ResponseHandler override
	//bool canHandleResponse(const Ogre::WorkQueue::Response* res, const Ogre::WorkQueue* srcQ);
	///// WorkQueue::ResponseHandler override
	//void handleResponse(const Ogre::WorkQueue::Response* res, const Ogre::WorkQueue* srcQ);

	void getCoords(long &x, long &y);
protected:
	Level *lvl;
	

	virtual bool prepareImpl(Ogre::Page::PageData* dataToPopulate);
	virtual bool prepareImpl(Ogre::StreamSerialiser& str, Ogre::Page::PageData* dataToPopulate);
	virtual void loadImpl();

	//DEBUG STUFF
	DynamicLines *lines;
	Ogre::SceneNode *linesNode;

};

#endif