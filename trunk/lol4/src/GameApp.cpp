//------- BEGIN OGRE & stuff includes ----------
#include <Ogre.h>

#include <OgreFrameListener.h>
//#include <OgreEventListeners.h>
//#include <OgreKeyEvent.h>


//#include <OgreTextAreaOverlayElement.h>
#include <OgreFontManager.h>
#include <vector>
#include <OgreNewt.h>
#include "tinyxml.h"
#include "Level.h"
#include "../res/resource.h"
#include "xmlwrapper.h"


////mem probs without this next one
//#include <OgreNoMemoryMacros.h>
#include <CEGUIImageset.h>
#include <CEGUISystem.h>
#include <CEGUILogger.h>
#include <CEGUISchemeManager.h>
#include <CEGUIWindowManager.h>
#include <CEGUIWindow.h>
#include <RendererModules/Ogre/CEGUIOgreRenderer.h>
#include <RendererModules/Ogre/CEGUIOgreResourceProvider.h>
#include <elements/CEGUIPushButton.h>
#include <elements/CEGUIScrollablePane.h>
#include <elements/CEGUICombobox.h>
#include <elements/CEGUIListboxTextItem.h>
#include <elements/CEGUITabControl.h>
#include <elements/CEGUIScrolledContainer.h>
#include <RendererModules/Ogre/CEGUIOgreRenderer.h>
#include <RendererModules/Ogre/CEGUIOgreResourceProvider.h>

#include <CEGUITexture.h>
#include <RendererModules/Ogre/CEGUIOgreTexture.h>


#include "FwDec.h"
//#include "game_obj.h"
#include "GameChar.h"
//#include "global_class.h"
#include "GameFrameListener.h"
#include "GameApp.h"
#include "contactCallback.h"
#include "Inventory.h"
#include "functions.h"
#include "defines.h"
#include "Spellbook.h"
#include "windows.h"
#include "sound.h"
#include "SoundManager.h"
#include "Listener.h"
#include "Buffer.h"
#include "Source.h"

#include <HoldingSlotWindow.h>
#include <OptionsWindow.h>
#include <HUD.h>
#include <ContainerWindow.h>
#include <InventoryWindow.h>
#include <SpellbookWindow.h>
#include <MainMenu.h>
#include <SaveLoadMenu.h>
#include <TypeConverter.h>
#include <zip.h>


using namespace Ogre;
using namespace CEGUI;



GameApp::GameApp(Ogre::String cmdline):
	playerSpellbookView(NULL),
	playerInventoryView(NULL),
	showConfigDialog(false),
	menuMode(GUI_DISABLE),
	paused(false),
	shutDown(false),
	cursorMode(false),
	mGUISystem(NULL),
	mGUIRenderer(NULL),
	mIngameGui(NULL),
	mMainMenuGui(NULL),
	
	tempSaveGame(NULL),
	configLoaded(false),
	DebugDoOnce(false),
	screenTextShown(false),
	screenTextTime(0),
	waitingUntilUnpause(0),
	battleMusic(false),
	zeroTime(false),
	music(NULL),
	compassWnd(NULL),	
	ambient1(NULL),
	ambient2(NULL),
	playersfx(NULL),
	screenshotBase("lol4_"),
	
	otherInventoryView(NULL),
	/*otherInvOpen(false),
	
	curOpenInventory(NULL),*/
	sllLevel(""),
	sllEntrance(""),
	sll(false),
	
	unpauseNextFrame(false),
	mFrameListener(0),
	curHolding(NULL),
	mHud(NULL),
	mOptWnd(NULL)
	
{
    
  
	
	/*curHolding.inventory = NULL;
	curHolding.spell = NULL;*/


	//first, get the params
    if(!cmdline.empty())
	    paramList = Ogre::StringUtil::split(cmdline," ");
	



}

GameApp::~GameApp()
{

	saveConfig();

	if(mHud)
		delete mHud;

	if(playerInventoryView)
        delete playerInventoryView;
    if(playerSpellbookView)
        delete playerSpellbookView;
    if(otherInventoryView)
		delete otherInventoryView;



	delete currentLevel;
	
	
	if(mIngameGui)
	{
		wmgr->destroyWindow(mIngameGui);
	}
	if(mGUISystem)
	{		
		mGUISystem->destroy();
		//delete mGUISystem;
		mGUISystem = 0;
	}
	if(mGUIRenderer)
	{
		CEGUI::OgreRenderer::destroy(*mGUIRenderer);
		mGUIRenderer = 0;
	}
	if(mWindow)
	{
		if(mWindow->getNumViewports())
			mWindow->removeAllViewports();
	}
    
    

	mRoot->removeFrameListener(mFrameListener);
	if (mFrameListener)
        delete mFrameListener;

    


	if(soundMgr)
		delete soundMgr;




	if (mRoot)
        delete mRoot;
	//log("ok, aus4");

}


bool GameApp::setup(void)
{


    //mRoot = new Root();
	mRoot = new Root("","",mLogFileName);

//	log("setup in GA");

	//root ist da, jetzt kann ich loggen
	for(Ogre::StringVector::iterator itr = paramList.begin();itr != paramList.end();itr++)
	{
		Ogre::String param = *itr;
		if(param == "-config")
		{
			showConfigDialog = true;
		}
		Ogre::LogManager::getSingleton().logMessage("param="+param);
	}
    //BEGINNE NEUEN CODE
    
    TiXmlDocument *doc = loadConfigXml();
    
    setupPlugins(doc->FirstChildElement("Plugins"));
    //game und editor können damit verschiedene renderer settings haben, zB für mehr Leistung beim Editieren
    //zugunsten der Grafik
    setupRenderSystem(doc->FirstChildElement("game")->FirstChildElement("Renderer"));
    setupResources(doc->FirstChildElement("Resources"));
    //AB HIER ALTER CODE

	//mWorld = new OgreNewt::World();

    //setupResources();
//	log("resourcen gesetzt");

    bool carryOn = configure();
    if (!carryOn) return false;

	createResourceListener();
	// Load resources
	loadResources();

    loadManualLoD();

	loadGameData();


	createDefaultSceneMgr();








	createGUI();
//	log("nach createGUI();");
	setMenu(GUI_MAINMENU);
    createFrameListener();
    setupConfig(doc->FirstChildElement("game"));
//	loadConfig();

	
	initSound();

    delete doc;
    return true;

}

void GameApp::initSound()
{
	//SOUND 
	soundMgr = new SimpleSound::SoundManager();
	
	soundMgr->setDefaultMaxDist(settings.max_distance);
	soundMgr->setDefaultRefDistance(settings.ref_distance);
	soundMgr->setDefaultRolloffFactor(settings.rolloff_factor);

	soundMgr->setDistanceModel(settings.distance_model);
	soundMgr->getListener()->setGain(settings.gain_all);

	clickSnd = soundMgr->createBuffer("click.ogg");
	clickSnd->setType(btGlobal);

	

	music = SimpleSound::SoundManager::getSingletonPtr()->createSource();
	music->setType(stMusic);
	music->setRelativeToListener(true);

	ambient1 = SimpleSound::SoundManager::getSingletonPtr()->createSource();
	ambient1->setType(stMusic);
	ambient1->setRelativeToListener(true);

	ambient2 = SimpleSound::SoundManager::getSingletonPtr()->createSource();
	ambient2->setType(stMusic);
	ambient2->setRelativeToListener(true);

	playersfx = SimpleSound::SoundManager::getSingletonPtr()->createSource();
	playersfx->setType(stMusic);
	playersfx->setRelativeToListener(true);
	
}
//
void GameApp::setMusic(SimpleSound::Buffer *buff, bool loop)
{

	
	music->setBuffer(buff);
	if(buff)
	{
		music->setLoop(loop);
		music->setGain(settings.gain_music);
		music->play();
	}

}

void GameApp::playPlayerSFX(SimpleSound::Buffer *buff)
{
	////1x spielen und aus
	
	
	playersfx->setBuffer(buff);
	if(buff)
	{
		playersfx->setLoop(false);
		playersfx->setGain(settings.gain_player_sfx);
		playersfx->play();
	}


}

void GameApp::setAmbient1(SimpleSound::Buffer *buff)
{
	
	if(!ambient1->isStopped())
		ambient1->stop();
	ambient1->setBuffer(buff);
	if(buff)
	{
		ambient1->setLoop(false);
		ambient1->setGain(settings.gain_sfx);
		ambient1->play();
	}

}
void GameApp::setAmbient2(SimpleSound::Buffer *buff,bool loop)
{
	
	if(!ambient2->isStopped())
		ambient2->stop();
	ambient2->setBuffer(buff);
	if(buff)
	{
		ambient2->setLoop(loop);
		ambient2->setGain(settings.gain_sfx);
		ambient2->play();
	}

}


void GameApp::setPaused(bool set)
{
	
	paused = set;
	if(soundMgr)
	{
		if(paused)
			soundMgr->pauseAllSources();
		else
			soundMgr->resumeAllSources();
	}
	
}


void GameApp::saveConfig()
{
	if(!configLoaded)
		return;
	TiXmlDocument* doc = loadConfigXml();

	OgreXmlElement *gameElem = static_cast<OgreXmlElement *>(doc->FirstChildElement("game"));
	if(!gameElem)
	{
		gameElem = new OgreXmlElement("game");
		doc->LinkEndChild(gameElem);
	}

    gameElem->Clear();

	//OGRE config/renderer settings
	OgreXmlElement *rendererElem = new OgreXmlElement("Renderer");
    rendererElem->SetAttribute("selected",selectedRenderer->getName());
    gameElem->LinkEndChild(rendererElem);
	renderSettingsToXml(rendererElem);
	gameElem->LinkEndChild(settings.getAsXml());
    

	saveConfigXml(doc);
}

bool GameApp::configure(void)
{

    // Show the configuration dialog and initialise the system
    // You can skip this and use root.restoreConfig() to load configuration
    // settings if you were sure there are valid ones saved in ogre.cfg
	//mRoot->
	//TEST

	//RenderSystemList *rlist = mRoot->getAvailableRenderers();
	//for(RenderSystemList::iterator itr = rlist->begin();itr!=rlist->end();itr++)
	//{
	//	RenderSystem *rs = *itr;
	//	log("render="+rs->getName());
	//	ConfigOptionMap options = rs->getConfigOptions();
	//	for(ConfigOptionMap::iterator it2 = options.begin();it2!=options.end();it2++)
	//	{
	//		String name = it2->first;
	//		ConfigOption opt = it2->second;
	//		log("-option "+name);
	//		log("--currentVal="+opt.currentValue);
	//		log("--immutable="+StringConverter::toString(opt.immutable));
	//		log("--name="+opt.name);
	//		log("--possibleValues=");
	//		for(Ogre::StringVector::iterator it3 = opt.possibleValues.begin();it3 != opt.possibleValues.end();it3++)
	//		{
	//			String val = *it3;
	//			log("---"+val);
	//
	//		}
	//	}
	//}
	/*if(!showConfigDialog)
	{
		std::ifstream cf;
		cf.open(OGRECONFIG, std::ios::in | std::ios::binary);
		if(!cf)
		{
			showConfigDialog = true;
		}else
		{
			if(!mRoot->restoreConfig())
			{
				return false;
			}
		}


	}*/

	if(showConfigDialog)
	{
		if(!mRoot->showConfigDialog())
		{
			return false;
		}
	}






	mWindow = mRoot->initialise(true, WINDOW_NAME);
	selectedRenderer = mRoot->getRenderSystem();
//	curSelectedRenderer = newRenderer;
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	HWND hwnd;
	mWindow->getCustomAttribute("WINDOW", (void*)&hwnd);
	LONG iconID   = (LONG)LoadIcon( GetModuleHandle(0), MAKEINTRESOURCE(IDI_ICON1) );
	SetClassLong( hwnd, GCL_HICON, iconID );
#endif
    return true;
	//if(!showConfigDialog)
	//{
	//	if(mRoot->restoreConfig())
	//	{
	//		mWindow = mRoot->initialise(true, "LoL4 test app");
	//		newRenderer = mRoot->getRenderSystem()->getName();
	//		return true;
	//	}
	//}

 //   if(mRoot->showConfigDialog())
 //   {
 //       // If returned true, user clicked OK so initialise
 //       // Here we choose to let the system create a default rendering window by passing 'true'
 //       mWindow = mRoot->initialise(true, "LoL4 test app");
	//	newRenderer = mRoot->getRenderSystem()->getName();
 //       return true;
 //   }
 //   else
 //   {
 //       return false;
 //   }
}
//void GameApp::chooseSceneManager(void)
//{
//// Create the Ogre::SceneManager, in this case a generic one
////-----------------------------------------------------
//// 4 Create the Ogre::SceneManager
////
////		ST_GENERIC = octree
////		ST_EXTERIOR_CLOSE = simple terrain
////		ST_EXTERIOR_FAR = nature terrain (depreciated)
////		ST_EXTERIOR_REAL_FAR = paging landscape
////		ST_INTERIOR = Quake3 BSP
////-----------------------------------------------------
////mSceneMgr = mRoot->createSceneManager(ST_GENERIC);//, "ExampleSMInstance");
//
//}

//void GameApp::createCamera(void)
//{
//    // Create the camera
//    //mCamera = mSceneMgr->createCamera("PlayerCam");
//
//    //// Position it at 500 in Z direction
//    ////mCamera->setPosition(Vector3(0,0,500));
//    //// Look back along -Z
//    ////mCamera->lookAt(Vector3(0,0,-300));
//    //mCamera->setNearClipDistance(0.01);
//
//}
//
//void GameApp::createMaterials(void)
//{
//	defaultMaterialID = mWorld->getDefaultMaterialID();
//
//	//OgreNewt::MaterialPair* default_default = new OgreNewt::MaterialPair( mWorld, defaultMaterialID, defaultMaterialID );
//	//default_default->setDefaultFriction(1.0f, 0.5f);
//	//default_default->setDefaultSoftness(0.05f);
//	//default_default->setDefaultElasticity(0.1f);
//
//	//charMaterial = new OgreNewt::MaterialID(mWorld);
//	////mBody->setMaterialGroupID(charMaterial);
//	////mBody->setLinearDamping(10);
//
//	//OgreNewt::MaterialPair* default_char = new OgreNewt::MaterialPair( mWorld, charMaterial, defaultMaterialID );
//
//
//	////cc_static_char = new ContCallback();
//
//
//	////material_pair2->setDefaultFriction(0.0f, 0.0f);
//	//default_char->setDefaultFriction(0,0);
//	//default_char->setDefaultSoftness(1);
//	//default_char->setDefaultElasticity(0);
//	//default_char->setContinuousCollisionMode(1);
//	////default_char->setContactCallback(cc_static_char);
//
//}

//void GameApp::createPlayer(void)
//{
//	//add my camera and apply material pair with floor
//
//
//
//	////player = new GameChar(mSceneMgr,mWorld,charMaterial,this,Vector3(0.0,15.0,0.0));
//	//player = new GameChar(this,"player",Vector3(0,15,0));
//	//if(player->failed())
//	//{
//	//	log("WTF? can't construct player");
//	//	mFrameListener->requestShutdown();
//	//}
//	//player->AttachPlayerCam();
//
//	////create another char
//	//testguy = new GameChar(this,"testguy",Vector3(0,15,-20));
//	//if(testguy->failed())
//	//{
//	//	delete testguy;
//	//	testguy = 0;
//	//	setDebugText("no testguy");
//	//	log("no testguy");
//	//}
//
//
//
//}
void GameApp::createFrameListener(void)
{
    //mFrameListener= new GameFrameListener(mWindow, mCamera);
    //mFrameListener->showDebugOverlay(true);


	

	/*mFrameListener = new GameFrameListener(this,mEventProcessor->getInputReader(),mGUIRenderer,mWindow, mSceneMgr, mWorld, 120 );*/
	mFrameListener = new GameFrameListener();//this,mEventProcessor->getInputReader(),mGUIRenderer);//, 120 );

	mRoot->addFrameListener(mFrameListener);
//
//	log("frame listener online");


}

//void GameApp::createScene(void)
//{
//	//mSceneMgr->setAmbientLight(ColourValue(1, 1, 1));
//	//log("licht->kk");
//
//
//
//	//loadLvl("start.xml");
//
//
//
//}

//void GameApp::destroyScene(void){}    // Optional to override this

//void GameApp::createViewports(void)
//{
//
//    //// Create one viewport, entire window
//    //Viewport* vp = mWindow->addViewport(mCamera);
//    //vp->setBackgroundColour(ColourValue(0,0,0));
//
//    //// Alter the camera aspect ratio to match the viewport
//    //mCamera->setAspectRatio(
//    //    Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
//}

/// Method which will define the source of resources (other than current folder)
//void GameApp::setupResources(void)
//{
//
//	// Load resource paths from config file
//
//	// File format is:
//	//  [ResourceGroupName]
//	//  ArchiveType=Path
//	//  .. repeat
//	// For example:
//	//  [General]
//	//  FileSystem=media/
//	//  Zip=packages/level1.zip
//
//	ConfigFile cf;
//	cf.load("resources.cfg");
//	Ogre::LogManager::getSingleton().logMessage("muh");
//	// Go through all sections & settings in the file
//	ConfigFile::SectionIterator seci = cf.getSectionIterator();
//
//	String secName, typeName, archName;
//	while (seci.hasMoreElements())
//	{
//		secName = seci.peekNextKey();
//		ConfigFile::SettingsMultiMap *settings = seci.getNext();
//		ConfigFile::SettingsMultiMap::iterator i;
//		for (i = settings->begin(); i != settings->end(); ++i)
//		{
//			typeName = i->first;
//			archName = i->second;
//			ResourceGroupManager::getSingleton().addResourceLocation(
//				archName, typeName, secName);
//			log(archName+" - "+typeName+" - "+secName);
//		}
//	}
//	log("fertig");
//
//}

void GameApp::createResourceListener(void)
{

}

/// Optional override method where you can perform resource group loading
/// Must at least do ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
//void GameApp::loadResources(void)
//{
//	// Initialise, parse scripts etc
//	ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
//	log("res init done");
//
//
//}

GameChar *GameApp::getPlayer()
{
	return currentLevel->getPlayer();
}

void GameApp::showInfoText(Ogre::String txt, Ogre::Real rel_x,Ogre::Real rel_y)
{
	if(!infoText->isVisible())
	{
		infoText->setVisible(true);
	}
	if(infoText->isDisabled())
	{
		infoText->enable();
	}
	Ogre::Real x,y;
	x = rel_x;
	y = rel_y;
	if(x>1)
		x = 1;
	if(x<0)
		x=0;
	if(y>1)
		y = 1;
	if(y<0)
		y=0;

	/*infoText->setPosition(CEGUI::Point(x,y));*/
	//infoText->setSize(CEGUI::Size(0.2f, 0.05f));
	CEGUI::String str = txt.c_str();
	if(str != infoText->getText())
	{
		infoText->setText(str);

		CEGUI::Font *fnt = infoText->getFont();
		Ogre::Real width = fnt->getTextExtent(str)+6;

		//CEGUI::Size mSize = mIngameGui->absoluteToRelative(CEGUI::Size(width,0.05f));
		//
		//mSize.d_height = 0.05f;
		//infoText->setSize(mSize);
		infoText->setSize(CEGUI::UVector2(CEGUI::UDim(0,width),CEGUI::UDim(0,30)));


	}

	infoText->setPosition(UVector2(UDim(0.5-infoText->getWidth().d_scale/2,0),UDim(y,0)));//CEGUI::Point(0.5-infoText->getRelativeWidth()/2,y));


}
void GameApp::updateInfoText(Ogre::Real rel_x, Ogre::Real rel_y)
{
	infoText->setPosition(UVector2(UDim(rel_x,0),UDim(rel_y,0)));//CEGUI::Point(rel_x,rel_y));
}
void GameApp::hideInfoText()
{
	if(infoText->isVisible())
	{
		infoText->setVisible(false);
	}
	if(infoText->isDisabled())
	{
		infoText->disable();
	}
}

