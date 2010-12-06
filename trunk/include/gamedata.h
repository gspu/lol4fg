#ifndef __praGameData
#define __praGameData

#include "data.h"

enum GamedataType
	{
		gtNotSet,
		gtDynamic,
		gtContainer,
		gtItem,
		gtSpell,
		gtCharacter,
		gtDoor,
		gtExtra//for gamedata_extra holds just random data
	};


	class gamedata
	{
	public:
        Ogre::String id;
		GamedataType type;
		//deletes things like pointers when the program is closing. should NOT be called in the regular destructor
		virtual void prepareFinalRemoval(){}
	};

	class gamedata_extra: public gamedata
	{
	public:
		int value;//this is to be assigned to any random stuff
	};

    class gamedata_hotkeyable: public gamedata
    {
    public:
        //diese funktion wird aufgerufen, wenn der Spieler den Gegenstand per Hotkey benutzen will.
        //Implementation ist Sache der davon erbenden Klassen.
        //rückgabewert soll dem Spieler sagen, ob die Benutzung erfolgreich war: 
        //true bei erfolg, false wenn zB nicht genug Mana oder gegenstand nicht benutzbar
        virtual bool onHotkey(GameChar *user){return true;}
        //bild fürs inventar...
		Ogre::String inv_image;
        //name        
		Ogre::String name;
        
    };
	//klasse, die Dinge speichert, die potentiell Schaden zufügen können
    class gamedata_damagedealer : public gamedata_hotkeyable
    {
    public:
        /*Ogre::SharedPtr<MissileData> Missile;
        Ogre::SharedPtr<ExplosionData> Explosion;
        Ogre::SharedPtr<BeamData> Beam;*/
		MissileData *Missile;
        ExplosionData *Explosion;
        BeamData *Beam;
		virtual void prepareFinalRemoval();
    };
		
	class gamedata_dynamic: public gamedata
	{
	public:
		gamedata_dynamic()
		{
			type = gtDynamic;
		}
		WorldArtData wData;
		CollisionData cData;
		int flags;
		
		//da ist zeug für "sub-elemente"
		std::vector<gamedata_dynamic> subObjects;
		//das ist wie es zu seinem parent verbunden ist
		joint_data jointData;
	};

    

	class gamedata_container:public gamedata
	{
	public:
		gamedata_container()
		{
			type = gtContainer;
		}
		WorldArtData wData;
		CollisionData cData;
		int flags;
		//ob man es nehmen und ausrüsten kann
		bool carryable;
		//wenn carryable, dann hier das inventarbild
		//Ogre::String inv_image;
		//wie groß das Inventar per default ist
		unsigned int defaultSize;
        //wenn carryable, dann hält es hier nen Pointer auf das Item, was die
        //leere Tasche im Inventar repräsentiert
        gamedata_item *inv_item;//removed during the standard object removal loop
		Ogre::String name;
	};

    enum item_PropertyFlags
	{
		PF_NONE				=   0,
		PF_NODROP			=	1,
		//dämliches wort, aber es heißt, man kann das item mixen
		PF_MIXABLE			=	2,
		//kann per rechtsklick benutzt werden. das muss aber gescriptet werden...
		PF_USABLE			=	4,
		//der plasmastab als fernkampfwaffe wäre ein extra-item, mit diesem flag. sollte evtl "special" o.ä. heißen
		PF_SECONDARY		=	8,
		//legt fest, dass man das item "wegwirft", wenn man damit schießt. dolch zB.
		PF_LOSE_ON_RANGED	=	16,
		//analog zu PF_LOSE_ON_RANGED, nur für nahkampf. ne verwendung fällt mir nicht ein, der vollständigkeit halber
		PF_LOSE_ON_MELEE	=	32,
		//also "essen":
		PF_LOSE_ON_USE		=	64
	};
enum item_EquipFlags
	{
		EF_NULL		=  0,
		EF_ARMOR	=  1,
		EF_SHIELD	=  2,
		EF_MELEE	=  4,
		EF_RANGED	=  8,
		EF_ELSE		= 16,
		EF_BAG		= 32
	};

