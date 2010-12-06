#include "FrameListener.h"
#include "OgreNewt.h"
#include "GameChar.h"
#include "Level.h"
#include "functions.h"
#include "GameItem.h"
#include "ContainerObject.h"

#include <iostream>
#include "winbase.h"
#include <windows.h>
#include "guiwrapper.h"
CFrameListener::CFrameListener()//GameApp *_app, InputReader* inputReader,CEGUI::Renderer* renderer)//,/*Ogre::RenderWindow* win, Ogre::SceneManager* mgr, OgreNewt::World* W,*/ int update_framerate)
{
	
	app = (GameApp*)StandardApplication::getSingletonPtr();
    //mInputReader = inputReader;
	shutDown = false;
	//desired_framerate = update_framerate;
	//mWorld = app->getOgreNewtWorld();//W;
	//m_update = (Ogre::Real)(1.0f / (Ogre::Real)desired_framerate);
	//m_elapsed = 0.0f;
	/*Debugger::getSingleton().init( app->getCurrentLevel()->getSceneManager() );*/
	thirdpersonMode = 0;
	showLines = false;
	timeSinceLastAction = 1;

	frameLimiter = 0;

	mGUIRenderer = app->getCEGUIRenderer();//renderer;



    //mSceneMgr = mgr;
	mWindow = app->getRenderWindow();//win;


	//INIT the OIS stuff
	using namespace OIS;

	Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
	ParamList pl;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;

	mWindow->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

	mInputManager = InputManager::createInputSystem( pl );

	//Create all devices 
	mKeyboard = static_cast<Keyboard*>(mInputManager->createInputObject( OISKeyboard, true ));//bufferedKeys ));
	mMouse = static_cast<Mouse*>(mInputManager->createInputObject( OISMouse, true ));// bufferedMouse ));
	
	mMouse->setEventCallback(this);
	mKeyboard->setEventCallback(this);

	//Set initial mouse clipping size
	windowResized(mWindow);

	//showDebugOverlay(true);

	//Register as a Window listener
	WindowEventUtilities::addWindowEventListener(mWindow, this);

	//keycnt = 23;
	////defaultbelegtung der primärtasten		
	//mainKeys[keyMoveForward]	= OIS::KC_W; 
	//mainKeys[keyMoveBack]		= OIS::KC_S; 
	//mainKeys[keyMoveRight]		= OIS::KC_D; 
	//mainKeys[keyMoveLeft]		= OIS::KC_A;
	//mainKeys[keyJump]			= OIS::KC_E; 
	//mainKeys[keyAttack]			= OIS::MB_Left;
	//mainKeys[keySpellcast]		= OIS::MB_Right; 
	//mainKeys[keyRun]			= OIS::KC_LSHIFT; 
	//mainKeys[keyAction]			= OIS::KC_SPACE; 
	//mainKeys[keyInventory]		= OIS::KC_I; 
	//mainKeys[keySpellbook]		= OIS::KC_M; 

	//mainKeys[keyInvTakeOne]     = OIS::MB_Left; 
	////mainKeys[keyInvTakeOne].ignoreModifier = false;
	//mainKeys[keyInvTakeAll]		= OIS::MB_Middle; 
	////mainKeys[keyInvTakeAll].ignoreModifier = false;
	//mainKeys[keyInvUse]         = OIS::MB_Right; 
	////mainKeys[keyInvUse].ignoreModifier = false;

	//mainKeys[keyScreenshot]		= OIS::KC_SYSRQ; 
	//mainKeys[keyMainmenu]		= OIS::KC_ESCAPE; 
	//mainKeys[keyLookUp]			= OIS::KC_PGUP; 
	//mainKeys[keyLookDown]		= OIS::KC_PGDOWN; 
	//mainKeys[keyLookLeft]		= OIS::KC_LEFT; 
	//mainKeys[keyLookRight]		= OIS::KC_RIGHT; 
	//mainKeys[keyToggleMouselooking] = OIS::KC_X;
	////this is for debugging
	//mainKeys[keyDebug3rdp] = OIS::KC_F1;
	//mainKeys[keyDebugshowlines] = OIS::KC_F2;

	//	
	////defaultbelegung der alternativtasten
	//altKeys[keyMoveForward]		= OIS::KC_UP; 
	//altKeys[keyMoveBack]		= OIS::KC_DOWN; 
	////altKeys[keyMoveRight]		= OIS::KC_D; 
	////altKeys[keyMoveLeft]		= OIS::KC_A;
	////altKeys[keyJump]			= OIS::KC_E; 
	//altKeys[keyAttack]			= OIS::KC_F;
	//altKeys[keySpellcast]		= OIS::KC_R; 
	////altKeys[keyRun]			= OIS::KC_LSHIFT; 
	////altKeys[keyAction]			= OIS::KC_SPACE; 
	//altKeys[keyInventory]		= OIS::KC_TAB; 
	////altKeys[keyInvTakeOne]     = OIS::MB_Left; 
	//altKeys[keyInvTakeAll]		= UniKey(OIS::MB_Left,true); 
	////altKeys[keyInvUse]         = OIS::MB_Right; 
	////altKeys[keyScreenshot]		= OIS::KC_SYSRQ; 
	////altKeys[keyMainmenu]		= OIS::KC_ESCAPE; 
	////altKeys[keyLookUp]			= OIS::KC_PGUP; 
	////altKeys[keyLookDown]		= OIS::KC_PGDOWN; 
	////altKeys[keyLookLeft]		= OIS::KC_LEFT; 
	////altKeys[keyLookRight]		= OIS::KC_RIGHT; 
	////altKeys[keyToggleMouselooking] = OIS::KC_X;
	////this is for debugging
	////altKeys[keyDebug3rdp] = OIS::KC_F1;
	////altKeys[keyDebugshowlines] = OIS::KC_F2;
	
	//end OIS


//    pressed_1 = false;
//    pressed_2 = false;
//    mRotate = 0.1;
//    mMove = 250;
}
CFrameListener::~CFrameListener()
{
		WindowEventUtilities::removeWindowEventListener(mWindow, this);
		windowClosed(mWindow);
}