//void GameApp::updateAudioWindow()
//{
//	/*setScrollbarPos((CEGUI::Scrollbar*)(wmgr->getWindow("options/gainAll")),gainAll);
//
//	setScrollbarPos((CEGUI::Scrollbar*)(wmgr->getWindow("options/gainMusic")),gainMusic);
//	setScrollbarPos((CEGUI::Scrollbar*)(wmgr->getWindow("options/gainSFX")),gainSFX);
//	setScrollbarPos((CEGUI::Scrollbar*)(wmgr->getWindow("options/gainSFXPlayer")),gainSFXPlayer);
//	setScrollbarPos((CEGUI::Scrollbar*)(wmgr->getWindow("options/gainSpeech")),gainSpeech);
//
//	
//	wmgr->getWindow("options/distance_model")->setText(distanceModelToString(distModel).c_str() );
//
//	wmgr->getWindow("options/maxdist")->setText(str(maxDistance).c_str());
//	wmgr->getWindow("options/rolloff")->setText(str(rollofFactor).c_str());
//	wmgr->getWindow("options/refdist")->setText(str(refDistance).c_str());*/
//}
//void GameApp::updateOptionsWindow()
//{
//	//jetzt video
//	updateVideoWindow();
//	//Jetzt Audio
//	updateAudioWindow();
//	//jetzt gamesettings
//	//
//	/*setScrollbarPos((CEGUI::Scrollbar*)(wmgr->getWindow("options/sens_x")),sensibility_x/2);
//	setScrollbarPos((CEGUI::Scrollbar*)(wmgr->getWindow("options/sens_y")),sensibility_y/2);*/
//	static_cast<CEGUI::Checkbox*>(wmgr->getWindow("options/mouselooking"))->setSelected(settings.mouselooking);
//}


void GameApp::updateStatBars(Ogre::Real hp, Ogre::Real mp, int maxHP, int maxMP)
{
	mHud->updateStatBars(hp,mp,maxHP,maxMP);
	/*Ogre::Real manaFactor = mp/float(maxMP);
	Ogre::Real hpFactor = hp/float(maxHP);

	UDim newHPWidth = hpBarWidth;
	newHPWidth.d_scale *= hpFactor;

	UDim newMPWidth = manaBarWidth;
	newMPWidth.d_scale *= manaFactor;

	manaBar->setWidth(newMPWidth);
	hpBar->setWidth(newHPWidth);

	
	Ogre::String text = str(Math::Floor(mp))+"/"+str(maxMP);
	manaBar_container->setTooltipText(text.c_str());
	

	text = str(Math::Floor(hp))+"/"+str(maxHP);
	hpBar_container->setTooltipText(text.c_str());*/
}
void GameApp::updateCompass()
{
	//return;

	Quaternion ornt = currentLevel->getPlayer()->getOrientation();
	
	mHud->setCompassOrientation(currentLevel->getNorthOrientation() * ornt.Inverse());
	//compNeedleNode->setOrientation(currentLevel->getNorthOrientation() * ornt.Inverse());
	
	
}


void GameApp::createCompass()
{
	//return;

	compSceneMgr= mRoot->createSceneManager(ST_GENERIC);
	compNeedleNode = compSceneMgr->getRootSceneNode()->createChildSceneNode();
	Entity *ent = compSceneMgr->createEntity("compassent","CompassNeedle.mesh");

	SceneNode *meshNode = compNeedleNode->createChildSceneNode();
	meshNode->attachObject(ent);

	compassCam = compSceneMgr->createCamera("compasscam");
	compassWnd = wmgr->getWindow("compass");
	unsigned int width = compassWnd->getPixelSize().d_width;
	unsigned int height = compassWnd->getPixelSize().d_height;
	
	TexturePtr renderTex = TextureManager::getSingleton().createManual( "CompassRttTex", 
		ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, TEX_TYPE_2D, 
		width, height, 0, PF_R8G8B8A8, TU_RENDERTARGET );

	
	
	compassCam->setAspectRatio(Ogre::Real(width) / Ogre::Real(height));
	compassCam->setPosition(0,1.3,0);
	
	compassCam->pitch(Degree(-90));
	compassCam->setNearClipDistance(0.01);
	
	Ogre::RenderTarget *rttTex = renderTex->getBuffer()->getRenderTarget();
	Viewport *v = rttTex->addViewport(compassCam);

	v->setClearEveryFrame(true);
	v->setBackgroundColour(ColourValue(0,0,0,0));
	
    

    MaterialPtr mat = MaterialManager::getSingleton().create("CompassRttMat",
        ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	TextureUnitState* t = mat->getTechnique(0)->getPass(0)->createTextureUnitState("CompassRttTex");
    
  
	t->setTextureAddressingMode(TextureUnitState::TAM_CLAMP);
	
	CEGUI::Texture *ceguiTex = &mGUIRenderer->createTexture(renderTex);

	CEGUI::String imageSetName = "compassimgset";
	
	Imageset* textureImageSet = &ImagesetManager::getSingletonPtr()->create(imageSetName, *ceguiTex);

	CEGUI::Size size = ceguiTex->getSize();
	
		

	textureImageSet->defineImage("compassneedle", Point(0.0f, 0.0f), Size(size.d_width, size.d_height), Point(0.0f,0.0f));
	

	compassWnd->setProperty("Image", "set:"+imageSetName+" image:compassneedle");
	


}

void GameApp::createGUI()
{
	// Set up GUI system
	Ogre::SceneManager *smgr = NULL;
	if(currentLevel)
	{
		smgr = currentLevel->getSceneManager();
	}
	else
	{
		smgr = noLevelMgr;
	}
	
	mGUIRenderer = &CEGUI::OgreRenderer::create(*mWindow->getViewport(0)->getTarget());//mWindow, Ogre::RENDER_QUEUE_OVERLAY, false, 3000, smgr);

	ceguiOgreResProvider = new CEGUI::OgreResourceProvider();

	mGUISystem = &CEGUI::System::create(*mGUIRenderer,ceguiOgreResProvider);//new CEGUI::System(mGUIRenderer);



	CEGUI::Logger::getSingleton().setLoggingLevel(CEGUI::Informative);

	//taharezlook:
	//CEGUI::SchemeManager::getSingleton().create ((CEGUI::utf8*)"TaharezLookSkin.scheme");
	//LandsLook:
	CEGUI::SchemeManager::getSingleton().create ((CEGUI::utf8*)"LandsLook.scheme");
	//das ist temporär und kommt später weg:
	CEGUI::SchemeManager::getSingleton().create ((CEGUI::utf8*)"lands.scheme");
	mGUISystem->setDefaultMouseCursor((CEGUI::utf8*)"LandsLook", (CEGUI::utf8*)"MouseArrow");
	CEGUI::MouseCursor::getSingleton().setImage("LandsLook", "MouseArrow");
	mGUISystem->setDefaultFont((CEGUI::utf8*)"DejaVuSans-10");

	CEGUI::System::getSingletonPtr()->setDefaultTooltip( "LandsLook/Tooltip" );





	//standard-zeug. die pointer brauche ich dauernd
	imgr = CEGUI::ImagesetManager::getSingletonPtr();
	wmgr = CEGUI::WindowManager::getSingletonPtr();

	//mIngameGui = wmgr->loadWindowLayout((CEGUI::utf8*)"game-ingame.layout");

	//SETTING mIngameGui
	

	mHud = new HUD();
	mIngameGui = mHud->getMainWindow();
	ingameRootWindow = mIngameGui;


	//mMainMenuGui = wmgr->loadWindowLayout((CEGUI::utf8*)"game-mainmenu.layout");
	
	//mOptionsGui = wmgr->loadWindowLayout((CEGUI::utf8*)"empty.layout");

	mSaveLoadMenu = new SaveLoadMenu();
	mSavegameGui = mSaveLoadMenu->getMainWindow();
	//mSavegameGui = wmgr->loadWindowLayout((CEGUI::utf8*)"game-saveload.layout");
//	log("nach optionen gui load");

	mOptWnd = new OptionsWindow();
	//mOptWnd->addToWindow(mOptionsGui);
	mOptionsGui = mOptWnd->getMainWindow();
	mOptWnd->show();

	playerInventoryView = new InventoryWindow();	
	playerInventoryView->hide();


	playerSpellbookView = new SpellbookWindow();
	playerSpellbookView->hide();

	otherInventoryView = new ContainerWindow();
	otherInventoryView->hide();


	
	//manaBar = wmgr->getWindow((CEGUI::utf8*)"manabar");
	//hpBar = wmgr->getWindow((CEGUI::utf8*)"hpbar");
	//manaBar->setInheritsTooltipText(true);
	//hpBar->setInheritsTooltipText(true);

	/*hpBar_container = wmgr->getWindow((CEGUI::utf8*)"hpbar_container");
	manaBar_container = wmgr->getWindow((CEGUI::utf8*)"manabar_container");

	hpBarWidth = hpBar->getWidth();
	manaBarWidth = manaBar->getWidth();*/

	mGUISystem->setGUISheet(mMainMenuGui);

	infoText = wmgr->createWindow("LandsLook/StaticText", (CEGUI::utf8*)"ViewInfoText");


	mIngameGui->addChildWindow(infoText);
	
	infoText->disable();
	infoText->setVisible(false);


	
	curHolding = new HoldingSlotWindow();
	curHolding->clear();
	
	mainMenu = new MainMenu();
	mMainMenuGui = mainMenu->getMainWindow();


	/*
	CEGUI::PushButton* quitButton = (CEGUI::PushButton*)wmgr->getWindow((CEGUI::utf8*)"quit");
	CEGUI::PushButton* startButton = (CEGUI::PushButton*)wmgr->getWindow((CEGUI::utf8*)"startgame");
	CEGUI::PushButton* resumeButton = (CEGUI::PushButton*)wmgr->getWindow((CEGUI::utf8*)"resume");
	CEGUI::PushButton* optButton = (CEGUI::PushButton*)wmgr->getWindow((CEGUI::utf8*)"options");
	CEGUI::PushButton* saveButton = (CEGUI::PushButton*)wmgr->getWindow((CEGUI::utf8*)"saveload");




	saveButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameApp::saveLoadClick, this));
	quitButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameApp::handleQuit, this));
	startButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameApp::startGameClick, this));
	optButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameApp::optionsClick, this));
	resumeButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameApp::resumeClick, this));
	
	resumeButton->disable();
	*/

	/*hier das savegame-fenster
	((CEGUI::PushButton*)wmgr->getWindow((CEGUI::utf8*)"saveloadwnd/save"))->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameApp::savegameSaveClick, this));
	((CEGUI::PushButton*)wmgr->getWindow((CEGUI::utf8*)"saveloadwnd/load"))->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameApp::savegameLoadClick, this));
	((CEGUI::PushButton*)wmgr->getWindow((CEGUI::utf8*)"saveloadwnd/delete"))->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameApp::savegameDeleteClick, this));
	((CEGUI::PushButton*)wmgr->getWindow((CEGUI::utf8*)"saveloadwnd/cancel"))->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameApp::hideSaveLoadWnd, this));
	((CEGUI::Listbox*)wmgr->getWindow((CEGUI::utf8*)"saveloadwnd/savegamelist"))->subscribeEvent(CEGUI::Window::EventMouseClick, CEGUI::Event::Subscriber(&GameApp::savegameListClick, this));
	((CEGUI::Listbox*)wmgr->getWindow((CEGUI::utf8*)"saveloadwnd/savegamelist"))->subscribeEvent(CEGUI::Window::EventMouseDoubleClick, CEGUI::Event::Subscriber(&GameApp::savegameListDblClick, this));
	*/

	//CEGUI::PushButton* optionsCancel = (CEGUI::PushButton*)wmgr->getWindow((CEGUI::utf8*)"OptionsCancel");
	//CEGUI::PushButton* optionsOK = (CEGUI::PushButton*)wmgr->getWindow((CEGUI::utf8*)"OptionsOK");
	//CEGUI::PushButton* optionsAccept = (CEGUI::PushButton*)wmgr->getWindow((CEGUI::utf8*)"OptionsAccept");

	//optionsCancel->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameApp::optionsCancelClick, this));
	//optionsOK->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameApp::optionsOKClick, this));
	//optionsAccept->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameApp::optionsAcceptClick, this));



	//updateOptionsWindow();

	//audio-dropdown-liste
	///*CEGUI::Combobox *distMod = (CEGUI::Combobox*)wmgr->getWindow("options/distance_model");

	//

	//CEGUI::ListboxTextItem *itemINVERSE_DISTANCE = new CEGUI::ListboxTextItem("INVERSE_DISTANCE");
	//itemINVERSE_DISTANCE->setSelectionBrushImage("LandsLook","TextSelectionBrush");
	//distMod->addItem(itemINVERSE_DISTANCE);

	//CEGUI::ListboxTextItem *itemINVERSE_DISTANCE_CLAMPED = new CEGUI::ListboxTextItem("INVERSE_DISTANCE_CLAMPED");
	//itemINVERSE_DISTANCE_CLAMPED->setSelectionBrushImage("LandsLook","TextSelectionBrush");
	//distMod->addItem(itemINVERSE_DISTANCE_CLAMPED);

	//CEGUI::ListboxTextItem *itemLINEAR_DISTANCE = new CEGUI::ListboxTextItem("LINEAR_DISTANCE");
	//itemLINEAR_DISTANCE->setSelectionBrushImage("LandsLook","TextSelectionBrush");
	//distMod->addItem(itemLINEAR_DISTANCE);

	//CEGUI::ListboxTextItem *itemLINEAR_DISTANCE_CLAMPED = new CEGUI::ListboxTextItem("LINEAR_DISTANCE_CLAMPED");
	//itemLINEAR_DISTANCE_CLAMPED->setSelectionBrushImage("LandsLook","TextSelectionBrush");
	//distMod->addItem(itemLINEAR_DISTANCE_CLAMPED);

	//CEGUI::ListboxTextItem *itemEXPONENT_DISTANCE = new CEGUI::ListboxTextItem("EXPONENT_DISTANCE");
	//itemEXPONENT_DISTANCE->setSelectionBrushImage("LandsLook","TextSelectionBrush");
	//distMod->addItem(itemEXPONENT_DISTANCE);

	//CEGUI::ListboxTextItem *itemEXPONENT_DISTANCE_CLAMPED = new CEGUI::ListboxTextItem("EXPONENT_DISTANCE_CLAMPED");
	//itemEXPONENT_DISTANCE_CLAMPED->setSelectionBrushImage("LandsLook","TextSelectionBrush");
	//distMod->addItem(itemEXPONENT_DISTANCE_CLAMPED);*/

//	createCompass();

	//infofeld
	hideScreenText();


}



//bool GameApp::handleQuit(const CEGUI::EventArgs& e)
//{
//	mFrameListener->requestShutdown();
//	return true;
//}

void GameApp::ceguiSetImage(CEGUI::Window *wnd, CEGUI::String str)
{
	wnd->setProperty("Image", "set:"+str+" image:full_image");
}

//void GameApp::setHotkey(int index, gamedata_hotkeyable *item)
//{
//    if(index<0 || index>9)
//    {
//        mLog("GameApp::setHotkey mit index="+str(index)+" aufgerufen!");
//        return;
//    }
//    Ogre::String wid = "hotkey"+str(index+1);
//    CEGUI::Window *field = wmgr->getWindow(wid.c_str());
//    if(item)
//    {
//        //bildchen     
//		ceguiSetImage(field,item->inv_image.c_str());
//		field->setTooltipText(item->name.c_str());
//		
//    }
//    else
//    {
//        field->setProperty("Image", "False");		
//		
//		field->setTooltipText("");
//    }
//    hotkeys[index] = item;
//}
//gamedata_hotkeyable *GameApp::getHotkey(int index)
//{
//    if(index<0 || index>9)
//    {
//        mLog("GameApp::getHotkey mit index="+str(index)+" aufgerufen!");
//        return NULL;
//    }
//    return hotkeys[index];
//}
//bool GameApp::useHotkey(int index)
//{
//    if(index<0 || index>9)
//    {
//        return false;
//    }
//    if(!hotkeys[index])
//        return false;
//    //prüfen, ob player das hat
//    bool playerHas = false;
//    if(hotkeys[index]->type == gtItem)
//        playerHas = currentLevel->getPlayer()->hasItem(static_cast<gamedata_item*>(hotkeys[index]));
//    else
//        playerHas = currentLevel->getPlayer()->hasSpell(static_cast<gamedata_spell*>(hotkeys[index]));
//
//    if(playerHas)
//        return hotkeys[index]->onHotkey(currentLevel->getPlayer());
//    else
//        //oha, der player hat das item gar nicht!
//        return false;
//}

void GameApp::resetApp()
{
	//remove the tempsave
	if(tempSaveGame)
	{
		tempSaveGame->eraseArchive();
		delete tempSaveGame;
		tempSaveGame = NULL;
	}

    if(playerInventoryView && playerInventoryView->isShown())
		showInventory(false);
    if(playerSpellbookView && playerSpellbookView->isShown())
		showSpellbook(false);
    if(otherInventoryView && otherInventoryView->isShown())
		hideOtherInventory();

    for(int i=0;i<10;i++)
    {
		mHud->setHotkey(i,NULL);
    }

	enemyList.clear();

	curHolding->clear();

	battleMusic = false;
	


	setDisplayedSpell(NULL);
	setDisplayedWeapon(NULL);

}
//
//bool GameApp::startGameClick(const CEGUI::EventArgs& e)
//{
//	
//	tempSaveGame = new ZipSaveFile("tempsave",tempPath,SAVEGAME_EXTENSION);
//	tempSaveGame->removeFile();
//	/*CEGUI::PushButton* resumeButton = (CEGUI::PushButton*)wmgr->getWindow((CEGUI::utf8*)"resume");
//	resumeButton->enable();*/
//	mainMenu->setResumeEnabled(true);
//	resetApp();
//	loadLevel(settings.starting_level,settings.starting_entrance,true);
//	setMenu(GUI_INGAME,false);
//	/*if(soundMgr)
//		soundMgr->destroyAllSounds();*/
//	return true;
//}
//bool GameApp::resumeClick(const CEGUI::EventArgs& e)
//{
////	log("resumeClick");
//	if(currentLevel)
//	{
//		setMenu(GUI_INGAME);
//	}
//	return true;
//}

void GameApp::quitGame()
{
	mFrameListener->requestShutdown();
}
void GameApp::startGame()
{
	resetApp();
	//here we need a fresh and new tempSaveGame
	//the constuctor should also create the file
	tempSaveGame = new ZipSaveFile("tempsave",tempPath,SAVEGAME_EXTENSION);	
	
	loadLevel(settings.starting_level,settings.starting_entrance,true);
	setMenu(GUI_INGAME,false);
}
void GameApp::resumeGame()
{
	if(currentLevel)
	{
		setMenu(GUI_INGAME);
	}
}
void GameApp::showOptions()
{
	setMenu(GUI_OPTIONS);
}
void GameApp::showSaveLoad()
{
	setMenu(GUI_SAVEGAME);
}

void GameApp::loadHotkeys(TiXmlElement *elem)
{
    for(int i=0;i<=9;i++)
    {        
		mHud->setHotkey(i,NULL);
        //hotkeys[i] = NULL;
    }
    TiXmlElement *hk = elem->FirstChildElement("hotkey");
    int cur_i = 0;
    while(hk)
    {
        Ogre::String type = hk->Attribute("type");
        if(type == "item")
        {
			mHud->setHotkey(cur_i,getItemDataPtr(hk->Attribute("id")));
            //hotkeys[cur_i] = getItemDataPtr(hk->Attribute("id"));
            cur_i++;
        }
        else if(type == "spell")
        {
			mHud->setHotkey(cur_i,getSpellDataPtr(hk->Attribute("id")));
            //hotkeys[cur_i] = getSpellDataPtr(hk->Attribute("id"));
            cur_i++;
        }
        hk = hk->NextSiblingElement("hotkey");
    }
        
}

TiXmlElement *GameApp::getHotkeysXml()
{
    TiXmlElement * mainElem = new TiXmlElement( "hotkeys" );
    for(int i=0;i<=9;i++)
    {
        TiXmlElement * elem = new TiXmlElement( "hotkey" );
		gamedata_hotkeyable *cur = mHud->getHotkey(i);
        if(!cur)
        {
            elem->SetAttribute("type","none");
        }
        else
        {
            if(cur->type == gtItem)
                elem->SetAttribute("type","item");
            else
                elem->SetAttribute("type","spell");
            elem->SetAttribute("id",cur->id);
        }
        mainElem->LinkEndChild(elem);
    }
    return mainElem;
}

Ogre::String GameApp::getSavegameSettings()
{
	TiXmlDocument doc;
	//now try to write all the stuff to it...
	
	TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );


	TiXmlElement * mainElem = new TiXmlElement( "savegame" );
	mainElem->SetAttribute("name","erstmal test");
	mainElem->SetAttribute("playerlevel",currentLevel->getFileName().c_str());
    int playerSGID = currentLevel->getPlayer()->getSGID();
    mainElem->SetAttribute("playersgid",TypeConverter::ogre_str(playerSGID));
    //erstmal spielersettings
	//TiXmlElement * playerElem = currentLevel->getPlayer()->getXmlElem();
	//playerElem->SetValue("player");
	//mainElem->LinkEndChild(playerElem);
    //hotkeys
    mainElem->LinkEndChild(getHotkeysXml());
	//jetzt theoretisch noch so zeug wie globale vars...
	doc.LinkEndChild( decl );
	doc.LinkEndChild( mainElem );
	
	std::stringstream st;
	st << doc;
	return st.str();
	
	
}

