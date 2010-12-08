#ifndef __praLevel
#define __praLevel

/*
Klasse für alles rund ums level
*/

#include <Ogre.h>
#include <OgreNewt.h>
//#include "tinyxml.h"
#include <tinyxml.h>
#include "FwDec.h"
#include "defines.h"
//#include "GameObject.h"
//#include "OgrePagingLandScapeListenerManager.h"
#include "StandardApplication.h"
#include <map>
#include "vector2d.h"
//#include <boost/multi_array.hpp>
//#include "GameObject.h"
#include "SoundManager.h"
#include "Sound.h"

//#include "ETTerrainManager.h"
//#include "ETTerrainInfo.h"
//#include "ETBrush.h"
//#include "ETSplattingManager.h"
#include "ZipSaveFile.h"
#include <OgreTerrain.h>
#include <OgreTerrainGroup.h>
#include <OgreTerrainPaging.h>


using namespace SimpleSound;

class Level
{
public:
#ifdef __editor
	friend class EditorApp;
#else
	friend class GameApp;
#endif
	
	//typedef gamedata_char gamedata_char;
	//typedef gamedata_item gamedata_item;
	//typedef gamedata_obj_static gamedata_obj_static;

	//struct obj_container
	//{
	//	Ogre::String ObjId;
	//	OgreNewt::Body *NewtonBody;
	//	void *ptr;
	//	short type;
	//};
	//for storing entrances
//	typedef ObjType ObjType;
	struct Page
	{
		Page()
		{
			x=0;
			z=0;
			loaded=false;
		}
		int x;
		int z;
		bool loaded;
	};



	struct OrientedPoint
	{
		Ogre::Vector3 pos;
		Ogre::Quaternion orient;
	};
	enum LevelType
	{
		lvtGeneric,
		//lvtBSP,
//		lvtPaginglandscape
	};
	struct TerrainData
	{
		//this is the size of one "terrain" object alongside it's edge, in vertices
		//must be 2^n+1
		Ogre::uint16 terrainSize;
		//either the size of one page, or the size of all pages together. further testing needed
		Ogre::Real worldSize;
	};
	TerrainData mTerrainData;

	
	typedef std::map<int,GameObject*> SaveableObjectList;
	typedef std::map<int,TiXmlElement*> SaveableXmlList;

	SaveableXmlList preloadedObjects;


	

	ObjectList mObjects;
	ObjectList objectsToDelete;
	SaveableObjectList saveableObjects;

	GameObject *getObjectBySGID(int sgid);
	
	/*std::vector<GameObject*> mObjects;
	std::vector<GameObject*> objectsToDelete;
	std::vector<GameObject*> objectsToRegister;*/

	//this is for chars only (for update)
	//std::vector<GameChar*> charVector;
	//stellen, wo man das level betreten kann. DoorMarker in morrowind
	std::map<Ogre::String, OrientedPoint> entrances;
	//sav ist das SG zum daraus laden, wenn es NULL ist, dann wird es nicht benutzt
	Level(Ogre::String filename,ZipSaveFile *sav = NULL);
	~Level();
	//saves this entire level to XML file
	void saveToFileOld(Ogre::String filename); 
	/*beim savegamemodus werden keine statics gespeichert, 
	wenn SGIDs = true, werden die SGIDs mitgespeichert*/
	Ogre::String getSavegameXML();
	void saveToFile(Ogre::String filename);
	//adding time to this level. it takes care of newton, anims etc itself
	void update(Ogre::Real time);
	//das kümmert sich um das "löschen im nächsten Frame"
	void updateDelete();
	bool getDestructorCalled()
	{
		return destructorCalled;
	}
	void placePlayer(Ogre::Vector3 pos,Ogre::Quaternion ornt = Ogre::Quaternion::IDENTITY,GameChar *oldPlayer = NULL);
	void placePlayer(Ogre::String entranceName,GameChar *oldPlayer = NULL);
	//place something
	/*void placeObject(int objectType,Ogre::String objectID,Ogre::Vector3 pos,Ogre::Quaternion ornt,
							  Ogre::Vector3 modScale,DoorData mData);
	void placeObject(int objectType,Ogre::String objectID,Ogre::Vector3 pos,Ogre::Quaternion ornt = Ogre::Quaternion::IDENTITY,
		Ogre::Vector3 modScale = Ogre::Vector3::UNIT_SCALE);*/

