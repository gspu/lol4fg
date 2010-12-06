#ifndef __holdingSlot
#define __holdingSlot

#include <SlotView.h>
#include "Inventory.h"
#include "gamedata.h"

/*
For displaying the currently holding whatever. It also keeps all the pointers itself
*/

class HoldingSlotWindow: public SlotView::Slot
{
public:
	HoldingSlotWindow();
	~HoldingSlotWindow();
	


	bool hasItem()
	{
		return (item.count != 0);
	}

	bool hasSpell()
	{
		return (spell != NULL);
	}

	bool hasInventory()
	{
		return (inventory != NULL);
	}

	bool isEmpty()
	{
		return (!item.count && !spell && !inventory);
	}

	//to check if the holding item is a container
	bool isHoldingContainer()
	{
		return (item.item->Container != NULL);
	}

	void setItemCount(unsigned int cnt)
	{
		item.count = cnt;
		setCount(item.count);//updates the display
	}

	unsigned int getItemCount()
	{
		return item.count;
	}

	void modItemCount(int mod)
	{
		item.count += mod;
		if(item.count == 0)
			clear();
		else
			setCount(item.count);//updates the display
	}

	void updatePosition();

	void clear();

	void setHoldingItem(gamedata_item *it, unsigned int count = 1, Inventory *inv = NULL);

	void setHoldingItem(Inventory::invDataItem it, Inventory *inv = NULL)
	{
		setHoldingItem(it.item, it.count, inv);
	}

	void setHoldingSpell(gamedata_spell *spell, bool isSelected = false);

	gamedata_spell *getSpell()
	{
		return spell;
	}

	Inventory::invDataItem getItem()
	{
		return item;
	}

	Inventory* getInventory()
	{
		return inventory;
	}
	void setSpellBorder(bool set);

	void show();
private:
	
	//an item, including quantity and stuff. if no item is being held, quantity and the item pointer are NULL
	Inventory::invDataItem item;

	//a spell. NULL if none
	gamedata_spell *spell;

	//if the item is a container with something inside, then here is it's inventory, otherwise NULL
	Inventory *inventory;

	//whenever the current spell is selected
	bool isSelected;

	CEGUI::Window *selBorder;

	
};


#endif