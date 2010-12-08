#ifndef __praBoxQueryListener
#define __praBoxQueryListener

#include "Ogre.h"
#include "GroupObject.h"


class GroupingQueryListener: public Ogre::SceneQueryListener
{
public:
	//of no addTo is passed, it will create one itself
	GroupingQueryListener(Level *lvl,GroupObject *addTo = NULL);
	/** Called when a MovableObject is returned by a query.
        @remarks
            The implementor should return 'true' to continue returning objects,
            or 'false' to abandon any further results from this query.
        */
    bool queryResult(Ogre::MovableObject* object);
    /** Called when a WorldFragment is returned by a query.
    @remarks
        The implementor should return 'true' to continue returning objects,
        or 'false' to abandon any further results from this query.
    */
    bool queryResult(Ogre::SceneQuery::WorldFragment* fragment);
	GroupObject *group;
private:
	Level *mLvl;
};


#endif