//
//bool GameApp::saveLoadClick(const CEGUI::EventArgs& e)
//{
//	setMenu(GUI_SAVEGAME);
//	//updateSaveLoadWnd();
//	//if(!currentLevel)
//	//	return true;
//	//ZipSaveFile sav("test",saveGamePath,"zip");
//	//sav.openZipFile(true);
//	//sav.addTextFile(currentLevel->getSavegameXML(),"level/"+currentLevel->getFileName(),false);
//	//sav.addTextFile(getSavegameSettings(),"savegame.xml",false);
//
//	////jetzt zeug aus dem tempsave adden
//	//
//	//sav.copyFilesFrom(tempSaveGame);
//	//
//	//
//	//sav.closeZipFile();
//	
//	return true;
//}

//bool GameApp::optionsClick(const CEGUI::EventArgs& e)
//{
//	setMenu(GUI_OPTIONS);
//	return true;
//}


void GameApp::setMenu(MenuMode mId,bool changePausedState)
{
	//if(mEditorGuiSheet)
	//	CEGUI::WindowManager::getSingleton().destroyWindow(mEditorGuiSheet);
	menuMode = mId;
	switch(mId)
	{
	case GUI_DISABLE:
		if(settings.mouselooking)
			setCursorMode(false);
		if(changePausedState)
			setPaused(false);
		
		break;
	case GUI_INGAME:

		mGUISystem->setGUISheet(mIngameGui);
		if(changePausedState)
			setPaused(false);
		if(settings.mouselooking)
			setCursorMode(false);
		break;
	case GUI_MAINMENU:

		mGUISystem->setGUISheet(mMainMenuGui);
		if(changePausedState)
			setPaused(true);
		setCursorMode(true);
		break;
	case GUI_OPTIONS:
		//updateOptionsWindow();
		mGUISystem->setGUISheet(mOptionsGui);
		if(changePausedState)
			setPaused(true);
		setCursorMode(true);
		mOptWnd->updateShownSettings();
		break;
	case GUI_SAVEGAME:
		mSaveLoadMenu->update();
		//updateSaveLoadWnd();
		mGUISystem->setGUISheet(mSavegameGui);
		if(changePausedState)
			setPaused(true);
		setCursorMode(true);
		break;


	}
	

	
}





void GameApp::setDebugText(Ogre::String str,bool add)
{
	mHud->setDebugText(str,add);
}
//
//gamedata_char GameApp::getGameCharData(Ogre::String Id)
//{
////	log("--getGameCharData begin");
//	if(datamap_char.find(Id) != datamap_char.end())
//	{
////		log("--getGameCharData gut");
//		return datamap_char[Id];
//
//	}
//	else
//	{
////		log("--getGameCharData schlecht");
//		gamedata_char temp;
//		temp.id="";
//		return temp;
//	}
////	log("--getGameCharData end");
//}
//
//
//gamedata_char *GameApp::getGameCharDataPtr(Ogre::String Id)
//{
//
//	if(datamap_char.find(Id) != datamap_char.end())
//	{
//		return &datamap_char[Id];
//	}
//	else
//	{
//		return NULL;
//	}
//
//}
void GameApp::showInfoText(Ogre::String txt,Ogre::MovableObject *mObj)
{
	// get the projection of the object's AABB into screen space
	const Ogre::AxisAlignedBox& bbox = mObj->getWorldBoundingBox(true);
	Ogre::Matrix4 mat = currentLevel->getMainCam()->getViewMatrix();

	const Ogre::Vector3* corners = bbox.getAllCorners();

	float min_x = 1.0f, max_x = 0.0f, min_y = 1.0f, max_y = 0.0f;

        // expand the screen-space bounding-box so that it completely encloses
        // the object's AABB
	for (int i=0; i<8; i++) {
		Ogre::Vector3 corner = corners[i];

                // multiply the AABB corner vertex by the view matrix to
                // get a camera-space vertex
		corner = mat * corner;

                // make 2D relative/normalized coords from the view-space vertex
                // by dividing out the Z (depth) factor -- this is an approximation
		float x = corner.x / corner.z + 0.5;
		float y = corner.y / corner.z + 0.5;

		if (x < min_x)
			min_x = x;

		if (x > max_x)
			max_x = x;

		if (y < min_y)
			min_y = y;

		if (y > max_y)
			max_y = y;
	}

	// we now have relative screen-space coords for the object's bounding box; here
	// we need to center the text above the BB on the top edge. The line that defines
	// this top edge is (min_x, min_y) to (max_x, min_y)

	//m_pContainer->setPosition(min_x, min_y);
	showInfoText(txt,min_x,(min_y+1-max_y)/2);
	//m_pContainer->setPosition(1-max_x, min_y);  // Edited by alberts: This code works for me
	//m_pContainer->setDimensions(max_x - min_x, 0.1); // 0.1, just "because"
}
//{
//	//erstmal so:
//	if(player)
//		player->Update(time);
//	if(testguy)
//		testguy->Update(time);
//}

void GameApp::setLoadLevel(Ogre::String filename, Ogre::String entrance)
{
	sllLevel = filename;
	sllEntrance = entrance;
	sll = true;
}
void GameApp::loadLevel(Ogre::String filename, Ogre::String entrance,bool reload,bool placePlayer)
{
//	unpauseWaitTime = 0;

	paused = true;
	unpauseNextFrame = true;
	waitingUntilUnpause = 0;
	if(!placePlayer)
		reload = true;

	GameChar *oldPlayer = NULL;


	if(currentLevel)
	{
		
		if(!reload)
		{
			if(currentLevel->getFileName() == filename)
			{
				//wir sind im selben level. einfach position setzen
				currentLevel->placePlayer(entrance);
				paused = false;
				unpauseNextFrame = false;
				return;
			}
			//das soeben verlassene level sichern!

			tempSaveGame->addTextFile(currentLevel->getSavegameXML(),"level/"+currentLevel->getFileName(),true);

 		//	ZipSaveFile *temp2sav = new ZipSaveFile("tempsave2",tempPath,SAVEGAME_EXTENSION);
			////temp2sav->openZipFile(true);
			//temp2sav->addTextFile(currentLevel->getSavegameXML(),"level/"+currentLevel->getFileName());
			//temp2sav->copyFilesFrom(tempSaveGame);
			//temp2sav->closeZipFile();
			//tempSaveGame->removeFile();
			//temp2sav->renameFile(tempSaveGame->getZipFileName());
			//delete tempSaveGame;
			//tempSaveGame = temp2sav;

//			tempSaveGame->addTextFile(currentLevel->saveToFile("",true),"level/"+currentLevel->getFileName());
//		OgreNewt::Debugger::getSingleton().deInit();
			currentLevel->prepareForDestruction();
			//und ausserdem den Player aus dem alten level mitnehmen
			//das if(!reload) ist deshalb noch hier gültig, weil
			//der player neu erstellt werden muss
			oldPlayer = currentLevel->getPlayer();
		}
		
		
	}

	music->stop();
	ambient1->stop();
	ambient2->stop();
	playersfx->stop();
	
	
	Level *newLevel = new Level(filename,tempSaveGame);
//Debugger::getSingleton().init( newLevel->getSceneManager() );

	Camera *newCam = newLevel->getMainCam();
	Viewport* vp = mWindow->getViewport(0);
	vp->setCamera(newCam);

	// Alter the camera aspect ratio to match the viewport
	newCam->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

	if(placePlayer)
		newLevel->placePlayer(entrance,oldPlayer);
	//jetzt kann sich das alte Lvl auf seinen Tod vorbereiten
	/*if(currentLevel)
		currentLevel->destroyAllObjects();*/
	/*mGUIRenderer->setTargetSceneManager(newLevel->getSceneManager());
	mGUIRenderer->set*/

	oldLevel = currentLevel;




	//nicht jetzt, erst wenn die update() schleife durch ist
	//delete currentLevel;

	currentLevel = newLevel;
	currentLevel->setMusic();
	//unpaused wird es im framelistener
//	log("neues level eigentlich geladen");
	//newLevel->placeObject(GameChar



	//GUITEST
	



}

void GameApp::showScreenText(Ogre::String str)
{
	//CEGUI::Window *textOut = wmgr->getWindow((CEGUI::utf8*)"TextOut");
	//CEGUI::String test = str.c_str();

	mHud->setDebugText(str,false);
	//textOut->setText(test);
	if(str != "")
	{
		
		screenTextShown = true;
		screenTextTime = 0;
	}
	else
	{
		screenTextShown = false;
	}
}


void GameApp::frameEnded(Ogre::Real time)
{
	/*
    if(playerInventoryWnd && playerInventoryWnd->isRequestingToHide())
    {
        showInventory(false);
    }
	*/
    /*if(playerSpellbookWnd && playerSpellbookWnd->isRequestingToHide())
    {
        showSpellbook(false);
    }*/
    /*if(otherInventoryWnd && otherInventoryWnd->isRequestingToHide())
    {
        hideOtherInventory();
    }*/
	if(!DebugDoOnce)
	{
		/*DebugDoOnce = true;
		SimpleSound::SoundManager *test = new SimpleSound::SoundManager();
		SimpleSound::Source *src = test->createSource();
		SimpleSound::Buffer *buff = test->createBuffer("arrow_shoot.wav");
		src->setBuffer(buff);
		src->setPosition(Ogre::Vector3(0,0,-3));
		src->setLoop(true);
		src->play();
		
		test->getListener()->setPosition(Ogre::Vector3::ZERO);
		test->getListener()->setOrientation(Quaternion::IDENTITY);*/

	}
	screenTextTime += time;
	if(screenTextShown && screenTextTime>= SCREENTEXT_TIME)
	{
		hideScreenText();
	}
	if(unpauseNextFrame)
	{
		if(waitingUntilUnpause > 1)//eigentlich 0.85, aber sicher ist sicher //unpauseNextFrame)// && unpauseWaitTime >= 5)
		{
			setPaused(false);
			zeroTime = true;
			unpauseNextFrame = false;
			waitingUntilUnpause = 0;
		}
		else
		{
			waitingUntilUnpause+=time;
		}
	}
	if(otherInventoryView->isShown())
	{
		//schauen ob man nicht zu weit vom offenen objekt gegangen ist
		
		
		
		if(currentLevel->getPlayer()->getPosition().squaredDistance(otherInventoryView->getOwner()->getPosition()) 
			> otherInvSquaredDist+1)
		{
			hideOtherInventory();
		}
	}
	
	if(oldLevel)
	{
		delete oldLevel;
		oldLevel = NULL;
		//log("level gelöscht");
		
	}
	if(sll)
	{
		if(sllLevel != "")
		{
			loadLevel(sllLevel,sllEntrance);
		}
		sllLevel = "";
		sllEntrance = "";
		sll = false;
	}
}

void GameApp::createDefaultSceneMgr()
{
	noLevelMgr = mRoot->createSceneManager(ST_GENERIC);
	noLevelCam = noLevelMgr->createCamera("main");


    Viewport* vp = mWindow->addViewport(noLevelCam);
	vp->setBackgroundColour(ColourValue(0,0,0));
	noLevelCam->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

}

