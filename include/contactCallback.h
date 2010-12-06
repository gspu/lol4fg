#ifndef __praContCallback
#define __praContCallback

#include <Ogre.h>
/*
#include <OgreFrameListener.h>
#include <OgreEventListeners.h>
#include <OgreKeyEvent.h>


#include <OgreTextAreaOverlayElement.h>
#include <OgreFontManager.h>
#include <vector>
#include <OgreNewt.h>
#include "tinyxml.h"
*/



class ContCallback: public OgreNewt::ContactCallback
{
public:
	ContCallback();
	~ContCallback();

	int onAABBOverlap(OgreNewt::Body* body0, OgreNewt::Body* body1, int threadIndex);

	void contactsProcess(OgreNewt::ContactJoint &contactJoint, Ogre::Real timeStep, int threadIndex);

	OgreNewt::ContactJoint *getContactJoint()
	{
		return mContactJoint;
	}

private:
	//updates obj_0, obj_1, water and notwater from 2 bodies
	void updateObjectPointers(OgreNewt::Body *body0, OgreNewt::Body *body1);

	OgreNewt::ContactJoint *mContactJoint;

	GameObject *obj_0;
	GameObject *obj_1;
    
    GameObject *water;
    GameObject *notwater;
//	objCont *obj0, *obj1;
//	int type0, type1;
	void *ptr0, *ptr1;
};

#endif