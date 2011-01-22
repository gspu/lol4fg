#include "EditGameLogic.h"
//#include "DotSceneHandler.h"
//#include "MainMenu.h"

#include "LogManager.h"
#include "OgreWidget.h"

#include <OgreEntity.h>
#include <OgreRenderWindow.h>
#include <OgreResourceGroupManager.h>
#include <OgreRoot.h>

#include <QKeyEvent>
#include <QMouseEvent>
#include <QtCore\QSettings>
#include "QtEditorApp.h"
#include "EditFrameListener.h"
#include <Level.h>
#include "GroupObject.h"
#include "functions.h"
#include "DynamicLines.h"
#include "h/MainWindow.h"
#include <TypeConverter.h>
#include <OgreTerrain.h>
#include <OgreTerrainGroup.h>
#include <OgreTerrainPaging.h>
#include <TerrainEditDialog.h>
#include <LevelObjectList.h>

using namespace TypeConverter;

//das ist also mein framelistener nun...
namespace QtOgre
{
	EditGameLogic::EditGameLogic(void)
		:GameLogic(),
		lastKeyEvent(NULL),
		lastMouseEvent(NULL),
		mMoveKeyboard(200),
		mRotate(0.25),
		mMove(0.25),
		mScroll(0.05),
		mTerrainDecal(NULL),
		selectionLocked(false),
		flattenHeight(-1),
		rqListener(NULL),
		deformPosition(Ogre::Vector3::ZERO),
		//decal(NULL),
		mustUpdateLightmap(false),
		unSnappedScale(Ogre::Vector3::UNIT_SCALE),
		lastMouseDownOnGui(false),
		nextSelected(NULL),
		selBoxDrawing(false),
		selectedObject(NULL),
		prevSelected(NULL),
		curTransform(tNone),
		axesNode(NULL),
		axe_X(NULL),
		axe_Y(NULL),
		axe_Z(NULL),
		plane_XZ(NULL),
		plane_XY(NULL),
		plane_YZ(NULL),
		boxAll(NULL),
		rotateMoveNode(NULL),
		lastSnapVector(Ogre::Vector3::ZERO),
		oldHitpoint(Ogre::Vector3::ZERO),
		scaleDoOnce(false),
		prevCamPos(Ogre::Vector3::ZERO),
		shutDown(false),
		linesShown(false),
		view_rotate(false),
		view_move(false),
		view_rotatemove(false),
		cursorShown(true),
		wasOverGui(false),
		mousePosChanged(false),
		rotateStartDoOnce(false),
		mTMode(tmMove)
	{
		app = (EditorApp*)StandardApplication::getSingletonPtr();
		mRealApp = app->qtApp;
		
		
		mWindow = app->getRenderWindow();//win;
	
		


		



		key_modeMove	 = UniKey(OIS::KC_M);
		key_modeRotate	 = UniKey(OIS::KC_R);
		key_modeScale	 = UniKey(OIS::KC_S);
		key_viewRotate	 = OIS::MB_Middle;//OIS::KC_SPACE;
		key_viewMove	 = OIS::KC_SPACE;//OIS::KC_LCONTROL;
		key_showLines	 = OIS::KC_F3;
		key_screenShot	 = OIS::KC_SYSRQ;
		key_delete		= OIS::KC_DELETE;
		key_showDetails	= OIS::MB_Right;
		key_objMove		= OIS::MB_Left;
		key_viewRotateMove = OIS::KC_LSHIFT;//OIS::KC_LSHIFT;
		key_group		= UniKey(OIS::MB_Left,OIS::Keyboard::Ctrl);
		key_clone		= OIS::KC_D;
		key_clone.modifier = OIS::Keyboard::Ctrl;
		key_movedown = OIS::KC_F;
		key_savemap  = OIS::KC_F12;
		key_copy	 = UniKey(OIS::KC_C	,OIS::Keyboard::Ctrl);
		key_paste	 = UniKey(OIS::KC_V,OIS::Keyboard::Ctrl);
		key_cut		 = UniKey(OIS::KC_X	,OIS::Keyboard::Ctrl);
		key_viewtop  = OIS::KC_T;


		
		

		

		////Set initial mouse clipping size
		//windowResized(mWindow);

		////showDebugOverlay(true);

		////Register as a Window listener
		//Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

	}

	//void EditGameLogic::createDecal()
	//{
	//	/*Ogre::Vector2 dsize(app->brushScale,app->brushScale);
	//	Ogre::String tex = "decal.png";
	//	decal = new DecalCursor(app->currentLevel->getSceneManager(),app->currentLevel->getTerrainMaterial(),dsize,tex);

	//	decal->show();
	//	setDecalScale(app->brushScale);*/
	//}


	void EditGameLogic::levelChangeBegin(Level *newLevel)
	{
		mTerrainDecal->setLevel(newLevel);
		mTerrainDecal->hide();
	}
	void EditGameLogic::levelChangeEnd()
	{
		Level *lvl = app->getCurrentLevel();
		app->qtApp->mainWnd->terrainEditDialog->updateTextureList(lvl);
		mCamera = lvl->getMainCam();
		selectedObject = NULL;
		//erstmal das mit dem callback für die renderqueue usw...
		Ogre::SceneManager *mgr = lvl->getSceneManager();
		if(!rqListener)
		{
		
			rqListener = new MyRenderQueueListener();
		}
		
			
		mgr->addRenderQueueListener(rqListener);
		mRealApp->mainWnd->objectListDialog->setLevel(lvl);
		
		
	}

	void EditGameLogic::setDecalScale(Ogre::ushort scale)
	{
	//	Level *lvl = app->getCurrentLevel();
	//	Ogre::AxisAlignedBox box = lvl->getTerrainExtends();
	//	size_t t_height = lvl->getTerrainInfo()->getHeight();
	//	size_t t_width = lvl->getTerrainInfo()->getWidth();
	//	Ogre::Vector3 size = box.getSize();
	//	/*Ogre::Real width = size.x/100*scale;
	//	Ogre::Real height= size.z/100*scale;*/
	//	/*Ogre::Real width = 100*float(scale)/float(t_width);
	//	Ogre::Real height= 100*float(scale)/float(t_height);*/
	//	/*Ogre::Real width = float(scale)/float(t_width)*400;
	//	Ogre::Real height= float(scale)/float(t_height)*400;*/
	//	Ogre::Vector3 tscale = lvl->getTerrainScale();
	////    mDebugText(ogre_str(tscale));
	//	Ogre::Real width = tscale.z*scale;
	//	Ogre::Real height= tscale.z*scale;
	//	decal->setSize(Ogre::Vector2(width,height));
	}

	void EditGameLogic::initialise(void)
	{
		mWindow = mRealApp->ogreRenderWindow();

		mDemoLog = mRealApp->createLog("Demo");

		mDemoLog->logMessage("A demonstration debug message", LL_DEBUG);
		mDemoLog->logMessage("A demonstration info message", LL_INFO);
		mDemoLog->logMessage("A demonstration warning message", LL_WARNING);
		mDemoLog->logMessage("A demonstration error message", LL_ERROR);
	

		
		mTime = new QTime;
		mTime->start();

		
		
		  
	}

	void EditGameLogic::createDecal()
	{
		mTerrainDecal = new TerrainDecal();
		
		selBorder = new SelectionBorder();
	}

