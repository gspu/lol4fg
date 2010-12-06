#include <InventoryDisplayWindow.h>
#include <HoldingSlotWindow.h>
#include "GameApp.h"
#include "Alchemy.h"

/*****************InventorySlotView******************/
bool InventorySlotView::getItemData(int at, CEGUI::String &image, CEGUI::String &name, unsigned int &count)
{
	image = "";
	name = "";
	count = 0;
	if(!mInv)
	{		
		return false;
	}
	Inventory::invDataItem cur = mInv->getItemAt(at);//*itr;
	if(!cur.item)
	{		
		return false;
	}
	image = cur.item->inv_image;
	name = cur.item->name;
	count = cur.count;
	
	return true;
}

unsigned int InventorySlotView::getItemCount()
{
	return (mInv?mInv->getMaxSize():0);
	/*
	  if(mInv)
        cnt = mInv->getMaxSize();
    else
        cnt = cSb->getSpellCnt();*/
}

void InventorySlotView::slotClicked(UniKey key,int index,int x, int y)
{
	bool shouldPlaySound = false;
    GameApp *app = GameApp::getSingletonPtr();
	
    if(mInv)
    {
		Inventory::invDataItem curHolding = app->curHolding->getItem();
		if(app->curHolding->hasSpell() || app->curHolding->hasInventory())
		//ich halte einen spell oder eine volle tasche. somit nix inventar
		    return;

	
	    Inventory::invDataItem cur = mInv->getItemAt(index);
	    Inventory::invDataItem neu = cur;
	    Inventory::invDataItem newHolding = curHolding;

	    if(cur.count != 0)
	    {
		    //dh da liegt was drin    		
		    /*ich machs mal so: 
		    linksklick: eines nehmen/eines ablegen
		    mittelklick: alle nehmen/alle ablegen
		    rechtsklick: benutzen		
		    */
		    if(curHolding.count == 0)
		    {
			    //nehmen, wenn ich nix halte				
				if(app->mFrameListener->key_is(key,keyInvTakeOne))			    
			    {
				    //eines nehmen
				    mInv->removeItemAt(index,1);
				    neu.count -= 1;
                    updateSlot(x,y,neu.item->inv_image,neu.item->name,neu.count);
				    
				    newHolding.item = cur.item;
				    newHolding.count = 1;
				    app->setHoldingItem(newHolding);
				    shouldPlaySound = true;
			    }
			    else if(app->mFrameListener->key_is(key,keyInvTakeAll))				
			    {
				    //alle nehmen
				    newHolding = cur;				
				    mInv->removeItemAt(index,cur.count);
				    neu.count = 0;
				    neu.item = NULL;
                    updateSlot(x,y,"","",0);
				    
				    app->setHoldingItem(newHolding);
				    shouldPlaySound = true;
				    //break;
			    }
			    else if(app->mFrameListener->key_is(key,keyInvUse))
			    {
				    //benutzen, was drin liegt
					cur.item->onUse(app->getPlayer());
			    }		   
    			
		    }
		    else
		    {
			    //dh ich klicke auf ein feld wo was drin ist, und halte dabei schon was
			    if(cur.item == curHolding.item)
			    {
				    //wenn ich das gleiche halte wie das was im feld liegt
				    unsigned int placed;
				    
					if(app->mFrameListener->key_is(key,keyInvTakeOne))
				    {
					    //eines dazutun
					    placed = mInv->addItemAt(index,curHolding.item,1);
					    if(placed == 1)
					    {
						    shouldPlaySound = true;
						    neu.count = cur.count+1;
						    neu.item = curHolding.item;
						    updateSlot(x,y,neu.item->inv_image,neu.item->name,neu.count);

						    newHolding.count -= 1;						
						    app->setHoldingItem(newHolding);
					    }
				    }
				    else if(app->mFrameListener->key_is(key,keyInvTakeAll))
				    {				    
					    //soviel dazutun wie geht
					    placed		= mInv->addItemAt(index,curHolding.item,curHolding.count);
					    neu.item	= curHolding.item;
					    neu.count	= placed+cur.count;
					    updateSlot(x,y,neu.item->inv_image,neu.item->name,neu.count);
					    
					    newHolding.count  = curHolding.count - placed;
					    shouldPlaySound = true;
					    app->setHoldingItem(newHolding);
				    }
			    }//von if(cur.item == curHolding.item
			    else
			    {
				    //mit etwas auf etwas anderes klicken
					if(Alchemy::getSingleton().isMixable(cur.item,curHolding.item))
					{
					}

				    //zuerst misc attempt
				    //wenn linksklick->jeweils eines mischen
				    // wenn cur oder curHolding 1 waren, kommt es dahin
				    // sonst: einfach ins inv stecken
				    //wenn mittelklick:
				    //versuch, alle gehaltenen mit allen im feld zu mischen
				    //resultat kommt dahin, wo vorher weniger waren
				    //am besten function miscItems(invDataItem &educt1, invDataItem &educt2, invDataItem &procut, bool miscAll  = false)
				    //die sich gleich um alles kümmert
				    //map von struct oder array mit 2 pointern, die dort nach größe sortiert vorkommen
    					
				    //sonst, wenn beide jeweils 1 sind -> vertauschen    				
			    }
    			

		    }

	    }
	    else
	    {
		    //klick auf leeres feld
		    if(curHolding.count != 0)
		    {
			    Inventory::invDataItem neu = cur;
			    //...und wir halten was zum ablegen
			    unsigned int placed;
			    //switch(me.button)
			    //{
			    //case CEGUI::LeftButton:
			    if(app->mFrameListener->key_is(key,keyInvTakeOne))//if(key == mFrameListener->mainKeys[keyInvTakeOne] || key == mFrameListener->altKeys[keyInvTakeOne])
			    {
				    //nur eines
				    placed = mInv->addItemAt(index,curHolding.item,1);
				    if(placed == 1)
				    {
					    neu.count = 1;
					    neu.item = curHolding.item;
					    updateSlot(x,y,neu.item->inv_image,neu.item->name,neu.count);
					    newHolding.count -= 1;						
					    app->setHoldingItem(newHolding);
					    shouldPlaySound = true;
				    }
			    }
			    else if(app->mFrameListener->key_is(key,keyInvTakeAll))//if(key == mFrameListener->mainKeys[keyInvTakeAll] || key == mFrameListener->altKeys[keyInvTakeAll])
			    {	
				    //break;
			    //case CEGUI::MiddleButton:
				    //jetzt alles ablegen. oder zumindest so viel wie geht
				    placed = mInv->addItemAt(index,curHolding.item,curHolding.count);
				    neu.item = curHolding.item;
				    neu.count  = placed;
				    updateSlot(x,y,neu.item->inv_image,neu.item->name,neu.count);
				    //newHolding.itemID = neu.itemID;
				    newHolding.count  = curHolding.count - placed;
				    shouldPlaySound = true;

				    app->setHoldingItem(newHolding);
				    //break;
			    }
		    }
    		
	    }//ende von curCount != 0
	

	    
    }//ende von if(mInv)
	mInventoryWindow->inventoryChanged();//to update the item count display
    if(shouldPlaySound)
    {
	    app->playPlayerSFX(app->clickSnd);
    }		
}