CEGUI::OgreRenderer *GameApp::getCEGUIRenderer()
{
	return mGUIRenderer;
}
//
//bool GameApp::optionsCancelClick(const CEGUI::EventArgs& e)
//{
//	setMenu(GUI_MAINMENU);
//	return true;
//}
//bool GameApp::optionsOKClick(const CEGUI::EventArgs& e)
//{
//	acceptSettings();
//	setMenu(GUI_MAINMENU);
//	return true;
//}
//bool GameApp::optionsAcceptClick(const CEGUI::EventArgs& e)
//{
//	acceptSettings();
//	return true;
//}
//
//void GameApp::acceptSettings()
//{
//	//CEGUI::WindowManager *wmgr = CEGUI::WindowManager::getSingletonPtr();
//	//OGRE
//	CEGUI::ScrollablePane *videoOptionTab = static_cast<ScrollablePane*>(wmgr->getWindow("VideoOptionsScroll"));
//	CEGUI::Combobox *renderSelect = static_cast<CEGUI::Combobox*>(wmgr->getWindow("renderSelect"));
//	
//	selectedRenderer = mRoot->getRenderSystemByName(renderSelect->getText().c_str());
//	//Ogre::RenderSystem *rs = mRoot->getRenderSystemByName(selectedRenderer);
//	for(unsigned int i=0;i<videoOptionTab->getContentPane()->getChildCount();i++)
//	{
//		Window *curElem = videoOptionTab->getContentPane()->getChildAtIdx(i);
//		CEGUI::String cType = curElem->getType();
//		if(cType == CEGUI::String("LandsLook/Combobox"))
//		{
//			CEGUI::String name = curElem->getName();
//			CEGUI::String value = curElem->getText();
//
//			if(name != CEGUI::String("renderSelect") && name != CEGUI::String(""))
//			{
//				selectedRenderer->setConfigOption(name.c_str(),value.c_str());
//			}
//
//
//
//		}
//	}
//	//saveOgreConfig();
//	//audio
//
//	/*gainAll = getScrollbarPos((CEGUI::Scrollbar*)(wmgr->getWindow("options/gainAll")));
//
//	gainMusic = getScrollbarPos((CEGUI::Scrollbar*)(wmgr->getWindow("options/gainMusic")));
//	gainSFX = getScrollbarPos((CEGUI::Scrollbar*)(wmgr->getWindow("options/gainSFX")));
//	gainSFXPlayer = getScrollbarPos((CEGUI::Scrollbar*)(wmgr->getWindow("options/gainSFXPlayer")));
//	gainSpeech = getScrollbarPos((CEGUI::Scrollbar*)(wmgr->getWindow("options/gainSpeech")));*/
//
//	
//	soundMgr->setTypeGain(settings.gain_music,stMusic);
//	
//	soundMgr->setTypeGain(settings.gain_sfx,stSfxLevel);
//	soundMgr->setTypeGain(settings.gain_player_sfx,stSfxPlayer);
//	soundMgr->setTypeGain(settings.gain_speech,stSpeech);
//
//	soundMgr->getListener()->setGain(settings.gain_all);
//
//	
//	settings.distance_model= parseDistanceModel( wmgr->getWindow("options/distance_model")->getText().c_str() );
//	soundMgr->setDistanceModel(settings.distance_model);
//
//	settings.max_distance = StringConverter::parseReal(wmgr->getWindow("options/maxdist")->getText().c_str());
//	settings.rolloff_factor = StringConverter::parseReal(wmgr->getWindow("options/rolloff")->getText().c_str());
//	settings.ref_distance = StringConverter::parseReal(wmgr->getWindow("options/refdist")->getText().c_str());
//	soundMgr->setDefaultMaxDist(settings.max_distance);
//	soundMgr->setDefaultRefDistance(settings.ref_distance);
//	soundMgr->setDefaultRolloffFactor(settings.rolloff_factor);
//	//gamesettings
//	/*sensibility_x = getScrollbarPos((CEGUI::Scrollbar*)(wmgr->getWindow("options/sens_x")))*2;
//	sensibility_y = getScrollbarPos((CEGUI::Scrollbar*)(wmgr->getWindow("options/sens_y")))*2;
//	Ogre::Real phyTemp = getScrollbarPos((CEGUI::Scrollbar*)(wmgr->getWindow("options/physicval")))*540 + 60;*/
////physicsPrecision = 1.0f / phyTemp;
////	mLog("phytemp="+str(phyTemp));
//	settings.mouselooking = static_cast<CEGUI::Checkbox*>(wmgr->getWindow("options/mouselooking"))->isSelected();
//	
//
//}
//
//void GameApp::saveOgreConfig()
//{
//
//	std::ofstream of(mConfigFileName.c_str());
//	if (!of)
//		OGRE_EXCEPT(Ogre::Exception::ERR_CANNOT_WRITE_TO_FILE, "Cannot create settings file.",
//            "GameApp::saveConfigFile");
//	of << "Render System=" << newRenderer << std::endl;
//
//	for (RenderSystemList::const_iterator pRend = mRoot->getAvailableRenderers()->begin(); pRend != mRoot->getAvailableRenderers()->end(); ++pRend)
//    {
//        RenderSystem* rs = *pRend;
//        of << std::endl;
//        of << "[" << rs->getName() << "]" << std::endl;
//        const ConfigOptionMap& opts = rs->getConfigOptions();
//        for (ConfigOptionMap::const_iterator pOpt = opts.begin(); pOpt != opts.end(); ++pOpt)
//        {
//			of << pOpt->first << "=" << pOpt->second.currentValue << std::endl;
//        }
//    }
//
//    of.close();
//
///****** stolen from OgreRoot.cpp
//
//	void Root::saveConfig(void)
//    {
//		std::ofstream of(mConfigFileName.c_str());
//
//        if (!of)
//            OGRE_EXCEPT(Exception::ERR_CANNOT_WRITE_TO_FILE, "Cannot create settings file.",
//            "Root::saveConfig");
//
//        if (mActiveRenderer)
//        {
//            of << "Render System=" << mActiveRenderer->getName() << std::endl;
//        }
//        else
//        {
//            of << "Render System=" << std::endl;
//        }
//
//        for (RenderSystemList::const_iterator pRend = getAvailableRenderers()->begin(); pRend != getAvailableRenderers()->end(); ++pRend)
//        {
//            RenderSystem* rs = *pRend;
//            of << std::endl;
//            of << "[" << rs->getName() << "]" << std::endl;
//            const ConfigOptionMap& opts = rs->getConfigOptions();
//            for (ConfigOptionMap::const_iterator pOpt = opts.begin(); pOpt != opts.end(); ++pOpt)
//            {
//				of << pOpt->first << "=" << pOpt->second.currentValue << std::endl;
//            }
//        }
//
//        of.close();
//
//    }
//*/
//	////restore config:
//	/*
//        // Restores configuration from saved state
//        // Returns true if a valid saved configuration is
//        //   available, and false if no saved config is
//        //   stored, or if there has been a problem
//        ConfigFile cfg;
//        //RenderSystemList::iterator pRend;
//
//        try {
//            // Don't trim whitespace
//            cfg.load(mConfigFileName, "\t:=", false);
//        }
//        catch (Exception& e)
//        {
//            if (e.getNumber() == Exception::ERR_FILE_NOT_FOUND)
//            {
//                return false;
//            }
//            else
//            {
//                throw;
//            }
//        }
//
//        ConfigFile::SectionIterator iSection = cfg.getSectionIterator();
//        while (iSection.hasMoreElements())
//        {
//            const String& renderSystem = iSection.peekNextKey();
//            const ConfigFile::SettingsMultiMap& settings = *iSection.getNext();
//
//            RenderSystem* rs = getRenderSystemByName(renderSystem);
//            if (!rs)
//            {
//                // Unrecognised render system
//                continue;
//            }
//
//            ConfigFile::SettingsMultiMap::const_iterator i;
//            for (i = settings.begin(); i != settings.end(); ++i)
//            {
//                rs->setConfigOption(i->first, i->second);
//            }
//        }
//
//        RenderSystem* rs = getRenderSystemByName(cfg.getSetting("Render System"));
//        if (!rs)
//        {
//            // Unrecognised render system
//            return false;
//        }
//
//        setRenderSystem(rs);
//
//        // Successful load
//        return true;
//		*/
//
//
//}
//
//void GameApp::updateVideoWindow()
//{
//			//TabControl* optionTabs = static_cast<TabControl*>(wmgr.getWindow("OptionTabs"));
////	log("updateOptionsWindow begin WTF");
//	//CEGUI::WindowManager *wmgr = CEGUI::WindowManager::getSingletonPtr();
////	log("nach singleton ptr");
//	CEGUI::ScrollablePane *videoOptionTab = static_cast<ScrollablePane*>(wmgr->getWindow("VideoOptionsScroll"));
////	log("childcount ="+StringConverter::toString(videoOptionTab->getChildCount()));
////	log("childcount contentpane="+StringConverter::toString(videoOptionTab->getContentPane()->getChildCount()));
//	size_t cCount = videoOptionTab->getContentPane()->getChildCount();
//	size_t delIndex = 0;
//	if(cCount > 0)
//	{
//		for(size_t i=0;i<cCount;i++)
//		{
//			CEGUI::String curName = videoOptionTab->getContentPane()->getChildAtIdx(delIndex)->getName();
//			if(curName != CEGUI::String("renderSelectLabel") && curName != CEGUI::String("renderSelect"))
//			{
////				log(Ogre::String("lösche ")+curName.c_str());
//				/*videoOptionTab->getContentPane()->getChildAtIdx(0)->destroy();*/
//				wmgr->destroyWindow(curName);
//			}
//			else
//			{
////				log(Ogre::String("lösche NICHT ")+curName.c_str());
//				delIndex++;
//
//			}
//		}
//	}
//
//
//	//add the renderer dropdown menu
//	bool hadRenderMenu = true;
//
//	if(!wmgr->isWindowPresent("renderSelectLabel"))
//	{
//
//		CEGUI::Window *rendererText =wmgr->createWindow("LandsLook/StaticText","renderSelectLabel");
//		videoOptionTab->addChildWindow(rendererText);
//		rendererText->setArea(URect(UDim(0,20),UDim(0,5),UDim(0.5,-20),UDim(0,30)));
//		//curText->setArea(URect(UDim(0,20),UDim(0,70+40*hOffset),UDim(0.5,-20),UDim(0,100+40*hOffset)));
//		rendererText->setText("Renderer:");
//	}
//	CEGUI::Combobox *rendererDropdown = NULL;
//	if(!wmgr->isWindowPresent("renderSelect"))
//	{
//		rendererDropdown = static_cast<CEGUI::Combobox*>(wmgr->createWindow("LandsLook/Combobox","renderSelect"));
//		videoOptionTab->addChildWindow(rendererDropdown);
//		rendererDropdown->setReadOnly(true);
//		rendererDropdown->setArea(URect(UDim(0.5,20),UDim(0,5),UDim(1,-20),UDim(0,175)));
//		//quitButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameApp::handleQuit, this));
//		hadRenderMenu = false;
//		rendererDropdown->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted,Event::Subscriber(&GameApp::optionsRendererChanged, this));
//	}
//	else
//	{
//		rendererDropdown = static_cast<CEGUI::Combobox*>(wmgr->getWindow("renderSelect"));
//	}
//	Ogre::RenderSystem *cRS = NULL;
//
//	if(!hadRenderMenu)
//	{
//		cRS = mRoot->getRenderSystem();
//
//		rendererDropdown->setText(cRS->getName().c_str());
//
//		Ogre::RenderSystemList rlist = mRoot->getAvailableRenderers();
////		log("vor schleife");
//		for(RenderSystemList::iterator itr = rlist.begin();itr!=rlist.end();itr++)
//		{
//			Ogre::RenderSystem *rs = *itr;
//			Ogre::String name = rs->getName();
//			CEGUI::ListboxTextItem *renderItem = new ListboxTextItem(name.c_str());
//			renderItem->setSelectionBrushImage("LandsLook","TextSelectionBrush");
//			rendererDropdown->addItem(renderItem);
//		}
////		log("nach renderer schleife");
//	}
//	else
//	{
//		Ogre::String curSelName =  rendererDropdown->getText().c_str();
////		log("auswahl: "+curSelName);
//		cRS = mRoot->getRenderSystemByName(curSelName);
//
//	}
//	ConfigOptionMap options = cRS->getConfigOptions();
//	int hOffset = 0;
//	for(ConfigOptionMap::iterator it2 = options.begin();it2!=options.end();it2++)
//	{
//		//create dropdown menus for all the options
//
//
//		Ogre::String name = it2->first;
//		ConfigOption opt = it2->second;
//
////		log("-option "+name);
////		log("--currentVal="+opt.currentValue);
////		log("--immutable="+StringConverter::toString(opt.immutable));
////		log("--name="+opt.name);
////		log("--possibleValues=");
//
//		CEGUI::Window *curText =wmgr->createWindow("LandsLook/StaticText");
//		videoOptionTab->addChildWindow(curText);
//		curText->setArea(URect(UDim(0,20),UDim(0,70+40*hOffset),UDim(0.5,-20),UDim(0,95+40*hOffset)));
//		curText->setText(name.c_str());
//
////		log("vor curCB");
//		if(wmgr->isWindowPresent(name.c_str()))
//		{
////			log("mwaha");
//			wmgr->destroyWindow(name.c_str());
//		}
//		CEGUI::Combobox *curCB = static_cast<CEGUI::Combobox*>(wmgr->createWindow("LandsLook/Combobox",name.c_str()));
////		log("nach curCB");
//		videoOptionTab->addChildWindow(curCB);
//		curCB->setReadOnly(true);
//		curCB->setArea(URect(UDim(0.5,20),UDim(0,70+40*hOffset),UDim(1,-20),UDim(0,200+50*hOffset)));
//		curCB->setText(opt.currentValue.c_str());
//
//		//adding values
//
//
//
//
//
//		for(Ogre::StringVector::iterator it3 = opt.possibleValues.begin();it3 != opt.possibleValues.end();it3++)
//		{
//			Ogre::String val = *it3;
//			CEGUI::ListboxTextItem *item = new ListboxTextItem(val.c_str());
//			item->setSelectionBrushImage("LandsLook","TextSelectionBrush");
//			curCB->addItem(item);
////			log("---"+val);
//
//		}
//		hOffset++;
//	}
//
//}

//bool GameApp::optionsRendererChanged(const CEGUI::EventArgs& e)
//{
//	/*CEGUI::Combobox *ren = static_cast<CEGUI::Combobox*>(CEGUI::WindowManager::getSingletonPtr()->getWindow("renderSelect"));
//	Ogre::String rs = ren->getName().c_str();
//	if(rs != curSelectedRenderer)
//	{
//		updateOptionsWindow(mRoot->getRenderSystemByName(curSelectedRenderer.c_str()));
//	}*/
//	updateVideoWindow();
//	return true;
//}

void GameApp::showSpellbook(bool show)
{
    if(show)
    {
  //      //geht nicht wenn kontainer offen ist
		//if(otherInvOpen)
		//{
		//	//spellbookOpen = false;
		//	return;
		//}
		hideInfoText();
		//unsigned int rowLength = INV_ROW_LENGTH;
		Spellbook *sb = currentLevel->getPlayer()->getSpellbook();
		if(sb)
		{
			setCursorMode(true);
			playerSpellbookView->show(sb);
			//playerSpellbookView->setSpellbook(sb);
            //playerSpellbookWnd->show(sb);
			/*playerSpellbookWnd->enable();
			playerSpellbookWnd->setVisible(true);
			updateSpellbook(sb);*/
		}//ende von if(inv)
		
    }
    else
    {
		if(settings.mouselooking && !playerInventoryView->isShown())
		{
			setCursorMode(false);			
		}
        playerSpellbookView->hide();
    }
}

void GameApp::toggleSpellbook()
{
    //this->playerSpellbookWnd->isVisible());
	showSpellbook(!playerSpellbookView->isShown());
	


}
//
//void GameApp::updatePlayerInventory(Inventory *inv)
//{
//	if(inv)
//	{
//		unsigned int invSize = inv->getMaxSize();
//		//if(playerInventoryFieldsCnt != invSize)
//			updatePlayerInventoryWindow();
//		
//		
//
//		unsigned int x = 0;
//		unsigned int y = 0;
//		unsigned int rowLength = INV_ROW_LENGTH;
//		//CEGUI::ImagesetManager *imgr = CEGUI::ImagesetManager::getSingletonPtr();
//		//std::vector<Inventory::invItem>::iterator itr;
//		//for(itr = inv->itemVector.begin();itr != inv->itemVector.end();itr++)
//		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//		//!!!!!!DAS GEGEN ITERATOR MIT i++ IN DER SCHLEFE ERSETZEN!!!!!!
//		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//		for(size_t i = 0;i < invSize;i++)
//		{
//			
//			//alle items anzeigen
//			Inventory::invDataItem cur = inv->getItemAt(i);//*itr;
//			
//			updateInventoryField(x,y,cur);
//			x++;
//			if(x % rowLength == 0)
//			{
//				x = 0;
//				y++;
//			}
//		}
//	}
//	else
//	{
//		//if(playerInventoryFieldsCnt != 0)		
//			updatePlayerInventoryWindow();
//		
//	}
//}
//void GameApp::updateOtherInventory(Inventory *inv)
//{
//	if(inv)
//	{
//		unsigned int invSize = inv->getMaxSize();
//		if(invSize != otherInventorySize)
//			updateOtherInventoryWindow();
//		
//		
//
//		
//
//		unsigned int x = 0;
//		unsigned int y = 0;
//
//		//CEGUI::ImagesetManager *imgr = CEGUI::ImagesetManager::getSingletonPtr();
//		//std::vector<Inventory::invItem>::iterator itr;
//		//for(itr = inv->itemVector.begin();itr != inv->itemVector.end();itr++)
//		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//		//!!!!!!DAS GEGEN ITERATOR MIT i++ IN DER SCHLEFE ERSETZEN!!!!!!
//		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//		unsigned int rowLength = INV_ROW_LENGTH;
//		for(size_t i = 0;i < invSize;i++)
//		{
//			
//			//alle items anzeigen
//			Inventory::invDataItem cur = inv->getItemAt(i);//*itr;
//			
//			updateOtherInvField(x,y,cur);
//			x++;
//			if(x % rowLength == 0)
//			{
//				x = 0;
//				y++;
//			}
//		}
//		
//	}
//	else
//	{
//		if(otherInventorySize != 0)
//			updateOtherInventoryWindow();
//
//	}
//}
//void GameApp::updateSpellbook(Spellbook *sb)
//{
//	if(sb)
//	{
//		unsigned int size = sb->getSpellCnt();
//		if(spellbookSize != size)
//			updateSpellbookWindow();
//		
//		
//
//		unsigned int x = 0;
//		unsigned int y = 0;
//		
//		//CEGUI::ImagesetManager *imgr = CEGUI::ImagesetManager::getSingletonPtr();
//		//std::vector<Inventory::invItem>::iterator itr;
//		//for(itr = inv->itemVector.begin();itr != inv->itemVector.end();itr++)
//		unsigned int rowLength = INV_ROW_LENGTH;
//		for(size_t i = 0;i < spellbookSize;i++)
//		{
//			
//			
//			gamedata_spell* cur = sb->getAt(i);
//			
//			updateSpellbookField(x,y,cur);
//			x++;
//			if(x % rowLength == 0)
//			{
//				x = 0;
//				y++;
//			}
//		}
//	}
//	else
//	{
//		if(spellbookSize != 0)
//			updateSpellbookWindow();
//	}
//}

//void GameApp::inventoryCallback(bool show)
//{
////   showEquipmentWindow(show);
//    if(!show && otherInventoryWnd->isOpen())
//        otherInventoryWnd->hide();
//}

//void GameApp::showEquipmentWindow(bool show)
//{
//    if(show)
//    {
//        playerEquipWnd->enable();
//		playerEquipWnd->setVisible(true);
//        updateDisplayedStats();
//        GameChar *player = currentLevel->getPlayer();
//		//alle felder holen
//		CEGUI::Window *melee = wmgr->getWindow("PlayerInventory/melee");
//		CEGUI::Window *ranged = wmgr->getWindow("PlayerInventory/ranged");
//		CEGUI::Window *armor = wmgr->getWindow("PlayerInventory/armor");
//		CEGUI::Window *shield = wmgr->getWindow("PlayerInventory/shield");
//
//		CEGUI::Window *else1 = wmgr->getWindow("PlayerInventory/else1");
//		CEGUI::Window *else2 = wmgr->getWindow("PlayerInventory/else2");
//		CEGUI::Window *else3 = wmgr->getWindow("PlayerInventory/else3");
//		CEGUI::Window *else4 = wmgr->getWindow("PlayerInventory/else4");
//
//		CEGUI::Window *bag   = wmgr->getWindow("PlayerInventory/bag");
//
//		//CEGUI::ImagesetManager *imgr = CEGUI::ImagesetManager::getSingletonPtr();
//
//		//bildnamen aller items setzen
//		//melee
//		CEGUI::String meleeFileName = "";
//		CEGUI::String meleeToolTip = "";
//		gamedata_item *item = player->getMelee();
//		if(item)
//		{
//			meleeFileName = item->inv_image.c_str();
//			meleeToolTip = item->name.c_str();
//		}
//		else
//		{
//			meleeFileName = ICON_MELEE;
//		}	
//		//ranged
//		CEGUI::String rangedFileName = "";
//		CEGUI::String rangedToolTip = "";
//		item = player->getRanged();
//		if(item)
//		{
//			rangedFileName = item->inv_image.c_str();
//			rangedToolTip = item->name.c_str();
//		}
//		else
//		{
//			rangedFileName = ICON_RANGED;
//		}	
//		//armor
//		CEGUI::String armorFileName = "";
//		CEGUI::String armorToolTip = "";
//		item = player->getArmor();
//		if(item)
//		{
//			armorFileName = item->inv_image.c_str();
//			armorToolTip = item->name.c_str();
//		}
//		else
//		{
//			armorFileName = ICON_ARMOR;
//		}		
//		//shield
//		CEGUI::String shieldFileName = "";
//		CEGUI::String shieldToolTip = "";
//		item = player->getShield();
//		if(item)
//		{
//			shieldFileName = item->inv_image.c_str();
//			shieldToolTip = item->name.c_str();
//		}
//		else
//		{
//			shieldFileName = ICON_SHIELD;
//		}		
//		//else1
//		CEGUI::String else1FileName = "";
//		CEGUI::String else1ToolTip = "";
//		item = player->getElse1();
//		if(item)
//		{
//			else1FileName = item->inv_image.c_str();
//			else1ToolTip = item->name.c_str();
//		}
//		else
//		{
//			else1FileName = ICON_ELSE;
//		}		
//		//else2
//		CEGUI::String else2FileName = "";
//		CEGUI::String else2ToolTip = "";
//		item = player->getElse2();
//		if(item)
//		{
//			else2FileName = item->inv_image.c_str();
//			else2ToolTip = item->name.c_str();
//		}
//		else
//		{
//			else2FileName = ICON_ELSE;
//		}		
//		//else3
//		CEGUI::String else3FileName = "";
//		CEGUI::String else3ToolTip = "";
//		item = player->getElse3();
//		if(item)
//		{
//			else3FileName = item->inv_image.c_str();
//			else3ToolTip = item->name.c_str();
//		}
//		else
//		{
//			else3FileName = ICON_ELSE;
//		}		
//		//else4
//		CEGUI::String else4FileName = "";
//		CEGUI::String else4ToolTip = "";
//		item = player->getElse4();
//		if(item)
//		{
//			else4FileName = item->inv_image.c_str();
//			else4ToolTip = item->name.c_str();
//		}
//		else
//		{
//			else4FileName = ICON_ELSE;
//		}		
//		//tasche
//		CEGUI::String bagFileName = "";
//		CEGUI::String bagToolTip = "";
//		gamedata_item *bag_item = player->getBag();
//		if(bag_item)
//		{
//			bagFileName = bag_item->inv_image.c_str();
//			bagToolTip = bag_item->name.c_str();
//		}
//		else
//		{
//			bagFileName = ICON_BAG;
//		}		
//		//bilder setzen
//				ceguiSetImage(melee,meleeFileName);
//		melee->setTooltipText(meleeToolTip);
//		ceguiSetImage(ranged,rangedFileName);
//		ranged->setTooltipText(rangedToolTip);
//		ceguiSetImage(armor,armorFileName);
//		armor->setTooltipText(armorToolTip);
//		ceguiSetImage(shield,shieldFileName);
//		shield->setTooltipText(shieldToolTip);
//		ceguiSetImage(else1,else1FileName);
//		else1->setTooltipText(else1ToolTip);
//		ceguiSetImage(else2,else2FileName);
//		else2->setTooltipText(else2ToolTip);
//		ceguiSetImage(else3,else3FileName);
//		else3->setTooltipText(else3ToolTip);
//		ceguiSetImage(else4,else4FileName);
//		else4->setTooltipText(else4ToolTip);
//		ceguiSetImage(bag,bagFileName);
//		bag->setTooltipText(bagToolTip);
//    }
//    else
//    {
//        playerEquipWnd->disable();
//		playerEquipWnd->setVisible(false);
//    }
//}
void GameApp::showInventory(bool show)
{
    if(show)
    {
        hideInfoText();
		setCursorMode(true);		

		Inventory *inv = currentLevel->getPlayer()->getInventory();
		if(inv)
		{
			//playerInventoryView->setInventory(inv);
			playerInventoryView->show(inv);

			
			
			/*updatePlayerInventory(inv);*/
		}//ende von if(inv)
		
		//hier ausrüstung anzeigen

//	showEquipmentWindow(true);

    }
    else
    {
        
        //wenn das 2. inv offen ist, es mitschließen
        //showEquipmentWindow(false); das nun per CALLBACK!
		if(otherInventoryView->isShown())
		{
			hideOtherInventory();
		}
		
		if(settings.mouselooking && !playerSpellbookView->isShown())
		{
			setCursorMode(false);
		}
//       showEquipmentWindow(false);
        playerInventoryView->hide();
		/*playerInventoryWnd->disable();
		playerInventoryWnd->setVisible(false);*/
    }
}

