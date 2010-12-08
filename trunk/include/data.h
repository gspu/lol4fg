#ifndef praData
#define praData
#include "defines.h"
#include "fwdec.h"
#include "Ogre.h"

/*TODO: 
LevelData <- contains all stuff from a level, so no need to parse the xml again
CompoundData(?) <- like a parsed dotscene or stuff
*/

typedef std::list<SpellObject*> SpellList;

//paar FWDECS
struct MissileData;
struct ExplosionData;
struct BeamData;

//anz. der items, die man equippen kann
#define EQUIP_CNT 9
/*indices für das equipped-array*/
enum EquipType
{
	eqMelee		= 0,
	eqArmor		= 1,
	eqRanged	= 2,
	eqShield	= 3,
	eqElse1		= 4,
	eqElse2		= 5,
	eqElse3		= 6,
	eqElse4		= 7,
	eqBag		= 8
};

//type of the sound, primarily for adjusting gain
enum SoundType
{
	//in-game music
	stMusic			= 1,
	//NPCs talking
	stSpeech		= 2,
	//level ambient sounds
	stSfxLevel		= 4,
	//effects like click sounds
	stSfxPlayer		= 8,
	//means that it belongs to the level
	stOwnerIsLevel	= 16,
	//means that this is a global source, i.e. for music
	stGlobal		= 32
};
//type of the sound buffer, to selectively delete some
enum BufferType
{
	//anything level-dependant
	btLevel		= 1,
	//for doors 
	//there should be only one such buffer
	btDoor		= 2,
	//anything that should be always there. if a sound is used in both cases, this
	//should be used, since buffers of this type are not deleted on level end
	btGlobal	= 4
	
};

