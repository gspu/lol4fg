#include "GameWindow.h"
#include <GameApp.h>
#include <functions.h>

unsigned int GameWindow::prefixIndex = 0;

CEGUI::String GameWindow::getWindowType(CEGUI::Window *wnd)
{
	using namespace CEGUI;
	CEGUI::String fullType = wnd->getType();
	CEGUI::String layoutPrefix = CEGUI::String(STYLE_GAME)+CEGUI::String("/");
	CEGUI::String::size_type prefixLength = layoutPrefix.size();

	//get substring...
	if(fullType.substr(0,prefixLength) == layoutPrefix)
	{		
		return fullType.substr(prefixLength);
	}
	else
	{
		return fullType;
	}
	//CEGUI::type
}

void GameWindow::sizeComboList(CEGUI::Combobox* cb,CEGUI::uint maxItems)
{
	//STUFF ADDED
	float extra_size = 5;
    // get the size of the attached items
    float items_size = 0.0f;
	int itemCount = cb->getItemCount();
	if(itemCount > maxItems && maxItems != 0)
		itemCount = maxItems;
    for (int i = 0; i < itemCount; ++i)
        items_size += cb->getListboxItemFromIndex(i)->getPixelSize().d_height;

    // get size of editbox portion
    float edit_size = cb->getEditbox()->getPixelSize().d_height;

    // calculate how much size is taken by frame imagery & scrollbars
    CEGUI::ComboDropList* dl = cb->getDropList();
    CEGUI::String render_area_name(dl->getHorzScrollbar()->isVisible() ?
        "ItemRenderingAreaHScroll" : "ItemRenderingArea" );

    const CEGUI::WidgetLookFeel& wl =
        CEGUI::WidgetLookManager::getSingleton().getWidgetLook(dl->getLookNFeel());

    float images_size = dl->getPixelSize().d_height -
        wl.getNamedArea(render_area_name).getArea().getPixelRect(*dl).getHeight();

    // consider the possibility of an offset list position
    float droplist_offset =
        dl->getYPosition().asAbsolute(cb->getPixelSize().d_height) - edit_size;

    // set final size
    cb->setHeight( CEGUI::UDim( 0.0f,
                   items_size + edit_size + images_size + droplist_offset + extra_size) );
}

void GameWindow::applyImageToWindow(CEGUI::Window *staticimage,CEGUI::String filename,Ogre::String resourceGroup)
{
	CEGUI::ImagesetManager *imgr = CEGUI::ImagesetManager::getSingletonPtr();
	
	
	if(filename != "")
	{
		if(!Ogre::ResourceGroupManager::getSingletonPtr()->resourceExists(resourceGroup,filename.c_str()))
		{
			filename = ERROR_IMAGE;
		}
		if(!imgr->isDefined(filename))
		{
			imgr->createFromImageFile(filename, filename,resourceGroup.c_str());
		}
		/*if(!imgr->isImagesetPresent(filename))
		{
			
			imgr->createImagesetFromImageFile(filename,filename,resourceGroup.c_str());
		}	*/	
		staticimage->setProperty("Image", "set:"+filename+" image:full_image");
	}
	else
	{
		staticimage->setProperty("Image", "False");
	}
}

CEGUI::Window *GameWindow::createWindow(CEGUI::String type, CEGUI::String name)
{
	//mLog("Creating Window "+name+". actual name: "+(mPrefix+name));
	return CEGUI::WindowManager::getSingletonPtr()->createWindow(
		CEGUI::String(STYLE_GAME)+CEGUI::String("/")+type,
			name==""?"":mPrefix+name
		);
}

void GameWindow::destroyAllChildren(CEGUI::Window *parent)
{
	size_t cCount = parent->getChildCount();
	//size_t delIndex = 0;
	if(cCount > 0)
	{
		for(size_t i=0;i<cCount;i++)
		{
			destroyWindow(parent->getChildAtIdx(0));
		}
	}	
}

