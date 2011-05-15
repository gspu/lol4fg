#ifndef __lvlPage
#define __lvlPage


#include <Ogre.h>
#include "LevelTerrainPrereqs.h"
#include "FwDec.h"

class LevelPage: public Ogre::Page
{
protected:
	

	virtual bool prepareImpl(PageData* dataToPopulate);
	virtual bool prepareImpl(StreamSerialiser& str, PageData* dataToPopulate);
	virtual void loadImpl();



public:

	//i will calculate the id when necessary
	LevelPage(PagedWorldSection* parent);
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
	virtual void save(const String& filename);
	/// Save page data to a serialiser 
	virtual void save(StreamSerialiser& stream);

	/// Called when the frame starts
	virtual void frameStart(Real timeSinceLastFrame);
	/// Called when the frame ends
	virtual void frameEnd(Real timeElapsed);
	/// Notify a section of the current camera
	virtual void notifyCamera(Camera* cam);

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

	//those should be virtual
	/// WorkQueue::RequestHandler override
	bool canHandleRequest(const WorkQueue::Request* req, const WorkQueue* srcQ);
	/// WorkQueue::RequestHandler override
	WorkQueue::Response* handleRequest(const WorkQueue::Request* req, const WorkQueue* srcQ);
	/// WorkQueue::ResponseHandler override
	bool canHandleResponse(const WorkQueue::Response* res, const WorkQueue* srcQ);
	/// WorkQueue::ResponseHandler override
	void handleResponse(const WorkQueue::Response* res, const WorkQueue* srcQ);


};

#endif