#ifndef __praAI
#define __praAI
#include "FwDec.h"
#include "Ogre.h"
//#include "GameChar.h"


class AI
{
public:
	AI(GameChar *_gc);
	~AI();
	void updateGameChar();
	void gotHurt(GameObject *attacker);
	enum MovementType
	{
		mtNone,
		mtWander,
		mtFollow
	};
	void startCombat(GameObject *attacker);
	void stopCombat();
	void Die();
private:
	MovementType mType;

#ifdef _DEBUG
//	Ogre::SceneNode *debugRayNode1;
//	Ogre::SceneNode *debugRayNode2;
//	Ogre::SceneNode *debugRayNode3;
#endif
	Ogre::Real mJumpHeight;
	GameChar *myGameChar;
	GameApp *app;
	GameObject *enemy;
	Ogre::Real minDistance; //die distanz soll er zum spieler einhalten
	Ogre::Real maxDistance;

};

#endif