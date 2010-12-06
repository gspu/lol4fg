#ifndef TESTGAMELOGIC_H_
#define TESTGAMELOGIC_H_

/*
This class pretty much replaces the old EditFrameListener
*/

//#include "ChooseMeshWidget.h"
#include "GameLogic.h"
//#include "MainMenu.h"
#include "Application.h"
#include <OgrePrerequisites.h>
#include "FwDec.h"
#include <QtCore/QHash>
#include "Ogre.h"
#include <QtCore/QTime>
#include "data.h"
#include "UniKey.h"
#include "DecalCursor.h"

namespace QtOgre
{
    
	enum KeyStates
	{
		KS_RELEASED,
		KS_PRESSED
	};

	class EditGameLogic : public GameLogic
	{
        friend class QtEditorApp;
        friend class EditorApp;
	public:
		EditGameLogic(void);

		void initialise(void);
        //THIS IS PRETTY MUCH FrameStarted
		void update(void);
		void shutdown(void);

		void onKeyPress(QKeyEvent* event);
		void onKeyRelease(QKeyEvent* event);

		void onMouseMove(QMouseEvent* event);
		void onMousePress(QMouseEvent* event);
		void onMouseRelease(QMouseEvent* event);
		void onMouseDoubleClick(QMouseEvent* event);

		void onWheel(QWheelEvent* event);

		void showDebugLines(bool show);


		QtOgre::Log* demoLog(void);

        void processTerrainEditing(Ogre::Real time);

        //ENTFERNEN!
		//void loadScene(QString filename);

        //***BEGIN vom FrameListener klauen
        enum transformMode
	    {
		    tmMove,
		    tmRotate,
		    tmScale
	    };
	    enum TransformAxis
	    {
		    //nix
		    tNone,
		    //alles gleichzeitig, nur bei scale relevant
		    tAll,
		    //X-Achse
		    tAxisX,
		    //Y-achse
		    tAxisY,
		    //Z-Achse
		    tAxisZ,
		    //XY-ebene
		    tPlaneXY,
		    //XZ-ebene
		    tPlaneXZ,
		    //YZ-ebene
		    tPlaneYZ
	    };
	    //hilfsklasse
	    class MyRenderQueueListener: public Ogre::RenderQueueListener
	    {
	    public:
    		
		    void renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& invocation, 
				    bool& skipThisInvocation)
		    {
                if(queueGroupId == Ogre::RENDER_QUEUE_9)
			    {
				    //aha
				    Ogre::Root::getSingletonPtr()->getRenderSystem()->clearFrameBuffer(Ogre::FBT_DEPTH);
    //				mDebugText("jetzt");
			    }
		    }
    	        
		    void renderQueueEnded(Ogre::uint8 queueGroupId, const Ogre::String& invocation, 
				    bool& repeatThisInvocation)
		    {
		    }
        private:
		    //Ogre::SceneManager *mSceneMgr;

	    };
	    MyRenderQueueListener *rqListener;
	    TransformAxis curTransform;
	    transformMode mTMode;

    	void requestShutdown();

        void uniKeyPressed(UniKey key);
	    void uniKeyReleased(UniKey key);

        //terrain bearbeiten
        void processTerrainEditing(const Ogre::FrameEvent &evt);
    	
        //to notify the FL that the level has been changed
	    void levelChanged();

	    bool isKeyDown(UniKey k);

	    void setSelected(GameObject *obj);
	    GameObject *getSelected()
	    {
		    return selectedObject;
	    }
        void setDecalPosition(Ogre::Vector3 pos)
        {
            if(decal)
                decal->setPosition(pos);
        }
        //objekt von oben zeigen
        void showObjectFromTop(GameObject *obj);
        //auswahl sperren
        void setSelectionLocked(bool set);
        //***END vom FrameListener klauen

        //funktionen, die nach und nach von EditGameLogic geklaut werden
        //void loadLevel(Ogre::String filename);

     //   //VOM ALTEN EDITORAPP
     //   friend class Level;
	    //friend class EditFrameListener;
   
