#include "EquipmentView.h"
#include <GameChar.h>
#include <GameApp.h>
#include <algorithm>
#include <TypeConverter.h>
#include "HoldingSlotWindow.h"
#include "InventoryWindow.h"


using namespace CEGUI;
using namespace TypeConverter;

//little helper function to get a maximal damage value
int dmgMax(Damage dmg)
{
	int res = 0;
	for(size_t i=0;i<9;i++)
	{
		if(dmg[i] > res)
			res = dmg[i];
	}
	return res;
}



EquipmentView::EquipmentView(CEGUI::Window *target,InventoryWindow *parent):
	mInventoryWindow(parent)
{
	app = GameApp::getSingletonPtr();

	loadLayout("EquipmentView.layout");
	target->addChildWindow(mWnd);
	mWnd->setArea(UDim(0,0),UDim(0,0),UDim(1,0),UDim(1,0));

	//getting the fields
	wnd_melee = getWindow("PlayerInventory/melee");
	wnd_ranged = getWindow("PlayerInventory/ranged");
	wnd_armor = getWindow("PlayerInventory/armor");
	wnd_shield = getWindow("PlayerInventory/shield");

	wnd_else1 = getWindow("PlayerInventory/else1");
	wnd_else2 = getWindow("PlayerInventory/else2");
	wnd_else3 = getWindow("PlayerInventory/else3");
	wnd_else4 = getWindow("PlayerInventory/else4");

	wnd_bag = getWindow("PlayerInventory/bag");

	wnd_melee->setID(eqMelee);
	wnd_ranged->setID(eqRanged);
	wnd_armor->setID(eqArmor);
	wnd_shield->setID(eqShield);
	wnd_else1->setID(eqElse1);
	wnd_else2->setID(eqElse2);
	wnd_else3->setID(eqElse3);
	wnd_else4->setID(eqElse4);
	wnd_bag->setID(eqBag);

	//subscribing
	wnd_melee->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&EquipmentView::equipmentClick, this));
	wnd_ranged->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&EquipmentView::equipmentClick, this));
	wnd_armor->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&EquipmentView::equipmentClick, this));
	wnd_shield->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&EquipmentView::equipmentClick, this));
	wnd_else1->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&EquipmentView::equipmentClick, this));
	wnd_else2->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&EquipmentView::equipmentClick, this));
	wnd_else3->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&EquipmentView::equipmentClick, this));
	wnd_else4->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&EquipmentView::equipmentClick, this));
	wnd_bag->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&EquipmentView::equipmentClick, this));



	display_melee = static_cast<CEGUI::ProgressBar*>(getWindow("PlayerEquipment/display_melee"));
	display_ranged = static_cast<CEGUI::ProgressBar*>(getWindow("PlayerEquipment/display_ranged"));
	display_armor = static_cast<CEGUI::ProgressBar*>(getWindow("PlayerEquipment/display_armor"));

	display_items_left = getWindow("PlayerEquipment/itemsleft");	

	slotImage[eqMelee]	= ICON_MELEE;
	slotImage[eqArmor]	= ICON_ARMOR;
	slotImage[eqRanged] = ICON_RANGED;
	slotImage[eqShield] = ICON_SHIELD;
	slotImage[eqElse1]	= ICON_ELSE;
	slotImage[eqElse2]	= ICON_ELSE;
	slotImage[eqElse3]	= ICON_ELSE;
	slotImage[eqElse4]	= ICON_ELSE;
	slotImage[eqBag]	= ICON_BAG;


	
	
}



