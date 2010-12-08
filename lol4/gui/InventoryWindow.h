#ifndef _inventorywindow
#define _inventorywindow
#include <SlotView.h>
#include <EquipmentView.h>
#include <GameWindow.h>
#include <InventoryDisplayWindow.h>




//the actual window
class InventoryWindow: public InventoryDisplayWindow
{
public:
	InventoryWindow();

	~InventoryWindow();

	void show();

	inline void show(Inventory *inv)
	{
		slotView->setInventory(inv);
		show();
	}

	void setInventory(Inventory *inv)
	{
		mInv = inv;
		slotView->setInventory(inv);
		/*if(isShown())
			slotView->updateView();*/
		eqView->updateInventoryDisplay();
	}

	//to be called from InventorySlotView when something has been taken out or added
	//for updating the items left display
	void inventoryChanged();

	//to be called from EquipmentView when something was changed
	void equipmentChanged();
	//to be called from InventorySlotView when it wants to adapt the window size
	//void resizeWindow(Ogre::Real delta_x,Ogre::Real delta_y,bool force = false);

	//setting absolute sizes for the window
	//void setWindowSize(Ogre::Real width,Ogre::Real height);

	inline Inventory *getInventory()
	{
		return mInv;
	}

	//to enable and disable the part of the window where the inventory is displayed
	void showInventoryView();
	void hideInventoryView();

	bool eventMouseUp(const CEGUI::EventArgs& e)
	{
		return InventoryDisplayWindow::eventMouseUp(e);
	}
	bool eventCloseClicked(const CEGUI::EventArgs& e);
	
	//a little workaround...
	bool eventShown(const CEGUI::EventArgs& e)
	{
		return InventoryDisplayWindow::eventShown(e);
	}
	
private:
	//virtual void initLayout();

	//InventorySlotView *slotView;
	EquipmentView *eqView;
	//CEGUI::URect oldWndArea;//for checking if resizing is necessary
	//CEGUI::Rect oldWndAreaRect;//for checking if resizing is necessary
	////CEGUI::URect inventoryWndArea;//backupping the area when the inventory window is open
	//Ogre::Real inventoryDelta;//backupping the delta height value

	//bool eventMouseUp(const CEGUI::EventArgs& e);
	
	//
	////a little workaround...
	//bool eventShown(const CEGUI::EventArgs& e);
	
};

#endif