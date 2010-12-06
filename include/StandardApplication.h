#ifndef __praStandardApp
#define __praStandardApp
#include "Ogre.h"
#include "OgreNewt.h"
//singleton test
#include "OgreSingleton.h"
////#include <OgreNoMemoryMacros.h>
//#include <CEGUIImageset.h>
//#include <CEGUISystem.h>
//#include <CEGUILogger.h>
//#include <elements/CEGUIScrollbar.h>
//#include <CEGUISchemeManager.h>
//#include <CEGUIWindowManager.h>
//#include <CEGUIWindow.h>
//#include <RendererModules/Ogre/CEGUIOgreRenderer.h>
//#include <RendererModules/Ogre/CEGUIOgreResourceProvider.h>
//#include <elements/CEGUIPushButton.h>
#include "OIS/OIS.h"
#include "unikey.h"
//#include "FwDec.h"
#include "tinyxml.h"
#include "GameObject.h"
#include <FwDec.h>
//#include "Level.h"
//////#include "OgreAL.h"
#include "data.h"
//extern global_class *global;
#include "Sound.h"
#include "SoundManager.h"





class StandardApplication: public Ogre::Singleton<StandardApplication>
{
public:


	//typedef MissileData MissileData;
	//typedef CollisionData CollisionData;
	//typedef WorldArtData WorldArtData;

	
	//OgreAL::SoundManager *soundMgr;
	SimpleSound::SoundManager *soundMgr;
	

	bool showFog;
	bool showDebugWnd;

	//der teil ist für spielstände
	struct NameTime
	{
		Ogre::String name;
		__int64 time;
	};
	typedef std::vector<NameTime> NameTimeVector;

	void sortNameTimeVector(NameTimeVector &vec);

	//these are the two main data retrieval functions. 
	//get a pointer
	gamedata *getDataPtr(Ogre::String id);
	//get an object
	gamedata getData(Ogre::String id);

	gamedata_dynamic getDynamicData(Ogre::String id);
	gamedata_item getItemData(Ogre::String id);
	gamedata_char getCharData(Ogre::String id);
	gamedata_door getDoorData(Ogre::String id);
	gamedata_spell getSpellData(Ogre::String id);
	gamedata_container getContainerData(Ogre::String id);

	gamedata_dynamic *getDynamicDataPtr(Ogre::String id);
	gamedata_item *getItemDataPtr(Ogre::String id);
	gamedata_char *getCharDataPtr(Ogre::String id);
	gamedata_door *getDoorDataPtr(Ogre::String id);
	gamedata_spell *getSpellDataPtr(Ogre::String id);
	gamedata_container *getContainerDataPtr(Ogre::String id);
		

	//hier werden globale vars gespeichert
	std::map<Ogre::String, Ogre::String> gameGlobals_string;
	std::map<Ogre::String, int> gameGlobals_int;
	std::map<Ogre::String, bool> gameGlobals_bool;

	


	//here is the DATA for all the stuff. Ogre::String ID => data struct 
	/*std::map<Ogre::String, gamedata_dynamic> datamap_dynamic;
	std::map<Ogre::String, gamedata_item> datamap_item;
	std::map<Ogre::String, gamedata_spell> datamap_spell;
	std::map<Ogre::String, gamedata_char> datamap_char;
	std::map<Ogre::String, gamedata_door> datamap_door;
	std::map<Ogre::String, gamedata_container> datamap_container;*/

	typedef std::map<Ogre::String, gamedata*> GameDataMap;//this now stores POINTERS, so that I can cast
	GameDataMap gameDataMap;//this is now to contain all gamedata.

    /// Standard constructor
    StandardApplication();
    /// Standard destructor
    virtual ~StandardApplication();

    /// Start the app
    void go(void);
    

	//void showtextpanel();
	//void settext(Ogre::String txt, bool add=true);
	//void initGUI();

	void setWindowTitle(Ogre::String title);


	virtual void setDebugText(Ogre::String str,bool add=false);

	virtual void setClipboardText(Ogre::String txt); 
	/*{
		clipboard = txt;
	}*/
	virtual Ogre::String getClipboardText();
	/*{
		return clipboard;
	}*/


	



