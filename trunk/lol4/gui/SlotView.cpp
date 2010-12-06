#include "SlotView.h"
#include "elements/CEGUIScrollablePane.h"
#include "elements/CEGUIScrollbar.h"
#include "Inventory.h"
//#include "functions.h"
#include "TypeConverter.h"
#include "Alchemy.h"
#include "functions.h"
#include "GameApp.h"
#include "Level.h"
#include "GameChar.h"

const CEGUI::String SlotView::slotNamePrefix = "slot";
using namespace TypeConverter;
/*******************Slot********************/

SlotView::Slot::Slot(SlotView *parent,unsigned int x, unsigned int y,unsigned int id)
{	
	initGui();
    mWnd->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&SlotView::eventSlotClick, parent));
 
	parent->getContentPane()->addChildWindow(mWnd);
	mWnd->setPosition(CEGUI::UVector2(CEGUI::UDim(0,INV_IMAGESIZE*x),CEGUI::UDim(0,INV_IMAGESIZE*y)));
	mWnd->setID(id);
}


void SlotView::Slot::initGui()
{
	createPrefix();
	//initializing the gui
	mWnd = createWindow("StaticImage","imageContainer");
   
    countField = createWindow("StaticText","countField");
    
    countField->setText("");

    countField->setSize(CEGUI::UVector2(CEGUI::UDim(0,32),CEGUI::UDim(0,16)));
    mWnd->addChildWindow(countField);
    countField->setPosition(CEGUI::UVector2(CEGUI::UDim(1,-34),CEGUI::UDim(1,-16)));
    countField->setProperty("FrameEnabled","False");
    countField->setProperty("BackgroundEnabled","False");
    countField->setProperty("VertFormatting","TopAligned");
    countField->setProperty("HorzFormatting","RightAligned");
	countField->setProperty("MousePassThroughEnabled","True");
    
    mWnd->setSize(CEGUI::UVector2(CEGUI::UDim(0,INV_IMAGESIZE),CEGUI::UDim(0,INV_IMAGESIZE)));			
    

	
}
void SlotView::Slot::setPosition(unsigned int x, unsigned int y)
{
	mWnd->setPosition(CEGUI::UVector2(CEGUI::UDim(0,INV_IMAGESIZE*x),CEGUI::UDim(0,INV_IMAGESIZE*y)));
}
void SlotView::Slot::setImage(CEGUI::String filename)
{
	applyImageToWindow(mWnd,filename);
}
void SlotView::Slot::setCount(int count,bool showOne)
{
	if(!showOne && count == 1)
		countField->setText("");
	else
		countField->setText(cegui_str(count));
}
void SlotView::Slot::clear()
{
	applyImageToWindow(mWnd,"");
	countField->setText("");
	mWnd->setTooltipText("");
}
void SlotView::Slot::setToolTip(CEGUI::String tooltip)
{
	mWnd->setTooltipText(tooltip);
}



/*****************SlotView*******************/
void SlotView::setSize(unsigned int new_x,unsigned int new_y)
{
	unsigned int newSize = new_x * new_y;
	unsigned int oldSize = mSlots.size();
	if(newSize == oldSize)
		return;


	unsigned int x = 0;//loop variables, to hold the coordinates the currend field has to have
	unsigned int y = 0;

	if(newSize > oldSize)
	{
		//we have not enough. reorder and create more!
		mSlots.resize(newSize);

		
		for(unsigned int i=0;i<newSize;i++)
		{
			if(i<oldSize)//the existant ones
			{
				//reordering alerady existing ones
				Slot *cur = mSlots[i];
				cur->setPosition(x,y);
			}
			else//the new ones
			{
				Slot *cur = new Slot(this,x,y,i);//the i is important
				mSlots[i] = cur;
			}

			x++;
			if(x >= new_x)
			{
				x = 0;
				y++;
			}
		}
		
	}
	else
	{
		//we have too much. re-order, and then delete
		for(unsigned int i=0;i<oldSize;i++)
		{
			Slot *cur = mSlots[i];
			if(i<newSize)//the ones to keep
			{
				cur->setPosition(x,y);
			}
			else//the ones to delete
			{
				delete cur;
			}
			x++;
			if(x >= new_x)
			{
				x = 0;
				y++;
			}
		}
		mSlots.resize(newSize);
	}
	numfieldsX = new_x;
	numfieldsY = new_y;
}

