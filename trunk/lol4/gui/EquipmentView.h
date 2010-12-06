#ifndef __EquipmentView
#define __EquipmentView

#include "GameWindow.h"
#include <CEGUI.h>
#include <FwDec.h>
#include <data.h>

class EquipmentView: public GameWindow
{
public:
	//enum to identify my slots
	/*enum SlotType
	{
		stMelee		= 0,
		stRanged	= 1,
		stArmor		= 2,
		stShield	= 3,
		stElse1		= 4,
		stElse2		= 5,
		stElse3		= 6,
		stElse4		= 7,
		stBag		= 8
	};*/

	//constructs itself into the given target
	//it loads from EquipmentView.layout and adds
	//the root window from there as a child to target
	//at scale (0,0,1,1)
	EquipmentView(CEGUI::Window *target,InventoryWindow *parent);

	//updates the displayed stuff
	void update();

	//updates the stat displays
	void updateStats();

	//updates the inventory display thingy
	void updateInventoryDisplay();
private:
	CEGUI::Window *wnd_melee;
	CEGUI::Window *wnd_ranged;
	CEGUI::Window *wnd_armor;
	CEGUI::Window *wnd_shield;

	CEGUI::Window *wnd_else1;
	CEGUI::Window *wnd_else2;
	CEGUI::Window *wnd_else3;
	CEGUI::Window *wnd_else4;

	CEGUI::Window *wnd_bag;

	CEGUI::ProgressBar *display_melee;
	CEGUI::ProgressBar *display_ranged;
	CEGUI::ProgressBar *display_armor;

	CEGUI::Window *display_items_left;

	//storing it here in order not to retrieve it over and over
	GameApp *app;

	InventoryWindow *mInventoryWindow;
	////helper function for clicking with an empty mouse on a full slot
	//void takeOne(CEGUI::Window *wnd);
	////helper function for clicking on an empty slot while holding stuff
	//void placeOne(CEGUI::Window *wnd, Inventory::invDataItem item);
	
	//holds the names of the images to apply to empty slots
	CEGUI::String slotImage[EQUIP_CNT];
	
protected:
	bool equipmentClick(const CEGUI::EventArgs& e);
};

#endif