#include "SpellbookWindow.h"
#include <CEGUI.h>
#include <TypeConverter.h>
#include <GameApp.h>
#include "Spellbook.h"
#include <HoldingSlotWindow.h>
#include "GameChar.h"


/*****************SpellbookSlotView******************/
bool SpellbookSlotView::getItemData(int at, CEGUI::String &image, CEGUI::String &name, unsigned int &count)
{
	image = "";
	name = "";
	count = 0;
	if(!mSb)
	{		
		return false;
	}
	gamedata_spell *cur = mSb->getAt(at);
	
	if(!cur)
	{		
		return false;
	}
	image = cur->inv_image;
	name = cur->name;
	count = 1;
	
	return true;
}

unsigned int SpellbookSlotView::getItemCount()
{
	return (mSb?mSb->getSpellCnt():0);
	/*
	  if(mInv)
        cnt = mInv->getMaxSize();
    else
        cnt = cSb->getSpellCnt();*/
}

void SpellbookSlotView::slotClicked(UniKey key,int index,int x, int y)
{
	bool shouldPlaySound = false;
    GameApp *app = GameApp::getSingletonPtr();
	
    if(mSb)
    {
		

		if(app->curHolding->hasItem() || app->curHolding->hasInventory())
		//ich halte einen item oder eine volle tasche. somit nix inventar
		    return;
		
		gamedata_spell *holdingSpell = app->curHolding->getSpell();//the spell I'm holding
	
	    gamedata_spell *slotSpell = mSb->getAt(index);//the spell I cliced on
	    //gamedata_spell *neu = NULL;
	    //gamedata_spell *newHolding = holdingSpell;//the one which will be held

		gamedata_spell *playerSpell = app->getPlayer()->getSpell();

	    if(slotSpell)
	    {
		    //there is something in the slot  		
		    /*ich machs mal so: 
		    linksklick/mittelklick: eines nehmen/eines ablegen
		    
		    rechtsklick: benutzen		
		    */
		    if(!holdingSpell)
		    {
			    //not holding anything				
				if(app->mFrameListener->key_is(key,keyInvTakeOne) || app->mFrameListener->key_is(key,keyInvTakeAll))			    
			    {
				    //take it
					mSb->setAt(index,NULL);// removeItemAt(index,1);
				    //neu.count -= 1;
                    updateSlot(index,"","",0);
				    
				    //newHolding = slotSpell;
					if(playerSpell == slotSpell)
					{
						//the current one is active
						app->curHolding->setHoldingSpell(slotSpell,true);
						mSpellbookWindow->removeSelectionBorder();//removing border
					}
					else
					{
						app->curHolding->setHoldingSpell(slotSpell,false);
					}
					
				    //app->setHoldingSpell(newHolding);
				    shouldPlaySound = true;
			    }			    
			    else if(app->mFrameListener->key_is(key,keyInvUse))
			    {					
				    //setting the spell as active
					mSpellbookWindow->setSelectionBorder(index);
					app->getPlayer()->setSpell(slotSpell);
					//slotSpell.item->onUse(app->getPlayer());
					shouldPlaySound = true;
			    }		   
    			
		    }
		    else
		    {
			    //I hold one and there is one in the field
				//swap slotSpell and holdingSpell		    
				
				    
				if(app->mFrameListener->key_is(key,keyInvTakeOne) || app->mFrameListener->key_is(key,keyInvTakeAll))
			    {
					mSb->setAt(index,holdingSpell);	
					updateSlot(index,holdingSpell->inv_image,holdingSpell->name,1);

					if(slotSpell == playerSpell)
					{	
						//the one in the slot is active
						//means, it must be cleared, and curHolding will be active
						mSpellbookWindow->removeSelectionBorder();
						app->curHolding->setHoldingSpell(slotSpell,true);
						
					}
					else if(holdingSpell == playerSpell)
					{	
						//the one being held is active
						//means, the slot in the spellbook must be activated and the holding one must be cleared
						mSpellbookWindow->setSelectionBorder(index);
						app->curHolding->setHoldingSpell(slotSpell,false);
						
					}
					else
					{
						//no one is active
						app->curHolding->setHoldingSpell(slotSpell,false);
					}					

					
					shouldPlaySound = true;	    
					  
			    }
		    }
	    }
	    else
	    {
		    //clicking an empty field
		    if(holdingSpell)
		    {
			    //Spellbook::invDataItem neu = slotSpell;
			    //...holding something to put down
			   
			    if(app->mFrameListener->key_is(key,keyInvTakeOne) || app->mFrameListener->key_is(key,keyInvTakeAll))//if(key == mFrameListener->mainKeys[keyInvTakeOne] || key == mFrameListener->altKeys[keyInvTakeOne])
			    {
				    //putting in
					mSb->setAt(index,holdingSpell);
			    
				    updateSlot(index,holdingSpell->inv_image,holdingSpell->name,1);

					if(holdingSpell == playerSpell)
					{
						mSpellbookWindow->setSelectionBorder(index);
					}
				    					
				    app->curHolding->setHoldingSpell(NULL);
				    shouldPlaySound = true;
			    
			    }
		    }
    		
	    }//ende von slotSpellCount != 0
	

	    
    }//ende von if(mInv)
	mSpellbookWindow->spellbookChanged();//to update the item count display
    if(shouldPlaySound)
    {
	    app->playPlayerSFX(app->clickSnd);
    }		
}

void SpellbookSlotView::updateParentWindow()
{
	if(!isShown())
		return;
	Ogre::Real delta_x,delta_y;
	getWindowSizeDelta(delta_x,delta_y);
	mSpellbookWindow->resizeWindow(delta_x,delta_y);
}