bool CFrameListener::key_is(UniKey key,KeyName name)
{
	return app->settings.mainKeys[name] == key || app->settings.altKeys[name] == key;
}

bool CFrameListener::isKeyDown(KeyName k)
{
	//check main
	if(isUniKeyDown(app->settings.mainKeys[k]))
		return true;
	//check alt
	if(isUniKeyDown(app->settings.altKeys[k]))
		return true;
	//no
	return false;
}

bool CFrameListener::isUniKeyDown(UniKey k)
{
	//mMouse->getMouseState().buttonDown(
	if(k.isEmpty())
		return false;
	if(k.hasModifier())
	{
		if(k.isKeyboard)
		{
			return (mKeyboard->isKeyDown(k.keyboardID) && mKeyboard->isModifierDown(k.getModifier()));
		}
		else
		{
			return (mMouse->getMouseState().buttonDown(k.mouseID) && mKeyboard->isModifierDown(k.getModifier()));
		}
	}
	else
	{
		if(k.isKeyboard)
		{
			return (mKeyboard->isKeyDown(k.keyboardID));
		}
		else
		{
			return (mMouse->getMouseState().buttonDown(k.mouseID));
		}
	}

}

// Most code will go here. This gets called every frame
bool CFrameListener::frameStarted(const Ogre::FrameEvent &evt)
{

	if(mWindow->isClosed())
		return false;
//    mInputReader->capture();

	mKeyboard->capture();
	mMouse->capture();
	Ogre::Real time = evt.timeSinceLastFrame;

	CEGUI::System::getSingletonPtr()->injectTimePulse(evt.timeSinceLastFrame);
	if(app->zeroTime)
	{
		time = 0;
	}

	if(app->getCurrentLevel())
	{
		if(!app->getPaused())
		{
				
			if( isKeyDown(keyAction)  )//mKeyboard->isKeyDown(OIS::KC_SPACE))//mInputReader->isKeyDown(KC_SPACE))
			{
				if(timeSinceLastAction >= 1)
				{
					//do "action"
				
					app->getCurrentLevel()->getPlayer()->doAction();
					timeSinceLastAction = 0;
				}
				else
				{
					timeSinceLastAction += time;
				}

			}
			
			/*keyLookUp,
		keyLookDown,
		keyLookLeft,
		keyLookRight,*/
			GameChar *pl = app->getCurrentLevel()->getPlayer();
			if( isKeyDown(keyLookUp) )
			{
				pl->modLookingDirection(0,app->settings.sensibility_y_keyboard*time);
			}
			if( isKeyDown(keyLookDown) )
			{
				pl->modLookingDirection(0,-app->settings.sensibility_y_keyboard*time);
			}
			if( isKeyDown(keyLookLeft) )
			{
				pl->modLookingDirection(app->settings.sensibility_x_keyboard,0);
			}
			if( isKeyDown(keyLookRight) )
			{
				pl->modLookingDirection(-app->settings.sensibility_x_keyboard,0);
			}
//			sleep(0.1);
			
			app->getCurrentLevel()->update(time);
			
			
			
			//mLog("evt.timeSinceLastFrame="+str(evt.timeSinceLastFrame));
			//mLog("frameLimiter="+str(frameLimiter));
			
			
			
			//app->updateAllGameChars(evt.timeSinceLastFrame);

				//END processing player--------------------------

			////MEMO
			////Das LEVEL muss sich um sein Newtonzeug kümmern!
			//m_elapsed += evt.timeSinceLastFrame;
			//int count = 0;

			//if ((m_elapsed > m_update) && (m_elapsed < (1.0f)) )
			//{
			//	while (m_elapsed > m_update)
			//	{
			//		app->getCurrentLevel()->getWorld()->update( m_update );
			//		m_elapsed -= m_update;
			//		count++;
			//	}
			//}
			//else
			//{
			//	if (m_elapsed < (m_update))
			//	{
			//		// not enough time has passed this loop, so ignore for now.
			//	}
			//	else
			//	{
			//		app->getCurrentLevel()->getWorld()->update( m_elapsed );
			//		count++;
			//		m_elapsed = 0.0f; // reset the elapsed time so we don't become "eternally behind".
			//	}
			//}

			//Ogre::LogManager::getSingleton().logMessage("   Newton updates this loop: "+Ogre::StringConverter::toString(count));
		}
		//das immer
		app->currentLevel->updateDelete();

		/*if(showLines)
		{
			Debugger::getSingleton().showLines(app->getCurrentLevel()->getWorld());
		}*/

		//END OgreNewt Stuff-----------------------------

		//wenn das Level gewechselt wurde, wird hier das alte gelöscht
		
		////app->log("nach delete old level");
	}

	//nach dem updaten zerotime auf false setzen
	if(app->zeroTime)
	{
		app->zeroTime = false;
	}

	 return true;
	
}
// This gets called at the end of a frame. It's not necessary to use it.
bool CFrameListener::frameEnded(const Ogre::FrameEvent& evt)
{	
    //return true;
	app->frameEnded(evt.timeSinceLastFrame);
	return !shutDown;
}
/*
void CFrameListener::setGlobal(global_class *_g)
{
	global = _g;
}
*/