	    //void go();
//	    void log(Ogre::String txt);
//        Ogre::Real sensibility_x;
//        Ogre::Real sensibility_y;
//        //editormodus. ob man das level, das terrain, oder die objekte bearbeitet
//        enum EditorMode
//        {
//            //das level bearbeiten, also die objekte dort rumschieben
//            emLevel,
//            //das terrain bearbeiten, die heightmap deformieren und bemalen
//            emTerrain,
//            //einzelne objekte bearbeiten
//            emObjects
//        };
//
//        
//	    //ContCallback *cc_static_char;
//	    //GameChar *player, *playerBackup;
//        Ogre::Real conf_lookspeed;
//
//	    int menuMode;
//	    bool paused, 
//		    shutDown,
//		    cursorMode;
//		    //lvlLoaded;
//	    //Ogre::SceneNode *CamNode, *CamHeadNode;
//	    //well, faking drag&drop...
//	    ObjType dragItemType;
//	    Ogre::String dragItemID;
//
//
//	    //hier das configzeug. ERSTMAL
//	    Ogre::Real gridSnap;
//	    Ogre::Real angleSnap;
//	    Ogre::Real scaleSnap;
//	    bool snapOnGrid;
//	    bool snapOnAngle;
//	    bool snapOnScale;
//    	
//	    //bool dotSceneMode;
//
//    	
//	    Ogre::Real axesScaleFactor;
//	    Ogre::String windowName;
//	    Ogre::Vector3 camStartPos;
//	    //Ogre::Quaternion camStartOrient;
//
//        Ogre::StringVector paramList;
//        bool showConfigDialog;
//
//        Ogre::Vector3 oldHitpoint;//das ist für objectScale
//						//es speichert hier den vorherigen Punkt,
//						//an dem die ebene durchgestoßen wurde
//
//	    Ogre::SceneNode *axesNode;
//	    Ogre::SceneNode *axe_X;
//	    Ogre::SceneNode *axe_Y;
//	    Ogre::SceneNode *axe_Z;
//	    Ogre::SceneNode *plane_XZ;
//	    Ogre::SceneNode *plane_XY;
//	    Ogre::SceneNode *plane_YZ;
//	    Ogre::SceneNode *boxAll;
//        Ogre::SceneNode *rotateMoveNode;//diese node wird benutzt, um die cam um ein obj zu drehen
//
//        //StandardApplication *app;//hm. oder erbe ich selbst davon?
//
//	    //bool loadTerrainCollision;
//	    //configzeug ENDE
//
//	    //für das material-fenster
////	    CEGUI::Window *materialSelectTarget;
//
//	    //für particlesystem
////	    CEGUI::Window *psSelectTarget;
//        void setEditorMode(EditGameLogic::EditorMode mode);
//        void hideAxes();
//        //ENDE 

		//drops currently holding item - TO BE REPLACED with Qt's drag&drop-function
		void dropItem();

		//drops item using type and string-id
		//this function can also drop lights, entrances or statics

		void dropItem(ObjType objectType,Ogre::String objectID,WorldArtType staticType = WT_NONE);

		//drops item using it's gamedata
		void dropItem(gamedata *data);

		//drops item by id
		void dropItem(Ogre::String id);

		//gets position for dropping
		Ogre::Vector3 getDropPosition();


		void setTransformModeDirect(transformMode mod);

		void setTransformMode(transformMode mod);

	private:
		//input events
		QKeyEvent   *lastKeyEvent;
		QMouseEvent *lastMouseEvent;
//        //in erster Linie dazu, um festzustellen, wann die collision neu erzeugt werden muss
//        bool terrainDeformed;
//	    //ob man sich im EditTerrain-Modus befindet
//        EditorMode editingMode;
//	    bool editingTerrain;
//        
//	    //bool paintingHoles;
//	    //bool paintingTextures;    
//        //bool terrainFlattening;
//        //bool flatteningToClick;
//        Ogre::Real terrainIntensity;
//        bool intensityChangeInProgress;
//	    unsigned int currentTexture;
//	    //CEGUI::OgreCEGUIRenderer* mGUIRenderer;
//	    //CEGUI::System* mGUISystem;
//	    //CEGUI::Window* mEditorGuiSheet;
//	    //CEGUI::UVector2 barDimensions;
//	    //CEGUI::FrameWindow *detailswnd; //could be used more often
//	    //CEGUI::FrameWindow *editorsettings;
//	    //CEGUI::FrameWindow *levelsettings;
//	    //CEGUI::FrameWindow *inputdialog;
//	    //CEGUI::Window *selectionBorder;
//	    //CEGUI::UVector2 selBorderFirst;
//	    //CEGUI::Window *fpsBox;
//	    //CEGUI::Window *objInfo;
//
//	    //das Objekt, zu dem grad das detailfenster offen ist
//	    GameObject *curEditing;
//
//	    void (EditGameLogic::*inputDialogReturn)(Ogre::String value);
//        //ENDE GEKLAUT
//
////        bool paused;
//        Level *currentLevel;

