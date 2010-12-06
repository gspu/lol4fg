//#include "editorDefines.h"
#include "defines.h"
#include "EditFrameListener.h"
#include "EditorApp.h"
#include "functions.h"
#include "OIS/OIS.h"
#include "Level.h"
#include "GameObject.h"
#include "GameChar.h"
#include <CEGUI/CEGUIWindow.h>
#include "DynamicObject.h"
#include "GroupObject.h"
#include "QueryListener.h"
#include "DynamicLines.h"
//using namespace Ogre;
using namespace OgreNewt;



CEGUI::MouseButton convertOISMouseButtonToCegui(int buttonID)
{
    switch (buttonID)
    {
	case 0: return CEGUI::LeftButton;
	case 1: return CEGUI::RightButton;
	case 2:	return CEGUI::MiddleButton;
	case 3: return CEGUI::X1Button;
	default: return CEGUI::LeftButton;
    }
}

unsigned int convertOISNumpadToCegui(OIS::KeyCode code, unsigned int returnElse)
{
	switch(code)
	{
	case OIS::KC_NUMPAD0:
		return '0';
	case OIS::KC_NUMPAD1:
		return '1';
	case OIS::KC_NUMPAD2:
		return '2';
	case OIS::KC_NUMPAD3:
		return '3';
	case OIS::KC_NUMPAD4:
		return '4';
	case OIS::KC_NUMPAD5:
		return '5';
	case OIS::KC_NUMPAD6:
		return '6';
	case OIS::KC_NUMPAD7:
		return '7';
	case OIS::KC_NUMPAD8:
		return '8';
	case OIS::KC_NUMPAD9:
		return '9';
	case OIS::KC_ADD:
		return '+';
	case OIS::KC_DIVIDE:
		return '/';
	case OIS::KC_DECIMAL:
		return ',';
	case OIS::KC_NUMPADENTER:
		return 13;


	default:
		return returnElse;
	}

}

Ogre::Vector3 EditFrameListener::snapVector(Ogre::Degree angle,Ogre::Vector3 src,Ogre::Vector3 axis)
{
//	mDebugText("src="+str(src)+"\naxis="+str(axis));
	int cnt = Ogre::Math::Floor(Ogre::Degree(360/angle).valueDegrees()); //anzahl der richtungsvektoren
	Ogre::Quaternion rotBy; //rotation von einem r-vektor zum nächsten
	
	Ogre::Vector3 base;
	if(axis == Ogre::Vector3::UNIT_X)
	{
		base = Ogre::Vector3::UNIT_Z; //globale achse, von der die Rotation ausgeht. also der erste mögliche snap-vektor
		//locAxis = src		
		if(src.directionEquals(Ogre::Vector3::UNIT_X,Ogre::Degree(10)) || src.directionEquals(Ogre::Vector3::NEGATIVE_UNIT_X,Ogre::Degree(10)))
		{			
			base = Ogre::Vector3::UNIT_Y;
		}
		src.x = 0;
	}
	else if(axis == Ogre::Vector3::UNIT_Y)
	{
		base = Ogre::Vector3::UNIT_Z; //globale achse, von der die Rotation ausgeht. also der erste mögliche snap-vektor
		//locAxis = src		
		if(src.directionEquals(Ogre::Vector3::UNIT_Y,Ogre::Degree(10)) || src.directionEquals(Ogre::Vector3::NEGATIVE_UNIT_Y,Ogre::Degree(10)))
		{			
			base = Ogre::Vector3::UNIT_X;
		}
		src.y = 0;
	}
	else if(axis == Ogre::Vector3::UNIT_Z)
	{
		base = Ogre::Vector3::UNIT_Y; //globale achse, von der die Rotation ausgeht. also der erste mögliche snap-vektor
		//locAxis = src		
		if(src.directionEquals(Ogre::Vector3::UNIT_Z,Ogre::Degree(10)) || src.directionEquals(Ogre::Vector3::NEGATIVE_UNIT_Z,Ogre::Degree(10)))
		{			
			base = Ogre::Vector3::UNIT_X;
		}
		src.z = 0;
	}
	else
	{
		//fehler
		return Ogre::Vector3::ZERO;
	}
	//base.normalise();
	src.normalise();
	Ogre::Vector3 snap = base;
	rotBy.FromAngleAxis(angle,axis); //rotation von einem r-vektor zum nächsten
	for(int i=1;i<=cnt;i++)
	{
		
		Ogre::Degree winkel = Ogre::Degree(Ogre::Math::ACos(src.dotProduct(snap)));
		
		if(winkel <= angle/2)
		{
			//gefunden!


			return snap;
			//danach gleich snap verwenden
		}
		//das snap weiterdrehen
		/*mDebugText("snap "+str(i+1)+"mal gedreht");*/
		snap = rotBy * snap;
	}
	return Ogre::Vector3::ZERO;
    
}

EditFrameListener::EditFrameListener()//EditorApp *_app, InputReader* inputReader,CEGUI::Renderer* renderer,Ogre::RenderWindow* win, Ogre::SceneManager* mgr, OgreNewt::World* W, int update_framerate)
{
    selectionLocked = false;
    flattenHeight = -1;
	rqListener = NULL;
    deformPosition = Ogre::Vector3::ZERO;
    decal = NULL;
    mustUpdateLightmap = false;
	unSnappedScale = Ogre::Vector3::UNIT_SCALE;
	lastMouseDownOnGui = false;
	nextSelected = NULL;
	selBoxDrawing = false;
	app = (EditorApp*)StandardApplication::getSingletonPtr();
	mWindow = app->getRenderWindow();//win;
	/*selectedNode = 0;
	selectedBody = 0;*/
	selectedObject = NULL;
	prevSelected = NULL;
	curTransform = tNone;
	axesNode = NULL;
	axe_X = NULL;
	axe_Y = NULL;
	axe_Z = NULL;
	plane_XZ = NULL;
	plane_XY = NULL;
	plane_YZ = NULL;
	boxAll = NULL;
    rotateMoveNode = NULL;
	lastSnapVector = Ogre::Vector3::ZERO;
	oldHitpoint = Ogre::Vector3::ZERO;
	scaleDoOnce = false;
	prevCamPos = Ogre::Vector3::ZERO;
//	rotInd1 = NULL;
//	rotInd2 = NULL;

//	rotatedOrientation = Ogre::Quaternion::IDENTITY;

	shutDown = false;
	linesShown = false;
	//desired_framerate = update_framerate;
	//mWorld = W;
	/*m_update = (Ogre::Real)(1.0f / (Ogre::Real)desired_framerate);
	m_elapsed = 0.0f;*/
	//Debugger::getSingleton().init( mgr );
	//mSceneMgr = app->getSceneManager()mgr;
	//mCamNode = mSceneMgr->getSceneNode("CamNode");
	view_rotate = false;
	view_move	= false;
	view_rotatemove=false;
	cursorShown=true;
	wasOverGui = false;
	mousePosChanged = false;

	rotateStartDoOnce = false;

	//moveMode = 0;
	mTMode = tmMove;
//	obj_rotate = false;
//	obj_scale = false;
	/*x_pressed = false;
	y_pressed = false;
	z_pressed = false;*/
	/*transformX.keyboard = true;
	transformX.keyboardID = OIS::KC_X;
	transformY.keyboard = true;
	transformY.keyboardID = OIS::KC_Y;
	transformZ.keyboard = true;
	transformZ.keyboardID = OIS::KC_Z;*/





	key_modeMove   = UniKey(OIS::KC_M);
	key_modeRotate = UniKey(OIS::KC_R);
	key_modeScale  = UniKey(OIS::KC_S);
    key_viewRotate = OIS::MB_Middle;//OIS::KC_SPACE;
    key_viewMove   = OIS::KC_SPACE;//OIS::KC_LCONTROL;
	key_showLines  = OIS::KC_F3;
	key_screenShot = OIS::KC_SYSRQ;
	key_delete	   = OIS::KC_DELETE;
	key_showDetails= OIS::MB_Right;
	key_objMove    = OIS::MB_Left;
    key_viewRotateMove = OIS::KC_LSHIFT;//OIS::KC_LSHIFT;
	key_group = OIS::KC_LCONTROL;
	key_clone = OIS::KC_D;
	key_clone.modifier = OIS::Keyboard::Ctrl;
//	key_clone.useModifier = true;
	key_movedown = OIS::KC_F;
	key_savemap  = OIS::KC_F12;
	key_copy = UniKey(OIS::KC_C,true);
	key_paste = UniKey(OIS::KC_V,true);
	key_cut = UniKey(OIS::KC_X,true);
    key_viewtop = OIS::KC_T;





//	oldScale = Ogre::Vector3::ZERO;



	/*LeftMouseDown = false;
	RightMouseDown = false;*/
	mRotate = 0.25;
	mMove = 0.25;
	mScroll = 0.05;




//	mRaySceneQuery = mSceneMgr->createRayQuery( Ogre::Ray( ) );


	mGUIRenderer = app->getCEGUIRenderer();

	//INIT the OIS stuff
	using namespace OIS;

	Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
	ParamList pl;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;

	if(!mWindow)
	{
		Ogre::LogManager::getSingletonPtr()->logMessage("no mWindow");
	}


	mWindow->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

    //erlaube das verlassen des fensters:
    //für windows:
    /*ParamList param_list;

    *add ogre render window handle stuff here*
*/
    //mouse
    pl.insert( std::make_pair("w32_mouse", "DISCL_FOREGROUND") );
    pl.insert( std::make_pair("w32_mouse", "DISCL_NONEXCLUSIVE") );

    //keyboard
    pl.insert( std::make_pair("w32_keyboard", "DISCL_FOREGROUND") );
    pl.insert( std::make_pair("w32_keyboard", "DISCL_NONEXCLUSIVE") );
    pl.insert( std::make_pair("w32_keyboard", "DISCL_NOWINKEY") );
/*
    //joystick/gamepad
    param_list.insert( make_pair("w32_joystick", "DISCL_FOREGROUND") );
    param_list.insert( make_pair("w32_joystick", "DISCL_NONEXCLUSIVE") );


    InputManager::createInputSystem( param_list );*/
	//pl.insert(std::make_pair(std::string("win32_mouse"),std::string("DISCL_NONEXCLUSIVE")));
    //linux?
    //mouse
    pl.insert( std::make_pair("x11_mouse_grab", "false") );

    //keyboard
    pl.insert( std::make_pair("x11_keyboard_grab", "false") );

	mInputManager = InputManager::createInputSystem( pl );


	//Create all devices
	mKeyboard = static_cast<Keyboard*>(mInputManager->createInputObject( OISKeyboard, true ));//bufferedKeys ));

	mKeyboard->setTextTranslation(OIS::Keyboard::Unicode);

	mMouse = static_cast<Mouse*>(mInputManager->createInputObject( OISMouse, true ));// bufferedMouse ));
	mMouse->setEventCallback(this);

	mKeyboard->setEventCallback(this);
	mKeyboard->setTextTranslation(OIS::Keyboard::Unicode);

	//Set initial mouse clipping size
	windowResized(mWindow);

	//showDebugOverlay(true);

	//Register as a Window listener
	Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

	//end OIS

	////INIT the OIS stuff
	//using namespace OIS;

	//LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
	//ParamList pl;
	//size_t windowHnd = 0;
	//std::ostringstream windowHndStr;

	//if(!mWindow)
	//{
	//	LogManager::getSingletonPtr()->logMessage("no window WTF?!");
	//}
	//LogManager::getSingletonPtr()->logMessage("vor getcustomattribute");
	//mWindow->getCustomAttribute("WINDOW", &windowHnd);
	//LogManager::getSingletonPtr()->logMessage("danach");
	//windowHndStr << windowHnd;
	//pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

	//mInputManager = InputManager::createInputSystem( pl );



	//LogManager::getSingletonPtr()->logMessage("vor creating devices");

	////Create all devices
	//mKeyboard = static_cast<Keyboard*>(mInputManager->createInputObject( OISKeyboard, true ));//bufferedKeys ));
	//mMouse = static_cast<Mouse*>(mInputManager->createInputObject( OISMouse, true ));// bufferedMouse ));
	//
	//mMouse->setEventCallback(this);
	//mKeyboard->setEventCallback(this);

	////Set initial mouse clipping size
	//windowResized(mWindow);

	////showDebugOverlay(true);

	////Register as a Window listener
	//WindowEventUtilities::addWindowEventListener(mWindow, this);
	//LogManager::getSingletonPtr()->logMessage("*** OIS online ***");
	//
	////end OIS

//    pressed_1 = false;
//    pressed_2 = false;
//    mRotate = 0.1;
//    mMove = 250;
}
EditFrameListener::~EditFrameListener()
{
	Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
	windowClosed(mWindow);
	if(rqListener)
		delete rqListener;

}



// Most code will go here. This gets called every frame
bool EditFrameListener::frameStarted(const Ogre::FrameEvent &evt)
{
   
	
	if(mWindow->isClosed())
		return false;
//    mInputReader->capture();

	mKeyboard->capture();
    mMouse->capture();
  

	//do not do stuff if there is no lvl
	if(!app->getCurrentLevel())
		return true;


    //exit if key KC_ESCAPE pressed
	/*if(mKeyboard->isKeyDown(OIS::KC_ESCAPE))
        shutDown = true;*/
		/////////////////////////////////////////////////////////////
	//		DEBUGGER
	lastFrameTime = evt.timeSinceLastFrame;
	Ogre::Vector3 transVector = Ogre::Vector3::ZERO;


	if ( mKeyboard->isKeyDown( OIS::KC_UP ))// || m_InputReader->isKeyDown( KC_W ) )
		transVector.z -= mMove;
	if ( mKeyboard->isKeyDown( OIS::KC_DOWN ))// || m_InputReader->isKeyDown( KC_S ) )
		transVector.z += mMove;
	if ( mKeyboard->isKeyDown( OIS::KC_LEFT ))// || m_InputReader->isKeyDown( KC_A ) )
		transVector.x -= mMove;
	if ( mKeyboard->isKeyDown( OIS::KC_RIGHT ))// || m_InputReader->isKeyDown( KC_D ) )
		transVector.x += mMove;
	if ( mKeyboard->isKeyDown( OIS::KC_PGUP ))// || m_InputReader->isKeyDown( KC_Q ) )
            transVector.y += mMove;
	if ( mKeyboard->isKeyDown( OIS::KC_PGDOWN ))// || m_InputReader->isKeyDown( KC_E ) )
		transVector.y -= mMove;
	//for wheel:
	//transVector.z -= m_InputReader->getMouseRelativeZ();
	////app->log(Ogre::StringConverter::toString(m_InputReader->getMouseRelativeZ()));

	Ogre::SceneNode *mCamNode = app->getCurrentLevel()->getSceneManager()->getSceneNode("CamNode");
	//mCamNode->getChild( 0 )->_getDerivedOrientation()
	mCamNode->translate( mCamNode->getChild( 0 )->_getDerivedOrientation() * transVector * evt.timeSinceLastFrame );
	
	if(axesNode)
	{
		updateAxes();
	}

	if(!app->paused)
	{


		app->currentLevel->update(evt.timeSinceLastFrame);
		//m_elapsed += evt.timeSinceLastFrame;
		//int count = 0;

		//if ((m_elapsed > m_update) && (m_elapsed < (1.0f)) )
		//{
		//	while (m_elapsed > m_update)
		//	{
		//		mWorld->update( m_update );
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
		//		mWorld->update( m_elapsed );
		//		count++;
		//		m_elapsed = 0.0f; // reset the elapsed time so we don't become "eternally behind".
		//	}
		//}

		//Ogre::LogManager::getSingleton().logMessage("   Newton updates this loop: "+Ogre::StringConverter::toString(count));
	}
	//das immer
	app->currentLevel->updateDelete();

	//END OgreNewt Stuff-----------------------------

	if(linesShown)
	{
		Debugger::getSingleton().showLines(app->getCurrentLevel()->getWorld());
	}
	app->cleanUp();

	if(app->fpsBox)
	{
		const Ogre::RenderTarget::FrameStats& stats = app->mWindow->getStatistics();
		app->fpsBox->setText("FPS: "+str(stats.lastFPS));




		//app->mRoot->getRenderSystem()->get
	}

	CEGUI::System::getSingleton().injectTimePulse(evt.timeSinceLastFrame);

	//terrain bearbeiten
	processTerrainEditing(evt);

    return true;

}

void EditFrameListener::processTerrainEditing(const Ogre::FrameEvent &evt)
{
    if(app->editingMode==EditorApp::emTerrain  && app->currentLevel->getTerrainManager() && !isMouseOverGui())
	{
		// choose a brush intensity, this determines
		// how extreme our brush works on the terrain
		bool lDown = false;
		bool rDown = false;
		if(mMouse->getMouseState().buttonDown(MB_Left))
		{
			lDown = true;
		}
		if(mMouse->getMouseState().buttonDown(MB_Right))
		{
			rDown = true;
		}
        

		//Ogre::Real mouseX = 0.5;
		//Ogre::Real mouseY = 0.5;
		//
		//Level *lvl = app->currentLevel;
		//
		//CEGUI::Point pt = CEGUI::MouseCursor::getSingletonPtr()->getPosition();
		//mouseX = pt.d_x/app->getRenderWindow()->getWidth();
		//mouseY = pt.d_y/app->getRenderWindow()->getHeight();
		//

		//Ogre::Ray mouseRay = lvl->getMainCam()->getCameraToViewportRay(mouseX,mouseY);	


		//
		//// since ETM is no longer a scene manager, we don't use a ray scene query,
		//// but instead query the terrain info directly
		//std::pair<bool, Ogre::Vector3> result = lvl->getTerrainInfo()->rayIntersects(mouseRay);
        Level *lvl = app->getCurrentLevel();
        //anpassen


        const ET::TerrainInfo *info = lvl->getTerrainInfo();
		/*
        Ogre::AxisAlignedBox box= info->getExtents();
        Ogre::Vector3 factor(0,0,0);
        factor.x = info->getHeight();
        factor.z = info->getWidth();
        factor.y = 1;
        factor = (box.getSize()/2)/factor;
        factor.y = 0;
        Ogre::Vector3 pos =deformPosition+factor;*/
        ET::Brush cur = app->getCurrentBrush();
        Ogre::Vector3 pos = deformPosition;// + lvl->getTerrainInfo()->getScaling();
		// int posToVertexX(float x) const { return (int) ((x-mOffset.x)/mScale.x); }
		Ogre::Vector3 vertexCoords;
		vertexCoords.x = round( (deformPosition.x-info->getOffset().x)/info->getScaling().x );
		vertexCoords.z = round( (deformPosition.z-info->getOffset().z)/info->getScaling().z );
		
		int x = (int)vertexCoords.x;//info->posToVertexX(deformPosition.x);
		int z = (int)vertexCoords.z;//info->posToVertexZ(deformPosition.z);
		
        deformPosition.x = info->vertexToPosX(x);
        deformPosition.z = info->vertexToPosZ(z);
        Ogre::Vector3 decalPosition = deformPosition;
        
        
		    
        if(app->tEditMode != EditorApp::tePaint && app->tEditMode != EditorApp::teHoles)
	    {
            //jetzt erstmal decal setzen
            if(cur.getHeight() % 2 == 0)
            {
                decalPosition -= lvl->getTerrainScale()/2;
                //decalPosition.x -= 0.5;
                //decalPosition.z -= 0.5;
            }
            //mDebugText("x="+str(decalPosition.x)+"\nz="+str(decalPosition.z));
    		
            setDecalPosition(decalPosition);//-lvl->getTerrainScale());
            if(lDown || rDown)
            {
                app->terrainDeformed = true;				
                mustUpdateLightmap = true;
                if(app->tEditMode == EditorApp::teFlatten || app->tEditMode == EditorApp::teSoften)
                {
				    //hier bügeln
				    ET::TerrainManager *tMgr = app->currentLevel->getTerrainManager();
					
				    ET::Brush temp = ET::Brush(cur);
				    tMgr->getHeights(x,z,temp);
                    //jetzt einen Übergang suchen

				    std::vector<float> newBrush;//(temp.getHeight()*temp.getWidth(),0.5f)
                    
				    //Ogre::Real flattenTo = 0.5;
                    if(app->tEditMode == EditorApp::teSoften)
                    {
                        flattenHeight = temp.at(Ogre::Math::Floor(temp.getWidth()/2),Ogre::Math::Floor(temp.getHeight()/2));
                    }
                    else
                    {
                        if(flattenHeight == -1)
                        {
                            flattenHeight = temp.at(Ogre::Math::Floor(temp.getWidth()/2),Ogre::Math::Floor(temp.getHeight()/2));
                        }
                    }
                    for(size_t h=0;h<temp.getHeight();h++)
					    for(size_t w=0;w<temp.getWidth();w++)                        
                        {
                            //der wert des origbrushes entscheidet über den faktor
						    //1=voll, 0=gar nicht
						    //abweichung vom Soll:
							
						    Ogre::Real curVal = temp.at(w,h);
                            float brushIntensity = evt.timeSinceLastFrame * 4 * (app->terrainIntensity/50) * (lDown? 1 : -1);
						    Ogre::Real diff = curVal-flattenHeight;
                            Ogre::Real newVal = curVal;
                            if(diff != 0)
                            {
                                Ogre::Real set = curVal - 0.05*Ogre::Math::Sign(diff)*cur.at(w,h)*(brushIntensity);
                                /*if(h==temp.getHeight()/2 && w==temp.getWidth()/2)
                                    mDebugText("set="+str(set)+"\ndiff="+str(diff));*/
                                if(Ogre::Math::Abs(diff) < 0.01)
                                    newVal = curVal - diff*cur.at(w,h)*(brushIntensity);//flattenHeight;
                                else
                                    newVal = set;                                   
                            }
                            //Ogre::Real newVal = curVal - (curVal-flattenHeight)*cur.at(w,h)*(brushIntensity);
						    //newVal += 0.5;
						    newBrush.push_back(newVal);
                        }
				    temp  = ET::Brush(newBrush,temp.getWidth(),temp.getHeight());
                    app->currentLevel->getTerrainManager()->setHeights(x,z,temp);

                }
                else
                {
				
		            //MB_Left = 0, MB_Right, MB_Middle,
                    float brushIntensity = evt.timeSinceLastFrame * 0.4 * (app->terrainIntensity/50) * (lDown? 1 : -1);
		            // translate our cursor position to vertex indexes

    				
		            // now tell the ETM to deform the terrain
		            app->currentLevel->getTerrainManager()->deform(x, z, app->getCurrentBrush(), brushIntensity);
		            //lvl->updateTerrainLightmap();
                    
                }
            }
	    }
	    else
	    {
            //decal zum malen anders positionieren:
			//wenn wir mit löchern malen
			
            setDecalPosition(decalPosition);
            if(lDown || rDown)
            {
				if(app->tEditMode == EditorApp::teHoles)
				{
					mustUpdateLightmap = true;
					app->terrainDeformed = true;
				}
		        // need to set our brush intensity larger for painting.
		        // for painting, all texture channels are only 1 byte
		        // large, so with a small intensity you won't get any
		        // effect at all.
		        float brushIntensity = evt.timeSinceLastFrame * (app->terrainIntensity/50) * 7.0 * (lDown? 1 : -1);
		        // retrieve edit points
		        /*Ogre::Vector3 paintPos = mPointer->getPosition();
		        int x = mTerrainInfo->posToVertexX(paintPos.x);
		        int z = mTerrainInfo->posToVertexZ(paintPos.z);*/
		        // now use the splatting manager to update the coverage maps
				
		        app->currentLevel->getSplattingManager()->paint(app->getCurrentTexture(), x, z, app->getCurrentBrush(), brushIntensity);
            }
	    }
    
		//}
    }
}

bool EditFrameListener::frameRenderingQueued(const Ogre::FrameEvent &evt)
{
	//mLog("hm");
	return true;
}