void CFrameListener::requestShutdown()
{
	shutDown = true;

}

//
//void CFrameListener::keyClicked(KeyEvent* e)
//{
//	if(e->key == KC_SYSRQ)
//	{
//		//screenshot
//		mWindow->writeContentsToTimestampedFile("lol4_",".png");
//		mWindow->setDebugText("screenshot erstellt");
//	}
//	if(app->getCurrentLevel())
//	{
//		if(e->key == KC_ESCAPE)
//		{
//			if(app->menuMode != GUI_MAINMENU)
//			{
//				//app->paused = true;
//				app->setMenu(GUI_MAINMENU);
//			}
//			else// if(app->menuMode == 2)
//			{
//				//app->paused = false;
//				app->setMenu(GUI_INGAME);
//			}
//		}
//		if(e->key == KC_TAB)
//		{
//			if(app->getCurrentLevel()->getPlayer())
//			{
//				Camera *playerCam = app->getCurrentLevel()->getSceneManager()->getCamera(CAM_NAME);
//				if(playerCam->getParentNode())
//				{
//					Node *camNode = playerCam->getParentNode();
//					thirdperson = !thirdperson;
//					if(thirdperson)
//					{
//						
//						camNode->translate(Vector3(0,0,10));//move it 10 units backward
//						//camNode->pitch(Degree(90));
//							
//						
//						//camNode = mNode->createChildSceneNode(Vector3(0,1,0));   
//					}
//					else
//					{
//						camNode->translate(Vector3(0,0,-10));//move it 10 units forward
//						//camNode->pitch(Degree(-90));
//					}
//					
//				}
//			}
//
//			
//		}
//		if (e->key == KC_F3)
//		{
//			showLines = ! showLines;
//			if(showLines)
//			{
//				Debugger::getSingleton().showLines( app->getCurrentLevel()->getWorld() );
//			}
//			else
//			{
//				Debugger::getSingleton().hideLines();
//			}
//		}
//	}//if(app->getCurrentLevel
//	// Do nothing
//	e->consume();
//}

