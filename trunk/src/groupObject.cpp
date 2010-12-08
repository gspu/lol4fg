#include "GroupObject.h"
#include "EditorApp.h"
#include "EditFrameListener.h"
#include "Level.h"
#include "functions.h"

GroupObject::GroupObject(Level *lvl,GameObject *first)
{
	//mLog("Erstelle Gruppe mit ersten objekt "+ptrToString(first));
	mNode = lvl->getSceneManager()->getRootSceneNode()->createChildSceneNode();//das ist die rootnode!
	mArt = NULL;
	mLevel = lvl;
	mBody = NULL;
	type = otGroup;
	ID = "";
//	name = "";
	baseScale = first->getScale();
	groupedObjects.push_back(first);
	mLevel->registerObject(this);
	EditorApp *app = (EditorApp*)StandardApplication::getSingletonPtr();
	//fl = app->getFrameListener();
	gl = app->qtApp->getGameLogic();
	dissolveCalled = false;
	deleteAllGrouped = true;
	isSelected = false;
    mNode->setPosition(first->getPosition());
    mNode->setOrientation(first->getOrientation());
    mNode->setScale(first->getScale());
}

//void GroupObject::addObject(GameObject *add)
//{
//
//	std::vector<GameObject*>::const_iterator itr;
//	for(itr = groupedObjects.begin();itr != groupedObjects.end();itr++)
//	{
//		//GameObject *cur = *itr;	
//		if(*itr == add)
//			return;
//	}
//	groupedObjects.push_back(add);
//}

void GroupObject::addObject(GameObject *add)		
{
	//mLog("adde zur gruppe "+ptrToString(add));
	if(add && !hasObject(add))
		groupedObjects.push_back(add);
//	updateRootValues();
}

void GroupObject::dissolveGroup()
{
	//mLog("dissolve group");
	/*if(fl->getSelected() == this)
	{
		fl->setSelected(NULL);
	}*/
	//einfach löschen
	if(dissolveCalled)
		return;
	dissolveCalled = true;
	if(!groupedObjects.empty() && isSelected)
	{
		gl->setSelected(groupedObjects[0]);
	}
	deleteAllGrouped = false;
	mLevel->deleteObject(this);
}

GroupObject::~GroupObject()
{
	//alles löschen
	//mLog("lösche gruppe");
	if(deleteAllGrouped && !mLevel->getDestructorCalled())
	{
		std::vector<GameObject*>::const_iterator itr;
		for(itr = groupedObjects.begin();itr != groupedObjects.end();itr++)
		{
			GameObject *cur = *itr;	
			delete cur;		
		}
	}
    
	//mLevel->unregisterObject(this);
}

void GroupObject::removeObject(GameObject *obj)
{
	//mLog("entferne aus gruppe "+ptrToString(obj));
	std::vector<GameObject*>::iterator itr;
	for(itr = groupedObjects.begin();itr != groupedObjects.end();itr++)
	{
		GameObject *cur = *itr;
		if(cur == obj)
		{
			//delete cur;
			groupedObjects.erase(itr);
			break;
		}
	}
	if(groupedObjects.size() == 1)
	{
		//nur noch 1 drin
		dissolveGroup();

	}
	else if(groupedObjects.empty())
	{
		mLevel->deleteObject(this);
	}
	else
	{
		gl->setSelected(this);
	}
	//updateRootValues();
}