enum ExceptionType
{
	etGenericError = 0,
	etInvalidId = 1,
	etCorruptXml= 2
};



	enum joint_type
	{
		JT_NONE,
		JT_BALL_AND_SOCKET,
		JT_HINGLE,
		JT_SLIDER,
		JT_UNIVERSAL,
		JT_UPVECTOR

	};

	struct joint_data
	{
		joint_type type;
		Ogre::Vector3 pin0;
		Ogre::Vector3 pin1;
		Ogre::Vector3 pos;
	};
	enum collision_type
	{
		CT_NOCOLLISION			= 0,
		CT_BOX					= 1,
		CT_CAPSULE				= 2,
		CT_CHAMFERCYLINDER		= 3,
		//compound collision to be implemented later 
		CT_CONE					= 4,
		CT_CONVEXHULL			= 5,
		CT_CYLINDER				= 6,
		CT_ELLIPSOID			= 7,
		CT_PYRAMID				= 8,
		CT_TREECOLLISION		= 9,
		//CT_TREECOLLISIONSCENE	= 10,
		CT_COMPOUND				= 10,
		CT_EXTERNAL				= 11
	};
	

	class CollisionData
	{
	public:
		CollisionData()
		{
			colType = CT_NOCOLLISION;
			collistionParams = Ogre::Vector3::UNIT_SCALE;
			mass = 0;
			collisionFile = "";
			positionOffset = Ogre::Vector3::ZERO;
			orientationOffset = Ogre::Quaternion::IDENTITY;
			upVector = Ogre::Vector3::ZERO;

		}
		//collisionType
		collision_type colType;
		////das ist vA für komplexe systeme
		//joint_type jointType;
		//params
		Ogre::Vector3 collistionParams;
		//masse
		Ogre::Real mass;
		//name einer .collision-datei
		Ogre::String collisionFile;
		//offsets...
		Ogre::Vector3 positionOffset;
		Ogre::Quaternion orientationOffset;
		//das ist nur für compound. ketten werden vom specialobject gehandlet
		std::vector<CollisionData> subBodyData;
		//ist zwar strenggenommen ein joint, aber hier macht es imho am meisten sinn
		Ogre::Vector3 upVector;
	};

	enum WorldArtType
	{
		WT_NONE,
		WT_MESH,
		WT_BILLBOARD,
		WT_DOTSCENE,
		WT_PLANE,
		WT_PARTICLESYSTEM,
		WT_BILLBOARDCHAIN,
		WT_RIBBONTRAIL
	};

	

	struct billboardChainData
	{
		billboardChainData()
		{
			maxElements=20;
			numberOfChains=1;
			useTextureCoords=true;
			useColours=true;
			dynamic=true;
		}
		size_t maxElements;//=20, 
		size_t numberOfChains;//=1, 
		bool useTextureCoords;//=true, 
		bool useColours;//=true, 
		bool dynamic;//=true)
	};

    
	struct PlaneData
	{
		PlaneData()
		{
			normal = Ogre::Vector3::NEGATIVE_UNIT_Z;
			constant = 0;
			width = 1;
			height = 1;
			xSeg = 1;
			ySeg = 1;
			numTexCoordSets = 1;
			uTile = 1;
			vTile = 1;
			up = Ogre::Vector3::UNIT_Y;
		}
		//normale der ebene
		Ogre::Vector3 normal;
		//abstand, an dem sie entlang der normalen bewegt wird
		Ogre::Real constant;
		Ogre::Real width;
		Ogre::Real height;
		int xSeg;
		int ySeg;
		int numTexCoordSets;
		Ogre::Real uTile;
		Ogre::Real vTile;
		Ogre::Vector3 up;
	};

	struct FluidData
    {
		FluidData(Ogre::String _material ="WaterMaterial", Ogre::String _sound="waterSPLASH.wav",Ogre::Real _density = 0.7, Ogre::Real _linearViscosity = 0.5, Ogre::Real _angularViscosity = 0.5)
		{
			 density = _density;
			 linearViscosity = _linearViscosity;
			 angularViscosity = _angularViscosity;
			 material= _material;
			 sound = _sound;
		}
        Ogre::Real density;
        Ogre::Real linearViscosity;
        Ogre::Real angularViscosity;
        Ogre::String material;
		Ogre::String sound;
		PlaneData plane;
    };


	struct WorldArtData
	{
		WorldArtData()
		{
			type = WT_NONE;
			filename = "";
			material = "";
			scale = Ogre::Vector3::UNIT_SCALE;
			positionOffset = Ogre::Vector3::ZERO;
			orientationOffset = Ogre::Quaternion::IDENTITY;
			bbColor = Ogre::ColourValue::White;
			shadow = true;
			loopAnim = "";

		}
		WorldArtType type;
		Ogre::String filename;
		Ogre::String material;
		Ogre::Vector3 scale;
		Ogre::Vector3 positionOffset;
		Ogre::Quaternion orientationOffset;
		Ogre::ColourValue bbColor;
		PlaneData pData;
		billboardChainData bbcData;
		bool shadow;
		Ogre::String loopAnim;
		
	};

	

	//test
	

	
	class Damage
	{
	public:
		Damage(int _hitBlunt = 0, int _hitCut = 0, int _hitPierce = 0, int _fire = 0, int _ice = 0,
		int _spark = 0, int _poison = 0, int _dark = 0, int _light = 0)
		{
			hitBlunt	= _hitBlunt;
			hitCut		= _hitCut;
			hitPierce	= _hitPierce;
			fire		= _fire;
			ice			= _ice;
			spark		= _spark;
			poison		= _poison;
			dark		= _dark;
			light		= _light;
		}
		int hitBlunt;
		int hitCut;
		int hitPierce;
		int fire;
		int ice;
		int spark;
		int poison;
		int dark;
		int light;

		int operator [] ( const size_t i )
		{
			assert( i < 9 );

			return *(&hitBlunt+i);
		}
	};



	enum StatOperator
	{
		opNone		= 0,
		opAdd			= 1,
		opMultiply	= 2,
		opSet			= 3
	};

	enum StatType
	{
		st_strength			= 0, 
		st_agility			= 1,
		st_magic			= 2,
		st_maxHP			= 3,
		st_maxMP			= 4,	
		st_resistBlunt		= 5,	
		st_resistCut		= 6,		
		st_resistPierce		= 7,	
		st_resistFire		= 8,	
		st_resistIce		= 9,		
		st_resistSpark		= 10,
		st_resistPoison		= 11,
		st_resistDark		= 12,
		st_resistLight		= 13,
		st_hpRegenRate		= 14,
		st_manaRegenRate	= 15,
		st_walkSpeed		= 16,
		st_runSpeed			= 17

	};