bool CFrameListener::keyPressed(const OIS::KeyEvent &arg)
{
	CEGUI::System::getSingleton().injectKeyDown(arg.key);
	CEGUI::System::getSingleton().injectChar(arg.text);

	if(!app->getCurrentLevel())
		return true;

	UniKey cur = arg.key;		
	//cur.ignoreModifier = false;
	
	if(mKeyboard->isModifierDown(OIS::Keyboard::Shift) && (arg.key != OIS::KC_LSHIFT && arg.key != OIS::KC_RSHIFT))
	{
		//cur.useModifier = true;
		cur.modifier = OIS::Keyboard::Shift;	
	}
	if(mKeyboard->isModifierDown(OIS::Keyboard::Ctrl) && (arg.key != OIS::KC_LCONTROL && arg.key != OIS::KC_RCONTROL))
	{
		//cur.useModifier = true;
		cur.modifier = OIS::Keyboard::Ctrl;
	}
	if(mKeyboard->isModifierDown(OIS::Keyboard::Alt) && (arg.key != OIS::KC_LMENU && arg.key != OIS::KC_RMENU))
	{
		//cur.useModifier = true;
		cur.modifier = OIS::Keyboard::Alt;
	}
	
	uniKeyPressed(cur);

	
	
		
	
	

	
//	e->consume();
	return true;
}

bool CFrameListener::keyReleased(const OIS::KeyEvent &arg)
{
	//compassCam
	CEGUI::System::getSingleton().injectKeyUp(arg.key);

	if(!app->getCurrentLevel())
		return true;
	//test
	//if(arg.key == OIS::KC_F3)
	//{
	//	Vector3 curpos = app->compassCam->getPosition();
	//	curpos.y += 1;
	//	mDebugText(str(curpos));
	//	app->compassCam->setPosition(curpos);
	//}
	//if(arg.key == OIS::KC_F4)
	//{
	//	Vector3 curpos = app->compassCam->getPosition();
	//	curpos.y -= 1;
	//	mDebugText(str(curpos));
	//	app->compassCam->setPosition(curpos);
	//	/*Ogre::SceneManager *mgr = app->getCurrentLevel()->getSceneManager();
	//	ShadowTechnique cur = mgr->getShadowTechnique();
	//	switch(cur)
	//	{
	//	case SHADOWTYPE_NONE:
	//		mgr->setShadowTechnique(SHADOWTYPE_STENCIL_MODULATIVE);
	//		app->setDebugText("SHADOWTYPE_STENCIL_MODULATIVE");
	//		break;
	//	case SHADOWTYPE_STENCIL_MODULATIVE:
	//		mgr->setShadowTechnique(SHADOWTYPE_STENCIL_ADDITIVE);
	//		app->setDebugText("SHADOWTYPE_STENCIL_ADDITIVE");
	//		break;		
	//	 
 //       case SHADOWTYPE_STENCIL_ADDITIVE:
	//		mgr->setShadowTechnique(SHADOWTYPE_TEXTURE_MODULATIVE);
	//		app->setDebugText("SHADOWTYPE_TEXTURE_MODULATIVE");
	//		break;		

 //       case SHADOWTYPE_TEXTURE_MODULATIVE:
	//		mgr->setShadowTechnique(SHADOWTYPE_TEXTURE_ADDITIVE);
	//		app->setDebugText("SHADOWTYPE_TEXTURE_ADDITIVE");
	//		break;		 

 //       case SHADOWTYPE_TEXTURE_ADDITIVE:
	//		mgr->setShadowTechnique(SHADOWTYPE_TEXTURE_ADDITIVE_INTEGRATED);
	//		app->setDebugText("SHADOWTYPE_TEXTURE_ADDITIVE_INTEGRATED");
	//		break;		 
	//	case SHADOWTYPE_TEXTURE_ADDITIVE_INTEGRATED:
	//		mgr->setShadowTechnique(SHADOWTYPE_TEXTURE_MODULATIVE_INTEGRATED);
	//		app->setDebugText("SHADOWTYPE_TEXTURE_MODULATIVE_INTEGRATED");
	//		break;		 
	//	case SHADOWTYPE_TEXTURE_MODULATIVE_INTEGRATED:
	//	default:
	//		mgr->setShadowTechnique(SHADOWTYPE_NONE);
	//		app->setDebugText("SHADOWTYPE_NONE");
	//		break;	*/	 
	//	//}
	//}
	//ende test
	UniKey cur = arg.key;		
	//cur.ignoreModifier = false;
	if(mKeyboard->isModifierDown(OIS::Keyboard::Shift))
		cur.modifier = OIS::Keyboard::Shift;	
	if(mKeyboard->isModifierDown(OIS::Keyboard::Ctrl))
		cur.modifier = OIS::Keyboard::Ctrl;
	if(mKeyboard->isModifierDown(OIS::Keyboard::Alt))
		cur.modifier = OIS::Keyboard::Alt;
	
	uniKeyReleased(cur);

	


	

//    e->consume();
	return true;
}

   // MouseDragged