	//creates an object using type and objectID
	//provides some additional functions, like, placing statics
	GameObject* placeObject(ObjType objectType,Ogre::String objectID,Ogre::Vector3 pos,Ogre::Quaternion ornt = Ogre::Quaternion::IDENTITY,
		Ogre::Vector3 modScale = Ogre::Vector3::UNIT_SCALE,WorldArtType staticType = WT_NONE);

	//places an object using it's gamedata
	GameObject* placeObject(gamedata *data,Ogre::Vector3 pos,Ogre::Quaternion ornt = Ogre::Quaternion::IDENTITY,
		Ogre::Vector3 modScale = Ogre::Vector3::UNIT_SCALE);
	//same, but by ID
	GameObject* placeObject(Ogre::String id,Ogre::Vector3 pos,Ogre::Quaternion ornt = Ogre::Quaternion::IDENTITY,
		Ogre::Vector3 modScale = Ogre::Vector3::UNIT_SCALE);

	//wenn bool fromLevelFile=true, wird bei allen geladenen Objekten isInLevelFile auf true gesetzt
	void loadObjects(TiXmlElement *elem, Ogre::Vector3 positionOffset = Ogre::Vector3::ZERO,bool fromLevelFile = true);
	//lädt ein einzelnes objekt
	void loadObject(TiXmlElement *elem, Ogre::Vector3 positionOffset = Ogre::Vector3::ZERO,bool fromLevelFile = true);
	void startObjectListener()
	{
		objectListenerUse = true;
		objectListenerList.clear();
	}
	ObjectList finishObjectListener()
	{
		objectListenerUse = false;
		return objectListenerList;
	}
	//überladen... irgendwie
	/*void removeObject(Ogre::SceneNode *sNode);
	void removeObject(Ogre::String nodeName);
	void removeObject(GameObject *obj);*/
	inline Ogre::Real getGravity()
	{
		return gravity;
	}

	inline Ogre::Camera *getMainCam()
	{
		return mCam;
	}

	inline OgreNewt::World *getWorld()
	{
		return mWorld;
	}
	inline Ogre::SceneManager *getSceneManager()
	{
		return mSceneMgr;
	}

	void showNewtonDebugLines(bool show);

	//static function to retrieve the current one
	static inline Level *getCurrent()
	{
		return StandardApplication::getSingletonPtr()->getCurrentLevel();
	}
	
	

	/*GameObject *getObject(Ogre::String nodeName);
	GameObject *getObject(Ogre::SceneNode *nod);*/

	//!!warnung!!
	//warscheinlich LAAAANGSAM!!!
	GameObject *findObject(Ogre::SceneNode *nod);

	inline GameChar *getPlayer()
	{
		return player;
	}
	void setPlayerControl(GameChar *target);
	void restorePlayerControl();

	inline Ogre::String getFileName()
	{
		return mFileName;
	}
    inline Ogre::String getBaseFileName()
    {
        Ogre::String base, ext;
        Ogre::StringUtil::splitBaseFilename(mFileName,base,ext);
        return base;
    }

	inline Ogre::String getName()
	{
		return mName;
	}

	

	Ogre::String getUniqueEntranceName(Ogre::String base="entr");
	bool isEntranceNameUnique(Ogre::String name);
/*skyPlaneDist = 3000;
	skyBoxDist   = 5000;
	skyDomeDist  = 4000;*/
	void setSkyBox(Ogre::String material, Ogre::Quaternion orientation=Ogre::Quaternion::IDENTITY,Ogre::Real distance = 5000);
	void removeSkyBox();
	void setSkyDome(Ogre::String material, Ogre::Real curvature=10, Ogre::Real tiling=8, int xseg=16,int yseg=16, Ogre::Quaternion orientation = Ogre::Quaternion::IDENTITY, Ogre::Real distance = 4000);
	void removeSkyDome();
	void setSkyPlane(Ogre::String material, Ogre::Real scale=1000, Ogre::Real tiling=10, Ogre::Real bow=0, int xseg=1,int yseg=1, Ogre::Real distance = 3000);
	void removeSkyPlane();