// This gets called at the end of a frame. It's not necessary to use it.
bool EditFrameListener::frameEnded(const Ogre::FrameEvent& evt)
{
    app->frameEnded(evt.timeSinceLastFrame);
	
    //return true;
	return !shutDown;
}


void EditFrameListener::requestShutdown()
{
	shutDown = true;

}

void EditFrameListener::setSelectionLocked(bool set)
{
    selectionLocked = set;
}

void EditFrameListener::showObjectFromTop(GameObject *obj)
{
    Ogre::Vector3 pos = obj->getPosition();
	pos.y += 10;
    
	Ogre::SceneManager *mSceneMgr = app->currentLevel->getSceneManager();
	Ogre::SceneNode *camNod = mSceneMgr->getSceneNode("CamNode");	
	Ogre::SceneNode *camHeadNod = mSceneMgr->getSceneNode("CamHeadNode");	
	camNod->setPosition(pos);		
	camNod->setOrientation(Ogre::Quaternion::IDENTITY);
	camHeadNod->setOrientation(Ogre::Quaternion(Ogre::Degree(-90),Ogre::Vector3::UNIT_X));
	
}

void EditFrameListener::uniKeyPressed(UniKey key)
{
	processTextKey(key);
	if(!app->getCurrentLevel())
		return;
	/*CEGUI::Window *wnd = getActiveWindow();
	wnd->getty
	CEGUI::System::getSingletonPtr()->getGUISheet()->activate
	if(wnd)
	{
		mDebugText(wnd->getName().c_str());
	}*/
	
	if(isMouseOverGui())
		return;
	selBoxStart = CEGUI::MouseCursor::getSingletonPtr()->getPosition();
	if (key == key_viewRotate)// == KEY_VROTATE)
	{
		//CEGUI::MouseCursor::getSingleton().hide();
		this->showMouseCursor(false);
		view_rotate = true;


	}
    //mDebugText("nein");
	if (key_viewMove == key)// == KEY_VMOVE)
	{

		this->showMouseCursor(false);
        //mDebugText("ja");
		view_move = true;


	}
	if (key_viewRotateMove == key)// == KEY_VMOVE)
	{

		this->showMouseCursor(false);
        //vorbereiten
		view_rotatemove = true;
		//if(selectedObject)
		//{
		//
		//	Ogre::Camera *cam = app->getCurrentLevel()->getMainCam();
		//	Ogre::SceneNode *mCamNode = cam->getParentSceneNode()->getParentSceneNode();//app->getCurrentLevel()->getSceneManager()->getSceneNode("CamNode");
		//	Ogre::Vector3 objPos = selectedObject->getPosition();
		//	Ogre::Vector3 camPos = mCamNode->getPosition();
		//	Ogre::Vector3 offset = objPos - camPos;
		//	cam->setPosition(-offset);
		//	mCamNode->setPosition(objPos);

		//}


	}
    if(key_viewtop == key)
    {
        //von oben-ansicht
        if(selectedObject)
            showObjectFromTop(selectedObject);

    }


	//ab hier nur noch objekt-relevantes
    if(app->editingMode == EditorApp::emTerrain)
		return;

	if(key == key_objMove)
	{
		//mDebugText("objRaycast");
		doObjectRaycast();

		/*if(selectedObject != doObjectRaycast())
		{
			if(selectedObject)
				selectedObject->markSelected(false);
			selectedObject = NULL;

		}		*/

	}
	if(key == key_clone)
	{
		
		if(selectedObject)
		{
			GameObject *cl = selectedObject->clone();	 
			if(selectedObject->getType() == otGroup)
			{
				app->updateCurObjectsList();				
			}
			else if(cl)
			{
				app->updateCurObjectsList(cl,true);
			}//sonst gar nix
		}
	}
	if(key == key_copy)
	{
		copyObjects();
	}
	if(key == key_paste)
	{
		pasteObjects();
	}
	if(key == key_cut)
	{
		copyObjects();		
		deleteSelected();
	}
	


}

void EditFrameListener::deleteSelected()
{
	hideAxes();
	if(selectedObject)
	{
		//ObjType type = selectedObject->getType();
		
		//if(type != otGroup)
		//{
		mLog("werde gleich ein update der liste hervorrufen");
			app->updateCurObjectsList(selectedObject,false);
		//}
		//else
		//{
		//	app->updateCurObjectsList();
		//}
		delete selectedObject;
		selectedObject = NULL;
	}
}

void EditFrameListener::copyObjects()
{
	if(selectedObject)
	{
		TiXmlDocument doc;
		TiXmlElement *elem = new TiXmlElement("objects");
		Ogre::Vector3 pos = selectedObject->getPosition();
		
		if(selectedObject->getType() ==  otGroup)
		{
			GroupObject *gr = (GroupObject*)(selectedObject);
			
			std::vector<GameObject*>::const_iterator itr;
			for(itr = gr->groupedObjects.begin();itr != gr->groupedObjects.end();itr++)
			{
				GameObject *cur = *itr;
				
				elem->LinkEndChild(cur->getXmlElem(pos));
			}
		}
		else
		{
			elem->LinkEndChild(selectedObject->getXmlElem(pos));
		}
		
		doc.LinkEndChild(elem);
		
		
		TiXmlPrinter *test = new TiXmlPrinter();
		
		doc.Accept(test);
		app->setClipboardText(test->CStr());
		
	}
}

void EditFrameListener::pasteObjects()
{
	TiXmlDocument doc;
	CEGUI::String str = app->getClipboardText();
	if(str != "")
	{
		doc.Parse(str.c_str());
		TiXmlElement *elem = doc.FirstChildElement("objects");
		if(elem)
		{
			Level *lvl = app->getCurrentLevel();
			Ogre::Vector3 pos = getDropPosition();
			lvl->startObjectListener();
			lvl->loadObjects(elem,pos);
			ObjectList list = lvl->finishObjectListener();
			if(list.size() == 1)
			{
				setSelected(*list.begin());
                app->updateCurObjectsList(*list.begin());
			}
			else
			{
				bool first = true;
				GroupObject *gr;
				for(ObjectList::iterator itr = list.begin();itr != list.end();itr++)
				{
					if(first)
					{
						gr = new GroupObject(lvl,*itr);
						first = false;
					}
					else
					{
						gr->addObject(*itr);
						showAxes(gr);
					}
				}
				setSelected(gr);
                app->updateCurObjectsList(gr);
			}
		}
	}
    
}

void EditFrameListener::uniKeyReleased(UniKey key)
{
    mDebugText("");
	if(key_screenShot == key)// == OIS::KC_SYSRQ)
	{
		//screenshot
		mWindow->writeContentsToTimestampedFile("editor_",".png");
		app->setDebugText("screenshot erstellt");
	}
	if(!app->getCurrentLevel())
		return;
	if(isMouseOverGui())
		return;

	if(key_savemap == key)// == OIS::KC_SYSRQ)
	{
		//screenshot
		app->saveMapImageInit();
		app->setDebugText("ganzlevel-screenshot erstellt");
	}
	

	if(key_showLines == key)// == OIS::KC_F3)
	{
		if(linesShown)
		{
			Debugger::getSingleton().hideLines();
		}
		else
		{
			Debugger::getSingleton().showLines(app->getCurrentLevel()->getWorld());
		}
		linesShown = !linesShown;
	}
	if (key == key_viewRotate)
	{
        if(!view_move && !view_rotatemove)
            showMouseCursor(true);

		view_rotate = false;
	}
	if (key == key_viewMove)
	{
        if(!view_rotate && !view_rotatemove)
		    showMouseCursor(true);
		view_move = false;
	}
	if (key_viewRotateMove == key)// == KEY_VMOVE)
	{
        if(!view_rotate && !view_move)
		    showMouseCursor(true);
  
		view_rotatemove = false;


	}
	//ab hier nur noch objekt-relevantes
    if(app->editingMode == EditorApp::emTerrain )
		return;
	if(key_delete == key)// == OIS::KC_DELETE)
	{
		deleteSelected();
	}
	


	if(key == key_showDetails)// == OIS::KC_RETURN)
	{

		//show details about object
		if(selectedObject)
		{
			app->showDetailsDialog(selectedObject);
		}
	}


	
/*	CEGUI::WindowManager *wmgr =
	EditFrameListener::manipulationMode m = mFrameListener->mMode;
	CEGUI::String str;
	switch(m)
	{
	case EditFrameListener::mmMove:
		mFrameListener->mMode = EditFrameListener::mmRotate;
		str = "rotate";
		break;
	case EditFrameListener::mmRotate:
		mFrameListener->mMode = EditFrameListener::mmScale;
		str = "scale";
		break;
	case EditFrameListener::mmScale:
		mFrameListener->mMode = EditFrameListener::mmMove;
		str = "move";
		break;
	}

	wmgr->getWindow("modType")->setText(str);
	return true;*/
	if(key_modeScale == key)// ==OIS::KC_S)
	{
		setTransformMode(tmScale);
		//mMode = mmScale;
		CEGUI::WindowManager::getSingletonPtr()->getWindow("modType")->setText("scale");
	}
	if(key_modeRotate == key)//==OIS::KC_R)
	{
		setTransformMode(tmRotate);
		//mMode = mmRotate;
		CEGUI::WindowManager::getSingletonPtr()->getWindow("modType")->setText("rotate");
	}
	if(key_modeMove == key)//==OIS::KC_M)
	{
		//mMode = mmMove;
		setTransformMode(tmMove);
		CEGUI::WindowManager::getSingletonPtr()->getWindow("modType")->setText("move");
	}
	if(key_movedown == key)//==OIS::KC_M)
	{
		//fallenlassen
		objectMoveDown();
	}
	if (key == key_objMove)// id == OIS::MB_Left )
	{
		CEGUI::MouseCursor::getSingleton().setImage((CEGUI::utf8*)STYLE_EDITOR,(CEGUI::utf8*)"MouseArrow");
		if(app->dragItemID != "")
		{
			//dropItem
			//HIER DROPPEN
			
			dropItem();
			return;
//			app->dragItemType=0;

		}
		if(!lastMouseDownOnGui)
		{

			if(!selBoxDrawing)
			{
				//doObjectRaycast();
				updateSelection();
				if(prevSelected != selectedObject)
				{
					prevSelected = selectedObject;
				}
				else
				{

					if(selectedObject && selectedObject->getBody())
					{
						selectedObject->updateBody();
						selectedObject->getBody()->unFreeze();
					}
				}

				//selektieren abschließen
				//prevSelected = selectedObject;
			}
			else
			{
				finishSelectionBox();
				prevSelected = selectedObject;
				//updateSelection();
			}
		}






	}

}

void EditFrameListener::createDecal()
{
    Ogre::Vector2 dsize(app->brushScale,app->brushScale);
    Ogre::String tex = "decal.png";
    decal = new DecalCursor(app->currentLevel->getSceneManager(),app->currentLevel->getTerrainMaterial(),dsize,tex);

    decal->show();
    setDecalScale(app->brushScale);
}

void EditFrameListener::dropItem()
{
	Ogre::Vector3 pos = getDropPosition();

	GameObject *placed = app->getCurrentLevel()->placeObject(app->dragItemType,app->dragItemID,pos,Ogre::Quaternion::IDENTITY);
	app->dragItemID="";
	app->updateCurObjectsList(placed,true); //hier gibts keine gruppen
}

void EditFrameListener::finishSelectionBox()
{
	selBoxDrawing = false;
    
	selBoxEnd = CEGUI::MouseCursor::getSingletonPtr()->getPosition();
	//mDebugText("box ende: start= ("+str(selBoxStart)+") => ("+str(selBoxEnd)+")");
	app->hideSelBorder();
	if(selectionLocked)
        return;
    //ok, hier jetzt der raycast
	/*
	1---2
	|   |
	3---4
	wobei 1=position
	und 4= pos+size
	*/
	Ogre::Real wWidth = app->mWindow->getWidth();
	Ogre::Real wHeight= app->mWindow->getHeight();
	
	CEGUI::URect rect = app->selectionBorder->getArea();
	//rect.
	CEGUI::UVector2 pos = app->selectionBorder->getPosition();
	CEGUI::UVector2 size= app->selectionBorder->getSize();
	//mDebugText("width="+str(size.d_x.d_offset)+"\nheight="+str(size.d_y.d_offset));
	if(size.d_x.d_offset < 5 || size.d_y.d_offset < 5)
		return;
	Ogre::Vector2 point1(pos.d_x.d_offset / wWidth,pos.d_y.d_offset / wHeight);
	Ogre::Vector2 point4((size.d_x.d_offset+pos.d_x.d_offset) / wWidth,(size.d_y.d_offset+pos.d_y.d_offset) / wHeight);
	Ogre::Vector2 point2(point4.x,point1.y);
	Ogre::Vector2 point3(point1.x,point4.y);
	//jetzt 4 rays anhand der punkte
	Ogre::Camera *cam = app->currentLevel->getMainCam();
	Ogre::Ray r1 = cam->getCameraToViewportRay(point1.x,point1.y);
	Ogre::Ray r2 = cam->getCameraToViewportRay(point2.x,point2.y);
	Ogre::Ray r3 = cam->getCameraToViewportRay(point3.x,point3.y);
	Ogre::Ray r4 = cam->getCameraToViewportRay(point4.x,point4.y);
	//jetzt daraus 4 ebenen bauen...
	/*
	r1-p2-r2
	|      |
    p1    p3
	|      |
	r3-p4-r4
	*/
	Ogre::Plane p1(r1.getOrigin(),r1.getPoint(100),r3.getPoint(50));
	Ogre::Plane p2(r1.getOrigin(),r1.getPoint(100),r2.getPoint(50));
	Ogre::Plane p3(r2.getOrigin(),r2.getPoint(100),r4.getPoint(50));
	Ogre::Plane p4(r3.getOrigin(),r3.getPoint(100),r4.getPoint(50));
	//p5 sollte genau vor der cam stehen
	Ogre::Plane p5(r1.getOrigin(),r2.getOrigin(),r3.getOrigin());
	//schauen, ob die normalen alle nach innen zeigen
	if(p1.getSide(r2.getPoint(10)) == Ogre::Plane::NEGATIVE_SIDE)
	{
		//das sollte die normale umdrehen und den aufpunkt auf die andere seite schieben, sodass es passt
		p1.d *= -1;
		p1.normal *= -1;
	}
	if(p2.getSide(r4.getPoint(10)) == Ogre::Plane::NEGATIVE_SIDE)
	{
		//das sollte die normale umdrehen und den aufpunkt auf die andere seite schieben, sodass es passt
		p2.d *= -1;
		p2.normal *= -1;
	}
	if(p3.getSide(r1.getPoint(10)) == Ogre::Plane::NEGATIVE_SIDE)
	{
		//das sollte die normale umdrehen und den aufpunkt auf die andere seite schieben, sodass es passt
		p3.d *= -1;
		p3.normal *= -1;
	}
	if(p4.getSide(r2.getPoint(10)) == Ogre::Plane::NEGATIVE_SIDE)
	{
		//das sollte die normale umdrehen und den aufpunkt auf die andere seite schieben, sodass es passt
		p4.d *= -1;
		p4.normal *= -1;
	}
	if(p5.getSide(r2.getPoint(10)) == Ogre::Plane::NEGATIVE_SIDE)
	{
		//das sollte die normale umdrehen und den aufpunkt auf die andere seite schieben, sodass es passt
		p5.d *= -1;
		p5.normal *= -1;
	}

	////debugtest
	//Ogre::SceneNode *debugNode = app->getCurrentLevel()->getSceneManager()->getRootSceneNode()->createChildSceneNode();
	////linien

	//DynamicLines *lines = new DynamicLines(RenderOperation::OT_LINE_LIST);
	//lines->addPoint(r1.getOrigin());
	//lines->addPoint(r1.getPoint(100));
	//lines->addPoint(r2.getOrigin());
	//lines->addPoint(r2.getPoint(100));
	//lines->addPoint(r4.getOrigin());
	//lines->addPoint(r4.getPoint(100));
	//lines->addPoint(r3.getOrigin());
	//lines->addPoint(r3.getPoint(100));
 //     
 //   lines->update();
 //     
 //   debugNode->attachObject(lines);
	////ebenen
	//Ogre::MeshManager::getSingleton().createPlane("debugPlane1",
	//   Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, p1,
	//   50,50,1,1,true,1,1,1,p1.normal.perpendicular());
	//Ogre::MeshManager::getSingleton().createPlane("debugPlane2",
	//   Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, p2,
	//   50,50,1,1,true,1,1,1,p2.normal.perpendicular());
	//Ogre::MeshManager::getSingleton().createPlane("debugPlane3",
	//   Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, p3,
	//   50,50,1,1,true,1,1,1,p3.normal.perpendicular());
	//Ogre::MeshManager::getSingleton().createPlane("debugPlane4",
	//   Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, p4,
	//   50,50,1,1,true,1,1,1,p4.normal.perpendicular());
	//Ogre::MeshManager::getSingleton().createPlane("debugPlane5",
	//   Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, p5,
	//   50,50,1,1,true,1,1,1,p5.normal.perpendicular());
	//Ogre::Entity *e1 = app->getCurrentLevel()->getSceneManager()->createEntity("debugent1","debugPlane1");
	//e1->setMaterialName("Examples/RustySteel");
	//debugNode->attachObject(e1);
	//Ogre::Entity *e2 = app->getCurrentLevel()->getSceneManager()->createEntity("debugent2","debugPlane2");
	//e2->setMaterialName("Examples/OgreLogo");
	//debugNode->attachObject(e2);
	//Ogre::Entity *e3 = app->getCurrentLevel()->getSceneManager()->createEntity("debugent3","debugPlane3");
	//e3->setMaterialName("Examples/Rocky");
	//debugNode->attachObject(e3);
	//Ogre::Entity *e4 = app->getCurrentLevel()->getSceneManager()->createEntity("debugent4","debugPlane4");
	//e4->setMaterialName("Examples/GrassFloor");
	//debugNode->attachObject(e4);
	//Ogre::Entity *e5 = app->getCurrentLevel()->getSceneManager()->createEntity("debugent5","debugPlane5");
	//e5->setMaterialName("Examples/Rockwall");
	//debugNode->attachObject(e5);
	//return;
	Ogre::PlaneBoundedVolume vol; 
	vol.planes.push_back(p1);
	vol.planes.push_back(p2);
	vol.planes.push_back(p3);
	vol.planes.push_back(p4);
	vol.planes.push_back(p5);
	Ogre::PlaneBoundedVolumeList vl;
	vl.push_back(vol);

	Ogre::PlaneBoundedVolumeListSceneQuery *qry = app->currentLevel->getSceneManager()->createPlaneBoundedVolumeQuery(vl);
	GroupObject *gr = NULL;
	if(isKeyDown(key_group) && selectedObject)
	{
		if(selectedObject->getType() == otGroup)
		{
			//adden
			gr = (GroupObject*)selectedObject;
		}
	}
	GroupingQueryListener *listener = new GroupingQueryListener(app->currentLevel,gr);
	qry->execute(listener);
	setSelected(listener->group);


}

Ogre::Vector3 EditFrameListener::getDropPosition()
{
	CEGUI::Point mousepos =	CEGUI::MouseCursor::getSingleton().getPosition();
	Ogre::Camera *mCamera = app->getCurrentLevel()->getMainCam();//mSceneMgr->getCamera("PlayerCam");
	Ogre::Real mouseX = mousepos.d_x/mWindow->getWidth();
	Ogre::Real mouseY = mousepos.d_y/mWindow->getHeight();
	Ogre::Ray mouseRay = mCamera->getCameraToViewportRay( mouseX, mouseY );

	Ogre::Vector3 pos = mouseRay.getPoint(10);
	return pos;
}


bool EditFrameListener::keyPressed(const OIS::KeyEvent &arg)
{

	CEGUI::System::getSingleton().injectKeyDown(arg.key);

	
	CEGUI::System::getSingleton().injectChar(convertOISNumpadToCegui(arg.key,arg.text));
	CEGUI::Window *wnd = getActiveWindow();
	/*if(wnd)
		mDebugText(wnd->getName().c_str());*/
	/*if(test)
		return true;*/ //verhindert, dass man den transformationsmodus ändert, wenn man tippt
	//do not do stuff if there is no lvl
	/*if(!app->getCurrentLevel())
		return true;*/
	//if(app->okToProcessEvent)
	//{
	//	app->okToProcessEvent = false;
	//}
	//else
	//{
	//	return;
	//}

	//if(arg.key == OIS::KC_X)
	//{
	//	x_pressed = true;
	//}
	//if(arg.key == OIS::KC_Y)
	//{
	//	y_pressed = true;
	//	//app->setDebugText("y");
	//}
	//if(arg.key == OIS::KC_Z)
	//{
	//	z_pressed = true;
	//}
	//if(arg.key == OIS::KC_S)
	//{
	//	obj_scale = true;
	//}
	//zum testen
	

	UniKey cur = arg.key;		
	
	if(mKeyboard->isModifierDown(OIS::Keyboard::Shift) && (arg.key != OIS::KC_LSHIFT && arg.key != OIS::KC_RSHIFT))
	{
//		cur.useModifier = true;
		cur.modifier = OIS::Keyboard::Shift;	
	}
	if(mKeyboard->isModifierDown(OIS::Keyboard::Ctrl) && (arg.key != OIS::KC_LCONTROL && arg.key != OIS::KC_RCONTROL))
	{
	//	cur.useModifier = true;
		cur.modifier = OIS::Keyboard::Ctrl;
	}
	if(mKeyboard->isModifierDown(OIS::Keyboard::Alt) && (arg.key != OIS::KC_LMENU && arg.key != OIS::KC_RMENU))
	{
	//	cur.useModifier = true;
		cur.modifier = OIS::Keyboard::Alt;
	}
	
	uniKeyPressed(cur);
	

	return true;
}

bool EditFrameListener::keyReleased(const OIS::KeyEvent &arg)
{

	CEGUI::System::getSingleton().injectKeyUp(arg.key);
	//do not do stuff if there is no lvl
	//do not do stuff if there is no lvl

	/*if(!app->getCurrentLevel())
		return true;*/

	UniKey cur = arg.key;		
	
	if(mKeyboard->isModifierDown(OIS::Keyboard::Shift))
		cur.modifier = OIS::Keyboard::Shift;	
	if(mKeyboard->isModifierDown(OIS::Keyboard::Ctrl))
		cur.modifier = OIS::Keyboard::Ctrl;
	if(mKeyboard->isModifierDown(OIS::Keyboard::Alt))
		cur.modifier = OIS::Keyboard::Alt;
	
	uniKeyReleased(cur);



	return true;
}

   // MouseDragged
