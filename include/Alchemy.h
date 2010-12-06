#ifndef __alchemy
#define __alchemy

#include <OgreSingleton.h>
#include "gamedata.h"
#include "Inventory.h"
#include <Tinyxml.h>

class Alchemy: public Ogre::Singleton<Alchemy>
{
public:
	typedef std::pair<gamedata_item*,gamedata_item*> EductPair;
	typedef std::map<EductPair,gamedata_item*> ReactionMap;

	void loadFromXml(TiXmlElement *elem);
	TiXmlElement* getXmlElem();
	/*
	mixes educt1 with educt2 to create product.
	params educt1, educt2: the educts for the reaction. quantities are reduced automatically
	param product: the product(s) of the reaction. an empty invDataItem should be passed here.
	param miscAll: if true, as much products are created as possible, otherwise only one
	returns whenever the reaction was successful
	*/
	bool miscItems(Inventory::invDataItem &educt1, Inventory::invDataItem &educt2, Inventory::invDataItem &product, bool miscAll = false);

	//checks if a reaction exists for 2 given items
	bool isMixable(gamedata_item *educt1,gamedata_item *educt2);
private:
	ReactionMap reactionMap;

	void addReaction(gamedata_item *item1,gamedata_item *item2,gamedata_item *result);

	//helper function to make a correct pair
	EductPair makePair(gamedata_item *item1,gamedata_item *item2);
};

bool operator< (Alchemy::EductPair &first, Alchemy::EductPair &second);
bool operator> (Alchemy::EductPair &first, Alchemy::EductPair &second);
bool operator== (Alchemy::EductPair &first, Alchemy::EductPair &second);

#endif