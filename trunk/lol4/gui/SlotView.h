#ifndef __slotView
#define __slotView
#include "GameWindow.h"
#include <CEGUI.h>
#include <fwdec.h>
#include <UniKey.h>


/* 
	Class for creating a slot view, like for a inventory.
	Requires any cegui window, adds there a ContentPane and 
	a ScrollBar as children
*/



class SlotView: public GameWindow
{
public:
	//class for making slots
	class Slot: public GameWindow
	{
	public:
		Slot(SlotView *parent,unsigned int x, unsigned int y,unsigned int id);
		~Slot()
		{
			destroyLayout();
		}
		void setPosition(unsigned int x, unsigned int y);
		void setID(unsigned int id)
		{
			mWnd->setID(id);
		}
		void setImage(CEGUI::String filename);
		void setCount(int count,bool showOne = false);
		void clear();
		void setToolTip(CEGUI::String tooltip);
		
	private:
		
		
		//CEGUI::Window *imageContainer;
		CEGUI::Window *countField;
	protected:
		void initGui();
		Slot(){};//a private constructor for classes inheriting from this
	};

	typedef std::vector<Slot*> SlotVector;

	

	SlotView(CEGUI::Window *target);

	~SlotView();

	void setSlotViewData();
	//updates everything. calls the important functions
	inline void updateView()
	{		
		updateSlots();
		updateScrollbar();
		updateVisibleArea();
	}
	 //fills the container with slots
    void updateSlots();
    //updates the scrollbar, dependant on the nr of total slots possible
    void updateScrollbar();
    //index of the line, which should be on the top
    unsigned int getScrollIndex();
    //updates one single slot
    //x,y: coordinates (relative)
    //filename: the image
	//tooltip: a string to show onmouseover
    //cnt: nr of items. is shown in the bottom right corner
	//highlight: whenever the slot should be highlighted (like for spells)
    void updateSlot(unsigned int x, unsigned int y,CEGUI::String filename,CEGUI::String tooltip,unsigned int cnt=1);

	void updateSlot(unsigned int index,CEGUI::String filename,CEGUI::String tooltip,unsigned int cnt=1);
    //updates visible area
    void updateVisibleArea();
	
    //retrieves the deltas for width and height to adjust the parent window's size
    void getWindowSizeDelta(Ogre::Real &delta_width, Ogre::Real &delta_height);
	
	inline CEGUI::Window *getContentPane()
	{
		return mContentPane;
	}

	Slot *getSlot(size_t index)
	{
		return mSlots[index];
	}
	Slot *getSlot(size_t x, size_t y)
	{
		return getSlot(y*numfieldsY + x);
	}

	
private:
	void initSlotGui();
	//updates the vector for a new size and sets numfieldsX/Y
	//also, creates slots if necessary.
	void setSize(unsigned int x,unsigned int y);
	CEGUI::Window *mContentPane;
	CEGUI::Scrollbar *mScrollBar;

	//here should be all slots
	SlotVector mSlots;

    //number of fields. must be set by whatever uses this class
    unsigned int numfieldsX;
    unsigned int numfieldsY;


   
    //das ist, um die Fenstergröße mit den Feldern zu synchronisieren
    CEGUI::URect oldWindowArea;


	//updates the parent. to be implemented
	virtual void updateParentWindow() = 0;
	/*
	gets displayed item data. to be implemented by something what inherites this
	param at: position of the item in the list
	param image: variable to hold the retrieved image
	param name: variable to hold the retrieved name (for tooltip)
	param count: variable to hold the number of items
	returns: whenever the specified item exists
	*/
	virtual bool getItemData(int at, CEGUI::String &image, CEGUI::String &name, unsigned int &count) = 0;

	/*
	checks if the stuff where item data is retrieved is alerady set
	*/
	virtual bool isDataSet() = 0;
	/*gets the total number of items. to be implemented*/
	virtual unsigned int getItemCount() = 0;
	///*the real snapWindowSize*/
	//virtual void snapWindowSize_parent(Ogre::Real delta_x,Ogre::Real delta_y) = 0;
protected:
    //wenn es resized wird
    bool eventResize(const CEGUI::EventArgs& e);
    //wenn es verschoben wird
    bool eventMove(const CEGUI::EventArgs& e);
    ////wenn man auf schließen klickt
    //bool eventWindowClose(const CEGUI::EventArgs& e);
    //wenn man die scrollbar verschiebt
    bool eventScroll(const CEGUI::EventArgs& e);
    //wenn man im hauptfenster scrollt
    bool eventScrollWnd(const CEGUI::EventArgs& e);
    //wenn man ein slot anklickt
    bool eventSlotClick(const CEGUI::EventArgs& e);

	//actual events, should be implemented by something

	
	/*when a slot is clicked. 
	param key: a UniKey made from the cegui event args, usually a mouse button and maybe modifiers
	param index: the number of the slot, corresponds to the number of the entry. can also be calculated from x and y...
	params x, y: the absolute coordinates of the slot (means it is not affected by scrolling)
	
	*/
	virtual void slotClicked(UniKey key,int index,int x, int y) = 0;

	static const CEGUI::String slotNamePrefix;
};



#endif