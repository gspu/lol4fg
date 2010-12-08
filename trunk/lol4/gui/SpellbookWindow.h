#ifndef __spellbookWnd
#define __spellbookWnd
#include "Spellbook.h"
#include "SlotView.h"
#include "GameWindow.h"

#include "FwDec.h"

class SpellbookWindow;


//Class for handling an spellbook view
class SpellbookSlotView: public SlotView
{
public:
	SpellbookSlotView(CEGUI::Window *target,SpellbookWindow *parent,Spellbook *sb = NULL)
		:SlotView(target),mSb(sb),mSpellbookWindow(parent)
	{
	}
	inline void setSpellbook(Spellbook *sb)
	{
		mSb = sb;
	}
	inline Spellbook *getSpellbook()
	{
		return mSb;
	}
	void updateParentWindow();
	
private:
	bool getItemData(int at, CEGUI::String &image, CEGUI::String &name, unsigned int &count);
	inline bool isDataSet()
	{
		return (mSb != NULL);
	}
	unsigned int getItemCount();
	//void snapWindowSize_parent(Ogre::Real delta_x,Ogre::Real delta_y);


	Spellbook *mSb;


	SpellbookWindow *mSpellbookWindow;

	
protected:
	void slotClicked(UniKey key,int index, int x, int y);
};


class SpellbookWindow: public GameWindow
{
public:
	SpellbookWindow();

	~SpellbookWindow();

	void show();
	inline void show(Spellbook *sb)
	{
		slotView->setSpellbook(sb);
		show();
	}

	void setSpellbook(Spellbook *sb)
	{
		mSb = sb;
		slotView->setSpellbook(sb);
	}

	//to be called from InventorySlotView when something has been taken out or added
	//for updating the items left display
	void spellbookChanged();

	//to be called from InventorySlotView when it wants to adapt the window size
	void resizeWindow(Ogre::Real delta_x,Ogre::Real delta_y,bool force = false);

	//setting absolute sizes for the window
	void setWindowSize(Ogre::Real width,Ogre::Real height);

	inline Spellbook *getSpellbook()
	{
		return mSb;
	}

	void hideInventoryView();

	void removeSelectionBorder();

	void setSelectionBorder(size_t index);

	void setSelectionBorder(CEGUI::Window *targetSlotWindow);

	Spellbook *mSb;
private:
	SpellbookSlotView *slotView;

	CEGUI::Window *selBorder;//for displaying the "spell is selected" border

	CEGUI::URect oldWndArea;//for checking if resizing is necessary
	CEGUI::Rect oldWndAreaRect;//for checking if resizing is necessary
	//CEGUI::URect inventoryWndArea;//backupping the area when the inventory window is open
	Ogre::Real inventoryDelta;//backupping the delta height value

	

	

	bool eventMouseUp(const CEGUI::EventArgs& e);
	bool eventCloseClicked(const CEGUI::EventArgs& e);
	
	//a little workaround...
	bool eventShown(const CEGUI::EventArgs& e);
};


#endif