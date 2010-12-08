#ifndef __praEditorApp
#define __praEditorApp
#include <Ogre.h>

#include <OgreFrameListener.h>



//#include <OgreTextAreaOverlayElement.h>
//#include <OgreFontManager.h>
#include <vector>
#include <OgreNewt.h>
#include "tinyxml.h"
#include "GameObject.h"

#include "EditorSettings.h"

#include "FwDec.h"
#include "EditGameLogic.h"
#include "StandardApplication.h"
#include "QtEditorApp.h"
#include "OgreSingleton.h"
/*
#ifdef __editor
#include "qnamespace.h"//muss ich das wirklich includen?
#endif
*/

OIS::KeyCode convertQtKeyToOis(Qt::Key key);
Qt::Key convertOisKeyToQt(OIS::KeyCode key);
Qt::MouseButton convertOisMouseToQt(OIS::MouseButtonID mouse);
OIS::MouseButtonID convertQtMouseToOis(Qt::MouseButton mouse);

class EditorApp: public StandardApplication
{
public:
	friend class Level;
	//friend class EditFrameListener;
    friend class QtOgre::EditGameLogic;
	EditorApp(Ogre::String cmdline = "");
	~EditorApp();
	//void go();
	void log(Ogre::String txt);
	Ogre::Real sensibility_x;
	Ogre::Real sensibility_y;
    //editormodus. ob man das level, das terrain, oder die objekte bearbeitet
    enum EditorMode
    {
        //das level bearbeiten, also die objekte dort rumschieben
        emLevel,
        //das terrain bearbeiten, die heightmap deformieren und bemalen
        emTerrain,
        //einzelne objekte bearbeiten
        emObjects
    };



	void setDebugText(Ogre::String str,bool add=false);

	//lädt ein bestimmtes level. wenn filename = "", dann wird ein neues erstellt
	void loadLevel(Ogre::String filename);
	
	////AUSBAUEN
	//void saveCurrentLvl();
	////AUSBAUEN
	//void saveCurrentLvlAs();

	//das ist die richtige. es speichert das level unter dem best. Namen
	//wenn name = "", wird der name des levels selbst genommen.
	void saveCurrentLevel(Ogre::String name = "");

	//CEGUI::Renderer *getCEGUIRenderer();

    void setupConfig(TiXmlElement *elem);
	void loadConfig();
	void saveConfig();

	/*void showDetailsDialog(GameObject *obj);
	void hideDetailsDialog();*/

	//void showInputDialog(Ogre::String text, Ogre::String title = WINDOW_NAME);
	//void hideInputDialog();

	void setObjectInfo(Ogre::String text);
	//wenn keine objekte angegeben, macht es ein volles update
	//sonst: 
    //*param changed: das hinzugefügte/entfernte obj
    //*param added: wahr wenn es hinzugefügt wurde, false wenn entfernt
	//oder entfernt wurde
	void updateCurObjectsList(GameObject *changed = NULL,bool added = true);
	//kleine hilfsfunktion, die schadet niemanden wenn sie hierbleibt
	/*CEGUI::String getObjListName(GameObject *cur);*/
	//void setClipboardText(Ogre::String txt);
	//Ogre::String getClipboardText();

	void cleanUp();

    QtOgre::EditGameLogic *getQtOgreGameLogic()
	{
		return qtApp->getGameLogic();
	}
	
	//void deleteOldLevel();


	//ContCallback *cc_static_char;
	//GameChar *player, *playerBackup;
	Ogre::Real conf_lookspeed;

	int menuMode;
	bool paused, 
		shutDown,
		cursorMode;
		//lvlLoaded;
	//Ogre::SceneNode *CamNode, *CamHeadNode;
	//well, faking drag&drop...
	ObjType dragItemType;
	Ogre::String dragItemID;


	//hier das configzeug. ERSTMAL
	Ogre::Real gridSnap;
	Ogre::Real angleSnap;
	Ogre::Real scaleSnap;
	bool snapOnGrid;
	bool snapOnAngle;
	bool snapOnScale;
	
	//bool dotSceneMode;

	
	Ogre::Real axesScaleFactor;
	Ogre::String windowName;
	Ogre::Vector3 camStartPos;
	//Ogre::Quaternion camStartOrient;

    Ogre::StringVector paramList;
    bool showConfigDialog;

	
	void saveMapImageInit();
	void saveMapImageFinish();
	int savingMapImageState;
    //void updateTerrainWindow();
    void setEditorMode(EditorApp::EditorMode mode);
	EditorApp::EditorMode getEditorMode()
	{
		return editingMode;
	}
	void setTerrainEditing(bool enable);
	
	enum TerrainEditMode
    {
        teDeform,
        tePaint,
        teSoften,
        teFlatten,
		teHoles
    };
    void setTerrainEditMode(TerrainEditMode mode)
	{
		tEditMode = mode;
	}
	TerrainEditMode getTerrainEditMode()
	{
		return tEditMode;
	}
	

    //NEW QT STUFF
    //beginnt QT-Initialisierung
    int go_qt(); 

    void setup_qt();

	//keeping the doc here, because at least one qt-relevant function needs to use it
	TiXmlDocument *configXml;

	//to be called from the constructor of qtApp, otherwise it will not see any RenderSystems
	void setupFromQtApp();

    
    QtOgre::QtEditorApp *qtApp;

