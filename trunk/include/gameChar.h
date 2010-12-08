#ifndef __pra_gameChar
#define __pra_gameChar

#include "Ogre.h"
#include "OgreNewt.h"
#include "FwDec.h"

#include "defines.h"
#include "Inventory.h"
#include "Spellbook.h"
#include "StandardApplication.h"
//class global_class;
//#include "global_class.h"
//#include "ai.h"
#include "GameObject.h"
#ifndef __editor
#include "GameApp.h"
#include "AI.h"
#include "DoorObject.h"
#endif
#include "SpellEffect.h"
#include "Sound.h"
#include "data.h"
//class global_class;
//class objCont;


using namespace OgreNewt;




class GameChar:
	/*public OgreNewt::Vehicle,*/ public GameObject
{
public:
	friend class StandardApplication;
	friend class Level;
#ifndef __editor
	friend class AI;
#endif

	/*hilfsstruct für animationen. enthält einen Pointer auf das Ogre::AnimationState
	der mesh und einen faktor, der auf die zeit multipliziert wird*/
	struct CharAnimation
	{
		CharAnimation()
		{
			anim = NULL;
			factor = 1;
		}
		Ogre::AnimationState* anim;
		Ogre::Real factor;
	};

	typedef std::vector<CharAnimation> AnimationList;

	

	struct LookAtResult
	{
		GameObject *obj;
		Ogre::Vector3 hitPoint;
		Ogre::Real objDist;
	};

	//gibt die aktuell gültigen Stats für diesen GameChar zurück
	Stats getStats()
	{
		return mStats;
	}

	//konstruktor über gamedata-pointer
	//soweit möglich, sollte dieser verwendet werden
	GameChar(gamedata_char* myData,Level *lvl, Ogre::Vector3 pos,Ogre::Quaternion orient = Ogre::Quaternion::IDENTITY)
	{
		init(lvl,myData,pos,orient);
	}
	GameChar(TiXmlElement *XMLNode,Level *lvl, Ogre::Vector3 positionOffset = Ogre::Vector3::ZERO,bool savegame = false);
	//konstruktor über Ogre::String-ID. dieser Konstruktor sucht nach dem zugehörigen
	//gamedata und erstellt das Objekt, bei Nichtgelingen wird int EX_CONSTR_FAILED
	//geworfen
	GameChar(Ogre::String gcId,Level *lvl, Ogre::Vector3 pos,Ogre::Quaternion orient = Ogre::Quaternion::IDENTITY)
	{	
		//prüfe, ob die angegebene ID stimmt
		gamedata_char *myData = StandardApplication::getSingletonPtr()->getCharDataPtr(gcId);//app->getGameCharDataPtr(gcId);
		//wenn nicht, ...
		if(!myData)//->id == "")
		{
			//hier exception throwen
			throw etInvalidId;
			//return;
		}
		init(lvl,myData,pos,orient);
	}
	//kopierkonstruktor, erstellt einen gamechar an der selben Position und mit den selben werten
	//wie src
	GameChar(GameChar *src, Level *targetLvl = NULL);
	void collisionWithWater(WaterPlane *water);
	//standard destruktor
	~GameChar();

    bool hasItem(gamedata_item *item);
    bool hasSpell(gamedata_spell *spell)
    {
        return mSb->hasSpell(spell);
    }
	//erstellt einen neuen gamechar mittels des Kopierkonstruktors
	//und gibt einen pointer darauf zurück
	GameObject* clone(Level *targetLvl = NULL)
	{
		return static_cast<GameObject*>(new GameChar(this,targetLvl));
	}

	//attached die Ogre::Camera mit dem Namen camName
	//an die camNode des GameChars
	void attachCam(Ogre::String camName = CAM_NAME);
	//attached die Ogre::Camera cam
	//an die camNode des GameChars
	void attachCam(Ogre::Camera *cam);
	
	//entfernt die camera namens camName von der camNode des chars
	void detachCam(Ogre::String camName = CAM_NAME);
	//entfernt cam von der camnode des chars
	void detachCam(Ogre::Camera *cam);

    bool isOnWaterSurface();

	//OgreNewt force callback. sollte nur von OgreNewt selbst aufgerufen werden!
	void forceCallback( OgreNewt::Body* me );
	//wird vom Level aufgerufen, um den gamechar zu aktualisieren
	void update(Ogre::Real time);

	void onCollision(GameObject *with,ContCallback *cb);
	
	virtual TiXmlElement* getXmlElem(Ogre::Vector3 startPosition = Ogre::Vector3::ZERO,bool savegame=false);
	//fügt einen Spell dem Char hinzu
	//gemeint ist nicht das Hinzufügen als verfügbare Zauber,
	//sondern als Zauber, die gerade auf den Char einwirken
	//param spell: datenpointer auf den zu erstellenden zauber
	//param caster: das Objekt, welches den zauber gewirkt hat
	void AddSpell(gamedata_spell *spell,GameObject *caster);	
	//entfernt den ersten auf den char einwirkenden zauber, der mit
	//"spell" als datenpointer erstellt wurde
	//der Eintrag wird NICHT sofort aus der Liste gelöscht!
	void RemoveSpell(gamedata_spell *spell);
	//entfernt ein SpellObject, welches auf den Char einwirkt
	//die Funktion setzt es auf die Liste der zu löschenden.
	//das Objekt selbst wird im nächsten Frame aus der aktiven liste entfernt und gelöscht, sodass
	//ein SpellObject oder dessen bestandteile diese funktion
	//nutzen können, um sich selbst zu löschen
	void RemoveSpell(SpellObject *spell);
	//löscht den spell von der liste, und sonst nichts
	void RemoveSpellFromActive(gamedata_spell *spell);
	//löscht den spell von der liste, und sonst nichts
	void RemoveSpellFromActive(SpellObject *spell);
	
	//lässt den char sterben
	void Die();

	//aktiviert/deaktiviert das Springen des Chars. 
	//wird once auf true gesetzt, wird das Springen 
	//nach dem ersten sprung automatisch deaktiviert
	inline void setMoveJump(bool set,bool once = false)
	{
	   jump = set;
	   jumpOnce = once;
	 
	}


	//aktiviert/deaktiviert die Bewegung vorwärts
	inline void setMoveUp(bool set)
	{
	   mvUp = set;
	 
	} 
	//aktiviert/deaktiviert die Bewegung rückwärts
	inline void setMoveDown(bool set)
	{

		mvDown = set;
		
	}
	//aktiviert/deaktiviert die seitliche Bewegung nach links
	inline void setMoveLeft(bool set)
	{

		mvLeft = set;
	}
	//aktiviert/deaktiviert die seitliche Bewegung nach rechts
	inline void setMoveRight(bool set)
	{
		mvRight = set;
	}

	//lässt den Char anhalten
	inline void moveStop()
	{
		mvDown = false;
		mvUp = false;
		mvRight = false;
		mvLeft = false;
		jump = false;
	}



	//aktiviert/deaktiviert das angreifen.
	//während der angriff aktiv ist, greift der Char das Objekt an,
	//welches sich in seiner unmittelbaren Sicht befindet,
	//bzw im Falle des Spielers im cursor mode ist das das objekt, über dem
	//sich der Mauszeiger befindet 
	//für den angriff werden die aktuell angelegten Waffen genutzt, 
	//ob nah- oder fernkampf wird automatisch anhand der entfernung entschieden
	inline void setAttack(bool set)
	{
		attacking = set;
	}
	//aktiviert/deaktiviert das spellcasten.
	//während das spellcasten aktiv ist, greift der Char das Objekt mit
	//dem als aktiv markiertem Zauber an,
	//welches sich in seiner unmittelbaren Sicht befindet,
	//bzw im Falle des Spielers im cursor mode ist das das objekt, über dem
	//sich der Mauszeiger befindet 
	inline void setSpellcast(bool set)
	{
		spellcasting = set;
	}

	//setzt die "initialposition" des chars
	//das ist Teil eines Workarounds,
	//der verhindert, dass der Char beim ersteb Updaten
	//der OgreNewt::World an undefinierten Koordinaten landet
	inline void setInitPositionOrientation(Ogre::Vector3 pos,Ogre::Quaternion ornt)
	{
		startingOrnt = ornt;
		startingPos = pos;
		setPositionOrientation(pos,ornt);
	}

	LookAtResult getLookingAt(Ogre::Real maxRange, Ogre::Real minRange = 0);
	Ogre::Ray getLookingRay();
	
	//gibt wahr zurück, wenn der char irgendwohin läuft, dh wenn
	//wenigstens eines der setMoveX auf wahr gesetzt wurde
	inline bool isMoving()
	{
		bool moveUD = mvDown != mvUp;//(mvDown || mvUp) && !(mvDown && mvUp);
		bool moveLR = mvRight != mvLeft;//(mvLeft || mvRight) && !(mvLeft && mvRight); 
		return (moveUD || moveLR);//(movement != Ogre::Vector3::ZERO);
	}
	//lässt den Char "aktion" auf das gerade angeschaute objekt machen
	inline void doAction()
	{
		action = true;
	}
	//aktiviert/deaktiviert das Rennen
	//Renngeschwindigkeit ist Teil der Stats
	inline void setRun(bool set)
	{
		run = set;
	}
	//gibt wahr zurück, wenn der Char im rennmodus ist
	inline bool getRun()
	{
		return run;
	}

	//gibt zurück ob der char angreift
	inline bool isAttacking()
	{
		return attacking;
	}
	//gibt zurück, ob der char springt
	inline bool isJumping()
	{
		return jump;
	}

	//for looking around with the mouse
	void modLookingDirection(Ogre::Real x, Ogre::Real y);
	//look along this vector
	void setLookingDirection(Ogre::Vector3 dir);

    Ogre::Vector3 getLookingDirection()        
    {
        return camNode->getOrientation()*Ogre::Vector3::NEGATIVE_UNIT_Z;
    }
    //returns the direction where the user wants the char to go
    Ogre::Vector3 getMovementVector(bool inWater);
	//look at this point
	void lookAt(Ogre::Vector3 target);

	//set the minimal height the char should reach 
	//berechnet die Kraft, die nötig ist, um die Sprunghöhe "height"
	//bei einer Gravitation von -9.81 durch einen vertikalen Sprung erreichen soll,
	//und setzt sie als aktuelle Sprungkraft
	/*TODO: das durch stats festlegen*/
	void setJumpHeight(Ogre::Real height);
	//gibt den mit setJumpHeight berechneten wert zurück
	Ogre::Real getJumpHeight();

	//überschreibt die defaultfunktionen, weil Chars nicht skalierbar sind
	inline Ogre::Vector3 getScale()
	{
		//return meshNode->getScale()/baseScale;
		return Ogre::Vector3::UNIT_SCALE;
	}
	inline void modScale(Ogre::Vector3 scale)
	{
		//meshNode->scale(scale);
		
	}
	inline void setScale(Ogre::Vector3 scale)
	{
		//meshNode->setScale(scale*baseScale);
	}
	//gibt die MeshNode zurück, die zugleich Ogre::Node des WorldArts ist
	inline Ogre::SceneNode *getMeshNode()
	{
		return meshNode;
	}	

	
	
	inline Ogre::String getName()
	{
		return static_cast<gamedata_char*>(mData)->name;
	}
	//blendet die anzeige der AABB der meshnode ein
	//nur für den editor relevant
	inline void markSelected(bool mark)
	{
		meshNode->showBoundingBox(mark);
	}
	//überschreibt die funktion, die das Collision des body neu erzeugen lässt
	inline void updateBody(){};

	//fügt dem char schaden zu. attacker ist das angreifer-objekt
	void onDamage(Damage dmg,GameObject *attacker = NULL);
	//führt "aktion" auf dem char aus
	//eigentlich kann user nur der player sein, der den Char anspricht
	//oder seine leiche fleddern will
	void onActivate(GameChar *user,bool cursorMode = false);

	inline Inventory *getInventory()
	{
		return inv;
	}
	inline void setInventory(Inventory *neu)
	{
		inv = neu;
	}
	inline Spellbook *getSpellbook()
	{
		return mSb;
	}

	bool
		mvUp,
		mvDown,
		mvLeft,
		mvRight; //i decided that this is better than a vector3

	inline Ogre::Vector3 getSize()
	{
		return mSize;
	}
	

	bool setMelee(Ogre::String itemID);
	bool setRanged(Ogre::String itemID);
	bool setShield(Ogre::String itemID);
	bool setArmor(Ogre::String itemID);
	bool setBag(Ogre::String containerID,Inventory *_inv = NULL);
	bool setElse1(Ogre::String itemID);
	bool setElse2(Ogre::String itemID);
	bool setElse3(Ogre::String itemID);
	bool setElse4(Ogre::String itemID);
	
	bool setMelee(gamedata_item *item);
	bool setRanged(gamedata_item *item);
	bool setShield(gamedata_item *item);
	bool setArmor(gamedata_item *item);
	bool setBag(gamedata_item* container,Inventory *_inv = NULL);
	bool setElse1(gamedata_item *item);
	bool setElse2(gamedata_item *item);
	bool setElse3(gamedata_item *item);
	bool setElse4(gamedata_item *item);

	bool equipItem(gamedata_item *item,unsigned int type,Inventory *_inv = NULL);
	bool equipItem(gamedata_item *item,EquipType type,Inventory *_inv = NULL)
	{
		equipItem(item,(unsigned int)type,_inv);
	}

    //das equipt automatisch ein best. Item
    bool equipItem(gamedata_item *item);
    //"essen"
    bool useItem(gamedata_item *item);

	bool setSpell(Ogre::String spellID);
	bool setSpell(gamedata_spell* spell);

	inline gamedata_item* getEquippedItem(EquipType type)
	{
		return equipped[type];
	}

	//to prevent enum -> int casting trouble
	inline gamedata_item* getEquippedItem(unsigned int type)
	{
		if(type < EQUIP_CNT)
			return equipped[type];
		return NULL;
	}

	inline gamedata_item* getMelee()
	{
		return equipped[eqMelee];
	}
	inline gamedata_item* getRanged()
	{
		return equipped[eqRanged];
	}
	inline gamedata_item* getShield()
	{
		return equipped[eqShield];
	}
	inline gamedata_item* getArmor()
	{
		return equipped[eqArmor];
	}

	inline gamedata_item* getElse1()
	{
		return equipped[eqElse1];
	}
	inline gamedata_item* getElse2()
	{
		return equipped[eqElse2];
	}
	inline gamedata_item* getElse3()
	{
		return equipped[eqElse3];
	}
	inline gamedata_item* getElse4()
	{
		return equipped[eqElse4];
	}
	inline gamedata_item* getBag()
	{
		return equipped[eqBag];
	}

	inline Ogre::String getMeleeId()
	{
		if(equipped[eqMelee])
			return equipped[eqMelee]->id;
		else
			return "";
	}
	inline Ogre::String getRangedId()
	{
		if(equipped[eqRanged])
			return equipped[eqRanged]->id;
		else
			return "";
	}
	inline Ogre::String getShieldId()
	{
		if(equipped[eqShield])
			return equipped[eqShield]->id;
		else
			return "";
	}
	inline Ogre::String getArmorId()
	{
		if(equipped[eqArmor])
			return equipped[eqArmor]->id;
		else
			return "";
	}

	inline Ogre::String getElse1Id()
	{
		if(equipped[eqElse1])
			return equipped[eqElse1]->id;
		else
			return "";
	}
	inline Ogre::String getElse2Id()
	{
		if(equipped[eqElse2])
			return equipped[eqElse2]->id;
		else
			return "";
	}
	inline Ogre::String getElse3Id()
	{
		if(equipped[eqElse3])
			return equipped[eqElse3]->id;
		else
			return "";
	}
	inline Ogre::String getElse4Id()
	{
		if(equipped[eqElse4])
			return equipped[eqElse4]->id;
		else
			return "";
	}
	inline Ogre::String getBagId()
	{
		if(equipped[eqBag])
            return equipped[eqBag]->Container->id;//important, everything appends "cointainer_" itself
		else
			return "";
	}
	inline gamedata_spell* getSpell()
	{
		return eqSpell;
	}
	inline Ogre::String getSpellId()
	{
		if(eqSpell)
			return eqSpell->id;
		else
			return "";
	}


	inline Ogre::Quaternion getFullOrientation()
	{
        return camNode->_getDerivedOrientation();
		//return camNode->_getDerivedOrientation();
	}
	inline Ogre::Vector3 getHeadPosition()
	{
        return camNode->_getDerivedPosition();//camNode->_getDerivedPosition();
	}
	//typedef std::list<SpellEffect*> EffectList;
	

	Ogre::String getDisplayName()
	{
		return ID;
	}
	bool isDead()
	{
		return dead;
	}
	enum AnimationType
	{
		//anim types
		AT_NONE = 0,
		AT_IDLE = 1,
		AT_WALK = 2,
		AT_ATKMELEE=3,
		AT_ATKRANGED=4,
		AT_DIE  = 5,
		AT_JUMP = 6,
		AT_RUN  = 7,
		AT_FALL = 8,
		AT_CAST = 9

	};

	//**hier die stat-funktionen
	//stats der angelegten items updaten. spell-stats werden bislang nciht berücksichtigt, überhaupt ist die 
	//funktion in erster linie zum initialisieren
	void updateStats();
	//wendet einen einzelnen stat an
	void addStat(ChStat st);
	//macht einen stat rückgängig
	void removeStat(ChStat st);
	//ganze statliste adden
	void addStatList(StatChangeList list)
	{
		for(StatChangeList::const_iterator itr=list.begin();itr!=list.end();itr++)
		{
			addStat(*itr);
		}
	}
	//liste entfernen...
	void removeStatList(StatChangeList list)
	{
		for(StatChangeList::const_iterator itr=list.begin();itr!=list.end();itr++)
		{
			removeStat(*itr);
		}
	}
	//**stat-funkionen ende
	
	//das gibt ein Ogre::Quaternion zurück, der die Schussrichtung beschreibt
	Ogre::Quaternion getFiringOrientation();
	//das holt alle möglichen Informationen zur GUckrichtung des Charakters
	//
	void getTargetData(Ogre::Quaternion &firingOrnt, Ogre::Vector3 &targetPoint, Ogre::Real &targetDist);

    void attackMelee(GameObject *target);
	void attackRanged();
	void castSpell();
private:
	bool dead;
	void init(Level *lvl, gamedata_char* myData,  Ogre::Vector3 pos,Ogre::Quaternion orient = Ogre::Quaternion::IDENTITY,int SaveGameID = 0);
	//hier zeug für zeitversetze multimissile
	//das hier speichert, wie viele repeats wir noch brauchen
	Ogre::Real missileToSpawnRepeat;
	//und das hier alles andere zur missile
	gamedata_damagedealer *missileToSpawn;
	//MissileData missileToSpawn;
	Ogre::Real timeSinceLastMissile;
	//Damage dmgToSpawn;
	//EffectDataList *effectsToSpawn;
	//Damage dmgToSpawn;
//	SpecificMissileData missileToSpawn;
	SimpleSound::Source *moveSoundSource; //diese quelle wird für alle möglichen bewegungssounds benutzt
	//der ist zum angreifen. buffer wird bei bedarf erzeugt
	SimpleSound::Source *attackSoundSource;

	//das sind die buffer für die 3 bewegungssnds
	SimpleSound::Buffer *walkSoundBuffer;
	SimpleSound::Buffer *jumpSoundBuffer;
	SimpleSound::Buffer *landSoundBuffer;

	
	Ogre::Real moveSoundPassed; //wie viel seit Ende des Sounds vergangen ist
	
	
	//SimpleSound::Source *jumpLandSound; //der Sound zum Springen und landen

	Ogre::Real cooldown;
	//Ogre::Real spellCooldown;

	Inventory *inv;
	Spellbook *mSb;

	//liste aller effekte
	SpellList activeSpells;
	SpellList spellsToDelete;
//	EffectList activeEffects;
//	EffectList effectsToDelete;

	//von oben nach unten die stats mit zunehmender priorität
	//"jungfräuliche" anfangsstats
	Stats baseStats;
	//baseStats, nachdem die ChStat mit mode = opAdd angewandt wurden
	Stats addStats;
	//addStats, nachdem die ChStat mit mode = opMultiply angewandt wurden
	Stats multStats;
	//und die entgültigen stats, nachdem einige werte von ChStat wg opSet geändert wurden
	Stats mStats;	
	
	
	gamedata_item* equipped[EQUIP_CNT];
	//gamedata_item* eqBag;

    //führt den raycast zum Boden durch
    bool isOnEarth();

	//
	void setPlayerControlled(bool set)
	{
		playerControlled = set;
	}

	gamedata_spell *eqSpell;
	
	//pointer to the AI class
	AI *mAI;
	//pointer to the ogre "animation state"
	//Ogre::AnimationState *mMoveState, *mStillState;
	//Ogre::AnimationState *currentAno;
	//for both to be played at the same time... i hope 

	struct AnimLists
	{
		AnimationList idle; 
		AnimationList walk;
		AnimationList death;
		AnimationList attackmelee;
		AnimationList attackranged;
		AnimationList jump;
		AnimationList run;
		AnimationList fall;
		AnimationList cast;
	};
	AnimLists mAnims;
	 

	//test
	//OgreNewt::BasicJoints::UpVector* rotpreventer;

	bool doOnce;
	int resetLag;
	Ogre::Vector3 startingPos;
	Ogre::Quaternion startingOrnt;
	



	//pointer to the char data (like mass, the anims etc.)
	

	AnimationType animType;
	//the level this char is currently in
	//Level *mLevel;  defined in GameObject
	
	
	//sensibility MUSS hier raus, die limits kann man drin lassen, aber auch irgendwie setzen
	Ogre::Real  mMass,
		//for testing
	
	

		//for moving:
			
		
			
			//for stuff

		//	currentSpeed,
			//maxSpeed,
			maxAccel,
			distToFloor,

		//for rotating
			rotation_x,
			y_rotation_cont,
			y_limit_a,
			y_limit_b;

	bool floorColliding; //ob die untere hälfte des spielers grad mit dem Boden kollidiert
	//JUMPING STUFF
	bool jump;//ob er springen soll
	bool jumping;//ob er gerade im Sprung ist wird false bei der landung!
	bool playJumpAnim; //ob ich wieder springen kann. wird false beim Abspringen und true nach ablauf der zeit
	bool jumpOnce; //wenn wahr, wird jump nach fertigem sprung auf false gesetzt
	bool jumpStartFinished; //true wenn sprungvorgang abgeschlossen
	bool jumpLanded; //wird wahr, sobald man nach dem Sprung landet. wichtig für den landesound
	Ogre::Real jumpTime; //die Zeit seit Sprungbeginn
	Ogre::Real jumpLandedTime; //zeit nach der Landung
	Ogre::Real jumpPower; //beschleunigung zum springen



			




	bool
		//movement allowed or not? <== still needed?!
		noMovement,
		
		
		//jumpOnce,//
		playerControlled,
		//on earth or not?
		onEarth,
		//actions like pick up, start talking or stuff
		action,
		//running or not?
		run,
		attacking,
		spellcasting;



	Ogre::Vector3	mSize;
	Ogre::Vector3 currentVel;
	//Ogre::Vector3 movement;			 //forward/backward, left/right

	GameObject *curMeleeObj; //das objekt, was man ATM anschaut


	Ogre::SceneNode *camNode;
	Ogre::SceneNode *meshNode;
	//Ogre::SceneNode *mNode;
	Ogre::Entity *ent;
	Ogre::String charName;

	CharAnimation currentAnim;
	void setRandomAnim(AnimationType animType,bool loop=false);
	CharAnimation getRandomAnimState(AnimationList animList);
	void fillAnimLists();


	void playAttackSound(Ogre::String filename);
	

	
	//erzeugt missiles, wie sie von what verlangt werden. wenn repeat -1 ist, wird der wert aus what->missileData
	//verwendet, sonst die Zahl aus repeat
	void spawnMissile(gamedata_damagedealer *what, bool resetRepeat = false);//Damage dmg);
	void spawnExplosion(gamedata_spell *sp);// ExplosionData data, EffectDataList *eff,Ogre::Real range);//Damage dmg,Ogre::Real range);
	void spawnBeam(gamedata_spell *sp);
	//diese funktiionen sollen nur aus update() heraus aufgerufen werden!
	void updateAnims(Ogre::Real time);
	//prüft nicht nur, was man anschaut, sondern feuert auch due waffen ab
	void updateLookingAt(Ogre::Real time);

	void updateMoveSound(Ogre::Real time);
	void updateSpells(Ogre::Real time);

	//macht einen raycast in blickrichtung mit der Länge maxRange und gibt das erste gefundene objekt oder NULL zurück
	
	

     
};

#endif