void GameApp::toggleInventory()
{
	//inventoryOpen = ;//!(playerInventoryWnd->isVisible());
	//toggle machen
		
	showInventory(!playerInventoryView->isShown());		
	
}

void GameApp::updateDisplayedStats()
{
	GameChar *player = currentLevel->getPlayer();
	Stats st = player->getStats();
	wmgr->getWindow("stat_strength")->setText(TypeConverter::ogre_str(st.mStats[st_strength]).c_str()); 
	wmgr->getWindow("stat_agility")->setText(TypeConverter::ogre_str(st.mStats[st_agility]).c_str());
	wmgr->getWindow("stat_magic")->setText(TypeConverter::ogre_str(st.mStats[st_magic]).c_str());
	wmgr->getWindow("stat_maxHP")->setText(TypeConverter::ogre_str(st.mStats[st_maxHP]).c_str());
	wmgr->getWindow("stat_maxMP")->setText(TypeConverter::ogre_str(st.mStats[st_maxMP]).c_str());	
	wmgr->getWindow("stat_resistBlunt")->setText(TypeConverter::ogre_str(st.mStats[st_resistBlunt]).c_str());	
	wmgr->getWindow("stat_resistCut")->setText(TypeConverter::ogre_str(st.mStats[st_resistCut]).c_str());		
	wmgr->getWindow("stat_resistPierce")->setText(TypeConverter::ogre_str(st.mStats[st_resistPierce]).c_str());	
	wmgr->getWindow("stat_resistFire")->setText(TypeConverter::ogre_str(st.mStats[st_resistFire]).c_str());	
	wmgr->getWindow("stat_resistIce")->setText(TypeConverter::ogre_str(st.mStats[st_resistIce]).c_str());		
	wmgr->getWindow("stat_resistSpark")->setText(TypeConverter::ogre_str(st.mStats[st_resistSpark]).c_str());
	wmgr->getWindow("stat_resistPoison")->setText(TypeConverter::ogre_str(st.mStats[st_resistPoison]).c_str());
	wmgr->getWindow("stat_resistDark")->setText(TypeConverter::ogre_str(st.mStats[st_resistDark]).c_str());
	wmgr->getWindow("stat_resistLight")->setText(TypeConverter::ogre_str(st.mStats[st_resistLight]).c_str());
	wmgr->getWindow("stat_hpRegenRate")->setText(TypeConverter::ogre_str(st.mStats[st_hpRegenRate]).c_str());
	wmgr->getWindow("stat_manaRegenRate")->setText(TypeConverter::ogre_str(st.mStats[st_manaRegenRate]).c_str());
}

void GameApp::setDisplayedWeapon(gamedata_item* item)
{
	mHud->setDisplayedWeapon(item);
	/*if(item == curDisplayedWeapon)
		return;

	curDisplayedWeapon = item;
	if(item)
		ceguiSetImage(wmgr->getWindow("currentweapon"),item->inv_image.c_str());
	else
		ceguiSetImage(wmgr->getWindow("currentweapon"),"");*/

}

void GameApp::setDisplayedSpell(gamedata_spell* spell)
{
	mHud->setDisplayedSpell(spell);
	/*if(spell == curDisplayedSpell)
		return;

	curDisplayedSpell = spell;
	if(spell)
		ceguiSetImage(wmgr->getWindow("currentspell"),spell->inv_image.c_str());
	else
		ceguiSetImage(wmgr->getWindow("currentspell"),"");*/

}

void GameApp::gameOver()
{
	CEGUI::PushButton* resumeButton = (CEGUI::PushButton*)wmgr->getWindow((CEGUI::utf8*)"resume");
	resumeButton->disable();
    
	resetApp();
	/*if(playerInventoryView->isShown())
		showInventory(false);
    if(playerSpellbookView->isShown())
		showSpellbook(false);
    if(otherInventoryView->isShown())
		hideOtherInventory();

	curHolding->clear();

	setDisplayedSpell(NULL);
	setDisplayedWeapon(NULL);*/
	
	setMenu(GUI_MAINMENU);
}

void GameApp::setCursorMode(bool enable)
{
	cursorMode = enable;
	/*fadenkreuz: MouseTarget*/
	CEGUI::MouseCursor *mouse = CEGUI::MouseCursor::getSingletonPtr();
	/*mGUISystem->setDefaultMouseCursor((CEGUI::utf8*)"TaharezLook", (CEGUI::utf8*)"MouseArrow");
	CEGUI::MouseCursor::getSingleton().setImage("TaharezLook", "MouseMoveCursor");*/
	if(cursorMode)
	{
		mGUISystem->setDefaultMouseCursor((CEGUI::utf8*)"LandsLook", (CEGUI::utf8*)"MouseArrow");
		mouse->setImage("LandsLook", "MouseArrow");
		if(!curHolding->isEmpty())
			curHolding->show();	
		
	}
	else
	{
		
		mGUISystem->getDefaultTooltip()->hide();
		mouse->setPosition(CEGUI::Point(mWindow->getWidth()/2,mWindow->getHeight()/2));
		mGUISystem->setDefaultMouseCursor((CEGUI::utf8*)"LandsLook", (CEGUI::utf8*)"MouseTarget");
		mouse->setImage("LandsLook", "MouseTarget");
		curHolding->hide();	
	}
	////CEGUI::MouseCursor::getSingleton().setVisible(enable);
	//if(cursorMode && curHolding->hasItem() || curHolding->hasSpell())
	//{
	//	holdingItemWnd->show();
	//}
}

//void GameApp::updateSpellbookWindow()
//{
//	unsigned int rowLength = INV_ROW_LENGTH;
//	Spellbook *sb = currentLevel->getPlayer()->getSpellbook();
//	unsigned int sbSize = 0;
//	if(sb)
//	{
//		sbSize = sb->getSpellCnt();
//	}
//		
//	
//	unsigned int x = 0;
//	unsigned int y = 0;
//	//CEGUI::WindowManager *wmgr = CEGUI::WindowManager::getSingletonPtr();
//	CEGUI::Window *invpane = wmgr->getWindow((CEGUI::utf8*)"PlayerSpellbook/contentPane");
//	if(wmgr->isWindowPresent("playerSpell0_0"))
//	{
//		CEGUI::Window *badchild = wmgr->getWindow("playerSpell0_0");
//		while(badchild)
//		{
//			
//			wmgr->destroyWindow(badchild);
//			x++;
//			if(x % rowLength == 0)
//			{
//				x = 0;
//				y++;
//			}
//			Ogre::String cname = "playerSpell"+str(x)+"_"+str(y);
//
//			if(wmgr->isWindowPresent(cname.c_str()))
//			{
//				badchild = wmgr->getWindow(cname.c_str());
//			}
//			else
//			{
//				badchild = NULL;
//			}
//
//
//		}
//
//	}
//
//	if(sb)
//	{
//		
//		
//		
//		//das erzeugt erstmal alle felder
//		x = y = 0;
//		Ogre::Real imgsize = INV_IMAGESIZE;
//		for(unsigned int i=0;i<sbSize;i++)
//		{
//			Ogre::String fieldname = "playerSpell"+str(x)+"_"+str(y);
//			CEGUI::Window *itest = wmgr->createWindow("LandsLook/StaticImage",(CEGUI::utf8*)fieldname.c_str());
//			itest->setSize(CEGUI::UVector2(CEGUI::UDim(0,imgsize),CEGUI::UDim(0,imgsize)));
//			invpane->addChildWindow(itest);
//			itest->setPosition(CEGUI::UVector2(CEGUI::UDim(0,imgsize*x),CEGUI::UDim(0,imgsize*y)));
//			itest->setID(i);
//			itest->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&GameApp::playerSpellbookClick, this));
//			
//			x++;
//			if(x % rowLength == 0)
//			{
//				x = 0;
//				y++;
//			}
//		}
//		spellbookSize = sbSize;
//	}//von if(inv)
//	else
//	{
//		spellbookSize = 0;
//	}
//	
//}
//--------------------------------------------------------------------------------------------------------
//bool GameApp::invWindowResize(const CEGUI::EventArgs& e)
//{
//    const CEGUI::WindowEventArgs& we = 
//		static_cast<const CEGUI::WindowEventArgs&>(e);
//    if(we.window->getName() == "PlayerInventory")
//    {
//        updatePlayerInventoryWindow();
//    }
//    return true;
//}
//--------------------------------------------------------------------------------------------------------
//void GameApp::updatePlayerInventoryWindow()
//{
//	
//    //nun hole ich mal die fensterdimensionen
//    CEGUI::Window *invpane = wmgr->getWindow((CEGUI::utf8*)"PlayerInventory/contentPane");
//    CEGUI::Rect dims = invpane->getInnerRect();//invpane->getPixelRect();
//    Ogre::Real wWidth = dims.getWidth();
//    Ogre::Real wHeight = dims.getHeight();
//    mLog("w="+str(wWidth)+", h="+str(wHeight));
//    //nun berechnen, wie viele reihen und spalten
//    unsigned int rowLength = Math::IFloor(wWidth / INV_IMAGESIZE);
//    unsigned int numRows = Math::IFloor(wHeight / INV_IMAGESIZE);
//	Inventory *inv = currentLevel->getPlayer()->getInventory();
//	unsigned int invSize = rowLength*numRows;
//	//mLog("Alte Invgröße="+str(playerInventoryFieldsCnt));
//    //mLog("Neue Invgröße="+str(invSize));
//    if(rowLength == playerInventoryFields_x && numRows == playerInventoryFields_y)
//        return;//muss nix gemacht werden
//    
//	//playerInventoryFieldsCnt = invSize;
//	playerInventoryFields_x = rowLength;
//    playerInventoryFields_y = numRows;
//	
//	
//	//CEGUI::WindowManager *wmgr = CEGUI::WindowManager::getSingletonPtr();
//    //nun 2 schleifen
//    int invIndex = 0;//diese IDs bleiben fest, beim Klicken wird daraus und aus der scrollpos das echte index bestimmt
//    for(int y=0;y<numRows;y++)
//    {
//        for(int x=0;x<rowLength;x++)
//        {
//            //schauen ob das feld existiert, wenn ja, es repositionieren
//            Ogre::String cname = "playerInv"+str(x)+"_"+str(y);
//            CEGUI::Window *curField = NULL;
//            if(wmgr->isWindowPresent(cname.c_str()))
//			{
//				curField = wmgr->getWindow(cname.c_str());
//			}
//			else
//			{
//                //erstellen
//				
//                curField = wmgr->createWindow("LandsLook/StaticImage",(CEGUI::utf8*)cname.c_str());
//                invpane->addChildWindow(curField);
//                Ogre::String fieldname = "playerInvCnt"+str(x)+"_"+str(y);
//                mLog("erzeuge Feld "+cname);
//			    CEGUI::Window *cntfield = wmgr->createWindow("LandsLook/StaticText",(CEGUI::utf8*)fieldname.c_str());
//			    cntfield->setSize(CEGUI::UVector2(CEGUI::UDim(0,32),CEGUI::UDim(0,16)));
//			    cntfield->setText("");
//			    curField->addChildWindow(cntfield);
//			    cntfield->setPosition(CEGUI::UVector2(CEGUI::UDim(1,-34),CEGUI::UDim(1,-16)));
//			    cntfield->setProperty("FrameEnabled","False");
//			    cntfield->setProperty("BackgroundEnabled","False");
//			    cntfield->setProperty("VertFormatting","TopAligned");
//			    cntfield->setProperty("HorzFormatting","RightAligned");
//                /*CEGUI::Window *itest = wmgr->createWindow("LandsLook/StaticImage",(CEGUI::utf8*)fieldname.c_str());
//			itest->setSize(CEGUI::UVector2(CEGUI::UDim(0,imgsize),CEGUI::UDim(0,imgsize)));
//			invpane->addChildWindow(itest);
//			itest->setPosition(CEGUI::UVector2(CEGUI::UDim(0,imgsize*x),CEGUI::UDim(0,imgsize*y)));
//			itest->setID(i);
//			itest->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&GameApp::playerInventoryClick, this));
//			
//			fieldname = "playerInvCnt"+str(x)+"_"+str(y);
//			CEGUI::Window *cntfield = wmgr->createWindow("LandsLook/StaticText",(CEGUI::utf8*)fieldname.c_str());
//			cntfield->setSize(CEGUI::UVector2(CEGUI::UDim(0,32),CEGUI::UDim(0,16)));
//			cntfield->setText("");
//			itest->addChildWindow(cntfield);
//			cntfield->setPosition(CEGUI::UVector2(CEGUI::UDim(1,-34),CEGUI::UDim(1,-16)));
//			cntfield->setProperty("FrameEnabled","False");
//			cntfield->setProperty("BackgroundEnabled","False");
//			cntfield->setProperty("VertFormatting","TopAligned");
//			cntfield->setProperty("HorzFormatting","RightAligned");*/
//			}
//            //nun ist curField gesetzt, nun wird der rest geamcht
//            curField->setSize(CEGUI::UVector2(CEGUI::UDim(0,INV_IMAGESIZE),CEGUI::UDim(0,INV_IMAGESIZE)));			
//			curField->setPosition(CEGUI::UVector2(CEGUI::UDim(0,INV_IMAGESIZE*x),CEGUI::UDim(0,INV_IMAGESIZE*y)));
//			curField->setID(invIndex);
//            invIndex++;
//        }
//        //nun schauen, ob am Ende der Zeile überflüssige Kästchen sind
//        int x= rowLength;
//        CEGUI::Window *badchild = NULL;
//        Ogre::String badName = "playerInv"+str(x)+"_"+str(y);
//        mLog("schau ob da ist: "+badName);
//	    if(wmgr->isWindowPresent(badName.c_str()))
//	    {
//		    badchild = wmgr->getWindow(badName.c_str());
//            mLog("ja!");
//	    }	
//	    while(badchild)
//	    {
//    //		mLog("lösche Feld "+cname);
//		    wmgr->destroyWindow(badchild);
//		    x++;
//		    
//		    Ogre::String cname = "playerInv"+str(x)+"_"+str(y);
//
//		    if(wmgr->isWindowPresent(cname.c_str()))
//		    {
//			    badchild = wmgr->getWindow(cname.c_str());
//		    }
//		    else
//		    {
//			    badchild = NULL;
//		    }
//	    }
//
//
//    }
//    //nun sind wir durch, jetzt schauen, ob wir überflüssige Zeilen haben, die gelöscht werden müssen
//    int x = 0;
//	int y = numRows;
//	
//	
//	CEGUI::Window *badchild = NULL;
//    Ogre::String badName = "playerInv"+str(x)+"_"+str(y);
//    mLog("schau ob da ist: "+badName);
//	if(wmgr->isWindowPresent(badName.c_str()))
//	{
//		badchild = wmgr->getWindow(badName.c_str());
//        mLog("ja!");
//	}	
//	while(badchild)
//	{
////		mLog("lösche Feld "+cname);
//		wmgr->destroyWindow(badchild);
//		x++;
//		if(x % rowLength == 0)
//		{
//			x = 0;
//			y++;
//		}
//		Ogre::String cname = "playerInv"+str(x)+"_"+str(y);
//
//		if(wmgr->isWindowPresent(cname.c_str()))
//		{
//			badchild = wmgr->getWindow(cname.c_str());
//		}
//		else
//		{
//			badchild = NULL;
//		}
//	}
//
//	
//
//	
//	
//}


