#include "gamedata.h"
#include "functions.h"
#include "gameChar.h"
#include "Inventory.h"
#include "Spellbook.h"


bool gamedata_item::onHotkey(GameChar *user)
{
    if(equipFlags != EF_NULL)
    {
        return user->equipItem(this);
    }
    else if(propertyFlags & PF_USABLE)
    {
		return this->onUse(user);
        //return user->useItem(this);
    }
    //equipFlags
    //propertyFlags

    //mDebugText(name);
    return false;
}


bool gamedata_item::onUse(GameChar *user)
{
    if(useSpell)
    {
		user->AddSpell(useSpell,user);
        if(propertyFlags & PF_LOSE_ON_USE)
			user->getInventory()->removeItem(this);
        return true;
    }
    return false;
}

bool gamedata_spell::onHotkey(GameChar *user)
{
    //mDebugText(name);
    if(!user->setSpell(this))
        return false;
    user->castSpell();
    return true;
}

void gamedata_char::prepareFinalRemoval()
{
	//inventory and spellbook must be deleted here
	if(defaultInventory)
		delete defaultInventory;
	if(defaultSpellbook)
		delete defaultSpellbook;
}

void gamedata_damagedealer::prepareFinalRemoval()
{
	delete Missile;
	delete Explosion;
	delete Beam;
}