#include <HoldingSlotWindow.h>

HoldingSlotWindow::HoldingSlotWindow():
	SlotView::Slot(),
	isSelected(false),		
	spell(NULL),
	inventory(NULL)
{
	initGui();

	//adding more
	selBorder = createWindow("StaticImage","selBorder");
	mWnd->addChildWindow(selBorder);
	applyImageToWindow(selBorder,"redborder.png");
	selBorder->setProperty("FrameEnabled","False");
    selBorder->setProperty("BackgroundEnabled","False");
	selBorder->setProperty("MousePassThroughEnabled","True");
	selBorder->hide();

	mWnd->setProperty("FrameEnabled","False");
    mWnd->setProperty("BackgroundEnabled","False");
	
	mWnd->setAlwaysOnTop(true);

	addToRoot();
	hide();
	item.item = NULL;
	item.count = 0;
}

HoldingSlotWindow::~HoldingSlotWindow()
{
	destroyLayout();
}

void HoldingSlotWindow::updatePosition()
{
	CEGUI::Point mp = CEGUI::MouseCursor::getSingletonPtr()->getPosition();
	mWnd->setPosition(CEGUI::UVector2(CEGUI::UDim(0,mp.d_x+8),CEGUI::UDim(0,mp.d_y+8) ));
}

void HoldingSlotWindow::setHoldingItem(gamedata_item *it, unsigned int count, Inventory *inv)
{
	item.item = it;
	item.count = count;
	inventory = inv;
	spell = NULL;
	setSpellBorder(false);

	if(it && count > 0)
	{
		setImage(it->inv_image);
		setCount(count);
		show();
	}
	else
	{
		setImage("");
		setCount(1);
		hide();
	}
}

void HoldingSlotWindow::setSpellBorder(bool set)
{
	if(set)
		selBorder->show();
	else
		selBorder->hide();
}

void HoldingSlotWindow::setHoldingSpell(gamedata_spell *spell, bool isSelected)
{
	this->isSelected = isSelected;
	item.item = NULL;
	item.count = 0;
	inventory = NULL;
	this->spell = spell;
	if(spell)
	{
		setImage(spell->inv_image);
		setCount(1);
		show();
		setSpellBorder(isSelected);
	}
	else
	{
		setImage("");
		setCount(1);
		setSpellBorder(false);
		hide();
	}
}
void HoldingSlotWindow::show()
{
	updatePosition();
	GameWindow::show();
	mWnd->activate();
}

void HoldingSlotWindow::clear()
{
	item.item = NULL;
	item.count = 0;
	inventory = NULL;
	this->spell = NULL;
	setImage("");
	setCount(1);
	setSpellBorder(false);
	hide();
}