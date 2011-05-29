#include "LevelPage.h"
#include "LevelPageManager.h"
#include "LevelPagedWorldSection.h"

LevelPage::LevelPage(long x, long y, LevelPagedWorldSection* parent):
	Ogre::Page(LevelPageManager::packIndex(x,y),static_cast<Ogre::PagedWorldSection*>(parent))
{
	lvl = parent->getLevel();
}

LevelPage::~LevelPage()
{
}

void LevelPage::getCoords(long &x, long &y)
{
	LevelPageManager::unpackIndex(mID,&x,&y);
}

/** Load this page. 
@param synchronous Whether to force this to happen synchronously.
*/
void LevelPage::load(bool synchronous)
{
	Ogre::Page::load(synchronous);
}
/** Unload this page. 
*/
void LevelPage::unload()
{
	Ogre::Page::unload();
}

/// Save page data to an automatically generated file name
void LevelPage::save()
{
	Ogre::Page::save();
}
/// Save page data to a file
void LevelPage::save(const Ogre::String& filename)
{
	Ogre::Page::save(filename);
}
/// Save page data to a serialiser 
void LevelPage::save(Ogre::StreamSerialiser& stream)
{
	Ogre::Page::save(stream);
}

/// Called when the frame starts
void LevelPage::frameStart(Ogre::Real timeSinceLastFrame)
{
	Ogre::Page::frameStart(timeSinceLastFrame);
}
/// Called when the frame ends
void LevelPage::frameEnd(Ogre::Real timeElapsed)
{
	Ogre::Page::frameStart(timeElapsed);
}
/// Notify a section of the current camera
void LevelPage::notifyCamera(Ogre::Camera* cam)
{
	Ogre::Page::notifyCamera(cam);
}

bool LevelPage::prepareImpl(Ogre::Page::PageData* dataToPopulate)
{
	return Ogre::Page::prepareImpl(dataToPopulate);
}
bool LevelPage::prepareImpl(Ogre::StreamSerialiser& str, Ogre::Page::PageData* dataToPopulate)
{
	return Ogre::Page::prepareImpl(str,dataToPopulate);
}
void LevelPage::loadImpl()
{
	Ogre::Page::loadImpl();
}