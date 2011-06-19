#include "LevelPage.h"
#include "LevelPageManager.h"
#include "LevelPagedWorldSection.h"
#include "functions.h"
#include "TypeConverter.h"
#include "DynamicLines.h"
#include "OgreGrid2DPageStrategy.h"
#include "Level.h"

using namespace TypeConverter;
using namespace Ogre;
LevelPage::LevelPage(long x, long y, LevelPagedWorldSection* parent):
	Ogre::Page(LevelPageManager::packIndex(x,y),static_cast<Ogre::PagedWorldSection*>(parent))
{
	lvl = parent->getLevel();
	
	Grid2DPageStrategyData* stratData = static_cast<Grid2DPageStrategyData*>(parent->getStrategyData());

	


	lines = NULL;

	
	
	

}

LevelPage::~LevelPage()
{
	if(lines)
	{
		linesNode->detachAllObjects();
		lvl->getSceneManager()->destroySceneNode(linesNode);
		delete lines;
	}
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
	long x;
	long y;
	LevelPageManager::unpackIndex(mID,&x,&y);
	mLog("Loading Page: "+ogre_str(x)+", "+ogre_str(y));

	

	Grid2DPageStrategyData* stratData = static_cast<Grid2DPageStrategyData*>(mParent->getStrategyData());


	if(!lines)
	{
		lines = new DynamicLines(Ogre::RenderOperation::OT_LINE_LIST);
		Real size = stratData->getCellSize();

		Ogre::Vector2 grid(x,y);
		Ogre::Vector3 world = Ogre::Vector3::ZERO;
		
		Real size_half = size/2;

		stratData->convertGridToWorldSpace(grid,world);

		world = size*world;
		//now do stuff.
		//world *should* contain the center of this page. testing required, though
		/*
		1--2
		|  |
		3--4
		*/

		Ogre::Vector3 point1(world.x-size_half,0,world.z+size_half);
		Ogre::Vector3 point2(world.x+size_half,0,world.z+size_half);
		Ogre::Vector3 point3(world.x-size_half,0,world.z-size_half);
		Ogre::Vector3 point4(world.x+size_half,0,world.z-size_half);

		lines->setMaterial("transform/red");
		lines->addPoint(point1);
		lines->addPoint(point2);

		lines->addPoint(point2);
		lines->addPoint(point4);

		lines->addPoint(point4);
		lines->addPoint(point3);

		lines->addPoint(point3);
		lines->addPoint(point1);

		//also, x
		lines->addPoint(point2);
		lines->addPoint(point3);

		lines->addPoint(point1);
		lines->addPoint(point4);

		lines->update();
		linesNode = lvl->getSceneManager()->getRootSceneNode()->createChildSceneNode();
		linesNode->attachObject(lines);

		Ogre::Page::load(synchronous);
	}
}
/** Unload this page. 
*/
void LevelPage::unload()
{
	long x;
	long y;
	LevelPageManager::unpackIndex(mID,&x,&y);
	mLog("UnLoading Page: "+ogre_str(x)+", "+ogre_str(y));
	if(lines)
	{
		linesNode->detachAllObjects();
		lvl->getSceneManager()->destroySceneNode(linesNode);
		delete lines;
		lines = NULL;
		linesNode = NULL;
	}
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