	void EditGameLogic::update(void)
	{
		mLastFrameTime = mCurrentTime;
		mCurrentTime = mTime->elapsed();
        //we don't have a frameEnded method here


		float timeElapsedInSeconds = (mCurrentTime - mLastFrameTime) / 1000.0f;

        frameStarted(timeElapsedInSeconds);

/*

		for (Ogre::SceneManager::MovableObjectIterator moi = mSceneManager->getMovableObjectIterator("Ogre::Entity"); moi.hasMoreElements(); moi.moveNext())
		{
			Ogre::Entity *entity = static_cast<Ogre::Entity*>(moi.peekNextValue());

			Ogre::AnimationStateSet* animationStateSet = entity->getAllAnimationStates();		
			if(animationStateSet && animationStateSet->hasAnimationState("Walk"))
			{
				Ogre::AnimationState* walkAnimationState = animationStateSet->getAnimationState("Walk");
				walkAnimationState->addTime(timeElapsedInSeconds);
			}
		}

		float distance = mCameraSpeed * timeElapsedInSeconds;

		if(mKeyStates[Qt::Key_W] == KS_PRESSED)
		{
			mCamera->setPosition(mCamera->getPosition() + mCamera->getDirection() * distance);
		}
		if(mKeyStates[Qt::Key_S] == KS_PRESSED)
		{
			mCamera->setPosition(mCamera->getPosition() - mCamera->getDirection() * distance);
		}
		if(mKeyStates[Qt::Key_A] == KS_PRESSED)
		{
			mCamera->setPosition(mCamera->getPosition() - mCamera->getRight() * distance);
		}
		if(mKeyStates[Qt::Key_D] == KS_PRESSED)
		{
			mCamera->setPosition(mCamera->getPosition() + mCamera->getRight() * distance);
		}

		if(!mIsFirstFrame)
		{
			QPoint mouseDelta = mCurrentMousePos - mLastFrameMousePos;
			mCamera->yaw(Ogre::Radian(-mouseDelta.x() * timeElapsedInSeconds));
			mCamera->pitch(Ogre::Radian(-mouseDelta.y() * timeElapsedInSeconds));

			int wheelDelta = mCurrentWheelPos - mLastFrameWheelPos;
			Ogre::Radian fov = mCamera->getFOVy();
			fov += Ogre::Radian(-wheelDelta * 0.001);
			fov = (std::min)(fov, Ogre::Radian(2.0f));
			fov = (std::max)(fov, Ogre::Radian(0.5f));
			mCamera->setFOVy(fov);
		}
		mLastFrameMousePos = mCurrentMousePos;
		mLastFrameWheelPos = mCurrentWheelPos;

		mIsFirstFrame = false;*/
	}

	void EditGameLogic::shutdown(void)
	{
		if(mTerrainDecal)
			delete mTerrainDecal;
		/*mSceneManager->clearScene();
		Ogre::Root::getSingleton().destroySceneManager(mSceneManager);*/
	}

	void EditGameLogic::onKeyPress(QKeyEvent* event)
	{
		lastKeyEvent = event;
		Qt::Key key = static_cast<Qt::Key>(event->key());
		

		UniKey cur = convertQtKeyToOis(key);
	
		if((mKeyStates[Qt::Key_Shift] == KS_PRESSED) && (key != Qt::Key_Shift))
		{
			cur.modifier = OIS::Keyboard::Shift;	
		}
		if((mKeyStates[Qt::Key_Control] == KS_PRESSED) && (key != Qt::Key_Control))//mKeyboard->isModifierDown(OIS::Keyboard::Ctrl) && (arg.key != OIS::KC_LCONTROL && arg.key != OIS::KC_RCONTROL))
		{
			cur.modifier = OIS::Keyboard::Ctrl;
		}
		if((mKeyStates[Qt::Key_Alt] == KS_PRESSED) && (key != Qt::Key_Alt))//mKeyboard->isModifierDown(OIS::Keyboard::Alt) && (arg.key != OIS::KC_LMENU && arg.key != OIS::KC_RMENU))
		{
			cur.modifier = OIS::Keyboard::Alt;
		}

		//NACH den abfragen!
		mKeyStates[key] = KS_PRESSED;

		uniKeyPressed(cur);

//		if(event->key() == Qt::Key_Escape)
//		{
//			//qApp->centerWidget(mMainMenu, qApp->mMainWindow);
////			mMainMenu->exec();
//		}
	}

	void EditGameLogic::onKeyRelease(QKeyEvent* event)
	{
		lastKeyEvent = event;
		Qt::Key key = static_cast<Qt::Key>(event->key());
		UniKey cur = convertQtKeyToOis(key);
	
		if((mKeyStates[Qt::Key_Shift] == KS_PRESSED) && (key != Qt::Key_Shift))
		{
			cur.modifier = OIS::Keyboard::Shift;	
		}
		if((mKeyStates[Qt::Key_Control] == KS_PRESSED) && (key != Qt::Key_Control))//mKeyboard->isModifierDown(OIS::Keyboard::Ctrl) && (arg.key != OIS::KC_LCONTROL && arg.key != OIS::KC_RCONTROL))
		{
			cur.modifier = OIS::Keyboard::Ctrl;
		}
		if((mKeyStates[Qt::Key_Alt] == KS_PRESSED) && (key != Qt::Key_Alt))//mKeyboard->isModifierDown(OIS::Keyboard::Alt) && (arg.key != OIS::KC_LMENU && arg.key != OIS::KC_RMENU))
		{
			cur.modifier = OIS::Keyboard::Alt;
		}
		mKeyStates[event->key()] = KS_RELEASED;
		uniKeyReleased(cur);
	}

	void EditGameLogic::onMousePress(QMouseEvent* event)
	{
		lastMouseEvent = event;
		Qt::MouseButton btn = event->button();
		

		UniKey cur = convertQtMouseToOis(btn);
	
		if(mKeyStates[Qt::Key_Shift] == KS_PRESSED)
		{
			cur.modifier = OIS::Keyboard::Shift;	
		}
		if(mKeyStates[Qt::Key_Control] == KS_PRESSED)//mKeyboard->isModifierDown(OIS::Keyboard::Ctrl) && (arg.key != OIS::KC_LCONTROL && arg.key != OIS::KC_RCONTROL))
		{
			cur.modifier = OIS::Keyboard::Ctrl;
		}
		if(mKeyStates[Qt::Key_Alt] == KS_PRESSED)//mKeyboard->isModifierDown(OIS::Keyboard::Alt) && (arg.key != OIS::KC_LMENU && arg.key != OIS::KC_RMENU))
		{
			cur.modifier = OIS::Keyboard::Alt;
		}
		mKeyStates[btn] = KS_PRESSED;

		uniKeyPressed(cur);

		mCurrentMousePos = event->pos();
		mLastFrameMousePos = mCurrentMousePos;
	}

	void EditGameLogic::onMouseRelease(QMouseEvent* event)
	{
		lastMouseEvent = event;
		Qt::MouseButton btn = event->button();
		flattenHeight = -1; //reset
		

		UniKey cur = convertQtMouseToOis(btn);
	
		if(mKeyStates[Qt::Key_Shift] == KS_PRESSED)
		{
			cur.modifier = OIS::Keyboard::Shift;	
		}
		if(mKeyStates[Qt::Key_Control] == KS_PRESSED)//mKeyboard->isModifierDown(OIS::Keyboard::Ctrl) && (arg.key != OIS::KC_LCONTROL && arg.key != OIS::KC_RCONTROL))
		{
			cur.modifier = OIS::Keyboard::Ctrl;
		}
		if(mKeyStates[Qt::Key_Alt] == KS_PRESSED)//mKeyboard->isModifierDown(OIS::Keyboard::Alt) && (arg.key != OIS::KC_LMENU && arg.key != OIS::KC_RMENU))
		{
			cur.modifier = OIS::Keyboard::Alt;
		}
		mKeyStates[event->button()] = KS_RELEASED;
		uniKeyReleased(cur);
	}

	void EditGameLogic::onMouseDoubleClick(QMouseEvent* event)
	{
		lastMouseEvent = event;
	}

	void EditGameLogic::onMouseMove(QMouseEvent* event)
	{
		lastMouseEvent = event;
		QPoint relMousePos = mCurrentMousePos-event->pos();
		mCurrentMousePos = event->pos();

		//if(app->dragItemID != "")
		//	return true;
		Level *lvl = app->currentLevel;
		//if(app->editingMode == EditorApp::emTerrain && lvl->getTerrainManager())
		//{
		//	Ogre::Ray mouseRay = getMouseRay();//lvl->getMainCam()->getCameraToViewportRay(mouseX,mouseY);	


		//	
		//	// since ETM is no longer a scene manager, we don't use a ray scene query,
		//	// but instead query the terrain info directly
		//	std::pair<bool, Ogre::Vector3> result = lvl->getTerrainInfo()->rayIntersects(mouseRay);
		//	
		//	if(result.first)
		//	{
		//		deformPosition = result.second;
		//
		//	}
		//}

		//do not do stuff if there is no lvl
		if(!app->getCurrentLevel())
			return;

		Ogre::SceneNode *mCamNode = app->getCurrentLevel()->getCamNode();

		

		if(view_rotate)
		{

			
			//Ogre::Real muh = 75;
			mCamNode->yaw( Ogre::Degree( relMousePos.x()*mRotate ) );
			mCamNode->getChild( 0 )->pitch( Ogre::Degree( relMousePos.y()*mRotate ) );



			//CEGUI::MouseCursor::getSingleton().setPosition(CEGUI::Point(e->getX(),e->getY()));

		}
		if(view_move)
		{

			Ogre::Vector3 transVector = Ogre::Vector3(0,0,0);

			transVector.x = relMousePos.x();
			transVector.y = -relMousePos.y();

			mCamNode->translate( mCamNode->getChild( 0 )->_getDerivedOrientation() * transVector * mMove);
			


		}
		if(view_rotatemove)
		{
			Ogre::Degree yawValue = Ogre::Degree( relMousePos.x()*mRotate );
			Ogre::Degree pitchValue = Ogre::Degree( relMousePos.y()*mRotate );
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

			
			}
		}