SlotView::SlotView(CEGUI::Window *target)
{
	mWnd = target;
	initSlotGui();

	
	//subscribing
	mContentPane->subscribeEvent(CEGUI::Window::EventSized, CEGUI::Event::Subscriber(&SlotView::eventResize, this));
    mContentPane->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&SlotView::eventMove, this));




    //wndMain->subscribeEvent(CEGUI::Window::EventCloseClicked, CEGUI::Event::Subscriber(&SlotWindow::eventWindowClose, this));
    mScrollBar->subscribeEvent(CEGUI::Scrollbar::EventScrollPositionChanged, CEGUI::Event::Subscriber(&SlotView::eventScroll, this));
    mScrollBar->subscribeEvent(CEGUI::Scrollbar::EventMouseWheel,CEGUI::Event::Subscriber(&SlotView::eventScrollWnd, this));
	mContentPane->subscribeEvent(CEGUI::Window::EventMouseWheel,CEGUI::Event::Subscriber(&SlotView::eventScrollWnd, this));
}


SlotView::~SlotView()
{
	CEGUI::WindowManager *wmgr = CEGUI::WindowManager::getSingletonPtr();
	wmgr->destroyWindow(mContentPane);
	wmgr->destroyWindow(mScrollBar);
}
void SlotView::initSlotGui()
{
	using namespace CEGUI;
	CEGUI::WindowManager *wmgr = CEGUI::WindowManager::getSingletonPtr();
	//generating
	createPrefix();
	//mContentPane = createWindow("ScrollablePane","contentPane");
	mContentPane = createWindow("StaticText","contentPane");
	mScrollBar = static_cast<CEGUI::Scrollbar*>(createWindow("VerticalScrollbar","scrollBar"));
	mWnd->addChildWindow(mContentPane);
	mContentPane->addChildWindow(mScrollBar);
	
	//mContentPane->setSize(UVector2(UDim(0,15),UDim(0,20)));
	//x,y,w,h
	mContentPane->setArea(UDim(0,0),UDim(0,0),UDim(1,0),UDim(1,0));
	mScrollBar->setArea(UDim(1,-15),UDim(0,0),UDim(0,15),UDim(1,0));
	//15 ist die breite der scrollbar
}


void SlotView::updateVisibleArea()
{

    unsigned int numFields = numfieldsX*numfieldsY;
	/*unsigned int x = 0;
	unsigned int y = 0;*/
	
	unsigned int scrollIndex = getScrollIndex();

	for(size_t i = 0;i < numFields;i++)
	{
		
		//alle items anzeigen
		CEGUI::String image = "";
        unsigned int cnt = 0;
        CEGUI::String name = "";
		getItemData(i+numfieldsX*scrollIndex,image,name,cnt);
        updateSlot(i,image,name,cnt);
		/*x++;
		if(x % numfieldsX == 0)
		{
			x = 0;
			y++;
		}*/
	}
}


void SlotView::updateSlot(unsigned int x, unsigned int y,CEGUI::String filename,CEGUI::String tooltip,unsigned int cnt)
{
	updateSlot(x+y*numfieldsX,filename,tooltip,cnt);
	//using namespace TypeConverter;

	//CEGUI::ImagesetManager *imgr = CEGUI::ImagesetManager::getSingletonPtr();
	//CEGUI::WindowManager *wmgr = CEGUI::WindowManager::getSingletonPtr();
	//CEGUI::String iname = slotNamePrefix+cegui_str(x)+"_"+cegui_str(y);
	//CEGUI::String cname = slotNamePrefix+"Cnt"+cegui_str(x)+"_"+cegui_str(y);
	////DEBUG
	//Ogre::String iname_test = iname.c_str();
	//Ogre::String cname_test = cname.c_str();
	//CEGUI::Window *imgfield = getWindow(iname);
	//CEGUI::Window *cntfield = getWindow(cname);
	//if(cnt == 0)
	//{
	//	applyImageToWindow(imgfield,"");
	//	/*imgfield->setProperty("Image", "False");*/
	//	cntfield->setText("");
	//	imgfield->setTooltipText("");

	//}
	//else
	//{
	//	applyImageToWindow(imgfield,filename);		
	//	
	//	imgfield->setTooltipText(tooltip);
	//	if(cnt != 1)
	//		cntfield->setText(cegui_str(cnt));
	//	else
	//		cntfield->setText("");
	//}
 //   if(highlight)
 //       highlightField(imgfield);
}


