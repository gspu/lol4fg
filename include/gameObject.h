#ifndef __praGameObjectWTF
#define __praGameObjectWTF
#include "Ogre.h"
#include "OgreNewt.h"
#include "defines.h"
#include "FwDec.h"
#include "WorldArt.h"
#include "data.h"
#include "Sound.h"
#include "gamedata.h"
/********************************************************************************************************* /
			#####						   #########			         #####
		   #######						  ##  ###  ## 		            #######
		   #######						  ##### #####		            #######
		   #######						   ###   ###  		            #######
			#####						   #########                     #####
			#####						   # # # # #                     #####
			 ###					   #                #                 ###  
			 ###					  ####            ####                ###
			  #							 ####      ####                    #
			   								########
			 ###						 ####      ####                   ###
		    #####					   ####           ####               #####
			 ###						#               #                 ###

UNTER GAR KEINEN UMSTÄNDEN die functions.h hier includen!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
Der Code wird dadurch UNKOMPILIERBAR!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

grund unbekannt, wird als pöhser code angesehen

// /* //#include "functions.h" */
/*********************************************************************************************************/

//#include "Level.h"
//#include "GameApp.h" //das hier evtl in die CPP only



//main class for all objects
//should be inherited
class GameObject
{
	friend class WorldArt;
public:
	

	
    GameObject();//standard konstruktor, der zeug initialisiert
	
	

	//virtual GameObject()
	////standard destructor
	virtual ~GameObject();//{} 
	//update movement/anim/etc
	virtual void update(Ogre::Real time); 
	//sets position
	virtual void setPosition(Ogre::Vector3 pos);
	//sets orientation
	virtual void setOrientation(Ogre::Quaternion ornt);
	//sets both
	virtual void setPositionOrientation(Ogre::Vector3 pos,Ogre::Quaternion ornt);
	//retrieves body position
	virtual Ogre::Vector3 getPosition();
	//retr. body orient
	virtual Ogre::Quaternion getOrientation();
	//retr. both (false if fails)
	virtual bool getPositionOrientation( Ogre::Vector3& pos, Ogre::Quaternion& orient );

    //gibt den Pointer des Objekts als Ogre::String (für unique namen zB)
    Ogre::String ptrAsStr()
    {
        long address = reinterpret_cast<long>(this);
	    char buffer[33];
	    //int base = 16;
	    ltoa(address,buffer,16);
	    return Ogre::String(buffer);
    }

	Ogre::String getDebugInfo();

	//name für eine anzeige oder so
	virtual Ogre::String getDisplayName()
	{
		return ID;
	}

	inline virtual void setScale(Ogre::Vector3 scale)
	{
		mArt->setScale(baseScale*scale);
		updateBody();
	}
	inline virtual void modScale(Ogre::Vector3 scale)
	{
		mArt->modScale(baseScale*scale);
		updateBody();
	}
	inline virtual Ogre::Vector3 getScale()
	{
		if(mArt)
			return mArt->getScale()/baseScale;
		else
			return Ogre::Vector3::UNIT_SCALE;
	}
	
	virtual OgreNewt::Body *getBody();
	virtual Ogre::SceneNode *getSceneNode();
	//pointer to the level this obj is currently in
	virtual Level *getLevel();
	virtual ObjType getType()
	{
		return type;
	}
	virtual WorldArt *getWorldArt()
	{
		return mArt;
	}
	virtual Ogre::String getID()
	{
		return ID;
	}
	virtual Ogre::String getName()
	{
		return "";
	}
    virtual WaterPlane *getCollidingWater()
    {
        return mWater;
    }
    virtual bool isOnWaterSurface();
    virtual bool isInWater();
    virtual void collisionWithWater(WaterPlane *water);
    
    virtual bool buoyancyCallback( int colID, OgreNewt::Body* me, const Ogre::Quaternion& orient, const Ogre::Vector3& pos, Ogre::Plane& plane );
    virtual void forceCallback( OgreNewt::Body* me );

	virtual void updateBody();
	virtual void markSelected(bool mark)
	{
		if(mArt)
			mArt->markSelected(mark);
	}

	virtual void rotate(const Ogre::Quaternion& q, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_LOCAL);
	virtual void rotate(const Ogre::Vector3& axis, const Ogre::Radian& angle, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_LOCAL);

	virtual GameObject* clone(Level *targetLvl = NULL) = 0;

	virtual void onCollision(GameObject *with, ContCallback *callback){};
	
	virtual void onActivate(GameChar *user, bool cursorMode = false){};
	virtual void onDamage(Damage dmg,GameObject *attacker = NULL){};

	virtual Ogre::AxisAlignedBox getCollisionAABB()
	{
		return collisionAABB;
	}
	virtual TiXmlElement* getXmlElem(Ogre::Vector3 startPosition = Ogre::Vector3::ZERO,bool savegame=false);
	virtual bool isStatic()
	{
			switch(type)
			{
			case otInvalid:
			case otLight:
			case otEntrance: 
			case otStatic:
			case otGroup:
			case otDoor:
            case otWater:
				return true;
			}
			return false;

	}
	virtual gamedata* getData()
	{
		return mData;
	}

	virtual	void AddSpell(gamedata_spell *spell,GameObject *caster){}
	
	
	virtual	void RemoveSpell(gamedata_spell *spell){}
	virtual	void RemoveSpell(SpellObject *spell){}
	int getSGID()
	{
		return SGID;
	}
	void setSGID(int set)
	{
		SGID = set;
	}
    virtual SimpleSound::Source *getContactSound()
    {
        return contactSound;
    }
    virtual void playContactSound(Ogre::String filename);
	virtual bool getContactSoundPlaying();
	//ist true, wenn das objekt in der original-leveldatei existiert
	//das gilt zB nicht für Items, die der Spieler im Level abgelegt hat
	bool isInLevelFile;
	//gameChar verwendet die methode, um das Ergebnis von isOnWaterSurface zwischenzuspeichern
   // bool onWaterSurface;
	//wird im forcecallback um 1 verringert, bis es 0 ist, bei collisionWithWater auf 2 gesetzt
	//dh 2=in diesem frame wasserkolision, 1=im letzten frame, 0=gar nicht
	short onWaterSurfaceState; 
protected:
    SimpleSound::Source *contactSound;
	WaterPlane *mWater;
    bool swimming;
    
	//ZipSaveFile ID
	int SGID;
	gamedata* mData;
	WorldArt *mArt;

	CollisionData mColData;

	Ogre::Vector3 baseScale; //scale, die in der objdefinition steht
	/*Ogre::Vector3 collisionParams;
	short colType;*/

	OgreNewt::Body *mBody;
	Ogre::SceneNode *mNode;
	Level *mLevel;
	ObjType type;
	/*Ogre::String uName;*/
	Ogre::String ID;

	//void createWorldArt(WorldArtData d,Ogre::SceneNode *nod = NULL);
	void createBody(CollisionData d,bool mustHaveBody = true);
	OgreNewt::CollisionPtr createCollision(CollisionData d, Ogre::Vector3 &inertia);
	Ogre::AxisAlignedBox collisionAABB;

	OgreNewt::Joint *createJoint(OgreNewt::Body *parent,OgreNewt::Body *child,joint_data data);


};

//paar typedefs
typedef std::vector<GameObject*> ObjectVector;
typedef std::list<GameObject*> ObjectList;


#endif