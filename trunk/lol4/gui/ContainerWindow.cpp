#include "ContainerWindow.h"
#include <GameApp.h>

/*******************************ContainerWindow*********************************/
ContainerWindow::ContainerWindow():
	mOwner(NULL)
	
{
	
	loadLayout("ContainerWindow.layout");
	
	CEGUI::Window *wnd = getWindow("ContainerWindow/content");
	slotView = new InventorySlotView(wnd,this);

	//eqView = new EquipmentView(getWindow("ContainerWindow/equipment"),this);

	//add meself to root
	slotView->setIgnoreEvents(true);
	addToRoot();
	slotView->setIgnoreEvents(false);
	oldWndArea = CEGUI::URect(CEGUI::UDim(0,0),CEGUI::UDim(0,0),CEGUI::UDim(0,0),CEGUI::UDim(0,0));
	oldWndAreaRect = CEGUI::Rect(0,0,0,0);
	//inventoryWndArea = mWnd->getArea();

	mWnd->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked, CEGUI::Event::Subscriber(&ContainerWindow::eventCloseClicked, this));
	mWnd->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&ContainerWindow::eventMouseUp, this));

	mWnd->subscribeEvent(CEGUI::Window::EventShown, CEGUI::Event::Subscriber(&ContainerWindow::eventShown, this));

	//TEST

	slotView->updateView();
	slotView->updateParentWindow();
}

ContainerWindow::~ContainerWindow()
{
	delete slotView;
	
	destroyLayout();
	//~GameWindow should be called after this and clean up
}

bool ContainerWindow::eventCloseClicked(const CEGUI::EventArgs& e)
{
	GameApp::getSingletonPtr()->hideOtherInventory();
	return true;
}


void ContainerWindow::inventoryChanged()
{
	
}

void ContainerWindow::show()
{
	slotView->setIgnoreEvents(true);
	GameWindow::show();
	slotView->setIgnoreEvents(false);
}