void SlotView::updateSlot(unsigned int index,CEGUI::String filename,CEGUI::String tooltip,unsigned int cnt)
{
	using namespace TypeConverter;
	//getting the slot
	Slot *slot = getSlot(index);
	
	if(cnt == 0)
	{
		slot->clear();
	}
	else
	{
		slot->setImage(filename);
		slot->setToolTip(tooltip);
		slot->setCount(cnt);
	}
    
}



void SlotView::updateScrollbar()
{
	unsigned int cnt = getItemCount();
    
	unsigned int rowsNeeded = Ogre::Math::IFloor(cnt / numfieldsX);

    mScrollBar->setDocumentSize(rowsNeeded);
    mScrollBar->setPageSize(numfieldsY);
    mScrollBar->setStepSize(1);
    mScrollBar->setOverlapSize(2);

}

unsigned int SlotView::getScrollIndex()
{
	Ogre::Real val = mScrollBar->getScrollPosition();
    //mDebugText("val="+str(val)+", ifloor="+str(Math::IFloor(val)));
	return Ogre::Math::IFloor(val);
}


void SlotView::updateSlots()
{
    CEGUI::WindowManager *wmgr = CEGUI::WindowManager::getSingletonPtr();
    //nun hole ich mal die fensterdimensionen    
	CEGUI::Rect dims = mContentPane->getUnclippedInnerRect();//wndContainer->getInnerRect();
    Ogre::Real wWidth = dims.getWidth();
    Ogre::Real wHeight = dims.getHeight();
    
    //nun berechnen, wie viele reihen und spalten
	unsigned int rowLength = Ogre::Math::IFloor(wWidth / INV_IMAGESIZE);
    unsigned int numRows = Ogre::Math::IFloor(wHeight / INV_IMAGESIZE);
	
    if(rowLength == numfieldsX && numRows == numfieldsY)
        return;//muss nix gemacht werden
    
	setSize(rowLength,numRows);
	//playerInventoryFieldsCnt = invSize;
	/*numfieldsX = rowLength;
    numfieldsY = numRows;*/
	
	
	//CEGUI::WindowManager *wmgr = CEGUI::WindowManager::getSingletonPtr();
    //nun 2 schleifen
 //   int invIndex = 0;//diese IDs bleiben fest, beim Klicken wird daraus und aus der scrollpos das echte index bestimmt
 //   for(int y=0;y<numRows;y++)
 //   {
 //       for(int x=0;x<rowLength;x++)
 //       {
 //           //schauen ob das feld existiert, wenn ja, es repositionieren
 //           CEGUI::String cname = slotNamePrefix+cegui_str(x)+"_"+cegui_str(y);
	//		Ogre::String cname_test = cname.c_str();
 //           CEGUI::Window *curField = NULL;
 //           if(hasWindow(cname))
	//		{
	//			curField = getWindow(cname);
	//		}
	//		else
	//		{
 //               //erstellen				
 //               curField = createWindow("StaticImage",cname);
 //               curField->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&SlotView::eventSlotClick, this));
 //               //curField->setProperty("MousePassThroughEnabled","True");
	//			mContentPane->addChildWindow(curField);
 //               CEGUI::String fieldname = slotNamePrefix+"Cnt"+cegui_str(x)+"_"+cegui_str(y);
 //              
	//		    CEGUI::Window *cntfield = createWindow("StaticText",(CEGUI::utf8*)fieldname.c_str());
	//		    
	//		    cntfield->setText("");

	//		    cntfield->setSize(CEGUI::UVector2(CEGUI::UDim(0,32),CEGUI::UDim(0,16)));
 //               curField->addChildWindow(cntfield);
	//		    cntfield->setPosition(CEGUI::UVector2(CEGUI::UDim(1,-34),CEGUI::UDim(1,-16)));
	//		    cntfield->setProperty("FrameEnabled","False");
	//		    cntfield->setProperty("BackgroundEnabled","False");
	//		    cntfield->setProperty("VertFormatting","TopAligned");
	//		    cntfield->setProperty("HorzFormatting","RightAligned");
	//			cntfield->setProperty("MousePassThroughEnabled","True");
 //               
	//		    curField->setSize(CEGUI::UVector2(CEGUI::UDim(0,INV_IMAGESIZE),CEGUI::UDim(0,INV_IMAGESIZE)));			
	//		    curField->setPosition(CEGUI::UVector2(CEGUI::UDim(0,INV_IMAGESIZE*x),CEGUI::UDim(0,INV_IMAGESIZE*y)));
 //           }
 //           //nun ist curField gesetzt, nun wird der rest geamcht
 //           
	//		curField->setID(invIndex);
 //           invIndex++;
 //       }
 //       //nun schauen, ob am Ende der Zeile überflüssige Kästchen sind
 //       int x= rowLength;
 //       CEGUI::Window *badchild = NULL;
 //       CEGUI::String badName = slotNamePrefix+cegui_str(x)+"_"+cegui_str(y);
	//    if(hasWindow(badName))
	//    {
	//	    badchild = getWindow(badName);            
	//    }	
	//    while(badchild)
	//    {
	//	    wmgr->destroyWindow(badchild);
	//	    x++;
	//	    
	//	    CEGUI::String cname = slotNamePrefix+cegui_str(x)+"_"+cegui_str(y);

	//	    if(hasWindow(cname))
	//	    {
	//		    badchild = getWindow(cname);
	//	    }
	//	    else
	//	    {
	//		    badchild = NULL;
	//	    }
	//    }


 //   }
 //   //nun sind wir durch, jetzt schauen, ob wir überflüssige Zeilen haben, die gelöscht werden müssen
 //   int x = 0;
	//int y = numRows;
	//
	//
	//CEGUI::Window *badchild = NULL;
 //   CEGUI::String badName = slotNamePrefix+cegui_str(x)+"_"+cegui_str(y);
	//if(hasWindow(badName))
	//{
	//	badchild = getWindow(badName);
	//}	
	//while(badchild)
	//{
	//	destroyWindow(badchild);
	//	x++;
	//	if(x % rowLength == 0)
	//	{
	//		x = 0;
	//		y++;
	//	}
	//	CEGUI::String cname = slotNamePrefix+cegui_str(x)+"_"+cegui_str(y);

	//	if(hasWindow(cname))
	//	{
	//		badchild = getWindow(cname);
	//	}
	//	else
	//	{
	//		badchild = NULL;
	//	}
	//}	

	
}