bool CFrameListener::mouseMoved(const OIS::MouseEvent &arg )
{
	
	if(app->getCurrentLevel())
	{
		GameChar *pl=app->getCurrentLevel()->getPlayer();
		if(pl)
		{
			

			if(!app->getCursorMode() && !app->getPaused())
			{
				/*pl->modLookingDirection(-e->getRelX()*app->sensibility_x,-e->getRelY()*app->sensibility_y);*/
				pl->modLookingDirection(
					-arg.state.X.rel*app->settings.sensibility_x_mouse,
					-arg.state.Y.rel*app->settings.sensibility_y_mouse);
				
			}

			if(app->getCursorMode())
			{
				
				CEGUI::System::getSingleton().injectMouseMove(
					arg.state.X.rel,// * mGUIRenderer->getWidth(), 
					arg.state.Y.rel);// * mGUIRenderer->getHeight());
				CEGUI::System::getSingleton().injectMouseWheelChange(arg.state.Z.rel/100);
				if(app->curHolding.count != 0 || app->curHoldingSpell || app->curHoldingInventory)
				{
					//fenster mitverschieben
					CEGUI::Point mp = CEGUI::MouseCursor::getSingletonPtr()->getPosition();
					
					//app->setDebugText("x="+str(arg.state.X.abs)+"\ny="+str(arg.state.Y.abs));
					
					app->holdingItemWnd->setPosition(CEGUI::UVector2(CEGUI::UDim(0,mp.d_x+8),CEGUI::UDim(0,mp.d_y+8) ));
					//app->holdingItemWnd->setPosition(CEGUI::UVector2(CEGUI::UDim(1,-34),CEGUI::UDim(1,-16)));
				}

				/*e->consume();*/
			}
		}
	}
	else
	{
		CEGUI::System::getSingleton().injectMouseWheelChange(arg.state.Z.rel);
		CEGUI::System::getSingleton().injectMouseMove(
					arg.state.X.rel,// * mGUIRenderer->getWidth(), 
					arg.state.Y.rel);// * mGUIRenderer->getHeight());
		//arg.state.
				/*e->consume();*/
	}
	return true;
	//e->consume();
	/*
	app->player->camera_rotation_x = -e->getRelX() * global->player->x_sensibility;
	global->player->camera_rotation_y = -e->getRelY() * global->player->y_sensibility;


	global->player->camNode->pitch(Degree(global->player->camera_rotation_y));
	global->player->y_rotation_cont += global->player->camera_rotation_y;

	if (app->player->y_rotation_cont > app->player->y_limit_a || app->player->y_rotation_cont < global->player->y_limit_b)  // if the total is bigger or smallest than the limits it will be reseted to it's previous value
	{
		global->player->camNode->pitch(Degree(-global->player->camera_rotation_y ));
		global->player->y_rotation_cont -= global->player->camera_rotation_y;
	} 
	*/
	
	//player->rotateFace(-e->getRelX(),-e->getRelY() );

    //charNode->yaw( Degree(-e->getRelX( ) * conf_lookspeed) );
    //charNode->getChild( 0 )->pitch( Degree(-e->getRelY() * conf_lookspeed) );
    //sicht
}
//void CFrameListener::mouseDragged(MouseEvent* e)
//{
//	if(app->getCurrentLevel())
//	{
//		GameChar *pl=app->getCurrentLevel()->getPlayer();
//		if(pl)
//		{
//
//			if(!app->cursorMode)
//			{
//				pl->modLookingDirection(-e->getRelX()*app->sensibility_x,-e->getRelY()*app->sensibility_y);
//			}
//			else// if(app->menuMode == 1)
//			{
//				
//				
//				CEGUI::System::getSingleton().injectMouseMove(
//					   e->getRelX() * mGUIRenderer->getWidth(), 
//					   e->getRelY() * mGUIRenderer->getHeight());
//				/*e->consume();*/
//			}
//		}
//	}
//	else
//	{
//		CEGUI::System::getSingleton().injectMouseMove(
//					   e->getRelX() * mGUIRenderer->getWidth(), 
//					   e->getRelY() * mGUIRenderer->getHeight());
//				/*e->consume();*/
//	}
//	e->consume();
//
//}
//
//// MouseListener
//void CFrameListener::mouseClicked(MouseEvent* e) { }
//void CFrameListener::mouseEntered(MouseEvent* e) { }
//void CFrameListener::mouseExited(MouseEvent* e) { }
bool CFrameListener::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	if(app->cursorMode)
	{
		CEGUI::System::getSingleton().injectMouseButtonDown(
		convertOISMouseButtonToCegui(id));
	}
	if(!app->getCurrentLevel())
	return true;

	UniKey cur = id;		
	//cur.ignoreModifier = false;

	if(mKeyboard->isModifierDown(OIS::Keyboard::Shift))
		cur.modifier = OIS::Keyboard::Shift;	
	if(mKeyboard->isModifierDown(OIS::Keyboard::Ctrl))
		cur.modifier = OIS::Keyboard::Ctrl;
	if(mKeyboard->isModifierDown(OIS::Keyboard::Alt))
		cur.modifier = OIS::Keyboard::Alt;
	
	uniKeyPressed(cur);

	


	
	return true;
}
bool CFrameListener::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	if(app->cursorMode)
	{
		CEGUI::System::getSingleton().injectMouseButtonUp(
			convertOISMouseButtonToCegui(id));
	}
	if(!app->getCurrentLevel())
		return true;


	UniKey cur = id;		
	//cur.ignoreModifier = false;
	
	if(mKeyboard->isModifierDown(OIS::Keyboard::Shift))
		cur.modifier = OIS::Keyboard::Shift;	
	if(mKeyboard->isModifierDown(OIS::Keyboard::Ctrl))
		cur.modifier = OIS::Keyboard::Ctrl;
	if(mKeyboard->isModifierDown(OIS::Keyboard::Alt))
		cur.modifier = OIS::Keyboard::Alt;
	
	uniKeyReleased(cur);
	
	
	return true;
}


