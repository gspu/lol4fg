#include "inventory.h"
#include "defines.h"

Inventory::Inventory(size_t _maxSize)
{
	maxSize = _maxSize;
	itemCount = 0;
	//itemVector.size(maxSize);
	
}

Inventory::Inventory(Inventory *inv)
{
	maxSize = inv->maxSize;
	itemCount = inv->itemCount;
	for(std::vector<invDataItem>::const_iterator itr = inv->itemVector.begin();itr!=inv->itemVector.end();itr++)
	{
		invDataItem cur = *itr;
		itemVector.push_back(cur);

	}
	//itemVector.size(maxSize);
	
}


unsigned int Inventory::addItem(gamedata_item *it,unsigned int count)
{
    if(!it)
        return 0;
	unsigned int items_left = count;
	if(itemCount >= maxSize)
		return 0;
	if(itemCount+items_left > maxSize)
	{
		items_left = maxSize-itemCount;
	}
	std::vector<invDataItem>::iterator itr;
	//items_left ist hier so viel wie wirklich geadded wird
	unsigned int result = items_left;


	//erstmal items mit gleicher ID suchen, und da adden
	for(itr = itemVector.begin();itr != itemVector.end();itr++)
	{
		invDataItem cur = *itr;
		if(cur.item == it)
		{
			//item mit gleicher ID gefunden
			if(cur.count < SLOT_MAX)
			{
				//es geht noch was. aber wie viel?
				if(cur.count+items_left > SLOT_MAX)
				{
					//nicht genug...
					items_left -=  SLOT_MAX-cur.count;
					itemCount += SLOT_MAX-cur.count;
					cur.count = SLOT_MAX;

				}
				else
				{
					//reicht
					cur.count += items_left;
					itemCount += items_left;
					items_left = 0;
					*itr = cur;
					return result;
				}
			}
			*itr = cur;
		}
		
		if(items_left == 0)
			return result;

	}
	//jetzt nach leeren suchen und da adden
	//gamedata_item* curItemPtr=StandardApplication::getSingletonPtr()->getItemDataPtr(ID);
	for(itr = itemVector.begin();itr != itemVector.end();itr++)
	{
		invDataItem cur = *itr;
		if(cur.count == 0)
		{
			//wenn 0 anzahl, dann ist es leer
			if(items_left > SLOT_MAX)
			{
				//passt nicht in ein feld
				cur.item = it;
				//cur.itemID = ID;
				cur.count = SLOT_MAX;
				itemCount += SLOT_MAX;
				items_left -= SLOT_MAX;
			}
			else
			{
				//passt in ein feld
				cur.count = items_left;
				itemCount += items_left;
				//cur.itemID = ID;
				cur.item = it;
				items_left = 0;
				*itr = cur;
				return result;
				
			}
			*itr = cur;
		}
		
		if(items_left == 0)
			return result;
	}
	//wenn wir hier noch leben, dann brauchen wir neue felder...
	while(items_left != 0)
	{
		invDataItem cur;
		//cur.itemID = ID;
		cur.item = it;
		if(items_left > SLOT_MAX)
		{
			//immer noch zu groß...
			cur.count = SLOT_MAX;
			itemCount += SLOT_MAX;
			items_left -= SLOT_MAX;

		}
		else
		{
			cur.count = items_left;
			itemCount += items_left;
			items_left = 0;
		}
		
		itemVector.push_back(cur);
	}
	assert(items_left == 0);
	return result;
	

}


unsigned int Inventory::addItem(Ogre::String ID,unsigned int count)
{	
	gamedata_item* curItemPtr=StandardApplication::getSingletonPtr()->getItemDataPtr(ID);
	return addItem(curItemPtr,count);
}

int Inventory::setItemAt(Inventory::invDataItem itm, size_t pos)
{
	if(itemCount >= maxSize)
		return 0;
	invDataItem set = itm;
	if(itm.count > SLOT_MAX)
		itm.count = SLOT_MAX;
	
	invDataItem cur;

	if(pos >= itemVector.size())
	{
		invDataItem empty;
		/*empty.count = 0;
		empty.itemID= "";*/
		for(size_t i = itemVector.size();i<=pos;i++)
		{
			itemVector.push_back(empty);
		}
		cur = empty;
	}
	else
	{
		cur = itemVector[pos];
	}
	
	itemVector[pos] = itm;
	return itm.count - cur.count;
}


unsigned int Inventory::removeItem(gamedata_item *it,unsigned int count)
{
    if(!it)
        return 0;
	unsigned int items_left = count;
	std::vector<invDataItem>::iterator itr;
	for(itr = itemVector.begin();itr != itemVector.end();itr++)
	{
		invDataItem cur = *itr;
		if(cur.item == it)
		{
			if(items_left >= cur.count)
			{
				//ich muss mehr entfernen als in dem feld ist
				items_left -= cur.count;
				itemCount -= cur.count;
				cur.count = 0;			
				cur.item = NULL;
			}
			else
			{
				//so viel entfernen wie items_left
				cur.count -= items_left;
				itemCount -= items_left;
				items_left = 0;
			}
			*itr = cur;
			if(items_left == 0)
				return count;
		}
	}
	return count-items_left;
}


