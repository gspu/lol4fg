#ifndef __praGameApp
#define __praGameApp
//#include "KeyListener.h"
//mem probs without this next one
//#include <OgreNoMemoryMacros.h>
//#include <CEGUI/CEGUIImageset.h>
//#include <CEGUI/CEGUISystem.h>
//#include <CEGUI/CEGUILogger.h>
//#include <CEGUI/CEGUISchemeManager.h>
//#include <CEGUI/CEGUIWindowManager.h>
//#include <CEGUI/CEGUIWindow.h>
//#include "OgreCEGUIRenderer.h"
//#include "OgreCEGUIResourceProvider.h"
//#include <CEGUI/elements/CEGUIPushButton.h>
//#include <CEGUI/elements/CEGUIStaticText.h>
//#include <CEGUI/elements/CEGUIListboxTextItem.h>
////regular mem handler
//#include <OgreMemoryMacros.h> 
#include "StandardApplication.h"
#include "Inventory.h"

#include "ZipSaveFile.h"
////#include "OgreAL.h"

//extern global_class *global;
using namespace Ogre;



class GameApplication: public StandardApplication
{
	friend class CFrameListener;
public:


	enum MenuMode
	{
		GUI_DISABLE,
		GUI_INGAME,
		GUI_MAINMENU,
		GUI_OPTIONS,
		GUI_SAVEGAME
	};




	//ContCallback *cc_static_char;

	Real sensibility_x;
	Real sensibility_y;

	Real sensibility_x_keyboard;
	Real sensibility_y_keyboard;

	MenuMode menuMode;
	/*
	NOW, AGAIN:

	*/
	
    /// Standard constructor
    GameApplication(String cmdline = "");
    /// Standard destructor
    ~GameApplication();

    /// Start the example
    //void go(void);

	//void showtextpanel();
	//void settext(String txt, bool add=true);
	//void initGUI();
	void setMenu(MenuMode mId,bool changePausedState = true);
	
//	bool handleQuit(const CEGUI::EventArgs& e);
//	void log(Ogre::String txt);//kleine Hilfsfunktion
	void setDebugText(String str,bool add=false);
	void showScreenText(String str);
	inline void hideScreenText()
	{
		showScreenText("");
	}

	void setHoldingItem(Inventory::invDataItem item);
	void setHoldingSpell(gamedata_spell* spell);
	void setHoldingContainer(gamedata_container* cont,Inventory *inv=NULL);



	gamedata_char getGameCharData(String Id);
	gamedata_char *getGameCharDataPtr(String Id);

	/*void updateAllLevels(Real time);*/
	void showInfoText(String txt, Real rel_x,Real rel_y);
	void showInfoText(String txt,Ogre::MovableObject *mObj);
	void updateInfoText(Real rel_x, Real rel_y);
	void hideInfoText();

	//lädt das level direkt. wenn reload auf true steht, wird das level neu geladen, auch wenn
	//der player gerade drin ist. ausserdem wird der player nicht aus dem alten level geklont, sondern
	//neu erstellt
	//wenn placeplayer auf false gesetzt wird, reload als true angenommen, und das Level geladen,
	//ohne den player zu setzen
	void loadLevel(Ogre::String filename, Ogre::String entrance = "",bool reload = false,bool placePlayer = true);
	//level zum Laden nach Abschluss des Frames vormerken
	void setLoadLevel(Ogre::String filename, Ogre::String entrance = "");
	//zum altes level löschen oder per setLoadLevel festgelegtes LVL laden
	void frameEnded(Real time);

	CEGUI::OgreCEGUIRenderer *getCEGUIRenderer();

	void toggleInventory();
	void toggleSpellbook();
	void showOtherInventory(Inventory *inv,GameObject *owner);
	void hideOtherInventory();
	void setCursorMode(bool enable);

	void gameOver();


	Inventory::invDataItem curHolding;
	
	gamedata_spell *curHoldingSpell;
	gamedata_container* curHoldingContainer;
	Inventory *curHoldingInventory;

	Inventory *curOpenInventory;
	GameObject *curOpenInvOwner;

	Ogre::String screenshotBase;
	Ogre::String defaultMusic;
	Ogre::String defaultCombatMusic;
	
	void setDisplayedWeapon(gamedata_item* item);
	void setDisplayedSpell(gamedata_spell* spell);