#define STAT_CNT 18

	

	class Stats
	{
	public:
		
		Stats()
		{
			hitpoints	= 100;
			mana		= 100;
			ancientMagic =0;

			mStats[st_strength]		= 1; 
			mStats[st_agility]		= 1;
			mStats[st_magic]		= 1;
			mStats[st_maxHP]		= 100;
			mStats[st_maxMP]		= 100;
			
			mStats[st_resistBlunt]	= 1;	// 0 = resistenz, 
			mStats[st_resistCut]	= 1;		//0.5 = halber schaden, 
			mStats[st_resistPierce]	= 1;	//1 = ganzer schaden, 
			mStats[st_resistFire]	= 1;	//2 = doppelter, 
			mStats[st_resistIce]	= 1;		//-1 = heileffekt
			mStats[st_resistSpark]	= 1;
			mStats[st_resistPoison]	= 1;
			mStats[st_resistDark]	= 1;
			mStats[st_resistLight]	= 1;
			mStats[st_hpRegenRate]	= 1;
			mStats[st_manaRegenRate]= 1;
			mStats[st_walkSpeed]	= 1;
			mStats[st_runSpeed]		= 3;
			
		}
		inline Stats& operator =(const Stats& other)
		{
			hitpoints = other.hitpoints; 
			mana = other.mana;
			ancientMagic = other.ancientMagic;
			for(int i=0;i<STAT_CNT;i++)
			{
				mStats[i] = other.mStats[i];
			}
			return *this;
		}
		Ogre::Real mStats[STAT_CNT];
		
		
		//HP und mana sind Ogre::Real, damit man auch Bruchteile addieren kann, wichtig für regeneration
		Ogre::Real hitpoints; 
		Ogre::Real mana;

		//ahnenmagie...
		int ancientMagic;

		

	
	};

	//jetzt folgendermaßen: 
	/*nur ein stat wird pro struct geändert. add hat vorrang vor multiply, und set überschreibt alles*/
	/*GameChar hat dann insges. 4 Stats: BaseStats von denen alles ausgeht, AddStats wie sie nach addition aussehen,
	MultStats wie nach multiplikation, und SetStats sind die finalen*/
	struct ChStat 
	{
		StatOperator op;
		Ogre::Real value;
		StatType type;
		ChStat inverse()
		{
			ChStat res = *this;
			if(op == opMultiply && value != 0)
				res.value = float(1.0f / res.value);
			else
				res.value *= -1;
			return res;
		}
	};

	typedef std::vector<ChStat> StatChangeList;

	//explosionen, oder einfach kurze anims o.ä.
	struct ExplosionData
	{
		ExplosionData()
		{
			lifespan = -1;
			sound = "";
		}
		bool isEmpty()
		{
			if(lifespan == -1 || (lifespan == 0 && sound == ""))
				return true;
			return false;
		}
		//ok. wenn lifespan = -1, dann ist dieser struct ungültig.
		//wenn es 0 ist, dann wird auf den sound gewartet
		Ogre::Real lifespan; //wie lange es dauert, bis der effekt entfernt wird. gilt nur, 
		WorldArtData wData; //ist klar
//		Ogre::String animToPlay; //wenn mesh, dann kann eine animation gespielt werden. "" = gar nicht
//		unsigned int animToRepeat; //wie oft animation gespielt werden soll. 0=loop  <- IMHO gehört das nach WorldArtData
		Ogre::String sound; //der sound halt
		bool loopSound;
	};

	

	struct MissileData
	{
		WorldArtData wData;
		CollisionData cData;
		//ExplosionData eData;
		//speed the missile should move with
		Ogre::Real velocity;
		//fluggeräusch
		Ogre::String sound;
		
		//anzahl der missiles, die auf einmal gespawned werden
		unsigned int count;
		//time before the next missile should be spawned
		Ogre::Real multimissile_delay;
		//lol warum schreib ich englisch? der winkel zwischen den einzelnen missiles
		//wird so berechnet, dass der mittlere geradeaus fliegt. vorerst nur XZ-Ebene
		Ogre::Degree multimissile_angle;
		//anzahl der wiederholungen, wenn multimissile_delay != 0 ist
		unsigned int repeat;
	};

	
	struct BeamData
	{
		//das material eben. wird auf das manual object gelegt
		Ogre::String material;
		////die Größe der Tiles, aus denen das Ogre::ManualObject aufgebaut ist.
		////x ist die Breite, y die Höhe (Höhe = in Strahlrichtung)
		////die Werte beeinflussen die Materialskalierung!
		//Ogre::Vector2 tileDimensions;
        //die Breite des BillboardChains
        Ogre::Real width;
        //das sagt im Grunde aus, wie viele ChainElements erstellt werden
        //ein kleiner texCoord wert bedeutet, dass viele Zwischenelemente 
        //mit kleinen unterschieden in texturkoordinaten erstellt werden
        //ein hoher erzeugt wenige mit viel "abstand" dazwischen
        Ogre::Real texCoord;
		//der Sound, während der Strahl aktiv ist
		Ogre::String sound;
		//lebenszeit
		Ogre::Real lifespan;
        //Reichweite und damit max. länge des Strahls 
        Ogre::Real maxRange;
        //Die Farbe für die chainElems
        Ogre::ColourValue color;
        //das legt fest, ab wann (in Elements) begonnen wird, den Strahl transparent zu machen
        //
        int opacityStart;
	};

	//WIE kommt der Effekt zum Ziel?
	enum SpellHitType
	{
		shNone,
		shOnSelf,
		shMissile,
		shExplosion,
		shMissileRain,
		shBeam,
		shScript
	};

	//welcher effekt soll es genau sein?
	enum SpellEffectType
	{
		seNone,
		//normaler schaden
		seDamage,
		//stats setzen. brauche konstante, damit man einige stats als nicht setzbar markieren kann
		seModStats,
		//absorbieren. erstmal beides
		seAbsorb,
		//scripted
		seScript
	};
	

	struct SpellEffectData
	{
		SpellEffectData()
		{
			type = seNone;
			splashRadius = 0;
		}
		~SpellEffectData()
		{
			/*if(explosion)
				delete explosion;*/
		}
		SpellEffectType type;
		Damage dmg;
		//Stats_Item stat;
		StatChangeList stats;
		Ogre::Real splashRadius; //erstmal linear
	};

	typedef std::vector<SpellEffectData> EffectDataList;

	
	
    
   
	struct AnimData
	{
		AnimData()
		{
			animName = "";
			factor = 1;
		}
		Ogre::String animName;
		Ogre::Real factor;
	};

	typedef std::vector<AnimData> AnimDataList;

	
	
	enum ObjType
	{
		otInvalid, //für alle Fälle
		otDynamic,
		otItem,
		otChar,
		otDoor,
		otLight,
		otEntrance, //for editor only
		//otDotScene, //dotscene
		//otStatic, //einzelne mesh 
		otStatic, //static-objekt
		otMissile, //projektil, feuerball zB
		otContainer, //behälter
		otGroup, //gruppenobjekt
		otExplosion, //spezialeffekt
		otDeleted, //für SPielstände
		otWater,		//wasserebene
		otBeam	//strahl, wie Blitzschlag oder Sonnenstrahl
	};
	//türeninformationen, die pro tür gespeichert werden müssen
	struct DoorData
	{
		//filename of the level to teleport to
		Ogre::String targetLevel;
		//name of the entrance in the target LVL
		Ogre::String targetEntrance;
	};

    //diese Klasse enthält Informationen einer .scene datei
    //(und warscheinlich auch nen parser)    
    class DotSceneData
    {
    public:
        struct NodeData;
        struct EntityData;
        typedef std::vector<NodeData> NodeVector;
        typedef std::vector<EntityData> EntityVector;
        struct NodeData
        {
            Ogre::Vector3 position;
            Ogre::Quaternion orientation;
            Ogre::Vector3 scale;
            Ogre::String name;
            NodeVector subNodes;
            EntityVector entities;
        };
        struct EntityData
        {
            WorldArtType type;
		    Ogre::String filename;
		    Ogre::String material;		    
		    Ogre::ColourValue bbColor;
		    PlaneData pData;
		    billboardChainData bbcData;
		    bool shadow;
        };
        NodeVector nodes;
        
    };
	


	
	
	

	
#endif