		//ab dieser Stelle nur noch objekteditingrelevantes
		if(app->editingMode == EditorApp::emTerrain)
			return;

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
				

			}
		}

		if(isKeyDown(key_objMove) && curTransform == tNone && !wasOverGui)
		{
			drawSelectionBox();
		}
	}

	void EditGameLogic::onWheel(QWheelEvent* event)
	{
//		lastMouseEvent = event;
		mCurrentWheelPos += event->delta();
		Ogre::SceneNode *mCamNode = app->getCurrentLevel()->getCamNode();
		mCamNode->translate( mCamNode->getChild( 0 )->_getDerivedOrientation() * Ogre::Vector3(0,0,-1) * event->delta() * mScroll);// * lastFrameTime );
		//if(arg.state.Z.rel)
		//{
		//	mCamNode->translate( mCamNode->getChild( 0 )->_getDerivedOrientation() * Ogre::Vector3(0,0,-1) * arg.state.Z.rel * mScroll);// * lastFrameTime );
		//	
		//}
	}

	Log* EditGameLogic::demoLog(void)
	{
		return mDemoLog;
	}

//	void EditGameLogic::loadScene(QString filename)
//	{
//        
//        
//
//        mCamera = mSceneManager->createCamera("mainCam");
//		//The QtOgre DotScene loading code will clear the existing scene except for cameras, as these
//		//could be used by existing viewports. Therefore we clear and viewports and cameras before
//		//calling the loading code.
//		//mApplication->ogreRenderWindow()->removeAllViewports();
//		//mSceneManager->destroyAllCameras();
//
//		//Now load the scene.
////		DotSceneHandler handler(mSceneManager);
//		//QXmlSimpleReader reader;
//		//reader.setContentHandler(&handler);
//		//reader.setErrorHandler(&handler);
//
//		//QFile file(filename);
//		//file.open(QFile::ReadOnly | QFile::Text);
//		//QXmlInputSource xmlInputSource(&file);
//		//reader.parse(xmlInputSource);
//
//		//Now create a viewport, using the first camera in the scene.
//		//mCamera = mSceneManager-> getCameraIterator().peekNextValue();
//
//		//mCamera->setPosition(0, 0, 20);
//		//mCamera->lookAt(0, 0, 0);
//		mCamera->setNearClipDistance(1.0);
//		mCamera->setFarClipDistance(1000.0);
//		//mCamera->setFOVy(Ogre::Radian(1.0f));
//
//        Ogre::Entity *ent = mSceneManager->createEntity("test","ogrehead.mesh");
//        Ogre::SceneNode *nod = mSceneManager->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(0,0,-50));
//        nod->attachObject(ent);
//
//        Ogre::Viewport* viewport = mRealApp->ogreRenderWindow()->addViewport(mCamera);
//		viewport->setBackgroundColour(Ogre::ColourValue::Black);
//	}
    
    bool EditGameLogic::frameStarted(Ogre::Real time){
        //do not do stuff if there is no lvl
	    if(!app->getCurrentLevel())
		    return true;

        
        
	    lastFrameTime = time;
	    Ogre::Vector3 transVector = Ogre::Vector3::ZERO;
        
        //abfrage mit mKeyStates[Qt::Key_W] == KS_PRESSED
        //Qt::key
        //mKeyStates

		//Ogre::Real moveFactor = 500;
		
		if ( mKeyStates[ Qt::Key_Up ] == KS_PRESSED)// || m_InputReader->isKeyDown( KC_W ) )
		    transVector.z -= mMoveKeyboard;
		if ( mKeyStates[ Qt::Key_Down ] == KS_PRESSED)// || m_InputReader->isKeyDown( KC_S ) )
		    transVector.z += mMoveKeyboard;
	    if ( mKeyStates[ Qt::Key_Left ] == KS_PRESSED)// || m_InputReader->isKeyDown( KC_A ) )
		    transVector.x -= mMoveKeyboard;
	    if ( mKeyStates[ Qt::Key_Right ] == KS_PRESSED)// || m_InputReader->isKeyDown( KC_D ) )
		    transVector.x += mMoveKeyboard;
	    if ( mKeyStates[ Qt::Key_PageUp ] == KS_PRESSED)// || m_InputReader->isKeyDown( KC_Q ) )
                transVector.y += mMoveKeyboard;
	    if ( mKeyStates[ Qt::Key_PageDown ] == KS_PRESSED)// || m_InputReader->isKeyDown( KC_E ) )
		    transVector.y -= mMoveKeyboard;

	
	    Ogre::SceneNode *mCamNode = app->getCurrentLevel()->getSceneManager()->getSceneNode("CamNode");

		if(time < 1)
		{
			mCamNode->translate( mCamNode->getChild( 0 )->_getDerivedOrientation() * transVector * time );
		}
		else
		{
			mLog("zu viel Zeit, diesmal kein Translate");
		}

    	
	    if(axesNode)
	    {
		    updateAxes();
	    }

	    if(!app->paused)
	    {
			app->getCurrentLevel()->update(time);		  
	    }
		/*Ogre::Camera *cam = app->getCurrentLevel()->getMainCam();
		Ogre::Viewport *vp = cam->getViewport();
		
		mRealApp->mainWnd->setStatusText("Left: "+TypeConverter::ogre_str(vp->getLeft())+
			"Top: "+TypeConverter::ogre_str(vp->getTop())+
			"Width: "+TypeConverter::ogre_str(vp->getWidth())+
			"Height: "+TypeConverter::ogre_str(vp->getHeight())
			);*/
	    //das immer
	    app->getCurrentLevel()->updateDelete();

	    //END OgreNewt Stuff-----------------------------

	    
	    app->cleanUp();

	   
	    processTerrainEditing(time);
        return true;
    }
        
    void EditGameLogic::processTerrainEditing(Ogre::Real time)
    {
		if(app->getEditorMode() != EditorApp::emTerrain || !lastMouseEvent)
			return;
		/**
		
		Qt::MouseButton qtKey = convertOisMouseToQt(key.mouseID);
			return ();
		*/
			//mKeyStates[Qt::LeftButton] == KS_PRESSED
		using namespace Ogre;
		Level *curLevel = app->getCurrentLevel();

		
		Ogre::TerrainGroup *mTerrainGroup = curLevel->getTerrainGroup();
		///////////////////this part finds out where on the terrain the mouse is

		//get camera ray 
		Ray ray; 
		//ray = mCamera->getCameraToViewportRay(0.5, 0.5);
		ray = getMouseRay();//mTrayMgr->getCursorRay(mCamera);

		size_t brushWidth = mRealApp->mainWnd->terrainEditDialog->getSelectedBrush().getWidth();
		TerrainGroup::RayResult rayResult = mTerrainGroup->rayIntersects(ray);

		if (rayResult.hit)
		{
			mTerrainDecal->update(rayResult.position.x,rayResult.position.z,brushWidth);
		}
		

		if(app->editingMode==EditorApp::emTerrain && curLevel->hasTerrain()
			&& (mKeyStates[Qt::LeftButton] == KS_PRESSED || mKeyStates[Qt::RightButton] == KS_PRESSED))
		{
			Real mBrushSizeTerrainSpace = app->getCurrentLevel()->lengthWorldToTerrain(mRealApp->mainWnd->terrainEditDialog->getBrushSize());// = 0.2f;

			
			
			if (rayResult.hit)
			{				
				//mEditMarker->setVisible(true);
				//mEditNode->setPosition(rayResult.position); //<- so, here I can position  an edit marker. good.
				Ogre::Real worldSize = mTerrainGroup->getTerrainWorldSize();

				//set stuff for flatten/soften
				if(app->getTerrainEditMode() == EditorApp::teSoften)
				{
					flattenHeight = mTerrainGroup->getHeightAtWorldPosition(rayResult.position);
					//flattenHeight = terrain->getHeightAtPoint(Ogre::Math::Floor(temp.getWidth()/2),Ogre::Math::Floor(temp.getHeight()/2));
				}
				else if(app->getTerrainEditMode() == EditorApp::teFlatten)
				{
					if(flattenHeight == -1)
					{
						flattenHeight = mTerrainGroup->getHeightAtWorldPosition(rayResult.position);
					}
				}
				
				// figure out which terrains this affects
				TerrainGroup::TerrainList terrainList;
				Real brushSizeWorldSpace = worldSize * mBrushSizeTerrainSpace;
				Sphere sphere(rayResult.position, brushSizeWorldSpace);
				mTerrainGroup->sphereIntersects(sphere, &terrainList);

				for (TerrainGroup::TerrainList::iterator ti = terrainList.begin();
					ti != terrainList.end(); ++ti)
				{
					doTerrainModify(*ti, rayResult.position, time);
				}
			}
			mTerrainGroup->update();



			///////////////////////////////////


			//// we need point coords
			
			
			//		Real terrainSize = (terrain->getSize() - 1);
			//		long startx = (tsPos.x - mBrushSizeTerrainSpace) * terrainSize;
			//		long starty = (tsPos.y - mBrushSizeTerrainSpace) * terrainSize;
			//		long endx = (tsPos.x + mBrushSizeTerrainSpace) * terrainSize;
			//		long endy= (tsPos.y + mBrushSizeTerrainSpace) * terrainSize;
			//		startx = std::max(startx, 0L);
			//		starty = std::max(starty, 0L);
			//		endx = std::min(endx, (long)terrainSize);
			//		endy = std::min(endy, (long)terrainSize);
			//		for (long y = starty; y <= endy; ++y)
			//		{
			//			for (long x = startx; x <= endx; ++x)
			//			{
			//				Real tsXdist = (x / terrainSize) - tsPos.x;
			//				Real tsYdist = (y / terrainSize)  - tsPos.y;

			//				Real weight = std::min((Real)1.0, 
			//					Math::Sqrt(tsYdist * tsYdist + tsXdist * tsXdist) / Real(0.5 * mBrushSizeTerrainSpace));
			//				weight = 1.0 - (weight * weight);

			//				float addedHeight = weight * 250.0 * timeElapsed;
			//				float newheight;
			//				if (mKeyboard->isKeyDown(OIS::KC_EQUALS))
			//					newheight = terrain->getHeightAtPoint(x, y) + addedHeight;
			//				else
			//					newheight = terrain->getHeightAtPoint(x, y) - addedHeight;
			//				terrain->setHeightAtPoint(x, y, newheight);

			//			}
			//		}
			//		if (mHeightUpdateCountDown == 0)
			//			mHeightUpdateCountDown = mHeightUpdateRate;
		}
		
   //     if(app->editingMode==EditorApp::emTerrain  && app->currentLevel->getTerrainManager())
	  //  {
		 //   // choose a brush intensity, this determines
		 //   // how extreme our brush works on the terrain
		 //   bool lDown = false;
		 //   bool rDown = false;
			//if( mKeyStates[Qt::LeftButton] )//mMouse->getMouseState().buttonDown(MB_Left))
		 //   {
			//    lDown = true;
		 //   }
		 //   if(mKeyStates[Qt::RightButton])//mMouse->getMouseState().buttonDown(MB_Right))
		 //   {
			//    rDown = true;
		 //   }
   //         

   //         Level *lvl = app->getCurrentLevel();
   //         //anpassen


   //         const ET::TerrainInfo *info = lvl->getTerrainInfo();
   // 		
   //         ET::Brush cur = app->getCurrentBrush();
   //         Ogre::Vector3 pos = deformPosition;// + lvl->getTerrainInfo()->getScaling();
		 //   // int posToVertexX(float x) const { return (int) ((x-mOffset.x)/mScale.x); }
		 //   Ogre::Vector3 vertexCoords;
		 //   vertexCoords.x = round( (deformPosition.x-info->getOffset().x)/info->getScaling().x );
		 //   vertexCoords.z = round( (deformPosition.z-info->getOffset().z)/info->getScaling().z );
   // 		
		 //   int x = (int)vertexCoords.x;//info->posToVertexX(deformPosition.x);
		 //   int z = (int)vertexCoords.z;//info->posToVertexZ(deformPosition.z);
   // 		
   //         deformPosition.x = info->vertexToPosX(x);
   //         deformPosition.z = info->vertexToPosZ(z);
   //         Ogre::Vector3 decalPosition = deformPosition;
   //         
   //         
   // 		    
   //         if(app->tEditMode != EditorApp::tePaint && app->tEditMode != EditorApp::teHoles)
	  //      {
   //             //jetzt erstmal decal setzen
   //             if(cur.getHeight() % 2 == 0)
   //             {
   //                 decalPosition -= lvl->getTerrainScale()/2;
   //                 //decalPosition.x -= 0.5;
   //                 //decalPosition.z -= 0.5;
   //             }
   //             //mDebugText("x="+ogre_str(decalPosition.x)+"\nz="+ogre_str(decalPosition.z));
   //     		
   //             setDecalPosition(decalPosition);//-lvl->getTerrainScale());
   //             if(lDown || rDown)
   //             {
   //                 app->terrainDeformed = true;				
   //                 mustUpdateLightmap = true;
   //                 if(app->tEditMode == EditorApp::teFlatten || app->tEditMode == EditorApp::teSoften)
   //                 {
			//	        //hier bügeln
			//	        ET::TerrainManager *tMgr = app->currentLevel->getTerrainManager();
   // 					
			//	        ET::Brush temp = ET::Brush(cur);
			//	        tMgr->getHeights(x,z,temp);
   //                     //jetzt einen Übergang suchen

			//	        std::vector<float> newBrush;//(temp.getHeight()*temp.getWidth(),0.5f)
   //                     
			//	        //Ogre::Real flattenTo = 0.5;
   //                     if(app->tEditMode == EditorApp::teSoften)
   //                     {
   //                         flattenHeight = temp.at(Ogre::Math::Floor(temp.getWidth()/2),Ogre::Math::Floor(temp.getHeight()/2));
   //                     }
   //                     else
   //                     {
   //                         if(flattenHeight == -1)
   //                         {
   //                             flattenHeight = temp.at(Ogre::Math::Floor(temp.getWidth()/2),Ogre::Math::Floor(temp.getHeight()/2));
   //                         }
   //                     }
   //                     for(size_t h=0;h<temp.getHeight();h++)
			//		        for(size_t w=0;w<temp.getWidth();w++)                        
   //                         {
   //                             //der wert des origbrushes entscheidet über den faktor
			//			        //1=voll, 0=gar nicht
			//			        //abweichung vom Soll:
   // 							
			//			        Ogre::Real curVal = temp.at(w,h);
   //                             float brushIntensity = time * 4 * (app->terrainIntensity/50) * (lDown? 1 : -1);
			//			        Ogre::Real diff = curVal-flattenHeight;
   //                             Ogre::Real newVal = curVal;
   //                             if(diff != 0)
   //                             {
   //                                 Ogre::Real set = curVal - 0.05*Ogre::Math::Sign(diff)*cur.at(w,h)*(brushIntensity);
   //                                 /*if(h==temp.getHeight()/2 && w==temp.getWidth()/2)
   //                                     mDebugText("set="+ogre_str(set)+"\ndiff="+ogre_str(diff));*/
   //                                 if(Ogre::Math::Abs(diff) < 0.01)
   //                                     newVal = curVal - diff*cur.at(w,h)*(brushIntensity);//flattenHeight;
   //                                 else
   //                                     newVal = set;                                   
   //                             }
   //                             //Ogre::Real newVal = curVal - (curVal-flattenHeight)*cur.at(w,h)*(brushIntensity);
			//			        //newVal += 0.5;
			//			        newBrush.push_back(newVal);
   //                         }
			//	        temp  = ET::Brush(newBrush,temp.getWidth(),temp.getHeight());
   //                     app->currentLevel->getTerrainManager()->setHeights(x,z,temp);

   //                 }
   //                 else
   //                 {
   // 				
		 //               //MB_Left = 0, MB_Right, MB_Middle,
   //                     float brushIntensity = time * 0.4 * (app->terrainIntensity/50) * (lDown? 1 : -1);
		 //               // translate our cursor position to vertex indexes

   //     				
		 //               // now tell the ETM to deform the terrain
		 //               app->currentLevel->getTerrainManager()->deform(x, z, app->getCurrentBrush(), brushIntensity);
		 //               //lvl->updateTerrainLightmap();
   //                     
   //                 }
   //             }
	  //      }
	  //      else
	  //      {
   //             //decal zum malen anders positionieren:
			//    //wenn wir mit löchern malen
   // 			
   //             setDecalPosition(decalPosition);
   //             if(lDown || rDown)
   //             {
			//	    if(app->tEditMode == EditorApp::teHoles)
			//	    {
			//		    mustUpdateLightmap = true;
			//		    app->terrainDeformed = true;
			//	    }
		 //           // need to set our brush intensity larger for painting.
		 //           // for painting, all texture channels are only 1 byte
		 //           // large, so with a small intensity you won't get any
		 //           // effect at all.
		 //           float brushIntensity = time * (app->terrainIntensity/50) * 7.0 * (lDown? 1 : -1);
		 //           // retrieve edit points
		 //           /*Ogre::Vector3 paintPos = mPointer->getPosition();
		 //           int x = mTerrainInfo->posToVertexX(paintPos.x);
		 //           int z = mTerrainInfo->posToVertexZ(paintPos.z);*/
		 //           // now use the splatting manager to update the coverage maps
   // 				
		 //           app->currentLevel->getSplattingManager()->paint(app->getCurrentTexture(), x, z, app->getCurrentBrush(), brushIntensity);
   //             }
	  //      }
   //     
		 //   //}
   //     }
    }

	void EditGameLogic::setCoverageAboveLayer(Ogre::Terrain *terrain, Ogre::uint8 layer, size_t x, size_t y, Ogre::Real val)
	{
		
	}

	Ogre::Real EditGameLogic::getCoverageAboveLayer(Ogre::Terrain *terrain, Ogre::uint8 layer, size_t x, size_t y)
	{

		Ogre::Real result = 1;
		for(Ogre::uint8 i = layer+1;i<terrain->getLayerCount();i++)
		{
			Ogre::TerrainLayerBlendMap *cur = terrain->getLayerBlendMap(i);			
			result *= (1-cur->getBlendValue(x,y));
		}
		return result;
	}

	Ogre::Real EditGameLogic::getCombinedBlendValue(Ogre::Terrain *terrain, Ogre::uint8 layer, size_t x, size_t y)
	{
		return terrain->getLayerBlendMap(layer)->getBlendValue(x,y) * getCoverageAboveLayer(terrain,layer,x,y);
	}

	void EditGameLogic::setCombinedBlendValue(Ogre::Terrain *terrain, Ogre::uint8 layer, size_t x, size_t y, Ogre::Real val)
	{
		//now try to do this
		Ogre::TerrainLayerBlendMap *curLayer = terrain->getLayerBlendMap(layer);
		Ogre::Real coverage = getCoverageAboveLayer(terrain,layer,x,y);
		Ogre::Real blendval = curLayer->getBlendValue(x,y);
		if(coverage == 1)//either they all are zero, or we are on top
		{
			curLayer->setBlendValue(x,y,val);
		}
		else
		{
			//I should try to distribute val onto the layers above this first, starting with the top one
			Ogre::Real restVal = val;
			for(Ogre::uint8 i = terrain->getLayerCount()-1;i>layer;i--)
			{
				Ogre::TerrainLayerBlendMap *cur = terrain->getLayerBlendMap(i);			
				Ogre::Real curVal = cur->getBlendValue(x,y);
				//now try to set curVal to restVal, if it works, all other curVals are 1 (means, blendvalues are zero)
				if(curVal < restVal)
				{
					restVal = restVal / (1-curVal);

				}
			}
		}
 
	}

	void EditGameLogic::doTerrainModify(Ogre::Terrain* terrain, const Ogre::Vector3& centrepos, Ogre::Real timeElapsed)
	{
		using namespace Ogre;

		

		Ogre::Image img = mRealApp->mainWnd->terrainEditDialog->getSelectedBrush();

		
		Real mBrushSizeTerrainSpace = 0.02f;//seems to be the brush radius, in terrain coords or so
		//0,02 ^= ca. 188
		//eventuell worldSize * diesen Wert
		mBrushSizeTerrainSpace = app->getCurrentLevel()->lengthWorldToTerrain(mRealApp->mainWnd->terrainEditDialog->getBrushSize());


		Ogre::Vector3 tsPos;
		terrain->getTerrainPosition(centrepos, &tsPos);

		
		// we need point coords
		Real terrainSize = (terrain->getSize() - 1);
		Real intensity = mRealApp->mainWnd->terrainEditDialog->getIntensityFactor();
	
	
		long xSize = img.getWidth()/2; //half the size of the brush
		mRealApp->mainWnd->setStatusText("xSize = "+TypeConverter::ogre_str(xSize));

		long centerOfBrushX = (tsPos.x) * terrainSize;//center point of the brush AKA the click point in terrain coords
		long centerOfBrushY = (tsPos.y) * terrainSize;

		//start and end values for looping through the brush area
		long startx = centerOfBrushX - xSize;
		long starty = centerOfBrushY - xSize;
		long endx	= centerOfBrushX + xSize;
		long endy	= centerOfBrushY + xSize;		

		



		switch(app->getTerrainEditMode())
		{
		case EditorApp::teDeform:					
		case EditorApp::teSoften:
		case EditorApp::teFlatten:
			//here is the stuff for everything deforming-related

			//I think this is for when we are outside of the current terrain part
			startx = std::max(startx, 0L);
			starty = std::max(starty, 0L);
			endx = std::min(endx, (long)terrainSize);
			endy = std::min(endy, (long)terrainSize);

			//getTerrainEditMode
			for (long y = starty; y <= endy; ++y)
			{
				for (long x = startx; x <= endx; ++x)
				{
					Real tsXdist = (x / terrainSize) - tsPos.x;
					Real tsYdist = (y / terrainSize)  - tsPos.y;

					//now try to get color from png
					Ogre::ColourValue color = img.getColourAt(x-startx,y-starty,0);
					//hm, ignore it if they are exactly at the corner?
					Ogre::Real medValue = ((color.r + color.g + color.b)/3);

					switch(app->getTerrainEditMode())
					{
					case EditorApp::teDeform:
						{				
							Real weight = medValue;

							float addedHeight = weight * 250.0 * timeElapsed * intensity;
							float newheight;
							if(mKeyStates[Qt::LeftButton] == KS_PRESSED)
								newheight = terrain->getHeightAtPoint(x, y) + addedHeight;
							else if(mKeyStates[Qt::RightButton] == KS_PRESSED)
								newheight = terrain->getHeightAtPoint(x, y) - addedHeight;
							terrain->setHeightAtPoint(x, y, newheight);
						}
						break;
					case EditorApp::teSoften:
					case EditorApp::teFlatten:
						{
							Real curHeight = terrain->getHeightAtPoint(x, y);
							Real diff = curHeight-flattenHeight;
							Real newVal = curHeight;
							if(diff != 0)
							{
							   Ogre::Real set = curHeight - 0.5*Ogre::Math::Sign(diff)*medValue*intensity;//*(brushIntensity);
							   
							   if(Ogre::Math::Abs(diff) < 0.01)
								   newVal = curHeight - diff*medValue;//*(brushIntensity);//flattenHeight;
							   else
								   newVal = set;                                   
							}
							terrain->setHeightAtPoint(x, y, newVal);
						}
						break;					
					}//case(...)

				}//for(x
			}//for(y
			break;
		case EditorApp::tePaint:
			int texIndex = mRealApp->mainWnd->terrainEditDialog->getCurrentTexture();
			//and here painting and maybe hole-making, too

			TerrainLayerBlendMap* layer = NULL;

			if(texIndex > 0)
			{
				//paint
				layer = terrain->getLayerBlendMap(texIndex);
			}
			// we need image coords
			Real imgSize = terrain->getLayerBlendMapSize();


			xSize *= (16);

			/*
			long xSize = img.getWidth()/2; <- i keep this
			*/
			centerOfBrushX = (tsPos.x) * imgSize;
			centerOfBrushY = (tsPos.y) * imgSize;

			//start and end values for looping through the brush area
			startx	= centerOfBrushX - xSize;
			starty	= centerOfBrushY - xSize;
			endx	= centerOfBrushX + xSize;
			endy	= centerOfBrushY + xSize;	
		


			startx = std::max(startx, 0L);
			starty = std::max(starty, 0L);
			endx = std::min(endx, (long)imgSize);
			endy = std::min(endy, (long)imgSize);
			for (long y = starty; y <= endy; ++y)
			{
				for (long x = startx; x <= endx; ++x)
				{
					Real tsXdist = (x / imgSize) - tsPos.x;
					Real tsYdist = (y / imgSize)  - tsPos.y;

					//now try to get color from png
					Ogre::ColourValue color = img.getColourAt((x-startx)/16,(y-starty)/16,0);
					//hm, ignore it if they are exactly at the corner?
					Ogre::Real medValue = ((color.r + color.g + color.b)/3);					

					float paint = medValue * timeElapsed * 5 *intensity;
					if(mKeyStates[Qt::RightButton] == KS_PRESSED)
						paint *= -1;//assume it is the left key otherwise
					size_t imgY = imgSize - y;
					float val;


					if(texIndex > 0)
					{
						val = layer->getBlendValue(x, imgY) + paint;
						

						val = Math::Clamp(val, 0.0f, 1.0f);
					
						layer->setBlendValue(x, imgY, val);
					}
					else
					{
						val = 1+paint;
					}
					/*Ogre::TerrainLayerBlendMap *cur = terrain->getLayerBlendMap(texIndex);
					cur->setBlendValue(x,imgY,1-val);*/
					//app->getCurrentLevel()->setCombinedBlendValue(terrain,texIndex,x,imgY,val);
					////and now, clean everything above
					//for(int i=texIndex+1;i<terrain->getLayerCount();i++)
					//{
					//	Ogre::TerrainLayerBlendMap *cur = terrain->getLayerBlendMap(i);
					//	val = 
					//	cur->setBlendValue(x,imgY,1-val);
					//}

				}
			}
			int layerUpdateStart = std::max(texIndex,1);
			for(int i = layerUpdateStart;i < terrain->getLayerCount();i++)
			{
				Ogre::TerrainLayerBlendMap *layer = terrain->getLayerBlendMap(i);
				layer->dirty();
				layer->update();
			}
			//} 
			break;

		}
		

		

		/*
		
		*/
		/*for(Ogre::uint8 i = 1;i<terrain->getLayerCount();i++)
		{
			Ogre::TerrainLayerBlendMap* bm = terrain->getLayerBlendMap(i);
			bm->dirty();
			bm->update();
		}*/
		/*if (mHeightUpdateCountDown == 0)
			mHeightUpdateCountDown = mHeightUpdateRate;*/
	
		
	}

    void EditGameLogic::updateAxes()
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
	    //mDebugText(ogre_str(scalar));
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

        //das swicht die Achsenpfeile
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
    	

        //und das findet nun die quadrantnummer raus, um danach die planes und evtl. das dreieck richtig zu setzen
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
	void EditGameLogic::setSelectionLocked(bool set)
	{
		selectionLocked = set;
	}
    
    void EditGameLogic::hideAxes()
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

	void EditGameLogic::setSelected(GameObject *obj)
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

	
	void EditGameLogic::setTransformModeDirect(transformMode mod)
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

	void EditGameLogic::setTransformMode(transformMode mod)
	{
		switch(mod)
		{
		case tmMove:
			mRealApp->mainWnd->ui->actionMove_Mode->setChecked(true);
			break;
		case tmRotate:
			mRealApp->mainWnd->ui->actionRotate_Mode->setChecked(true);
			break;
		case tmScale:
			mRealApp->mainWnd->ui->actionScale_Mode->setChecked(true);
			break;
		}
		setTransformModeDirect(mod);
	}

	void EditGameLogic::showObjectFromTop(GameObject *obj)
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

	void EditGameLogic::updateObjectInfo()
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
					type=ogre_str(cnt)+" objects";
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
					transformations = "("+ogre_str(pos.x,3)+"/"+ogre_str(pos.y,3)+"/"+ogre_str(pos.z,3)+")";
				}
				break;
			case tmRotate:
				{
					Ogre::Quaternion ornt = selectedObject->getOrientation();
					transformations = "("+ogre_str(ornt.w,3)+"/"+ogre_str(ornt.x,3)+"/"+ogre_str(ornt.y,3)+"/"+ogre_str(ornt.z,3)+")";
				}
				break;
			case tmScale:
				{
					Ogre::Vector3 scale = selectedObject->getScale();
					transformations = "("+ogre_str(scale.x,3)+"/"+ogre_str(scale.y,3)+"/"+ogre_str(scale.z,3)+")";
				}
				break;
			}
			app->setObjectInfo(selectedObject->getID()+" "+transformations+" "+type);
		}
		else
			app->setObjectInfo("");
	}

	
	void EditGameLogic::showAxes(GameObject *obj)
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

	bool EditGameLogic::isKeyDown(UniKey key)
	{
		if(key.hasModifier())
		{
			Qt::Key mod = convertOisKeyToQt(key.getModifierAsKeyCode());
			if(mKeyStates[mod] != KS_PRESSED)
				return false;//wenn der modifier da sein soll, aber nicht gedrückt ist
		}
		if(key.isKeyboard)
		{
			Qt::Key qtKey = convertOisKeyToQt(key.keyboardID);
			return (mKeyStates[qtKey] == KS_PRESSED);
		}
		else
		{
			//maus
			Qt::MouseButton qtKey = convertOisMouseToQt(key.mouseID);
			return (mKeyStates[qtKey] == KS_PRESSED);
		}
		/*OIS::KeyCode kc = convertQtKeyToOis(qtKey);
		return */
		/*
		OIS::KeyCode convertQtKeyToOis(Qt::Key key);
		Qt::Key convertOisKeyToQt(OIS::KeyCode key);
		*/
	}


	void EditGameLogic::uniKeyPressed(UniKey key)
	{
		//processTextKey(key);
		if(!app->getCurrentLevel())
			return;
		/*CEGUI::Window *wnd = getActiveWindow();
		wnd->getty
		CEGUI::System::getSingletonPtr()->getGUISheet()->activate
		if(wnd)
		{
			mDebugText(wnd->getName().c_str());
		}*/
		
		//if(isMouseOverGui())
		//	return;
		//OLD CEGUI STUFFF
		//selBoxStart = CEGUI::MouseCursor::getSingletonPtr()->getPosition();

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

		if(key == key_objMove || key == key_group)
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

	void EditGameLogic::uniKeyReleased(UniKey key)
	{
		if(key_screenShot == key)// == OIS::KC_SYSRQ)
		{
			//screenshot
			mWindow->writeContentsToTimestampedFile("../screenshots/editor_",".png");
			app->setDebugText("screenshot erstellt");
		}
		if(!app->getCurrentLevel())
			return;
		

		if(key_savemap == key)// == OIS::KC_SYSRQ)
		{
			//screenshot
			app->saveMapImageInit();
			app->setDebugText("ganzlevel-screenshot erstellt");
		}
		

		if(key_showLines == key)// == OIS::KC_F3)
		{
			linesShown = !linesShown;
			showDebugLines(linesShown);
			/*if(linesShown)
			{
				OgreNewt::Debugger::getSingleton().hideLines();
			}
			else
			{
				OgreNewt::Debugger::getSingleton().showLines(app->getCurrentLevel()->getWorld());
			}*/
			
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
				mRealApp->mainWnd->showObjectDetails(selectedObject);
//				app->showDetailsDialog(selectedObject);
			}
		}


		
	/*	CEGUI::WindowManager *wmgr =
		EditFrameListener::manipulationMode m = mFrameListener->mMode;
		CEGUI::String ogre_str;
		switch(m)
		{
		case EditFrameListener::mmMove:
			mFrameListener->mMode = EditFrameListener::mmRotate;
			ogre_str = "rotate";
			break;
		case EditFrameListener::mmRotate:
			mFrameListener->mMode = EditFrameListener::mmScale;
			ogre_str = "scale";
			break;
		case EditFrameListener::mmScale:
			mFrameListener->mMode = EditFrameListener::mmMove;
			ogre_str = "move";
			break;
		}

		wmgr->getWindow("modType")->setText(ogre_str);
		return true;*/
		if(key_modeScale == key)// ==OIS::KC_S)
		{
			setTransformMode(tmScale);
			//mMode = mmScale;
			//CEGUI::WindowManager::getSingletonPtr()->getWindow("modType")->setText("scale");
		}
		if(key_modeRotate == key)//==OIS::KC_R)
		{
			setTransformMode(tmRotate);
			//mMode = mmRotate;
			//CEGUI::WindowManager::getSingletonPtr()->getWindow("modType")->setText("rotate");
		}
		if(key_modeMove == key)//==OIS::KC_M)
		{
			//mMode = mmMove;
			setTransformMode(tmMove);
			//CEGUI::WindowManager::getSingletonPtr()->getWindow("modType")->setText("move");
		}
		if(key_movedown == key)//==OIS::KC_M)
		{
			//fallenlassen
			objectMoveDown();
		}
		if (key == key_objMove || key == key_group)// id == OIS::MB_Left )
		{
			//CEGUI::MouseCursor::getSingleton().setImage((CEGUI::utf8*)STYLE_EDITOR,(CEGUI::utf8*)"MouseArrow");
			if(app->dragItemID != "")
			{
				//dropItem
				//HIER DROPPEN
				
				dropItem();
				return;
	//			app->dragItemType=0;

			}
		/*	if(!lastMouseDownOnGui)
			{*/

				if(!selBoxDrawing)
				{
					//doObjectRaycast();
					updateSelection(key == key_group);
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
			//}






		}

	}

	    void EditGameLogic::deleteSelected()
    {
        hideAxes();
        if(selectedObject)
        {
            app->updateCurObjectsList(selectedObject,false);
            delete selectedObject;
            selectedObject = NULL;
        }
    }


    void EditGameLogic::pasteObjects()
    {
        TiXmlDocument doc;
        Ogre::String ogre_str = app->getClipboardText();
        if(ogre_str != "")
        {
            doc.Parse(ogre_str.c_str());
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


    void EditGameLogic::copyObjects()
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

	Ogre::Ray EditGameLogic::getMouseRay()
	{
		QCursor cur = mRealApp->getOgreWidget()->cursor();
		QPoint pos = lastMouseEvent->pos();

        Ogre::Real mouseX = float(pos.x())/float(mWindow->getWidth());
        Ogre::Real mouseY = float(pos.y())/float(mWindow->getHeight());
		//mRealApp->logMessage("pos = ("+ogre_str(pos.x())+";"+ogre_str(pos.y())+")");
		//mRealApp->logMessage("wnd = ("+ogre_str(mWindow->getWidth())+";"+ogre_str(mWindow->getHeight())+")");


        return mCamera->getCameraToViewportRay( mouseX, mouseY );
	}

    GameObject* EditGameLogic::doObjectRaycast()
    {
        if(selectionLocked)
            return selectedObject;



        holdingOffset = Ogre::Vector3(0,0,0);
        intersectionPoint = Ogre::Vector3(0,0,0);

	
        Ogre::Ray mouseRay = getMouseRay();//mCamera->getCameraToViewportRay( mouseX, mouseY );
		
		Ogre::SceneManager *mSceneMgr = app->currentLevel->getSceneManager();

		/*DynamicLines *lines = new DynamicLines(Ogre::RenderOperation::OT_LINE_LIST);
		
		lines->addPoint(mouseRay.getOrigin());
		lines->addPoint(mouseRay.getPoint(100));
		
		lines->update();
		Ogre::SceneNode *linesNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("lines");
		linesNode->attachObject(lines);*/
		//dh der strahl stimmt schonmal
		
        //neue herangehensweise:
        //wenn unter den ergebnissen eines der achsendinger ist, hat das priorität!

        
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
            if(itr->movable && itr->movable->getMovableType() == "Entity")
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

        //jetzt prüfen ob ich was neues habe
        if(closestAxis != tNone)
        {
            
            closestObject = selectedObject;		
            
            intersectionPoint = mouseRay.getPoint(closestAxisRes.distance);
            selectedObject->getPositionOrientation(startPosition,startOrientation);
            holdingOffset = intersectionPoint - startPosition;	
            startScale = selectedObject->getScale();
            unSnappedScale = startScale;

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

        nextSelected = closestObject;
        //mDebugText("nextSelected="+ptrToString(nextSelected));


        return closestObject;
    }


    void EditGameLogic::showMouseCursor(bool show)
    {
        //ausbauen?
    }


    //void EditGameLogic::processTextKey(UniKey key)
    //{
    //    //ausbauen?
    //    // if(!key.isKeyboard || !key.hasModifier() || !(key.modifier == OIS::Keyboard::Ctrl))
    //        // //alles mit Strg
    //        // return;
    //    // CEGUI::Window *wnd = getActiveWindow();
    //    // if(!wnd)
    //        // return;
    //    // Ogre::String type = wnd->getType().c_str();
    //    // if(!Ogre::StringUtil::endsWith(type,"editbox"))
    //        // return;
    //    // CEGUI::Editbox *edit = (CEGUI::Editbox*)wnd;
    //    // size_t selStart = edit->getSelectionStartIndex();
    //    // size_t selEnd	= edit->getSelectionEndIndex();
    //    // size_t selLength= edit->getSelectionLength();
    //    // CEGUI::String txt		= edit->getText();//.c_str();
    //    // switch(key.keyboardID)
    //    // {
    //    // case OIS::KC_C:
    //        // if(selLength != 0)// && !edit->isReadOnly())
    //        // {
    //            // //nicht so wie in VC++, dass kopieren von leerem text "" kopiert -_-
    //            // CEGUI::String sub = txt.substr(selStart,selLength);
    //            // //mDebugText(sub);
    //            // app->setClipboardText(sub);
    //        // }
    //        // break;
    //    // case OIS::KC_V:
    //        // if(!edit->isReadOnly())
    //        // {
    //            // //Ogre::String neu = txt;
    //            // //mDebugText(neu+"\n");
    //            // CEGUI::String davor = txt.substr(0,selStart);
    //            // //mDebugText(davor+"\n",true);
    //            // CEGUI::String danach = txt.substr(selEnd);
    //            // //mDebugText(danach+"\n",true);
    //            // CEGUI::String cb = app->getClipboardText();
    //            // CEGUI::String neu = davor+cb+danach;
    //            // edit->setText(neu);
    //            // //std::string-methoden sind extrem fürn Arsch, wenn man grad nicht nachgoogeln kann...
    //            // //mDebugText(neu,true);
    //        // }
    //        // break;
    //    // case OIS::KC_X:
    //        // if(!edit->isReadOnly())
    //        // {
    //            // CEGUI::String sub = txt.substr(selStart,selLength);
    //            // //mDebugText(sub);
    //            // app->setClipboardText(sub);
    //            // //Ogre::String neu = txt;
    //            // //mDebugText(neu+"\n");
    //            // CEGUI::String davor = txt.substr(0,selStart);
    //            // //mDebugText(davor+"\n",true);
    //            // CEGUI::String danach = txt.substr(selEnd);
    //            // //mDebugText(danach+"\n",true);
    //            // CEGUI::String neu = davor+danach;
    //            // edit->setText(neu);
    //            // //std::string-methoden sind extrem fürn Arsch, wenn man grad nicht nachgoogeln kann...
    //            // //mDebugText(neu,true);
    //        // }
    //        // break;
    //    // case OIS::KC_A:
    //        // //alles markieren
    //        // edit->setSelection(0,txt.length());

    //    // }
    //    
    //}


    void EditGameLogic::finishSelectionBox()
    {
		if(!selBoxDrawing)
			return;
        selBoxDrawing = false;
		if(selectionLocked)
		{
			selBorder->cancel();
            return;
		}
		QCursor cur = mRealApp->getOgreWidget()->cursor();
		QPoint pos = lastMouseEvent->pos();

		GroupObject *gr = NULL;
		if(isKeyDown(key_group) && selectedObject)
		{
			if(selectedObject->getType() == otGroup)
			{
				//adden
				gr = (GroupObject*)selectedObject;
			}
		}

		setSelected(selBorder->end(pos.x(),pos.y(),gr));
        //selBoxEnd = CEGUI::MouseCursor::getSingletonPtr()->getPosition();
        //mDebugText("box ende: start= ("+ogre_str(selBoxStart)+") => ("+ogre_str(selBoxEnd)+")");
        //app->hideSelBorder();
        

		


		/*
        //ok, hier jetzt der raycast
        /*
        1---2
        |   |
        3---4
        wobei 1=position
        und 4= pos+size
        */
		/*
        Ogre::Real wWidth = app->mWindow->getWidth();
        Ogre::Real wHeight= app->mWindow->getHeight();
        
        CEGUI::URect rect = app->selectionBorder->getArea();
        //rect.
        CEGUI::UVector2 pos = app->selectionBorder->getPosition();
        CEGUI::UVector2 size= app->selectionBorder->getSize();
        //mDebugText("width="+ogre_str(size.d_x.d_offset)+"\nheight="+ogre_str(size.d_y.d_offset));
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
		/*
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
        ////DIESEN TEIL LASS ICH ERSTMAL
        ////Man kann ihn entkommentieren, um den Rahmen zu debuggen
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

		*/
    }


    void EditGameLogic::updateSelection(bool grouping)
    {
        //ich habe ein neues objekt gefunden
        //prüfen, ob ich ne taste gedrückt halte, um zu gruppieren
        if(nextSelected == selectedObject)
            return;
        if(grouping && selectedObject)
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

	void EditGameLogic::dropItem(gamedata *data)
	{
		if(data->type == gtItem)
		{
			gamedata_item *itemData = static_cast<gamedata_item *>(data);
			if(itemData->Container)
			{
				//this is the dummy item for a container. place the container instead.
				dropItem(itemData->Container);
				return;
			}
		}
		GameObject *placed = app->getCurrentLevel()->placeObject(data,getDropPosition(),Ogre::Quaternion::IDENTITY);
		app->updateCurObjectsList(placed,true); //hier gibts keine gruppen
		//hm
		app->qtApp->getOgreWidget()->repaint();
	}

	void EditGameLogic::dropItem()
	{
		dropItem(app->dragItemType,app->dragItemID,WT_NONE);
		app->dragItemID="";			
	}

    void EditGameLogic::dropItem(ObjType objectType,Ogre::String objectID,WorldArtType staticType)
	{	
		Ogre::Vector3 pos = getDropPosition();

		GameObject *placed = app->getCurrentLevel()->placeObject(objectType,objectID,pos,Ogre::Quaternion::IDENTITY,Ogre::Vector3::UNIT_SCALE,staticType);
		app->updateCurObjectsList(placed,true); //hier gibts keine gruppen
		//hm
		app->qtApp->getOgreWidget()->repaint();
	}

	Ogre::Vector3 EditGameLogic::getDropPosition()
	{
		Ogre::Ray halfRay = mCamera->getCameraToViewportRay( 0.5, 0.5);
	
		return halfRay.getPoint(10); //getDropPosition();
	}


    void EditGameLogic::objectMoveDown()
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



	
	void EditGameLogic::drawSelectionBox()
	{
		if(selectionLocked || !app->getCurrentLevel())
			return;
		QCursor cur = mRealApp->getOgreWidget()->cursor();
		QPoint pos = lastMouseEvent->pos();
		if(!selBoxDrawing)
		{
			
		
			selBoxDrawing = true;
			
			selBorder->begin(pos.x(),pos.y());
			//app->initSelBorder(selBoxStart);
		}
		//app->updateSelBorder(CEGUI::MouseCursor::getSingletonPtr()->getPosition());
		selBorder->update(pos.x(),pos.y());
	}

	
	void EditGameLogic::objectScale()
	{
		//CEGUI::Point mousepos =	CEGUI::MouseCursor::getSingleton().getPosition();

		//Ogre::Real mouseX = mousepos.d_x/mWindow->getWidth();
		//Ogre::Real mouseY = mousepos.d_y/mWindow->getHeight();

		////CEGUI::MouseCursor::getSingleton().setImage((CEGUI::utf8*)STYLE_EDITOR,(CEGUI::utf8*)"MouseMoveCursor");


		//Ogre::Camera *mCamera = app->getCurrentLevel()->getMainCam();
		//Ogre::Ray mouseRay = mCamera->getCameraToViewportRay( mouseX, mouseY );
		Ogre::Ray mouseRay = getMouseRay();
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
						//res = Ogre::Math::intersects(mouseRay,pl);
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

						
						//res = Ogre::Math::intersects(mouseRay,pl);
						Ogre::Real testval = Ogre::Math::Abs(normal.dotProduct(mouseRay.getDirection())); 
						
						if(testval < 0.5)//!res.first)
						{				
							//es ist besser mit der anderen ebene			
							normal = oldOrientation * Ogre::Vector3::UNIT_Z; //normale der ebene, IN der ich ziehe.
							pl.redefine(normal,pos);					

						}
					}
					break;
				case tAxisZ:
					{
						cIndex = 2;
						axis = oldOrientation * Ogre::Vector3::UNIT_Z; //die achse, an der ich ziehe
						normal = oldOrientation * Ogre::Vector3::UNIT_Y; //normale der ebene, IN der ich ziehe.
						pl.redefine(normal,pos);	

						
						//res = Ogre::Math::intersects(mouseRay,pl);
						Ogre::Real testval = Ogre::Math::Abs(normal.dotProduct(mouseRay.getDirection())); 
						
						if(testval < 0.5)//!res.first)
						{				
							//es ist besser mit der anderen ebene			
							normal = oldOrientation * Ogre::Vector3::UNIT_X; //normale der ebene, IN der ich ziehe.
							pl.redefine(normal,pos);							

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
				
				
				newScale = unSnappedScale;
				if(app->snapOnScale)
				{
					Ogre::Real scaleSnap = app->scaleSnap;
					//mDebugText(ogre_str(newScale.x)+"\n");
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
					//mDebugText(ogre_str(newScale.x),true);
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
	//			mDebugText(ogre_str(ratio));
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
					//mDebugText(ogre_str(newScale.x)+"\n");
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
					//mDebugText(ogre_str(newScale.x),true);
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
	//			mDebugText(ogre_str(testval));
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
					//mDebugText(ogre_str(newScale.x)+"\n");
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
					//mDebugText(ogre_str(newScale.x),true);
				}
				//ende planecode

				
			}
			break;
		}



		
		//mDebugText(ogre_str(newScale));
		selectedObject->setScale(newScale);
	}

		
	void EditGameLogic::objectRotate()
	{

		Ogre::Quaternion newOrientation;


		Ogre::Vector3 oldPosition;// = selectedObject->getPosition();
		Ogre::Quaternion oldOrientation;// = selectedObject->getOrientation();
		selectedObject->getPositionOrientation(oldPosition,oldOrientation);
		//mDebugText(ogre_str(oldOrientation));
		

	//	CEGUI::Point mousepos =	CEGUI::MouseCursor::getSingleton().getPosition();

	//	Ogre::Real mouseX = mousepos.d_x/mWindow->getWidth();
	//	Ogre::Real mouseY = mousepos.d_y/mWindow->getHeight();

	//	CEGUI::MouseCursor::getSingleton().setImage((CEGUI::utf8*)STYLE_EDITOR,(CEGUI::utf8*)"MouseMoveCursor");

	///*	app->setDebugText("cegui: x="+Ogre::StringConverter::toString()+
	//						" y="+Ogre::StringConverter::toString(mousepos.d_y/mRenderWindow->getHeight())+"\n");
	//	app->setDebugText("ogre: x="+Ogre::StringConverter::toString(e->getX())+
	//						" y="+Ogre::StringConverter::toString(e->getY()),true);*/

	//	Ogre::Camera *mCamera = app->getCurrentLevel()->getMainCam();//mSceneMgr->getCamera("PlayerCam");
		Ogre::Ray mouseRay = getMouseRay();//mCamera->getCameraToViewportRay( mouseX, mouseY );
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

			//mDebugText("dir="+ogre_str(dir));
			//mDebugText("\nbasedir="+ogre_str(basedir),true);
			//mDebugText("\naxis="+ogre_str(axis),true);

			

			

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

		
	void EditGameLogic::objectMove()
	{
		/*CEGUI::Point mousepos =	CEGUI::MouseCursor::getSingleton().getPosition();

		Ogre::Real mouseX = mousepos.d_x/mWindow->getWidth();
		Ogre::Real mouseY = mousepos.d_y/mWindow->getHeight();

		CEGUI::MouseCursor::getSingleton().setImage((CEGUI::utf8*)STYLE_EDITOR,(CEGUI::utf8*)"MouseMoveCursor");


		Ogre::Camera *mCamera = app->getCurrentLevel()->getMainCam();*/
		Ogre::Ray mouseRay = getMouseRay();//mCamera->getCameraToViewportRay( mouseX, mouseY );
		Ogre::Vector3 rayOrigin = mouseRay.getOrigin();
		Ogre::Vector3 rayDirection = mouseRay.getDirection();
		Ogre::Vector3 newPosition, oldPosition;
		oldPosition = selectedObject->getPosition();

		Ogre::Vector3 pointOnPlane = intersectionPoint;

		Ogre::Vector3 hOffset = holdingOffset;
		//mDebugText(ogre_str(rayDirection));
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

		
	Ogre::Vector3 EditGameLogic::snapVector(Ogre::Degree angle,Ogre::Vector3 src,Ogre::Vector3 axis)
	{
	//	mDebugText("src="+ogre_str(src)+"\naxis="+ogre_str(axis));
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
			/*mDebugText("snap "+ogre_str(i+1)+"mal gedreht");*/
			snap = rotBy * snap;
		}
		return Ogre::Vector3::ZERO;
	    
	}
	
	
	void EditGameLogic::showDebugLines(bool show)
	{
		Level::getCurrent()->showNewtonDebugLines(show);	
		//EditorApp::getSingletonPtr()->getCurrentLevel()->showNewtonDebugLines(show);	
	}
}