bool EditFrameListener::mouseMoved(const OIS::MouseEvent &arg)
{
    
    //mDebugText("x="+str(arg.state.X.abs)+"\ny="+str(arg.state.Y.abs));
    //CEGUI::MouseCursor::getSingletonPtr()->show();
    //CEGUI::MouseCursor::getSingletonPtr()->setPosition(CEGUI::Point(arg.state.X.abs,arg.state.Y.abs));
    
    //TEST!
    
    //CEGUI::Window* mouseOverWin = CEGUI::System::getSingleton().getWindowContainingMouse();
    //if (mouseOverWin == NULL) 
    //   mDebugText("kein fenster");
    //else
    ////CEGUI::String type = mouseOverWin->getType();
    //mDebugText(mouseOverWin->getName().c_str());
	mousePosChanged = true;
    
	if(cursorShown)
	{
        CEGUI::System *sys = CEGUI::System::getSingletonPtr();
    
        sys->injectMousePosition(arg.state.X.abs,arg.state.Y.abs);
		
		/*sys->injectMouseMove(
					arg.state.X.rel,
					arg.state.Y.rel);*/
        
        //sys->setmouse
		//app->setDebugText(str(arg.state.Z.rel));
		sys->injectMouseWheelChange(arg.state.Z.rel/120);

		////do not do stuff if there is no lvl
		//if(!app->getCurrentLevel())
		//	return true;

	}
    else
    {
        /*CEGUI::MouseCursor *mouse = CEGUI::MouseCursor::getSingletonPtr();
        CEGUI::Point pos = mouse->getPosition();
        mMouse->
        arg.state.X.abs = pos.d_x;
        arg.state.Y.abs = pos.d_y;*/
    }
    
	//CEGUI::Point mp = CEGUI::MouseCursor::getSingletonPtr()->getPosition();
	//if(mp.d_x == 0 || mp.d_x == mWindow->getWidth()-1 || mp.d_y == 0 || mp.d_y == mWindow->getHeight()-1)
	//{
	//	//fenster defocussen
	//	
	//	Ogre::Root::
	//}
	//mDebugText(str(mp.d_x)+"\n"+str(mp.d_y));
	

	if(app->dragItemID != "")
		return true;
    Level *lvl = app->currentLevel;
    if(app->editingMode == EditorApp::emTerrain && lvl->getTerrainManager())
    {
        
        //editierposition anpassen
        Ogre::Real mouseX = 0.5;
        Ogre::Real mouseY = 0.5;
		
		
        
		CEGUI::Point pt = CEGUI::MouseCursor::getSingletonPtr()->getPosition();
		mouseX = pt.d_x/app->getRenderWindow()->getWidth();
		mouseY = pt.d_y/app->getRenderWindow()->getHeight();
		

		Ogre::Ray mouseRay = lvl->getMainCam()->getCameraToViewportRay(mouseX,mouseY);	


		
		// since ETM is no longer a scene manager, we don't use a ray scene query,
		// but instead query the terrain info directly
		std::pair<bool, Ogre::Vector3> result = lvl->getTerrainInfo()->rayIntersects(mouseRay);
		
		if(result.first)
		{
            deformPosition = result.second;
            //deformPosition = lvl->toTerrainPosition(deformPosition);
            //deformPosition = lvl->toOgrePosition(deformPosition);
            //Ogre::Vector3 tScale = lvl->getTerrainScale();
            //deformPosition += tScale/2;
            //deformPosition.x = lvl->getTerrainInfo()->posToVertexX(deformPosition.x);
		    //deformPosition.z = lvl->getTerrainInfo()->posToVertexZ(deformPosition.z);
            //deformPosition *= tScale;
            //deformPosition += lvl->getTerrainExtends().getMinimum();
           //s mLog("test");

        }
    }

	if(isMouseOverGui() && cursorShown)
	{
		if(!wasOverGui)
		{
			//maustasten "heben"
			if(arg.state.buttonDown(OIS::MB_Left))
			{
				uniKeyReleased(OIS::MB_Left);
			}
			if(arg.state.buttonDown(OIS::MB_Right))
			{
				uniKeyReleased(OIS::MB_Right);
			}
		}

		wasOverGui = true;
		return true;
	}
	

	//do not do stuff if there is no lvl
	if(!app->getCurrentLevel())
			return true;

	Ogre::SceneNode *mCamNode = app->getCurrentLevel()->getSceneManager()->getSceneNode("CamNode");

	if(arg.state.Z.rel)
	{
		mCamNode->translate( mCamNode->getChild( 0 )->_getDerivedOrientation() * Ogre::Vector3(0,0,-1) * arg.state.Z.rel * mScroll);// * lastFrameTime );
		
	}

	if(view_rotate)
	{


		//Ogre::Real muh = 75;
		mCamNode->yaw( Ogre::Degree( -arg.state.X.rel*mRotate ) );
		mCamNode->getChild( 0 )->pitch( Ogre::Degree( -arg.state.Y.rel*mRotate ) );



		//CEGUI::MouseCursor::getSingleton().setPosition(CEGUI::Point(e->getX(),e->getY()));

	}
	if(view_move)
	{

		Ogre::Vector3 transVector = Ogre::Vector3(0,0,0);

		transVector.x = -arg.state.X.rel;
		transVector.y = arg.state.Y.rel;

		mCamNode->translate( mCamNode->getChild( 0 )->_getDerivedOrientation() * transVector * mMove);
		


	}
	if(view_rotatemove)
	{
        Ogre::Degree yawValue = Ogre::Degree( -arg.state.X.rel*mRotate );
		Ogre::Degree pitchValue = Ogre::Degree( -arg.state.Y.rel*mRotate );
        if(!selectedObject)
        {
		    
		    mCamNode->yaw( yawValue );
		    mCamNode->getChild( 0 )->pitch( pitchValue );
        }
        else
        {
            Ogre::Quaternion oldOrnt = mCamNode->getOrientation();
            Ogre::Vector3 relZaxis = oldOrnt * Ogre::Vector3::UNIT_X;
            relZaxis.y = 0;
            relZaxis.normalise();
            Ogre::Vector3 targetPos;
            Ogre::Quaternion targetOrnt;
            selectedObject->getPositionOrientation(targetPos,targetOrnt);
            Ogre::Quaternion ornt = Ogre::Quaternion(yawValue,Ogre::Vector3::UNIT_Y) * Ogre::Quaternion(pitchValue,relZaxis);
            Ogre::Quaternion rotBy = Ogre::Quaternion(yawValue,Ogre::Vector3::UNIT_Y);// * targetOrnt.Inverse();
            Ogre::Quaternion rotByChild = Ogre::Quaternion(pitchValue,Ogre::Vector3::UNIT_Z);// * targetOrnt.Inverse();

            Ogre::SceneNode *mCamHeadNode = static_cast<Ogre::SceneNode*>(mCamNode->getChild(0));

            //Ogre::Quaternion rotBy1 = Ogre::Quaternion(yawValue,Ogre::Vector3::UNIT_Y) * 
            
            Ogre::Vector3 relPos = mCamNode->getPosition()-targetPos;
            Ogre::Vector3 newPosRel = ornt * relPos;
        
            mCamNode->setPosition(newPosRel + targetPos);

            Ogre::Quaternion camNodeOrnt = ornt * mCamNode->getOrientation();
            
            mCamNode->yaw( yawValue );
            mCamNode->getChild( 0 )->pitch( pitchValue );

            //mCamHeadNode->setDirection(-relPos,Ogre::Node::TS_WORLD);
            //Ogre::Vector3 targetDir = targetPos - 
            //ornt;// = Ogre::Quaternion(pitchValue,Ogre::Vector3::NEGATIVE_UNIT_X);
            //rotBy = ornt * targetOrnt.Inverse();
            //rotByChild = Ogre::Vector3::NEGATIVE_UNIT_Z.getRotationTo(newPosRel);
            //Ogre::Quaternion camNodeChildOrnt = rotByChild * mCamHeadNode->getOrientation();
            //mCamHeadNode->setDirection(newPosRel);
           // mCamNode->setOrientation(camNodeOrnt);
           // mCamHeadNode->setOrientation(camNodeChildOrnt); //setOrientation(rotByChild *  mCamHeadNode->getOrientation());
		    //mCamNode->getChild( 0 )->setOrientation(camNodeChildOrnt);
            //static_cast<Ogre::SceneNode*>(mCamNode->getChild( 0 ))->lookAt(
            /*
            Ogre::Degree yawValue = Ogre::Degree( -arg.state.X.rel*mRotate );
		    Ogre::Degree pitchValue = Ogre::Degree( -arg.state.Y.rel*mRotate );
            rotateMoveNode->yaw( -yawValue );
		    rotateMoveNode->pitch( -pitchValue );*/
        }
    }


	//ab dieser Stelle nur noch objekteditingrelevantes
	if(app->editingMode == EditorApp::emTerrain)
		return true;

	if(selectedObject)
	{
		if(isKeyDown(key_objMove) && selectedObject == prevSelected && curTransform != EditFrameListener::tNone)
		{
			switch(mTMode)
			{
			case tmMove:
				objectMove();
				break;
			case tmRotate:
				objectRotate();
				break;
			case tmScale:
				objectScale();
			}
			updateObjectInfo();
			

			//if(mMode == mmRotate)
			//{
			//	objectRotate();
			//}
			//else if(mMode == mmScale)
			//{
			//	objectScale();
			//}
			//else
			//{
			//	objectMove();
			//}
		}
	}
	/*else
	{
		
	}*/
	//mDebugText(str(curTransform));
	if(isKeyDown(key_objMove) && curTransform == tNone && !wasOverGui)
	{
		drawSelectionBox();
	}

	wasOverGui = false;

	return true;

}

void EditFrameListener::drawSelectionBox()
{
	if(selectionLocked)
        return;
	if(!selBoxDrawing)
	{
	//	mDebugText("selectionbox begin");
		selBoxDrawing = true;
		app->initSelBorder(selBoxStart);
	}
	app->updateSelBorder(CEGUI::MouseCursor::getSingletonPtr()->getPosition());

}
//void EditFrameListener::mouseDragged()
//{
//
//	//do not do stuff if there is no lvl
//	if(!app->getCurrentLevel())
//		return;
//	//no need to do anything if nothing selected
//	if(!selectedObject)
//		return;
//	if(isKeyDown(key_objMove) && selectedObject == prevSelected)
//	{
//
//		if(mMode == mmRotate)
//		{
//			objectRotate();
//		}
//		else if(mMode == mmScale)
//		{
//			objectScale();
//		}
//		else
//		{
//			objectMove();
//		}
//	}
//
//
//
//
//
//
//
//
////	//app->log(Ogre::StringConverter::toString(Ogre::Degree(-mRotate * m_InputReader->getMouseRelativeX())));
//}



bool EditFrameListener::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{


	CEGUI::System::getSingleton().injectMouseButtonDown(
	convertOISMouseButtonToCegui(id));
	//do not do stuff if there is no lvl
	if(!app->getCurrentLevel())
		return true;
	/*if(app->okToProcessEvent)
	{
		app->okToProcessEvent = false;
	}
	else
	{
		return true;
	}*/
	if(isMouseOverGui())
	{
		/*if(selectedObject)
		{
			setSelected(NULL);
		}*/
		lastMouseDownOnGui = true;
		return true;
	}
	lastMouseDownOnGui = false;
	//CEGUI::System::getSingletonPtr()->getGUISheet()->activate();
	deactivateWindow();
//	mDebugText("root activated");
	mousePosChanged = false;
	//app->setDebugText("FL: mousedown");

	//OIS::MouseState mouseState = mMouse->getMouseState();
	UniKey cur = id;		
	
	if(mKeyboard->isModifierDown(OIS::Keyboard::Shift))
		cur.modifier = OIS::Keyboard::Shift;	
	if(mKeyboard->isModifierDown(OIS::Keyboard::Ctrl))
		cur.modifier = OIS::Keyboard::Ctrl;
	if(mKeyboard->isModifierDown(OIS::Keyboard::Alt))
		cur.modifier = OIS::Keyboard::Alt;
	
	uniKeyPressed(cur);

	//if ( id == OIS::MB_Left )
	//{
	//	//settext("links unten");
	//	LeftMouseDown = true;
	//	moveMode = MM_MOVE_XZ;
	//}
	//if ( id == OIS::MB_Right )
	//{
	//	//settext("rechts unten");
	//	obj_rotate = true;
	//	rotateStartDoOnce = true;
	//	RightMouseDown = true;
	//}



		return true;

}
//void EditFrameListener::mousePressed(MouseEvent* e)
//{
//	CEGUI::System::getSingleton().injectMouseButtonDown(
//		convertOgreButtonToCegui(e->getButtonID()));
//	if(app->okToProcessEvent)
//	{
//		app->okToProcessEvent = false;
//	}
//	else
//	{
//		return;
//	}
//	//app->log("FL: mousedown");
//	if(!LeftMouseDown && !RightMouseDown)
//	{
//		//if(e->getButtonID() & MouseEvent::BUTTON0_MASK )
//		//{
//
//			holdingOffset = Ogre::Vector3(0,0,0);
//			intersectionPoint = Ogre::Vector3(0,0,0);
//			if(selectedNode)
//				selectedNode->showBoundingBox(false);
//			selectedBody = 0;
//			selectedNode = 0;
//			//linksklick
//			// Setup the ray scene query
//			CEGUI::Point mousepos =	CEGUI::MouseCursor::getSingleton().getPosition();
//			Ogre::Camera *mCamera = mSceneMgr->getCamera("PlayerCam");
//			Ogre::Real mouseX = mousepos.d_x/mRenderWindow->getWidth();
//			Ogre::Real mouseY = mousepos.d_y/mRenderWindow->getHeight();
//			Ogre::Ray mouseRay = mCamera->getCameraToViewportRay( mouseX, mouseY );
//			mRaySceneQuery->setRay( mouseRay );
//
//			mRaySceneQuery->setSortByDistance(true);
//
//		//   Ogre::Vector3 rayOrigin = mouseRay.getOrigin();
//		//   Ogre::Vector3 rayDirection = mouseRay.getDirection();
//
//		   // Execute query
//			RaySceneQueryResult &result = mRaySceneQuery->execute();
//
//			app->setDebugText("NEU: \n");
//
//			Ogre::String nearest = "";
//			if(selectedNode)
//			   selectedNode->showBoundingBox(false);
//
//			Ogre::Real closest_distance = -1.0f;
//		//	Ogre::Real notrly_closest_dist = -1.0f;
//			Ogre::Vector3 closest_result;
//			if(result.begin() != result.end())
//			{
//				for(RaySceneQueryResult::iterator itr = result.begin( );
//				itr != result.end();itr++)
//				{
//					Ogre::String name = itr->movable->getName();
//
//					if(itr->movable)
//					{
//
//						// app->setDebugText(itr->movable->getMovableType()+" : "+name,true);
//						if(itr->movable->getMovableType() == "Ogre::Entity")
//						{
//							//if(itr->distance < closest_distance || closest_distance == -1)
//							//{
//							//// //for finding first object
//							// nearest = name;
//							// closest_distance = itr->distance;
//							//}
//							//now try the raycast to the polygon thingy
//							//polygon check BEGIN-------------------------------------
//							// get the entity to check
//							Ogre::Entity *Ent = static_cast<Ogre::Entity*>(itr->movable);
//
//							// mesh data to retrieve
//							size_t vertex_count;
//							size_t index_count;
//							Ogre::Vector3 *vertices;
//							unsigned long *indices;
//
//							// get the mesh information
//							Ogre::MeshPtr temp = Ent->getMesh();
//							Ogre::Mesh *myMesh = temp.getPointer();
//
//							getMeshInformation(myMesh, vertex_count, vertices, index_count, indices,
//							Ent->getParentNode()->_getDerivedPosition(),
//							Ent->getParentNode()->_getDerivedOrientation(),
//							Ent->getParentNode()->getScale());
//
//							// test for hitting individual triangles on the mesh
//							bool new_closest_found = false;
//							for (int i = 0; i < static_cast<int>(index_count); i += 3)
//							{
//								// check for a hit against this triangle
//								std::pair<bool, Ogre::Real> hit = Ogre::Math::intersects(mouseRay, vertices[indices[i]],
//								vertices[indices[i+1]], vertices[indices[i+2]], true, false);
//
//								// if it was a hit check if its the closest
//								if (hit.first)
//								{
//									if ((closest_distance < 0.0f) ||
//									(hit.second < closest_distance))
//									{
//										// this is the closest so far, save it off
//										closest_distance = hit.second;
//									//	notrly_closest_dist = itr->distance;
//										new_closest_found = true;
//									}
//								}
//							}
//
//							// free the verticies and indicies memory
//							delete[] vertices;
//							delete[] indices;
//
//							// if we found a new closest raycast for this object, update the
//							// closest_result before moving on to the next object.
//							if (new_closest_found)
//							{
//								//closest_result = mouseRay.getPoint(closest_distance);
//								nearest = itr->movable->getName();
//								app->setDebugText("new closest: "+nearest+"\n",true);
//							}
//							//polygon check END--------------------------
//
//
//						}//von if(entity
//
//					}
//					else
//					{
//						//no movable
//					}
//					//app->setDebugText("="+Ogre::StringConverter::toString(itr->distance)+"\n",true);
//
//
//
//
//
//
//				}//for(RaySceneQueryResult::iterator itr = result.begin( );...
//				app->setDebugText("nearest: "+app->getObjIdByEntityName(nearest)+"\n",true);
//				intersectionPoint = mouseRay.getPoint(closest_distance);
//				/*intersectionPoint = mouseRay.getPoint(notrly_closest_dist);*/
//				Ogre::Vector3 objectPosition;
//
//				selectedBody = app->getBodyByEntityName(nearest);
//				selectedNode = (Ogre::SceneNode*)mSceneMgr->getEntity(nearest)->getParentNode();
//				Ogre::Quaternion dummy;
//				if(!selectedBody)
//				{
//					objectPosition = selectedNode->getPosition();
//					app->setDebugText("kein Body WTF?!",true);
//					startOrientation = selectedNode->getOrientation();
//				}
//				else
//				{
//					selectedBody->getPositionOrientation(objectPosition,startOrientation);
//					selectedBody->unFreeze();
//				}
//				holdingOffset = intersectionPoint - objectPosition;
//
//				selectedNode->showBoundingBox(true);
//			}//if(result.begin != result.end())
//
//
//		   // Get results, create a node/entity on the position
//
//
//
//		mRaySceneQuery->clearResults();
//
//	//	}//if(mousebutton0...
//	}//leftmousedown
//
//if ( e->getButtonID() & MouseEvent::BUTTON0_MASK )
//{
//	//settext("links unten");
//	LeftMouseDown = true;
//	moveMode = MM_MOVE_XZ;
//}
//if ( e->getButtonID() & MouseEvent::BUTTON1_MASK )
//{
//	//settext("rechts unten");
//	obj_rotate = true;
//	rotateStartDoOnce = true;
//	RightMouseDown = true;
//}
//
//
//
//		e->consume();
//
//}

bool EditFrameListener::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    flattenHeight = -1;
	//app->setDebugText("mouse up\n");
	CEGUI::System::getSingleton().injectMouseButtonUp(
         convertOISMouseButtonToCegui(id));
	//do not do stuff if there is no lvl

	/*if(app->okToProcessEvent)
	{
		app->okToProcessEvent = false;
	}
	else
	{
		return true;
	}*/
	//updated die lightmap
	if(app->editingMode == EditorApp::emTerrain && app->getCurrentLevel() && mustUpdateLightmap)
	{
		app->getCurrentLevel()->updateTerrainLightmap();
        mustUpdateLightmap = false;
	}
	if(isMouseOverGui())
	{
		if(selBoxDrawing)
		{
			selBoxDrawing = false;
			app->hideSelBorder();
		}

		app->dragItemID="";
//		app->dragItemType=0;

		return true;
	}
	if(app->dragItemID == "" && lastMouseDownOnGui)
	{
		return true;
	}
	if(!app->getCurrentLevel())
		return true;

	UniKey cur = id;		
	
	if(mKeyboard->isModifierDown(OIS::Keyboard::Shift))
		cur.modifier = OIS::Keyboard::Shift;	
	if(mKeyboard->isModifierDown(OIS::Keyboard::Ctrl))
		cur.modifier = OIS::Keyboard::Ctrl;
	if(mKeyboard->isModifierDown(OIS::Keyboard::Alt))
		cur.modifier = OIS::Keyboard::Alt;
	
	uniKeyReleased(cur);
//	if ( id == OIS::MB_Right )
//	{
//		//app->setDebugText("right up",true);
//		//settext("rechts unten");
////		obj_rotate = false;
//		RightMouseDown = false;
//	}


	//if(app->menuMode <= 1)
	//{
	//	if ( e->getButtonID() & MouseEvent::BUTTON0_MASK )
	//	{
	//		//settext("links unten");
	//	}
	//	if ( e->getButtonID() & MouseEvent::BUTTON1_MASK )
	//	{
	//		//settext("rechts unten");
	//	}
	//}
	//else
	//{


	//}
	return true;
}

void EditFrameListener::updatePositionInfoBox(Ogre::Vector3 pos)
{}
void EditFrameListener::updateRotationInfoBox(Ogre::Real deg,short axis)
{}


void EditFrameListener::windowMoved(Ogre::RenderWindow* rw)
{

}

void EditFrameListener::windowResized(Ogre::RenderWindow* rw)
{
	unsigned int width, height, depth;
	int left, top;
	rw->getMetrics(width, height, depth, left, top);

	const OIS::MouseState &ms = mMouse->getMouseState();
	ms.width = width;
	ms.height = height;

    //app->getRoot()->getren
    app->getGuiRenderer()->setDisplaySize(CEGUI::Size(width, height));
    //CEGUI::System::getRenderer
    //CEGUI::System::getSingletonPtr()->getRenderer()->setd//getGUISheet()->setSize(CEGUI::UVector2(CEGUI::UDim(0,width),CEGUI::UDim(0,height)));
}


void EditFrameListener::windowClosed(Ogre::RenderWindow* rw)
{
	//Only close for window that created OIS (the main window in these demos)
	if( rw == mWindow )
	{
		if( mInputManager )
		{

			mInputManager->destroyInputObject( mKeyboard );

			OIS::InputManager::destroyInputSystem(mInputManager);
			mInputManager = 0;
		}
	}
}


void EditFrameListener::windowFocusChange(Ogre::RenderWindow* rw)
{
    
}

void EditFrameListener::levelChanged()
{
	
	selectedObject = NULL;
	//erstmal das mit dem callback für die renderqueue usw...
	Ogre::SceneManager *mgr = app->getCurrentLevel()->getSceneManager();
	if(!rqListener)
	{
	
		rqListener = new MyRenderQueueListener();
	}
	
		
	mgr->addRenderQueueListener(rqListener);
}