class gamedata_item:public gamedata_damagedealer
	{
	public:
		gamedata_item()
		{
			
			secondaryType = EF_NULL;
			equipFlags	= EF_NULL;
			propertyFlags= PF_NONE;
			//these should be deleted explicitely through prepareFinalRemoval()
			Explosion = NULL;
			Missile = NULL;
			Beam = NULL;
			meleeCooldown = 1;
			rangedCooldown = 1;
			//these are inside the default gameDataMap
			secondaryItemPtr = NULL;
			meleeSpell = NULL;
			rangedSpell = NULL;
			equipSpell = NULL;
			useSpell = NULL;
            Container = NULL;
			type = gtItem;
		}
		~gamedata_item()
		{
		
		}
        bool onHotkey(GameChar *user);//{return true;}
		bool onUse(GameChar *user);
		WorldArtData wData;
		CollisionData cData;
		//hier beginnen die item-eigenschaften
		
		//die flags legen fest, wo man das item equippen kann. auch mehrfachbelegung möglich
		unsigned int equipFlags;
		//diese flags legen anderes zeug fest, zB ob mans mixen oder essen kann
		unsigned int propertyFlags;
		//schaden, den das ding als melee oder fernwaffe anrichtet
		Damage meleeDamage;
		Damage rangedDamage;	
		
		//feuer-sounds
		Ogre::String meleeSound;
		Ogre::String rangedSound;
		Ogre::String useSound;
		
		//cooldown
		Ogre::Real meleeCooldown;
		Ogre::Real rangedCooldown;

		//effekte. sind ab jetzt spells
		gamedata_spell *meleeSpell;
		gamedata_spell *rangedSpell;
		gamedata_spell *equipSpell;
		gamedata_spell *useSpell;

		StatChangeList equipStats;
		
		
		//"secondary" das ist für sowas wie der Plasmastab, der hätte primär melee und sekundär ranged
		//der secondaryType legt fest, wo das sekundäritem erscheint (oder ist 0,
		//wenn kein sekundäritem verwendet), secondaryType ist
		//die ID des items, was dort verwendet wird. dieses Item muss in seinen
		//propertyFlags PF_SECONDARY an haben
		item_EquipFlags secondaryType;
		Ogre::String secondaryItem;
		//beim einlesen der daten wird versucht, den pointer zu holen. wenn es mißlingt, wird es nochmal versucht,
		//wenn es zum ersten mal gebraucht wird
		gamedata_item *secondaryItemPtr;

		/*MissileData *Missile;
		ExplosionData *Explosion;*/
		/*Ogre::SharedPtr<MissileData> Missile;
		Ogre::SharedPtr<ExplosionData> Explosion;*/

        //wenn das das Item zu einem Container ist, ist hier der Pointer auf den Container
        gamedata_container *Container;

	};

    
	
	class gamedata_spell: public gamedata_damagedealer
	{
	public:
		gamedata_spell()
		{
			
			//spellEffectType = seNone;
			spellHitType = shNone;
			cooldown = 1;
			Explosion = NULL;
			Missile = NULL;
			Beam = NULL;
			range = 1;
			sound = "";
			manaCost = 0;
			ancient = false;
			duration = 0;
			type = gtSpell;
		}
		/*MissileData *Missile;
		ExplosionData *Explosion;*/
		/*Ogre::SharedPtr<MissileData> Missile;
		Ogre::SharedPtr<ExplosionData> Explosion;*/
		SpellHitType spellHitType;
		//SpellEffectData effectData;
		EffectDataList effects;
		//Damage dmg;
		Ogre::Real cooldown;
		Ogre::Real range;
		Ogre::Real duration;
		Ogre::String sound;
		Ogre::Real manaCost;
		bool ancient;
        bool onHotkey(GameChar *user);
	};

    
	class gamedata_char:public gamedata
	{
	public:
		gamedata_char()
		{
			name = "";
			head_position = Ogre::Vector3::ZERO;
			flags = 0;
			defaultInventory = NULL;
			defaultSpellbook = NULL;
			defaultMelee = NULL;
			defaultRanged = NULL;
			defaultArmor = NULL;
			defaultShield = NULL;
			defaultElse1 = NULL;
			defaultElse2 = NULL;
			defaultElse3 = NULL;
			defaultElse4 = NULL;
			defaultBag = NULL; 

			stepSound = STEPSOUND_DEFAULT;
			jumpSound = STEPSOUND_DEFAULT;
			landSound = STEPSOUND_DEFAULT;
			stepSoundWaitWalk = 1;
			stepSoundWaitRun = 1.0f / 3.0f;
			movAccel = 100;
			type = gtCharacter;
		}

		virtual void prepareFinalRemoval();
		
		Ogre::String name;
		WorldArtData wData;
		//stattdessen wird es aus kollision verwendet
		//Ogre::Vector3 size;
		Ogre::Vector3 head_position;
		/*Ogre::Quaternion mesh_orientation_offset;
		Ogre::Real mass;*/
		CollisionData cData;
		/*Ogre::Real walk_factor;
		Ogre::Real run_factor;*/
		AnimDataList anim_idle; 
		AnimDataList anim_walk;
		AnimDataList anim_death;
		AnimDataList anim_attackmelee;
		AnimDataList anim_attackranged;
		AnimDataList anim_jump;
		AnimDataList anim_run;
		AnimDataList anim_fall;
		AnimDataList anim_cast;
		int flags;
		Inventory *defaultInventory;
		Spellbook *defaultSpellbook;
		gamedata_item *defaultMelee;
		gamedata_item *defaultRanged;
		gamedata_item *defaultArmor;
		gamedata_item *defaultShield;
		gamedata_item *defaultElse1;
		gamedata_item *defaultElse2;
		gamedata_item *defaultElse3;
		gamedata_item *defaultElse4;
		gamedata_item *defaultBag;

		Ogre::String stepSound;
		Ogre::String jumpSound;
		Ogre::String landSound;
		Ogre::Real stepSoundWaitWalk;
		Ogre::Real stepSoundWaitRun;

		Stats stats;
		Ogre::Real movAccel;
	};

	

	class gamedata_door: public gamedata
	{
	public:
		gamedata_door()
		{
			type = gtDoor;
		}
		WorldArtData wData;
		//Ogre::Real mass; //0 if static
		//short collisionType;
		//Ogre::Vector3 collisionParams;
		CollisionData cData;
		int flags;
		Ogre::String name;
		bool useOnCollision;
	};


#endif