//void GroupObject::updateChild(GameObject *child)
//{
//
//	Ogre::Vector3 parentPos;
//	Ogre::Vector3 parentScale;
//	Ogre::Quaternion parentOrnt;
//	GameObject *parent = groupedObjects[0];
//	parentScale = parent->getScale();
//	parent->getPositionOrientation(parentPos,parentOrnt);
//
//	Ogre::Vector3 mPos;
//	Ogre::Vector3 mScale;
//	Ogre::Quaternion mOrnt;
//	mScale = child->getScale();
//	child->getPositionOrientation(mPos,mOrnt);
//	//CODE FRON OGRE::NODE
// // Update orientation
//	Ogre::Vector3 connectionVector = oldPos-mPos;//verbindung parent->child
//	/*Ogre::Vector3 rotatedOld = oldOrnt * connectionVector;
//	Ogre::Vector3 rotatedNew= parentOrnt * connectionVector;*/
//	
//	Ogre::Quaternion rotBy = parentOrnt* oldOrnt.Inverse();//rotatedOld.getRotationTo(rotatedNew);
//	
//    // Combine orientation with that of parent
//	Ogre::Quaternion mNewOrnt = /*parentOrnt*/rotBy * mOrnt;
//	
//
//    
//   
//     // Scale own position by parent scale, NB just combine
//     // as equivalent axes, no shearing
//     Ogre::Vector3 mNewScale = parentScale * mScale;
//
//	 
//    
//
//    // Change position vector based on parent's orientation & scale
//	 Ogre::Vector3 mNewPos = /*parentOrnt*/rotBy * ((parentScale * connectionVector));
//
//    // Add altered position vector to parents
//	
//    mNewPos += parentPos;
//	mDebugText(str(mNewPos));
//	child->setScale(mNewScale);
//	child->setPositionOrientation(mNewPos,mNewOrnt);
//
//}
void GroupObject::setPosition(Ogre::Vector3 pos)
{
	//das erste objekt wird als parent angesehen
	GameObject *parent = groupedObjects[0];
	Ogre::Vector3 oldPos = parent->getPosition();
	mNode->setPosition(pos);

	std::vector<GameObject*>::const_iterator itr;
	parent->setPosition(pos);
	Ogre::Vector3 posChange = pos-oldPos;
	
	
	for(itr = groupedObjects.begin();itr != groupedObjects.end();itr++)
	{
		GameObject *cur = *itr;
		if(cur != parent)
		    cur->setPosition(cur->getPosition()+posChange);						
		
	}
}

//void GroupObject::updateRootValues()
//{
//	if(groupedObjects.size() == 0)
//		return;
//	Ogre::Vector3 sum = Ogre::Vector3::ZERO;
//	std::vector<GameObject*>::const_iterator itr;
//	for(itr = groupedObjects.begin();itr != groupedObjects.end();itr++)
//	{
//		sum += (*itr)->getPosition();
//	}
//	mNode->setPosition(sum / groupedObjects.size());
//	mNode->setOrientation(Ogre::Quaternion::IDENTITY);
//	mNode->setScale(Ogre::Vector3::UNIT_SCALE);
//
//}

void GroupObject::setOrientation(Ogre::Quaternion ornt)
{
	//das erste objekt wird als parent angesehen
	GameObject *parent = groupedObjects[0];
//	Ogre::Quaternion oldOrnt = mNode->getOrientation();
//	Ogre::Vector3 oldScale = rootScale;
	//Ogre::Vector3 oldPos = mNode->getPosition();
	std::vector<GameObject*>::const_iterator itr;

	Ogre::Vector3 oldPos;
	Ogre::Quaternion oldOrnt;

	Ogre::Vector3 oldScale = parent->getScale();
	parent->getPositionOrientation(oldPos,oldOrnt);

	parent->setOrientation(ornt);
	
	Ogre::Quaternion rotBy = ornt * oldOrnt.Inverse();
	for(itr = groupedObjects.begin();itr != groupedObjects.end();itr++)
	{
		GameObject *cur = *itr;
		if(cur != parent)
        {
		    Ogre::Quaternion newOrnt = rotBy * cur->getOrientation();
		
		    Ogre::Vector3 relPos = cur->getPosition()-oldPos;
		    relPos = rotBy * relPos;
        
		    cur->setPositionOrientation(relPos+oldPos,newOrnt);
        }
		//updateChild(cur);
	
	}
    
	mNode->setOrientation(ornt);
	//rootOrientation = ornt;
}

void GroupObject::setPositionOrientation(Ogre::Vector3 pos,Ogre::Quaternion ornt)
{
	setPosition(pos);
	setOrientation(ornt);
}

//retrieves body position
Ogre::Vector3 GroupObject::getPosition()
{
	//return mNode->getPosition();
	return groupedObjects[0]->getPosition();

}
//retr. body orient
Ogre::Quaternion GroupObject::getOrientation()
{
	return groupedObjects[0]->getOrientation();

}
//retr. both (false if fails)
bool GroupObject::getPositionOrientation( Ogre::Vector3& pos, Ogre::Quaternion& orient )
{
	/*pos = getPosition();
	orient = getOrientation();
	return true;*/
	return groupedObjects[0]->getPositionOrientation(pos,orient);

}