void EditFrameListener::objectMove()
{
	CEGUI::Point mousepos =	CEGUI::MouseCursor::getSingleton().getPosition();

	Ogre::Real mouseX = mousepos.d_x/mWindow->getWidth();
	Ogre::Real mouseY = mousepos.d_y/mWindow->getHeight();

	CEGUI::MouseCursor::getSingleton().setImage((CEGUI::utf8*)STYLE_EDITOR,(CEGUI::utf8*)"MouseMoveCursor");


	Ogre::Camera *mCamera = app->getCurrentLevel()->getMainCam();
	Ogre::Ray mouseRay = mCamera->getCameraToViewportRay( mouseX, mouseY );
	Ogre::Vector3 rayOrigin = mouseRay.getOrigin();
	Ogre::Vector3 rayDirection = mouseRay.getDirection();
	Ogre::Vector3 newPosition, oldPosition;
	oldPosition = selectedObject->getPosition();

	Ogre::Vector3 pointOnPlane = intersectionPoint;

	Ogre::Vector3 hOffset = holdingOffset;
	//mDebugText(str(rayDirection));
	switch(curTransform)
	{
	case tAxisX:
		if(Ogre::Math::Abs(rayDirection.z) > 0.1)
		{
			//unlikely, but possible
			Ogre::Real lambda = (intersectionPoint.z - rayOrigin.z)/rayDirection.z;
			pointOnPlane.x=rayOrigin.x+lambda*rayDirection.x;
			//hOffset.z = pointOnPlane.z;

		}
		else
		{
			//just take the other plane
			Ogre::Real lambda = (intersectionPoint.y - rayOrigin.y)/rayDirection.y;
			pointOnPlane.x=rayOrigin.x+lambda*rayDirection.x;
			//hOffset.y = pointOnPlane.y;

		}
		newPosition = pointOnPlane-hOffset/*holdingOffset*/;
		break;
	case tAxisY:
		/*
		IDEA:
		use the YX or YZ plane, and check the ray to intersect. then use only
		the y coord of the intersection point
		*/
		//Ogre::Vector3 pointOnPlane = intersectionPoint;

		if(Ogre::Math::Abs(rayDirection.z) > 0.1)
		{
			//unlikely, but possible
			Ogre::Real lambda = (intersectionPoint.z - rayOrigin.z)/rayDirection.z;
			pointOnPlane.y=rayOrigin.y+lambda*rayDirection.y;
			//hOffset.z = pointOnPlane.z;


		}
		else
		{
			//just take the other plane
			Ogre::Real lambda = (intersectionPoint.x - rayOrigin.x)/rayDirection.x;
			pointOnPlane.y=rayOrigin.y+lambda*rayDirection.y;
			//hOffset.x = pointOnPlane.x;

		}
		newPosition = pointOnPlane-hOffset/*holdingOffset*/;
		break;
	case tAxisZ:
		if(Ogre::Math::Abs(rayDirection.y) > 0.1)
		{
			//unlikely, but possible
			Ogre::Real lambda = (intersectionPoint.y - rayOrigin.y)/rayDirection.y;
			pointOnPlane.z=rayOrigin.z+lambda*rayDirection.z;
			//hOffset.y = pointOnPlane.y;


		}
		else
		{
			//just take the other plane
			Ogre::Real lambda = (intersectionPoint.x - rayOrigin.x)/rayDirection.x;
			pointOnPlane.z=rayOrigin.z+lambda*rayDirection.z;
			//hOffset.x = pointOnPlane.x;

		}
		newPosition = pointOnPlane-hOffset/*holdingOffset*/;
		break;
	case tPlaneXY:
		{
			Ogre::Vector3 pointOnPlane = Ogre::Vector3::ZERO;
			//ray = origin + lambda * direction
			pointOnPlane.z=intersectionPoint.z;
			Ogre::Real lambda = (intersectionPoint.z - rayOrigin.z)/rayDirection.z;
			pointOnPlane.x=rayOrigin.x+lambda*rayDirection.x;
			pointOnPlane.y=rayOrigin.y+lambda*rayDirection.y;

			//hOffset.y = intersectionPoint.y;

			//holdingOffset = intersectionPoint - objectPosition;
			newPosition = pointOnPlane-hOffset/*holdingOffset*/;
		}
		break;
	case tPlaneXZ:
		{
			//create a plane through the intersection point. erstmal y = objectINtersection.y
			//calc the ray's intersection with this point
			//move the first ray/obj intersection to the ray/plane intersection point


			Ogre::Vector3 pointOnPlane = Ogre::Vector3::ZERO;
			//ray = origin + lambda * direction
			pointOnPlane.y=intersectionPoint.y;
			Ogre::Real lambda = (intersectionPoint.y - rayOrigin.y)/rayDirection.y;
			pointOnPlane.x=rayOrigin.x+lambda*rayDirection.x;
			pointOnPlane.z=rayOrigin.z+lambda*rayDirection.z;

			//hOffset.y = intersectionPoint.y;

			//holdingOffset = intersectionPoint - objectPosition;
			newPosition = pointOnPlane-hOffset/*holdingOffset*/;
		}
		break;
	case tPlaneYZ:
		{
			Ogre::Vector3 pointOnPlane = Ogre::Vector3::ZERO;
			//ray = origin + lambda * direction
			pointOnPlane.x=intersectionPoint.x;
			Ogre::Real lambda = (intersectionPoint.x - rayOrigin.x)/rayDirection.x;
			pointOnPlane.z=rayOrigin.z+lambda*rayDirection.z;
			pointOnPlane.y=rayOrigin.y+lambda*rayDirection.y;

			//hOffset.y = intersectionPoint.y;

			//holdingOffset = intersectionPoint - objectPosition;
			newPosition = pointOnPlane-hOffset/*holdingOffset*/;
		}
		break;
	}


	//if(isKeyDown(key_trX))//mKeyboard->isKeyDown(OIS::KC_X))
	//{
	//	Ogre::Vector3 pointOnPlane = intersectionPoint;

	//	if(rayDirection.z != 0)
	//	{
	//		//unlikely, but possible
	//		Ogre::Real lambda = (intersectionPoint.z - rayOrigin.z)/rayDirection.z;
	//		pointOnPlane.x=rayOrigin.x+lambda*rayDirection.x;
	//		//hOffset.z = pointOnPlane.z;


	//	}
	//	else
	//	{
	//		//just take the other plane
	//		Ogre::Real lambda = (intersectionPoint.y - rayOrigin.y)/rayDirection.y;
	//		pointOnPlane.x=rayOrigin.x+lambda*rayDirection.x;
	//		//hOffset.y = pointOnPlane.y;

	//	}
	//	newPosition = pointOnPlane-hOffset/*holdingOffset*/;
	//}
	//else if(isKeyDown(key_trY))//mKeyboard->isKeyDown(OIS::KC_Y))
	//{

	//	/*
	//	IDEA:
	//	use the YX or YZ plane, and check the ray to intersect. then use only
	//	the y coord of the intersection point
	//	*/
	//	Ogre::Vector3 pointOnPlane = intersectionPoint;

	//	if(rayDirection.z != 0)
	//	{
	//		//unlikely, but possible
	//		Ogre::Real lambda = (intersectionPoint.z - rayOrigin.z)/rayDirection.z;
	//		pointOnPlane.y=rayOrigin.y+lambda*rayDirection.y;
	//		//hOffset.z = pointOnPlane.z;


	//	}
	//	else
	//	{
	//		//just take the other plane
	//		Ogre::Real lambda = (intersectionPoint.x - rayOrigin.x)/rayDirection.x;
	//		pointOnPlane.y=rayOrigin.y+lambda*rayDirection.y;
	//		//hOffset.x = pointOnPlane.x;

	//	}
	//	newPosition = pointOnPlane-hOffset/*holdingOffset*/;


	//}
	//else if(isKeyDown(key_trZ))//mKeyboard->isKeyDown(OIS::KC_Z))
	//{
	//	Ogre::Vector3 pointOnPlane = intersectionPoint;

	//	if(rayDirection.y != 0)
	//	{
	//		//unlikely, but possible
	//		Ogre::Real lambda = (intersectionPoint.y - rayOrigin.y)/rayDirection.y;
	//		pointOnPlane.z=rayOrigin.z+lambda*rayDirection.z;
	//		//hOffset.y = pointOnPlane.y;


	//	}
	//	else
	//	{
	//		//just take the other plane
	//		Ogre::Real lambda = (intersectionPoint.x - rayOrigin.x)/rayDirection.x;
	//		pointOnPlane.z=rayOrigin.z+lambda*rayDirection.z;
	//		//hOffset.x = pointOnPlane.x;

	//	}
	//	newPosition = pointOnPlane-hOffset/*holdingOffset*/;
	//}
	//else
	//{


	//	//create a plane through the intersection point. erstmal y = objectINtersection.y
	//	//calc the ray's intersection with this point
	//	//move the first ray/obj intersection to the ray/plane intersection point


	//	Ogre::Vector3 pointOnPlane = Ogre::Vector3::ZERO;
	//	//ray = origin + lambda * direction
	//	pointOnPlane.y=intersectionPoint.y;
	//	Ogre::Real lambda = (intersectionPoint.y - rayOrigin.y)/rayDirection.y;
	//	pointOnPlane.x=rayOrigin.x+lambda*rayDirection.x;
	//	pointOnPlane.z=rayOrigin.z+lambda*rayDirection.z;

	//	//hOffset.y = intersectionPoint.y;

	//	//holdingOffset = intersectionPoint - objectPosition;
	//	newPosition = pointOnPlane-hOffset/*holdingOffset*/;
	//}


	if(app->snapOnGrid)
	{
		//anders
		//mit floor und ceil die nächsten Punke finden, und an den näheren bewegen
		Ogre::Vector3 newPosCeil;// = newPosition;
		Ogre::Vector3 newPosFloor;
		Ogre::Real gridSnap = app->gridSnap;

		newPosCeil.x = ceil(newPosition.x/gridSnap)*gridSnap;
		newPosCeil.y = ceil(newPosition.y/gridSnap)*gridSnap;
		newPosCeil.z = ceil(newPosition.z/gridSnap)*gridSnap;

		newPosFloor.x = floor(newPosition.x/gridSnap)*gridSnap;
		newPosFloor.y = floor(newPosition.y/gridSnap)*gridSnap;
		newPosFloor.z = floor(newPosition.z/gridSnap)*gridSnap;

		if(abs(newPosCeil.x - newPosition.x) < abs(newPosFloor.x - newPosition.x))
		{
			//dh ceil ist näher
			newPosition.x = newPosCeil.x;
		}
		else
		{
			newPosition.x = newPosFloor.x;
		}

		if(abs(newPosCeil.y - newPosition.y) < abs(newPosFloor.y - newPosition.y))
		{
			//dh ceil ist näher
			newPosition.y = newPosCeil.y;
		}
		else
		{
			newPosition.y = newPosFloor.y;
		}
		if(abs(newPosCeil.z - newPosition.z) < abs(newPosFloor.z - newPosition.z))
		{
			//dh ceil ist näher
			newPosition.z = newPosCeil.z;
		}
		else
		{
			newPosition.z = newPosFloor.z;
		}

	}
	selectedObject->setPosition(newPosition);
}

void EditFrameListener::objectMoveDown()
{
	Ogre::Vector3 pos = selectedObject->getPosition();
//	selectedObject->getSceneNode()->_update(true,false);
//	Ogre::AxisAlignedBox box = selectedObject->getSceneNode()->_getWorldAABB();//selectedObject->getCollisionAABB();
	//GameObject *temp = selectedObject;
	//setSelected(NULL);
	hideAxes();
	Ogre::AxisAlignedBox box = getSceneNodeBoundingBox(selectedObject->getSceneNode());
	//app->currentLevel->debugShowPoint(box.getMinimum());
	Ogre::Real yOffset = Ogre::Math::Abs(box.getMinimum().y-pos.y);//box.getCorner(Ogre::AxisAlignedBox::FAR_LEFT_BOTTOM).y;//.getHalfSize().y;
	/*if(axe_Y)
	{
		Ogre::Real bRad = axe_Y->getAttachedObject(0)->getBoundingRadius();
		if(bRad > yOffset)
			yOffset = bRad;
	}
	*/
	//yOffset += 0.01f;
	Ogre::Vector3 rayStartPos = pos;
	rayStartPos.y -= yOffset;
	//pos.y -= yOffset; 
	//return;
	Ogre::Ray mRay(rayStartPos,Ogre::Vector3::NEGATIVE_UNIT_Y);
	Ogre::Vector3 poswtf = app->currentLevel->getMainCam()->getDerivedPosition();
	
	
	Ogre::RaySceneQueryResultEntry res = getClosest(app->currentLevel->getSceneManager(),mRay,0,0.1);
	//setSelected(temp);
	showAxes(selectedObject);
	pos.y -= res.distance;
	//pos.y += yOffset; 
	selectedObject->setPosition(pos);

	/*Ogre::Vector3 pos = selectedObject->getPosition();
	Ogre::AxisAlignedBox box = selectedObject->getCollisionAABB();
	box.setMaximum(box.getMaximum()+pos);
	box.getMinimum(box.getMinimum()+pos);
	box.setMinimumY(box.getMinimum().y-100);
	Ogre::DefaultAxisAlignedBoxSceneQuery *boxQry = app->currentLevel->getSceneManager()->createAABBQuery(box);*/
	//boxQry->execute(
}

void EditFrameListener::objectScale()
{
	CEGUI::Point mousepos =	CEGUI::MouseCursor::getSingleton().getPosition();

	Ogre::Real mouseX = mousepos.d_x/mWindow->getWidth();
	Ogre::Real mouseY = mousepos.d_y/mWindow->getHeight();

	CEGUI::MouseCursor::getSingleton().setImage((CEGUI::utf8*)STYLE_EDITOR,(CEGUI::utf8*)"MouseMoveCursor");


	Ogre::Camera *mCamera = app->getCurrentLevel()->getMainCam();
	Ogre::Ray mouseRay = mCamera->getCameraToViewportRay( mouseX, mouseY );
	Ogre::Vector3 rayOrigin = mouseRay.getOrigin();
	Ogre::Vector3 rayDirection = mouseRay.getDirection();
	Ogre::Vector3 newScale;
	Ogre::Vector3 pos = selectedObject->getPosition();

	

	//if(oldScale == Ogre::Vector3::ZERO)
	Ogre::Vector3	oldScale=selectedObject->getScale();

	Ogre::Quaternion oldOrientation;
	Ogre::Vector3 oldPosition;
	selectedObject->getPositionOrientation(oldPosition,oldOrientation);

	Ogre::Plane pl;	
	Ogre::Vector3 axis; //die achse, an der ich ziehe
	Ogre::Vector3 normal; //normale der ebene, IN der ich ziehe.

	//erstmal die Achsen
	switch(curTransform)
	{
	case tAxisX:
	case tAxisY:
	case tAxisZ:
		{
			std::pair<bool, Ogre::Real> res;
			int cIndex; //das ist für direct component access, also Ogre::Vector3[i]
			switch(curTransform)
			{
			case tAxisX:
				{
					cIndex = 0;
			
					axis = oldOrientation * Ogre::Vector3::UNIT_X; //die achse, an der ich ziehe
					normal = oldOrientation * Ogre::Vector3::UNIT_Y; //normale der ebene, IN der ich ziehe.
					pl.redefine(normal,pos);	
					
					//der teil prüft, ob ich die aktuelle Ebene benutzen kann
					res;// = Ogre::Math::intersects(mouseRay,pl);
					//der nachfolgende teil prüft, ob der Mouseray nicht zu parallel zur ebene verläuft, weil dann böse
					Ogre::Real testval = Ogre::Math::Abs(normal.dotProduct(mouseRay.getDirection())); 
					
					if(testval < 0.5)//!res.first)
					{				
						//es ist besser mit der anderen ebene			
						normal = oldOrientation * Ogre::Vector3::UNIT_Z; //normale der ebene, IN der ich ziehe.
						pl.redefine(normal,pos);
					}
					//das war die letzte achsenabhängige anweisung
				}
				break;
			case tAxisY:
				{
					cIndex = 1;
					axis = oldOrientation * Ogre::Vector3::UNIT_Y; //die achse, an der ich ziehe
					normal = oldOrientation * Ogre::Vector3::UNIT_X; //normale der ebene, IN der ich ziehe.
					pl.redefine(normal,pos);	

					
					res = Ogre::Math::intersects(mouseRay,pl);
					Ogre::Real testval = Ogre::Math::Abs(normal.dotProduct(mouseRay.getDirection())); 
					
					if(testval < 0.5)//!res.first)
					{				
						//es ist besser mit der anderen ebene			
						normal = oldOrientation * Ogre::Vector3::UNIT_Z; //normale der ebene, IN der ich ziehe.
						pl.redefine(normal,pos);//OMFG ich n00b!
						//hab die Zeile von da oben kopiert, wo steht `Ogre::Plane pl(normal,pos)`,
						//und wundere mich, warum es nicht funzt -.-
						

					}
				}
				break;
			case tAxisZ:
				{
					cIndex = 2;
					axis = oldOrientation * Ogre::Vector3::UNIT_Z; //die achse, an der ich ziehe
					normal = oldOrientation * Ogre::Vector3::UNIT_Y; //normale der ebene, IN der ich ziehe.
					pl.redefine(normal,pos);	

					
					res = Ogre::Math::intersects(mouseRay,pl);
					Ogre::Real testval = Ogre::Math::Abs(normal.dotProduct(mouseRay.getDirection())); 
					
					if(testval < 0.5)//!res.first)
					{				
						//es ist besser mit der anderen ebene			
						normal = oldOrientation * Ogre::Vector3::UNIT_X; //normale der ebene, IN der ich ziehe.
						pl.redefine(normal,pos);//OMFG ich n00b!
						//hab die Zeile von da oben kopiert, wo steht `Ogre::Plane pl(normal,pos)`,
						//und wundere mich, warum es nicht funzt -.-
						

					}
				}
			};


			res = Ogre::Math::intersects(mouseRay,pl);
			if(!res.first)
			{
				//o_O
				//throw Ogre::Exception(0,"irgendwie trifft der mouseray nix","EditFrameListener::objectScale");
				return; //keine emoware sein
			}
			

			Ogre::Vector3 hitPoint = mouseRay.getPoint(res.second)-pos; //das ist jetzt der Punkt der ebene, den ich mit der maus getroffen habe
			//jetzt hole ich den Abstand von hitPoint zu der ebene durch den Nullpunkt,
			//deren Normale `axis` ist
			if(scaleDoOnce)
			{
				oldHitpoint = hitPoint;
				scaleDoOnce = false;
			}
			Ogre::Plane cmp(axis,0);
			Ogre::Real dist = cmp.getDistance(hitPoint);
			Ogre::Real dist2 = cmp.getDistance(oldHitpoint);		
			


			Ogre::Real ratio = Ogre::Math::Abs(dist-dist2);
//			mDebugText(str(ratio));

			Ogre::Vector3 testSideNor = startPosition-intersectionPoint;
			Ogre::Plane testSide(testSideNor,0);
			if(testSide.getDistance(hitPoint)>testSide.getDistance(oldHitpoint))
			{
				ratio *= -1;
			}
		
			ratio /= axesNode->getScale().x;
			ratio *= oldScale[cIndex];


			oldHitpoint = hitPoint;

			//ratio /= 2;
			//unSnappedScale = oldScale;
			oldScale = unSnappedScale;

			//hier beginnt wieder achsenspezifisches zeug
			//ne eigentlich nicht

			
			unSnappedScale[cIndex] = oldScale[cIndex]+ratio;
			/*unSnappedScale.x = oldScale.x+ratio;
			unSnappedScale.y = oldScale.y;
			unSnappedScale.z = oldScale.z;*/
			
			newScale = unSnappedScale;
			if(app->snapOnScale)
			{
				Ogre::Real scaleSnap = app->scaleSnap;
				//mDebugText(str(newScale.x)+"\n");
				Ogre::Real fVal = Ogre::Math::Floor(newScale[cIndex] / scaleSnap)*scaleSnap;
				Ogre::Real cVal = Ogre::Math::Ceil(newScale[cIndex] / scaleSnap)*scaleSnap;

				if(abs(cVal - newScale[cIndex]) < abs(fVal- newScale[cIndex]))
				{
					//dh ceil ist näher
					newScale[cIndex] = cVal;
				}
				else
				{
					newScale[cIndex] = fVal;
				}
				
				//newScale.x = Ogre::Math::Floor(newScale.x / scaleSnap)*scaleSnap;
				//mDebugText(str(newScale.x),true);
			}
		}
		break;
	
	//jetzt die Ebenen:
	case tPlaneXY:
	case tPlaneXZ:
	case tPlaneYZ:
		{
			std::pair<bool, Ogre::Real> res;
			int index1;
			int index2;
			Ogre::Vector3 normal;
			switch(curTransform)
			{
			case tPlaneXY:
				index1 = 0;
				index2 = 1;
				normal = oldOrientation * Ogre::Vector3::UNIT_Z;
				break;
			case tPlaneXZ:
				index1 = 0;
				index2 = 2;
				normal = oldOrientation * Ogre::Vector3::UNIT_Y;
				break;
			case tPlaneYZ:
				index1 = 1;
				index2 = 2;
				normal = oldOrientation * Ogre::Vector3::UNIT_X;
				break;
			}
			//Ogre::Vector3 axis = oldOrientation * Ogre::Vector3::UNIT_Z; //die achse, an der ich ziehe
			//Ogre::Vector3 normal = oldOrientation * Ogre::Vector3::UNIT_Z; //normale der ebene, IN der ich ziehe.
			Ogre::Plane pl(normal,pos);	

			
			res = Ogre::Math::intersects(mouseRay,pl);
		
			if(!res.first)
			{
				//o_O
				//throw Ogre::Exception(0,"irgendwie trifft der mouseray nix","EditFrameListener::objectScale");
				return;
			}

			Ogre::Vector3 hitPoint = mouseRay.getPoint(res.second)-pos;
			if(scaleDoOnce)
			{
				oldHitpoint = hitPoint;
				scaleDoOnce = false;
			}
			//Ogre::Plane cmp(axis,0);
			Ogre::Real dist = hitPoint.length();//cmp.getDistance(hitPoint);
			Ogre::Real dist2 = oldHitpoint.length();//cmp.getDistance(oldHitpoint);

			Ogre::Real ratio = Ogre::Math::Abs(dist-dist2);
//			mDebugText(str(ratio));
			Ogre::Vector3 testSideNor = startPosition-intersectionPoint;
			Ogre::Plane testSide(testSideNor,0);
			if(testSide.getDistance(hitPoint)>testSide.getDistance(oldHitpoint))
			{
				ratio *= -1;
			}

			ratio /= axesNode->getScale().x;
			ratio *= oldScale[index1];

			oldHitpoint = hitPoint;
			oldScale = unSnappedScale;
			

			
			unSnappedScale[index1] = oldScale[index1]+ratio;
			unSnappedScale[index2] = oldScale[index2]+ratio;
			/*unSnappedScale.x = oldScale.x+ratio;
			unSnappedScale.y = oldScale.y;
			unSnappedScale.z = oldScale.z;*/
			
			newScale = unSnappedScale;
			if(app->snapOnScale)
			{
				Ogre::Real scaleSnap = app->scaleSnap;
				//mDebugText(str(newScale.x)+"\n");
				Ogre::Real fVal = Ogre::Math::Floor(newScale[index1] / scaleSnap)*scaleSnap;
				Ogre::Real cVal = Ogre::Math::Ceil(newScale[index1] / scaleSnap)*scaleSnap;

				if(abs(cVal - newScale[index1]) < abs(fVal- newScale[index1]))
				{
					//dh ceil ist näher
					newScale[index1] = cVal;
				}
				else
				{
					newScale[index1] = fVal;
				}
				fVal = Ogre::Math::Floor(newScale[index2] / scaleSnap)*scaleSnap;
				cVal = Ogre::Math::Ceil(newScale[index2] / scaleSnap)*scaleSnap;

				if(abs(cVal - newScale[index2]) < abs(fVal- newScale[index2]))
				{
					//dh ceil ist näher
					newScale[index2] = cVal;
				}
				else
				{
					newScale[index2] = fVal;
				}
				
				//newScale.x = Ogre::Math::Floor(newScale.x / scaleSnap)*scaleSnap;
				//mDebugText(str(newScale.x),true);
			}
		}
		break;
	case tAll:
		{
            Ogre::Quaternion camOrnt = mCamera->getDerivedOrientation();
			Ogre::Vector3 normal = camOrnt * Ogre::Vector3::NEGATIVE_UNIT_Z; //normale der ebene, IN der ich ziehe.
			Ogre::Plane pl(normal,pos);	

			
			std::pair<bool, Ogre::Real> res = Ogre::Math::intersects(mouseRay,pl);
			Ogre::Real testval = Ogre::Math::Abs(normal.dotProduct(mouseRay.getDirection())); 
			//hier kann ich alle 3 ebenen durchprobieren
//			mDebugText(str(testval));
			if(testval < 0.2)//!res.first)
			{				
				//es ist besser mit der anderen ebene			
				normal = oldOrientation * Ogre::Vector3::UNIT_Z; //normale der ebene, IN der ich ziehe.
				pl.redefine(normal,pos);//OMFG ich n00b!
				//hab die Zeile von da oben kopiert, wo steht `Ogre::Plane pl(normal,pos)`,
				//und wundere mich, warum es nicht funzt -.-
				testval = Ogre::Math::Abs(normal.dotProduct(mouseRay.getDirection())); 
			
				if(testval < 0.2)//!res.first)
				{				
					//es ist besser mit der anderen ebene			
					normal = oldOrientation * Ogre::Vector3::UNIT_X; //normale der ebene, IN der ich ziehe.
					pl.redefine(normal,pos);

				}
				

			}

			res = Ogre::Math::intersects(mouseRay,pl);
			if(!res.first)
			{
				//o_O
				//throw Ogre::Exception(0,"irgendwie trifft der mouseray nix","EditFrameListener::objectScale");
				return;
			}
			
			



			Ogre::Vector3 hitPoint = mouseRay.getPoint(res.second)-pos; //das ist jetzt der Punkt der ebene, den ich mit der maus getroffen habe
			if(scaleDoOnce)
			{
				oldHitpoint = hitPoint;
				scaleDoOnce = false;
			}
			//jetzt hole ich den Abstand von hitPoint zu der ebene durch den Nullpunkt,
			//deren Normale `axis` ist
//			Ogre::Plane cmp(axis,0);
			Ogre::Real dist = hitPoint.length();
			Ogre::Real dist2 = oldHitpoint.length();

	
			Ogre::Real ratio = Ogre::Math::Abs(dist-dist2);
			//Ogre::Real ratio = Ogre::Math::Abs(dist/dist2);

			Ogre::Vector3 testSideNor = startPosition-intersectionPoint;
			Ogre::Plane testSide(testSideNor,0);
			if(testSide.getDistance(hitPoint)>testSide.getDistance(oldHitpoint))
			{
				ratio *= -1;
				
//				ratio = 1/ratio;
			}
			
			//test
			ratio /= axesNode->getScale().x;
			ratio *= oldScale.x;
		
			oldHitpoint = hitPoint;
			oldScale = unSnappedScale;

			unSnappedScale = oldScale+Ogre::Vector3(ratio);
			//unSnappedScale = oldScale*Ogre::Vector3(ratio);
			/*unSnappedScale.x = oldScale.x+ratio;
			unSnappedScale.y = oldScale.y;
			unSnappedScale.z = oldScale.z;*/
			
			newScale = unSnappedScale;
			if(app->snapOnScale)
			{
				Ogre::Real scaleSnap = app->scaleSnap;
				//mDebugText(str(newScale.x)+"\n");
				Ogre::Real fValX = Ogre::Math::Floor(newScale.x / scaleSnap)*scaleSnap;
				Ogre::Real cValX = Ogre::Math::Ceil(newScale.x / scaleSnap)*scaleSnap;

				Ogre::Real fValY = Ogre::Math::Floor(newScale.y / scaleSnap)*scaleSnap;
				Ogre::Real cValY = Ogre::Math::Ceil(newScale.y / scaleSnap)*scaleSnap;

				Ogre::Real fValZ = Ogre::Math::Floor(newScale.z / scaleSnap)*scaleSnap;
				Ogre::Real cValZ = Ogre::Math::Ceil(newScale.z / scaleSnap)*scaleSnap;

				if(abs(cValX - newScale.x) < abs(fValX- newScale.x))
				{
					//dh ceil ist näher
					newScale.x = cValX;
				}
				else
				{
					newScale.x = fValX;
				}
				if(abs(cValY - newScale.y) < abs(fValY- newScale.y))
				{
					//dh ceil ist näher
					newScale.y = cValY;
				}
				else
				{
					newScale.y = fValY;
				}
				if(abs(cValZ - newScale.z) < abs(fValZ- newScale.z))
				{
					//dh ceil ist näher
					newScale.z = cValZ;
				}
				else
				{
					newScale.z = fValZ;
				}
				
				
				//newScale.x = Ogre::Math::Floor(newScale.x / scaleSnap)*scaleSnap;
				//mDebugText(str(newScale.x),true);
			}
			//ende planecode

			
		}
		break;
	}



	
	//mDebugText(str(newScale));
	selectedObject->setScale(newScale);
}