		QHash<int, KeyStates> mKeyStates;
		QPoint mLastFrameMousePos;
		QPoint mCurrentMousePos;

		int mLastFrameWheelPos;
		int mCurrentWheelPos;
		QTime* mTime;

		int mLastFrameTime;
		int mCurrentTime;

		bool mIsFirstFrame;

		float mCameraSpeed;

        

//		ChooseMeshWidget* mChooseMeshWidget;

//		MainMenu* mMainMenu;

		Ogre::Camera* mCamera;
		Ogre::SceneManager* mSceneManager;
		QtOgre::Log* mDemoLog;
		QtEditorApp *mRealApp;
        //*** BEGIN vom FrameListener klauen
        //die höhe, zu der hin abgeflacht wird
        //-1 = nicht festgelegt, es wird bei mouseUp auf -1 gesetzt
        Ogre::Real flattenHeight;

        //ob die auswahl gesperrt ist
        bool selectionLocked;
        DecalCursor *decal;
        void createDecal();
        void deleteDecal()
        {
            if(!decal)
                return;
            decal->hide();
            delete decal;
            decal = NULL;
        }
    	
	    void setDecalScale(Ogre::ushort scale);



        Ogre::Vector3 deformPosition;

	    EditorApp *app;
//	    CEGUI::Renderer* mGUIRenderer;
	    UniKey		
		    key_modeMove,
		    key_modeRotate,
		    key_modeScale,
		    key_viewRotate,
		    key_viewMove,
		    key_viewRotateMove,
		    key_showLines,
		    key_screenShot,
		    key_delete,
		    key_showDetails,
		    key_objMove,
		    key_clone,
		    key_group,
		    key_copy,
		    key_paste,
		    key_cut,
		    key_movedown,
		    key_savemap,
            key_viewtop;







	    //short moveMode;
	    bool wasOverGui; //ob davor über der GUI war. "hebt" maustasten beim gui-betreten
	    bool lastMouseDownOnGui; //wenn ja, wird das nächste mouseUp über Nichtgui nur so weit verarbeitet, dass man items droppt
	    bool mousePosChanged; //fürs Selektieren
	    //das ist wg cegui. eine funktion wird den cursor anzeigen/ausblenden, dabei diese var ändern
	    //wenn sie falsch ist, kriegt cegui keine mausbewegungen
	    bool cursorShown;
        
        

	    //die var hält nur solange, wie man irgendwas am terrain macht
	    //wird beim loslassen der maus auf false gesetzt
        bool mustUpdateLightmap;
        //ob ich beenden muss
	    bool shutDown;
        //ob OgreNewt debuglinien angezeigt werden
		bool linesShown;
        bool view_rotate;
        bool view_move;
        bool view_rotatemove;            
        bool rotateStartDoOnce;

	    Ogre::Vector3 holdingOffset; //difference between obj position and intersection point
	    Ogre::Vector3 intersectionPoint;
	    Ogre::Vector3 oldIntersectionPoint;//das ist für rotation.
	    Ogre::Vector3 lastSnapVector;
	    Ogre::Vector3 prevCamPos; //alte cam-position, um die achsen nicht unnötig zu skalieren

	    Ogre::Vector3 startPosition;
	    Ogre::Vector3 startScale;
	    Ogre::Quaternion startOrientation;
	    Ogre::Vector3 unSnappedScale; //scale, ohne durch scalesnap betroffen zu sein
	    //Ogre::Quaternion rotatedOrientation;


	 /*   OIS::InputManager* mInputManager;
	    OIS::Mouse*    mMouse;
	    OIS::Keyboard* mKeyboard;*/

	    Ogre::Real lastFrameTime;
	    Ogre::Real mRotate;
	    Ogre::Real mMove;
	    Ogre::Real mScroll;


	  
	    GameObject *nextSelected;
	    GameObject *selectedObject;
	    GameObject *prevSelected; //das "vorher selektierte"
								    //beim mouseUp wird selectedObject damit verglichen
								    //bei nichtübereinstimmung werden sie gleichgesetzt
								    //wenn sie im mouseDragged nicht übereinstimmen, wird
								    //kein movement zugelassen