	void registerObject(GameObject *obj);
	void unregisterObject(GameObject *obj);

	void LevelForceCallback( OgreNewt::Body* bod );

    
	//void tileLoaded(PagingLandscapeEvent* e);
	//void tileUnLoaded(PagingLandscapeEvent* e);

	//void pageLoaded(PagingLandscapeEvent *e);
	//void pageUnLoaded(PagingLandscapeEvent *e);
	/*Aufräumfunktion, die das Löschen des Levels vorbereitet,
	unter anderem werden Leveleigene Buffer und PLSM-Callbacks gelöscht.
	Objekte bleiben vorerst erhalten, sie werden erst durch den richtigen Destruktor
	gelöscht.
	nachdem diese Funktion aufgerufen wurde, bricht Level::update()
	sofort nach dem Aufrufen ab.
	Wird keepBuffers auf true gesetzt, werden die Buffer beibehalten, zB für den Fall, dass
	das gleiche Level nochmal geladen wird*/
	void prepareForDestruction(bool keepBuffers = false);

	void destroyAllObjects();

	inline Ogre::Quaternion getNorthOrientation()
	{
		return northOrientation;
	}

	void enableFog(bool enable)
	{
		if(enable)
		{
			mSceneMgr->setFog(mFogMode,mFogColor,mFogDensity,mFogStart,mFogEnd);
			//StandardApplication::getSingletonPtr()->getRenderWindow()->getViewport(0)->setBackgroundColour(mFogColor);
		}
		else            
            mSceneMgr->setFog(Ogre::FOG_NONE);
	}

	/*void spawnMissile(GameChar *caster,MissileData mData, Ogre::Vector3 pos, Ogre::Quaternion ornt, Damage dmg);*/


	////level aktivieren... hm
	//void setActive();
	////und deaktivieren?
	//void setInactive();
	//naja später irgendwie


	//materialzeug
	OgreNewt::MaterialID 
		*charMaterial,
		*projMaterial,
		*matGlass,
		*matWood,
		*matMetal,
		*matStone; 
	/*const OgreNewt::MaterialID *defaultMaterialID;*/
	void deleteObject(GameObject *obj);

	void debugShowPoint(Ogre::Vector3 pos);
    void debugShowPlane(Ogre::Plane pl);
	void debugShowLine(Ogre::Vector3 p1, Ogre::Vector3 p2);

	Ogre::SceneNode *debugNode;
    Ogre::SceneNode *debugNode2;
	Ogre::SceneNode *debugNode3;

	Buffer *getBuffer(Ogre::String filename);
	Source *createSource(SimpleSound::Buffer *buffer = NULL, int type = 0, bool loop = false, bool relativeToListener = false, bool stream = false);
	void destroySource(Source *src);



    void playSound(Ogre::Vector3 position,Ogre::String filename);

    //Hilfsfunktionen für das neu erzeugen von Covmaps
    Ogre::PixelFormat numChannelsToPixelFormat(int channels)
    {
        switch (channels)
        {
            case 1: return Ogre::PF_BYTE_A;
            case 2: return Ogre::PF_BYTE_LA;
            case 3: return Ogre::PF_BYTE_RGB;
            case 4: return Ogre::PF_BYTE_RGBA;
            case -1: return Ogre::PF_BYTE_A;
            case -2: return Ogre::PF_BYTE_LA;
            case -3: return Ogre::PF_BYTE_BGR;
            case -4: return Ogre::PF_BYTE_BGRA;
            default: return Ogre::PF_UNKNOWN;
        }
    }
    int pixelFormatToNumChannels(Ogre::PixelFormat format)
    {
        switch (format)
        {
            case Ogre::PF_BYTE_A: return 1;
            case Ogre::PF_BYTE_LA: return 2;
            case Ogre::PF_BYTE_RGB: return 3;
            case Ogre::PF_BYTE_RGBA: return 4;
            /*case PF_BYTE_A: return -1;
            case PF_BYTE_LA: return -2;
            case PF_BYTE_BGR: return -3;
            case PF_BYTE_BGRA: return -4;*/
            default: return 0;
        }
    }
    int covmapNrFromTextureIndex(int texture,int nrOfChannels)
    {
        return Ogre::Math::IFloor(float(texture)/float(nrOfChannels));
    }
	