/*
backup der objectScale.
an der X-achse ist schon modifiziert
void EditFrameListener::objectScale()
{
	CEGUI::Point mousepos =	CEGUI::MouseCursor::getSingleton().getPosition();

	Ogre::Real mouseX = mousepos.d_x/mWindow->getWidth();
	Ogre::Real mouseY = mousepos.d_y/mWindow->getHeight();

	CEGUI::MouseCursor::getSingleton().setImage((CEGUI::utf8*)STYLE_EDITOR,(CEGUI::utf8*)"MouseMoveCursor");


	Ogre::Camera *mCamera = app->getCurrentLevel()->getMainCam();
	Ogre::Ray mouseRay = mCamera->getCameraToViewportRay( mouseX, mouseY );
	Ogre::Vector3 rayOrigin = mouseRay.getOrigin();
	Ogre::Vector3 rayDirection = mouseRay.getDirection();
	Ogre::Vector3 newScale;
	Ogre::Vector3 pos = selectedObject->getPosition();

	

	//if(oldScale == Ogre::Vector3::ZERO)
	Ogre::Vector3	oldScale=selectedObject->getScale();

	Ogre::Quaternion oldOrientation;
	Ogre::Vector3 oldPosition;
	selectedObject->getPositionOrientation(oldPosition,oldOrientation);

	

	switch(curTransform)
	{
	case tAxisX:
		{
			
			Ogre::Vector3 axis = oldOrientation * Ogre::Vector3::UNIT_X; //die achse, an der ich ziehe
			Ogre::Vector3 normal = oldOrientation * Ogre::Vector3::UNIT_Y; //normale der ebene, IN der ich ziehe.
			Ogre::Plane pl(normal,pos);	

			
			std::pair<bool, Ogre::Real> res;// = Ogre::Math::intersects(mouseRay,pl);
			//der nachfolgende teil prüft, ob der Mouseray nicht zu parallel zur ebene verläuft, weil dann böse
			Ogre::Real testval = Ogre::Math::Abs(normal.dotProduct(mouseRay.getDirection())); 
			
			if(testval < 0.5)//!res.first)
			{				
				//es ist besser mit der anderen ebene			
				normal = oldOrientation * Ogre::Vector3::UNIT_Z; //normale der ebene, IN der ich ziehe.
				pl.redefine(normal,pos);//OMFG ich n00b!
				//hab die Zeile von da oben kopiert, wo steht `Ogre::Plane pl(normal,pos)`,
				//und wundere mich, warum es nicht funzt -.-
				

			}

			res = Ogre::Math::intersects(mouseRay,pl);
			if(!res.first)
			{
				//o_O
				//throw Ogre::Exception(0,"irgendwie trifft der mouseray nix","EditFrameListener::objectScale");
				return; //keine emoware sein
			}
			

			Ogre::Vector3 hitPoint = mouseRay.getPoint(res.second)-pos; //das ist jetzt der Punkt der ebene, den ich mit der maus getroffen habe
			//jetzt hole ich den Abstand von hitPoint zu der ebene durch den Nullpunkt,
			//deren Normale `axis` ist
			if(scaleDoOnce)
			{
				oldHitpoint = hitPoint;
				scaleDoOnce = false;
			}
			Ogre::Plane cmp(axis,0);
			Ogre::Real dist = cmp.getDistance(hitPoint);
			Ogre::Real dist2 = cmp.getDistance(oldHitpoint);

			
			


			Ogre::Real ratio = Ogre::Math::Abs(dist-dist2);

			Ogre::Vector3 testSideNor = startPosition-intersectionPoint;
			Ogre::Plane testSide(testSideNor,0);
			if(testSide.getDistance(hitPoint)>testSide.getDistance(oldHitpoint))
			{
				ratio *= -1;
			}
		


			oldHitpoint = hitPoint;

			//ratio /= 2;
			//unSnappedScale = oldScale;
			oldScale = unSnappedScale;

			
			unSnappedScale.x = oldScale.x+ratio;
			unSnappedScale.y = oldScale.y;
			unSnappedScale.z = oldScale.z;
			
			newScale = unSnappedScale;
			if(app->snapOnScale)
			{
				Ogre::Real scaleSnap = app->scaleSnap;
				mDebugText(str(newScale.x)+"\n");
				Ogre::Real fVal = Ogre::Math::Floor(newScale.x / scaleSnap)*scaleSnap;
				Ogre::Real cVal = Ogre::Math::Ceil(newScale.x / scaleSnap)*scaleSnap;

				//if(abs(cVal - newScale.x) < abs(fVal- newScale.x))
				//{
				//	//dh ceil ist näher
				//	newScale.x = cVal;
				//}
				//else
				//{
				//	newScale.x = fVal;
				//}
				//
				newScale.x = Ogre::Math::Floor(newScale.x / scaleSnap)*scaleSnap;
				mDebugText(str(newScale.x),true);
			}
		}
		break;
	case tAxisY:
		{

			Ogre::Vector3 axis = oldOrientation * Ogre::Vector3::UNIT_Y; //die achse, an der ich ziehe
			Ogre::Vector3 normal = oldOrientation * Ogre::Vector3::UNIT_X; //normale der ebene, IN der ich ziehe.
			Ogre::Plane pl(normal,pos);	

			
			std::pair<bool, Ogre::Real> res = Ogre::Math::intersects(mouseRay,pl);
			Ogre::Real testval = Ogre::Math::Abs(normal.dotProduct(mouseRay.getDirection())); 
			
			if(testval < 0.5)//!res.first)
			{				
				//es ist besser mit der anderen ebene			
				normal = oldOrientation * Ogre::Vector3::UNIT_Z; //normale der ebene, IN der ich ziehe.
				pl.redefine(normal,pos);//OMFG ich n00b!
				//hab die Zeile von da oben kopiert, wo steht `Ogre::Plane pl(normal,pos)`,
				//und wundere mich, warum es nicht funzt -.-
				

			}

			res = Ogre::Math::intersects(mouseRay,pl);
			if(!res.first)
			{
				//o_O
				//throw Ogre::Exception(0,"irgendwie trifft der mouseray nix","EditFrameListener::objectScale");
				return;
			}
			

			Ogre::Vector3 hitPoint = mouseRay.getPoint(res.second)-pos; //das ist jetzt der Punkt der ebene, den ich mit der maus getroffen habe
			//jetzt hole ich den Abstand von hitPoint zu der ebene durch den Nullpunkt,
			//deren Normale `axis` ist
			if(scaleDoOnce)
			{
				oldHitpoint = hitPoint;
				scaleDoOnce = false;
			}
			Ogre::Plane cmp(axis,0);
			Ogre::Real dist = cmp.getDistance(hitPoint);
			Ogre::Real dist2 = cmp.getDistance(oldHitpoint);

			oldHitpoint = hitPoint;

		


			Ogre::Real ratio = Ogre::Math::Abs(dist/dist2);
			//app->setDebugText(str(ratio));

			newScale.x = oldScale.x;
			newScale.y = oldScale.y*ratio;
			newScale.z = oldScale.z;
			if(app->snapOnScale)
			{
				Ogre::Real scaleSnap = app->scaleSnap;
				newScale.y = Ogre::Math::Floor(newScale.y / scaleSnap)*scaleSnap;
			}

		}
		break;
	case tAxisZ:
		{
			Ogre::Vector3 axis = oldOrientation * Ogre::Vector3::UNIT_Z; //die achse, an der ich ziehe
			Ogre::Vector3 normal = oldOrientation * Ogre::Vector3::UNIT_Y; //normale der ebene, IN der ich ziehe.
			Ogre::Plane pl(normal,pos);	

			
			std::pair<bool, Ogre::Real> res = Ogre::Math::intersects(mouseRay,pl);
			Ogre::Real testval = Ogre::Math::Abs(normal.dotProduct(mouseRay.getDirection())); 
			
			if(testval < 0.5)//!res.first)
			{				
				//es ist besser mit der anderen ebene			
				normal = oldOrientation * Ogre::Vector3::UNIT_X; //normale der ebene, IN der ich ziehe.
				pl.redefine(normal,pos);//OMFG ich n00b!
				//hab die Zeile von da oben kopiert, wo steht `Ogre::Plane pl(normal,pos)`,
				//und wundere mich, warum es nicht funzt -.-
				

			}

			res = Ogre::Math::intersects(mouseRay,pl);
			if(!res.first)
			{
				//o_O
				//throw Ogre::Exception(0,"irgendwie trifft der mouseray nix","EditFrameListener::objectScale");
				return;
			}
			
			Ogre::Vector3 hitPoint = mouseRay.getPoint(res.second)-pos; //das ist jetzt der Punkt der ebene, den ich mit der maus getroffen habe
			//jetzt hole ich den Abstand von hitPoint zu der ebene durch den Nullpunkt,
			//deren Normale `axis` ist
			if(scaleDoOnce)
			{
				oldHitpoint = hitPoint;
				scaleDoOnce = false;
			}
			Ogre::Plane cmp(axis,0);
			Ogre::Real dist = cmp.getDistance(hitPoint);
			Ogre::Real dist2 = cmp.getDistance(oldHitpoint);

			oldHitpoint = hitPoint;

		


			Ogre::Real ratio = Ogre::Math::Abs(dist/dist2);
			//app->setDebugText(str(ratio));

			newScale.x = oldScale.x;
			newScale.y = oldScale.y;
			newScale.z = oldScale.z*ratio;
			if(app->snapOnScale)
			{
				Ogre::Real scaleSnap = app->scaleSnap;
				newScale.z = Ogre::Math::Floor(newScale.z / scaleSnap)*scaleSnap;
			}
			
		}
		break;
	case tPlaneXY:
		{
			//Ogre::Vector3 axis = oldOrientation * Ogre::Vector3::UNIT_Z; //die achse, an der ich ziehe
			Ogre::Vector3 normal = oldOrientation * Ogre::Vector3::UNIT_Z; //normale der ebene, IN der ich ziehe.
			Ogre::Plane pl(normal,pos);	

			
			std::pair<bool, Ogre::Real> res = Ogre::Math::intersects(mouseRay,pl);
			//Ogre::Real testval = Ogre::Math::Abs(normal.dotProduct(mouseRay.getDirection())); 
			
			//if(testval < 0.5)//!res.first)
			//{				
			//	//es ist besser mit der anderen ebene			
			//	normal = oldOrientation * Ogre::Vector3::UNIT_X; //normale der ebene, IN der ich ziehe.
			//	pl.redefine(normal,pos);//OMFG ich n00b!
			//	//hab die Zeile von da oben kopiert, wo steht `Ogre::Plane pl(normal,pos)`,
			//	//und wundere mich, warum es nicht funzt -.-
			//	

			//}

			//res = Ogre::Math::intersects(mouseRay,pl);
			if(!res.first)
			{
				//o_O
				//throw Ogre::Exception(0,"irgendwie trifft der mouseray nix","EditFrameListener::objectScale");
				return;
			}
			



			Ogre::Vector3 hitPoint = mouseRay.getPoint(res.second)-pos; //das ist jetzt der Punkt der ebene, den ich mit der maus getroffen habe
			if(scaleDoOnce)
			{
				oldHitpoint = hitPoint;
				scaleDoOnce = false;
			}
			//jetzt hole ich den Abstand von hitPoint zu der ebene durch den Nullpunkt,
			//deren Normale `axis` ist
//			Ogre::Plane cmp(axis,0);
			Ogre::Real dist = hitPoint.length();
			Ogre::Real dist2 = oldHitpoint.length();

			oldHitpoint = hitPoint;

		


			Ogre::Real ratio = Ogre::Math::Abs(dist/dist2);

			newScale.x = oldScale.x*ratio;
			newScale.y = oldScale.y*ratio;
			newScale.z = oldScale.z;
			if(app->snapOnScale)
			{
				Ogre::Real scaleSnap = app->scaleSnap;
				newScale.x = Ogre::Math::Floor(newScale.x / scaleSnap)*scaleSnap;
				newScale.y = Ogre::Math::Floor(newScale.y / scaleSnap)*scaleSnap;
			}

		


		}
		break;
	case tPlaneXZ:
		{
			Ogre::Vector3 normal = oldOrientation * Ogre::Vector3::UNIT_Y; //normale der ebene, IN der ich ziehe.
			Ogre::Plane pl(normal,pos);	

			
			std::pair<bool, Ogre::Real> res = Ogre::Math::intersects(mouseRay,pl);
			//Ogre::Real testval = Ogre::Math::Abs(normal.dotProduct(mouseRay.getDirection())); 
			//
			//if(testval < 0.5)//!res.first)
			//{				
			//	//es ist besser mit der anderen ebene			
			//	normal = oldOrientation * Ogre::Vector3::UNIT_X; //normale der ebene, IN der ich ziehe.
			//	pl.redefine(normal,pos);//OMFG ich n00b!
			//	//hab die Zeile von da oben kopiert, wo steht `Ogre::Plane pl(normal,pos)`,
			//	//und wundere mich, warum es nicht funzt -.-
			//	

			//}

			//res = Ogre::Math::intersects(mouseRay,pl);
			if(!res.first)
			{
				//o_O
				//throw Ogre::Exception(0,"irgendwie trifft der mouseray nix","EditFrameListener::objectScale");
				return;
			}
			



			Ogre::Vector3 hitPoint = mouseRay.getPoint(res.second)-pos; //das ist jetzt der Punkt der ebene, den ich mit der maus getroffen habe
			if(scaleDoOnce)
			{
				oldHitpoint = hitPoint;
				scaleDoOnce = false;
			}
			//jetzt hole ich den Abstand von hitPoint zu der ebene durch den Nullpunkt,
			//deren Normale `axis` ist
//			Ogre::Plane cmp(axis,0);
			Ogre::Real dist = hitPoint.length();
			Ogre::Real dist2 = oldHitpoint.length();

			oldHitpoint = hitPoint;

		


			Ogre::Real ratio = Ogre::Math::Abs(dist/dist2);

			newScale.x = oldScale.x*ratio;
			newScale.y = oldScale.y;
			newScale.z = oldScale.z*ratio;
			if(app->snapOnScale)
			{
				Ogre::Real scaleSnap = app->scaleSnap;
				newScale.x = Ogre::Math::Floor(newScale.x / scaleSnap)*scaleSnap;
				newScale.z = Ogre::Math::Floor(newScale.z / scaleSnap)*scaleSnap;
			}

			


		}
		break;
	case tPlaneYZ:
		{
			Ogre::Vector3 normal = oldOrientation * Ogre::Vector3::UNIT_X; //normale der ebene, IN der ich ziehe.
			Ogre::Plane pl(normal,pos);	

			
			std::pair<bool, Ogre::Real> res = Ogre::Math::intersects(mouseRay,pl);
			//Ogre::Real testval = Ogre::Math::Abs(normal.dotProduct(mouseRay.getDirection())); 
			//
			//if(testval < 0.5)//!res.first)
			//{				
			//	//es ist besser mit der anderen ebene			
			//	normal = oldOrientation * Ogre::Vector3::UNIT_Z; //normale der ebene, IN der ich ziehe.
			//	pl.redefine(normal,pos);//OMFG ich n00b!
			//	//hab die Zeile von da oben kopiert, wo steht `Ogre::Plane pl(normal,pos)`,
			//	//und wundere mich, warum es nicht funzt -.-
			//	

			//}

			//res = Ogre::Math::intersects(mouseRay,pl);
			if(!res.first)
			{
				//o_O
				//throw Ogre::Exception(0,"irgendwie trifft der mouseray nix","EditFrameListener::objectScale");
				return;
			}
			



			Ogre::Vector3 hitPoint = mouseRay.getPoint(res.second)-pos; //das ist jetzt der Punkt der ebene, den ich mit der maus getroffen habe
			if(scaleDoOnce)
			{
				oldHitpoint = hitPoint;
				scaleDoOnce = false;
			}
			//jetzt hole ich den Abstand von hitPoint zu der ebene durch den Nullpunkt,
			//deren Normale `axis` ist
//			Ogre::Plane cmp(axis,0);
			Ogre::Real dist = hitPoint.length();
			Ogre::Real dist2 = oldHitpoint.length();

			oldHitpoint = hitPoint;

		


			Ogre::Real ratio = Ogre::Math::Abs(dist/dist2);

			newScale.x = oldScale.x;
			newScale.z = oldScale.z*ratio;
			newScale.y = oldScale.y*ratio;
			if(app->snapOnScale)
			{
				Ogre::Real scaleSnap = app->scaleSnap;
				newScale.y = Ogre::Math::Floor(newScale.y / scaleSnap)*scaleSnap;
				newScale.z = Ogre::Math::Floor(newScale.z / scaleSnap)*scaleSnap;
			}

		


		}
		break;
	case tAll:
		{
			Ogre::Quaternion camOrnt = mCamera->_getDerivedOrientation();
			Ogre::Vector3 normal = camOrnt * Ogre::Vector3::NEGATIVE_UNIT_Z; //normale der ebene, IN der ich ziehe.
			Ogre::Plane pl(normal,pos);	

			
			std::pair<bool, Ogre::Real> res = Ogre::Math::intersects(mouseRay,pl);
			Ogre::Real testval = Ogre::Math::Abs(normal.dotProduct(mouseRay.getDirection())); 
			//hier kann ich alle 3 ebenen durchprobieren
//			mDebugText(str(testval));
			if(testval < 0.2)//!res.first)
			{				
				//es ist besser mit der anderen ebene			
				normal = oldOrientation * Ogre::Vector3::UNIT_Z; //normale der ebene, IN der ich ziehe.
				pl.redefine(normal,pos);//OMFG ich n00b!
				//hab die Zeile von da oben kopiert, wo steht `Ogre::Plane pl(normal,pos)`,
				//und wundere mich, warum es nicht funzt -.-
				testval = Ogre::Math::Abs(normal.dotProduct(mouseRay.getDirection())); 
			
				if(testval < 0.2)//!res.first)
				{				
					//es ist besser mit der anderen ebene			
					normal = oldOrientation * Ogre::Vector3::UNIT_X; //normale der ebene, IN der ich ziehe.
					pl.redefine(normal,pos);

				}
				

			}

			res = Ogre::Math::intersects(mouseRay,pl);
			if(!res.first)
			{
				//o_O
				//throw Ogre::Exception(0,"irgendwie trifft der mouseray nix","EditFrameListener::objectScale");
				return;
			}
			
			



			Ogre::Vector3 hitPoint = mouseRay.getPoint(res.second)-pos; //das ist jetzt der Punkt der ebene, den ich mit der maus getroffen habe
			if(scaleDoOnce)
			{
				oldHitpoint = hitPoint;
				scaleDoOnce = false;
			}
			//jetzt hole ich den Abstand von hitPoint zu der ebene durch den Nullpunkt,
			//deren Normale `axis` ist
//			Ogre::Plane cmp(axis,0);
			Ogre::Real dist = hitPoint.length();
			Ogre::Real dist2 = oldHitpoint.length();

			oldHitpoint = hitPoint;

		


			Ogre::Real ratio = Ogre::Math::Abs(dist/dist2);

			newScale.x = oldScale.x*ratio;
			newScale.z = oldScale.z*ratio;
			newScale.y = oldScale.y*ratio;
			if(app->snapOnScale)
			{
				Ogre::Real scaleSnap = app->scaleSnap;
				newScale.x = Ogre::Math::Floor(newScale.x / scaleSnap)*scaleSnap;
				newScale.y = Ogre::Math::Floor(newScale.y / scaleSnap)*scaleSnap;
				newScale.z = Ogre::Math::Floor(newScale.z / scaleSnap)*scaleSnap;
			}
			
		}
		break;
	}



	
	selectedObject->setScale(newScale);
}

*/