	static EditorApp *getSingletonPtr()
	{
		return (EditorApp*)StandardApplication::getSingletonPtr();
	}

	void showObjectFromTop(GameObject *obj);

	SimpleSound::Source *createSource(int type, bool loop = false, bool relativeToListener = false, bool stream = false)
	{return NULL;}//nothing yet

	Ogre::SceneManager* getDefaultSceneManager()
	{
		return mDefaultSceneMgr;
	}

	EditorSettings settings;
private:
	//in erster Linie dazu, um festzustellen, wann die collision neu erzeugt werden muss
    bool terrainDeformed;
	//ob man sich im EditTerrain-Modus befindet
    EditorMode editingMode;
	bool editingTerrain;
    
	//bool paintingHoles;
	//bool paintingTextures;    
    //bool terrainFlattening;
    //bool flatteningToClick;
    Ogre::Real terrainIntensity;
    bool intensityChangeInProgress;
	unsigned int currentTexture;
	

	//das Objekt, zu dem grad das detailfenster offen ist
	GameObject *curEditing;

	void (EditorApp::*inputDialogReturn)(Ogre::String value);

    

protected:
    Ogre::String listboxTypeText;//wenn man im listbox tippt
    Ogre::Real timeSinceLastLBDown; //zeit seit letztem keydown auf ein listbox
    //CEGUI::Window *lastLB;
    //Ogre::Root *mRoot;
    Ogre::Camera* mDefaultCam;
	Ogre::SceneNode *mDefaultCamNode;
    Ogre::SceneManager* mDefaultSceneMgr;
    //EditFrameListener *mFrameListener;
	//CEGUI::WindowManager *wmgr;
	//EventProcessor *mEventProcessor;
    //Ogre::RenderWindow* mWindow;
	/*OgreNewt::World* mWorld;*/

    void frameEnded(Ogre::Real time);
	 
	/*Ogre::String levelPath,
		currentLvl;*/
	void createTerrainBrushes();
    void updateBrushesWindow();
    void updateTexturesList();

	//main terrain globals. created and destroyed by the EditorApp
	Ogre::TerrainGlobalOptions *mTerrainGlobals;
	//initialises the global terrain stuff (which needs singletons and so on)
	void initTerrainGlobals();

	//void fillMaterialComboBox(CEGUI::Combobox *cb);

        
    
	void createDefaultSceneMgr(void);
    /** Sets up the application - returns false if the user chooses to abandon configuration. */
	bool setup(void) {return true;}//defined only for StandardApplication's sake
	/** Configures the application - returns false if the user chooses to abandon configuration. */
    bool configure(void);
	//void createDefaultSceneMgr(void);
 //   void createCamera(void);
	//void createCameraNodes(void);
	//void createMaterials(void);

    void createFrameListener(void);

    void initializeQt();

	//void createScene(void);
 //   void destroyScene(void);    // Optional to override this

    //void createViewports(void);

    /// Method which will define the source of resources (other than current folder)
  /*  void setupResources(void);*/

	/// Optional override method where you can create resource listeners (e.g. for loading screens)
	void createResourceListener(void);

	/// Optional override method where you can perform resource group loading
	/// Must at least do Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	//void loadResources(void);
	//void loadGameData(void);


	

	void createGUI();
    void initSound();
	void updateLevelList();

	void saveAsDialogConfirm(Ogre::String name);	
	
    
    TerrainEditMode tEditMode;

 //   
 //   //intensität ändern
 //   bool changeBrushIntensity(const CEGUI::EventArgs& e);
 //   //terrain deformieren (normal)
 //   bool terrainDeformClick(const CEGUI::EventArgs& e);
 //   //"glätten" anhaken
 //   bool terrainFlattenClick(const CEGUI::EventArgs& e);
 //   //das andere glätten:
 //   bool terrainSoftenClick(const CEGUI::EventArgs& e);
 //   //mit texturen malen
 //   bool paintTexturesClick(const CEGUI::EventArgs& e);
	////mit löchern malen
	//bool paintHolesClick(const CEGUI::EventArgs& e);

	////in der levelsettings-auflösungs liste was aswählen
	//bool selectTerrainResolution(const CEGUI::EventArgs& e);
	////diese funktion wählt einen best. Eintrag in der Liste aus
	//void setDisplayedTerrainEditMode(TerrainEditMode mode);

    ////"textures..." klicken
    //bool editTexturesShow(const CEGUI::EventArgs& e);
    ////textur anklicken->preview
    //bool textureListboxClick(const CEGUI::EventArgs& e);
    ////"up" klicken
    //bool editTexturesUpClick(const CEGUI::EventArgs& e);
    ////"down" klicken
    //bool editTexturesDownClick(const CEGUI::EventArgs& e);
    ////"add" klicken
    //bool editTexturesAddClick(const CEGUI::EventArgs& e);
    ////"remove" klicken
    //bool editTexturesRemoveClick(const CEGUI::EventArgs& e);
    ////"ok" klicken
    //bool editTexturesOkClick(const CEGUI::EventArgs& e);
    ////"apply" im terrainedit-fenster klicken
    //bool terrainsettingsApply(const CEGUI::EventArgs& e);

    ////allgemeine listbox-tipp-suchfunktion
    //bool listboxTypeFind(const CEGUI::EventArgs& e);

};

#endif