	SimpleSound::Source *music;
	SimpleSound::Source *ambient1;
	SimpleSound::Source *ambient2;
	SimpleSound::Source *playersfx;
	inline bool getZeroTime()
	{
		return zeroTime;
	}
	inline bool getPaused()
	{
		return paused;
	}
	inline bool getCursorMode()
	{
		return cursorMode;
	}
	void setPaused (bool set);

	//einfach nur damit ich nen pointer auf das aktuelle musik-soundobjekt habe
	void setMusic(SimpleSound::Buffer *buff, bool loop = false);
	
	void playPlayerSFX(SimpleSound::Buffer *buff);
	void setAmbient1(SimpleSound::Buffer *buff);
	void setAmbient2(SimpleSound::Buffer *buff,bool loop = false);

	void updateStatBars(Real hp, Real mp, int maxHP, int maxMP);
	void updateCompass();
	bool hasEnemy(GameObject *has)
	{
		for(ObjectList::iterator itr=enemyList.begin();itr!=enemyList.end();itr++)
		{
			if(*itr == has)
			{
				return true;
			}
		}
		return false;
	}
	void addEnemy(GameObject *add)
	{
		if(hasEnemy(add))
			return;
		enemyList.push_back(add);
		setBattleMusic(true);
	}
	void removeEnemy(GameObject *remove)
	{
		for(ObjectList::iterator itr=enemyList.begin();itr!=enemyList.end();itr++)
		{
			if(*itr == remove)
			{
				enemyList.erase(itr);
				break;
			}
		}
		if(enemyList.empty())
			setBattleMusic(false);
	}

private:
	//Sounds
	SimpleSound::Buffer *clickSnd;
	//Sounds ENDE
	Real screenTextTime; //zeit, um den bildschirmtext unten links auszublenden
	bool screenTextShown;
	gamedata_item* curDisplayedWeapon;
	gamedata_spell* curDisplayedSpell;
	bool inventoryOpen;
	bool spellbookOpen;
	bool otherInvOpen;
	bool configLoaded;
	Real otherInvSquaredDist;
	CEGUI::OgreCEGUIRenderer* mGUIRenderer;
	CEGUI::System *mGUISystem;
	CEGUI::Window *mIngameGui, *mMainMenuGui, *mOptionsGui, *mSavegameGui;
	CEGUI::Window *playerInventoryWnd, *playerEquipWnd, *playerSpellbookWnd, *otherInventoryWnd;
	CEGUI::Window *holdingItemWnd;
//	CEGUI::StaticText* infoText;
	CEGUI::Window *infoText;
	CEGUI::Window* hpBar;
	CEGUI::Window* manaBar;
	CEGUI::Window* hpBar_container;
	CEGUI::Window* manaBar_container;
	CEGUI::UDim hpBarWidth;
	CEGUI::UDim manaBarWidth;
	
	SceneManager *noLevelMgr; //für das mainmenu am anfang
	
	Camera *noLevelCam;  //gehört zu dem sceneMgr
	std::vector<String> paramList;
	bool showConfigDialog;

	Ogre::String newRenderer,
		curSelectedRenderer;

	//für setLoadLevel
	Ogre::String sllLevel;
	Ogre::String sllEntrance;
	bool sll;
	bool unpauseNextFrame;
	Real waitingUntilUnpause;//anscheinend muss ich im releasemodus min. 0.85 sekunden warten, bevor ich das level updaten kann
	//nach dem unpausen, damit es nicht mit zu viel zeit geupdated wird
	bool zeroTime;
	bool DebugDoOnce;
	Ogre::String startingLevel;		
	Ogre::String startingEntrance;

	//ob cursormodus standard ist
	bool useMouseLooking;

	void setHoldingDisplay(Ogre::String imgName,Ogre::String subtext = "");

	void updateDisplayedStats();
	
	
	void setBattleMusic(bool set);
	
	ZipSaveFile *tempSaveGame;
	/*void saveGameBegin(String name);
	void saveGameAddFile(void *buf,size_t size,Ogre::String fileName);
	void saveGameAddFile(Ogre::String fileContents,Ogre::String fileName);
	void saveGameAddFileFromDisk(Ogre::String fileNameSrc,Ogre::String fileNameDest);
	void saveGameFinish();
	*/
protected:
	