bool SlotView::eventResize(const CEGUI::EventArgs& e)
{
	if(!isShown())
		return true;
	if(ignoreEvents)
		return true;
    updateView();
//    snapWindowSize();
    //oldWindowArea = wndMain->getArea(); //das erst am Ende. die Methoden brauchen evtl die alte Area
    return true;
}

bool SlotView::eventMove(const CEGUI::EventArgs& e)
{
	if(ignoreEvents)
		return true;
    //snapWindowSize();
    //oldWindowArea = mWnd->getArea();    
    return true;
}

//bool SlotView::eventWindowClose(const CEGUI::EventArgs& e)
//{
//	if(ignoreEvents)
//		return true;
//    //hide();
////   requestingHiding = true;
//    return true;
//}

bool SlotView::eventScroll(const CEGUI::EventArgs& e)
{   
	if(ignoreEvents)
		return true;
    updateVisibleArea();
    return true;
}

void SlotView::getWindowSizeDelta(Ogre::Real &delta_width, Ogre::Real &delta_height)
{
    
    using namespace CEGUI;
    //nun die Größe berechnen, die das Contentfeld haben muss
    CEGUI::WindowManager *wmgr = CEGUI::WindowManager::getSingletonPtr();
    //nun hole ich mal die fensterdimensionen    
    

    CEGUI::Rect contDims = mContentPane->getUnclippedInnerRect();
    Ogre::Real oldWidth = contDims.getWidth();
    Ogre::Real oldHeight = contDims.getHeight();

    Ogre::Real newWidth = numfieldsX*INV_IMAGESIZE + 15;//+width of the scrollbar!
    
	Ogre::Real newHeight= numfieldsY*INV_IMAGESIZE;
    delta_width = oldWidth - newWidth;
    delta_height = oldHeight - newHeight;


	/*setIgnoreEvents(true);
	snapWindowSize_parent(deltaWidth, deltaHeight);
    setIgnoreEvents(false);*/
   
}