//
//void EditFrameListener::objectRotate()
//{
//
//	short axis = AXIS_Y;
//	if(isKeyDown(key_trX))
//		axis = AXIS_X;
//	if(isKeyDown(key_trY))
//		axis = AXIS_Y;
//	if(isKeyDown(key_trZ))
//		axis = AXIS_Z;
//
//	Ogre::Vector3 oldPosition;// = selectedObject->getPosition();
//	Ogre::Quaternion oldOrientation;// = selectedObject->getOrientation();
//	selectedObject->getPositionOrientation(oldPosition,oldOrientation);
//
////das ist der code für X
//	CEGUI::Point mousepos =	CEGUI::MouseCursor::getSingleton().getPosition();
//
//	Ogre::Real mouseX = mousepos.d_x/mWindow->getWidth();
//	Ogre::Real mouseY = mousepos.d_y/mWindow->getHeight();
//
//	CEGUI::MouseCursor::getSingleton().setImage((CEGUI::utf8*)STYLE_EDITOR,(CEGUI::utf8*)"MouseMoveCursor");
//
//
//	Ogre::Camera *mCamera = app->getCurrentLevel()->getMainCam();//mSceneMgr->getCamera("PlayerCam");
//	Ogre::Ray mouseRay = mCamera->getCameraToViewportRay( mouseX, mouseY );
//	Ogre::Vector3 rayOrigin = mouseRay.getOrigin();
//	Ogre::Vector3 rayDirection = mouseRay.getDirection();
//
//
//
//	//intersectionPoint: der Punkt, wo der Ogre::Ray das Objekt getroffen hat
//	//holdingOffset = intersectionPoint - objectPosition;
//	Ogre::Vector3 planeHoldingPoint = intersectionPoint;
//	Ogre::Vector3 intersectWithPlane;
//
//	//erstmal in der XZ-Ebene, und ohne snap
//	//die Ebene wird jetzt am Ursprung des Objekts angesetzt
//	//punkt finden, an dem der mouseRay sich mit der Ebene schneidet
//	//formel für einfache Ebenen, die an den Koord.achsen verschoben sind, also x = 5, y=8 oder z=1337: 
//	//lambda = (koord_der_ebene - gerade_startpunkt)/gerade_richtung
//	Ogre::Real lambda = 0;
//	switch(axis)
//	{
//	case AXIS_X:
//		lambda = (oldPosition.x - rayOrigin.x)/rayDirection.x;
//		//den haltepunkt und den Mauspunkt in die Ebene verschieben:	
//		planeHoldingPoint -= oldPosition;
//		
//		planeHoldingPoint.x = 0;
//		planeHoldingPoint = oldOrientation*planeHoldingPoint;
//
//		intersectWithPlane = rayOrigin + rayDirection*lambda;
//		intersectWithPlane -= oldPosition;
//		//intersectWithPlane = oldOrientation * intersectWithPlane;
//		intersectWithPlane.x = 0;
//		break;
//	case AXIS_Y:
//		lambda = (oldPosition.y - rayOrigin.y)/rayDirection.y;
//		//den haltepunkt und den Mauspunkt in die Ebene verschieben:	
//		planeHoldingPoint -= oldPosition;
//		
//		planeHoldingPoint.y = 0;
//		planeHoldingPoint = oldOrientation*planeHoldingPoint;
//
//		intersectWithPlane = rayOrigin + rayDirection*lambda;
//		intersectWithPlane -= oldPosition;
//		//intersectWithPlane = oldOrientation * intersectWithPlane;
//		intersectWithPlane.y = 0;
//
//		break;
//	case AXIS_Z:
//		lambda = (oldPosition.z - rayOrigin.z)/rayDirection.z;
//		//den haltepunkt und den Mauspunkt in die Ebene verschieben:	
//		planeHoldingPoint -= oldPosition;
//		
//		planeHoldingPoint.z = 0;
//		planeHoldingPoint = oldOrientation*planeHoldingPoint;
//
//		intersectWithPlane = rayOrigin + rayDirection*lambda;
//		intersectWithPlane -= oldPosition;
//		//intersectWithPlane = oldOrientation * intersectWithPlane;
//		intersectWithPlane.z = 0;
//	}
//	//jetzt habe ich den alten Haltepunkt mit planeHoldingPoint
//	//und den neuen mit intersectWithPlane. jetzt vom einen zum anderen rotieren
//
//	//object-ursprung -> haltepunkt
//	//Ogre::Vector3 holdingPointRel = holdingOffset - oldPosition;
//	//holdingPointRel = oldOrientation*holdingPointRel; //vektor an akt. position drehen
//	//Ogre::Vector3 interSectPointRel = intersectWithPlane - oldPosition; //ist eh schon gedreht
//
//	
//	/*planeHoldingPoint.normalise();
//	intersectWithPlane.normalise();*/
//	//planeHoldingPoint = oldOrientation*Ogre::Vector3::UNIT_Z;
//
//	//"planeHoldingPoint" bzw der erste Vektor in der Zeile hier muss der vektor sein,
//	//der in RIchtung intersectWithPlane gedreht wird. also eigentlich der Haltepunkt,
//	//relativ zum Objekt. er muss mit oldOrientation multipliziert werden
//	Ogre::Quaternion rotateBy = planeHoldingPoint.getRotationTo(intersectWithPlane);
//	
//
//	mDebugText(str(planeHoldingPoint)+"\n"
//		+str(intersectWithPlane));
//	selectedObject->rotate(rotateBy);//rotate(rotateBy);
//
//
//}


void EditFrameListener::objectRotate()
{

	Ogre::Quaternion newOrientation;


	Ogre::Vector3 oldPosition;// = selectedObject->getPosition();
	Ogre::Quaternion oldOrientation;// = selectedObject->getOrientation();
	selectedObject->getPositionOrientation(oldPosition,oldOrientation);
	//mDebugText(str(oldOrientation));
	

	CEGUI::Point mousepos =	CEGUI::MouseCursor::getSingleton().getPosition();

	Ogre::Real mouseX = mousepos.d_x/mWindow->getWidth();
	Ogre::Real mouseY = mousepos.d_y/mWindow->getHeight();

	CEGUI::MouseCursor::getSingleton().setImage((CEGUI::utf8*)STYLE_EDITOR,(CEGUI::utf8*)"MouseMoveCursor");

/*	app->setDebugText("cegui: x="+Ogre::StringConverter::toString()+
						" y="+Ogre::StringConverter::toString(mousepos.d_y/mRenderWindow->getHeight())+"\n");
	app->setDebugText("ogre: x="+Ogre::StringConverter::toString(e->getX())+
						" y="+Ogre::StringConverter::toString(e->getY()),true);*/

	Ogre::Camera *mCamera = app->getCurrentLevel()->getMainCam();//mSceneMgr->getCamera("PlayerCam");
	Ogre::Ray mouseRay = mCamera->getCameraToViewportRay( mouseX, mouseY );
	Ogre::Vector3 rayOrigin = mouseRay.getOrigin();
	Ogre::Vector3 rayDirection = mouseRay.getDirection();

	Ogre::Real lambda = 0;
	Ogre::Vector3 intersectWithPlane; //der Punkt zu dem hingedreht werden soll

	switch(curTransform)
	{
	case tAxisX:
		lambda = (intersectionPoint.x - rayOrigin.x)/rayDirection.x;
		intersectWithPlane = rayOrigin+rayDirection*lambda;
		break;
	case tAxisY:
		lambda = (intersectionPoint.y - rayOrigin.y)/rayDirection.y;
		intersectWithPlane = rayOrigin+rayDirection*lambda;
		break;
	case tAxisZ:
		lambda = (intersectionPoint.z - rayOrigin.z)/rayDirection.z;
		intersectWithPlane = rayOrigin+rayDirection*lambda;
		break;
	}

	Ogre::Vector3 interSectPointRel = intersectWithPlane - oldPosition; //ist eh schon gedreht

	


		//intersectWithPlane = intersectionPoint;

		//ZY-EBENE
		//Ogre::Vector3 pointOnPlane = Ogre::Vector3::ZERO;
		////ray = origin + lambda * direction
		//pointOnPlane.x=intersectionPoint.x;
		
		/*switch(axis)
		{
		case AXIS_X:
			lambda = (intersectionPoint.x - rayOrigin.x)/rayDirection.x;
			break;
		case AXIS_Y:
			lambda = (intersectionPoint.z - rayOrigin.z)/rayDirection.z;
			break;
		case AXIS_Z:
			lambda = (intersectionPoint.y - rayOrigin.y)/rayDirection.y;
		}*/

		/*Ogre::Vector3 intersectWithPlane;
		intersectWithPlane.x = rayOrigin.x+rayDirection.x*lambda;
		intersectWithPlane.y = rayOrigin.y+rayDirection.y*lambda;
		intersectWithPlane.z = rayOrigin.z+rayDirection.z*lambda;*/

		//object-ursprung -> haltepunkt
		//Ogre::Vector3 holdingPointRel = holdingOffset;// - oldPosition;
	if(rotateStartDoOnce)
	{
		oldIntersectionPoint =holdingOffset;
		rotateStartDoOnce = false;
		startOrientation = oldOrientation;
	}
			
	//interSectPointRel = oldOrientation*interSectPointRel;
	switch(curTransform)
	{
	case tAxisX:
		oldIntersectionPoint.x = 0;
		interSectPointRel.x = 0;
		break;
	case tAxisY:
		oldIntersectionPoint.y = 0;
		interSectPointRel.y = 0;
		break;
	case tAxisZ:
		oldIntersectionPoint.z = 0;
		interSectPointRel.z = 0;
	}

	



	newOrientation = oldIntersectionPoint.getRotationTo(interSectPointRel);
//		rotatedOrientation = newOrientation * rotatedOrientation;

	
	oldIntersectionPoint = interSectPointRel;
	
	//else
	//{
	//	Ogre::Vector3 axis;
	//	//hier der snap-code
	//	Ogre::Quaternion test = Ogre::Quaternion::IDENTITY;
	//	Ogre::Vector3 facingDir; //richtung, in die das objekt zeigt, abh von achse
	//	switch(curTransform)
	//	{
	//	case tAxisX:
	//		if(rotateStartDoOnce)
	//		{
	//			oldIntersectionPoint = snapVector(Ogre::Degree(app->angleSnap),holdingOffset,Ogre::Vector3::UNIT_X);
	//			
	//			rotateStartDoOnce = false;
	//			facingDir = Ogre::Vector3::UNIT_Y;
	//			facingDir = oldOrientation * facingDir;
	//			facingDir.x = 0;
	//			test = facingDir.getRotationTo(oldIntersectionPoint);
	//		}	
	//		
	//		axis = Ogre::Vector3::UNIT_X;
	//		oldIntersectionPoint.x = 0;
	//		interSectPointRel.x = 0;
	//		break;
	//	case tAxisY:
	//		if(rotateStartDoOnce)
	//		{
	//			oldIntersectionPoint = snapVector(Ogre::Degree(app->angleSnap),holdingOffset,Ogre::Vector3::UNIT_X);
	//			
	//			rotateStartDoOnce = false;
	//			facingDir = Ogre::Vector3::UNIT_Z;
	//			facingDir = oldOrientation * facingDir;
	//			facingDir.y = 0;
	//			test = facingDir.getRotationTo(oldIntersectionPoint);
	//		}		
	//		
	//		axis = Ogre::Vector3::UNIT_Y;
	//		oldIntersectionPoint.y = 0;
	//		interSectPointRel.y = 0;
	//		break;
	//	case tAxisZ:
	//		if(rotateStartDoOnce)
	//		{
	//			oldIntersectionPoint = snapVector(Ogre::Degree(app->angleSnap),holdingOffset,Ogre::Vector3::UNIT_X);
	//			
	//			rotateStartDoOnce = false;
	//			facingDir = Ogre::Vector3::UNIT_Z;
	//			facingDir = oldOrientation * facingDir;
	//			facingDir.z = 0;
	//			test = facingDir.getRotationTo(oldIntersectionPoint);
	//		}	
	//		
	//		axis = Ogre::Vector3::UNIT_Z;
	//		oldIntersectionPoint.z = 0;
	//		interSectPointRel.z = 0;
	//		break;
	//	}
	//	
	//	interSectPointRel = snapVector(Ogre::Degree(app->angleSnap),interSectPointRel,axis);

	//	newOrientation = test*oldIntersectionPoint.getRotationTo(interSectPointRel);
	//	//rotatedOrientation = newOrientation * rotatedOrientation;

	//	
	//	oldIntersectionPoint = interSectPointRel;
	//	

	//}
	//
//
//	if(app->snapOnAngle)
//	{
//
//		int cnt = floor(360/app->angleSnap); //anzahl der richtungsvektoren
//		Ogre::Quaternion rotBy; //rotation von einem r-vektor zum nächsten
//		Ogre::Vector3 baseAxis; //globale achse, von der alles ausgeht
//		Ogre::Vector3 locAxis; //lokale achse mit !!newOrientation!!. entspricht der baseAxis, nur gedreht. also die richtung des objekts
//		Ogre::Vector3 oldLocAxis; //lok. achse mit !!oldOrientation!!
//		switch(curTransform)
//		{
//		case tAxisX:
//			rotBy.FromAngleAxis(Ogre::Degree(app->angleSnap),Ogre::Vector3::UNIT_X); //rotation von einem r-vektor zum nächsten
//			baseAxis = Ogre::Vector3::UNIT_Z; //globale achse, von der die Rotation ausgeht. also der erste mögliche snap-vektor
//			locAxis = (newOrientation*oldOrientation)* baseAxis; //lokale achse mit !!newOrientation!!. entspricht der baseAxis, nur gedreht. also die richtung des objekts
//			oldLocAxis = oldOrientation * baseAxis; //lok. achse mit !!oldOrientation!!
//
//			locAxis.normalise();
//			if(locAxis.directionEquals(Ogre::Vector3::UNIT_X,Ogre::Degree(10)) || locAxis.directionEquals(Ogre::Vector3::NEGATIVE_UNIT_X,Ogre::Degree(10)))
//			{
//				//wenn Z zu nahe an Y-achse, dann nehmen wir X
//				app->setDebugText("if getroffen");
//				baseAxis = Ogre::Vector3::UNIT_Y;
//				oldLocAxis = oldOrientation * baseAxis; //baseAxis drehen, wie das objekt
//				locAxis = (newOrientation*oldOrientation) * baseAxis;
//
//			}
//			locAxis.x = 0;
//			oldLocAxis.x = 0;
//			break;
//		case tAxisZ:
//			rotBy.FromAngleAxis(Ogre::Degree(app->angleSnap),Ogre::Vector3::UNIT_Z); //rotation von einem r-vektor zum nächsten
//
//			baseAxis = Ogre::Vector3::UNIT_Y; //globale achse, von der alles ausgeht
//
//
//			locAxis = (newOrientation*oldOrientation )* baseAxis; //lokale achse mit !!newOrientation!!. entspricht der baseAxis, nur gedreht. also die richtung des objekts
//			oldLocAxis = oldOrientation * baseAxis; //lok. achse mit !!oldOrientation!!
//			locAxis.normalise();
//
//			if(locAxis.directionEquals(Ogre::Vector3::UNIT_Z,Ogre::Degree(10)) || locAxis.directionEquals(Ogre::Vector3::NEGATIVE_UNIT_Z,Ogre::Degree(10)))
//			{
//				//wenn Z zu nahe an Y-achse, dann nehmen wir X
//
//				baseAxis = Ogre::Vector3::UNIT_X;
//				oldLocAxis = oldOrientation * baseAxis; //baseAxis drehen, wie das objekt
//
//
//				locAxis = (newOrientation*oldOrientation ) * baseAxis;
//
//
//			}
//			locAxis.z = 0;
//			oldLocAxis.z = 0;
//			break;
//		case tAxisY:
//			rotBy.FromAngleAxis(Ogre::Degree(app->angleSnap),Ogre::Vector3::UNIT_Y); //rotation von einem r-vektor zum nächsten
//			baseAxis = Ogre::Vector3::UNIT_Z; //globale achse, von der alles ausgeht
//			locAxis = (newOrientation*oldOrientation )* baseAxis; //lokale achse mit !!newOrientation!!. entspricht der baseAxis, nur gedreht. also die richtung des objekts
//			oldLocAxis = oldOrientation * baseAxis; //lok. achse mit !!oldOrientation!!
//
//			locAxis.normalise();
//			if(locAxis.directionEquals(Ogre::Vector3::UNIT_Y,Ogre::Degree(10)) || locAxis.directionEquals(Ogre::Vector3::NEGATIVE_UNIT_Y,Ogre::Degree(10)))
//			{
//				//wenn Z zu nahe an Y-achse, dann nehmen wir X
//
//				baseAxis = Ogre::Vector3::UNIT_X;
//				oldLocAxis = oldOrientation * baseAxis; //baseAxis drehen, wie das objekt
//
//
//				locAxis = (newOrientation*oldOrientation ) * baseAxis;
//
//
//			}
//			locAxis.y = 0;
//			oldLocAxis.y = 0;
//		}//ende von switch
//		Ogre::Vector3 snap = baseAxis; //diese var wird in der schleife geupdated
//		locAxis = interSectPointRel;
//		
//		Ogre::Quaternion debug = Ogre::Vector3::UNIT_Z.getRotationTo(oldLocAxis);
//		rotInd1->setOrientation(debug);
//		
//		debug = Ogre::Vector3::UNIT_Z.getRotationTo(locAxis);
//		rotInd2->setOrientation(debug);
//		
//		Ogre::Quaternion offset = baseAxis.getRotationTo(locAxis); //die aktuelle Rotation der lokalen achse als quaternion
//		mLog("***beginne Suche!***");
//		locAxis.normalise();
//		
//		Ogre::Vector3 hold_temp = oldOrientation*holdingOffset;
//		hold_temp.y = 0;
//		hold_temp.normalise();
//		oldLocAxis.normalise();
//		Ogre::Quaternion muh = hold_temp.getRotationTo(oldLocAxis);
//		locAxis = startOrientation.Inverse()*locAxis;
//		locAxis.y = 0;
//		//Ogre::Degree phi = Ogre::Degree(Ogre::Math::ACos(hold_temp.dotProduct(oldLocAxis)));
////		Ogre::Real wtf = Ogre::Math::Floor(phi.valueDegrees() / app->angleSnap);
//		
//		mDebugText("snap 1mal gedreht");
//		int cnt = floor(360/app->angleSnap); //anzahl der richtungsvektoren
//		Ogre::Quaternion rotBy; //rotation von einem r-vektor zum nächsten
//		for(int i=1;i<=cnt;i++)
//		{
//			
//			Ogre::Degree winkel = Ogre::Degree(Ogre::Math::ACos(locAxis.dotProduct(snap)));
//			
//			if(winkel <= Ogre::Degree(app->angleSnap/2))
//			{
//				//gefunden!
//
//
//				break;
//				//danach gleich snap verwenden
//			}
//			//das snap weiterdrehen
//			mDebugText("snap "+str(i+1)+"mal gedreht");
//			snap = rotBy * snap;
//		}
//
//		newOrientation = oldLocAxis.getRotationTo(snap);
//	}
	
/**********************************************************************************/
	//if(rotateStartDoOnce)
	//{
	//	rotateStartDoOnce = false;
	//	mDebugText("doOnce lol!");
	//	Ogre::Vector3::NEGATIVE_UNIT_Z.getRotationTo(
	//	//newOrientation = oldOrientation*newOrientation;
	//}
	//newOrientation = oldOrientation*newOrientation;
	//mDebugText(str(oldOrientation*newOrientation));
	//rotInd2->rotate(newOrientation,Ogre::Node::TS_WORLD);
	//if(app->snapOnAngle)
	//{

	//	int cnt = floor(360/app->angleSnap); //anzahl der richtungsvektoren
	//	Ogre::Quaternion rotBy; //rotation von einem r-vektor zum nächsten
	//	Ogre::Vector3 baseAxis; //globale achse, von der alles ausgeht
	//	Ogre::Vector3 locAxis; //lokale achse mit !!newOrientation!!. entspricht der baseAxis, nur gedreht. also die richtung des objekts
	//	Ogre::Vector3 oldLocAxis; //lok. achse mit !!oldOrientation!!

	//	switch(axis)
	//	{
	//	case AXIS_X:
	//		rotBy.FromAngleAxis(Ogre::Degree(app->angleSnap),Ogre::Vector3::UNIT_X); //rotation von einem r-vektor zum nächsten
	//		baseAxis = Ogre::Vector3::UNIT_Z; //globale achse, von der alles ausgeht
	//		locAxis = (newOrientation*oldOrientation )* baseAxis; //lokale achse mit !!newOrientation!!. entspricht der baseAxis, nur gedreht. also die richtung des objekts
	//		oldLocAxis = oldOrientation * baseAxis; //lok. achse mit !!oldOrientation!!

	//		locAxis.normalise();
	//		if(locAxis.directionEquals(Ogre::Vector3::UNIT_X,Ogre::Degree(10)) || locAxis.directionEquals(Ogre::Vector3::NEGATIVE_UNIT_X,Ogre::Degree(10)))
	//		{
	//			//wenn Z zu nahe an Y-achse, dann nehmen wir X
	//			app->setDebugText("if getroffen");
	//			baseAxis = Ogre::Vector3::UNIT_Y;
	//			oldLocAxis = oldOrientation * baseAxis; //baseAxis drehen, wie das objekt
	//			locAxis = (newOrientation*oldOrientation ) * baseAxis;

	//		}
	//		locAxis.x = 0;
	//		oldLocAxis.x = 0;
	//		break;
	//	case AXIS_Y:
	//		rotBy.FromAngleAxis(Ogre::Degree(app->angleSnap),Ogre::Vector3::UNIT_Z); //rotation von einem r-vektor zum nächsten

	//		baseAxis = Ogre::Vector3::UNIT_Y; //globale achse, von der alles ausgeht


	//		locAxis = (newOrientation*oldOrientation )* baseAxis; //lokale achse mit !!newOrientation!!. entspricht der baseAxis, nur gedreht. also die richtung des objekts
	//		oldLocAxis = oldOrientation * baseAxis; //lok. achse mit !!oldOrientation!!
	//		locAxis.normalise();

	//		if(locAxis.directionEquals(Ogre::Vector3::UNIT_Z,Ogre::Degree(10)) || locAxis.directionEquals(Ogre::Vector3::NEGATIVE_UNIT_Z,Ogre::Degree(10)))
	//		{
	//			//wenn Z zu nahe an Y-achse, dann nehmen wir X

	//			baseAxis = Ogre::Vector3::UNIT_X;
	//			oldLocAxis = oldOrientation * baseAxis; //baseAxis drehen, wie das objekt


	//			locAxis = (newOrientation*oldOrientation ) * baseAxis;


	//		}
	//		locAxis.z = 0;
	//		oldLocAxis.z = 0;
	//		break;
	//	case AXIS_Z:
	//		rotBy.FromAngleAxis(Ogre::Degree(app->angleSnap),Ogre::Vector3::UNIT_Y); //rotation von einem r-vektor zum nächsten
	//		baseAxis = Ogre::Vector3::UNIT_Z; //globale achse, von der alles ausgeht
	//		locAxis = (newOrientation*oldOrientation )* baseAxis; //lokale achse mit !!newOrientation!!. entspricht der baseAxis, nur gedreht. also die richtung des objekts
	//		oldLocAxis = oldOrientation * baseAxis; //lok. achse mit !!oldOrientation!!

	//		locAxis.normalise();
	//		if(locAxis.directionEquals(Ogre::Vector3::UNIT_Y,Ogre::Degree(10)) || locAxis.directionEquals(Ogre::Vector3::NEGATIVE_UNIT_Y,Ogre::Degree(10)))
	//		{
	//			//wenn Z zu nahe an Y-achse, dann nehmen wir X

	//			baseAxis = Ogre::Vector3::UNIT_X;
	//			oldLocAxis = oldOrientation * baseAxis; //baseAxis drehen, wie das objekt


	//			locAxis = (newOrientation*oldOrientation ) * baseAxis;


	//		}
	//		locAxis.y = 0;
	//		oldLocAxis.y = 0;
	//		break;
	//	}



	//	Ogre::Vector3 snap = baseAxis; //diese var wird in der schleife geupdated
	//	//Ogre::Quaternion snapQuat = Ogre::Quaternion::IDENTITY; //das ist das quaternion zwischen baseAxis und snap. es wird danach verwendet werden

	//	Ogre::Quaternion offset = baseAxis.getRotationTo(locAxis); //die aktuelle Rotation der lokalen achse als quaternion
	//	mLog("***beginne Suche!***");
	//	for(int i=1;i<=cnt;i++)
	//	{
	//		
	//		Ogre::Degree winkel = Ogre::Degree(Ogre::Math::ACos(locAxis.dotProduct(snap)));
	//		
	//		if(winkel <= Ogre::Degree(app->angleSnap/2))
	//		{
	//			//gefunden!


	//			break;
	//			//danach gleich snap verwenden
	//		}
	//		//das snap weiterdrehen
	//		snap = rotBy * snap;
	//	}

	//	newOrientation = oldLocAxis.getRotationTo(snap);


	//}
	if(!app->snapOnAngle)
	{
		selectedObject->rotate(newOrientation,Ogre::Node::TS_WORLD);
		startOrientation = newOrientation*oldOrientation;
	}
	else
	{
		//genauso, nur dass es einrastet
		//dir wird immer gedreht, so als ob es das objekt wäre, startOrientation
		//ist der zugehörige quaternion:
		Ogre::Vector3 axis; //rotationsachse
		Ogre::Vector3 basedir; //davon geht die rotation in der Ebene aus
		Ogre::Vector3 dir;
		Ogre::Vector3 snap;
		switch(curTransform)
		{
		case tAxisX:
			axis = Ogre::Vector3::UNIT_X;
			basedir = Ogre::Vector3::UNIT_Y;
			dir = (newOrientation*startOrientation)*basedir;//Ogre::Vector3::UNIT_X;
			//dir kann nahezu null werden!
			//das wählt die andere mögliche achse aus, wenn 
			//nach der Rotation kaum noch was übrigbleibt
			if(Ogre::Math::Abs(dir.y) < 0.01 && Ogre::Math::Abs(dir.z) < 0.01)
			{
				basedir = Ogre::Vector3::UNIT_Z;
				dir = (newOrientation*startOrientation)*basedir;//Ogre::Vector3::UNIT_X;
			}
			snap = snapVector(Ogre::Degree(app->angleSnap),dir,axis);//Ogre::Vector3::UNIT_Y);
			dir.x = 0;
			snap.x = 0;
			break;
		case tAxisY:
			axis = Ogre::Vector3::UNIT_Y;
			basedir = Ogre::Vector3::UNIT_Z;
			dir = (newOrientation*startOrientation)*basedir;//Ogre::Vector3::UNIT_X;
			if(Ogre::Math::Abs(dir.x) < 0.01 && Ogre::Math::Abs(dir.z) < 0.01)
			{
				basedir = Ogre::Vector3::UNIT_X;
				dir = (newOrientation*startOrientation)*basedir;//Ogre::Vector3::UNIT_X;
			}
			snap = snapVector(Ogre::Degree(app->angleSnap),dir,axis);//Ogre::Vector3::UNIT_Y);
			dir.y = 0;
			snap.y = 0;
			break;
		case tAxisZ:
			axis = Ogre::Vector3::UNIT_Z;
			basedir = Ogre::Vector3::UNIT_X;
			dir = (newOrientation*startOrientation)*basedir;//Ogre::Vector3::UNIT_X;
			if(Ogre::Math::Abs(dir.x) < 0.01 && Ogre::Math::Abs(dir.y) < 0.01)
			{
				basedir = Ogre::Vector3::UNIT_Y;
				dir = (newOrientation*startOrientation)*basedir;//Ogre::Vector3::UNIT_X;
				//if(Ogre::Math::Abs(dir.x) < 0.01 && Ogre::Math::Abs(dir.y) < 0.01 && Ogre::Math::Abs(dir.z) < 0.01)
				//{
				//	basedir = Ogre::Vector3::UNIT_Z;
				//	dir = (newOrientation*startOrientation)*basedir;//Ogre::Vector3::UNIT_X;
				//}
			}

			snap = snapVector(Ogre::Degree(app->angleSnap),dir,axis);//Ogre::Vector3::UNIT_Y);
			dir.z = 0;
			snap.z = 0;
		}

		//mDebugText("dir="+str(dir));
		//mDebugText("\nbasedir="+str(basedir),true);
		//mDebugText("\naxis="+str(axis),true);

		

		

		//Ogre::Quaternion debug = Ogre::Vector3::UNIT_Z.getRotationTo(snap);
//		rotInd1->setOrientation(newOrientation);
		//rot

		//debug = Ogre::Vector3::UNIT_Z.getRotationTo(dir);
//		rotInd2->setOrientation(newOrientation*startOrientation);
		//blau

		startOrientation = newOrientation * startOrientation;
		//selectedObject->rotate(newOrientation,Ogre::Node::TS_WORLD);

		if(snap != lastSnapVector)
		{
			Ogre::Vector3 curdir = oldOrientation*basedir;//Ogre::Vector3::UNIT_X;
			switch(curTransform)
			{
			case tAxisX:
				curdir.x = 0;
				break;
			case tAxisY:
				curdir.y = 0;
				break;
			case tAxisZ:
				curdir.z = 0;
			}
			
			newOrientation = curdir.getRotationTo(snap);
			selectedObject->rotate(newOrientation,Ogre::Node::TS_WORLD);	
		//	startOrientation = newOrientation * startOrientation;
			lastSnapVector = snap;
		//	
		}	
		//else
		//{
		//	//selectedObject->rotate(newOrientation,Ogre::Node::TS_WORLD);	
		//	startOrientation = newOrientation * startOrientation;
		//}
		
	}
}