//
//void GameApp::updateOtherInventoryWindow()
//{
//	unsigned int rowLength = INV_ROW_LENGTH;
//	//Inventory *inv = currentLevel->getPlayer()->getInventory();
//	unsigned int invSize = 0;
//	if(curOpenInventory)
//	{
//		invSize = curOpenInventory->getMaxSize();
//	}
//	
//	
//	unsigned int x = 0;
//	unsigned int y = 0;
//	//CEGUI::WindowManager *wmgr = CEGUI::WindowManager::getSingletonPtr();
//	CEGUI::Window *invpane = wmgr->getWindow((CEGUI::utf8*)"OtherInventory/contentPane");
//	if(wmgr->isWindowPresent("otherInv0_0"))
//	{
//		CEGUI::Window *badchild = wmgr->getWindow("otherInv0_0");
//		while(badchild)
//		{
//			
//			wmgr->destroyWindow(badchild);
//			x++;
//			if(x % rowLength == 0)
//			{
//				x = 0;
//				y++;
//			}
//			Ogre::String cname = "otherInv"+str(x)+"_"+str(y);
//
//			if(wmgr->isWindowPresent(cname.c_str()))
//			{
//				badchild = wmgr->getWindow(cname.c_str());
//			}
//			else
//			{
//				badchild = NULL;
//			}
//
//
//		}
//
//	}
//
//	if(curOpenInventory)
//	{
//		
//		
//		
//		//das erzeugt erstmal alle felder
//		x = y = 0;
//		Ogre::Real imgsize = INV_IMAGESIZE;
//		for(unsigned int i=0;i<invSize;i++)
//		{
//			Ogre::String fieldname = "otherInv"+str(x)+"_"+str(y);
//			CEGUI::Window *itest = wmgr->createWindow("LandsLook/StaticImage",(CEGUI::utf8*)fieldname.c_str());
//			itest->setSize(CEGUI::UVector2(CEGUI::UDim(0,imgsize),CEGUI::UDim(0,imgsize)));
//			invpane->addChildWindow(itest);
//			itest->setPosition(CEGUI::UVector2(CEGUI::UDim(0,imgsize*x),CEGUI::UDim(0,imgsize*y)));
//			itest->setID(i);
//			itest->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&GameApp::playerInventoryClick, this));
//			
//			fieldname = "otherInvCnt"+str(x)+"_"+str(y);
//			CEGUI::Window *cntfield = wmgr->createWindow("LandsLook/StaticText",(CEGUI::utf8*)fieldname.c_str());
//			cntfield->setSize(CEGUI::UVector2(CEGUI::UDim(0,32),CEGUI::UDim(0,16)));
//			cntfield->setText("");
//			itest->addChildWindow(cntfield);
//			cntfield->setPosition(CEGUI::UVector2(CEGUI::UDim(1,-34),CEGUI::UDim(1,-16)));
//			cntfield->setProperty("FrameEnabled","False");
//			cntfield->setProperty("BackgroundEnabled","False");
//			cntfield->setProperty("VertFormatting","TopAligned");
//			cntfield->setProperty("HorzFormatting","RightAligned");
//			
//			
//
//			x++;
//			if(x % rowLength == 0)
//			{
//				x = 0;
//				y++;
//			}
//		}
//		otherInventorySize = invSize;
//	}//von if(inv)
//	else
//	{
//		otherInventorySize = 0;
//	}
//	
//}
//
//bool GameApp::playerSpellbookClick(const CEGUI::EventArgs &e)
//{
//	if(curHolding.item.count != 0)
//		return true;
//
//	bool shouldPlaySound = false;
//	const CEGUI::MouseEventArgs& me = 
//		static_cast<const CEGUI::MouseEventArgs&>(e);
//	
//	UniKey curKey = convertCeguiMouseButtonToOis(me.button);	
//	if(me.sysKeys & CEGUI::Control)
//	{
//		curKey.useModifier = true;
//		curKey.modifier = OIS::Keyboard::Ctrl;
//	}
//	else if(me.sysKeys & CEGUI::Alt)
//	{
//		curKey.useModifier = true;
//		curKey.modifier = OIS::Keyboard::Alt;
//	}
//	else if(me.sysKeys & CEGUI::Shift)
//	{
//		curKey.useModifier = true;
//		curKey.modifier = OIS::Keyboard::Shift;
//	}
//	
//	Ogre::String name = me.window->getName().c_str();
//
//	gamedata_spell* playerSpell = currentLevel->getPlayer()->getSpell();
//
//	if(Ogre::StringUtil::startsWith(name,"playerspell"))
//	{
//		//das ist die vorhin zugewiesene ID des fensters. entspricht der nr im inventar
//		unsigned int wid = me.window->getID();
//		int x = Math::IFloor(wid % INV_ROW_LENGTH);
//		int y = Math::IFloor(wid / INV_ROW_LENGTH);
//		Spellbook *sb = currentLevel->getPlayer()->getSpellbook();
//		gamedata_spell* cur = sb->getAt(wid);
//		if(curKey == mFrameListener->mainKeys[keyInvTakeOne] || curKey == mFrameListener->altKeys[keyInvTakeOne])
//		{
//			//nehmen-taste:
//			if(cur)
//			{
//				//da ist was drin
//				if(!curHolding.spell)
//				{				
//					//ich halte nichts
//					if(curKey == mFrameListener->mainKeys[keyInvTakeOne] || curKey == mFrameListener->altKeys[keyInvTakeOne])
//					{
//						//aufnehmen...
//						sb->setAt(wid,NULL);
//						
//						setHoldingSpell(cur);
//						updateSpellbookField(x,y,NULL);
//						if(cur == playerSpell)
//						{
//							setSpellBorder(holdingItemWnd);
//						}
//						shouldPlaySound = true;
//					}
//					
//				}
//				else
//				{
//					//ich halte was. tauschen
//					gamedata_spell* old = sb->getAt(wid);
//					if(cur == playerSpell)
//					{
//						//dann wird der aktive spell grad aufgenommen
//						setSpellBorder(holdingItemWnd);
//					}
//					shouldPlaySound = true;
//					//if(curHolding.spell == playerSpell)
//					//{
//					//	//dann wirds grad abgelegt. updateSpellbookField sollte sich darum kümmern
//
//
//					//}
//					sb->setAt(wid,curHolding.spell);
//					updateSpellbookField(x,y,curHolding.spell);
//					setHoldingSpell(cur);
//					//prüfen, ob eins davon aktiv ist
//
//
//				}
//			}
//			else
//			{
//				//da ist nix drin
//				if(curHolding.spell)
//				{				
//					//ich halte was
//					if(curKey == mFrameListener->mainKeys[keyInvTakeOne] || curKey == mFrameListener->altKeys[keyInvTakeOne])
//					{
//						//ablegen...
//						sb->setAt(wid,curHolding.spell);					
//						
//						updateSpellbookField(x,y,curHolding.spell);
//						setHoldingSpell(NULL);
//						shouldPlaySound = true;
//					}
//				}
//
//			}
//
//		}
//		else if(curKey == mFrameListener->mainKeys[keyInvUse] || curKey == mFrameListener->altKeys[keyInvUse])
//		{
//			//rechtsklicken bzw aktivieren
//			//nur wenn dort ein Spell ist
//			if(cur)
//			{
//				setSpellBorder(me.window);
//				currentLevel->getPlayer()->setSpell(cur);
////				setDisplayedSpell(cur);
//
//			}			
//
//		}
//
//		
//	}
//
//	if(shouldPlaySound)
//	{
//		playPlayerSFX(clickSnd);
//	}
//	return true;//OMFG
//}
//
//void GameApp::highlightField(Window *field)
//{
//	
//	CEGUI::Window *border;
//	//"anlegen"
//	if(!wmgr->isWindowPresent(NAME_SPELLBORDER))
//	{
//		border = wmgr->createWindow("LandsLook/StaticImage",NAME_SPELLBORDER);
//		border->setProperty("FrameEnabled","False");
//		border->setProperty("BackgroundEnabled","False");
//		//border->setProperty("VertFormatting","TopAligned");
//		//border->setProperty("HorzFormatting","RightAligned");
//		ceguiSetImage(border,ICON_SELECTBORDER);
//		/*border->setSize(CEGUI::UVector2( 
//			CEGUI::UDim(0,INV_IMAGESIZE-8),
//			CEGUI::UDim(0,INV_IMAGESIZE-8) 
//		));*/
//	}
//	else
//	{
//		border = wmgr->getWindow(NAME_SPELLBORDER);
//		if(border->getParent())
//			border->getParent()->removeChildWindow(border);
//	}
//	if(field)
//	{
//		border->setVisible(true);
//		field->addChildWindow(border);
//		border->setArea(UDim(0,0),UDim(0,0),UDim(1,0),UDim(1,0));
//	}
//	else
//	{
//		border->setVisible(false);
//		if(border->getParent())
//		{
//			border->getParent()->removeChildWindow(border);
//		}
//		
//	}
//}

//
//bool GameApp::otherInventoryClick(const CEGUI::EventArgs &e)
//{
//	if(curHolding.spell != "" || curHoldingContainer != "")
//		//ich halte einen spell oder tasche. somit nix inventar
//		return true;
//
//	const CEGUI::MouseEventArgs& me = 
//		static_cast<const CEGUI::MouseEventArgs&>(e);
//	
//	UniKey curKey = convertCeguiMouseButtonToOis(me.button);	
//	curKey.ignoreModifier = false;
//	if(me.sysKeys & CEGUI::Control)
//	{
//		curKey.useModifier = true;
//		curKey.modifier = OIS::Keyboard::Ctrl;
//	}
//	else if(me.sysKeys & CEGUI::Alt)
//	{
//		curKey.useModifier = true;
//		curKey.modifier = OIS::Keyboard::Alt;
//	}
//	else if(me.sysKeys & CEGUI::Shift)
//	{
//		curKey.useModifier = true;
//		curKey.modifier = OIS::Keyboard::Shift;
//	}
//	
//	Ogre::String name = me.window->getName().c_str();
//	if(Ogre::StringUtil::startsWith(name,"playerinv"))
//	{
//		//das ist die vorhin zugewiesene ID des fensters. entspricht der nr im inventar
//		unsigned int wid = me.window->getID();
//		int x = Math::IFloor(wid % INV_ROW_LENGTH);
//		int y = Math::IFloor(wid / INV_ROW_LENGTH);
//		Inventory *inv = curOpenInventory;
//		Inventory::invItem cur = inv->getItemAt(wid);
//
//		Inventory::invItem neu = cur;
//		Inventory::invItem newHolding = curHolding;
//
//		if(cur.count != 0)
//		{
//			//dh da liegt was drin
//			
//			/*ich machs mal so: 
//			linksklick: eines nehmen/eines ablegen
//			mittelklick: alle nehmen/alle ablegen
//			rechtsklick: benutzen		
//			*/
//			if(curHolding.item.count == 0)
//			{
//				//nehmen, wenn ich nix halte
//				//switch(me.button)
//				//{
//				if(curKey == mFrameListener->mainKeys[GameFrameListener::keyInvTakeOne] || curKey == mFrameListener->altKeys[GameFrameListener::keyInvTakeOne])
//				//case CEGUI::LeftButton:
//				{
//					//eines nehmen
//
//					inv->removeItemAt(wid,1);
//					neu.count -= 1;
//					updateInventoryField(x,y,neu);
//					newHolding.itemID = cur.itemID;
//					newHolding.count = 1;
//					setHoldingItem(newHolding);
//				}
//				else if(curKey == mFrameListener->mainKeys[GameFrameListener::keyInvTakeAll] || curKey == mFrameListener->altKeys[GameFrameListener::keyInvTakeAll])
//					//break;
//				//case CEGUI::MiddleButton:
//				{
//					newHolding = cur;				
//					inv->removeItemAt(wid,cur.count);
//					neu.count = 0;
//					neu.itemID= "";
//					updateInventoryField(x,y,neu);
//					setHoldingItem(newHolding);
//
//					//break;
//				}
//				else if(curKey == mFrameListener->mainKeys[GameFrameListener::keyInvUse] || curKey == mFrameListener->altKeys[GameFrameListener::keyInvUse])
//				{
//
//				}
//				//case CEGUI::RightButton:
//				//	break;
//				
//			}
//			else
//			{
//				//dh ich klicke auf ein feld wo was drin ist, und halte dabei schon was
//				if(cur.itemID == curHolding.itemID)
//				{
//					//wenn ich das gleiche halte wie das was im feld liegt
//					unsigned int placed;
//					//switch(me.button)
//					//{
//					//case CEGUI::LeftButton:
//					if(curKey == mFrameListener->mainKeys[GameFrameListener::keyInvTakeOne]	|| curKey == mFrameListener->altKeys[GameFrameListener::keyInvTakeOne])
//					{
//						//eines dazutun
//						placed = inv->addItemAt(wid,curHolding.itemID,1);
//						if(placed == 1)
//						{
//							neu.count = cur.count+1;
//							neu.itemID = curHolding.itemID;
//							updateInventoryField(x,y,neu);
//
//							newHolding.count -= 1;						
//							setHoldingItem(newHolding);
//						}
//					}
//					else if(curKey == mFrameListener->mainKeys[GameFrameListener::keyInvTakeAll] || curKey == mFrameListener->altKeys[GameFrameListener::keyInvTakeAll])
//					{
//					//	break;
//					//case CEGUI::MiddleButton:
//						placed = inv->addItemAt(wid,curHolding.itemID,curHolding.item.count);
//						neu.itemID = curHolding.itemID;
//						neu.count  = intToByte(placed+cur.count);
//						updateInventoryField(x,y,neu);
//						//newHolding.itemID = neu.itemID;
//						newHolding.count  = curHolding.item.count - intToByte(placed);
//
//						setHoldingItem(newHolding);
//
//						//break;	
//					}
//				}
//				
//
//			}
//
//		}
//		else
//		{
//			//klick auf leeres feld
//			if(curHolding.item.count != 0)
//			{
//				Inventory::invItem neu = cur;
//				//...und wir halten was zum ablegen
//				unsigned int placed;
//				//switch(me.button)
//				//{
//				//case CEGUI::LeftButton:
//				if(curKey == mFrameListener->mainKeys[GameFrameListener::keyInvTakeOne] || curKey == mFrameListener->altKeys[GameFrameListener::keyInvTakeOne])
//				{
//					//nur eines
//					placed = inv->addItemAt(wid,curHolding.itemID,1);
//					if(placed == 1)
//					{
//						neu.count = 1;
//						neu.itemID = curHolding.itemID;
//						updateInventoryField(x,y,neu);
//						newHolding.count -= 1;						
//						setHoldingItem(newHolding);
//					}
//				}
//				else if(curKey == mFrameListener->mainKeys[GameFrameListener::keyInvTakeAll] || curKey == mFrameListener->altKeys[GameFrameListener::keyInvTakeAll])
//				{	
//					//break;
//				//case CEGUI::MiddleButton:
//					//jetzt alles ablegen. oder zumindest so viel wie geht
//					placed = inv->addItemAt(wid,curHolding.itemID,curHolding.item.count);
//					neu.itemID = curHolding.itemID;
//					neu.count  = intToByte(placed);
//					updateInventoryField(x,y,neu);
//					//newHolding.itemID = neu.itemID;
//					newHolding.count  = curHolding.item.count - intToByte(placed);
//
//					setHoldingItem(newHolding);
//					//break;
//				}
//			}
//			
//		}//ende von curCount != 0
//		
//
//		
//	}
//	return true;
//}
//--------------------------------------------------------------------------------------------------------
//bool GameApp::playerInventoryClick(const CEGUI::EventArgs &e)
//{
//	if(curHolding.spell || curHolding.inventory)
//		//ich halte einen spell oder eine volle tasche. somit nix inventar
//		return true;
//
//	const CEGUI::MouseEventArgs& me = 
//		static_cast<const CEGUI::MouseEventArgs&>(e);
//	
//	UniKey curKey = convertCeguiMouseButtonToOis(me.button);	
//	curKey.ignoreModifier = false;
//	if(me.sysKeys & CEGUI::Control)
//	{
//		curKey.useModifier = true;
//		curKey.modifier = OIS::Keyboard::Ctrl;
//	}
//	else if(me.sysKeys & CEGUI::Alt)
//	{
//		curKey.useModifier = true;
//		curKey.modifier = OIS::Keyboard::Alt;
//	}
//	else if(me.sysKeys & CEGUI::Shift)
//	{
//		curKey.useModifier = true;
//		curKey.modifier = OIS::Keyboard::Shift;
//	}
//	
//	Ogre::String name = me.window->getName().c_str();
//	//diese funktion macht sowohl inventar des players als auch kontainer, der nachfolgende bool entscheidet
//	//drüber, welches
//	bool playerInv;
//	bool shouldPlaySound = false;
//	if(Ogre::StringUtil::startsWith(name,"playerinv"))
//		playerInv = true;
//	else if(Ogre::StringUtil::startsWith(name,"otherinv"))
//		playerInv = false;
//	else
//		return true;
//
//	
//	//das ist die vorhin zugewiesene ID des fensters. entspricht der nr im inventar
//	unsigned int wid = me.window->getID();
//	int x = Math::IFloor(wid % INV_ROW_LENGTH);
//	int y = Math::IFloor(wid / INV_ROW_LENGTH);
//	Inventory *inv;
//	if(playerInv)
//		inv = currentLevel->getPlayer()->getInventory();
//	else
//		inv = curOpenInventory;
//	Inventory::invDataItem cur = inv->getItemAt(wid);
//
//	Inventory::invDataItem neu = cur;
//	Inventory::invDataItem newHolding = curHolding;
//
//	if(cur.count != 0)
//	{
//		//dh da liegt was drin
//		
//		/*ich machs mal so: 
//		linksklick: eines nehmen/eines ablegen
//		mittelklick: alle nehmen/alle ablegen
//		rechtsklick: benutzen		
//		*/
//		if(curHolding.item.count == 0)
//		{
//			//nehmen, wenn ich nix halte
//			//switch(me.button)
//			//{
//			if(curKey == mFrameListener->mainKeys[keyInvTakeOne] || curKey == mFrameListener->altKeys[keyInvTakeOne])
//			//case CEGUI::LeftButton:
//			{
//				//eines nehmen
//
//				inv->removeItemAt(wid,1);
//				neu.count -= 1;
//				if(playerInv)
//					updateInventoryField(x,y,neu);
//				else
//					updateOtherInvField(x,y,neu);
//				newHolding.item = cur.item;
//				newHolding.count = 1;
//				setHoldingItem(newHolding);
//				shouldPlaySound = true;
//			}
//			else if(curKey == mFrameListener->mainKeys[keyInvTakeAll] || curKey == mFrameListener->altKeys[keyInvTakeAll])
//				//break;
//			//case CEGUI::MiddleButton:
//			{
//				//alle nehmen
//				newHolding = cur;				
//				inv->removeItemAt(wid,cur.count);
//				neu.count = 0;
//				neu.item = NULL;
//				if(playerInv)
//					updateInventoryField(x,y,neu);
//				else
//					updateOtherInvField(x,y,neu);
//				setHoldingItem(newHolding);
//				shouldPlaySound = true;
//				//break;
//			}
//			else if(curKey == mFrameListener->mainKeys[keyInvUse] || curKey == mFrameListener->altKeys[keyInvUse])
//			{
//				//benutzen, was drin liegt
//
//			}
//			//case CEGUI::RightButton:
//			//	break;
//			
//		}
//		else
//		{
//			//dh ich klicke auf ein feld wo was drin ist, und halte dabei schon was
//			if(cur.item == curHolding.item)
//			{
//				//wenn ich das gleiche halte wie das was im feld liegt
//				unsigned int placed;
//				//switch(me.button)
//				//{
//				//case CEGUI::LeftButton:
//				if(curKey == mFrameListener->mainKeys[keyInvTakeOne]	|| curKey == mFrameListener->altKeys[keyInvTakeOne])
//				{
//					//eines dazutun
//					placed = inv->addItemAt(wid,curHolding.item,1);
//					if(placed == 1)
//					{
//						shouldPlaySound = true;
//						neu.count = cur.count+1;
//						neu.item = curHolding.item;
//						if(playerInv)
//							updateInventoryField(x,y,neu);
//						else
//							updateOtherInvField(x,y,neu);
//
//						newHolding.count -= 1;						
//						setHoldingItem(newHolding);
//					}
//				}
//				else if(curKey == mFrameListener->mainKeys[keyInvTakeAll] || curKey == mFrameListener->altKeys[keyInvTakeAll])
//				{
//				//	break;
//				//case CEGUI::MiddleButton:
//					//soviel dazutun wie geht
//					placed = inv->addItemAt(wid,curHolding.item,curHolding.item.count);
//					neu.item = curHolding.item;
//					neu.count  = intToByte(placed+cur.count);
//					if(playerInv)
//						updateInventoryField(x,y,neu);
//					else
//						updateOtherInvField(x,y,neu);
//					//newHolding.itemID = neu.itemID;
//					newHolding.count  = curHolding.item.count - intToByte(placed);
//					shouldPlaySound = true;
//					setHoldingItem(newHolding);
//
//					//break;	
//				}
//			}//von if(cur.item == curHolding.item
//			else
//			{
//				//mit etwas auf etwas anderes klicken
//				//zuerst misc attempt
//				//wenn linksklick->jeweils eines mischen
//				// wenn cur oder curHolding 1 waren, kommt es dahin
//				// sonst: einfach ins inv stecken
//				//wenn mittelklick:
//				//versuch, alle gehaltenen mit allen im feld zu mischen
//				//resultat kommt dahin, wo vorher weniger waren
//				//am besten function miscItems(invDataItem &educt1, invDataItem &educt2, invDataItem &procut, bool miscAll  = false)
//				//die sich gleich um alles kümmert
//				//map von struct oder array mit 2 pointern, die dort nach größe sortiert vorkommen
//					
//				//sonst, wenn beide jeweils 1 sind -> vertauschen
//				
//			}
//			
//
//		}
//
//	}
//	else
//	{
//		//klick auf leeres feld
//		if(curHolding.item.count != 0)
//		{
//			Inventory::invDataItem neu = cur;
//			//...und wir halten was zum ablegen
//			unsigned int placed;
//			//switch(me.button)
//			//{
//			//case CEGUI::LeftButton:
//			if(curKey == mFrameListener->mainKeys[keyInvTakeOne] || curKey == mFrameListener->altKeys[keyInvTakeOne])
//			{
//				//nur eines
//				placed = inv->addItemAt(wid,curHolding.item,1);
//				if(placed == 1)
//				{
//					neu.count = 1;
//					neu.item = curHolding.item;
//					if(playerInv)
//						updateInventoryField(x,y,neu);
//					else
//						updateOtherInvField(x,y,neu);
//					newHolding.count -= 1;						
//					setHoldingItem(newHolding);
//					shouldPlaySound = true;
//				}
//			}
//			else if(curKey == mFrameListener->mainKeys[keyInvTakeAll] || curKey == mFrameListener->altKeys[keyInvTakeAll])
//			{	
//				//break;
//			//case CEGUI::MiddleButton:
//				//jetzt alles ablegen. oder zumindest so viel wie geht
//				placed = inv->addItemAt(wid,curHolding.item,curHolding.item.count);
//				neu.item = curHolding.item;
//				neu.count  = intToByte(placed);
//				if(playerInv)
//					updateInventoryField(x,y,neu);
//				else
//					updateOtherInvField(x,y,neu);
//				//newHolding.itemID = neu.itemID;
//				newHolding.count  = curHolding.item.count - intToByte(placed);
//				shouldPlaySound = true;
//
//				setHoldingItem(newHolding);
//				//break;
//			}
//		}
//		
//	}//ende von curCount != 0
//	
//
//	if(shouldPlaySound)
//	{
//		playPlayerSFX(clickSnd);
//	}
//	
//
//
//	return true;
//	//Ogre::String sX = name.substr(
//}