void CFrameListener::windowMoved(Ogre::RenderWindow* rw)   
{

}

void CFrameListener::windowResized(Ogre::RenderWindow* rw) 
{
	unsigned int width, height, depth;
	int left, top;
	rw->getMetrics(width, height, depth, left, top);

	const OIS::MouseState &ms = mMouse->getMouseState();
	ms.width = width;
	ms.height = height;
}


void CFrameListener::windowClosed(Ogre::RenderWindow* rw)  
{
	//Only close for window that created OIS (the main window in these demos)
	if( rw == mWindow )
	{
		if( mInputManager )
		{
			mInputManager->destroyInputObject( mMouse );
			mInputManager->destroyInputObject( mKeyboard );

			OIS::InputManager::destroyInputSystem(mInputManager);
			mInputManager = 0;
		}
	}
}


void CFrameListener::windowFocusChange(Ogre::RenderWindow* rw) 
{

}

bool CFrameListener::processDropKeys(UniKey key)
{
    if(
		(
			app->playerInventoryWnd->isOpen() || 
			app->playerSpellbookWnd->isOpen()
		) && (
			key == app->settings.mainKeys[keyInvUse] || 
			key == app->settings.altKeys[keyInvUse] || 
			key == app->settings.mainKeys[keyInvTakeAll] || 
			key == app->settings.altKeys[keyInvTakeAll]
		)
	)
		//wenn spezial-inventartasten gedrückt sind, sie nicht weiter verarbeiten
		return false;
	if(app->cursorMode && 
		key_is(key,keyInvTakeOne))
	{
		//wenn man in die Landschaft klickt
		//wenn das funktioniert, wird die funktion sonst nichts mehr überprüfen
		if(app->curHoldingSpell)
			return false; //halte nen spell, damit kann ich mir das raycasting sparen

		//anfang von mouseovergui
		if(!isMouseOverGui())
		{
			CEGUI::Size size = mGUIRenderer->getDisplaySize();
			
			Level *lvl = app->getCurrentLevel();
			CEGUI::Point mp = CEGUI::MouseCursor::getSingletonPtr()->getPosition();
			Ogre::Ray mRay = lvl->getMainCam()->getCameraToViewportRay(
				mp.d_x/size.d_width,mp.d_y/size.d_height);		

			GameObject *clickObject = NULL;
			
			OgreNewt::BasicRaycast lookingAt(lvl->getWorld(),mRay.getOrigin(),mRay.getPoint(PICKUP_RANGE),true);
			OgreNewt::BasicRaycast::BasicRaycastInfo rci;
			//rci.
			if(lookingAt.getHitCount() > 0)
			{
				rci = lookingAt.getFirstHit();
				OgreNewt::Body *bod = rci.mBody;
				
				OgreNewt::Body *mBody = lvl->getPlayer()->getBody();
				
				if(bod == mBody)
				{
					bod = NULL;
					for(int i = 0;i<lookingAt.getHitCount();i++)
					{
						rci = lookingAt.getInfoAt(i);
						if(rci.mBody != mBody)
						{
							bod = rci.mBody;
							break;
						}
					}
				}
				if(bod)
				{				
					clickObject = Ogre::any_cast<GameObject*>(bod->getUserData());
				}
			}
			GameChar *pl = app->getCurrentLevel()->getPlayer();
			Quaternion ornt = pl->getOrientation();
			Ogre::Vector3 dropPos;
			if(clickObject)
			{
				dropPos = mRay.getPoint(rci.mDistance*PICKUP_RANGE);
			}
			else
			{
				dropPos = mRay.getPoint(PICKUP_RANGE);
			}
            if(app->curHolding.count != 0)
            {
                if(!app->curHolding.item->Container)
                {
                    //item droppen
				    Inventory::invDataItem cur = app->curHolding;
				    cur.count--;
				    app->setHoldingItem(cur);
    				
				    GameItem *it = new GameItem(cur.item,lvl,dropPos,ornt);
				    if(clickObject)
				    {
					    Ogre::AxisAlignedBox box = it->getCollisionAABB();
					    Ogre::Vector3 min =box.getMinimum();
					    dropPos.y -= min.y;						
					    it->setPositionOrientation(dropPos,ornt);
				    }

                }
                else
                {
                     //tasche droppen
		            gamedata_container* c = app->curHolding.item->Container;
		            Inventory *inv = app->curHoldingInventory;
                    
			        ContainerObject *ct = new ContainerObject(c,lvl,dropPos,ornt,Ogre::Vector3::UNIT_SCALE,inv);
			            
                   
                    if(clickObject)
		            {
			            Ogre::AxisAlignedBox box = ct->getCollisionAABB();
			            Ogre::Vector3 min =box.getMinimum();
			            dropPos.y -= min.y;//+dropPos;								
			            ct->setPositionOrientation(dropPos,pl->getOrientation());
    					
		            }
		            app->setHoldingContainer(NULL);
                }
            }
			/*if(app->curHolding.count != 0 && !app->curHoldingInventory)
			{
				
				
			}
			else if(app->curHolding.count == 0 && app->curHoldingInventory)
			{
				

			}*/
			else if(clickObject && app->curHolding.count == 0 && !app->curHoldingInventory)
			{
				//clickObject->onActivate(pl,true);
				delete clickObject;
			}			
			
		}
		//abbrechen. wenn diese taste gleichzeitig angreifen o.ä. ist, passiert nix
		return false;
	}
	//ende des spezialblocks
	return true;
}

