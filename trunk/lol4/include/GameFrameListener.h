#ifndef __praFrameListener
#define __praFrameListener

//------- BEGIN OGRE & stuff includes ----------
#include <Ogre.h>

#include <OgreFrameListener.h>
#include <OIS.h>
//#include <OgreEventListeners.h>
//#include <OgreKeyEvent.h>


//#include <OgreTextAreaOverlayElement.h>
//#include <OgreFontManager.h>
//#include <vector>
#include <OgreNewt.h>
#include "tinyxml.h"



//mem probs without this next one
//#include <OgreNoMemoryMacros.h>
#include <CEGUIImageset.h>
#include <CEGUISystem.h>
#include <CEGUILogger.h>
#include <CEGUISchemeManager.h>
#include <CEGUIWindowManager.h>
#include <CEGUIWindow.h>
//#include <RendererModules/Ogre/CEGUIOgreRenderer.h>
#include <RendererModules/ogre/CEGUIOgreRenderer.h>
//#include <RendererModules/Ogre/CEGUIOgreResourceProvider.h>
#include <RendererModules/ogre/CEGUIOgreResourceProvider.h>
#include <elements/CEGUIPushButton.h>
//regular mem handler
//#include <OgreMemoryMacros.h> 
//------- END OGRE & stuff includes ----------

#include "FwDec.h"
//#include "game_obj.h"
//#include "GameChar.h"
//#include "global_class.h"
//#include "GameFrameListener.h"
#include "GameApp.h"
//#include "contactCallback.h"
#include "unikey.h"
#include "GameSettings.h"
//using namespace Ogre;

//CEGUI::MouseButton convertOgreButtonToCegui(int buttonID)
//{
//   switch (buttonID)
//   {
//   case MouseEvent::BUTTON0_MASK:
//       return CEGUI::LeftButton;
//   case MouseEvent::BUTTON1_MASK:
//       return CEGUI::RightButton;
//   case MouseEvent::BUTTON2_MASK:
//       return CEGUI::MiddleButton;
//   case MouseEvent::BUTTON3_MASK:
//       return CEGUI::X1Button;
//   default:
//       return CEGUI::LeftButton;
//   }
//}



class GameFrameListener : public Ogre::FrameListener, public OIS::KeyListener, public OIS::MouseListener, public Ogre::WindowEventListener/*, public KeyListener, public MouseMotionListener, public MouseListener*/
{
public:
	friend class GameApp;
    GameFrameListener();//GameApp *_app, InputReader* inputReader,CEGUI::Renderer* renderer);//,/*Ogre::RenderWindow* win, Ogre::SceneManager* mgr, OgreNewt::World* W,*/ int update_framerate = 60);
	~GameFrameListener();
    // Most code will go here. This gets called every frame
    bool frameStarted(const Ogre::FrameEvent &evt);
	// This gets called at the end of a frame. It's not necessary to use it.
    bool frameEnded(const Ogre::FrameEvent& evt);
	//void setGlobal(global_class *_g);
	void requestShutdown();

	//void keyClicked(KeyEvent* e);

	bool keyPressed(const OIS::KeyEvent &arg);

	bool keyReleased(const OIS::KeyEvent &arg);

	bool mouseMoved(const OIS::MouseEvent &arg );
	bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id );

	void windowMoved(Ogre::RenderWindow* rw);
	void windowResized(Ogre::RenderWindow* rw);
	void windowClosed(Ogre::RenderWindow* rw);
	void windowFocusChange(Ogre::RenderWindow* rw);


	//   // MouseDragged
	//void mouseMoved(MouseEvent* e);

	//void mouseDragged(MouseEvent* e);

	//// MouseListener
	//void mouseClicked(MouseEvent* e);
	//void mouseEntered(MouseEvent* e);
	//void mouseExited(MouseEvent* e);
	//void mousePressed(MouseEvent* e);
	//void mouseReleased(MouseEvent* e);
	//simply a shortcut to compare a UniKey to main and alt, by KeyName
	bool key_is(UniKey key, KeyName name);

	//activates key detection mode
	//in this mode, everything a key can cause is ignored, until the mode is disabled
	//key detection mode auto-disables on next keyUp
	void enableKeyDetectionMode()
	{
		isKeyDetectionMode = true;
	}

	//retrieves the key detected with key detection mode
	UniKey getDetectedKey();
    
private:
	bool isKeyDetectionMode;
	bool keyDetectionPressed;//is true when pressed a key in detection mode, so that the inital click isn't counted
	UniKey detectedKey;
	//UniKey mainKeys[23];
	//UniKey altKeys[23];
	//unsigned int keycnt;
	

//    InputReader* mInputReader;
	GameApp *app;
	CEGUI::Renderer* mGUIRenderer;
	bool shutDown;
	int thirdpersonMode;
	bool showLines;
	
	Ogre::Real frameLimiter;
	//OgreNewt::World* mWorld;

 //   Ogre::SceneManager* mSceneMgr;
    Ogre::RenderWindow* mWindow;

		//OIS Input devices
	OIS::InputManager* mInputManager;
	OIS::Mouse*    mMouse;
	OIS::Keyboard* mKeyboard;

	//checks if a specified UniKey is down
	bool isUniKeyDown(UniKey k);

	//checks if a specified key is down, by keyName. checks both main and alt automatically
	bool isKeyDown(KeyName k);


	//initialises dragging of objects
	void initDragging();
	//stops dragging. the object falls down
	void stopDragging();
	//updates the dragging target point
	void updateDragging();



protected:
	void uniKeyPressed(UniKey key);
	void uniKeyReleased(UniKey key);
	//kümmert sich um Tasten zum Items ablegen und sowas. wenn es false zurückgibt,
	//dann bricht uniKeyPressed ab
	bool processDropKeys(UniKey key);
	//int desired_framerate;
	Ogre::Real timeSinceLastAction;
	
	OgreNewt::KinematicController* bodyDragController;

	Ogre::Real bodyDragDistance;
};

#endif
