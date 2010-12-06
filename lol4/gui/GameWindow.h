#ifndef _gamewindow
#define _gamewindow

#include <defines.h>
#include <CEGUI.h>
#include <Ogre.h>

////This is a class for the usage of CEGUI::Scrollbar as a slider
////NOT inheriting CEGUI::Scrollbar since I have no idea what would happen
////if I were to downcast a CEGUI::Scrollbar to this
////should be better if this is not used as a pointer. it will never delete the scrollbar.
//class GameScrollBar
//{
//	/*
//	general stuff about cegui scrollbars:
//	- maximum size is documentSize - pageSize
//	- step size should be the smallest unit of position data
//	- cegui seems to have arrows on the scroll thingy itself
//	- amount of position change on clicking on these arrows: (pageSize - overlapSize) 
//	- amount of position change on clicking on the regular arrows: stepSize
//	overlapSize should be 0 here
//	*/
//public:
//	GameScrollBar(CEGUI::Scrollbar *bar, float min, float max, float stepSize):
//		mBar(bar),
//		min(0),
//		max(1)
//	{}
//	//just sets stuff
//	void setMinMax(float _min, float _max)
//	{
//		min = _min;
//		max = _max;
//		//also try something
//		mBar->setDocumentSize(max-min);
//	}
//	//makes it so that only multiples of size are valid scrollbar positions
//	//min-max should be dividable by size. 1 is a good value
//	void setStepSize(float size)
//	{
//		mBar->setStepSize(size);
//	}
//
//	//gets the scrollbar position, which is made to be between min and max
//	inline float getScrollbarPos()
//	{
//		//return min+(bar->getScrollPosition()/(bar->getDocumentSize()-bar->getPageSize()))*(max-min);
//		return (mBar->getScrollPosition()*(max-min))/(mBar->getDocumentSize()-mBar->getPageSize()) + min;
//	}
//
//	//sets the scrollbar position to a value between given min and max
//	inline void setScrollbarPos(float pos)
//	{		
//		mBar->setScrollPosition((pos-min)/(max-min) * (mBar->getDocumentSize()-mBar->getPageSize()));
//	}
//private:
//	CEGUI::Scrollbar *mBar;
//	float min;
//	float max;
//
//};
//
//primary cegui wrapper class
class GameWindow
{
public:
	//default constructor. does nothing. yet.
	GameWindow():
		mWnd(NULL),
		ignoreEvents(false)
	{}

	//~GameWindow();

	/*
	//loads layoutFile and assigns result to mWnd
	inline GameWindow(CEGUI::String layoutFile)
	{
		loadLayout(layoutFile);
	}
	*/
	//retrieves the primary window
	inline CEGUI::Window *getMainWindow()
	{
		return mWnd;
	}
	//gets a window with a specified name. basically it adds mPrefix and
	//retrieves it from mWnd
	CEGUI::Window *getWindow(CEGUI::String name);
	//checks if a window with the specified name is child of this window
	bool hasWindow(CEGUI::String name);

	//returns the type of mWnd without the layout prefix
	CEGUI::String getType()
	{
		return getWindowType(mWnd);
	}

	//returns the type of given window without the layout prefix
	CEGUI::String getWindowType(CEGUI::Window *wnd);

	//resizes a combobox so that the list is the right size for the items
	//shamelessly stolen from http://www.cegui.org.uk/phpBB2/viewtopic.php?f=10&t=3684&start=0
	//param cb: the combobox to resize
	//param maxItems: limits the number of items to check the size of
	void sizeComboList(CEGUI::Combobox* cb,CEGUI::uint maxItems = 10);

	//generates a window from a type and name. 
	//@param type: the type of the window, auto-prefixed with STYLE_GAME
	//@param name: name of the window, auto-prefixed with mPrefix
	CEGUI::Window *createWindow(CEGUI::String type, CEGUI::String name);
	//removes a window - by string. if the window has no child with given name, nothing happens
	void destroyWindow(CEGUI::String name);
	//removes a window - by reference. this will destroy child even if it is not a child of mWnd
	void destroyWindow(CEGUI::Window *child)
	{
		CEGUI::WindowManager::getSingletonPtr()->destroyWindow(child);
	}
	//removes all children from a given window
	void destroyAllChildren(CEGUI::Window *parent);


	//adds the CEGUI::Window of this GameWindow to the in-game root window
	void addToRoot();

	//adds the CEGUI::Window of this GameWindow to wnd
	void addToWindow(CEGUI::Window *wnd);

	//sets itself as the current layout
	void setAsLayout();
	
	virtual void show();
	virtual void hide();
	inline bool isShown()
	{
		if(!mWnd)
			return false;
		return mWnd->isVisible();
	}
	inline void setIgnoreEvents(bool ignore)
	{
		ignoreEvents = ignore;
	}

protected:
	//if true, events like eventresize are ignored. to be used in constructors etc.
	bool ignoreEvents;
	//primary widget of this window. FrameWindow containing everything, for example
	CEGUI::Window *mWnd;

	//static index for prefixing the windows. now a layout can be loaded multiple times, yay!
	static unsigned int prefixIndex;	

	//actual prefix of this window
	CEGUI::String mPrefix;

	

	////builds the actual string prefix from a given index
	//inline CEGUI::String buildPrefix(unsigned int p)
	//{
	//	return "wnd"+CEGUI::PropertyHelper::uintToString(p)+"_";
	//}
	//generates a prefix and stores it
	inline void createPrefix()
	{
		mPrefix = "wnd"+CEGUI::PropertyHelper::uintToString(prefixIndex)+"_";
		prefixIndex++;
	}

	//makes staticimage display the file filename
	//set filename to "" in order to clear a displayed image
	void applyImageToWindow(CEGUI::Window *staticimage,CEGUI::String filename,Ogre::String resourceGroup = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	//highlights given field with a border
	void highlightField(CEGUI::Window *field);

	////gets scrollbar position. in %, i guess...
	//inline Ogre::Real getScrollbarPos(CEGUI::Scrollbar *bar)
	//{
	//	return bar->getScrollPosition()/(bar->getDocumentSize()-bar->getPageSize());
	//}

	//inline void setScrollbarPos(CEGUI::Scrollbar *bar, Ogre::Real pos)
	//{		
	//	bar->setScrollPosition(pos * (bar->getDocumentSize()-bar->getPageSize()));
	//}

	//gets the scrollbar position, which is made to be between min and max
	inline Ogre::Real getScrollbarPos(CEGUI::Scrollbar *bar,Ogre::Real min = 0, Ogre::Real max = 1)
	{
		//return min+(bar->getScrollPosition()/(bar->getDocumentSize()-bar->getPageSize()))*(max-min);
		return (bar->getScrollPosition()*(max-min))/(bar->getDocumentSize()-bar->getPageSize()) + min;
	}

	//sets the scrollbar position to a value between given min and max
	inline void setScrollbarPos(CEGUI::Scrollbar *bar, Ogre::Real pos, Ogre::Real min = 0, Ogre::Real max = 1)
	{		
		bar->setScrollPosition((pos-min)/(max-min) * (bar->getDocumentSize()-bar->getPageSize()));
	}


	//loads a layout from a file and assigns it to mWnd
	void loadLayout(CEGUI::String filename);
	//makes CEGUI destroy whatever is in mWnd. For cleaning up after loadLayout was used
	void destroyLayout();

};

#endif