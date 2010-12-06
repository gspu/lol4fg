#include "InventoryWindow.h"
#include <CEGUI.h>
#include <TypeConverter.h>
#include <GameApp.h>
#include <HoldingSlotWindow.h>
#include "Alchemy.h"


InventoryWindow::InventoryWindow()
{
	mInv = NULL;
	
	loadLayout("InventoryWindow.layout");
	
	CEGUI::Window *wnd = getWindow("InventoryWindow/content");
	slotView = new InventorySlotView(wnd,this);

	eqView = new EquipmentView(getWindow("InventoryWindow/equipment"),this);

	//add meself to root
	slotView->setIgnoreEvents(true);
	addToRoot();
	slotView->setIgnoreEvents(false);
	oldWndArea = CEGUI::URect(CEGUI::UDim(0,0),CEGUI::UDim(0,0),CEGUI::UDim(0,0),CEGUI::UDim(0,0));
	oldWndAreaRect = CEGUI::Rect(0,0,0,0);

	mWnd->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked, CEGUI::Event::Subscriber(&InventoryWindow::eventCloseClicked, this));
	mWnd->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&InventoryWindow::eventMouseUp, this));

	mWnd->subscribeEvent(CEGUI::Window::EventShown, CEGUI::Event::Subscriber(&InventoryWindow::eventShown, this));

	//TEST

	slotView->updateView();
	slotView->updateParentWindow();
}

InventoryWindow::~InventoryWindow()
{
	delete slotView;
	delete eqView;
	destroyLayout();
	//~GameWindow should be called after this and clean up
}

void InventoryWindow::inventoryChanged()
{
	eqView->updateInventoryDisplay();
}

void InventoryWindow::show()
{
	slotView->setIgnoreEvents(true);
	GameWindow::show();
	eqView->update();
	slotView->setIgnoreEvents(false);
}

bool InventoryWindow::eventCloseClicked(const CEGUI::EventArgs& e)
{
	GameApp::getSingletonPtr()->showInventory(false);
	return true;
}

void InventoryWindow::showInventoryView()
{
	mWnd->setProperty("SizingEnabled","True");
	slotView->show();
	resizeWindow(0,-inventoryDelta,true);
	slotView->updateParentWindow();
}

void InventoryWindow::hideInventoryView()
{
	mWnd->setProperty("SizingEnabled","False");
	//inventoryWndArea = mWnd->getArea();
	slotView->hide();
	//now resize it
	//CEGUI::URect newArea = eqView->getMainWindow()->getArea();

	CEGUI::Rect innerEqArea = eqView->getMainWindow()->getUnclippedOuterRect();

	CEGUI::Rect innerRect = mWnd->getUnclippedInnerRect();
	CEGUI::Rect outerRect = mWnd->getUnclippedOuterRect();

	float innerHeight = innerRect.d_bottom-innerRect.d_top;
	//float outerHeight = outerRect.d_bottom-outerRect.d_top;

	float eqHeight = innerEqArea.d_bottom-innerEqArea.d_top;

	inventoryDelta = innerHeight - eqHeight;

	resizeWindow(0,inventoryDelta,true);
	/*CEGUI::UDim height = inventoryWndArea.getHeight();
	
	inventoryWndArea.setHeight();*/
	mWnd->setProperty("SizingEnabled","False");

}