	    void updatePositionInfoBox(Ogre::Vector3 pos);
	    //1=x 2=y 3=z
	    void updateRotationInfoBox(Ogre::Real deg,short axis);
	    //1=x 2=y 3=z
	    void objectRotate();
	    void objectMove();
	    void objectScale();
	    void objectMoveDown();
	   

		//aktualisiert die auswahl. prüft nicht mehrs elber, ob die group-taste gedrückt ist, muss nun übergeben werden
	    void updateSelection(bool grouping = false);

	    void updateObjectInfo();

	    bool selBoxDrawing;
//	    CEGUI::Point selBoxStart, selBoxEnd;
        //damn.... selektion muss nun iwie anders laufen...
        Ogre::Vector2 selBoxStart, selBoxEnd;
	    void drawSelectionBox();
	    void finishSelectionBox();

	    void showMouseCursor(bool show);

		Ogre::Ray getMouseRay();

	    GameObject* doObjectRaycast();
	    bool scaleDoOnce; //einmal-var fürs skalieren
	    Ogre::Vector3 oldHitpoint;//das ist für objectScale
						    //es speichert hier den vorherigen Punkt,
						    //an dem die ebene durchgestoßen wurde

	    Ogre::SceneNode *axesNode;
	    Ogre::SceneNode *axe_X;
	    Ogre::SceneNode *axe_Y;
	    Ogre::SceneNode *axe_Z;
	    Ogre::SceneNode *plane_XZ;
	    Ogre::SceneNode *plane_XY;
	    Ogre::SceneNode *plane_YZ;
	    Ogre::SceneNode *boxAll;
        Ogre::SceneNode *rotateMoveNode;//diese node wird benutzt, um die cam um ein obj zu drehen
	   
	    void showAxes(GameObject *obj);
	    void hideAxes();
	    void updateAxes();
    	
    	

	    void copyObjects();
	    void pasteObjects();
	    void deleteSelected();

    	
	    
	    //void processTextKey(UniKey key);

        //initiiert rotatemove (um ein objekt herumdrehen)
        //das ist etwas komplizierter, da es die camera an die axesnode hängt
        void initRotatemove();
        //löscht das rotatemove. es ist danach mit dem normalen Umschauen identisch
        void cancelRotatemove();
    	
	    //das "rastet" einen Vektor ein,
	    //dh sucht das um ein Vielfaches von `angle` an `axis` gedrehtes `base`, dass
	    //dem src am nächsten ist. Das `base`wird jetzt automatisch bestimmt
	    Ogre::Vector3 snapVector(Ogre::Degree angle,Ogre::Vector3 src,Ogre::Vector3 axis);
        //*** END vom FrameListener klauen

//		bool isKeyPressed(UniKey key);
		

    protected:
     //   Ogre::String listboxTypeText;//wenn man im listbox tippt
     //   Ogre::Real timeSinceLastLBDown; //zeit seit letztem keydown auf ein listbox
     //   //CEGUI::Window *lastLB;
     //   //Ogre::Root *mRoot;
     //   Ogre::Camera* mDefaultCam;
	    //Ogre::SceneNode *mDefaultCamNode;
     //   Ogre::SceneManager* mDefaultSceneMgr;
     //   EditFrameListener *mFrameListener;
	    //CEGUI::WindowManager *wmgr;
	    //EventProcessor *mEventProcessor;
        Ogre::RenderWindow* mWindow;
	    /*OgreNewt::World* mWorld;*/

        //*** BEGIN vom FrameListener klauen
        int desired_framerate;
	    Ogre::Real m_update, m_elapsed;
        //*** END vom FrameListener klauen
        
        /**
        * this functions will emulate the old FrameListner behaviour... or not?!
        */
        //void frameEnded(Ogre::Real time);

        bool frameStarted(Ogre::Real time);
    	 
	    /*Ogre::String levelPath,
		    currentLvl;*/
	    /*void createTerrainBrushes();
        void updateBrushesWindow();
        void updateTexturesList();*/
	    //void fillMaterialComboBox(CEGUI::Combobox *cb);
	};
}

#endif /*DEMOGAMELOGIC_H_*/