//
////--------------------------------------------------------------------------------------------------------
//bool GameApp::equipmentClick(const CEGUI::EventArgs &e)
//{
//	if(curHolding.inventory || curHolding.spell)
//		return true;
//	const CEGUI::MouseEventArgs& me = 
//		static_cast<const CEGUI::MouseEventArgs&>(e);
//	GameChar *player = currentLevel->getPlayer();
//	CEGUI::String wname = me.window->getName();
//	bool shouldPlaySound = false;
//
//	if(wname == "PlayerInventory/melee")
//	{
//		gamedata_item *cur = player->getMelee();
//		if(curHolding.item.count != 0 && cur == NULL)
//		{
//			//ich halte was, und der slot ist leer
//			if(player->setMelee(curHolding.item.item))
//			{			
//				curHolding.item.count -= 1;
//				setHoldingItem(curHolding.item);
//				CEGUI::String invimage = curHolding.item.item->inv_image.c_str();
//				me.window->setTooltipText(curHolding.item.item->name);
//				ceguiSetImage(me.window,invimage);
//				shouldPlaySound = true;
//			}
//		}
//		else if(curHolding.item.count == 0 && cur)
//		{
//			//ich halte nichts und im slot ist was drin
//			if(player->setMelee(NULL))
//			{
//				curHolding.item.count = 1;
//				curHolding.item.item = cur;
//				setHoldingItem(curHolding.item);
//				me.window->setTooltipText("");
//				ceguiSetImage(me.window,ICON_MELEE);
//				shouldPlaySound = true;
//			}
//		}
//	}
//	else if(wname == "PlayerInventory/armor")
//	{
//		gamedata_item *cur = player->getArmor();
//		if(curHolding.item.count != 0 && !cur)
//		{
//			//ich halte was, und der slot ist leer
//			if(player->setArmor(curHolding.item.item))
//			{			
//				curHolding.item.count -= 1;
//				setHoldingItem(curHolding.item);
//				CEGUI::String invimage = curHolding.item.item->inv_image.c_str();
//				me.window->setTooltipText(curHolding.item.item->name);
//				ceguiSetImage(me.window,invimage);
//				shouldPlaySound = true;
//			}
//		}
//		else if(curHolding.item.count == 0 && cur)
//		{
//			//ich halte nichts und im slot ist was drin
//			if(player->setArmor(NULL))
//			{
//				curHolding.item.count = 1;
//				curHolding.item.item = cur;
//				setHoldingItem(curHolding.item);
//				me.window->setTooltipText("");
//				ceguiSetImage(me.window,ICON_ARMOR);	
//				shouldPlaySound = true;
//			}
//		}
//	}
//	else if(wname == "PlayerInventory/ranged")
//	{
//		gamedata_item *cur = player->getRanged();
//		if(curHolding.item.count != 0 && !cur)
//		{
//			//ich halte was, und der slot ist leer
//			if(player->setRanged(curHolding.item.item))
//			{			
//				curHolding.item.count -= 1;
//				setHoldingItem(curHolding.item);
//				CEGUI::String invimage = curHolding.item.item->inv_image.c_str();
//				me.window->setTooltipText(curHolding.item.item->name);
//				ceguiSetImage(me.window,invimage);
//				shouldPlaySound = true;
//			}
//		}
//		else if(curHolding.item.count == 0 && cur)
//		{
//			//ich halte nichts und im slot ist was drin
//			if(player->setRanged(NULL))
//			{
//				curHolding.item.count = 1;
//				curHolding.item.item = cur;
//				setHoldingItem(curHolding.item);
//				me.window->setTooltipText("");
//				ceguiSetImage(me.window,ICON_RANGED);	
//				shouldPlaySound = true;
//			}
//		}
//	}
//	else if(wname == "PlayerInventory/shield")
//	{
//		gamedata_item *cur = player->getShield();
//		if(curHolding.item.count != 0 && !cur)
//		{
//			//ich halte was, und der slot ist leer
//			if(player->setShield(curHolding.item.item))
//			{			
//				curHolding.item.count -= 1;
//				setHoldingItem(curHolding.item);
//				CEGUI::String invimage = curHolding.item.item->inv_image.c_str();
//				me.window->setTooltipText(curHolding.item.item->name);
//				ceguiSetImage(me.window,invimage);
//				shouldPlaySound = true;
//			}
//		}
//		else if(curHolding.item.count == 0 && cur)
//		{
//			//ich halte nichts und im slot ist was drin
//			if(player->setShield(NULL))
//			{
//				curHolding.item.count = 1;
//				curHolding.item.item = cur;
//				setHoldingItem(curHolding.item);
//				me.window->setTooltipText("");
//				ceguiSetImage(me.window,ICON_SHIELD);		
//				shouldPlaySound = true;
//			}
//		}
//	}
//	else if(wname == "PlayerInventory/else4")
//	{
//		gamedata_item *cur = player->getElse4();
//		if(curHolding.item.count != 0 && !cur)
//		{
//			//ich halte was, und der slot ist leer
//			if(player->setElse4(curHolding.item.item))
//			{			
//				curHolding.item.count -= 1;
//				setHoldingItem(curHolding.item);
//				CEGUI::String invimage = curHolding.item.item->inv_image.c_str();
//				ceguiSetImage(me.window,invimage);
//				me.window->setTooltipText(curHolding.item.item->name);
//				shouldPlaySound = true;
//			}
//		}
//		else if(curHolding.item.count == 0 && cur)
//		{
//			//ich halte nichts und im slot ist was drin
//			if(player->setElse4(NULL))
//			{
//				curHolding.item.count = 1;
//				curHolding.item.item = cur;
//				setHoldingItem(curHolding.item);
//				ceguiSetImage(me.window,ICON_ELSE);	
//				me.window->setTooltipText("");
//				shouldPlaySound = true;
//			}
//		}
//	}
//	else if(wname == "PlayerInventory/else3")
//	{
//		gamedata_item *cur = player->getElse3();
//		if(curHolding.item.count != 0 && !cur)
//		{
//			//ich halte was, und der slot ist leer
//			if(player->setElse3(curHolding.item.item))
//			{			
//				curHolding.item.count -= 1;
//				setHoldingItem(curHolding.item);
//				CEGUI::String invimage = curHolding.item.item->inv_image.c_str();
//				ceguiSetImage(me.window,invimage);
//				me.window->setTooltipText(curHolding.item.item->name);
//				shouldPlaySound = true;
//			}
//		}
//		else if(curHolding.item.count == 0 && cur)
//		{
//			//ich halte nichts und im slot ist was drin
//			if(player->setElse3(NULL))
//			{
//				curHolding.item.count = 1;
//				curHolding.item.item = cur;
//				setHoldingItem(curHolding.item);
//				ceguiSetImage(me.window,ICON_ELSE);		
//				me.window->setTooltipText("");
//				shouldPlaySound = true;
//			}
//		}
//	}
//	else if(wname == "PlayerInventory/else2")
//	{
//		gamedata_item *cur = player->getElse2();
//		if(curHolding.item.count != 0 && !cur)
//		{
//			//ich halte was, und der slot ist leer
//			if(player->setElse2(curHolding.item.item))
//			{			
//				curHolding.item.count -= 1;
//				setHoldingItem(curHolding.item);
//				CEGUI::String invimage = curHolding.item.item->inv_image.c_str();
//				ceguiSetImage(me.window,invimage);
//				me.window->setTooltipText(curHolding.item.item->name);
//				shouldPlaySound = true;
//			}
//		}
//		else if(curHolding.item.count == 0 && cur)
//		{
//			//ich halte nichts und im slot ist was drin
//			if(player->setElse2(NULL))
//			{
//				curHolding.item.count = 1;
//				curHolding.item.item = cur;
//				setHoldingItem(curHolding.item);
//				ceguiSetImage(me.window,ICON_ELSE);		
//				me.window->setTooltipText("");
//				shouldPlaySound = true;
//			}
//		}
//	}
//	else if(wname == "PlayerInventory/else1")
//	{
//		gamedata_item *cur = player->getElse1();
//		if(curHolding.item.count != 0 && !cur)
//		{
//			//ich halte was, und der slot ist leer
//			if(player->setElse1(curHolding.item.item))
//			{			
//				curHolding.item.count -= 1;
//				setHoldingItem(curHolding.item);
//				CEGUI::String invimage = curHolding.item.item->inv_image.c_str();
//				ceguiSetImage(me.window,invimage);
//				me.window->setTooltipText(curHolding.item.item->name);
//				shouldPlaySound = true;
//			}
//		}
//		else if(curHolding.item.count == 0 && cur)
//		{
//			//ich halte nichts und im slot ist was drin
//			if(player->setElse1(NULL))
//			{
//				curHolding.item.count = 1;
//				curHolding.item.item = cur;
//				setHoldingItem(curHolding.item);
//				ceguiSetImage(me.window,ICON_ELSE);		
//				me.window->setTooltipText(curHolding.item.item->name);
//				shouldPlaySound = true;
//			}
//		}
//	}
//	if(shouldPlaySound)
//	{
//		playPlayerSFX(clickSnd);
//	}
//	updateDisplayedStats();
//	return true;
//
//}