unsigned int Inventory::removeAllItems(gamedata_item *it)
{
	if(!it)
        return 0;
	unsigned int removed = 0;
	std::vector<invDataItem>::iterator itr;
	for(itr = itemVector.begin();itr != itemVector.end();itr++)
	{
		invDataItem cur = *itr;
		if(cur.item == it)
		{
			//remove everything
			removed += cur.count;
			
			
			cur.count = 0;			
			cur.item = NULL;
			
			*itr = cur;
		}
	}
	itemCount -= removed;
	return removed;
}

unsigned int Inventory::removeAllItems(Ogre::String ID)
{
	gamedata_item *it = StandardApplication::getSingletonPtr()->getItemDataPtr(ID);
    return removeAllItems(it);	
}

void Inventory::trim()
{
	ItemVector::iterator firstEmpty = itemVector.end();
	for(ItemVector::iterator itr = itemVector.end()--;itr>=itemVector.begin();itr--)
	{
		invDataItem cur = *itr;
		if(cur.count == 0)
			firstEmpty = itr;
		else
			break;
	}
	itemVector.erase(firstEmpty,itemVector.end());
}


//hier Kopie der anderen Funktion, weil das schneller gehen dürfte, als den Pointer von StandardApp zu holen
unsigned int Inventory::removeItem(Ogre::String ID,unsigned int count)
{
    if(ID == "")
        return 0;
	unsigned int items_left = count;
	std::vector<invDataItem>::iterator itr;
	for(itr = itemVector.begin();itr != itemVector.end();itr++)
	{
		invDataItem cur = *itr;
		if(cur.item->id == ID)
		{
			if(items_left >= cur.count)
			{
				//ich muss mehr entfernen als in dem feld ist
				items_left -= cur.count;
				itemCount -= cur.count;
				cur.count = 0;			
				cur.item = NULL;
			}
			else
			{
				//so viel entfernen wie items_left
				cur.count -= items_left;
				itemCount -= items_left;
				items_left = 0;
			}
			*itr = cur;
			if(items_left == 0)
				return count;
		}
	}
	return count-items_left;
}

Inventory::invDataItem Inventory::getItemAt(size_t pos)
{
	invDataItem res;
	if(pos >= itemVector.size())
	{
		
		res.count = 0;
		res.item = NULL;
		return res;
	}
	
	return itemVector[pos];
}
unsigned int Inventory::addItemAt(size_t pos,gamedata_item *item,unsigned int count)
{
	if(itemCount > maxSize || !item)
	{
		return 0;
	}
	if(itemCount+count > maxSize)
	{
		count = maxSize-itemCount;		
	}
	if(count == 0)
			return 0;
	if(pos >= itemVector.size())
	{
		invDataItem empty;
		/*empty.count = 0;
		empty.itemID= "";*/
		for(size_t i = itemVector.size();i<=pos;i++)
		{
			itemVector.push_back(empty);
		}
	}
	

	invDataItem cur = itemVector[pos];
	if(cur.item != NULL && cur.count != 0 && cur.item != item)
		return 0;

	cur.item = item;
	unsigned int result = count;
	if(cur.count+count > SLOT_MAX)
	{
		count = SLOT_MAX-cur.count;
		cur.count = SLOT_MAX;
		result = count;
		itemCount += count;
	}
	else
	{
		cur.count += count;
		itemCount += count;
	}
	itemVector[pos] = cur;
	return result;

	

}

unsigned int Inventory::addItemAt(size_t pos,Ogre::String ID,unsigned int count)
{
	gamedata_item *it = StandardApplication::getSingletonPtr()->getItemDataPtr(ID);
    return addItemAt(pos,it,count);	
}

unsigned int Inventory::removeItemAt(size_t pos,unsigned int count)
{
	if(pos >= itemVector.size())
		return 0;
	invDataItem cur = itemVector[pos];
	unsigned int result = count;
	if(cur.count-count < 0)
	{
		itemCount -= cur.count;
		result = cur.count;
		cur.count = 0;
		cur.item = NULL;
	}
	else
	{
		itemCount -= count;
		cur.count -= count;
	}
	itemVector[pos] = cur;
	return result;
}
unsigned int Inventory::getCount(Ogre::String ID)
{
	unsigned int cnt = 0;
	std::vector<invDataItem>::const_iterator itr;
	for(itr = itemVector.begin();itr != itemVector.end();itr++)
	{
		invDataItem cur = *itr;
		if(cur.item && cur.item->id == ID)
		{
			cnt += cur.count;
		}
	}
	return cnt;

}

unsigned int Inventory::getCount(gamedata_item *it)
{
	unsigned int cnt = 0;
    if(!it)
        return 0;
	std::vector<invDataItem>::const_iterator itr;
	for(itr = itemVector.begin();itr != itemVector.end();itr++)
	{
		invDataItem cur = *itr;
		if(cur.item == it)
		{
			cnt += cur.count;
		}
	}
	return cnt;

}

void Inventory::resyncItemCount()
{
	itemCount = 0;
	std::vector<invDataItem>::const_iterator itr;
	for(itr = itemVector.begin();itr != itemVector.end();itr++)
	{
		invDataItem cur = *itr;
		itemCount += cur.count;
	}
}