	Ogre::Root *getRoot();
	/*Ogre::Camera* getMainCam();
	Ogre::SceneManager* getSceneManager();
	Ogre::String getCurrentLvlName();*/
	inline Level *StandardApplication::getCurrentLevel()
    {
	    return currentLevel;
    }

	Ogre::RenderWindow* getRenderWindow();


	//pfade zum zeug speichern
	Ogre::String levelPath; //enthält levels und pages
	Ogre::String collisionPath; //enthält kollisionsdateien
	Ogre::String saveGamePath; //enthält spielstände
	Ogre::String tempPath; //enthält savegame-versionen

	
	

	////den nicht zerstören, sondern hier aufheben
	//Ogre::SceneManager *plsm2SceneMgr;	
	////hier das speichern, was bei "setworldgeometry" reinkommt
	//Ogre::String plsm2WorldGeometry;

	Ogre::String uniKeyToString(UniKey key);
	UniKey stringToUniKey(Ogre::String str);

	Ogre::String distanceModelToString(SimpleSound::SoundManager::DistanceModel mod);
	SimpleSound::SoundManager::DistanceModel parseDistanceModel(Ogre::String str);

	//xml -> data
	billboardChainData parseBBChainData(TiXmlElement *elem);
	PlaneData parsePlaneData(TiXmlElement *elem);
	void parseInventory(TiXmlElement *elem, Inventory *inv);
	void parseSpellbook(TiXmlElement *elem, Spellbook *sb);
	
	BeamData parseBeamData(TiXmlElement *elem);
	MissileData parseMissileData(TiXmlElement *elem);
	Damage parseDamageData(TiXmlElement *elem);
	Stats parseStats(TiXmlElement *elem);
	ChStat parseChStats(TiXmlElement *elem);
	void parseStatChangeList(TiXmlElement *elem, StatChangeList &list);
	ExplosionData parseExplosionData(TiXmlElement *elem);
	SpellEffectData parseSpellEffectData(TiXmlElement *elem);

	collision_type stringToColType(Ogre::String str);
	Ogre::String colTypeToString(collision_type type);

	Ogre::String gameDataTypeToString(GamedataType type);
	//GamedataType gameDataTypeToString(Ogre::String str);

	SpellHitType stringToSpellHit(Ogre::String str);
	SpellEffectType stringToSpellEffect(Ogre::String str);

	WorldArtData parseWorldArtData(TiXmlElement *XMLNode);
	CollisionData parseCollisionData(TiXmlElement *XMLNode,bool processCompound = true);
	
	//zum parsen von SpellList
	void spellListToXml(TiXmlElement *elem,SpellList list);

	//data -> xml
	//schreibt alles in *elem rein, ändert nicht das value
	void inventoryToXml(TiXmlElement *elem, Inventory *inv);
	//schreibt alles in *elem rein, ändert nicht das value
	void worldArtToXml(WorldArtData wData,TiXmlElement *elem);
	//schreibt alles in *elem rein, ändert nicht das value
	void statsToXml(TiXmlElement *elem, Stats stat);
	//erzeugt eine "zeile" der Form <resistBlunt value="0.75" mode="multiply" />
	TiXmlElement *chStatsToXml(ChStat stat);
	//schreibt alles in *elem rein, ändert nicht das value
	void statChangeListToXml(TiXmlElement *elem, StatChangeList list);
	//schreibt alles in *elem rein, ändert nicht das value
	void explosionDataToXml(TiXmlElement *elem, ExplosionData data);
	//schreibt alles in *elem rein, ändert nicht das value
	void spellEffectDataToXml(TiXmlElement *elem, SpellEffectData data);

	Ogre::String spellHitToString(SpellHitType t);

	Ogre::String SpellEffectToString(SpellEffectType set);

	void spellbookToXml(TiXmlElement *elem, Spellbook *sb);
	
	void missileDataToXml(TiXmlElement *elem, MissileData data);
	void damageDataToXml(TiXmlElement *elem, Damage dmg);

	void collisionDataToXml(TiXmlElement *XMLNode,CollisionData data);
	