    int channelNrFromTextureIndex(int texture,int nrOfChannels)
    {
        return texture % nrOfChannels;
    }
    int getOldTextureIndex(Ogre::String name)
    {
        for(unsigned int i=0;i<terrainTextures.size();i++)
        {
            if(terrainTextures[i] == name)
                return i;
        }
        return -1;
    }
    void initBuffer(Ogre::uchar *buffer,unsigned int size,Ogre::uchar value=0)
    {
        for(unsigned int i=0;i<size;i++)
        {
            buffer[i]=value;
        }
    }
    //void add
    
    //gibt es holes?
	bool hasHoleMap()
	{
		if(terrainTextures[0] == "hole.png")
			return true;
		return false;
	}
    
	//liste der texuren updaten
    void updateTextureList(Ogre::StringVector newList);
    //diese nimmt jetzt folgendes an:
    //-ambient entspricht der ambienteinstellung des levels
    //-shadowed ist immer true
    //somit müssen nur noch lightDir und etLightColor gespeichert werden. und width und height
	void updateTerrainLightmap();
	//erstellt das Material für das Terrain anhand von ETTerrainMaterialBase
	void createETMaterial();

	//erzeugt anhand von terrainTextures und mSplatMgr->getNumMaps()
	//Pässe für eine technik. sollte nur von createETMaterial aufgerufen werden
	//der pass sollte vertex_program_ref und fragment_program_ref beinhalten
	//erzeugt auch den lightmap pass
	//texturesPerPass ist die anzahl der texturen, ohne coverage maps, muss zwischen 0 und 12 sein
	//wenn texturesPerPass = 0, dann ist es die fallback technique
	//es wird als 3 angesehen, und der fallback shader wird benutzt
	//void createETMaterialPasses(Ogre::Technique *tech,unsigned int texturesPerPass);

	//inline Ogre::MaterialPtr getTerrainMaterial()
	//{
	//	return terrainMaterial;
	//}

    void getSkyPlaneParams(Ogre::SceneManager::SkyPlaneGenParameters &genParams,Ogre::String &material, Ogre::Real &distance)
    {
        genParams = skyPlaneParams;
        material = skyBoxMaterial;
        distance = skyPlaneDist;
    }

    void getSkyDomeParams(Ogre::SceneManager::SkyDomeGenParameters &genParams,Ogre::String &material)
    {
        genParams = skyDomeParams;
        material = skyBoxMaterial;
    }

    void getSkyBoxParams(Ogre::SceneManager::SkyBoxGenParameters &genParams,Ogre::String &material)
    {
        genParams = skyBoxParams;
        material = skyBoxMaterial;
    }

    void getHeightmapDimensions(Ogre::uint &width,Ogre::uint &height)
    {
        width = hmWidth;
        height= hmHeight;
    }
    void getLightmapDimensions(Ogre::uint &width,Ogre::uint &height)
    {
        width = lmWidth;
        height= lmHeight;
    }
    void getSplattingDimensions(Ogre::uint &width,Ogre::uint &height)
    {
        width = splatWidth;
        height= splatHeight;
    }

    /*unsigned int getNumSplatChannels()
    {
        return splatChannels;
    }*/
    //entfernt das terrain
    void removeTerrain();
    //erzeugt ein leeres terrain, das mit default.png bedeckt ist
    void addBlankTerrain(unsigned int heightmapWidth,unsigned int heightmapHeight,unsigned int lightmapWidth,unsigned int lightmapHeight,unsigned int splattingWidth,unsigned int splattingHeight);
    //inline Ogre::AxisAlignedBox getTerrainExtends()
    //{
    //    return terrainExtends;
    //}

