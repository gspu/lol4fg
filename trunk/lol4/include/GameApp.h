#ifndef __praGameApp
#define __praGameApp
//#include "KeyListener.h"
//mem probs without this next one
//#include <OgreNoMemoryMacros.h>
//#include <CEGUIImageset.h>
//#include <CEGUISystem.h>
//#include <CEGUILogger.h>
//#include <CEGUISchemeManager.h>
//#include <CEGUIWindowManager.h>
//#include <CEGUIWindow.h>
//#include <RendererModules/Ogre/CEGUIOgreRenderer.h>
//#include <RendererModules/Ogre/CEGUIOgreResourceProvider.h>
//#include <elements/CEGUIPushButton.h>
//#include <elements/CEGUIStaticText.h>
//#include <elements/CEGUIListboxTextItem.h>
////regular mem handler
////#include <OgreMemoryMacros.h> 
#include "StandardApplication.h"
#include "Inventory.h"
#include "GameFrameListener.h"
#include "ZipSaveFile.h"
#include "GameSettings.h"
/*#include "InventoryWindow.h"
#include "SpellbookWindow.h"*/
#include "HUD.h"

//extern global_class *global;
using namespace Ogre;



class GameApp: public StandardApplication
{
	friend class GameFrameListener;
public:


	enum MenuMode
	{
		GUI_DISABLE,
		GUI_INGAME,
		GUI_MAINMENU,
		GUI_OPTIONS,
		GUI_SAVEGAME
	};
	
	//this is for saving everything related to what the player currently holds
	class HoldingData
	{
	public:
		//an item, including quantity and stuff. if no item is being held, quantity and the item pointer are NULL
		Inventory::invDataItem item;
	
		//a spell. NULL if none
		gamedata_spell *spell;

		//if the item is a container with something inside, then here is it's inventory, otherwise NULL
		Inventory *inventory;

		bool isEmpty()
		{
			return (item.count == 0 && spell == NULL);
		}

		bool hasItem()
		{
			return (item.count != 0);
		}
	};

	static GameApp *getSingletonPtr()
	{
		return (GameApp*)StandardApplication::getSingletonPtr();
	}


	//more converter stuff




	MenuMode menuMode;
	
    GameApp(String cmdline = "");
    /// Standard destructor
    ~GameApp();


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
	void setHoldingContainer(gamedata_item* cont,Inventory *inv=NULL);

    void setupConfig(TiXmlElement *elem);

	//gamedata_char getGameCharData(String Id);
	//gamedata_char *getGameCharDataPtr(String Id);

	/*void updateAllLevels(Ogre::Real time);*/
	void showInfoText(String txt, Ogre::Real rel_x,Ogre::Real rel_y);
	void showInfoText(String txt,Ogre::MovableObject *mObj);
	void updateInfoText(Ogre::Real rel_x, Ogre::Real rel_y);
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
	void frameEnded(Ogre::Real time);

	
	CEGUI::OgreRenderer *getCEGUIRenderer();

    //öffnet und schließt das eqfenster und evtl auch das "otherInv"
   // void inventoryCallback(bool show);
    //void showEquipmentWindow(bool show);
    void showInventory(bool show);
    void showSpellbook(bool show);
	void toggleInventory();
	void toggleSpellbook();
	void showOtherInventory(Inventory *inv,GameObject *owner);
	void hideOtherInventory();
	void setCursorMode(bool enable);

	void gameOver();


	//HoldingData curHolding;
	
	HoldingSlotWindow *curHolding;

	MainMenu *mainMenu;

	//Inventory *curOpenInventory;
	//GameObject *curOpenInvOwner;

	Ogre::String screenshotBase;

	
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

	void updateStatBars(Ogre::Real hp, Ogre::Real mp, int maxHP, int maxMP);
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
    //void setHotkey(int index, gamedata_hotkeyable *item = NULL);
    //gamedata_hotkeyable *getHotkey(int index);
    bool useHotkey(int index)
	{
		return mHud->useHotkey(index);
	}
    
    //Sounds
	SimpleSound::Buffer *clickSnd;
    //void highlightField(CEGUI::Window *field);
    /*inline void highlightHoldingField()
    {
        highlightField(holdingItemWnd);
    }*/
    void loadGame(Ogre::String name);
    void saveGame(Ogre::String name);

	void quitGame();
	void startGame();
	void resumeGame();
	void showOptions();
	void showSaveLoad();

	void ceguiSetImage(CEGUI::Window *wnd, CEGUI::String str);


	SimpleSound::Source *createSource(int type, bool loop = false, bool relativeToListener = false, bool stream = false);

	GameSettings settings;
	//the framelisteners will differ
	GameFrameListener *mFrameListener;

	CEGUI::Window *getRootWindow()
	{
		return ingameRootWindow;
	}

	GameChar *getPlayer();

	bool key_is(UniKey key,KeyName name)
	{
		return settings.keys[name].main == key || settings.keys[name].alt == key;
	}
	

	//returns the renderer which is currently considered as selected
	inline Ogre::RenderSystem *getSelectedRenderer()
	{
		return selectedRenderer;
	}

	//sets the selected renderer
	inline void setSelectedRenderer(Ogre::RenderSystem* rs)
	{
		selectedRenderer = rs;
	}
	OptionsWindow * getOptionsWindow()
	{
		return mOptWnd;
	}

	//some converter stuff, game-related only

	//converts a  member of the KeyName enum to a string, for xml saving purposes
	//param name is an int for looping purposes
	Ogre::String keyNameToString(unsigned int name);
	//converts a string with the key name to a member of the KeyName enum
	KeyName parseKeyName(Ogre::String str);

private:
	