bool EditFrameListener::isKeyDown(UniKey k)
{
	//mMouse->getMouseState().buttonDown(
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

GameObject* EditFrameListener::doObjectRaycast()
{
    if(selectionLocked)
        return selectedObject;



	holdingOffset = Ogre::Vector3(0,0,0);
	intersectionPoint = Ogre::Vector3(0,0,0);



	
	// Setup the ray scene query
	CEGUI::Point mousepos =	CEGUI::MouseCursor::getSingleton().getPosition();
	Ogre::Camera *mCamera = app->getCurrentLevel()->getMainCam();//mSceneMgr->getCamera("PlayerCam");
	Ogre::Real mouseX = mousepos.d_x/mWindow->getWidth();
	Ogre::Real mouseY = mousepos.d_y/mWindow->getHeight();
	Ogre::Ray mouseRay = mCamera->getCameraToViewportRay( mouseX, mouseY );

	//neue herangehensweise:
	//wenn unter den ergebnissen eines der achsendinger ist, hat das priorität!

	Ogre::SceneManager *mSceneMgr = app->currentLevel->getSceneManager();
	Ogre::RaySceneQuery *	mRaySceneQuery = mSceneMgr->createRayQuery(mouseRay);
	mRaySceneQuery->setSortByDistance(true);
	// Execute query
	Ogre::RaySceneQueryResult &result = mRaySceneQuery->execute();

	Ogre::RaySceneQueryResultEntry closestObjectRes;
	GameObject *closestObject = NULL;
	Ogre::RaySceneQueryResultEntry closestAxisRes;
	TransformAxis closestAxis = tNone;

	
	for(Ogre::RaySceneQueryResult::iterator itr = result.begin( );
		itr != result.end();itr++)
	{
		if(itr->movable && itr->movable->getMovableType() == "Ogre::Entity")
		{
			Ogre::Entity *ent = static_cast<Ogre::Entity*>(itr->movable);  //mSceneMgr->getEntity(closest_name);
			Ogre::SceneNode *nodeFound = ent->getParentSceneNode();

			Ogre::RaySceneQueryResultEntry curEntry;
			curEntry.distance = -1.0f;
			curEntry.movable = NULL;
			//bis-zum-polygon-test machen
			//polygon check BEGIN-------------------------------------
			// get the entity to check
			
			// mesh data to retrieve
			size_t vertex_count;
			size_t index_count;
			Ogre::Vector3 *vertices;
			unsigned long *indices;

			// get the mesh information
			Ogre::MeshPtr temp = ent->getMesh();
			Ogre::Mesh *myMesh = temp.getPointer();

			getMeshInformation(myMesh, vertex_count, vertices, index_count, indices,
			nodeFound->_getDerivedPosition(),
			nodeFound->_getDerivedOrientation(),
			getWorldScale(mSceneMgr,nodeFound)/*Ent->getParentNode()->getScale()*/);

			/*Ogre::Vector3 test1 = nodeFound->_getDerivedPosition();
			Ogre::Quaternion test2 = nodeFound->_getDerivedOrientation();
			Ogre::Vector3 test3 = nodeFound->getScale();
			Ogre::Vector3 test4 = getWorldScale(mSceneMgr,nodeFound);*/

			// test for hitting individual triangles on the mesh
			//bool new_closest_found = false;
			Ogre::Real closest_distance = -1.0f;
			Ogre::Vector3 closest_result;
			for (int i = 0; i < static_cast<int>(index_count); i += 3)
			{
				// check for a hit against this triangle
				std::pair<bool, Ogre::Real> hit = Ogre::Math::intersects(mouseRay, vertices[indices[i]],
				vertices[indices[i+1]], vertices[indices[i+2]], true, false);

				// if it was a hit check if its the closest
				if (hit.first)
				{
					if ((closest_distance < 0.0f) ||
					(hit.second < closest_distance))
					{
						// this is the closest so far, save it off
						closest_distance = hit.second;

						//closest_mov = itr->movable;

						curEntry = *itr;




						//new_closest_found = true;
					}
				}
			}

			// free the verticies and indicies memory
			delete[] vertices;
			delete[] indices;

			
			//polygon check END--------------------------
			if(!curEntry.movable)
				continue;
			
			//intersectionPoint = mouseRay.getPoint(closestResult.distance);			
			//wenn das any gesetzt ist, ist das das objekt
			if(!closestObject && !ent->getUserAny().isEmpty())
			{
				closestObjectRes = curEntry;
				closestObject = Ogre::any_cast<GameObject*>(ent->getUserAny());				
			}
			if(closestAxis == tNone)
			{
				if(axe_X == nodeFound)
					closestAxis = tAxisX;
				else if(axe_Y == nodeFound)
					closestAxis = tAxisY;
				else if(axe_Z == nodeFound)
					closestAxis = tAxisZ;

				else if(plane_XY == nodeFound)
					closestAxis = tPlaneXY;
				else if(plane_XZ == nodeFound)
					closestAxis = tPlaneXZ;
				else if(plane_YZ == nodeFound)
					closestAxis = tPlaneYZ;

				else if(boxAll == nodeFound)
					closestAxis = tAll;

				if(closestAxis != tNone)
					closestAxisRes = curEntry;
			}
			if(closestAxis != tNone)
				break;//da die achsensache priorität hat, kann man jetzt aufhören

		}
	}

//	Ogre::RaySceneQueryResultEntry closestResult = getClosest(app->getCurrentLevel()->getSceneManager(),
//		mouseRay,0,0,true, "Ogre::Entity");
//
//
//
//	
//
//
//	if(closestResult.movable)
//	{
////		app->setDebugText(closestResult.movable->getMovableType()+"\n"+closestResult.movable->getName());
//
//
//		if(closestResult.movable->getMovableType() == "Ogre::Entity")
//		{
//			//HIER OBJEKT SELEKTIEREN
//
//			//closest_name is the name of the entity. let's find the node...
//			Ogre::Entity *ent = static_cast<Ogre::Entity*>(closestResult.movable);  //mSceneMgr->getEntity(closest_name);
//
//			nodeFound = ent->getParentSceneNode();
//			//objParentNode = getMostParentNode(app->getCurrentLevel()->getSceneManager(),objParentNode);
//			intersectionPoint = mouseRay.getPoint(closestResult.distance);
//			
//			/*Ogre::SceneNode *selectedNode = (Ogre::SceneNode*)objParentNode;
//			selectedObject = app->getCurrentLevel()->getObject(selectedNode);*/
//			if(!ent->getUserAny().isEmpty())
//			{
//				newSelected = Ogre::any_cast<GameObject*>(ent->getUserAny());
//			}
//			else
//				newSelected = NULL;
//
//			//mDebugText(ent->getName());
//			
//
//		}//wenn closest_name == ""
//		else
//		{
//			//definitiv deselect
////			setSelected(NULL);
//			nextSelected = NULL;
////			mDebugText("deselect");
//			
//
//		}
//	}//if(result.begin != result.end())

	//jetzt prüfen ob ich was neues habe
	if(closestAxis != tNone)
	{
		
		closestObject = selectedObject;		
		
		intersectionPoint = mouseRay.getPoint(closestAxisRes.distance);
		selectedObject->getPositionOrientation(startPosition,startOrientation);
		holdingOffset = intersectionPoint - startPosition;	
		startScale = selectedObject->getScale();
		unSnappedScale = startScale;

//		rotatedOrientation = startOrientation;
//		Ogre::Quaternion test = Ogre::Vector3::NEGATIVE_UNIT_Z.getRotationTo(holdingOffset);
//		rotInd1->setOrientation(test);
		//app->getCurrentLevel()->debugShowPoint(intersectionPoint);
		rotateStartDoOnce = true;
		scaleDoOnce = true;
		oldHitpoint = Ogre::Vector3::ZERO;
		lastSnapVector = Ogre::Vector3::ZERO;

	}
	else
	{
		//dh keine achsen, jetzt das eventuelle gewählte objekt verarbeiten
		//gar nix?!
	}
	curTransform = closestAxis;
//	if(newSelected && newSelected != selectedObject)
//	{
//		
////		mDebugText("neues Obj gefunden");
//		
//
//	}
//	else if(!newSelected && nodeFound)
//	{
//		newSelected = selectedObject;
//		//nextSelected = NULL;
////		mDebugText("achse");
//		//kein Objekt, aber ne Ogre::Node
//		if(axe_X == nodeFound)
//			curTransform = tAxisX;
//		else if(axe_Y == nodeFound)
//			curTransform = tAxisY;
//		else if(axe_Z == nodeFound)
//			curTransform = tAxisZ;
//
//		else if(plane_XY == nodeFound)
//			curTransform = tPlaneXY;
//		else if(plane_XZ == nodeFound)
//			curTransform = tPlaneXZ;
//		else if(plane_YZ == nodeFound)
//			curTransform = tPlaneYZ;
//
//		else if(boxAll == nodeFound)
//			curTransform = tAll;
//
//		//Ogre::Vector3 objectPosition;
//		//objectPosition = selectedObject->getPosition();
//		//startOrientation = selectedObject->getOrientation();//nodeFound->getOrientation();
//		//holdingOffset = intersectionPoint - objectPosition;	
//
//		selectedObject->getPositionOrientation(startPosition,startOrientation);
//		holdingOffset = intersectionPoint - startPosition;	
//		startScale = selectedObject->getScale();
//		unSnappedScale = startScale;
//
////		rotatedOrientation = startOrientation;
////		Ogre::Quaternion test = Ogre::Vector3::NEGATIVE_UNIT_Z.getRotationTo(holdingOffset);
////		rotInd1->setOrientation(test);
//		//app->getCurrentLevel()->debugShowPoint(intersectionPoint);
//		rotateStartDoOnce = true;
//		scaleDoOnce = true;
//		oldHitpoint = Ogre::Vector3::ZERO;
//		lastSnapVector = Ogre::Vector3::ZERO;
//
//	}
//	else if(newSelected != selectedObject)
//	{
////			mDebugText("deselect 2");
//		//nix. deselect.
//		//setSelected(NULL);
//		//newSelected = NULL;
//	}
	nextSelected = closestObject;
	mDebugText("nextSelected="+ptrToString(nextSelected));


		//END ALTER CODE


	//
	//intersectPointTemp = Ogre::Vector3(0,0,0);
	//
	////linksklick
	//// Setup the ray scene query
	//CEGUI::Point mousepos =	CEGUI::MouseCursor::getSingleton().getPosition();
	//Ogre::Camera *mCamera = app->getCurrentLevel()->getMainCam();//mSceneMgr->getCamera("PlayerCam");
	//Ogre::Real mouseX = mousepos.d_x/mWindow->getWidth();
	//Ogre::Real mouseY = mousepos.d_y/mWindow->getHeight();
	//Ogre::Ray mouseRay = mCamera->getCameraToViewportRay( mouseX, mouseY );

	//Ogre::RaySceneQueryResultEntry closestResult = getClosest(app->getCurrentLevel()->getSceneManager(),
	//	mouseRay,0,0,true);


	//

	//
	//if(closestResult.movable)
	//{
	//
	//
	//	if(closestResult.movable->getMovableType() == "Ogre::Entity")
	//	{
	//		//closest_name is the name of the entity. let's find the node...
	//		Ogre::Entity *ent = static_cast<Ogre::Entity*>(closestResult.movable);  //mSceneMgr->getEntity(closest_name);
	//
	//		Ogre::SceneNode *objParentNode = ent->getParentSceneNode();
	//		objParentNode = getMostParentNode(app->getCurrentLevel()->getSceneManager(),objParentNode);
	//
	//
	//		//app->setDebugText("closest: "+closestResult.movable->getName()+"\n",true);
	//
	//		intersectPointTemp = mouseRay.getPoint(closestResult.distance);
	//		/*intersectionPoint = mouseRay.getPoint(notrly_closest_dist);*/
	//

	//		Ogre::SceneNode *selectedNode = (Ogre::SceneNode*)objParentNode;

	//		//objParentNode->show

	//		return app->getCurrentLevel()->getObject(selectedNode);
	//		//selectedObject = app->getCurrentLevel()->getObject(selectedNode);

	//		//

	//		////selectedBody = selectedObject->getBody();//app->getBodyBySceneNode((Ogre::SceneNode*)objParentNode);
	//		////mSceneMgr->getSceneNode(closest_name);
	//		//
	//		//objectPosition = selectedObject->getPosition();
	//		//startOrientation = selectedObject->getOrientation();

	//		//holdingOffset = intersectionPoint - objectPosition;
	//		//
	//		////selectedNode->showBoundingBox(true);
	//		//selectedObject->markSelected(true);
	//
	//	}//wenn closest_name == ""
	//	else
	//	{
	//		return NULL;
	//
	//
	//	}
	//}//if(result.begin != result.end())
	//return NULL;
	return closestObject;
}

void EditFrameListener::updateSelection()
{
	//ich habe ein neues objekt gefunden
	//prüfen, ob ich ne taste gedrückt halte, um zu gruppieren
	if(nextSelected == selectedObject)
		return;
	if(isKeyDown(key_group) && selectedObject)
	{
		if(selectedObject->getType() == otGroup)
		{
			//adden
			GroupObject *gr = (GroupObject*)selectedObject;
			if(gr->hasObject(nextSelected))
			{
				//abwählen
				gr->removeObject(nextSelected);
				nextSelected->markSelected(false);
				
			}
			else
			{
				gr->addObject(nextSelected);
				gr->markSelected(true);
			}
			updateObjectInfo();
			setSelected(gr);
		}
		else
		{
			GroupObject *gr = new GroupObject(app->getCurrentLevel(),selectedObject);
			gr->addObject(nextSelected);
			setSelected(gr);
		}
	}
	else
	{
		setSelected(nextSelected);
		
		
	}

}

void EditFrameListener::setDecalScale(Ogre::ushort scale)
{
    Level *lvl = app->getCurrentLevel();
    Ogre::AxisAlignedBox box = lvl->getTerrainExtends();
    size_t t_height = lvl->getTerrainInfo()->getHeight();
    size_t t_width = lvl->getTerrainInfo()->getWidth();
    Ogre::Vector3 size = box.getSize();
    /*Ogre::Real width = size.x/100*scale;
    Ogre::Real height= size.z/100*scale;*/
    /*Ogre::Real width = 100*float(scale)/float(t_width);
    Ogre::Real height= 100*float(scale)/float(t_height);*/
    /*Ogre::Real width = float(scale)/float(t_width)*400;
    Ogre::Real height= float(scale)/float(t_height)*400;*/
    Ogre::Vector3 tscale = lvl->getTerrainScale();
//    mDebugText(str(tscale));
    Ogre::Real width = tscale.z*scale;
    Ogre::Real height= tscale.z*scale;
    decal->setSize(Ogre::Vector2(width,height));
}

void EditFrameListener::showMouseCursor(bool show)
{
	if(show)
	{
		CEGUI::MouseCursor::getSingleton().show();
		cursorShown = true;
        if(decal)
            decal->show();
	}
	else
	{
		CEGUI::MouseCursor::getSingleton().hide();
		cursorShown = false;
        if(decal)
            decal->hide();
	}
}

void EditFrameListener::showAxes(GameObject *obj)
{
	//Ogre::RenderQueue
	if(axesNode)
		hideAxes();

	Ogre::SceneManager *mgr = app->getCurrentLevel()->getSceneManager();
	axesNode = obj->getSceneNode()->createChildSceneNode();
	axesNode->setInheritOrientation(false);
	axesNode->setInheritScale(false);
	Ogre::String meshName = MESH_MOVE;
	Ogre::Vector3 axesScale = Ogre::Vector3::UNIT_SCALE;
	bool addPlanes = false;
	switch(mTMode)
	{
	case tmMove:
		{
			addPlanes = true;
			meshName = MESH_MOVE;
			axesScale = Ogre::Vector3(0.1);
			
		}
		break;
	case tmRotate:
		{
			
			meshName = MESH_ROTATE;
			//debug: das soll dann zeigen, um wie viel man rotiert
			
			
		}
		break;
	case tmScale:
		{
			axesScale = Ogre::Vector3(0.1);
			meshName = MESH_SCALE;
			addPlanes = true;

			Ogre::Entity *ent_box = mgr->createEntity(getUniqueEntityName(mgr),"tetraeder.mesh");
			ent_box->setQueryFlags(QM_TRANSFORMAXES);
			ent_box->setRenderQueueGroup(Ogre::RENDER_QUEUE_9);
			boxAll = axesNode->createChildSceneNode();
			ent_box->setMaterialName(MAT_ALL);
			boxAll->attachObject(ent_box);
			boxAll->setScale(Ogre::Vector3(0.2,0.2,0.2));
			/*plane_XZ = NULL;
			plane_XY = NULL;
			plane_YZ = NULL;
			boxAll = NULL;*/
			axesNode->setInheritOrientation(true);

			/*Ogre::Entity *ent_ind1 = mgr->createEntity(getUniqueEntityName(mgr),ENTR_MESH);
			ent_ind1->setQueryFlags(QM_TRANSFORMAXES);
			rotInd1 = axesNode->createChildSceneNode(Ogre::Vector3::ZERO,Ogre::Quaternion(Ogre::Degree(180),Ogre::Vector3::UNIT_Y));
			ent_ind1->setMaterialName(MATP_X);
			rotInd1->attachObject(ent_ind1);
			rotInd1->setScale(Ogre::Vector3(0.1,0.1,0.7));
			rotInd1->setOrientation(selectedObject->getOrientation());

			Ogre::Entity *ent_ind2 = mgr->createEntity(getUniqueEntityName(mgr),ENTR_MESH);
			ent_ind2->setQueryFlags(QM_TRANSFORMAXES);
			rotInd2 = axesNode->createChildSceneNode(Ogre::Vector3::ZERO,Ogre::Quaternion(Ogre::Degree(180),Ogre::Vector3::UNIT_Y));
			ent_ind2->setMaterialName(MATP_Y);
			rotInd2->attachObject(ent_ind2);
			rotInd2->setScale(Ogre::Vector3(0.1,0.1,0.7));
			rotInd2->setOrientation(selectedObject->getOrientation());*/
		}
		break;
	}

	//ebenen
	if(addPlanes)
	{
		Ogre::Entity *ent_xz = mgr->createEntity(getUniqueEntityName(mgr),"keil.mesh");
		ent_xz->setQueryFlags(QM_TRANSFORMAXES);
		ent_xz->setRenderQueueGroup(Ogre::RENDER_QUEUE_9);
		plane_XZ = axesNode->createChildSceneNode();
		ent_xz->setMaterialName(MATP_Y);
		plane_XZ->attachObject(ent_xz);
		plane_XZ->setPosition(Ogre::Vector3(0.04,0,-0.04));
		plane_XZ->setScale(Ogre::Vector3(0.25,0.25,0.01));
		plane_XZ->setOrientation(Ogre::Quaternion(Ogre::Degree(-90),Ogre::Vector3::UNIT_X));

		Ogre::Entity *ent_xy = mgr->createEntity(getUniqueEntityName(mgr),"keil.mesh");
		ent_xy->setRenderQueueGroup(Ogre::RENDER_QUEUE_9);
		ent_xy->setQueryFlags(QM_TRANSFORMAXES);
		plane_XY = axesNode->createChildSceneNode();
		ent_xy->setMaterialName(MATP_Z);
		plane_XY->attachObject(ent_xy);
		plane_XY->setPosition(Ogre::Vector3(0.04,0.04,0));
		plane_XY->setScale(Ogre::Vector3(0.25,0.25,0.01));
		//plane_XY->setOrientation(Ogre::Quaternion(Ogre::Degree(45),Ogre::Vector3::UNIT_Z));

		Ogre::Entity *ent_yz = mgr->createEntity(getUniqueEntityName(mgr),"keil.mesh");
		ent_yz->setRenderQueueGroup(Ogre::RENDER_QUEUE_9);
		ent_yz->setQueryFlags(QM_TRANSFORMAXES);
		plane_YZ = axesNode->createChildSceneNode();
		ent_yz->setMaterialName(MATP_X);
		plane_YZ->attachObject(ent_yz);
		plane_YZ->setScale(Ogre::Vector3(0.25,0.25,0.01));
		plane_YZ->setPosition(Ogre::Vector3(0,0.04,-0.04));
		plane_YZ->setOrientation(Ogre::Quaternion(Ogre::Degree(90),Ogre::Vector3::UNIT_Y));
	}
	
	//hauptachsen
	axe_X = axesNode->createChildSceneNode(Ogre::Vector3::ZERO,Ogre::Quaternion(Ogre::Degree(90),Ogre::Vector3::UNIT_Y));	
	Ogre::Entity *ent_x = mgr->createEntity(getUniqueEntityName(mgr),meshName);	
	ent_x->setQueryFlags(QM_TRANSFORMAXES);
	ent_x->setMaterialName(MAT_X);
	axe_X->attachObject(ent_x);
	ent_x->setRenderQueueGroup(Ogre::RENDER_QUEUE_9);	
	axe_X->setScale(axesScale);

	axe_Y = axesNode->createChildSceneNode(Ogre::Vector3::ZERO,Ogre::Quaternion(Ogre::Degree(90),Ogre::Vector3::UNIT_X));	
	Ogre::Entity *ent_y = mgr->createEntity(getUniqueEntityName(mgr),meshName);
	ent_y->setRenderQueueGroup(Ogre::RENDER_QUEUE_9);
	ent_y->setQueryFlags(QM_TRANSFORMAXES);
	ent_y->setMaterialName(MAT_Y);
	axe_Y->attachObject(ent_y);
	axe_Y->setScale(axesScale);

	axe_Z = axesNode->createChildSceneNode();	
	Ogre::Entity *ent_z = mgr->createEntity(getUniqueEntityName(mgr),meshName);
	ent_z->setRenderQueueGroup(Ogre::RENDER_QUEUE_9);
	ent_z->setQueryFlags(QM_TRANSFORMAXES);
	ent_z->setMaterialName(MAT_Z);
	axe_Z->attachObject(ent_z);
	axe_Z->setScale(axesScale);

	
	updateAxes();

    //nun ggF das rotatemove re-initen
    


}

//
//void EditFrameListener::showMoveAxes(GameObject *obj)
//{
//	if(axesNode)
//		hideAxes();
//	Ogre::SceneManager *mgr = app->getCurrentLevel()->getSceneManager();
//	axesNode = obj->getSceneNode()->createChildSceneNode();
//	axesNode->setInheritOrientation(false);
//	axesNode->setInheritScale(false);
//	
//
//	axe_X = axesNode->createChildSceneNode(Ogre::Vector3::ZERO,Ogre::Quaternion(Ogre::Degree(90),Ogre::Vector3::UNIT_Y));	
//	Ogre::Entity *ent_x = mgr->createEntity(getUniqueEntityName(mgr),MESH_MOVE);
//	ent_x->setMaterialName(MAT_X);
//	axe_X->attachObject(ent_x);
//
//	axe_Y = axesNode->createChildSceneNode(Ogre::Vector3::ZERO,Ogre::Quaternion(Ogre::Degree(90),Ogre::Vector3::UNIT_X));	
//	Ogre::Entity *ent_y = mgr->createEntity(getUniqueEntityName(mgr),MESH_MOVE);
//	ent_y->setMaterialName(MAT_Y);
//	axe_Y->attachObject(ent_y);
//
//	axe_Z = axesNode->createChildSceneNode();	
//	Ogre::Entity *ent_z = mgr->createEntity(getUniqueEntityName(mgr),MESH_MOVE);
//	ent_z->setMaterialName(MAT_Z);
//	axe_Z->attachObject(ent_z);
//
//
//}
//void EditFrameListener::showRotateAxes(GameObject *obj)
//{
//	if(axesNode)
//		hideAxes();
//	Ogre::SceneManager *mgr = app->getCurrentLevel()->getSceneManager();
//	axesNode = obj->getSceneNode()->createChildSceneNode();
//	axesNode->setInheritOrientation(false);
//	axesNode->setInheritScale(false);
//
//	axe_X = axesNode->createChildSceneNode(Ogre::Vector3::ZERO,Ogre::Quaternion(Ogre::Degree(90),Ogre::Vector3::UNIT_Y));	
//	Ogre::Entity *ent_x = mgr->createEntity(getUniqueEntityName(mgr),MESH_ROTATE);
//	ent_x->setMaterialName(MAT_X);
//	axe_X->attachObject(ent_x);
//
//	axe_Y = axesNode->createChildSceneNode(Ogre::Vector3::ZERO,Ogre::Quaternion(Ogre::Degree(90),Ogre::Vector3::UNIT_X));	
//	Ogre::Entity *ent_y = mgr->createEntity(getUniqueEntityName(mgr),MESH_ROTATE);
//	ent_y->setMaterialName(MAT_Y);
//	axe_Y->attachObject(ent_y);
//
//	axe_Z = axesNode->createChildSceneNode();	
//	Ogre::Entity *ent_z = mgr->createEntity(getUniqueEntityName(mgr),MESH_ROTATE);
//	ent_z->setMaterialName(MAT_Z);
//	axe_Z->attachObject(ent_z);
//}
//void EditFrameListener::showScaleAxes(GameObject *obj)
//{
//	if(axesNode)
//		hideAxes();
//	Ogre::SceneManager *mgr = app->getCurrentLevel()->getSceneManager();
//	axesNode = obj->getSceneNode()->createChildSceneNode();
//	axesNode->setInheritOrientation(false);
//	axesNode->setInheritScale(false);
//
//	axe_X = axesNode->createChildSceneNode(Ogre::Vector3::ZERO,Ogre::Quaternion(Ogre::Degree(90),Ogre::Vector3::UNIT_Y));	
//	Ogre::Entity *ent_x = mgr->createEntity(getUniqueEntityName(mgr),MESH_SCALE);
//	ent_x->setMaterialName(MAT_X);
//	axe_X->attachObject(ent_x);
//
//	axe_Y = axesNode->createChildSceneNode(Ogre::Vector3::ZERO,Ogre::Quaternion(Ogre::Degree(90),Ogre::Vector3::UNIT_X));	
//	Ogre::Entity *ent_y = mgr->createEntity(getUniqueEntityName(mgr),MESH_SCALE);
//	ent_y->setMaterialName(MAT_Y);
//	axe_Y->attachObject(ent_y);
//
//	axe_Z = axesNode->createChildSceneNode();	
//	Ogre::Entity *ent_z = mgr->createEntity(getUniqueEntityName(mgr),MESH_SCALE);
//	ent_z->setMaterialName(MAT_Z);
//	axe_Z->attachObject(ent_z);
//}
void EditFrameListener::updateAxes()
{

	if(!axesNode)
		return;
	if(!selectedObject)
	{
		hideAxes();
		return;
	}
	Ogre::Vector3 camPos = app->getCurrentLevel()->getMainCam()->getDerivedPosition();
	Ogre::Quaternion objOrient = axesNode->_getDerivedOrientation();
	//if(camPos == prevCamPos)
	//	return; //nicht nötig
	Ogre::Vector3 objPos = axesNode->_getDerivedPosition();
	Ogre::Vector3 camPosRel = objOrient.Inverse()*(camPos-objPos);
	
	Ogre::Real scalar = Ogre::Math::Sqrt(camPosRel.squaredLength()*app->axesScaleFactor);
	//mDebugText(str(scalar));
	axesNode->setScale(Ogre::Vector3(scalar));
	prevCamPos = camPos;

	//erstmal rausfinden, im welchen 3D-quadrant (relativ zum objekt) die cam sich befindet
	if(mTMode == tmRotate)
		return;
	
	//und jetzt hängt alles von den Vorzeichen der Komponenten von camPosRel ab
	int quadrant = 1;
	/*     
		    /\ y
	        |
	    __________
	   / 5  / 6  /|
	  /____/____/ |
	 /    /    /| /
	/____/___ / |/|   -->x
	|    |    | /7|
8->	| 1  | 2  |/| /
	|---------| |/
	| 3  | 4  | /
	|_________|/

	  /
    |/_  z

	*/

	if(camPosRel.x < 0)
		axe_X->setOrientation(Ogre::Quaternion(Ogre::Degree(90),Ogre::Vector3::UNIT_Y));
	else
		axe_X->setOrientation(Ogre::Quaternion(Ogre::Degree(-90),Ogre::Vector3::UNIT_Y));
	
	if(camPosRel.y > 0)
		axe_Y->setOrientation(Ogre::Quaternion(Ogre::Degree(90),Ogre::Vector3::UNIT_X));	
	else
		axe_Y->setOrientation(Ogre::Quaternion(Ogre::Degree(-90),Ogre::Vector3::UNIT_X));	
	
	if(camPosRel.z < 0)
		axe_Z->setOrientation(Ogre::Quaternion::IDENTITY);	
	else
		axe_Z->setOrientation(Ogre::Quaternion(Ogre::Degree(180),Ogre::Vector3::UNIT_Y));	
	

	if(camPosRel.x >= 0)
	{
		//2,4,6,7 
		if(camPosRel.y >= 0)
		{
			//2,6
			if(camPosRel.z >= 0)
				quadrant = 2;
			else
				quadrant = 6;
		}
		else
		{
			//4,7
			if(camPosRel.z >= 0)
				quadrant = 4;
			else
				quadrant = 7;
		}
	}
	else
	{
		//1,3,5,8
		if(camPosRel.y >= 0)
		{
			//1,5
			if(camPosRel.z >= 0)
				quadrant = 1;
			else
				quadrant = 5;
		}
		else
		{
			//3,8
			if(camPosRel.z >= 0)
				quadrant = 3;
			else
				quadrant = 8;
		}

	}
	//mDebugText(str(axe_X->getAttachedObject(0)->getRenderQueueGroup()));
//	mDebugText(str(quadrant));
	switch(quadrant)
	{
	case 1:		
		plane_XZ->setPosition(Ogre::Vector3(-0.04,0,0.04));
		plane_XZ->setOrientation(Ogre::Quaternion(0.5,0.5,-0.5,0.5));
		

		
		plane_XY->setPosition(Ogre::Vector3(-0.04,0.04,0));
		plane_XY->setOrientation(Ogre::Quaternion(0,0,1,0));//-1.04308e-007 0 1 0
		
		plane_YZ->setPosition(Ogre::Vector3(0,0.04,0.04)); //
		plane_YZ->setOrientation(Ogre::Quaternion(0.707107,0,-0.707107,0));

		if(boxAll)
		{
			boxAll->setPosition(Ogre::Vector3(-0.04,0.04,0.04));
			boxAll->setOrientation(Ogre::Quaternion(Ogre::Degree(90),Ogre::Vector3::NEGATIVE_UNIT_Y));
		}

		//90° an -Y
		break;
	case 2:
		plane_XZ->setPosition(Ogre::Vector3(0.04,0,0.04));
		plane_XZ->setOrientation(Ogre::Quaternion(Ogre::Degree(90),Ogre::Vector3::UNIT_X));

		
		plane_XY->setPosition(Ogre::Vector3(0.04,0.04,0));
		plane_XY->setOrientation(Ogre::Quaternion::IDENTITY);
		
		plane_YZ->setPosition(Ogre::Vector3(0,0.04,0.04)); //
		plane_YZ->setOrientation(Ogre::Quaternion(0.707107,0,-0.707107,0));

		if(boxAll)
		{
			boxAll->setPosition(Ogre::Vector3(0.04,0.04,0.04));
			boxAll->setOrientation(Ogre::Quaternion::IDENTITY);
		}
		//identity
		break;
	case 3:		
		plane_XZ->setPosition(Ogre::Vector3(-0.04,0,0.04));
		plane_XZ->setOrientation(Ogre::Quaternion(0.5,0.5,-0.5,0.5));

		plane_XY->setPosition(Ogre::Vector3(-0.04,-0.04,0));
		plane_XY->setOrientation(Ogre::Quaternion(0,0.707107,-0.707107,0));

		plane_YZ->setPosition(Ogre::Vector3(0,-0.04,0.04)); //
		plane_YZ->setOrientation(Ogre::Quaternion(-0.5,-0.5,0.5,0.5));

		if(boxAll)
		{
			boxAll->setPosition(Ogre::Vector3(-0.04,-0.04,0.04));
			boxAll->setOrientation(Ogre::Quaternion(0.5,0.5,-0.5,0.5));
		}
		
		//
		break;
	case 4:
		plane_XZ->setPosition(Ogre::Vector3(0.04,0,0.04));
		plane_XZ->setOrientation(Ogre::Quaternion(Ogre::Degree(90),Ogre::Vector3::UNIT_X));

		plane_XY->setPosition(Ogre::Vector3(0.04,-0.04,0));
		plane_XY->setOrientation(Ogre::Quaternion(0,1,0,0));

		plane_YZ->setPosition(Ogre::Vector3(0,-0.04,0.04)); //
		plane_YZ->setOrientation(Ogre::Quaternion(-0.5,-0.5,0.5,0.5));

		if(boxAll)
		{
			boxAll->setPosition(Ogre::Vector3(0.04,-0.04,0.04));
			boxAll->setOrientation(Ogre::Quaternion(Ogre::Degree(90),Ogre::Vector3::UNIT_X));
		}
		//90 an X
		break;
	case 5:
		plane_XZ->setPosition(Ogre::Vector3(-0.04,0,-0.04));
		plane_XZ->setOrientation(Ogre::Quaternion(0,0,-0.707107,0.707107));

		plane_XY->setPosition(Ogre::Vector3(-0.04,0.04,0));
		plane_XY->setOrientation(Ogre::Quaternion(0,0,1,0));//-1.04308e-007 0 1 0

		plane_YZ->setPosition(Ogre::Vector3(0,0.04,-0.04)); //
		plane_YZ->setOrientation(Ogre::Quaternion(0.707107,0,0.707107,0));

		if(boxAll)
		{
			boxAll->setPosition(Ogre::Vector3(-0.04,0.04,-0.04));
			boxAll->setOrientation(Ogre::Quaternion(Ogre::Degree(180),Ogre::Vector3::UNIT_Y));
		}
		//180 an Y
		break;
	case 6:
		plane_XZ->setPosition(Ogre::Vector3(0.04,0,-0.04));
		plane_XZ->setOrientation(Ogre::Quaternion(Ogre::Degree(-90),Ogre::Vector3::UNIT_X));

		plane_XY->setPosition(Ogre::Vector3(0.04,0.04,0));
		plane_XY->setOrientation(Ogre::Quaternion::IDENTITY);

		//YZ 
		plane_YZ->setPosition(Ogre::Vector3(0,0.04,-0.04)); //
		plane_YZ->setOrientation(Ogre::Quaternion(0.707107,0,0.707107,0));

		if(boxAll)
		{
			boxAll->setPosition(Ogre::Vector3(0.04,0.04,-0.04));
			boxAll->setOrientation(Ogre::Quaternion(Ogre::Degree(90),Ogre::Vector3::UNIT_Y));
		}
		//90 an Y
		break;
	case 7:
		plane_XZ->setPosition(Ogre::Vector3(0.04,0,-0.04));
		plane_XZ->setOrientation(Ogre::Quaternion(Ogre::Degree(-90),Ogre::Vector3::UNIT_X));

		plane_XY->setPosition(Ogre::Vector3(0.04,-0.04,0));
		plane_XY->setOrientation(Ogre::Quaternion(0,1,0,0));
		//XY 
		plane_YZ->setPosition(Ogre::Vector3(0,-0.04,-0.04)); //
		plane_YZ->setOrientation(Ogre::Quaternion(0,-0.707107,0,0.707107));
		
		if(boxAll)
		{
			boxAll->setPosition(Ogre::Vector3(0.04,-0.04,-0.04));
			boxAll->setOrientation(Ogre::Quaternion(-0.5, -0.5, -0.5, 0.5));
		}
		//-0.5 -0.5 -0.5 0.5
		break;
	case 8:
		plane_XZ->setPosition(Ogre::Vector3(-0.04,0,-0.04));
		plane_XZ->setOrientation(Ogre::Quaternion(0,0,-0.707107,0.707107));

		plane_XY->setPosition(Ogre::Vector3(-0.04,-0.04,0));
		plane_XY->setOrientation(Ogre::Quaternion(0,0.707107,-0.707107,0));
		//xy 
		plane_YZ->setPosition(Ogre::Vector3(0,-0.04,-0.04)); //
		plane_YZ->setOrientation(Ogre::Quaternion(0,-0.707107,0,0.707107));

		if(boxAll)
		{
			boxAll->setPosition(Ogre::Vector3(-0.04,-0.04,-0.04));
			boxAll->setOrientation(Ogre::Quaternion(0, 0, -0.707107, 0.707107));
		}

		//3.72529e-008 1.19209e-007 -0.707107 0.707107
		
		break;
	};


}


void EditFrameListener::hideAxes()
{
	if(!axesNode)
		return;
    
	Ogre::SceneManager *mgr = app->getCurrentLevel()->getSceneManager();
	destroyNode(mgr,axesNode);
	axesNode = NULL;
	axe_X = NULL;
	axe_Y = NULL;
	axe_Z = NULL;
	plane_XZ = NULL;
	plane_XY = NULL;
	plane_YZ = NULL;
	boxAll = NULL;
}
void EditFrameListener::updateObjectInfo()
{
	//infofeld anpassen
	if(selectedObject)
	{
		Ogre::String type;
		switch(selectedObject->getType())
		{
		case otChar:
			type="character";
			break;
		case otContainer:
			type="container";
			break;
		case otDoor:
			type="door";
			break;
		case otEntrance:
			type="entrance";
			break;
		case otItem:
			type="item";
			break;
		case otLight:
			type="light";
			break;
		case otMissile:
			type="missile";
			break;
		case otDynamic:
			type="special";
			break;
		case otGroup:
			{
				
				size_t cnt = static_cast<GroupObject*>(selectedObject)->getCount();
				type=str(cnt)+" objects";
			}
			break;
		case otStatic:
			type="static";
			switch(selectedObject->getWorldArt()->getType())
			{
			case WT_BILLBOARD:
				type += " (billboard)";
				break;
			case WT_DOTSCENE:
				type += " (dotScene)";
				break;
			case WT_MESH:
				type += " (mesh)";
				break;
			case WT_PLANE:
				type += " (plane)";
				break;
			case WT_PARTICLESYSTEM:
				type += " (particle sys)";
			}
			break;
		}
		Ogre::String transformations;
		switch(mTMode)
		{
		case tmMove:
			{
				Ogre::Vector3 pos = selectedObject->getPosition();
				transformations = "("+str(pos.x,3)+"/"+str(pos.y,3)+"/"+str(pos.z,3)+")";
			}
			break;
		case tmRotate:
			{
				Ogre::Quaternion ornt = selectedObject->getOrientation();
				transformations = "("+str(ornt.w,3)+"/"+str(ornt.x,3)+"/"+str(ornt.y,3)+"/"+str(ornt.z,3)+")";
			}
			break;
		case tmScale:
			{
				Ogre::Vector3 scale = selectedObject->getScale();
				transformations = "("+str(scale.x,3)+"/"+str(scale.y,3)+"/"+str(scale.z,3)+")";
			}
			break;
		}
		app->setObjectInfo(selectedObject->getID()+" "+transformations+" "+type);
	}
	else
		app->setObjectInfo("");
}
void EditFrameListener::setSelected(GameObject *obj)
{
	if(selectedObject && selectedObject != obj)
	{
		selectedObject->markSelected(false);
		if(selectedObject->getType() == otGroup)
		{
			//gruppe auflösen
			static_cast<GroupObject*>(selectedObject)->dissolveGroup();
		}
	}

	if(obj)
	{
		
		selectedObject = obj;
		obj->markSelected(true);
		showAxes(obj);
		updateObjectInfo();
		
		/*startPosition = selectedObject->getPosition();
		startOrientation = selectedObject->getOrientation();*/
		selectedObject->getPositionOrientation(startPosition,startOrientation);
		holdingOffset = intersectionPoint - startPosition;	
		startScale = selectedObject->getScale();
		unSnappedScale = startScale;
		//CEGUI::Window *wnd = CEGUI::WindowManager::getSingletonPtr()->getWindow("objectInfo");

		/*switch(mTMode)
		{
		case tmMove:
			showMoveAxes(obj);
			break;
		case tmRotate:
			showRotateAxes(obj);
			break;
		case tmScale:
			showScaleAxes(obj);
			break;
		}*/
	}
	else
	{
		
		hideAxes();
		selectedObject = NULL;
	}
}

void EditFrameListener::processTextKey(UniKey key)
{
    if(!key.isKeyboard || !key.hasModifier() || !(key.modifier == OIS::Keyboard::Ctrl))
		//alles mit Strg
		return;
	CEGUI::Window *wnd = getActiveWindow();
	if(!wnd)
		return;
	Ogre::String type = wnd->getType().c_str();
	if(!Ogre::StringUtil::endsWith(type,"editbox"))
		return;
	CEGUI::Editbox *edit = (CEGUI::Editbox*)wnd;
	size_t selStart = edit->getSelectionStartIndex();
	size_t selEnd	= edit->getSelectionEndIndex();
	size_t selLength= edit->getSelectionLength();
	CEGUI::String txt		= edit->getText();//.c_str();
	switch(key.keyboardID)
	{
	case OIS::KC_C:
		if(selLength != 0)// && !edit->isReadOnly())
		{
			//nicht so wie in VC++, dass kopieren von leerem text "" kopiert -_-
			CEGUI::String sub = txt.substr(selStart,selLength);
			//mDebugText(sub);
			app->setClipboardText(sub);
		}
		break;
	case OIS::KC_V:
		if(!edit->isReadOnly())
		{
			//Ogre::String neu = txt;
			//mDebugText(neu+"\n");
			CEGUI::String davor = txt.substr(0,selStart);
			//mDebugText(davor+"\n",true);
			CEGUI::String danach = txt.substr(selEnd);
			//mDebugText(danach+"\n",true);
			CEGUI::String cb = app->getClipboardText();
			CEGUI::String neu = davor+cb+danach;
			edit->setText(neu);
			//std::string-methoden sind extrem fürn Arsch, wenn man grad nicht nachgoogeln kann...
			//mDebugText(neu,true);
		}
		break;
	case OIS::KC_X:
		if(!edit->isReadOnly())
		{
			CEGUI::String sub = txt.substr(selStart,selLength);
			//mDebugText(sub);
			app->setClipboardText(sub);
			//Ogre::String neu = txt;
			//mDebugText(neu+"\n");
			CEGUI::String davor = txt.substr(0,selStart);
			//mDebugText(davor+"\n",true);
			CEGUI::String danach = txt.substr(selEnd);
			//mDebugText(danach+"\n",true);
			CEGUI::String neu = davor+danach;
			edit->setText(neu);
			//std::string-methoden sind extrem fürn Arsch, wenn man grad nicht nachgoogeln kann...
			//mDebugText(neu,true);
		}
		break;
	case OIS::KC_A:
		//alles markieren
		edit->setSelection(0,txt.length());

	}
	
}

void EditFrameListener::setTransformMode(transformMode mod)
{
	mTMode = mod;
	if(selectedObject)
	{
		showAxes(selectedObject);
		/*switch(mTMode)
		{
		case tmMove:
			showMoveAxes(selectedObject);
			break;
		case tmRotate:
			showRotateAxes(selectedObject);
			break;
		case tmScale:
			showScaleAxes(selectedObject);
			break;
		}*/
	}
	updateObjectInfo();
}