    //inline Ogre::Vector3 getTerrainScale()
    //{
    //    //return mTerrainInfo->getScaling();
    //    Ogre::Vector3 hmSize(hmWidth-1,1,hmHeight-1);
    //    return terrainExtends.getSize()/hmSize;
    //}
    //inline Ogre::Vector3 getCovmapScale()
    //{
    //    Ogre::Vector3 size(splatWidth,1,splatHeight);
    //    return terrainExtends.getSize()/size;
    //}
    //inline Ogre::Vector3 toTerrainPosition(Ogre::Vector3 ogrePosition)
    //{   

    //    return Ogre::Vector3(mTerrainInfo->posToVertexX(ogrePosition.x),0,mTerrainInfo->posToVertexZ(ogrePosition.z));
    //}
    //inline Ogre::Vector3 toOgrePosition(Ogre::Vector3 terrainPosition,bool useCovmapScale = false)
    //{
    //    if(useCovmapScale)
    //        return terrainPosition * getCovmapScale() + getTerrainExtends().getMinimum();
    //    else
    //        return terrainPosition * getTerrainScale() + getTerrainExtends().getMinimum();
    //    //return Ogre::Vector3(mTerrainInfo->posToVertexX(ogrePosition.x),0,mTerrainInfo->posToVertexZ(ogrePosition.z));
    //}
    //void generateTerrainCollision();
	inline Ogre::SceneNode *getCamHeadNode()
	{
		return CamHeadNode;
	}
	inline Ogre::SceneNode *getCamNode()
	{
		return CamNode;
	}


	/** OGRE TERRAIN/PAGING STUFF **/

	bool hasTerrain()
	{
		return has_terrain;
		//return (mTerrainGroup != NULL);
		//return true;//TODO
	}

	//helper functions
	//converts a length in world dimensions to terrain dimensions
	Ogre::Real lengthWorldToTerrain(Ogre::Real worldLength);
	//converts a length in terrain dimensions to world dimensions
	Ogre::Real lengthTerrainToWorld(Ogre::Real terrainLength);

	/*Ogre::TerrainGlobalOptions* mTerrainGlobals;
	Ogre::TerrainGroup* mTerrainGroup;
	bool mPaging;
	Ogre::TerrainPaging* mTerrainPaging;
	Ogre::PageManager* mPageManager;*/

	Ogre::TerrainGlobalOptions* getTerrainGlobals()
	{
		return mTerrainGlobals;
	}
    Ogre::TerrainGroup* getTerrainGroup()
	{
		return mTerrainGroup;
	}

	Ogre::TerrainGlobalOptions* mTerrainGlobals;
    Ogre::TerrainGroup* mTerrainGroup;
    bool mTerrainsImported;

	Ogre::Light *terrainLight;
 
    void defineTerrain(long x, long y);
    void initBlendMaps(Ogre::Terrain* terrain);
    void configureTerrainDefaults();

	//gets
	Ogre::Real getCombinedBlendValue(Ogre::Terrain *terrain, Ogre::uint8 layer, size_t x, size_t y);

	void setCombinedBlendValue(Ogre::Terrain *terrain, Ogre::uint8 layer, size_t x, size_t y, Ogre::Real val);

	//now, my own stuff:
	void loadTerrain();

	//experiment to do the decal using frustum
	//void addDecalPasses();


	

	/** TERRAIN STUFF END **/




	/*/// This class just pretends to provide prcedural page content to avoid page loading
	class DummyPageProvider : public PageProvider
	{
	public:
		bool prepareProceduralPage(Page* page, PagedWorldSection* section) { return true; }
		bool loadProceduralPage(Page* page, PagedWorldSection* section) { return true; }
		bool unloadProceduralPage(Page* page, PagedWorldSection* section) { return true; }
		bool unprepareProceduralPage(Page* page, PagedWorldSection* section) { return true; }
	};
	DummyPageProvider mDummyPageProvider;*/


private:
	

