#ifndef __InventoryDisplayWindow
#define __InventoryDisplayWindow

#include <SlotView.h>

class InventoryDisplayWindow;

//Class for handling an inventory view
class InventorySlotView: public SlotView
{
public:
	InventorySlotView(CEGUI::Window *target,InventoryDisplayWindow *parent,Inventory *inv = NULL)
		:SlotView(target),mInv(inv),mInventoryWindow(parent)
	{
	}
	inline void setInventory(Inventory *inv)
	{
		mInv = inv;
	}
	inline Inventory *getInventory()
	{
		return mInv;
	}
	void updateParentWindow();

	

private:
	//virtual void initLayout();

	bool getItemData(int at, CEGUI::String &image, CEGUI::String &name, unsigned int &count);
	inline bool isDataSet()
	{
		return (mInv != NULL);
	}
	unsigned int getItemCount();
	//void snapWindowSize_parent(Ogre::Real delta_x,Ogre::Real delta_y);
	Inventory *mInv;
	InventoryDisplayWindow *mInventoryWindow;

	
protected:
	void slotClicked(UniKey key,int index, int x, int y);
};




//the class inbetween
class InventoryDisplayWindow: public GameWindow
{
public:
	InventoryDisplayWindow():
		mInv(NULL)
	{
	}

	~InventoryDisplayWindow(){}

	virtual void show();

	inline void show(Inventory *inv)
	{
		slotView->setInventory(inv);
		show();
	}

	/*inline void show(Inventory *inv, GameChar *owner)
	{
		mOwner = owner;
		slotView->setInventory(inv);
		show();
	}*/

	virtual void setInventory(Inventory *inv)
	{
		mInv = inv;
		slotView->setInventory(inv);
	}

	//to be called from InventorySlotView when something has been taken out or added
	//for updating the items left display
	virtual void inventoryChanged() = 0;
	
	//to be called from InventorySlotView when it wants to adapt the window size
	virtual void resizeWindow(Ogre::Real delta_x,Ogre::Real delta_y,bool force = false);

	//setting absolute sizes for the window
	virtual void setWindowSize(Ogre::Real width,Ogre::Real height);

	inline Inventory *getInventory()
	{
		return mInv;
	}



	Inventory *mInv;

	


	virtual bool eventMouseUp(const CEGUI::EventArgs& e);
	virtual bool eventCloseClicked(const CEGUI::EventArgs& e);
	
	//a little workaround...
	virtual bool eventShown(const CEGUI::EventArgs& e);

protected:
	InventorySlotView *slotView;

	CEGUI::URect oldWndArea;//for checking if resizing is necessary
	CEGUI::Rect oldWndAreaRect;//for checking if resizing is necessary
	//CEGUI::URect inventoryWndArea;//backupping the area when the inventory window is open
	Ogre::Real inventoryDelta;//backupping the delta height value

	

	GameChar *mOwner;
	
};


#endif