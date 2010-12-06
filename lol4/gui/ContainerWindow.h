#ifndef __containerWindow
#define __containerWindow

#include "SlotView.h"
#include "EquipmentView.h"
#include "GameWindow.h"
#include "InventoryDisplayWindow.h"


//the actual window
class ContainerWindow: public InventoryDisplayWindow
{
public:
	ContainerWindow();

	~ContainerWindow();

	void show();

	inline void show(Inventory *inv, GameObject *owner)
	{
		mOwner = owner;
		slotView->setInventory(inv);
		show();
	}

	void setInventory(Inventory *inv, GameObject *owner)
	{
		mOwner = owner;
		mInv = inv;
		slotView->setInventory(inv);
	}
	
	bool eventCloseClicked(const CEGUI::EventArgs& e);
	
	bool eventMouseUp(const CEGUI::EventArgs& e)
	{
		return InventoryDisplayWindow::eventMouseUp(e);
	}
	
	//a little workaround...
	bool eventShown(const CEGUI::EventArgs& e)
	{
		return InventoryDisplayWindow::eventShown(e);
	}

	void inventoryChanged();

	GameObject *getOwner()
	{
		return mOwner;
	}
private:
	GameObject *mOwner;

	
	
};


#endif