	SimpleSound::Source *createSound(Ogre::String filename, int type, bool loop = false, bool relativeToListener = false, bool stream = false, BufferType bType = btLevel);
	//erstellt ein neues Source und attached param buffer
	SimpleSound::Source *createSound(SimpleSound::Buffer *buffer, int type, bool loop = false, bool relativeToListener = false, bool stream = false);

	//erstellt nur die source
	virtual SimpleSound::Source *createSource(int type, bool loop = false, bool relativeToListener = false, bool stream = false) = 0;
	//OgreAL::Sound *createSound(Ogre::String filename, SoundType type, bool loop = false, bool stream = false, bool relativeToListener = false);

	//kleine hilfsfunktion. es zerstört nur die source, der buffer wird beibehalten!
	void destroySound(SimpleSound::Source *snd);

	Ogre::Vector2 getScreenSize();
protected:
	Alchemy *alchemy;
	unsigned int soundinc;
	std::map<OIS::KeyCode,Ogre::String> keys;
    Ogre::Root *mRoot;
    //Ogre::Camera* mCamera;
    //Ogre::SceneManager* mSceneMgr;
	
	Ogre::String mPluginFileName,
		mConfigFileName,
		mLogFileName;
    Ogre::String configFilename;
	
   
	//EventProcessor *mEventProcessor;
    Ogre::RenderWindow* mWindow;
	

	Level *currentLevel, *oldLevel;

	Ogre::String clipboard;

	

    // These internal methods package up the stages in the startup process
    /** Sets up the application - returns false if the user chooses to abandon configuration. */
	
	virtual bool setup(void) = 0;
	/** Configures the application - returns false if the user chooses to abandon configuration. */
    virtual bool configure(void);
	//virtual void chooseSceneManager(void);
    //virtual void createCamera(void);
	//virtual void createMaterials(void);	
    virtual void createFrameListener(void) = 0;
	//virtual void createScene(void);
    //virtual void destroyScene(void);    // Optional to override this
    //virtual void createViewports(void);
   
    
    /// Method which will define the source of resources (other than current folder) DEPRECATED
    virtual void setupResources(void);
    //erwartet den Pointer auf das <Resources> Element
    virtual void setupResources(TiXmlElement *elem);
    //erwartet den Pointer auf das <Plugins> Element
    virtual void setupPlugins(TiXmlElement *elem);
    //erwartet den Pointer auf das <Renderer> Element
    virtual void setupRenderSystem(TiXmlElement *elem);
    //erwartet den Pointer auf das <game> oder <editor> Element
    virtual void setupConfig(TiXmlElement *elem) = 0;

    //lädt die config.xml
    virtual TiXmlDocument *loadConfigXml();
    //speichert doc und löscht das objekt
    virtual void saveConfigXml(TiXmlDocument *doc);
	/// Optional override method where you can create resource listeners (e.g. for loading screens)
	virtual void createResourceListener(void) = 0;

	

	//schreibt die aktuellen render einstellungen in ein XML-Element
	virtual void renderSettingsToXml(OgreXmlElement *elem);

	/// Optional override method where you can perform resource group loading
	/// Must at least do Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	virtual void loadResources(void);

	//nötig
	virtual void loadGameData(void);
    
    //das ist die LoD-Funktion
    virtual void loadManualLoD();

	//these functions load the different object types from XML
	virtual void processDoorData(TiXmlElement *XMLNode);
	virtual void processCharData(TiXmlElement *XMLNode);
	virtual void processSpellData(TiXmlElement *XMLNode);
	virtual void processItemData(TiXmlElement *XMLNode);
	virtual void processDynamicData(TiXmlElement *XMLNode);
	virtual void processContainerData(TiXmlElement *XMLNode);
	virtual void processAlchemyData(TiXmlElement *XMLNode);
	//adds the data to gameDataMap. also handles alerady existing IDs
	virtual void registerGameData(gamedata *data);

	virtual gamedata_dynamic processDynamicSubobject(TiXmlElement *XMLNode);

	
	


	virtual void setDefaultData(void);

	//void loadLvl(Ogre::String filename);
	/*virtual void createGUI();*/
	/*virtual bool handleQuit(const CEGUI::EventArgs& e);*/

	
		
	


};
#endif