void EquipmentView::update()
{
	GameChar *player = app->getPlayer();

	//melee	
	gamedata_item *item = player->getMelee();
	if(item)
	{
		applyImageToWindow(wnd_melee,item->inv_image.c_str());
		wnd_melee->setTooltipText(item->name.c_str());
	}
	else
	{
		applyImageToWindow(wnd_melee,ICON_MELEE);
		wnd_melee->setTooltipText("");
	}	

	//ranged
	item = player->getRanged();
	if(item)
	{
		applyImageToWindow(wnd_ranged,item->inv_image.c_str());
		wnd_ranged->setTooltipText(item->name.c_str());
	}
	else
	{
		applyImageToWindow(wnd_ranged,ICON_RANGED);
		wnd_ranged->setTooltipText("");
	}	

	//armor
	item = player->getArmor();
	if(item)
	{
		applyImageToWindow(wnd_armor,item->inv_image.c_str());
		wnd_armor->setTooltipText(item->name.c_str());
	}
	else
	{
		applyImageToWindow(wnd_armor,ICON_ARMOR);
		wnd_armor->setTooltipText("");
	}
		
	//shield
	item = player->getShield();
	if(item)
	{
		applyImageToWindow(wnd_shield,item->inv_image.c_str());
		wnd_shield->setTooltipText(item->name.c_str());
	}
	else
	{
		applyImageToWindow(wnd_shield,ICON_SHIELD);
		wnd_shield->setTooltipText("");
	}
	//else1
	item = player->getElse1();
	if(item)
	{
		applyImageToWindow(wnd_else1,item->inv_image.c_str());
		wnd_else1->setTooltipText(item->name.c_str());
	}
	else
	{
		applyImageToWindow(wnd_else1,ICON_ELSE);
		wnd_else1->setTooltipText("");
	}	
	//else2
	item = player->getElse2();
	if(item)
	{
		applyImageToWindow(wnd_else2,item->inv_image.c_str());
		wnd_else2->setTooltipText(item->name.c_str());
	}
	else
	{
		applyImageToWindow(wnd_else2,ICON_ELSE);
		wnd_else2->setTooltipText("");
	}			
	//else3
	item = player->getElse3();
	if(item)
	{
		applyImageToWindow(wnd_else3,item->inv_image.c_str());
		wnd_else3->setTooltipText(item->name.c_str());
	}
	else
	{
		applyImageToWindow(wnd_else3,ICON_ELSE);
		wnd_else3->setTooltipText("");
	}	
	//else4
	item = player->getElse4();
	if(item)
	{
		applyImageToWindow(wnd_else4,item->inv_image.c_str());
		wnd_else4->setTooltipText(item->name.c_str());
	}
	else
	{
		applyImageToWindow(wnd_else4,ICON_ELSE);
		wnd_else4->setTooltipText("");
	}	
	//tasche
	item = player->getBag();
	if(item)
	{
		applyImageToWindow(wnd_bag,item->inv_image.c_str());
		wnd_bag->setTooltipText(item->name.c_str());
	}
	else
	{
		applyImageToWindow(wnd_bag,ICON_BAG);
		wnd_bag->setTooltipText("");
	}			
	updateStats();
}

void EquipmentView::updateStats()
{
	//getting player stats
	GameChar *player = app->getPlayer();
	Stats stats = player->getStats();
	//also, his weapons
	gamedata_item *melee = player->getMelee();
	gamedata_item *ranged = player->getRanged();
	//now calculating what we should display
	//armor
	Ogre::Real armor = 0;
	for(int i=st_resistBlunt;i<=st_resistPierce;i++)
	{
		//"convert" the float nr to 0.00 -> 1.00
		Ogre::Real percentage = (1-stats.mStats[i]);
		if(percentage > 1)
			percentage = 1;
		if(percentage > armor)
			armor = percentage;
	}
	
	//damage for melee
	//assume 100 is max
	Ogre::Real melee_val = 0; 
	if(melee)
		melee_val = dmgMax(melee->meleeDamage);
	Ogre::Real ranged_val = 0;
	if(ranged)
		ranged_val = dmgMax(ranged->rangedDamage);

	display_armor->setProgress(armor);
	display_melee->setProgress(melee_val/100);
	display_ranged->setProgress(ranged_val/100);

	//nun die items.
	
	updateInventoryDisplay();

}

