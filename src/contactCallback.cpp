#include "Ogre.h"
#include "OgreNewt.h"
#include <OgreTextAreaOverlayElement.h>
#include <OgreFontManager.h>
//class GameChar;
//#include "GameChar.h"

////#include <OgreNoMemoryMacros.h>
//#include <CEGUIImageset.h>
//#include <CEGUISystem.h>
//#include <CEGUILogger.h>
//#include <CEGUISchemeManager.h>
//#include <CEGUIWindowManager.h>
//#include <CEGUIWindow.h>
//#include <RendererModules/Ogre/CEGUIOgreRenderer.h>
//#include <RendererModules/Ogre/CEGUIOgreResourceProvider.h>
//#include <elements/CEGUIPushButton.h>
////regular mem handler
//#include <OgreMemoryMacros.h> 
#include "functions.h"
#include "FwDec.h"
#include "contactCallback.h"
#include "GameChar.h"
#include "MissileObject.h"
#include "Level.h"
#include "WaterPlane.h"
//#include "global_class.h"

int ContCallback::onAABBOverlap(OgreNewt::Body* body0, OgreNewt::Body* body1, int threadIndex)
{
	if(body0->getType() == otMissile && body1->getType() == otMissile)
		return false;
	updateObjectPointers(body0,body1);	
	return true;
	
}

void ContCallback::contactsProcess(OgreNewt::ContactJoint &contactJoint, Ogre::Real timeStep, int threadIndex)
{
	mContactJoint = &contactJoint;
	OgreNewt::Contact firstContact = mContactJoint->getFirstContact();
	/*while(debugTest)
	{
		OgreNewt::Body *test1 = debugTest.getBody0();
		OgreNewt::Body *test2 = debugTest.getBody1();
		debugTest = debugTest.getNext();
	}*/
	updateObjectPointers(firstContact.getBody0(),firstContact.getBody1());

	water = NULL;
    notwater = NULL;
    if(obj_0 && obj_0->getType() == otWater)
    {
        water = obj_0;
        notwater=obj_1;
    }
    else if(obj_1 && obj_1->getType() == otWater)
    {
        water = obj_1;
        notwater=obj_0;
    }


	if(water)
    {
        water->onCollision(notwater,this);
		notwater->collisionWithWater(static_cast<WaterPlane*>(water));
		//return false;
		//cancel collision here
		OgreNewt::Contact contact = contactJoint.getFirstContact();
		while(contact)
        {
			OgreNewt::Contact next = contact.getNext(); 
			contact.remove();
			contact = next;
		}

    }
	else
    {
        if(obj_0)
	        obj_0->onCollision(obj_1,this);
        if(obj_1)
	        obj_1->onCollision(obj_0,this);
    }

	

	//return true;
}




ContCallback::ContCallback()//global_class *gl)
{
}

ContCallback::~ContCallback()
{
}

void ContCallback::updateObjectPointers(OgreNewt::Body *body0, OgreNewt::Body *body1)
{
	obj_0 = Ogre::any_cast<GameObject*>(body0->getUserData());
	obj_1 = Ogre::any_cast<GameObject*>(body1->getUserData());

	water = NULL;
    notwater = NULL;
    if(obj_0 && obj_0->getType() == otWater)
    {
        water = obj_0;
        notwater=obj_1;
    }
    else if(obj_1 && obj_1->getType() == otWater)
    {
        water = obj_1;
        notwater=obj_0;
    }
}