void CFrameListener::uniKeyPressed(UniKey key)
{
	GameChar *pl = app->getCurrentLevel()->getPlayer();
	if(app->menuMode == GameApp::GUI_DISABLE || app->menuMode == GameApp::GUI_INGAME)
	{
		if(pl)
		{
			
			if(!processDropKeys(key))
				return;
			//von diesen Tasten kann man auch mehrere drücken
			if ( key_is(key,keyRun))
			{
				pl->setRun(true);
			}

			if ( key_is(key,keyJump))
			{
				pl->setMoveJump(true);
			}

			if ( key_is(key,keyMoveForward))
			{
			  pl->setMoveUp(true);
			}

			if ( key_is(key,keyMoveBack))
			{
			  pl->setMoveDown(true);
			}

			if ( key_is(key,keyMoveLeft))
			{
			  pl->setMoveLeft(true);
			}

			if ( key_is(key,keyMoveRight))
			{
			  pl->setMoveRight(true);
			} 
			//GameChar *pl = app->getCurrentLevel()->getPlayer();
		
			if ( key_is(key,keyAttack))//e->getButtonID() & MouseEvent::BUTTON0_MASK )
			{
				//settext("links unten");
				pl->setAttack(true);
			}
			if ( key_is(key,keySpellcast))//e->getButtonID() & MouseEvent::BUTTON1_MASK )
			{
				pl->setSpellcast(true);
				//settext("rechts unten");
			}			
		}
	}
}
void CFrameListener::uniKeyReleased(UniKey key)
{
	/*GUI_DISABLE,
		GUI_INGAME,
		GUI_MAINMENU,
		GUI_OPTIONS,
		GUI_SAVEGAME*/
	//diese tasten nur während dem spiel verarbeiten
	if(app->menuMode == GameApp::GUI_DISABLE || app->menuMode == GameApp::GUI_INGAME)
	{
		//if(if((key == mainKeys[keyInvTakeOne] || key == altKeys[keyInvTakeOne] || key == altKeys[keyInvUse]) && app->cursorMode ))
        if(
			app->playerInventoryWnd->isOpen() && (
				key_is(key,keyInvTakeOne) ||
				key_is(key,keyInvUse) ||
				key_is(key,keyInvTakeAll) 
				)
			)
			//wenn eine der inventartasten gedrückt wurde während man im cursormodus ist
			return;
			
		if(key_is(key,keyAction))
		{
			timeSinceLastAction = 1;
		}

		if(key_is(key,keyToggleMouselooking))
		{
			app->settings.mouselooking = !app->settings.mouselooking;
			app->setCursorMode(!app->settings.mouselooking);
		}

		GameChar *pl = app->getCurrentLevel()->getPlayer();
		if(pl)
		{
			
		
			if ( key_is(key,keyRun))
			{
				pl->setRun(false);
			}

			if ( key_is(key,keyMoveForward))
			{
				pl->setMoveUp(false);
			}

			if ( key_is(key,keyMoveBack))
			{
				pl->setMoveDown(false);
			}

			if ( key_is(key,keyMoveLeft))
			{
				pl->setMoveLeft(false);		  
			}

			if ( key_is(key,keyMoveRight))
			{
				pl->setMoveRight(false);		  
			}
			if(key_is(key,keyJump))
			{
				pl->setMoveJump(false);
			}
			if(key_is(key,keyInventory))
			{
				app->toggleInventory();
			}
			if(key_is(key,keySpellbook))
			{
				app->toggleSpellbook();
			}
		
		
			if ( key_is(key,keyAttack))
			{
				//settext("links unten");
				pl->setAttack(false);
			}
			if ( key_is(key,keySpellcast) )
			{
				pl->setSpellcast(false);
				//settext("rechts unten");
			}
            //hier die Hotkeys
            if( key == OIS::KC_1 )
            {
                app->useHotkey(0);
            }
            else if( key == OIS::KC_2 )
            {
                app->useHotkey(1);
            }
            else if( key == OIS::KC_3 )
            {
                app->useHotkey(2);
            }
            else if( key == OIS::KC_4 )
            {
                app->useHotkey(3);
            }
            else if( key == OIS::KC_5 )
            {
                app->useHotkey(4);
            }
            else if( key == OIS::KC_6 )
            {
                app->useHotkey(5);
            }
            else if( key == OIS::KC_7 )
            {
                app->useHotkey(6);
            }
            else if( key == OIS::KC_8 )
            {
                app->useHotkey(7);
            }
            else if( key == OIS::KC_9 )
            {
                app->useHotkey(8);
            }
            else if( key == OIS::KC_0 )
            {
                app->useHotkey(9);
            }
            
		}//von if(player)
	}//von if app->menuMode...
		

	

	if( key_is(key,keyScreenshot))
	{
		//screenshot
		mWindow->writeContentsToTimestampedFile("../screenshots/"+app->screenshotBase,".png");
		app->setDebugText("Screenshot erstellt");
//		mWindow->setDebugText("screenshot erstellt");
	}
	
	if(key_is(key,keyMainmenu))
	{
		if(app->menuMode != GameApp::GUI_MAINMENU)
		{
			//app->paused = true;
			app->setMenu(GameApp::GUI_MAINMENU);
		}
		else// if(app->menuMode == 2)
		{
			//app->paused = false;
			app->setMenu(GameApp::GUI_INGAME);
		}
	}
	if(key_is(key,keyDebug3rdp))
	{
		if(app->getCurrentLevel()->getPlayer())
		{
			Camera *playerCam = app->getCurrentLevel()->getSceneManager()->getCamera(CAM_NAME);
			if(playerCam->getParentNode())
			{
				Node *camNode = playerCam->getParentNode();
				thirdpersonMode++;//= !thirdperson;
				switch(thirdpersonMode)
				{
				case 1:
					//von rechts
					playerCam->setPosition(15,0,0);
					playerCam->setOrientation(Quaternion(Degree(90),Ogre::Vector3::UNIT_Y));
					break;
				case 2:
					//von links
					playerCam->setPosition(-15,0,0);
					playerCam->setOrientation(Quaternion(Degree(-90),Ogre::Vector3::UNIT_Y));
					break;
				case 3:
					//von oben
					playerCam->setPosition(0,15,0);
					playerCam->setOrientation(Quaternion(Degree(-90),Ogre::Vector3::UNIT_X));
					break;
				case 4:
					//von hinten
					playerCam->setPosition(0,0,15);
					playerCam->setOrientation(Quaternion::IDENTITY);
					break;
				case 5:
					//von vorn
					playerCam->setPosition(0,0,-15);
					playerCam->setOrientation(Quaternion(Degree(180),Ogre::Vector3::NEGATIVE_UNIT_Y));
					break;
				default:
					playerCam->setPosition(0,0,0);
					playerCam->setOrientation(Quaternion::IDENTITY);
					thirdpersonMode = 0;
				}
				

				
				
			}
		}

		
	}
	if ( key_is(key,keyDebugshowlines))
	{
		showLines = ! showLines;
		/*if(showLines)
		{
			Debugger::getSingleton().showLines( app->getCurrentLevel()->getWorld() );
		}
		else
		{
			Debugger::getSingleton().hideLines();
		}*/
	}

}