void InventorySlotView::updateParentWindow()
{
	if(!isShown())
		return;
	Ogre::Real delta_x,delta_y;
	getWindowSizeDelta(delta_x,delta_y);
	mInventoryWindow->resizeWindow(delta_x,delta_y);
}
/*******************************************************************************/

void InventoryDisplayWindow::show()
{
	slotView->setIgnoreEvents(true);
	GameWindow::show();
	
	slotView->setIgnoreEvents(false);
}

bool InventoryDisplayWindow::eventMouseUp(const CEGUI::EventArgs& e)
{
	//request the delta size from inventoryslotview
	slotView->updateParentWindow();
	/*Ogre::Real delta_w, delta_h;
	slotView->getWindowSizeDelta(delta_w,delta_h);
	resizeWindow(delta_w,delta_h);*/
	return true;
}

bool InventoryDisplayWindow::eventCloseClicked(const CEGUI::EventArgs& e)
{
	GameApp::getSingletonPtr()->hideOtherInventory();
	return true;
}

bool InventoryDisplayWindow::eventShown(const CEGUI::EventArgs& e)
{
	slotView->updateView();
	slotView->updateParentWindow();
	return true;
}

void InventoryDisplayWindow::setWindowSize(Ogre::Real width,Ogre::Real height)
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

void InventoryDisplayWindow::resizeWindow(Ogre::Real delta_x,Ogre::Real delta_y,bool force)
{
	using namespace CEGUI;
	//CEGUI::Size screenSize = TypeConverter::toCeguiSize(GameApp::getSingletonPtr()->getScreenSize());
	

	//CEGUI::URect wndArea = mWnd->getArea();
	CEGUI::Rect wndArea = mWnd->getUnclippedOuterRect();
	if(!force && wndArea == oldWndAreaRect)
		return;

    //CEGUI::Rect windowSizeAbs = wndArea.asAbsolute(screenSize);

    /*windowSizeAbs.d_x -= delta_x;
    windowSizeAbs.d_y -= delta_y;

    CEGUI::Size maxWndSize(windowSizeAbs.d_x,windowSizeAbs.d_y);
    
    CEGUI::Rect newRect = wndArea.asAbsolute(screenSize).constrainSizeMax(maxWndSize);
	*/
    /*CEGUI::URect area(UDim(0,windowSizeAbs.d_left),UDim(0,windowSizeAbs.d_top),UDim(0,windowSizeAbs.d_right),UDim(0,windowSizeAbs.d_bottom));
   
    mWnd->setArea(area);

	oldWndArea = wndArea;*/
	wndArea.setWidth(wndArea.getWidth()-delta_x);
	wndArea.setHeight(wndArea.getHeight()-delta_y);
  
    CEGUI::URect area(UDim(0,wndArea.d_left),UDim(0,wndArea.d_top),UDim(0,wndArea.d_right),UDim(0,wndArea.d_bottom));
   
    mWnd->setArea(area);

	oldWndAreaRect = wndArea;
}