void GameWindow::loadLayout(CEGUI::String layoutFile)
{
	CEGUI::WindowManager *wmgr = CEGUI::WindowManager::getSingletonPtr();	
	//mPrefix = buildPrefix(prefixIndex);	
	createPrefix();
	mWnd = wmgr->loadWindowLayout(layoutFile,mPrefix);
	//prefixIndex++;
}

bool GameWindow::hasWindow(CEGUI::String name)
{
	mLog("has Window? "+name+". actual name: "+(mPrefix+name));
	if(!mWnd)
		return false;
	/*if(!mWnd || !mWnd->isChild(mPrefix+name))
		return false;*/
	return CEGUI::WindowManager::getSingletonPtr()->isWindowPresent(mPrefix+name);
}

CEGUI::Window *GameWindow::getWindow(CEGUI::String name)
{
	mLog("getting Window "+name+". actual name: "+(mPrefix+name));
	if(!CEGUI::WindowManager::getSingletonPtr()->isWindowPresent(mPrefix+name))
		return NULL;
	return CEGUI::WindowManager::getSingletonPtr()->getWindow(mPrefix+name);
	/*if(!mWnd || !mWnd->isChild(mPrefix+name))
		return NULL;*/
	//return mWnd->getChild(mPrefix+name);	
}

void GameWindow::destroyWindow(CEGUI::String name)
{
	mLog("deleting Window "+name+". actual name: "+(mPrefix+name));
	CEGUI::WindowManager::getSingletonPtr()->destroyWindow(mPrefix+name);
}


void GameWindow::destroyLayout()
{
	CEGUI::WindowManager *wmgr = CEGUI::WindowManager::getSingletonPtr();
	wmgr->destroyWindow(mWnd);
}

//adds the CEGUI::Window of this GameWindow to the in-game root window
void GameWindow::addToRoot()
{
	addToWindow(GameApp::getSingletonPtr()->getRootWindow());
}

//adds the CEGUI::Window of this GameWindow to wnd
void GameWindow::addToWindow(CEGUI::Window *wnd)
{
	wnd->addChildWindow(mWnd);
}

void GameWindow::setAsLayout()
{
	CEGUI::System::getSingletonPtr()->setGUISheet(mWnd);	
}


void GameWindow::show()
{
	if(!mWnd)
		return;
	mWnd->show();
	
	/*mWnd->setVisible(true);
	mWnd->setEnabled(true);*/
}
void GameWindow::hide()
{
	if(!mWnd)
		return;
	mWnd->hide();
	/*mWnd->setVisible(false);
	mWnd->setEnabled(false);*/
}

void GameWindow::highlightField(CEGUI::Window *field)
{
	using namespace CEGUI;
	
	CEGUI::Window *border;
	//"anlegen"
	if(!hasWindow(NAME_SPELLBORDER))
	{
		border = createWindow("StaticImage",NAME_SPELLBORDER);
		border->setProperty("FrameEnabled","False");
		border->setProperty("BackgroundEnabled","False");
		//border->setProperty("VertFormatting","TopAligned");
		//border->setProperty("HorzFormatting","RightAligned");
		applyImageToWindow(border,ICON_SELECTBORDER);
		/*border->setSize(CEGUI::UVector2( 
			CEGUI::UDim(0,INV_IMAGESIZE-8),
			CEGUI::UDim(0,INV_IMAGESIZE-8) 
		));*/
	}
	else
	{
		border = getWindow(NAME_SPELLBORDER);
		if(border->getParent())
			border->getParent()->removeChildWindow(border);
	}
	if(field)
	{
		border->setVisible(true);
		field->addChildWindow(border);
		border->setArea(UDim(0,0),UDim(0,0),UDim(1,0),UDim(1,0));
	}
	else
	{
		border->setVisible(false);
		if(border->getParent())
		{
			border->getParent()->removeChildWindow(border);
		}
		
	}
}