/*******************************SpellbookWindow*********************************/
SpellbookWindow::SpellbookWindow():
	selBorder(NULL),
	mSb(NULL)
{
	
	
	loadLayout("SpellbookWindow.layout");
	//Ogre::String test = TypeConverter::orge_str_TEST(mWnd->getName());
	//Ogre::String test = mWnd->getName();
	CEGUI::Window *wnd = getWindow("SpellbookWindow/content");
	slotView = new SpellbookSlotView(wnd,this);

	selBorder = createWindow("StaticImage","selBorder");
	applyImageToWindow(selBorder,"redborder.png");
	selBorder->setProperty("FrameEnabled","False");
    selBorder->setProperty("BackgroundEnabled","False");
	selBorder->setProperty("MousePassThroughEnabled","True");
	selBorder->hide();

	//add meself to root
	slotView->setIgnoreEvents(true);
	addToRoot();
	slotView->setIgnoreEvents(false);
	oldWndArea = CEGUI::URect(CEGUI::UDim(0,0),CEGUI::UDim(0,0),CEGUI::UDim(0,0),CEGUI::UDim(0,0));
	oldWndAreaRect = CEGUI::Rect(0,0,0,0);
	//SpellbookWndArea = mWnd->getArea();

	mWnd->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked, CEGUI::Event::Subscriber(&SpellbookWindow::eventCloseClicked, this));
	mWnd->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&SpellbookWindow::eventMouseUp, this));

	mWnd->subscribeEvent(CEGUI::Window::EventShown, CEGUI::Event::Subscriber(&SpellbookWindow::eventShown, this));

	slotView->updateView();
	slotView->updateParentWindow();

}

SpellbookWindow::~SpellbookWindow()
{
	delete slotView;
	destroyWindow(selBorder);
	destroyLayout();
	//~GameWindow should be called after this and clean up
}

void SpellbookWindow::setSelectionBorder(size_t index)
{	
	SlotView::Slot *slot = slotView->getSlot(index);
	setSelectionBorder(slot->getMainWindow());	
}

void SpellbookWindow::removeSelectionBorder()
{
	selBorder->getParent()->removeChildWindow(selBorder);
	selBorder->hide();
}

void SpellbookWindow::setSelectionBorder(CEGUI::Window *targetSlotWindow)
{
	if(targetSlotWindow)
	{
		targetSlotWindow->addChildWindow(selBorder);
		selBorder->show();
	}
	else
	{
		removeSelectionBorder();
	}
}

void SpellbookWindow::spellbookChanged()
{
//	eqView->updateSpellbookDisplay();
}

void SpellbookWindow::show()
{
	slotView->setIgnoreEvents(true);
	GameWindow::show();
//	eqView->update();
	slotView->setIgnoreEvents(false);
}

bool SpellbookWindow::eventMouseUp(const CEGUI::EventArgs& e)
{
	//request the delta size from Spellbookslotview
	slotView->updateParentWindow();
	/*Ogre::Real delta_w, delta_h;
	slotView->getWindowSizeDelta(delta_w,delta_h);
	resizeWindow(delta_w,delta_h);*/
	return true;
}

bool SpellbookWindow::eventCloseClicked(const CEGUI::EventArgs& e)
{
	GameApp::getSingletonPtr()->showSpellbook(false);
	return true;
}

bool SpellbookWindow::eventShown(const CEGUI::EventArgs& e)
{
	slotView->updateView();
	slotView->updateParentWindow();
	return true;
}

void SpellbookWindow::setWindowSize(Ogre::Real width,Ogre::Real height)
{
	using namespace CEGUI;
	//CEGUI::Size screenSize = TypeConverter::toCeguiSize(GameApp::getSingletonPtr()->getScreenSize());
	

	//CEGUI::URect wndArea = mWnd->getArea();
	CEGUI::Rect wndArea = mWnd->getUnclippedOuterRect();
	if(wndArea == oldWndAreaRect)
		return;
    /*if(wndArea.asAbsolute(screenSize) == oldWndArea.asAbsolute(screenSize))
        return;*/
    //CEGUI::Rect windowSizeAbs = wndArea.asAbsolute(screenSize);
	/*windowSizeAbs.setWidth(width);
	windowSizeAbs.setHeight(height);*/
	wndArea.setWidth(width);
	wndArea.setHeight(height);
  
    CEGUI::URect area(UDim(0,wndArea.d_left),UDim(0,wndArea.d_top),UDim(0,wndArea.d_right),UDim(0,wndArea.d_bottom));
   
    mWnd->setArea(area);

	oldWndAreaRect = wndArea;
}

void SpellbookWindow::resizeWindow(Ogre::Real delta_x,Ogre::Real delta_y,bool force)
{
	using namespace CEGUI;
	//CEGUI::Size screenSize = TypeConverter::toCeguiSize(GameApp::getSingletonPtr()->getScreenSize());
	

	//CEGUI::URect wndArea = mWnd->getArea();
	CEGUI::Rect wndArea = mWnd->getUnclippedOuterRect();
	if(!force && wndArea == oldWndAreaRect)
		return;

    
	wndArea.setWidth(wndArea.getWidth()-delta_x);
	wndArea.setHeight(wndArea.getHeight()-delta_y);
  
    CEGUI::URect area(UDim(0,wndArea.d_left),UDim(0,wndArea.d_top),UDim(0,wndArea.d_right),UDim(0,wndArea.d_bottom));
   
    mWnd->setArea(area);

	oldWndAreaRect = wndArea;
}