void EquipmentView::updateInventoryDisplay()
{
	if(mInventoryWindow->mInv)
	{
		int invSize = mInventoryWindow->mInv->getMaxSize();
		int itemCount = mInventoryWindow->mInv->getTotalCount();

		display_items_left->setText(cegui_str(itemCount)+"/"+cegui_str(invSize));
	}
	else
	{
		display_items_left->setText("0/0");
	}
}

bool EquipmentView::equipmentClick(const CEGUI::EventArgs& e)
{
	if(app->curHolding->hasSpell())
		return true;

	
	GameChar *player = app->getPlayer();

	const CEGUI::MouseEventArgs& me = 
		static_cast<const CEGUI::MouseEventArgs&>(e);

	CEGUI::uint id = me.window->getID();
	
	
	bool shouldPlaySound = false;

	gamedata_item *slotItem = player->getEquippedItem(id);
	CEGUI::String emptySlot = slotImage[id];


	UniKey key(me.button,me.sysKeys);
	
	//if(app->key_is(
	//hier gibt es nur eine valide aktion
	//eines ablegen (wenn nichts drin, aber was in der hand)
	//eines nehmen (wenn etwas drin, aber nichts in der hand)
	//tauschen, wenn was drin, und genau eines in der hand

	//mInventoryWindow->showInventoryView();
	//mInventoryWindow->hideInventoryView();
	
	if(app->mFrameListener->key_is(key,keyInvTakeOne))
	{
		if(app->curHolding->hasItem() && slotItem == NULL)
		{
			//holding stuff, nothing inside
			gamedata_item *holdingItem = app->curHolding->getItem().item;
			Inventory *inv = NULL;
			//checking if this is a bag
			if(id == eqBag && app->curHolding->hasInventory())
			{
				inv = app->curHolding->getInventory();
			}
			//try to equip?
			if(player->equipItem(holdingItem,id,inv))
			{
				//worked
				CEGUI::String invimage = app->curHolding->getItem().item->inv_image.c_str();
				me.window->setTooltipText(app->curHolding->getItem().item->name);
				applyImageToWindow(me.window,invimage);
				shouldPlaySound = true;
				app->curHolding->modItemCount(-1);
				if(id == eqBag)
				{
					mInventoryWindow->showInventoryView();
				}
			}
			
			
		}
		else if(!app->curHolding->hasItem() && slotItem)
		{
			//holding nothing, one inside
			//if it's a bag, then we need the pointer to the inventory
			Inventory *inv = NULL;
			if(id == eqBag)
				inv = player->getInventory();
			//try to unequip
			if(player->equipItem(NULL,id))
			{
				//make us hold it
				app->curHolding->setHoldingItem(slotItem,1,inv);
				//remove it from the slot
				me.window->setTooltipText("");
				applyImageToWindow(me.window,slotImage[id]);
				shouldPlaySound = true;
				if(id == eqBag)
				{
					mInventoryWindow->hideInventoryView();
				}
			}

		}
		else if(app->curHolding->hasItem() && app->curHolding->getItemCount() == 1 && slotItem)
		{
			//holding exactly one and something inside
			//swap
			Inventory *slotInv = NULL;
			Inventory *holdInv = NULL;
			if(id == eqBag)
			{
				//inventories should be swapped, too
				holdInv = app->curHolding->getInventory();
				slotInv = player->getInventory();
			}
			gamedata_item *holdingItem = app->curHolding->getItem().item;
			if(player->equipItem(holdingItem,id,holdInv))
			{
				CEGUI::String invimage = app->curHolding->getItem().item->inv_image.c_str();
				me.window->setTooltipText(app->curHolding->getItem().item->name);
				applyImageToWindow(me.window,invimage);
				shouldPlaySound = true;

							
				app->curHolding->setHoldingItem(slotItem,1,slotInv);
			}
			
		}
	}
	




	
	if(shouldPlaySound)
	{
		
		app->playPlayerSFX(app->clickSnd);
	}
	updateStats();
	return true;
}

