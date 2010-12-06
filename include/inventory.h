#ifndef __praInventory
#define __praInventory
#include "Ogre.h"
#include "StandardApplication.h"



class Inventory
{
public:
	/*struct invItem
	{
		Ogre::String itemID;
		BYTE count;
	};*/
	struct invDataItem
	{
		invDataItem():
			item(NULL),
			count(0),
			flags(0)
			{}
		gamedata_item* item;
		unsigned int count;
		unsigned int flags;
	};
	
	Inventory(size_t maxSize);
	~Inventory(){/*default*/}
	//copy constructor
	Inventory(Inventory *inv);
	/*
	Adds specific items to the inventory. Will not add items beyond the total item limit.
	param it: the item to be added
	param count: how much of it to add
	returns: the number of items that was actually added	
	*/
    unsigned int addItem(gamedata_item *it,unsigned int count = 1);
	unsigned int addItem(Ogre::String ID,unsigned int count = 1);
	
	/*
	Removes specific items
	param it: the item to be removed
	param count: how much of it to remove
	returns: the number of items that were actually removed
	*/
	unsigned int removeItem(gamedata_item *it,unsigned int count = 1);
	unsigned int removeItem(Ogre::String ID,unsigned int count = 1);

	/*
	removes the given item completely from the inventory
	returns how often the item existed
	*/
	unsigned int removeAllItems(gamedata_item *it);
	unsigned int removeAllItems(Ogre::String ID);

	/*
	Adds specific items to a specific place in the inventory. Will not add items beyond the limit per slot.
	param pos: the position of the slot
	param it: the item to be added
	param count: how much of it to add
	returns: the number of items that was actually added	
	*/
	//returns the number of items that was actually added
	unsigned int addItemAt(size_t pos,Ogre::String ID,unsigned int count = 1); 
	unsigned int addItemAt(size_t pos,gamedata_item *it,unsigned int count = 1); 
	//returns the number of items that were actually removed
	unsigned int removeItemAt(size_t pos,unsigned int count = 1);
	//returns count of all items with specified ID
	unsigned int getCount(Ogre::String ID);
    unsigned int getCount(gamedata_item *it);
	//returns the number of items in specified slot
	invDataItem getItemAt(size_t pos);
	//returns number of all items in this inventory
	inline size_t getTotalCount()
	{
		//resyncItemCount();
		return itemCount;
	}
	
	//sets a slot to a specific value. returns how much items were added (can be negative if items were removed)
	int setItemAt(invDataItem itm, size_t pos);
	//makes the inventory re-count it's items
	void resyncItemCount();
	//returns the maximal nr. of items this inventory can hold
	inline size_t getMaxSize()
	{
		return maxSize;
	}
	//checks if the inventory has space for given amount of items
	bool hasSpace(size_t space = 1)
	{
		return (itemCount+space <= maxSize );		
	}
	//deletes all items from the inventory
	void clear()
	{
		itemVector.clear();
		itemCount = 0;
	}
	//changes the maximal size of this inventory
	inline void setMaxSize(size_t size)
	{
		maxSize = size;
	}

	//removes unnecessary empty elements at the end of the inventory and makes it
	//re-allocate the used space to fit it's size
	void trim();

	//std::vector<invItem> itemVector;
	typedef std::vector<invDataItem> ItemVector;
	ItemVector itemVector;
//	std::vector<gamedata_item> itemDataVector;
private:
	size_t maxSize;
	size_t itemCount;


};
#endif