void GroupObject::setScale(Ogre::Vector3 scale)
{
	GameObject *parent = groupedObjects[0];
	std::vector<GameObject*>::const_iterator itr;
	Ogre::Quaternion oldOrnt;
	Ogre::Vector3 oldScale = parent->getScale();
	Ogre::Vector3 oldPos;
	parent->getPositionOrientation(oldPos,oldOrnt);

	Ogre::Vector3 scaleBy = scale/oldScale;


	parent->setScale(scale);	
	
	/*std::vector<GameObject*>::const_iterator itr;
	Ogre::Quaternion oldOrnt = mNode->getOrientation();
	Ogre::Vector3 oldScale = mNode->getScale();
	Ogre::Vector3 oldPos = mNode->getPosition();

	Ogre::Vector3 scaleBy = scale/oldScale;*/


	
	//oldOrnt = oldOrnt.Inverse();

	for(itr = groupedObjects.begin();itr != groupedObjects.end();itr++)
	{
		GameObject *cur = *itr;
		if(cur != parent)
        {
		    Ogre::Vector3 relPos = cur->getPosition()-oldPos;
		    //relPos = oldOrnt*relPos;
		    cur->modScale(scaleBy);
    		
		    relPos = oldOrnt.Inverse()*relPos;
		    relPos *= scaleBy;
		    relPos = oldOrnt*relPos;
		    //relPos *= oldOrnt.Inverse()*relPos;
    		
		    cur->setPosition(relPos+oldPos);
        }
		
	}
	mNode->setScale(scale);
}
void GroupObject::modScale(Ogre::Vector3 scale)
{
	GameObject *parent = groupedObjects[0];
	std::vector<GameObject*>::const_iterator itr;
	Ogre::Quaternion oldOrnt;
	Ogre::Vector3 oldPos;
	parent->getPositionOrientation(oldPos,oldOrnt);
	

	parent->modScale(scale);	
/*
	std::vector<GameObject*>::const_iterator itr;
	Ogre::Quaternion oldOrnt = mNode->getOrientation();
	Ogre::Vector3 oldPos = mNode->getPosition();*/

	for(itr = groupedObjects.begin();itr != groupedObjects.end();itr++)
	{
		GameObject *cur = *itr;
		if(cur != parent)
        {
		    Ogre::Vector3 relPos = cur->getPosition()-oldPos;
		    cur->modScale(scale);
		    relPos = oldOrnt.Inverse()*relPos;
		    relPos *= scale;
		    relPos = oldOrnt*relPos;
		    cur->setPosition(relPos+oldPos);
        }
		
	}
	mNode->scale(scale);

}
Ogre::Vector3 GroupObject::getScale()
{
	return groupedObjects[0]->getScale();
}

void GroupObject::updateBody()
{
	std::vector<GameObject*>::const_iterator itr;
	
	for(itr = groupedObjects.begin();itr != groupedObjects.end();itr++)
	{
		GameObject *cur = *itr;
		cur->updateBody();
	}
}
void GroupObject::markSelected(bool mark)
{
	std::vector<GameObject*>::const_iterator itr;
	isSelected = mark;
	for(itr = groupedObjects.begin();itr != groupedObjects.end();itr++)
	{
		GameObject *cur = *itr;
		cur->markSelected(mark);
	}
}


void GroupObject::rotate(const Ogre::Vector3& axis, const Ogre::Radian& angle, Ogre::Node::TransformSpace relativeTo)
{
	Ogre::Quaternion q;
	q.FromAngleAxis(angle,axis);
	
	rotate(q, relativeTo);
	
	
}



void GroupObject::rotate(const Ogre::Quaternion& q, Ogre::Node::TransformSpace relativeTo)
{
	
	Ogre::Quaternion oldOrnt = getOrientation();
	
	Ogre::Quaternion newOrnt;
	
	switch(relativeTo)
	{
	case Ogre::Node::TS_WORLD:
	case Ogre::Node::TS_PARENT:
		// Rotations are normally relative to local axes, transform up
		newOrnt = q * oldOrnt;
		break;

	case Ogre::Node::TS_LOCAL:
		// Note the order of the mult, i.e. q comes after
		newOrnt = oldOrnt * q;
		break;
	}

	setOrientation(newOrnt);

}