	typedef std::list<GameObject*> ObjectList;
	ObjectList enemyList;
	CEGUI::Window *compassWnd;
	//TextureUnitState *compassTexState;
	SceneManager *compSceneMgr;
	SceneNode *compNeedleNode;
	Camera *compassCam;
	bool battleMusic;
	bool paused; 
	bool shutDown;
	bool cursorMode;
	void loadConfig();
	void saveConfig();
	CEGUI::ImagesetManager *imgr;
	CEGUI::WindowManager *wmgr;
	

	//das zeichnet die items neu
	void updatePlayerInventory(Inventory *inv = NULL);
	void updateOtherInventory(Inventory *inv = NULL);
	void updateSpellbook(Spellbook *sb = NULL);
	//das erzeugt die felder neu
    void updatePlayerInventoryWindow();
	void updateOtherInventoryWindow();
	void updateSpellbookWindow();
	unsigned int playerInventorySize;
	unsigned int spellbookSize;
	unsigned int otherInventorySize;
	void updateInventoryField(unsigned int x, unsigned int y,Inventory::invDataItem it);
	void updateOtherInvField(unsigned int x, unsigned int y,Inventory::invDataItem it);
	void updateSpellbookField(unsigned int x, unsigned int y,gamedata_spell* spell);

	

	void setSpellBorder(CEGUI::Window *field);

	

    //the framelisteners will differ
	CFrameListener *mFrameListener;
	
	/*
	EventProcessor *mEventProcessor;
    RenderWindow* mWindow;
	OgreNewt::World* mWorld;
	bool lvlLoaded;*/
	//global_class *global; //pointer to the global stuff container

	

    // These internal methods package up the stages in the startup process
    /** Sets up the application - returns false if the user chooses to abandon configuration. */
	bool setup(void);
	/** Configures the application - returns false if the user chooses to abandon configuration. */
    bool configure(void);
	void createDefaultSceneMgr(void);
    //void createCamera(void);
	//void createMaterials(void);

	
    void createFrameListener(void);

	/*void createScene(void);*/
    //void destroyScene(void);    // Optional to override this

    //void createViewports(void);

    /// Method which will define the source of resources (other than current folder)
    //void setupResources(void);

	/// Optional override method where you can create resource listeners (e.g. for loading screens)
	void createResourceListener(void);

	/// Optional override method where you can perform resource group loading
	/// Must at least do ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	//void loadResources(void);
	//void loadGameData(void);
	/*void createPlayer(void);*/
	//String *getCommonValues(TiXmlElement *Node);
	//void lvlFromXml();
	//void loadLvl(String filename);
	void createGUI();
	void initSound();
	void updateVideoWindow();
	void updateOptionsWindow();
	void updateAudioWindow();
	
	

	
	void createCompass();
	void acceptSettings();
	//erzeugt einen string mit einer XML-Datei als inhalt, wo spielstandspezifische
	//Daten drin stehen
	Ogre::String getSavegameSettings();

	void saveOgreConfig();
	
	//setzt das aktuelle Spiel zurück, zB um ein neues Spiel zu starten
	void resetApp();

	bool handleQuit(const CEGUI::EventArgs& e);
	bool startGameClick(const CEGUI::EventArgs& e);
	bool resumeClick(const CEGUI::EventArgs& e);
	bool optionsClick(const CEGUI::EventArgs& e);
	bool saveLoadClick(const CEGUI::EventArgs& e);

	bool savegameLoadClick(const CEGUI::EventArgs& e);
	bool savegameSaveClick(const CEGUI::EventArgs& e);
	bool savegameDeleteClick(const CEGUI::EventArgs& e);
	bool savegameListClick(const CEGUI::EventArgs& e);
	bool savegameListDblClick(const CEGUI::EventArgs& e);
	void updateSaveLoadWnd();
	bool hideSaveLoadWnd(const CEGUI::EventArgs& e);

	bool optionsCancelClick(const CEGUI::EventArgs& e);
	bool optionsOKClick(const CEGUI::EventArgs& e);
	bool optionsAcceptClick(const CEGUI::EventArgs& e);

	bool optionsRendererChanged(const CEGUI::EventArgs& e);

	bool playerInventoryClick(const CEGUI::EventArgs& e);
	//bool otherInventoryClick(const CEGUI::EventArgs& e);
	bool playerSpellbookClick(const CEGUI::EventArgs& e);
	bool equipmentClick(const CEGUI::EventArgs& e);

	//bool spellEquipClick(const CEGUI::EventArgs& e);
	
	bool bagEquipClick(const CEGUI::EventArgs& e);


};

//}


#endif