    //Ogre::Vector3 getTerrainVertex(size_t x,size_t z);
    //das ist die ZIP-Datei, aus der das Lvl geladen wird..
    ZipSaveFile *levelSGfile;

	SimpleSound::SoundManager::BufferList mBuffers;
	SimpleSound::SoundManager::SourceList mSources;
    SimpleSound::SoundManager::SourceList mTempSources;
	bool objectListenerUse;
	ObjectList objectListenerList;
	int lastSGID;
//	unsigned int levelVersion; //das ist für abwärtskompatibilität
	/*
	version 1: 
	<type>
		<id pos="x y z" ... />
	</type>

	version 2:
	<objects>
		<type id="foobar" pos=" ... />
	</objects>
	*/
    
	//Ladefunktionen. um sie mal auszulagern
	void processSGID(GameObject *obj);
	void loadFile(ZipSaveFile *sav = NULL);
	void loadVersion1(TiXmlDocument *doc); 
	void loadVersion2(TiXmlDocument *doc);
	//diese Funktion funktioniert wie loadObjects, lädt aber erstmal nur statische Objekte, die anderen werden
	//als TiXmlElement in die preloadedObjects geschrieben    
	void loadSavegamePrepare(TiXmlElement *curXml);
	//aktualisiert preloadedObjects vom savegame, und lädt es
	void loadFromSavegame(ZipSaveFile *sav);
	

	void processLevelSettings(TiXmlElement *elem);
	void processAudioSettings(TiXmlElement *elem);
	void processStatics(TiXmlElement *elem);
	void processDoors(TiXmlElement *elem);
	void processItems(TiXmlElement *elem);
	void processCharacters(TiXmlElement *elem);
	void processLights(TiXmlElement *elem);
	void processEntrances(TiXmlElement *elem);
	void processSpecial(TiXmlElement *elem);
	void processContainers(TiXmlElement *elem);

	Ogre::String getSplattingBasename()
	{
		return splattingBasename;
	}

	void loadDotScene(TiXmlElement *elem);
	void saveDotScene();

	void createCamera();
	void createCameraNodes();

	void createNewtonWorld();

	//void createGeometry(); //das wird später editor only
	//void createCollisionFromPage(unsigned int page_x, unsigned int page_z);
	//void processPage();

	//void loadTerrainGeometry(Ogre::SceneNode* tileNode, Ogre::Vector3* vertices, int numVertices, Ogre::IndexData* indexData);

	typedef std::vector<SimpleSound::Buffer*> BufferVector;
	
    void getTerrainLightSettings(Ogre::ColourValue &lightColor, Ogre::ColourValue &ambientColor, Ogre::Vector3 &lightDirection)
    {
        lightColor = etLightColor;
        ambientColor = etAmbientColor;
        lightDirection = etLightDirection;
    }
    void setTerrainExtends(Ogre::AxisAlignedBox extends);
    void setTerrainLightSettings(Ogre::ColourValue lightColor, Ogre::ColourValue ambientColor, Ogre::Vector3 lightDirection)
    {
        etLightColor = lightColor;
        etAmbientColor = ambientColor;
        etLightDirection = lightDirection;
        updateTerrainLightmap();
    }
	
protected:
	Ogre::SceneNode *CamNode, *CamHeadNode;//die kameranodes
	void setMusic(bool battle=false);
	void setAmbientLoop();
	void setAmbientRandom();

	void preloadMusic();
	bool musicPreloaded;
	BufferVector musicExpPreloaded;
	BufferVector musicBatPreloaded;
	BufferVector ambientLoopPreloaded;
	BufferVector ambientRandomPreloaded;

	Ogre::Quaternion northOrientation;