//BACKUP
/*
GroupObject::GroupObject(Level *lvl,GameObject *first)
{
	//mLog("Erstelle Gruppe mit ersten objekt "+ptrToString(first));
	mNode = lvl->getSceneManager()->getRootSceneNode()->createChildSceneNode();//das ist die rootnode!
	mArt = NULL;
	mLevel = lvl;
	mBody = NULL;
	type = otGroup;
	ID = "";
//	name = "";
	baseScale = first->getScale();
	groupedObjects.push_back(first);
	mLevel->registerObject(this);
	EditorApp *app = (EditorApp*)StandardApplication::getSingletonPtr();
	fl = app->getFrameListener();
	dissolveCalled = false;
	deleteAllGrouped = true;
	isSelected = false;
}



void GroupObject::addObject(GameObject *add)		
{
	//mLog("adde zur gruppe "+ptrToString(add));
	if(add && !hasObject(add))
		groupedObjects.push_back(add);
	updateRootValues();
}

void GroupObject::dissolveGroup()
{

	//einfach löschen
	if(dissolveCalled)
		return;
	dissolveCalled = true;
	if(!groupedObjects.empty() && isSelected)
	{
		fl->setSelected(groupedObjects[0]);
	}
	deleteAllGrouped = false;
	mLevel->deleteObject(this);
}

GroupObject::~GroupObject()
{

	if(deleteAllGrouped && !mLevel->getDestructorCalled())
	{
		std::vector<GameObject*>::const_iterator itr;
		for(itr = groupedObjects.begin();itr != groupedObjects.end();itr++)
		{
			GameObject *cur = *itr;	
			delete cur;		
		}
	}
}

void GroupObject::removeObject(GameObject *obj)
{
	std::vector<GameObject*>::iterator itr;
	for(itr = groupedObjects.begin();itr != groupedObjects.end();itr++)
	{
		GameObject *cur = *itr;
		if(cur == obj)
		{
			//delete cur;
			groupedObjects.erase(itr);
			break;
		}
	}
	if(groupedObjects.size() == 1)
	{
		//nur noch 1 drin
		dissolveGroup();

	}
	else if(groupedObjects.empty())
	{
		mLevel->deleteObject(this);
	}
	else
	{
		fl->setSelected(this);
	}
	updateRootValues();
}

void GroupObject::setPosition(Ogre::Vector3 pos)
{

	Ogre::Vector3 oldPos = mNode->getPosition();// parent->getPosition();
	mNode->setPosition(pos);

	std::vector<GameObject*>::const_iterator itr;
	//parent->setPosition(pos);
	Ogre::Vector3 posChange = pos-oldPos;
	
	
	for(itr = groupedObjects.begin();itr != groupedObjects.end();itr++)
	{
		GameObject *cur = *itr;
		
		cur->setPosition(cur->getPosition()+posChange);						
		
	}
}

void GroupObject::updateRootValues()
{
	if(groupedObjects.size() == 0)
		return;
	Ogre::Vector3 sum = Ogre::Vector3::ZERO;
	std::vector<GameObject*>::const_iterator itr;
	for(itr = groupedObjects.begin();itr != groupedObjects.end();itr++)
	{
		sum += (*itr)->getPosition();
	}
	mNode->setPosition(sum / groupedObjects.size());
	mNode->setOrientation(Ogre::Quaternion::IDENTITY);
	mNode->setScale(Ogre::Vector3::UNIT_SCALE);

}

void GroupObject::setOrientation(Ogre::Quaternion ornt)
{
	//das erste objekt wird als parent angesehen
	//GameObject *parent = groupedObjects[0];
	Ogre::Quaternion oldOrnt = mNode->getOrientation();
	//Ogre::Vector3 oldScale = rootScale;
	Ogre::Vector3 oldPos = mNode->getPosition();
	std::vector<GameObject*>::const_iterator itr;


	
	Ogre::Quaternion rotBy = ornt * oldOrnt.Inverse();
	for(itr = groupedObjects.begin();itr != groupedObjects.end();itr++)
	{
		GameObject *cur = *itr;
		
		Ogre::Quaternion newOrnt = rotBy * cur->getOrientation();
		
		Ogre::Vector3 relPos = cur->getPosition()-oldPos;
		relPos = rotBy * relPos;
		cur->setPositionOrientation(relPos+oldPos,newOrnt);
		//updateChild(cur);
	
	}
	mNode->setOrientation(ornt);
	//rootOrientation = ornt;
}

void GroupObject::setPositionOrientation(Ogre::Vector3 pos,Ogre::Quaternion ornt)
{
	setPosition(pos);
	setOrientation(ornt);
}

//retrieves body position
Ogre::Vector3 GroupObject::getPosition()
{
	return mNode->getPosition();
	//return groupedObjects[0]->getPosition();

}
//retr. body orient
Ogre::Quaternion GroupObject::getOrientation()
{
	return mNode->getOrientation(); //groupedObjects[0]->getOrientation();

}
//retr. both (false if fails)
bool GroupObject::getPositionOrientation( Ogre::Vector3& pos, Ogre::Quaternion& orient )
{
	pos = getPosition();
	orient = getOrientation();
	return true;
	//return groupedObjects[0]->getPositionOrientation(pos,orient);

}

void GroupObject::setScale(Ogre::Vector3 scale)
{

	
	std::vector<GameObject*>::const_iterator itr;
	Ogre::Quaternion oldOrnt = mNode->getOrientation();
	Ogre::Vector3 oldScale = mNode->getScale();
	Ogre::Vector3 oldPos = mNode->getPosition();

	Ogre::Vector3 scaleBy = scale/oldScale;


	
	//oldOrnt = oldOrnt.Inverse();

	for(itr = groupedObjects.begin();itr != groupedObjects.end();itr++)
	{
		GameObject *cur = *itr;
		
		Ogre::Vector3 relPos = cur->getPosition()-oldPos;
		//relPos = oldOrnt*relPos;
		cur->modScale(scaleBy);
		
		relPos = oldOrnt.Inverse()*relPos;
		relPos *= scaleBy;
		relPos = oldOrnt*relPos;
		//relPos *= oldOrnt.Inverse()*relPos;
		
		cur->setPosition(relPos+oldPos);
		
	}
	mNode->setScale(scale);
}
void GroupObject::modScale(Ogre::Vector3 scale)
{

	std::vector<GameObject*>::const_iterator itr;
	Ogre::Quaternion oldOrnt = mNode->getOrientation();
	Ogre::Vector3 oldPos = mNode->getPosition();

	for(itr = groupedObjects.begin();itr != groupedObjects.end();itr++)
	{
		GameObject *cur = *itr;
		
		Ogre::Vector3 relPos = cur->getPosition()-oldPos;
		cur->modScale(scale);
		relPos = oldOrnt.Inverse()*relPos;
		relPos *= scale;
		relPos = oldOrnt*relPos;
		cur->setPosition(relPos+oldPos);
		
	}
	mNode->setScale(rootScale * scale);

}
Ogre::Vector3 GroupObject::getScale()
{
	return mNode->getScale();// groupedObjects[0]->getScale();
}

void GroupObject::updateBody()
{
	std::vector<GameObject*>::const_iterator itr;
	
	for(itr = groupedObjects.begin();itr != groupedObjects.end();itr++)
	{
		GameObject *cur = *itr;
		cur->updateBody();
	}
}
void GroupObject::markSelected(bool mark)
{
	std::vector<GameObject*>::const_iterator itr;
	isSelected = mark;
	for(itr = groupedObjects.begin();itr != groupedObjects.end();itr++)
	{
		GameObject *cur = *itr;
		cur->markSelected(mark);
	}
}


void GroupObject::rotate(const Ogre::Vector3& axis, const Ogre::Radian& angle, Ogre::Node::TransformSpace relativeTo)
{
	Ogre::Quaternion q;
	q.FromAngleAxis(angle,axis);
	
	rotate(q, relativeTo);
	
	
}



void GroupObject::rotate(const Ogre::Quaternion& q, Ogre::Node::TransformSpace relativeTo)
{
	
	Ogre::Quaternion oldOrnt = rootOrientation;//getOrientation();
	
	Ogre::Quaternion newOrnt;
	
	switch(relativeTo)
	{
	case Ogre::Node::TS_WORLD:
	case Ogre::Node::TS_PARENT:
		// Rotations are normally relative to local axes, transform up
		newOrnt = q * oldOrnt;
		break;

	case Ogre::Node::TS_LOCAL:
		// Note the order of the mult, i.e. q comes after
		newOrnt = oldOrnt * q;
		break;
	}

	setOrientation(newOrnt);

}
*/