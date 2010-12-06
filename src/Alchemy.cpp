#include "Alchemy.h"
#include <functions.h>
//singleton
template<> Alchemy* Ogre::Singleton<Alchemy>::ms_Singleton = 0;

bool Alchemy::miscItems(Inventory::invDataItem &educt1, Inventory::invDataItem &educt2, Inventory::invDataItem &product, bool miscAll)
{
	if(educt1.count == 0 || educt2.count == 0)
		return false;
	EductPair pair = makePair(educt1.item,educt2.item);
	ReactionMap::iterator itr = reactionMap.find(pair);
	if(itr == reactionMap.end())
		return false;
	//here we can assume that we do have a reaction
	gamedata_item *result = itr->second;
	if(miscAll)
	{
		//check what is less
		unsigned int resultCount = educt1.count;
		if(educt2.count < educt1.count)
			resultCount = educt2.count;
		product.count = resultCount;
		product.item = result;
		educt1.count -= resultCount;
		if(educt1.count == 0)
			educt1.item = NULL;
		educt2.count -= resultCount;
		if(educt2.count == 0)
			educt2.item = NULL;
	}
	else
	{
		product.count = 1;
		product.item = result;
		educt1.count--;
		educt2.count--;		
	}
	return true;
}

Alchemy::EductPair Alchemy::makePair(gamedata_item *item1,gamedata_item *item2)
{
	//sort them. the smaller pointer comes first
	if(item1 < item2)
		return EductPair(item1,item2);
	else
		return EductPair(item2,item1);
}

bool Alchemy::isMixable(gamedata_item *educt1,gamedata_item *educt2)
{
	if(educt1 == educt2)
		return false;//trivial case

	
	EductPair pair = makePair(educt1,educt2);
	
	//now check if we have this
	return reactionMap.find(pair) != reactionMap.end();
	//return false;
}

void Alchemy::addReaction(gamedata_item *item1,gamedata_item *item2,gamedata_item *result)
{
	if(!item1 || !item2 || !result || (item1 == item2))	
	{
		return;
	}
	EductPair pair = makePair(item1,item2);

	reactionMap[pair] = result;//will overwrite
}

void Alchemy::loadFromXml(TiXmlElement *elem)
{
	//awaits the <alchemy> element
	using namespace Ogre;
	StandardApplication *app = StandardApplication::getSingletonPtr();
	TiXmlElement *mix = elem->FirstChildElement("mix");
	while(mix)
    {
		String item1_id = mix->Attribute("item1");
		String item2_id = mix->Attribute("item2");
		String result_id = mix->Attribute("result");
		//retrieving the pointers
		gamedata_item *item1 = app->getItemDataPtr(item1_id);
		gamedata_item *item2 = app->getItemDataPtr(item2_id);
		gamedata_item *result = app->getItemDataPtr(result_id);
		if(item1 && item2 && result)
		{
			addReaction(item1,item2,result);
		}
		else
		{
			mLog("Error while processing alchemy: mixture \""+item1_id+"\" + \""+item2_id+"\" = \""+result_id+"\" is not valid");
		}

		mix = mix->NextSiblingElement("mix");
	}
}

//OPERATORS
bool operator< (Alchemy::EductPair &first, Alchemy::EductPair &second)
{
	if(first.first == second.first)
		return first.second < second.second;
	return first.first < second.first;
}

bool operator> (Alchemy::EductPair &first, Alchemy::EductPair &second)
{
	if(first.first == second.first)
		return first.second > second.second;
	return first.first > second.first;
}

bool operator== (Alchemy::EductPair &first, Alchemy::EductPair &second)
{
	return (first.first == second.first) && (first.second == second.second);
}
