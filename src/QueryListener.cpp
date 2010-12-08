#include "QueryListener.h"
#include "Level.h"

GroupingQueryListener::GroupingQueryListener(Level *lvl,GroupObject *addTo)
{
	group = addTo;//new GroupObject(
	mLvl = lvl;
}



bool GroupingQueryListener::queryResult(Ogre::MovableObject* object)
{
	//schauen ob es ein any hat
	if(object->getUserAny().isEmpty())
		return true;
	GameObject *cur = Ogre::any_cast<GameObject*>(object->getUserAny());
	if(cur)
	{
		if(!group)
		{
			group = new GroupObject(mLvl,cur);
		}
		else
		{
			group->addObject(cur);
		}
	}
	return true;
}

bool GroupingQueryListener::queryResult(Ogre::SceneQuery::WorldFragment* fragment)
{
	return true;
}