	//Sounds ENDE
	Ogre::Real screenTextTime; //zeit, um den bildschirmtext unten links auszublenden
	bool screenTextShown;
	gamedata_item* curDisplayedWeapon;
	gamedata_spell* curDisplayedSpell;
	//bool inventoryOpen;
	//bool spellbookOpen;
	//bool otherInvOpen;
	bool configLoaded;
	Ogre::Real otherInvSquaredDist;
	CEGUI::OgreRenderer *mGUIRenderer;
	CEGUI::OgreResourceProvider *ceguiOgreResProvider;
	CEGUI::System *mGUISystem;
	CEGUI::Window *mIngameGui, *mMainMenuGui, *mOptionsGui, *mSavegameGui;
	
	HUD *mHud;
	OptionsWindow *mOptWnd;

	CEGUI::Window *infoText;
	SaveLoadMenu *mSaveLoadMenu;
	/*CEGUI::Window* hpBar;
	CEGUI::Window* manaBar;
	CEGUI::Window* hpBar_container;
	CEGUI::Window* manaBar_container;*/
	CEGUI::UDim hpBarWidth;
	CEGUI::UDim manaBarWidth;
	//COMPASS TEXTURE
	
	Ogre::SceneManager *noLevelMgr; //für das mainmenu am anfang
	
	Camera *noLevelCam;  //gehört zu dem sceneMgr
	Ogre::StringVector paramList;
	bool showConfigDialog;

	//the renderer which has been selected. not necessarily identical to the currently used one
	//initialized on root creation, might change through option dialog selection
	//Ogre::String selectedRenderer;
	Ogre::RenderSystem *selectedRenderer;
	//Ogre::String curSelectedRenderer;

	//für setLoadLevel
	Ogre::String sllLevel;
	Ogre::String sllEntrance;
	bool sll;
	bool unpauseNextFrame;
	Ogre::Real waitingUntilUnpause;//anscheinend muss ich im releasemodus min. 0.85 sekunden warten, bevor ich das level updaten kann
	//nach dem unpausen, damit es nicht mit zu viel zeit geupdated wird
	bool zeroTime;
	bool DebugDoOnce;
	

	

	//void setHoldingDisplay(Ogre::String imgName,Ogre::String subtext = "");

	void updateDisplayedStats();
	
	
	void setBattleMusic(bool set);
	
	ZipSaveFile *tempSaveGame;
	/*void saveGameBegin(String name);
	void saveGameAddFile(void *buf,size_t size,Ogre::String fileName);
	void saveGameAddFile(Ogre::String fileContents,Ogre::String fileName);
	void saveGameAddFileFromDisk(Ogre::String fileNameSrc,Ogre::String fileNameDest);
	void saveGameFinish();
	*/

    //vergleicht einen UniKey mit einem aus der Konfig. also schaut zB, ob der Key dem Key fürs Springen entspricht
    
	
protected:
	

	typedef std::list<GameObject*> ObjectList;
	ObjectList enemyList;
	CEGUI::Window *compassWnd;
	//TextureUnitState *compassTexState;
	Ogre::SceneManager *compSceneMgr;
	SceneNode *compNeedleNode;
	Camera *compassCam;
	bool battleMusic;
	bool paused; 
	bool shutDown;
	bool cursorMode;
	//void loadConfig();
	void saveConfig();
	CEGUI::ImagesetManager *imgr;
	CEGUI::WindowManager *wmgr;
	CEGUI::Window *ingameRootWindow;


	InventoryWindow *playerInventoryView;
	SpellbookWindow *playerSpellbookView;
	ContainerWindow *otherInventoryView;


	

    // These internal methods package up the stages in the startup process
    /** Sets up the application - returns false if the user chooses to abandon configuration. */
	bool setup(void);
	/** Configures the application - returns false if the user chooses to abandon configuration. */
    bool configure(void);
	void createDefaultSceneMgr(void);
    //void createCamera(void);
	//void createMaterials(void);

	
    void createFrameListener(void);


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
	/*void updateVideoWindow();
	void updateOptionsWindow();
	void updateAudioWindow();*/
	
	

	
	void createCompass();
	//void acceptSettings();
	//erzeugt einen string mit einer XML-Datei als inhalt, wo spielstandspezifische
	//Daten drin stehen
	Ogre::String getSavegameSettings();
    //erzeugt den <hotkeys> XML-block
    TiXmlElement *getHotkeysXml();
    //lädt hotkeys aus dem o.g. <hotkeys> block
    void loadHotkeys(TiXmlElement *elem);

	//void saveOgreConfig();
	
	//setzt das aktuelle Spiel zurück, zB um ein neues Spiel zu starten
	void resetApp();

	

	//bool handleQuit(const CEGUI::EventArgs& e);
	//bool startGameClick(const CEGUI::EventArgs& e);
	//bool resumeClick(const CEGUI::EventArgs& e);
	//bool optionsClick(const CEGUI::EventArgs& e);
	//bool saveLoadClick(const CEGUI::EventArgs& e);

	//bool savegameLoadClick(const CEGUI::EventArgs& e);
	//bool savegameSaveClick(const CEGUI::EventArgs& e);
	//bool savegameDeleteClick(const CEGUI::EventArgs& e);
	//bool savegameListClick(const CEGUI::EventArgs& e);
	//bool savegameListDblClick(const CEGUI::EventArgs& e);
	////void updateSaveLoadWnd();
	//bool hideSaveLoadWnd(const CEGUI::EventArgs& e);

	/*bool optionsCancelClick(const CEGUI::EventArgs& e);
	bool optionsOKClick(const CEGUI::EventArgs& e);
	bool optionsAcceptClick(const CEGUI::EventArgs& e);

	bool optionsRendererChanged(const CEGUI::EventArgs& e);*/




};

//}


#endif