//---------------------------------------------------------------------------------------------------------
//void GameApp::updateSpellbookField(unsigned int x, unsigned int y,gamedata_spell* spell)
//{
//	//CEGUI::ImagesetManager *imgr = CEGUI::ImagesetManager::getSingletonPtr();
//	//CEGUI::WindowManager *wmgr = CEGUI::WindowManager::getSingletonPtr();
//	Ogre::String iname = "playerSpell"+str(x)+"_"+str(y);
//	CEGUI::Window *imgfield = wmgr->getWindow(iname.c_str());
//	if(!spell)
//	{
//		ceguiSetImage(imgfield,"");	
//		imgfield->setTooltipText("");
//	}
//	else
//	{
//		ceguiSetImage(imgfield,spell->inv_image.c_str());
//		imgfield->setTooltipText(spell->name.c_str());
//		/*gamedata_spell curData = getSpellData(spell);
//		CEGUI::String imgFileName = curData.inv_image.c_str();
//		if(!imgr->isImagesetPresent(imgFileName))
//		{
//			imgr->createImagesetFromImageFile(imgFileName,imgFileName);
//		}
//		
//		
//		imgfield->setProperty("Image", "set:"+imgFileName+" image:full_image");*/
//
//		if(spell && spell == currentLevel->getPlayer()->getSpell())
//		{
//			setSpellBorder(imgfield);
//		}
//		
//		
//		//cntfield->setText(str(cur.count).c_str());
//	}
//}
//--------------------------------------------------------------------------------------------------------
//void GameApp::updateInventoryField(unsigned int x, unsigned int y,Inventory::invDataItem cur)
//{
//    return;
//	//CEGUI::ImagesetManager *imgr = CEGUI::ImagesetManager::getSingletonPtr();
//	//CEGUI::WindowManager *wmgr = CEGUI::WindowManager::getSingletonPtr();
//	Ogre::String iname = "playerInv"+str(x)+"_"+str(y);
//	Ogre::String cname = "playerInvCnt"+str(x)+"_"+str(y);
//	CEGUI::Window *imgfield = wmgr->getWindow(iname.c_str());
//	CEGUI::Window *cntfield = wmgr->getWindow(cname.c_str());
//	if(cur.count == 0)
//	{
//		imgfield->setProperty("Image", "False");		
//		cntfield->setText("");
//		imgfield->setTooltipText("");
//
//	}
//	else
//	{
//		
//		/*CEGUI::String imgFileName = cur.item->inv_image.c_str();
//		if(!imgr->isImagesetPresent(imgFileName))
//		{
//			imgr->createImagesetFromImageFile(imgFileName,imgFileName);
//		}
//		
//		
//		imgfield->setProperty("Image", "set:"+imgFileName+" image:full_image");*/
//		ceguiSetImage(imgfield,cur.item->inv_image.c_str());
//		imgfield->setTooltipText(cur.item->name.c_str());
//		if(cur.count != 1)
//			cntfield->setText(str(cur.count).c_str());
//		else
//			cntfield->setText("");
//	}
//}
//void GameApp::updateOtherInvField(unsigned int x, unsigned int y,Inventory::invDataItem cur)
//{
//	//CEGUI::ImagesetManager *imgr = CEGUI::ImagesetManager::getSingletonPtr();
//	//CEGUI::WindowManager *wmgr = CEGUI::WindowManager::getSingletonPtr();
//	Ogre::String iname = "otherInv"+str(x)+"_"+str(y);
//	Ogre::String cname = "otherInvCnt"+str(x)+"_"+str(y);
//	CEGUI::Window *imgfield = wmgr->getWindow(iname.c_str());
//	CEGUI::Window *cntfield = wmgr->getWindow(cname.c_str());
//	if(cur.count == 0)
//	{
//		imgfield->setProperty("Image", "False");		
//		cntfield->setText("");
//		imgfield->setTooltipText("");
//
//	}
//	else
//	{
//		
//		/*CEGUI::String imgFileName = cur.item->inv_image.c_str();
//		if(!imgr->isImagesetPresent(imgFileName))
//		{
//			imgr->createImagesetFromImageFile(imgFileName,imgFileName);
//		}
//
//		
//		
//		
//		imgfield->setProperty("Image", "set:"+imgFileName+" image:full_image");*/
//		ceguiSetImage(imgfield,cur.item->inv_image.c_str());
//		imgfield->setTooltipText(cur.item->name.c_str());
//		if(cur.count != 1)
//			cntfield->setText(str(cur.count).c_str());
//		else
//			cntfield->setText("");
//	}
//}
//--------------------------------------------------------------------------------------------------------
//void GameApp::setHoldingDisplay(Ogre::String imgName, Ogre::String subtext)
//{
//	if(imgName != "")
//	{
//		holdingItemWnd->enable();
//		holdingItemWnd->setVisible(true);
//		CEGUI::Point mp = MouseCursor::getSingletonPtr()->getPosition();					
//		holdingItemWnd->setPosition(CEGUI::UVector2(CEGUI::UDim(0,mp.d_x+8),CEGUI::UDim(0,mp.d_y+8) ));
//					
//
//		//CEGUI::ImagesetManager *imgr = CEGUI::ImagesetManager::getSingletonPtr();
//		
//		/*if(!imgr->isImagesetPresent(imgName))
//		{
//			imgr->createImagesetFromImageFile(imgName,imgName);
//		}	
//		holdingItemWnd->setProperty("Image", "set:"+imgName+" image:full_image");*/
//		ceguiSetImage(holdingItemWnd,imgName);
//
//		CEGUI::Window *cntfield = wmgr->getWindow("CurrentHoldingCount");
//		cntfield->setText(subtext.c_str());
//	}
//	else
//	{
//		holdingItemWnd->disable();
//		holdingItemWnd->setVisible(false);
//	}
//
//}
//--------------------------------------------------------------------------------------------------------
void GameApp::setHoldingSpell(gamedata_spell* spell)
{
	curHolding->setHoldingSpell(spell);
	//curHolding.spell = spell;
	//if(curHolding.spell)
	//{
	//	setHoldingDisplay(spell->inv_image);
	//	
	//}
	//else
	//{
	//	setHoldingDisplay("");
	//	//holdingItemWnd->disable();
	//	//holdingItemWnd->setVisible(false);
	//}
}
//--------------------------------------------------------------------------------------------------------
void GameApp::setHoldingItem(Inventory::invDataItem item)
{
	curHolding->setHoldingItem(item);
	//curHolding.item = item;
	//if(curHolding.item.count != 0)
	//{
	//	//holdingItemWnd->enable();
	//	//holdingItemWnd->setVisible(true);
	//	//CEGUI::Point mp = CEGUI::MouseCursor::getSingletonPtr()->getPosition();					
	//	//holdingItemWnd->setPosition(CEGUI::UVector2(CEGUI::UDim(0,mp.d_x+8),CEGUI::UDim(0,mp.d_y+8) ));
	//				

	//	//CEGUI::ImagesetManager *imgr = CEGUI::ImagesetManager::getSingletonPtr();
	//	Ogre::String cntstr = "";
	//	if(item.count != 1)
	//		cntstr = str(item.count);
	//	setHoldingDisplay(curHolding.item.item->inv_image,cntstr);
	//	/*CEGUI::String imgFileName = curData.inv_image.c_str();
	//	if(!imgr->isImagesetPresent(imgFileName))
	//	{
	//		imgr->createImagesetFromImageFile(imgFileName,imgFileName);
	//	}	
	//	holdingItemWnd->setProperty("Image", "set:"+imgFileName+" image:full_image");
	//	CEGUI::Window *cntfield = wmgr->getWindow("CurrentHoldingCount");
	//	cntfield->setText(str(curHolding.item.count).c_str());*/
	//}
	//else
	//{
	//	setHoldingDisplay("");
	//	//holdingItemWnd->disable();
	//	//holdingItemWnd->setVisible(false);
	//}
}
void GameApp::setHoldingContainer(gamedata_item* cont,Inventory *inv)
{
	if(cont)
		curHolding->setHoldingItem(cont,1,inv);
	else
		curHolding->setHoldingItem(NULL,0,NULL);
 //   if(cont == NULL)// || inv == NULL)
	//{
	//	curHolding.inventory = NULL;
 //       curHolding.item.item = NULL;		
 //       curHolding.item.count = 0;
	//	setHoldingDisplay("");
	//}
	//else
	//{
 //       if(!cont->Container)
 //           return;
	//	curHolding.inventory = inv;
 //       curHolding.item.item = cont;
 //       curHolding.item.count = 1;
	//	//curHoldingContainer = cont;
 //       setHoldingDisplay(curHolding.item.item->inv_image);
	//}
}
//
//bool GameApp::hotkeyClick(const CEGUI::EventArgs& e)
//{
//    const CEGUI::MouseEventArgs& me = 
//		static_cast<const CEGUI::MouseEventArgs&>(e);
//    GameChar *player = currentLevel->getPlayer();
//	bool shouldPlaySound = false;
//    Ogre::String wndName = me.window->getName().c_str();
//    int hotkeyIndex = 0;
//    
//    if(wndName == "hotkey10")
//        hotkeyIndex = 9;
//    else
//        hotkeyIndex = StringConverter::parseInt(wndName.substr(6,1))-1;
////    mDebugText(str(hotkeyIndex));
//    UniKey curKey = convertCeguiMouseButtonToOis(me.button);	
//	if(me.sysKeys & CEGUI::Control)
//	{
//		//curKey.useModifier = true;
//		curKey.modifier = OIS::Keyboard::Ctrl;
//	}
//	else if(me.sysKeys & CEGUI::Alt)
//	{
//		//curKey.useModifier = true;
//		curKey.modifier = OIS::Keyboard::Alt;
//	}
//	else if(me.sysKeys & CEGUI::Shift)
//	{
//		//curKey.useModifier = true;
//		curKey.modifier = OIS::Keyboard::Shift;
//	}
//	gamedata_hotkeyable *holding = NULL;
//	//Ogre::String curID = player->getBag();
//	if(curHolding->hasItem() && !curHolding->hasInventory())
//    {
//		holding = curHolding->getItem().item;//curHolding.item.item;
//    }
//    else if(curHolding->hasSpell())
//    {
//		holding = curHolding->getSpell();
//    }
//    if(curKey == settings.mainKeys[keyInvTakeOne] || curKey == settings.altKeys[keyInvTakeOne])
//    {
//        if(holding)
//        {
//            setHotkey(hotkeyIndex,holding);
//            shouldPlaySound = true;
//        }
//    }
//    else if(curKey == settings.mainKeys[keyInvUse] || curKey == settings.altKeys[keyInvUse])
//    {
//        //löschen
//        if(getHotkey(hotkeyIndex))
//        {
//            setHotkey(hotkeyIndex,0);
//            shouldPlaySound = true;
//        }
//    }
//    
//	//{
//	//	//ich halte was, und der slot ist leer
//	//	if(player->setBag(curHolding.item,curHolding.inventory))
//	//	{			
//	//		//setHoldingItem(curHolding.item);
//	//		CEGUI::String invimage = curHolding.item->inv_image.c_str();
//	//		ceguiSetImage(me.window,invimage);
//	//		me.window->setTooltipText(curHolding.item.item->name.c_str());
//	//		updatePlayerInventory(curHolding.inventory);
//	//		setHoldingContainer(NULL);
//	//		shouldPlaySound = true;
//	//		
//	//	}
//	//}
//	//else if(curHolding.item.count==0 && !curHolding.inventory && curBag)
//	//{
//	//	//ich halte nichts und im slot ist was drin
//	//	Inventory *pInv = player->getInventory();
//	//	if(player->setBag(NULL))
//	//	{
//	//		setHoldingContainer(curBag,pInv);
//	//		ceguiSetImage(me.window,ICON_BAG);	
//	//		me.window->setTooltipText("");
//	//		updatePlayerInventory();
//	//		shouldPlaySound = true;
//	//	}
//	//}
//	if(shouldPlaySound)
//	{
//		playPlayerSFX(clickSnd);
//	}
//	return true;
//}
SimpleSound::Source *GameApp::createSource(int type, bool loop, bool relativeToListener, bool stream)
{
	SimpleSound::Source *res = soundMgr->createSource();
	if(!res)
		return NULL;
	res->setType(type);
	res->setLoop(loop);
	res->setRelativeToListener(relativeToListener);
	Ogre::Real gain = settings.gain_sfx;
	switch(type)
	{
	case stMusic:
		gain = settings.gain_music;
		break;
	case stSpeech:
		gain = settings.gain_speech;
		break;
	case stSfxPlayer:
		gain = settings.gain_player_sfx;
		break;
	};
	res->setGain(gain);
	/*res->setType(type);
	res->setMaxDistance(maxDistance);
	res->setRolloffFactor(rollofFactor);
	res->setReferenceDistance(refDistance);*/
	return res;
}
//----------------------------------------------------------------------------------
//bool GameApp::bagEquipClick(const CEGUI::EventArgs& e)
//{
//	const CEGUI::MouseEventArgs& me = 
//		static_cast<const CEGUI::MouseEventArgs&>(e);
//	GameChar *player = currentLevel->getPlayer();
//	bool shouldPlaySound = false;
//	gamedata_item *curBag = player->getBag();
//	//Ogre::String curID = player->getBag();
//    if(curHolding.item.count!=0 && curHolding.item.item->Container && curHolding.inventory && !curBag)
//	{
//		//ich halte was, und der slot ist leer
//		if(player->setBag(curHolding.item.item,curHolding.inventory))
//		{			
//			//setHoldingItem(curHolding.item);
//			CEGUI::String invimage = curHolding.item.item->inv_image.c_str();
//			ceguiSetImage(me.window,invimage);
//			me.window->setTooltipText(curHolding.item.item->name.c_str());
//            playerInventoryView->show(curHolding.inventory);//curHolding.inventory);
//			//playerInventoryView->setInventory(curHolding.inventory);
//			//updatePlayerInventory(curHolding.inventory);
//			setHoldingContainer(NULL);
//			shouldPlaySound = true;
//			
//		}
//	}
//	else if(curHolding.item.count==0 && !curHolding.inventory && curBag)
//	{
//		//ich halte nichts und im slot ist was drin
//		Inventory *pInv = player->getInventory();
//		if(player->setBag(NULL))
//		{
//			setHoldingContainer(curBag,pInv);
//			ceguiSetImage(me.window,ICON_BAG);	
//			me.window->setTooltipText("");
//			//updatePlayerInventory();
//            playerInventoryView->hide();
//			shouldPlaySound = true;
//		}
//	}
//	if(shouldPlaySound)
//	{
//		playPlayerSFX(clickSnd);
//	}
//	return true;
//
//}


//bool GameApp::savegameListClick(const CEGUI::EventArgs& e)
//{
//	CEGUI::Editbox *editbox = static_cast<CEGUI::Editbox*>(wmgr->getWindow("saveloadwnd/namebox"));
//	CEGUI::Listbox* saveList = (CEGUI::Listbox*)wmgr->getWindow("saveloadwnd/savegamelist");
//	CEGUI::ListboxItem *cur = saveList->getFirstSelectedItem();
//	if(!cur)
//		return true;
//	editbox->setText(cur->getText());
//	return true;
//}
//bool GameApp::savegameListDblClick(const CEGUI::EventArgs& e)
//{
//	return true;
//}
//
//void GameApp::updateSaveLoadWnd()
//{
//	CEGUI::Window *slWnd = wmgr->getWindow("saveloadwnd");
//
//	//loading list of saves
//	StringVectorPtr savevector = ResourceGroupManager::getSingletonPtr()->findResourceNames("Savegame",Ogre::String("*.")+SAVEGAME_EXTENSION);
//	//liste sortieren
//	NameTimeVector timevector;
//	for (Ogre::StringVector::iterator i = savevector->begin(); i != savevector->end(); ++i)
//	{
//		NameTime cur;
//		cur.name = *i;
//		cur.time = getFileDate( saveGamePath+"/"+cur.name );
//		timevector.push_back(cur);
//	}
//	sortNameTimeVector(timevector);
//	CEGUI::Listbox* saveList = (CEGUI::Listbox*)wmgr->getWindow("saveloadwnd/savegamelist");
//	size_t cnt = saveList->getItemCount();		
//	for(size_t i = 0;i < cnt;i++)
//	{
//		CEGUI::ListboxItem *listboxitem = saveList->getListboxItemFromIndex(0);
//		//setDebugText(String("\nremoved:")+listboxitem->getText().c_str(),true);
//		saveList->removeItem(listboxitem);
//	}
//	//saveList->subscribeEvent(CEGUI::Window::EventMouseButtonDown,CEGUI::Event::Subscriber(&EditorApp::meshListDown,this));
//	for (NameTimeVector::iterator i = timevector.begin(); i != timevector.end(); ++i)
//	{
//		//Ogre::String wtf = *i;
//		//getFileDate( saveGamePath+"/"+wtf );
//		Ogre::String fname, fext;
//		Ogre::StringUtil::splitBaseFilename((*i).name,fname,fext);
//		CEGUI::ListboxTextItem *listboxitem = new CEGUI::ListboxTextItem(fname.c_str());
//		listboxitem->setSelectionBrushImage(STYLE_GAME,"TextSelectionBrush");
//		//listboxitem->setTextColours(clBlack);
//		saveList->addItem(listboxitem);
//	}
//	
//
//	//END list of saves
//
//
//	
//	/*slWnd->setVisible(true);
//	slWnd->setEnabled(true);
//	slWnd->activate();*/
//	//slWnd->se
//}

void GameApp::loadGame(Ogre::String name)
{
    ZipSaveFile *cur = new ZipSaveFile(name,saveGamePath,SAVEGAME_EXTENSION);
	Ogre::String sgConfigFile = cur->getFileAsString("savegame.xml");
	if(sgConfigFile == "")
		return;
	//ab diesem Punkt wird versucht, zu laden
	

	resetApp();
	//copy the savegame to the target path and swich to it
	cur->copyArchive("tempsave",tempPath,SAVEGAME_EXTENSION,true,true);
	
	tempSaveGame = cur;

	//jetzt das xml parsen
	TiXmlDocument *doc = new TiXmlDocument();
	doc->Parse(sgConfigFile.c_str());
	/*<savegame name="erstmal test" playerlevel="newstart.lvl">*/
	TiXmlElement *savelem = doc->FirstChildElement("savegame");
	Ogre::String lvlname = savelem ->Attribute("playerlevel");
	loadLevel(lvlname,"",true,false);
    int playerSGID = StringConverter::parseInt(savelem ->Attribute("playersgid"));

    GameObject *maybePlayer = currentLevel->getObjectBySGID(playerSGID);
    if(!maybePlayer || maybePlayer->getType() != otChar)
        throw Ogre::Exception(0,"Error while loading savegame "+name+": Object with SGID "+TypeConverter::ogre_str(playerSGID)+" does not exist or is not a character!","GameApp::loadGame");
    GameChar *player = static_cast<GameChar*>(maybePlayer);//new GameChar(savelem->FirstChildElement("player"),currentLevel,Ogre::Vector3::ZERO,true);
	currentLevel->setPlayerControl(player);
    loadHotkeys(savelem->FirstChildElement("hotkeys"));
	//setDisplayedSpell(player->getSpell());
	//...
	//cur->closeZipFile();

	delete doc;
	
	
	setMenu(GUI_INGAME,false);
	mainMenu->setResumeEnabled(true);
	/*CEGUI::PushButton* resumeButton = (CEGUI::PushButton*)wmgr->getWindow((CEGUI::utf8*)"resume");
	resumeButton->enable();*/
}

//bool GameApp::savegameLoadClick(const CEGUI::EventArgs& e)
//{
//	CEGUI::Editbox *editbox = static_cast<CEGUI::Editbox*>(wmgr->getWindow("saveloadwnd/namebox"));
//	Ogre::String name = editbox->getText().c_str();
//	if(name == "")
//		return true;
//	
//	loadGame(name);
//	return true;
//}

void GameApp::saveGame(Ogre::String name)
{
	//copy the tempsave
	tempSaveGame->copyArchive(name,saveGamePath,SAVEGAME_EXTENSION,true,false);

	//open the copy
    ZipSaveFile sav(name,saveGamePath,SAVEGAME_EXTENSION);
	
	//add the current level and the generic savegame data file    
	sav.addTextFile(currentLevel->getSavegameXML(),"level/"+currentLevel->getBaseFileName()+".lvl",true);
	sav.addTextFile(getSavegameSettings(),"savegame.xml",true);
	
	
	sav.close();

}
//
//bool GameApp::savegameSaveClick(const CEGUI::EventArgs& e)
//{
//	if(!currentLevel)
//		return true;
//	
//	CEGUI::Editbox *editbox = static_cast<CEGUI::Editbox*>(wmgr->getWindow("saveloadwnd/namebox"));
//	Ogre::String name = editbox->getText().c_str();
//	if(name == "")
//		return true;
//	saveGame(name);
//    
//	//updateSaveLoadWnd();
//	return true;
//}
//bool GameApp::savegameDeleteClick(const CEGUI::EventArgs& e)
//{
//	CEGUI::ListboxItem* cur = static_cast<CEGUI::Listbox*>(wmgr->getWindow("saveloadwnd/savegamelist"))->getFirstSelectedItem();
//	if(!cur)
//		return true;
//	ZipSaveFile toDel(cur->getText().c_str(),saveGamePath,SAVEGAME_EXTENSION);
//	
//	toDel.removeFile();
////updateSaveLoadWnd();
//	return true;
//}

//bool GameApp::hideSaveLoadWnd(const CEGUI::EventArgs& e)
//{
//	setMenu(GUI_MAINMENU);
//	/*CEGUI::Window *slWnd = wmgr->getWindow("saveloadwnd");
//	slWnd->setEnabled(false);
//	slWnd->setVisible(false);*/
//	return true;
//}

void GameApp::showOtherInventory(Inventory *inv,GameObject *owner)
{
    if(!playerInventoryView->isShown())
        showInventory(true);
	//curOpenInvOwner = owner;
	//curOpenInventory = inv;
	
	setCursorMode(true);
    otherInventoryView->show(inv, owner);
	otherInvSquaredDist = owner->getPosition().squaredDistance(currentLevel->getPlayer()->getPosition());
	
}
void GameApp::hideOtherInventory()
{
//curOpenInvOwner = NULL;
	//curOpenInventory = NULL;
    otherInventoryView->hide();
	//otherInvOpen = false;
}

void GameApp::setBattleMusic(bool set)
{
	if(set != battleMusic)
	{
		currentLevel->setMusic(set);
		battleMusic = set;
	}
	
}


void GameApp::setupConfig(TiXmlElement *elem)
{
    //settings
	settings.loadSettings(elem->FirstChildElement("settings"));
  
	configLoaded = true;
//updateAudioWindow();
    
}

//converts a  member of the KeyName enum to a string, for xml saving purposes
Ogre::String GameApp::keyNameToString(unsigned int name)
{
	switch(name)
	{
	case keyNone:			return "None";
	case keyMoveForward:	return "MoveForward";
	case keyMoveBack:		return "MoveBack";
	case keyMoveRight:		return "MoveRight";
	case keyMoveLeft:		return "MoveLeft";
	case keyJump:			return "Jump";
	case keyAttack:			return "Attack";
	case keySpellcast:		return "Spellcast";
	case keyRun:			return "Run";
	case keyAction:			return "Action";
	case keyInventory:		return "Inventory";
	case keyInvTakeOne:		return "InvTakeOne";
	case keyInvTakeAll:		return "InvTakeAll";
	case keyInvUse:			return "InvUse";
	case keySpellbook:		return "Spellbook";
	case keyScreenshot:		return "Screenshot";
	case keyMainmenu:		return "Mainmenu";
	case keyLookUp:			return "LookUp";
	case keyLookDown:		return "LookDown";
	case keyLookLeft:		return "LookLeft";
	case keyLookRight:		return "LookRight";
	case keyToggleMouselooking: return "ToggleMouselooking";
	case keyDebug3rdp:		return "Debug3rdp";
	case keyDebugshowlines: return "Debugshowlines";
	case keyDragItem:		return "DragItem";
	};
	return "INVALID_KEY_NAME";
}

//converts a string with the key name to a member of the KeyName enum
KeyName GameApp::parseKeyName(Ogre::String str)
{
	if(str == "None")
		return keyNone;
	if(str == "MoveForward")
		return keyMoveForward;
	if(str == "MoveBack")
		return keyMoveBack;
	if(str == "MoveRight")
		return keyMoveRight;
	if(str == "MoveLeft")
		return keyMoveLeft;
	if(str == "Jump")
		return keyJump;
	if(str == "Attack")
		return keyAttack;
	if(str == "Spellcast")
		return keySpellcast;
	if(str == "Run")
		return keyRun;
	if(str == "Action")
		return keyAction;
	if(str == "Inventory")
		return keyInventory;
	if(str == "InvTakeOne")
		return keyInvTakeOne;
	if(str == "InvTakeAll")
		return keyInvTakeAll;
	if(str == "InvUse")
		return keyInvUse;
	if(str == "Spellbook")
		return keySpellbook;
	if(str == "Screenshot")
		return keyScreenshot;
	if(str == "Mainmenu")
		return keyMainmenu;
	if(str == "LookUp")
		return keyLookUp;
	if(str == "LookDown")
		return keyLookDown;
	if(str == "LookLeft")
		return keyLookLeft;
	if(str == "LookRight")
		return keyLookRight;
	if(str == "ToggleMouselooking")
		return keyToggleMouselooking;
	if(str == "Debug3rdp")
		return keyDebug3rdp;
	if(str == "Debugshowlines")
		return keyDebugshowlines;
	if(str == "DragItem")
		return keyDragItem;
	return keyNone;
}