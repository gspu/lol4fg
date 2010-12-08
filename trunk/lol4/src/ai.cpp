#include "ai.h"
#include "OgreNewt.h"
#include "GameChar.h"
#include "GameApp.h"
#include "Level.h"
#include "functions.h"


//using namespace Ogre;

AI::AI(GameChar *_gc)
{
	myGameChar = _gc;
	mJumpHeight = myGameChar->getJumpHeight();
	app = (GameApp*)StandardApplication::getSingletonPtr();
	enemy = NULL;
	mType = mtWander;
	maxDistance = 8;
	minDistance = 4;
#ifdef _DEBUG
//	debugRayNode1 = NULL;
//	debugRayNode2 = NULL;
//	debugRayNode3 = NULL;
	
	
#endif
} 

AI::~AI()
{
	
#ifdef _DEBUG
//	destroyNode(myGameChar->getLevel()->getSceneManager(),debugRayNode1);
//	destroyNode(myGameChar->getLevel()->getSceneManager(),debugRayNode2);
//	destroyNode(myGameChar->getLevel()->getSceneManager(),debugRayNode3);
#endif
}

void AI::Die()
{
	app->removeEnemy(myGameChar);
}

void AI::updateGameChar()
{
	if(enemy)
	{
		Ogre::Vector3 ePos = enemy->getPosition();
		Ogre::Vector3 selfPos = myGameChar->getPosition();
		myGameChar->lookAt(ePos);
		Ogre::Real sqDist = selfPos.squaredDistance(ePos);

		if(sqDist > maxDistance*maxDistance)
		{
			//zu weit weg
			myGameChar->setAttack(false);
			myGameChar->setMoveUp(true);
			myGameChar->setRun(true);
		}
		else if(sqDist < minDistance*minDistance)
		{
			//zu nahe
			myGameChar->setAttack(false);
			myGameChar->setMoveDown(true);
			myGameChar->setRun(true);
		}
		else
		{
			//passt
			myGameChar->moveStop();
			myGameChar->setAttack(true);
		
		}
	}
	if(mType == mtWander)
	{
		//cast 2 rays, like this
		//         /¯¯¯\
		// r1<-----|   |
		//         | x |  das x markiert den Nullpunkt des chars!!1
		// r2   <--|   |
		//      |  \___/
		//     \|/
		//     r3

		//r1 for checking if something is in front of me
		//r2 for checking if something is in front of my feet -> jump
		//r3 for checking if i'm going to fall

		Ogre::Real headRayLength=2.5;
		Ogre::Real feetRayLength=1;
		Ogre::Real downRayLength1=1;
		Ogre::Real downRayLength2=2;

		OgreNewt::World *mWorld = myGameChar->getLevel()->getWorld();//app->getOgreNewtWorld();
		Quaternion gc_orient = myGameChar->getOrientation();
		Ogre::Vector3 lookingDir =  gc_orient * Ogre::Vector3::NEGATIVE_UNIT_Z;
		lookingDir.normalise();
		Ogre::Vector3 gc_pos = myGameChar->getPosition();

		
		Ogre::Vector3 pSize = myGameChar->getSize();

		Ogre::Vector3 feetPos = gc_pos+Ogre::Vector3(0,0.1-pSize.y/2,0) + (lookingDir*(pSize.z+0.1));
		Ogre::Vector3 feetEndPos = feetPos+lookingDir * feetRayLength; 
		Ogre::Vector3 headPos = gc_pos+(lookingDir*(pSize.z+0.05));
		//Vector3 headPos = gc_pos+Vector3(0,mJumpHeight-pSize.y/2,0)  +(lookingDir*(pSize.z));
		Ogre::Vector3 headEndPos = headPos+lookingDir * headRayLength;

		Ogre::Vector3 downPos = gc_pos+Ogre::Vector3(0,0.1-pSize.y/2,0) + (lookingDir*downRayLength2);
		Ogre::Vector3 downEndPos = feetEndPos;
		downEndPos.y -= downRayLength1;
	//
	//#ifdef _DEBUG
	//
	//	if(!debugRayNode1)
	//	{
	//		debugRayNode1 = myGameChar->getLevel()->getSceneManager()->getRootSceneNode()->createChildSceneNode();
	//		Ogre::Entity *ent = myGameChar->getLevel()->getSceneManager()->createEntity(getUniqueEntityName(myGameChar->getLevel()->getSceneManager(),"asdf"),"axes.mesh");
	//		debugRayNode1->attachObject(ent);
	//		debugRayNode1->setScale(Vector3(0.01));
	//	}
	//	if(!debugRayNode2)
	//	{
	//		debugRayNode2 = myGameChar->getLevel()->getSceneManager()->getRootSceneNode()->createChildSceneNode();
	//		Ogre::Entity *ent = myGameChar->getLevel()->getSceneManager()->createEntity(getUniqueEntityName(myGameChar->getLevel()->getSceneManager(),"asdf"),"axes.mesh");
	//		debugRayNode2->attachObject(ent);
	//		debugRayNode2->setScale(Vector3(0.01));
	//	}
	//	if(!debugRayNode3)
	//	{
	//		debugRayNode3 = myGameChar->getLevel()->getSceneManager()->getRootSceneNode()->createChildSceneNode();
	//		Ogre::Entity *ent = myGameChar->getLevel()->getSceneManager()->createEntity(getUniqueEntityName(myGameChar->getLevel()->getSceneManager(),"asdf"),"arrow.mesh");
	//		debugRayNode3->attachObject(ent);
	//		debugRayNode3->setScale(Vector3(0.5));
	//	}
	//
	//	debugRayNode1->setPosition(downPos);
	//	debugRayNode2->setPosition(downEndPos);
	//	debugRayNode3->setPosition(gc_pos);
	//	debugRayNode3->setDirection(lookingDir,Ogre::Node::TS_WORLD);
	//	//debugRayNode3->setOrientation(gc_orient);
	//
	//#endif

		OgreNewt::BasicRaycast headRay(mWorld,headPos,headEndPos,true);  
		OgreNewt::BasicRaycast feetRay(mWorld,feetPos,feetEndPos,true);  
		OgreNewt::BasicRaycast downRay(mWorld,downPos,downEndPos,true);


		Ogre::Real headDistance = headRayLength;
		Ogre::Real feetDistance = feetRayLength;
		
		if(!myGameChar->isMoving())
		{
			myGameChar->setMoveUp(true);

		}

		if(headRay.getHitCount() > 0)
		{
			//das später
			////neuen Weg finden
			//Quaternion rot45(Degree(45),Vector3::UNIT_Y);
			//Vector3 rot1 = (Vector3::NEGATIVE_UNIT_Z * rot45) * lookingDir;
			//rot45 = Quaternion(Degree(-45),Vector3::UNIT_Y);
			//Vector3 rot2 = (Vector3::NEGATIVE_UNIT_Z * rot45) * lookingDir;
			////von oben
			////rot1     rot2
			//// |\¯     ¯/|
			////   \     /
			////    \   /
			////     \ /
			////     |¯|
			//Vector3 headPos1    = gc_pos+(rot1*(pSize.z+0.05));		
			//Vector3 headEndPos1 = headPos+rot1 * headRayLength;

			//Vector3 headPos2    = gc_pos+(rot2*(pSize.z+0.05));		
			//Vector3 headEndPos2 = headPos+rot2 * headRayLength;
			myGameChar->modLookingDirection(45,0);


		}

		if(downRay.getHitCount() == 0)
		{
			
			

			//zur seite drehen
			myGameChar->modLookingDirection(45,0);
		}
		if(feetRay.getHitCount() > 0)
		{
			if(feetRay.getFirstHit().mBody == myGameChar->getBody())
			{
				mLog("ZOMG selfhit");
				//assert(false);
			}
			
			myGameChar->setMoveJump(true,true);
		}
	}
	
}

void AI::gotHurt(GameObject *attacker)
{
	startCombat(attacker);
}
void AI::startCombat(GameObject *attacker)
{
	enemy = attacker;
	mType = mtNone;
	if(enemy)
	{
		myGameChar->lookAt(enemy->getPosition());
		
		app->addEnemy(myGameChar);
	}
	
}