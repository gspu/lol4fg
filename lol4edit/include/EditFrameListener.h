#ifndef __praFrameListener
#define __praFrameListener

//------- BEGIN OGRE & stuff includes ----------
#include <Ogre.h>

#include <OgreFrameListener.h>
#include <OIS/OIS.h>
//#include <OgreEventListeners.h>
//#include <OgreKeyEvent.h>
//
//
//#include <OgreTextAreaOverlayElement.h>
//#include <OgreFontManager.h>
//#include <vector>
#include <OgreNewt.h>
#include "tinyxml.h"
#include "unikey.h"
#include "DecalCursor.h"
////mem probs without this next one
////#include <OgreNoMemoryMacros.h>
//#include <CEGUI/CEGUIImageset.h>
//#include <CEGUI/CEGUISystem.h>
//#include <CEGUI/CEGUILogger.h>
//#include <CEGUI/CEGUISchemeManager.h>
//#include <CEGUI/CEGUIWindowManager.h>
//#include <CEGUI/CEGUIWindow.h>
//#include "OgreCEGUIRenderer.h"
//#include "OgreCEGUIResourceProvider.h"
//#include <CEGUI/elements/CEGUIPushButton.h>
////regular mem handler
//#include <OgreMemoryMacros.h>
////------- END OGRE & stuff includes ----------

//#include "editFwDec.h"
#include "FwDec.h"
//#include "editorDefines.h"
#include "defines.h"
//#include "global_class.h"

#include "EditorApp.h"






class EditFrameListener : public Ogre::FrameListener, public OIS::KeyListener, public OIS::MouseListener, public Ogre::WindowEventListener
{
public:
	friend class EditorApp;
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
	
	/*struct UniKey
	{
		bool keyboard;
		OIS::MouseButtonID mouseID;
		OIS::KeyCode keyboardID;
	};*/
    EditFrameListener();//EditorApp *_app, InputReader* inputReader,CEGUI::Renderer* renderer,Ogre::RenderWindow* win, Ogre::SceneManager* mgr, OgreNewt::World* W, int update_framerate = 60);
	~EditFrameListener();
    // Most code will go here. This gets called every frame
    bool frameStarted(const Ogre::FrameEvent &evt);

	bool frameRenderingQueued(const Ogre::FrameEvent &evt);
	// This gets called at the end of a frame. It's not necessary to use it.
    bool frameEnded(const Ogre::FrameEvent& evt);
	//void setGlobal(global_class *_g);
	void requestShutdown();

	bool keyPressed(const OIS::KeyEvent &arg);

	bool keyReleased(const OIS::KeyEvent &arg);

	bool mouseMoved(const OIS::MouseEvent &arg );
	bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	//void mouseDragged();

	void uniKeyPressed(UniKey key);
	void uniKeyReleased(UniKey key);

    //terrain bearbeiten
    void processTerrainEditing(const Ogre::FrameEvent &evt);
	


	void windowMoved(Ogre::RenderWindow* rw);
	void windowResized(Ogre::RenderWindow* rw);
	void windowClosed(Ogre::RenderWindow* rw);
	void windowFocusChange(Ogre::RenderWindow* rw);

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
private:
    //die höhe, zu der hin abgeflacht wird
    //-1 = nicht festgelegt, es wird bei mouseUp auf -1 gesetzt
    Ogre::Real flattenHeight;

    //ob die auswahl gesperrt ist
    bool selectionLocked;
	Ogre::RenderWindow* mWindow;
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
//	CEGUI::Renderer* mGUIRenderer;
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

	bool /*obj_rotate,
		obj_scale,*/
		/*x_pressed,
		y_pressed,
		z_pressed,*/
		shutDown,
		linesShown,
		view_rotate,
		view_move,
		view_rotatemove,
        //view_rotatemove_inited
		/*LeftMouseDown,
		RightMouseDown,*/
		rotateStartDoOnce;

	Ogre::Vector3 holdingOffset, //difference between obj position and intersection point
			intersectionPoint,
			oldIntersectionPoint,//das ist für rotation.
			lastSnapVector,
			prevCamPos; //alte cam-position, um die achsen nicht unnötig zu skalieren

	Ogre::Vector3 startPosition;
	Ogre::Vector3 startScale;
	Ogre::Quaternion startOrientation;
	Ogre::Vector3 unSnappedScale; //scale, ohne durch scalesnap betroffen zu sein
	//Ogre::Quaternion rotatedOrientation;


	OIS::InputManager* mInputManager;
	OIS::Mouse*    mMouse;
	OIS::Keyboard* mKeyboard;

	Ogre::Real lastFrameTime;
	Ogre::Real mRotate;
	Ogre::Real mMove;
	Ogre::Real mScroll;


	//Ogre::Vector3 oldScale; //spezial-variable fürs skalieren

	//OgreNewt::Body *selectedBody; //to be removed
	//Ogre::SceneNode *selectedNode; //to be removed
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
	void dropItem();

	void updateSelection();

	void updateObjectInfo();

	bool selBoxDrawing;
//	CEGUI::Point selBoxStart, selBoxEnd;
	void drawSelectionBox();
	void finishSelectionBox();

	void showMouseCursor(bool show);

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
	//Ogre::SceneNode *rotInd1;
	//Ogre::SceneNode *rotInd2;
	/*WorldArt *axe_X;
	WorldAr*/

	/*void showMoveAxes(GameObject *obj);
	void showRotateAxes(GameObject *obj);
	void showScaleAxes(GameObject *obj);*/
	void showAxes(GameObject *obj);
	void hideAxes();
	void updateAxes();
	
	

	void copyObjects();
	void pasteObjects();
	void deleteSelected();

	//position, an die man das objekt droppen würde
	Ogre::Vector3 getDropPosition();
	
	void setTransformMode(transformMode mod);

	void processTextKey(UniKey key);

    //initiiert rotatemove (um ein objekt herumdrehen)
    //das ist etwas komplizierter, da es die camera an die axesnode hängt
    void initRotatemove();
    //löscht das rotatemove. es ist danach mit dem normalen Umschauen identisch
    void cancelRotatemove();
	
	//das "rastet" einen Vektor ein,
	//dh sucht das um ein Vielfaches von `angle` gedrehtes `base`, dass
	//dem src am nächsten ist.  DAs `base`wird jetzt automatisch bestimmt
	Ogre::Vector3 snapVector(Ogre::Degree angle,Ogre::Vector3 src,Ogre::Vector3 axis);

protected:
	int desired_framerate;
	Ogre::Real m_update, m_elapsed;
//    bool pressed_1;
//    bool pressed_2;
   // Ogre::Real mRotate;          // The rotate constant
   // Ogre::Real mMove;            // The movement constant
};

#endif