bool SlotView::eventScrollWnd(const CEGUI::EventArgs& e)
{
	if(!isShown())
		return true;
    const CEGUI::MouseEventArgs& me = 
		static_cast<const CEGUI::MouseEventArgs&>(e);
    
    mScrollBar->setScrollPosition(mScrollBar->getScrollPosition() - mScrollBar->getStepSize()*me.wheelChange);
    updateVisibleArea();
    
    //mDebugText(str(me.wheelChange));
    return true;
}


//void SlotWindow::highlightField(Window *field)
//{
//	
//	CEGUI::Window *border;
//    CEGUI::WindowManager *wmgr = CEGUI::WindowManager::getSingletonPtr();
//	//"anlegen"
//	if(!wmgr->isWindowPresent(NAME_SPELLBORDER))
//	{
//		border = wmgr->createWindow("LandsLook/StaticImage",NAME_SPELLBORDER);
//		border->setProperty("FrameEnabled","False");
//		border->setProperty("BackgroundEnabled","False");
//		//border->setProperty("VertFormatting","TopAligned");
//		//border->setProperty("HorzFormatting","RightAligned");
//		ceguiSetImage(border,ICON_SELECTBORDER);
//		/*border->setSize(CEGUI::UVector2( 
//			CEGUI::UDim(0,INV_IMAGESIZE-8),
//			CEGUI::UDim(0,INV_IMAGESIZE-8) 
//		));*/
//	}
//	else
//	{
//		border = wmgr->getWindow(NAME_SPELLBORDER);
//		if(border->getParent())
//			border->getParent()->removeChildWindow(border);
//	}
//	if(field)
//	{
//		border->setVisible(true);
//		field->addChildWindow(border);
//		border->setArea(UDim(0,0),UDim(0,0),UDim(1,0),UDim(1,0));
//	}
//	else
//	{
//		border->setVisible(false);
//		if(border->getParent())
//		{
//			border->getParent()->removeChildWindow(border);
//		}
//		
//	}
//}

bool SlotView::eventSlotClick(const CEGUI::EventArgs& e)
{
	using namespace Ogre;
	if(ignoreEvents)
		return true;


    const CEGUI::MouseEventArgs& me = 
		static_cast<const CEGUI::MouseEventArgs&>(e);

    unsigned int wid = me.window->getID();
    int x = Math::IFloor(wid % numfieldsX);
    int y = Math::IFloor(wid / numfieldsX);

    wid += getScrollIndex()*numfieldsX;

    UniKey key = convertCeguiMouseButtonToOis(me.button);	

	if(me.sysKeys & CEGUI::Control)
	{
		key.setModifier(OIS::Keyboard::Ctrl);
	}
	else if(me.sysKeys & CEGUI::Alt)
	{
		key.setModifier(OIS::Keyboard::Alt);
	}
	else if(me.sysKeys & CEGUI::Shift)
	{
		key.setModifier(OIS::Keyboard::Shift);
	}
	
    slotClicked(key,wid,x,y);
    return true;
}