	//hintergrundfarbe:
	Ogre::ColourValue backgroundColour;
	//für sounds
	Ogre::StringVector musicExplore;
	Ogre::StringVector musicBattle;
	Ogre::StringVector ambientLoop; //wind/regengeräusche o.ä.
	Ogre::StringVector ambientRandom; //normale random ambients
	Ogre::Real wait_min, wait_max; //wie lange zwischen 2 random ambientsounds warten. 
									 //wait_min=0 => kann auch sofort
									 //wait_max=0 => nach oben unbegrenzt. hm kann nicht 0 sein. sagen wir, maxwert = 10 sekunden
	Ogre::Real timeSinceLastAmbient;
	Ogre::Real timeAmbientWait;
	bool hasMusicExplore, hasMusicBattle, hasAmbientLoop, hasAmbientRandom; //um abfragen, ob verctoren leer, vorzubeugen
	size_t lastExIndex; //damit nicht 2x das gleiche hintereinander kommt^^
	size_t lastBaIndex;

	LevelType mLvlType;
	Ogre::String worldGeometryFile;
	//PagingLandscapeDelegate *loadTileDelegate;
	/*PagingLandscapeDelegate *loadPageDelegate;
	PagingLandscapeDelegate *unLoadPageDelegate;*/
	//std::map<Ogre::SceneNode*,OgreNewt::Body*> tilesBodies;
	bool destructorCalled;
	bool destructionPrepared;
	bool isConstructing;
	
	////ob dieses level paging verwendet
	//bool paging;
	////also folgendermaßen:
	///* anzahl d. pages wird als unendlich angesehen, 
	//wenn nötig, prüft das level, ob es die datei gibt
	//erstmal wie morrowind, 9 pages um den spieler rum laden
	//                Z
	// |     |     |     |     |     |   
	//-+-----+-----+-----+-----+-----+---
	// |     |     |0/+1 |     |     |
	//-+-----+-----+-----+-----+-----+---
	// |     |-1/0 |  0  |+1/0 |     |   X
	//-+-----+-----+-----+-----+-----+---
	// |     |     |0/-1 |     |     |
 //   -+-----+-----+-----+-----+-----+---
	// |     |     |     |     |     |
	//*/

	//Page mLoadedPages[9];
	//void loadPage(int x,int z);
	//void unloadPage(int x, int z);
	//bool isPageNeighbour(int main_x, int main_z,int x, int z);
	//bool isPageLoaded(int x, int z);
	//void setPageLoaded(int x, int z,bool set);
	////das BERECHNET, wo der spieler grad nicht. hat nix mit den curPage_ vars zu tun
	////wenn useBorders = false, dann wird die exakte Position zurückgegeben, ohne
	////die grenzunschärfe zu beachten
	//void getPlayerPage(int &x, int &z,bool useBorders = true);
	////schaut nach was geladen/entladen werden muss und lädt/entlädt
	//void updatePaging();
	//int curPage_x; //X-wert der aktuellen page, wo der spieler drin ist
	//int curPage_z; //Z-wert der aktuellen page, da in der XZ-ebene
	//Ogre::Vector3 pageStartOffset; // koordinaten der mitte der (0,0)-page
	//Ogre::Real pageSize; //länge/breite einer page, quadratisch
	//Ogre::Real pagingBorder; //das ist die "unschärfe" der Grenze

	ContCallback *mCallback;



	//Ogre::Real mWorldUpdate, mWorldElapsed; //for world updating
	//Ogre::Real mWorldWaitTime; //test

	//sky stuff
	Ogre::SceneManager::SkyBoxGenParameters skyBoxParams;
	Ogre::SceneManager::SkyDomeGenParameters skyDomeParams;
	Ogre::SceneManager::SkyPlaneGenParameters skyPlaneParams;
	Ogre::String skyBoxMaterial, skyDomeMaterial, skyPlaneMaterial;
	Ogre::Real skyPlaneDist, skyBoxDist, skyDomeDist;

    

	//fog stuff
	Ogre::FogMode mFogMode;
	Ogre::ColourValue mFogColor;
	Ogre::Real mFogDensity;
	Ogre::Real mFogStart;
	Ogre::Real mFogEnd;
	

	/***terrain stuff***/
	bool has_terrain;

	//TERRAIN PAGING STUFF
	Ogre::TerrainPaging* mTerrainPaging;
	Ogre::PageManager* mPageManager;

	class LevelPageProvider : public Ogre::PageProvider
	{
	public:
		bool prepareProceduralPage(Ogre::Page* page, Ogre::PagedWorldSection* section);
		bool loadProceduralPage(Ogre::Page* page, Ogre::PagedWorldSection* section);
		bool unloadProceduralPage(Ogre::Page* page, Ogre::PagedWorldSection* section);
		bool unprepareProceduralPage(Ogre::Page* page, Ogre::PagedWorldSection* section);
	};
	LevelPageProvider *mPageProvider;

    OgreNewt::Body *terrainBody;
    OgreNewt::CollisionPrimitives::TreeCollision *terrainCollision;
    const unsigned int splatChannels;
	/*ET::TerrainManager *mTerrainMgr;
	ET::SplattingManager* mSplatMgr;*/
	Ogre::String splattingBasename;
	Ogre::StringVector terrainTextures;
	/*const ET::TerrainInfo *mTerrainInfo;
	Ogre::MaterialPtr terrainMaterial;
	Ogre::TexturePtr terrainLightmap;*/
    Ogre::ColourValue etLightColor, etAmbientColor;
    Ogre::Vector3 etLightDirection;
    Ogre::AxisAlignedBox terrainExtends;
    //heightmap dimensions
    unsigned int hmWidth;
	unsigned int hmHeight;
    //lightmap dimensions
    unsigned int lmWidth;
	unsigned int lmHeight;
    //coverage/splatting map dimensions
    unsigned int splatWidth;
	unsigned int splatHeight;
	/***terrain stuff end***/

	//player stores the pointer to the char currently under player control
	//origPlayer is for backupping, here is always a pointer to the 
	//char the player is normally supposed to control
	GameChar *origPlayer, *player;
	//pointer to the app. StandardApplication, weil der editor das auch verwenden soll
	StandardApplication *app;
	//Gravity. Why not custom grav for each level?
	Ogre::Real gravity;
	//filename of the level file
	Ogre::String mFileName;
	//name of the level
	Ogre::String mName;


	/****PLSM STUFF*****/
	//unsigned int max_adjacent_pages;
	//Ogre::String plsm2MapName;
	//typedef boost::multi_array<OgreNewt::Body*, 2> body2d;
	
	
	//vector2d<OgreNewt::Body*> plsm2Bodies;
	//vector2d<bool> plsm2collisionCreated;
	//body2d plsm2Bodies;
	//bool plsm2CreatingCollision;
	//Ogre::Vector2 plsm2curPageToParse;

	/****DOTSCENE STUFF*/
	//dotScene filename
	/*Ogre::String dotSceneFileName;
	Ogre::Vector3 dotScenePos;
	Ogre::Vector3 dotSceneScale;
	Ogre::Quaternion dotSceneOrient;*/


	//Newton world of this lvl
	OgreNewt::World *mWorld;
	//Ogre Scene manager
	Ogre::SceneManager *mSceneMgr;
	//die hauptcam des levels
	Ogre::Camera *mCam;
	//objektinfos bleiben in der GameApp,
	//aber die std::maps für dei tatsächlichen objekte kommen hierhin



	void createStandardMaterials();

	void processTerrainSettings(TiXmlElement *elem);

	Ogre::String objectToXml(GameObject *obj);
	//diese funktion nimmt ein XML-element, welches die einzelnen objekte als kinder hat
	//zerparst es und erstellt objekte, mit posOffset als Versatz vom ursprung
	/*
	<objects> <!-- das ist dann das, was man an die funktion übergeben soll -->
		<static pos="0 4.5 0" filename="ogrehead.mesh" type="mesh" scale="0.0584188 0.0584188 0.0584188" />
	</objects>
	*/
	void XmlToObjects(TiXmlElement *objNode, Ogre::Vector3 posOffset = Ogre::Vector3::ZERO);
	


	
	
};


#endif