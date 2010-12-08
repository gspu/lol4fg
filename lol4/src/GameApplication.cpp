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



////mem probs without this next one
#include <OgreNoMemoryMacros.h>
#include <CEGUI/CEGUIImageset.h>
#include <CEGUI/CEGUISystem.h>
#include <CEGUI/CEGUILogger.h>
#include <CEGUI/CEGUISchemeManager.h>
#include <CEGUI/CEGUIWindowManager.h>
#include <CEGUI/CEGUIWindow.h>
#include "OgreCEGUIRenderer.h"
#include "OgreCEGUIResourceProvider.h"
#include <CEGUI/elements/CEGUIPushButton.h>
#include <CEGUI/elements/CEGUIScrollablePane.h>
#include <CEGUI/elements/CEGUICombobox.h>
#include <CEGUI/elements/CEGUIListboxTextItem.h>
#include <CEGUI/elements/CEGUITabControl.h>
#include <CEGUI/elements/CEGUIScrolledContainer.h>
#include "OgreCEGUIRenderer.h"
#include "OgreCEGUIResourceProvider.h"

#include <CEGUI/CEGUITexture.h>
#include "ogreceguitexture.h"
//regular mem handler
#include <OgreMemoryMacros.h>
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
////regular mem handler
//#include <OgreMemoryMacros.h>
//------- END OGRE & stuff includes ----------

#include "FwDec.h"
//#include "game_obj.h"
#include "GameChar.h"
//#include "global_class.h"
#include "FrameListener.h"
#include "GameApplication.h"
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
////#include "OgreAL.h"

//#include "unzip.h"
//#include "zlib.h"
//#include "zip.h"
//#include "zip.h"
#include "zip.h"


using namespace std;
using namespace Ogre;
using namespace CEGUI;



GameApplication::GameApplication(Ogre::String cmdline)
{
	tempSaveGame = NULL;
	configLoaded = false;
	DebugDoOnce = false;
	screenTextShown = false;
	screenTextTime = 0;
	waitingUntilUnpause = 0;
	battleMusic = false;
	defaultMusic = "default.ogg";
//	musicPlayTime = 0;
	zeroTime = false;
	defaultCombatMusic = "default.ogg";
	gainMusic = 1;
	gainSFX   = 1;
	gainSpeech = 1;
	music = NULL;
	compassWnd = NULL;
	
	ambient1 = NULL;
	ambient2 = NULL;
	playersfx = NULL;
//	unpauseWaitTime = 0;
	screenshotBase = "lol4_";
	inventoryOpen = false;
	spellbookOpen = false;
	showFog = true;
	otherInventorySize = 0;
	otherInventoryWnd = NULL;
	otherInvOpen = false;
	curHoldingContainer = NULL;
	curHoldingInventory = NULL;
	curOpenInventory = NULL;
	//first, get the params
	paramList = StringUtil::split(cmdline," ");
	showConfigDialog = false;
	playerInventorySize = 0;
	sllLevel="";
	sllEntrance="";
	sll = false;
	spellbookSize = 0;
	curHoldingSpell = NULL;
	curHolding.count = 0;
	curHolding.item = NULL;

	startingLevel = "start.lvl";
	startingEntrance = "GameStartPosition";




	unpauseNextFrame = false;

	currentLevel = NULL;
	oldLevel = NULL;

    mFrameListener = 0;

    mRoot = 0;



	sensibility_x = 0.25;
	sensibility_y = 0.25;

	sensibility_x_keyboard = 1;
	sensibility_y_keyboard = 20;

//	gravity = -9.81;
	menuMode = GUI_DISABLE;
	paused = false;
	shutDown = false;
	cursorMode = false;
	useMouseLooking = true;
//	testguy = 0;
//	player = 0;
	mGUISystem = NULL;
	mGUIRenderer = NULL;
	mIngameGui = NULL;
	mMainMenuGui = NULL;
	mWindow = NULL;





	//NewtonSetWorldSize(mWorld->getNewtonWorld(),&p1,&p2);
//	mWorld->setWorldSize(Vector3(-1000,-1000,-1000),Vector3(1000,1000,1000));


}

GameApplication::~GameApplication()
{

	saveConfig();
	
	
	if(mIngameGui)
	{
		wmgr->destroyWindow(mIngameGui);
	}
	if(mGUISystem)
	{
		delete mGUISystem;
		mGUISystem = 0;
	}
	if(mGUIRenderer)
	{
		delete mGUIRenderer;
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


	////delete mKeyListener;
	//if(mEventProcessor)
	//	delete mEventProcessor;

	Debugger::getSingleton().deInit();
	delete currentLevel;


	if(soundMgr)
		delete soundMgr;

	//delete mOgreNewtListener;
	//if(player)
	//	delete player;
	//if(testguy)
	//	delete testguy;
	//if(mWorld)
	//	delete mWorld;
//	log("ok, aus2");


	//OgreNewt::Debugger::getSingleton().deInit();
//	log("ok, aus2,5");
	//objects.clear();
	/*if(plsm2SceneMgr)
		mRoot->destroySceneManager(plsm2SceneMgr);*/
//	log("ok, aus3");

	if (mRoot)
        delete mRoot;
	//log("ok, aus4");

}
//
//void GameApplication::go(void)
//{
//
//    if (!setup())
//        return;
//
//    mRoot->startRendering();
//
//    // clean up
//    destroyScene();
//}

bool GameApplication::setup(void)
{


    //mRoot = new Root();
	mRoot = new Root(mPluginFileName,mConfigFileName,mLogFileName);

//	log("setup in GA");

	//root ist da, jetzt kann ich loggen
	for(vector<Ogre::String>::iterator itr = paramList.begin();itr != paramList.end();itr++)
	{
		Ogre::String param = *itr;
		if(param == "-config")
		{
			showConfigDialog = true;
		}
		LogManager::getSingleton().logMessage("param="+param);
	}

	//mWorld = new OgreNewt::World();

    setupResources();
//	log("resourcen gesetzt");

    bool carryOn = configure();
    if (!carryOn) return false;

	createResourceListener();
	// Load resources
	loadResources();


	loadGameData();


	createDefaultSceneMgr();








	createGUI();
//	log("nach createGUI();");
	setMenu(GUI_MAINMENU);
    createFrameListener();
	loadConfig();

	
	initSound();
	//soundMgr = new OgreAL::SoundManager();
	/*music = soundMgr->createSound("BackgroundMusic",defaultMusic,true);
	music->setGain(gainMusic);
	music->setRelativeToListener(true);*/
	//music->play();
	

    return true;

}

void GameApplication::initSound()
{
	//SOUND 
	soundMgr = new SimpleSound::SoundManager();
	soundMgr->setDefaultMaxDist(maxDistance);
	soundMgr->setDefaultRefDistance(refDistance);
	soundMgr->setDefaultRolloffFactor(rollofFactor);

	soundMgr->setDistanceModel(distModel);
	soundMgr->getListener()->setGain(gainAll);

	clickSnd = soundMgr->createBuffer("click.wav");
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
void GameApplication::setMusic(SimpleSound::Buffer *buff, bool loop)
{
//	
//	
//	//setDebugText("ändere musik in "+snd->getFileName());
	
	music->setBuffer(buff);
	if(buff)
	{
		music->setLoop(loop);
		music->setGain(gainMusic);
		music->play();
	}
//		music->stop();
//		//soundMgr->destroySound(music);
//		music = NULL;
	
	
//	music = snd;
//	if(!music)
//		return;
//
//	
//	music->setGain(gainMusic);
//	music->setRelativeToListener(true);
//	music->setPriority(OgreAL::Sound::HIGH);
//	if(!paused)
//		music->play();
}

void GameApplication::playPlayerSFX(SimpleSound::Buffer *buff)
{
	////1x spielen und aus
	
	
	playersfx->setBuffer(buff);
	if(buff)
	{
		playersfx->setLoop(false);
		playersfx->setGain(gainSFXPlayer);
		playersfx->play();
	}

	//if(filename == "")
	//	return;

	//playersfx = soundMgr->createSound("PlayerSFX",filename,false);
	//playersfx->setGain(gainSFX);
	//playersfx->setRelativeToListener(true);
	//playersfx->play();
}

void GameApplication::setAmbient1(SimpleSound::Buffer *buff)
{
	
	if(!ambient1->isStopped())
		ambient1->stop();
	ambient1->setBuffer(buff);
	if(buff)
	{
		ambient1->setLoop(false);
		ambient1->setGain(gainSFX);
		ambient1->play();
	}
	//mLog("setAmbient1("+filename+")");
	////nur 1x spielen...
	//if(ambient1)
	//{
	//	ambient1->stop();
	//	soundMgr->destroySound(ambient1);
	//	ambient1 = NULL;
	//}
	//if(filename == "")
	//{
	//	return;
	//}
	//ambient1 = soundMgr->createSound("Ambient1",filename,false);
	//ambient1->setGain(gainSFX);
	//ambient1->setRelativeToListener(true);
	//
	//if(!paused)
	//	ambient1->play();
}
void GameApplication::setAmbient2(SimpleSound::Buffer *buff,bool loop)
{
	
	if(!ambient2->isStopped())
		ambient2->stop();
	ambient2->setBuffer(buff);
	if(buff)
	{
		ambient2->setLoop(loop);
		ambient2->setGain(gainSFX);
		ambient2->play();
	}
	////nur 1x spielen...
	//if(ambient2)
	//{
	//	ambient2->stop();
	//	soundMgr->destroySound(ambient2);
	//	ambient2 = NULL;
	//}
	//if(filename == "")
	//{
	//	return;
	//}
	//ambient2 = soundMgr->createSound("Ambient2",filename,loop);
	//ambient2->setGain(gainSFX);
	//ambient2->setRelativeToListener(true);
	//
	//if(!paused)
	//	ambient2->play();
}


void GameApplication::setPaused(bool set)
{
	
	paused = set;
	if(soundMgr)
	{
		if(paused)
			soundMgr->pauseAllSources();
		else
			soundMgr->resumeAllSources();
	}
	/*if(music)
	{
		if(set)
		{
			if(music)
				music->pause();
			if(ambient1)
				ambient1->pause();
			if(ambient2)
				ambient2->pause();
		}
		else
		{
			if(music)
				music->play();
			if(ambient1)
				ambient1->play();
			if(ambient2)
				ambient2->play();
		}
	}*/
}

void GameApplication::loadConfig()
{
	ConfigFile cf;
	cf.load("game.cfg");
	
	
	// Go through all sections & settings in the file
	ConfigFile::SectionIterator seci = cf.getSectionIterator();


	Ogre::String secName, varName, value;
	//CEGUI::WindowManager *wmgr = CEGUI::WindowManager::getSingletonPtr();
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		ConfigFile::SettingsMultiMap *settings = seci.getNext();
		ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			varName = i->first;
			value = i->second;	
			if(secName == "settings")
			{
				if(varName == "mouselooking")
					useMouseLooking = StringConverter::parseBool(value);
				else if(varName == "sensibility_x")
					sensibility_x = StringConverter::parseReal(value);
				else if(varName == "sensibility_y")
					sensibility_y = StringConverter::parseReal(value);
				else if(varName == "sensibility_x_keyboard")
					sensibility_x_keyboard = StringConverter::parseReal(value);
				else if(varName == "sensibility_y_keyboard")
					sensibility_y_keyboard = StringConverter::parseReal(value);
				else if(varName == "gain_music")
					gainMusic = StringConverter::parseReal(value);
				else if(varName == "gain_sfx")
					gainSFX = StringConverter::parseReal(value);
				else if(varName == "gain_player_sfx")
					gainSFXPlayer = StringConverter::parseReal(value);
				else if(varName == "gain_speech")
					gainSpeech = StringConverter::parseReal(value);
				else if(varName == "rollof_factor")
					rollofFactor = StringConverter::parseReal(value);
				else if(varName == "ref_distance")
					refDistance = StringConverter::parseReal(value);
				else if(varName == "max_distance")
					maxDistance = StringConverter::parseReal(value);
				

				
	
	
			}
			else if(secName == "game")
			{
				if(varName == "starting_level")				
					startingLevel = value;
				else if(varName == "starting_entrance")				
					startingEntrance = value;
				else if(varName == "default_music")
					defaultMusic = value;
				else if(varName == "default_combat_music")
					defaultCombatMusic = value;
			}
			else if(secName == "keys_main")
			{				
								if(varName == "MoveFoward")				
					mFrameListener->mainKeys[CFrameListener::keyMoveForward] = stringToUniKey(value);
				else if(varName == "MoveBack")				
					mFrameListener->mainKeys[CFrameListener::keyMoveBack] = stringToUniKey(value); 
				else if(varName == "MoveRight")				
					mFrameListener->mainKeys[CFrameListener::keyMoveRight] = stringToUniKey(value); 
				else if(varName == "MoveLeft")				
					mFrameListener->mainKeys[CFrameListener::keyMoveLeft] = stringToUniKey(value);
				else if(varName == "Jump")				
					mFrameListener->mainKeys[CFrameListener::keyJump] = stringToUniKey(value); 
				else if(varName == "Attack")				
					mFrameListener->mainKeys[CFrameListener::keyAttack] = stringToUniKey(value);
				else if(varName == "Spellcast")				
					mFrameListener->mainKeys[CFrameListener::keySpellcast] = stringToUniKey(value); 
				else if(varName == "Run")				
					mFrameListener->mainKeys[CFrameListener::keyRun] = stringToUniKey(value); 
				else if(varName == "Action")				
					mFrameListener->mainKeys[CFrameListener::keyAction] = stringToUniKey(value); 
				else if(varName == "Inventory")				
					mFrameListener->mainKeys[CFrameListener::keyInventory] = stringToUniKey(value); 
				else if(varName == "Spellbook")				
					mFrameListener->mainKeys[CFrameListener::keySpellbook] = stringToUniKey(value); 
				else if(varName == "InvTakeOne")				
					mFrameListener->mainKeys[CFrameListener::keyInvTakeOne] = stringToUniKey(value); 
				else if(varName == "InvTakeAll")				
					mFrameListener->mainKeys[CFrameListener::keyInvTakeAll] = stringToUniKey(value); 
				else if(varName == "InvUse")				
					mFrameListener->mainKeys[CFrameListener::keyInvUse] = stringToUniKey(value); 
				else if(varName == "Screenshot")				
					mFrameListener->mainKeys[CFrameListener::keyScreenshot] = stringToUniKey(value); 
				else if(varName == "Mainmenu")				
					mFrameListener->mainKeys[CFrameListener::keyMainmenu] = stringToUniKey(value); 
				else if(varName == "LookUp")				
					mFrameListener->mainKeys[CFrameListener::keyLookUp] = stringToUniKey(value); 
				else if(varName == "LookDown")				
					mFrameListener->mainKeys[CFrameListener::keyLookDown] = stringToUniKey(value); 
				else if(varName == "LookLeft")				
					mFrameListener->mainKeys[CFrameListener::keyLookLeft] = stringToUniKey(value); 
				else if(varName == "LookRight")				
					mFrameListener->mainKeys[CFrameListener::keyLookRight] = stringToUniKey(value); 
				else if(varName == "ToggleMouselooking")				
					mFrameListener->mainKeys[CFrameListener::keyToggleMouselooking] = stringToUniKey(value);
				else if(varName == "Debug3rdp")				
					mFrameListener->mainKeys[CFrameListener::keyDebug3rdp] = stringToUniKey(value);
				else if(varName == "Debugshowlines")				
					mFrameListener->mainKeys[CFrameListener::keyDebugshowlines] = stringToUniKey(value);
								
			}
			else if(secName == "keys_alt")
			{
				if(varName == "MoveFoward")				
					mFrameListener->altKeys[CFrameListener::keyMoveForward] = stringToUniKey(value);
				else if(varName == "MoveBack")				
					mFrameListener->altKeys[CFrameListener::keyMoveBack] = stringToUniKey(value); 
				else if(varName == "MoveRight")				
					mFrameListener->altKeys[CFrameListener::keyMoveRight] = stringToUniKey(value); 
				else if(varName == "MoveLeft")				
					mFrameListener->altKeys[CFrameListener::keyMoveLeft] = stringToUniKey(value);
				else if(varName == "Jump")				
					mFrameListener->altKeys[CFrameListener::keyJump] = stringToUniKey(value); 
				else if(varName == "Attack")				
					mFrameListener->altKeys[CFrameListener::keyAttack] = stringToUniKey(value);
				else if(varName == "Spellcast")				
					mFrameListener->altKeys[CFrameListener::keySpellcast] = stringToUniKey(value); 
				else if(varName == "Run")				
					mFrameListener->altKeys[CFrameListener::keyRun] = stringToUniKey(value); 
				else if(varName == "Action")				
					mFrameListener->altKeys[CFrameListener::keyAction] = stringToUniKey(value); 
				else if(varName == "Inventory")				
					mFrameListener->altKeys[CFrameListener::keyInventory] = stringToUniKey(value); 
				else if(varName == "Spellbook")				
					mFrameListener->altKeys[CFrameListener::keySpellbook] = stringToUniKey(value); 
				else if(varName == "InvTakeOne")				
					mFrameListener->altKeys[CFrameListener::keyInvTakeOne] = stringToUniKey(value); 
				else if(varName == "InvTakeAll")				
					mFrameListener->altKeys[CFrameListener::keyInvTakeAll] = stringToUniKey(value); 
				else if(varName == "InvUse")				
					mFrameListener->altKeys[CFrameListener::keyInvUse] = stringToUniKey(value); 
				else if(varName == "Screenshot")				
					mFrameListener->altKeys[CFrameListener::keyScreenshot] = stringToUniKey(value); 
				else if(varName == "Mainmenu")				
					mFrameListener->altKeys[CFrameListener::keyMainmenu] = stringToUniKey(value); 
				else if(varName == "LookUp")				
					mFrameListener->altKeys[CFrameListener::keyLookUp] = stringToUniKey(value); 
				else if(varName == "LookDown")				
					mFrameListener->altKeys[CFrameListener::keyLookDown] = stringToUniKey(value); 
				else if(varName == "LookLeft")				
					mFrameListener->altKeys[CFrameListener::keyLookLeft] = stringToUniKey(value); 
				else if(varName == "LookRight")				
					mFrameListener->altKeys[CFrameListener::keyLookRight] = stringToUniKey(value); 
				else if(varName == "ToggleMouselooking")				
					mFrameListener->altKeys[CFrameListener::keyToggleMouselooking] = stringToUniKey(value);
				else if(varName == "Debug3rdp")				
					mFrameListener->altKeys[CFrameListener::keyDebug3rdp] = stringToUniKey(value);
				else if(varName == "Debugshowlines")				
					mFrameListener->altKeys[CFrameListener::keyDebugshowlines] = stringToUniKey(value);

			}
			
		}//von for
	}
	configLoaded = true;
	updateAudioWindow();
}

void GameApplication::saveConfig()
{
	if(!configLoaded)
		return;
	std::ofstream of("game.cfg");

    if (!of)
		OGRE_EXCEPT(Ogre::Exception::ERR_CANNOT_WRITE_TO_FILE, "Cannot create settings file.",
        "GameApplication::saveConfig");

	/*
				

				
	
	
			}
			else if(secName == "game")
			{
				if(varName == "starting_level")				
					startingLevel = value;
				*/

	of << "[settings]" << std::endl;
	of << "mouselooking="<< str(useMouseLooking) << std::endl;
	of << "sensibility_x="<< str(sensibility_x) << std::endl;
	of << "sensibility_y="<< str(sensibility_y) << std::endl;
	of << "sensibility_x_keyboard="<< str(sensibility_x_keyboard) << std::endl;
	of << "gain_sfx="<< str(gainSFX) << std::endl;
	of << "gain_speech="<< str(gainSpeech) << std::endl;
	of << "gain_music="<< str(gainMusic) << std::endl;	
	of << "gain_player_sfx=" <<str(gainSFXPlayer)<<std::endl;
	of << "rollof_factor=" << str(rollofFactor) << std::endl;
	of << "ref_distance=" <<str(refDistance) << std::endl;
	of << "max_distance=" << str(maxDistance) << std::endl;
				
	of << std::endl;
  
	of << "[game]" << std::endl;
	of << "starting_level=" << startingLevel << std::endl;
	of << "starting_entrance=" << startingEntrance << std::endl;
	of << "default_music=" << defaultMusic << std::endl;
	of << "default_combat_music=" << defaultCombatMusic << std::endl;
	of << std::endl;

	of << "[keys_main]" << std::endl;
	of << "MoveFoward=" << uniKeyToString(mFrameListener->mainKeys[CFrameListener::keyMoveForward]) << std::endl;
	of << "MoveBack=" << uniKeyToString(mFrameListener->mainKeys[CFrameListener::keyMoveBack]) << std::endl; 
	of << "MoveRight=" << uniKeyToString(mFrameListener->mainKeys[CFrameListener::keyMoveRight]) << std::endl; 
	of << "MoveLeft=" << uniKeyToString(mFrameListener->mainKeys[CFrameListener::keyMoveLeft]) << std::endl;
	of << "Jump=" << uniKeyToString(mFrameListener->mainKeys[CFrameListener::keyJump]) << std::endl; 
	of << "Attack=" << uniKeyToString(mFrameListener->mainKeys[CFrameListener::keyAttack]) << std::endl;
	of << "Spellcast=" << uniKeyToString(mFrameListener->mainKeys[CFrameListener::keySpellcast]) << std::endl; 
	of << "Run=" << uniKeyToString(mFrameListener->mainKeys[CFrameListener::keyRun]) << std::endl; 
	of << "Action=" << uniKeyToString(mFrameListener->mainKeys[CFrameListener::keyAction]) << std::endl; 
	of << "Inventory=" << uniKeyToString(mFrameListener->mainKeys[CFrameListener::keyInventory]) << std::endl; 
	of << "InvTakeOne=" << uniKeyToString(mFrameListener->mainKeys[CFrameListener::keyInvTakeOne]) << std::endl; 
	of << "InvTakeAll=" << uniKeyToString(mFrameListener->mainKeys[CFrameListener::keyInvTakeAll]) << std::endl; 
	of << "InvUse=" << uniKeyToString(mFrameListener->mainKeys[CFrameListener::keyInvUse]) << std::endl; 
	of << "Screenshot=" << uniKeyToString(mFrameListener->mainKeys[CFrameListener::keyScreenshot]) << std::endl; 
	of << "Mainmenu=" << uniKeyToString(mFrameListener->mainKeys[CFrameListener::keyMainmenu]) << std::endl; 
	of << "LookUp=" << uniKeyToString(mFrameListener->mainKeys[CFrameListener::keyLookUp]) << std::endl; 
	of << "LookDown=" << uniKeyToString(mFrameListener->mainKeys[CFrameListener::keyLookDown]) << std::endl; 
	of << "LookLeft=" << uniKeyToString(mFrameListener->mainKeys[CFrameListener::keyLookLeft]) << std::endl; 
	of << "LookRight=" << uniKeyToString(mFrameListener->mainKeys[CFrameListener::keyLookRight]) << std::endl; 
	of << "ToggleMouselooking=" << uniKeyToString(mFrameListener->mainKeys[CFrameListener::keyToggleMouselooking]) << std::endl;
	of << "Debug3rdp=" << uniKeyToString(mFrameListener->mainKeys[CFrameListener::keyDebug3rdp]) << std::endl;
	of << "Debugshowlines=" << uniKeyToString(mFrameListener->mainKeys[CFrameListener::keyDebugshowlines]) << std::endl;
	of << std::endl;

	of << "[keys_alt]" << std::endl;
	of << "MoveFoward=" << uniKeyToString(mFrameListener->altKeys[CFrameListener::keyMoveForward]) << std::endl;
	of << "MoveBack=" << uniKeyToString(mFrameListener->altKeys[CFrameListener::keyMoveBack]) << std::endl; 
	of << "MoveRight=" << uniKeyToString(mFrameListener->altKeys[CFrameListener::keyMoveRight]) << std::endl; 
	of << "MoveLeft=" << uniKeyToString(mFrameListener->altKeys[CFrameListener::keyMoveLeft]) << std::endl;
	of << "Jump=" << uniKeyToString(mFrameListener->altKeys[CFrameListener::keyJump]) << std::endl; 
	of << "Attack=" << uniKeyToString(mFrameListener->altKeys[CFrameListener::keyAttack]) << std::endl;
	of << "Spellcast=" << uniKeyToString(mFrameListener->altKeys[CFrameListener::keySpellcast]) << std::endl; 
	of << "Run=" << uniKeyToString(mFrameListener->altKeys[CFrameListener::keyRun]) << std::endl; 
	of << "Action=" << uniKeyToString(mFrameListener->altKeys[CFrameListener::keyAction]) << std::endl; 
	of << "Inventory=" << uniKeyToString(mFrameListener->altKeys[CFrameListener::keyInventory]) << std::endl; 
	of << "InvTakeOne=" << uniKeyToString(mFrameListener->altKeys[CFrameListener::keyInvTakeOne]) << std::endl; 
	of << "InvTakeAll=" << uniKeyToString(mFrameListener->altKeys[CFrameListener::keyInvTakeAll]) << std::endl; 
	of << "InvUse=" << uniKeyToString(mFrameListener->altKeys[CFrameListener::keyInvUse]) << std::endl; 
	of << "Screenshot=" << uniKeyToString(mFrameListener->altKeys[CFrameListener::keyScreenshot]) << std::endl; 
	of << "Mainmenu=" << uniKeyToString(mFrameListener->altKeys[CFrameListener::keyMainmenu]) << std::endl; 
	of << "LookUp=" << uniKeyToString(mFrameListener->altKeys[CFrameListener::keyLookUp]) << std::endl; 
	of << "LookDown=" << uniKeyToString(mFrameListener->altKeys[CFrameListener::keyLookDown]) << std::endl; 
	of << "LookLeft=" << uniKeyToString(mFrameListener->altKeys[CFrameListener::keyLookLeft]) << std::endl; 
	of << "LookRight=" << uniKeyToString(mFrameListener->altKeys[CFrameListener::keyLookRight]) << std::endl; 
	of << "ToggleMouselooking=" << uniKeyToString(mFrameListener->altKeys[CFrameListener::keyToggleMouselooking]) << std::endl;
	of << "Debug3rdp=" << uniKeyToString(mFrameListener->altKeys[CFrameListener::keyDebug3rdp]) << std::endl;
	of << "Debugshowlines=" << uniKeyToString(mFrameListener->altKeys[CFrameListener::keyDebugshowlines]) << std::endl;

	

	of.close();
}

bool GameApplication::configure(void)
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
	//		for(StringVector::iterator it3 = opt.possibleValues.begin();it3 != opt.possibleValues.end();it3++)
	//		{
	//			String val = *it3;
	//			log("---"+val);
	//
	//		}
	//	}
	//}
	if(!showConfigDialog)
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


	}

	if(showConfigDialog)
	{
		if(!mRoot->showConfigDialog())
		{
			return false;
		}
	}






	mWindow = mRoot->initialise(true, WINDOW_NAME);
	newRenderer = mRoot->getRenderSystem()->getName();
	curSelectedRenderer = newRenderer;
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
//void GameApplication::chooseSceneManager(void)
//{
//// Create the SceneManager, in this case a generic one
////-----------------------------------------------------
//// 4 Create the SceneManager
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

//void GameApplication::createCamera(void)
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
//void GameApplication::createMaterials(void)
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

//void GameApplication::createPlayer(void)
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
void GameApplication::createFrameListener(void)
{
    //mFrameListener= new CFrameListener(mWindow, mCamera);
    //mFrameListener->showDebugOverlay(true);


	

	/*mFrameListener = new CFrameListener(this,mEventProcessor->getInputReader(),mGUIRenderer,mWindow, mSceneMgr, mWorld, 120 );*/
	mFrameListener = new CFrameListener();//this,mEventProcessor->getInputReader(),mGUIRenderer);//, 120 );

	mRoot->addFrameListener(mFrameListener);
//
//	log("frame listener online");


}

//void GameApplication::createScene(void)
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

//void GameApplication::destroyScene(void){}    // Optional to override this

//void GameApplication::createViewports(void)
//{
//
//    //// Create one viewport, entire window
//    //Viewport* vp = mWindow->addViewport(mCamera);
//    //vp->setBackgroundColour(ColourValue(0,0,0));
//
//    //// Alter the camera aspect ratio to match the viewport
//    //mCamera->setAspectRatio(
//    //    Real(vp->getActualWidth()) / Real(vp->getActualHeight()));
//}

/// Method which will define the source of resources (other than current folder)
//void GameApplication::setupResources(void)
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
//	LogManager::getSingleton().logMessage("muh");
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

void GameApplication::createResourceListener(void)
{

}

/// Optional override method where you can perform resource group loading
/// Must at least do ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
//void GameApplication::loadResources(void)
//{
//	// Initialise, parse scripts etc
//	ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
//	log("res init done");
//
//
//}

void GameApplication::showInfoText(Ogre::String txt, Real rel_x,Real rel_y)
{
	if(!infoText->isVisible())
	{
		infoText->setVisible(true);
	}
	if(infoText->isDisabled())
	{
		infoText->enable();
	}
	Real x,y;
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
		Real width = fnt->getTextExtent(str)+6;

		//CEGUI::Size mSize = mIngameGui->absoluteToRelative(CEGUI::Size(width,0.05f));
		//
		//mSize.d_height = 0.05f;
		//infoText->setSize(mSize);
		infoText->setSize(CEGUI::UVector2(CEGUI::UDim(0,width),CEGUI::UDim(0,30)));


	}

	infoText->setPosition(UVector2(UDim(0.5-infoText->getWidth().d_scale/2,0),UDim(y,0)));//CEGUI::Point(0.5-infoText->getRelativeWidth()/2,y));


}
void GameApplication::updateInfoText(Real rel_x, Real rel_y)
{
	infoText->setPosition(UVector2(UDim(rel_x,0),UDim(rel_y,0)));//CEGUI::Point(rel_x,rel_y));
}
void GameApplication::hideInfoText()
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

void GameApplication::updateAudioWindow()
{
	setScrollbarPos((CEGUI::Scrollbar*)(wmgr->getWindow("options/gainAll")),gainAll);

	setScrollbarPos((CEGUI::Scrollbar*)(wmgr->getWindow("options/gainMusic")),gainMusic);
	setScrollbarPos((CEGUI::Scrollbar*)(wmgr->getWindow("options/gainSFX")),gainSFX/2);
	setScrollbarPos((CEGUI::Scrollbar*)(wmgr->getWindow("options/gainSFXPlayer")),gainSFXPlayer);
	setScrollbarPos((CEGUI::Scrollbar*)(wmgr->getWindow("options/gainSpeech")),gainSpeech);

	
	wmgr->getWindow("options/distance_model")->setText(distanceModelToString(distModel).c_str() );

	wmgr->getWindow("options/maxdist")->setText(str(maxDistance).c_str());
	wmgr->getWindow("options/rolloff")->setText(str(rollofFactor).c_str());
	wmgr->getWindow("options/refdist")->setText(str(refDistance).c_str());
}
void GameApplication::updateOptionsWindow()
{
	//jetzt video
	updateVideoWindow();
	//Jetzt Audio
	updateAudioWindow();
	//jetzt gamesettings
	//
	setScrollbarPos((CEGUI::Scrollbar*)(wmgr->getWindow("options/sens_x")),sensibility_x/2);
	setScrollbarPos((CEGUI::Scrollbar*)(wmgr->getWindow("options/sens_y")),sensibility_y/2);
	static_cast<CEGUI::Checkbox*>(wmgr->getWindow("options/mouselooking"))->setSelected(useMouseLooking);
}
//
//void GameApplication::saveGameBegin(String name)
//{
//	if(saveGameZip)
//		return;
//	saveGameZip= CreateZip(name+".zip",0);
//}
//void GameApplication::saveGameFinish()
//{
//	if(!saveGameZip)
//		return;
//	CloseZip(saveGameZip);
//	//DataStreamPtr file = Ogre::ResourceGroupManager::getSingleton().openResource("start.lvl", "General");
//	//HZIP hz = CreateZip("test.zip",0);
//	//void *buf = (void*)malloc(file->size());
//	//file->read(buf,file->size());
//	//ZipAdd(hz,"test\start.lvl", buf,file->size());
//	//CloseZip(hz);
//	////delete buf;
//	//free(buf);
//}
//void GameApplication::saveGameAddFile(void *buf,size_t size,Ogre::String fileName)
//{
//	if(!saveGameZip)
//		return;
//	ZipAdd(saveGameZip,fileName.c_str(), buf,size);
//	
//}
//void GameApplication::saveGameAddFileFromDisk(Ogre::String fileNameSrc,Ogre::String fileNameDest)
//{
//	if(!saveGameZip)
//		return;
//	ZipAdd(saveGameZip,fileNameDest.c_str(),fileNameSrc.c_str());
//	
//}

void GameApplication::updateStatBars(Real hp, Real mp, int maxHP, int maxMP)
{
	Real manaFactor = mp/float(maxMP);
	Real hpFactor = hp/float(maxHP);

	UDim newHPWidth = hpBarWidth;
	newHPWidth.d_scale *= hpFactor;

	UDim newMPWidth = manaBarWidth;
	newMPWidth.d_scale *= manaFactor;

	manaBar->setWidth(newMPWidth);
	hpBar->setWidth(newHPWidth);

	
	Ogre::String text = str(Math::Floor(mp))+"/"+str(maxMP);
	manaBar_container->setTooltipText(text.c_str());
	

	text = str(Math::Floor(hp))+"/"+str(maxHP);
	hpBar_container->setTooltipText(text.c_str());
}
void GameApplication::updateCompass()
{
	//return;

	Quaternion ornt = currentLevel->getPlayer()->getOrientation();
	
	
	compNeedleNode->setOrientation(currentLevel->getNorthOrientation() * ornt.Inverse());
	
	
}


void GameApplication::createCompass()
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

	
	
	compassCam->setAspectRatio(Real(width) / Real(height));
	compassCam->setPosition(0,1.3,0);
	
	compassCam->pitch(Degree(-90));
	compassCam->setNearClipDistance(0.01);
	
	RenderTarget *rttTex = renderTex->getBuffer()->getRenderTarget();
	Viewport *v = rttTex->addViewport(compassCam);
//	v->setv
	v->setClearEveryFrame(true);
	v->setBackgroundColour(ColourValue(0,0,0,0));
	
    

    MaterialPtr mat = MaterialManager::getSingleton().create("CompassRttMat",
        ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	TextureUnitState* t = mat->getTechnique(0)->getPass(0)->createTextureUnitState("CompassRttTex");
    
  
	t->setTextureAddressingMode(TextureUnitState::TAM_CLAMP);
	
	CEGUI::Texture* ceguiTex = mGUIRenderer->createTexture(renderTex);

	CEGUI::String imageSetName = "compassimgset";
	Imageset* textureImageSet = ImagesetManager::getSingleton().createImageset(imageSetName, ceguiTex);

	textureImageSet->defineImage("compassneedle", Point(0.0f, 0.0f), Size(ceguiTex->getWidth(), ceguiTex->getHeight()), Point(0.0f,0.0f));
	

	compassWnd->setProperty("Image", "set:"+imageSetName+" image:compassneedle");
	
	//MUUUUUUUH
	//
	//RenderTarget *rttTex = texture->getBuffer()->getRenderTarget();
 //   {
 //       mReflectCam = mSceneMgr->createCamera("ReflectCam");
 //       mReflectCam->setNearClipDistance(mCamera->getNearClipDistance());
 //       mReflectCam->setFarClipDistance(mCamera->getFarClipDistance());
 //       mReflectCam->setAspectRatio(
 //           (Real)mWindow->getViewport(0)->getActualWidth() / 
 //           (Real)mWindow->getViewport(0)->getActualHeight());
	//	mReflectCam->setFOVy (mCamera->getFOVy());


 //       Viewport *v = rttTex->addViewport( mReflectCam );
 //       v->setClearEveryFrame( true );
 //       v->setBackgroundColour( ColourValue::Black );

 //       MaterialPtr mat = MaterialManager::getSingleton().create("RttMat",
 //           ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
 //       TextureUnitState* t = mat->getTechnique(0)->getPass(0)->createTextureUnitState("RustedMetal.jpg");
 //       t = mat->getTechnique(0)->getPass(0)->createTextureUnitState("RttTex");
 //       // Blend with base texture
 //       t->setColourOperationEx(LBX_BLEND_MANUAL, LBS_TEXTURE, LBS_CURRENT, ColourValue::White, 
 //           ColourValue::White, 0.25);
	//	t->setTextureAddressingMode(TextureUnitState::TAM_CLAMP);
	//	t->setProjectiveTexturing(true, mReflectCam);
 //       rttTex->addListener(this);

 //       // set up linked reflection
 //       mReflectCam->enableReflection(mPlane);
 //       // Also clip
 //       mReflectCam->enableCustomNearClipPlane(mPlane);
 //   }

	//compSceneMgr= mRoot->createSceneManager(ST_GENERIC);
	//compNeedleNode = compSceneMgr->getRootSceneNode()->createChildSceneNode();
	//Entity *ent = compSceneMgr->createEntity("compassent","CompassNeedle.mesh");
	////ent->setMaterialName("transform/red");
	//SceneNode *meshNode = compNeedleNode->createChildSceneNode();
	//meshNode->attachObject(ent);
	////meshNode->roll(Degree(90));
	////compNeedleNode->setScale(Ogre::Vector3(0.5));
	//compassCam = compSceneMgr->createCamera("compasscam");
	//compassWnd = wmgr->getWindow("compass");
	//unsigned int width = compassWnd->getPixelSize().d_width;
	//unsigned int height = compassWnd->getPixelSize().d_height;
	//
	//Ogre::String texName = "compassrtt";
	//RenderTexture *rtt = mRoot->getRenderSystem()->createRenderTexture(texName, width, height, TEX_TYPE_2D, PF_R8G8B8A8);
	//
	////Ogre::TexturePtr ogreTex= TextureManager::getSingletonPtr()->createManual(texName,ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,TEX_TYPE_2D,width,height,1,MIP_DEFAULT,PF_R8G8B8,TU_RENDERTARGET);
	//compassCam->setAspectRatio(Real(width) / Real(height));
	//compassCam->setPosition(0,1.2,0);
	////compassCam->setDirection(Ogre::Vector3::NEGATIVE_UNIT_Y);
	//compassCam->pitch(Degree(-90));
	////compassCam->lookAt(0,0,0);
	////cam->setPosition(0,100,0);
	////cam->lookAt(0,-100,0);
	//compassCam->setNearClipDistance(0.01);
	////compassCam->setFarClipDistance(5000);

	////Ogre:: MaterialPtr mMat = Ogre::MaterialManager::getSingletonPtr()->create(
	////	"test", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );
	////mMat->setSceneBlending( Ogre::SBT_TRANSPARENT_COLOUR ); 


	///*matName = "RttCompassMat");
	//MaterialPtr material = MaterialManager::getSingleton().create(matName, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	//material->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
	//material->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false); */

	////rttTex[buttonIndex]->addListener(this);

	//Viewport *v = rtt->addViewport(compassCam);
	//v->setClearEveryFrame(true);
	//v->setBackgroundColour(ColourValue(0,0,0,0));
	//v->setOverlaysEnabled(false);

	//

	//// Set the CEGUI Button's image to our new RTT
	//
	//Ogre::TexturePtr ogreTex = Ogre::TextureManager::getSingletonPtr()->getByName(texName);
	//CEGUI::Texture* ceguiTex = mGUIRenderer->createTexture(ogreTex);

	//CEGUI::String imageSetName = "compassimgset";
	//Imageset* textureImageSet = ImagesetManager::getSingleton().createImageset(imageSetName, ceguiTex);

	//textureImageSet->defineImage("compassneedle", Point(0.0f, 0.0f), Size(ceguiTex->getWidth(), ceguiTex->getHeight()), Point(0.0f,0.0f));
	////imageSetName = "set:compassimgset image:compassneedle";

	//compassWnd->setProperty("Image", "set:"+imageSetName+" image:compassneedle");
	//

}

void GameApplication::createGUI()
{
	// Set up GUI system
	SceneManager *smgr = NULL;
	if(currentLevel)
	{
		smgr = currentLevel->getSceneManager();
	}
	else
	{
		smgr = noLevelMgr;
	}
	mGUIRenderer = new CEGUI::OgreCEGUIRenderer(mWindow, Ogre::RENDER_QUEUE_OVERLAY, false, 3000, smgr);

	mGUISystem = new CEGUI::System(mGUIRenderer);

		/*CEGUI::Font* f;
f = FontManager::getSingleton().createFont("Tahoma-12", "Tahoma.ttf", 12, 0);
f->defineFontGlyphs(f->getAvailableGlyphs() + (utf32)223 + (utf32)228 + (utf32)246 + (utf32)252);*/

	CEGUI::Logger::getSingleton().setLoggingLevel(CEGUI::Informative);

	//taharezlook:
	CEGUI::SchemeManager::getSingleton().loadScheme((CEGUI::utf8*)"TaharezLookSkin.scheme");
	//LandsLook:
	CEGUI::SchemeManager::getSingleton().loadScheme((CEGUI::utf8*)"LandsLook.scheme");
	//das ist temporär und kommt später weg:
	CEGUI::SchemeManager::getSingleton().loadScheme((CEGUI::utf8*)"lands.scheme");
	mGUISystem->setDefaultMouseCursor((CEGUI::utf8*)"LandsLook", (CEGUI::utf8*)"MouseArrow");
	CEGUI::MouseCursor::getSingleton().setImage("LandsLook", "MouseArrow");
	mGUISystem->setDefaultFont((CEGUI::utf8*)"DejaVuSans-10");

	CEGUI::System::getSingletonPtr()->setDefaultTooltip( "LandsLook/Tooltip" );

		//german umlaut stuff

	//CEGUI::Font *f = mGUISystem->getDefaultFont();
	//
	//f->defineFontGlyphs(f->getAvailableGlyphs() + (CEGUI::utf32)223 + (CEGUI::utf32)228 + (CEGUI::utf32)246 + (CEGUI::utf32)252);

	//mEditorGuiSheet= CEGUI::WindowManager::getSingleton().createWindow((CEGUI::utf8*)"DefaultWindow", (CEGUI::utf8*)"Sheet");
	//mGUISystem->setGUISheet(mEditorGuiSheet);


	//mEditorGuiSheet->addChildWindow(quitButton);
	//quitButton->setPosition(CEGUI::Point(0.35f, 0.45f));
	//quitButton->setSize(CEGUI::Size(0.3f, 0.1f));
	//quitButton->setText("Quit");


	//standard-zeug. die pointer brauche ich dauernd
	imgr = CEGUI::ImagesetManager::getSingletonPtr();
	wmgr = CEGUI::WindowManager::getSingletonPtr();

	mIngameGui = wmgr->loadWindowLayout((CEGUI::utf8*)"game-ingame.layout");
//	log("vor 2. gui load");
	mMainMenuGui = wmgr->loadWindowLayout((CEGUI::utf8*)"game-mainmenu.layout");
//	log("nach 2. gui load");
	mOptionsGui = wmgr->loadWindowLayout((CEGUI::utf8*)"game-options.layout");
	mSavegameGui = wmgr->loadWindowLayout((CEGUI::utf8*)"game-saveload.layout");
//	log("nach optionen gui load");
	playerInventoryWnd = wmgr->getWindow((CEGUI::utf8*)"PlayerInventory");
	playerInventoryWnd->setVisible(false);
	playerInventoryWnd->disable();

	playerEquipWnd = wmgr->getWindow((CEGUI::utf8*)"PlayerEquipment");
	playerEquipWnd->setVisible(false);
	playerEquipWnd->disable();
	
	playerSpellbookWnd = wmgr->getWindow((CEGUI::utf8*)"PlayerSpellbook");
	playerSpellbookWnd->setVisible(false);
	playerSpellbookWnd->disable();

	otherInventoryWnd = wmgr->getWindow((CEGUI::utf8*)"OtherInventory");
	otherInventoryWnd->setVisible(false);
	otherInventoryWnd->disable();
	
	manaBar = wmgr->getWindow((CEGUI::utf8*)"manabar");
	hpBar = wmgr->getWindow((CEGUI::utf8*)"hpbar");
	manaBar->setInheritsTooltipText(true);
	hpBar->setInheritsTooltipText(true);

	hpBar_container = wmgr->getWindow((CEGUI::utf8*)"hpbar_container");
	manaBar_container = wmgr->getWindow((CEGUI::utf8*)"manabar_container");

	hpBarWidth = hpBar->getWidth();
	manaBarWidth = manaBar->getWidth();

	mGUISystem->setGUISheet(mMainMenuGui);

	infoText = wmgr->createWindow("LandsLook/StaticText", (CEGUI::utf8*)"ViewInfoText");


	mIngameGui->addChildWindow(infoText);
	
	infoText->disable();
	infoText->setVisible(false);


	holdingItemWnd = wmgr->createWindow("LandsLook/StaticImage",(CEGUI::utf8*)"CurrentHoldingItem");
	holdingItemWnd->setSize(CEGUI::UVector2( 
		CEGUI::UDim(0,INV_IMAGESIZE-8),
		CEGUI::UDim(0,INV_IMAGESIZE-8) 
	));
	mIngameGui->addChildWindow(holdingItemWnd);
	CEGUI::Window *cntfield = wmgr->createWindow("LandsLook/StaticText",(CEGUI::utf8*)"CurrentHoldingCount");
	holdingItemWnd->addChildWindow(cntfield);

	cntfield->setSize(CEGUI::UVector2(CEGUI::UDim(0,32),CEGUI::UDim(0,16)));
	cntfield->setText("");

	cntfield->setPosition(CEGUI::UVector2(CEGUI::UDim(1,-34),CEGUI::UDim(1,-16)));
	cntfield->setProperty("FrameEnabled","False");
	cntfield->setProperty("BackgroundEnabled","False");
	cntfield->setProperty("VertFormatting","TopAligned");
	cntfield->setProperty("HorzFormatting","RightAligned");	
	holdingItemWnd->setAlwaysOnTop(true);
	holdingItemWnd->disable();
	holdingItemWnd->setVisible(false);

	holdingItemWnd->setProperty("FrameEnabled","False");
	holdingItemWnd->setProperty("BackgroundEnabled","False");
	


	CEGUI::PushButton* quitButton = (CEGUI::PushButton*)wmgr->getWindow((CEGUI::utf8*)"quit");
	CEGUI::PushButton* startButton = (CEGUI::PushButton*)wmgr->getWindow((CEGUI::utf8*)"startgame");
	CEGUI::PushButton* resumeButton = (CEGUI::PushButton*)wmgr->getWindow((CEGUI::utf8*)"resume");
	CEGUI::PushButton* optButton = (CEGUI::PushButton*)wmgr->getWindow((CEGUI::utf8*)"options");
	CEGUI::PushButton* saveButton = (CEGUI::PushButton*)wmgr->getWindow((CEGUI::utf8*)"saveload");




	saveButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameApplication::saveLoadClick, this));
	quitButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameApplication::handleQuit, this));
	startButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameApplication::startGameClick, this));
	optButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameApplication::optionsClick, this));
	resumeButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameApplication::resumeClick, this));
	
	resumeButton->disable();

	/*hier das savegame-fenster*/
	((CEGUI::PushButton*)wmgr->getWindow((CEGUI::utf8*)"saveloadwnd/save"))->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameApplication::savegameSaveClick, this));
	((CEGUI::PushButton*)wmgr->getWindow((CEGUI::utf8*)"saveloadwnd/load"))->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameApplication::savegameLoadClick, this));
	((CEGUI::PushButton*)wmgr->getWindow((CEGUI::utf8*)"saveloadwnd/delete"))->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameApplication::savegameDeleteClick, this));
	((CEGUI::PushButton*)wmgr->getWindow((CEGUI::utf8*)"saveloadwnd/cancel"))->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameApplication::hideSaveLoadWnd, this));
	((CEGUI::Listbox*)wmgr->getWindow((CEGUI::utf8*)"saveloadwnd/savegamelist"))->subscribeEvent(CEGUI::Window::EventMouseClick, CEGUI::Event::Subscriber(&GameApplication::savegameListClick, this));
	((CEGUI::Listbox*)wmgr->getWindow((CEGUI::utf8*)"saveloadwnd/savegamelist"))->subscribeEvent(CEGUI::Window::EventMouseDoubleClick, CEGUI::Event::Subscriber(&GameApplication::savegameListDblClick, this));
	/**/

	CEGUI::PushButton* optionsCancel = (CEGUI::PushButton*)wmgr->getWindow((CEGUI::utf8*)"OptionsCancel");
	CEGUI::PushButton* optionsOK = (CEGUI::PushButton*)wmgr->getWindow((CEGUI::utf8*)"OptionsOK");
	CEGUI::PushButton* optionsAccept = (CEGUI::PushButton*)wmgr->getWindow((CEGUI::utf8*)"OptionsAccept");

	optionsCancel->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameApplication::optionsCancelClick, this));
	optionsOK->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameApplication::optionsOKClick, this));
	optionsAccept->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameApplication::optionsAcceptClick, this));

	wmgr->getWindow((CEGUI::utf8*)"PlayerInventory/melee")->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&GameApplication::equipmentClick, this));
	wmgr->getWindow((CEGUI::utf8*)"PlayerInventory/ranged")->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&GameApplication::equipmentClick, this));
	wmgr->getWindow((CEGUI::utf8*)"PlayerInventory/armor")->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&GameApplication::equipmentClick, this));
	wmgr->getWindow((CEGUI::utf8*)"PlayerInventory/shield")->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&GameApplication::equipmentClick, this));
	wmgr->getWindow((CEGUI::utf8*)"PlayerInventory/else1")->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&GameApplication::equipmentClick, this));
	wmgr->getWindow((CEGUI::utf8*)"PlayerInventory/else2")->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&GameApplication::equipmentClick, this));
	wmgr->getWindow((CEGUI::utf8*)"PlayerInventory/else3")->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&GameApplication::equipmentClick, this));
	wmgr->getWindow((CEGUI::utf8*)"PlayerInventory/else4")->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&GameApplication::equipmentClick, this));

	wmgr->getWindow((CEGUI::utf8*)"PlayerInventory/bag")->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&GameApplication::bagEquipClick, this));

	//wmgr->getWindow("PlayerSpellbook/activespell")->subscribeEvent(CEGUI::Window::EventMouseButtonDown, Event::Subscriber(&GameApplication::spellEquipClick, this));
//	log("nach events subscribed");

	//TabControl* optionTabs = static_cast<TabControl*>(wmgr.getWindow("OptionTabs"));
	//CEGUI::ScrollablePane *videoOptionTab = static_cast<ScrollablePane*>(wmgr.getWindow("VideoOptionsScroll"));
	//videoOptionTab->setContentPaneAutoSized(false);

	//add the renderer dropdown menu

	updateOptionsWindow();

	//audio-dropdown-liste
	CEGUI::Combobox *distMod = (CEGUI::Combobox*)wmgr->getWindow("options/distance_model");

	

	CEGUI::ListboxTextItem *itemINVERSE_DISTANCE = new CEGUI::ListboxTextItem("INVERSE_DISTANCE");
	itemINVERSE_DISTANCE->setSelectionBrushImage("LandsLook","TextSelectionBrush");
	distMod->addItem(itemINVERSE_DISTANCE);

	CEGUI::ListboxTextItem *itemINVERSE_DISTANCE_CLAMPED = new CEGUI::ListboxTextItem("INVERSE_DISTANCE_CLAMPED");
	itemINVERSE_DISTANCE_CLAMPED->setSelectionBrushImage("LandsLook","TextSelectionBrush");
	distMod->addItem(itemINVERSE_DISTANCE_CLAMPED);

	CEGUI::ListboxTextItem *itemLINEAR_DISTANCE = new CEGUI::ListboxTextItem("LINEAR_DISTANCE");
	itemLINEAR_DISTANCE->setSelectionBrushImage("LandsLook","TextSelectionBrush");
	distMod->addItem(itemLINEAR_DISTANCE);

	CEGUI::ListboxTextItem *itemLINEAR_DISTANCE_CLAMPED = new CEGUI::ListboxTextItem("LINEAR_DISTANCE_CLAMPED");
	itemLINEAR_DISTANCE_CLAMPED->setSelectionBrushImage("LandsLook","TextSelectionBrush");
	distMod->addItem(itemLINEAR_DISTANCE_CLAMPED);

	CEGUI::ListboxTextItem *itemEXPONENT_DISTANCE = new CEGUI::ListboxTextItem("EXPONENT_DISTANCE");
	itemEXPONENT_DISTANCE->setSelectionBrushImage("LandsLook","TextSelectionBrush");
	distMod->addItem(itemEXPONENT_DISTANCE);

	CEGUI::ListboxTextItem *itemEXPONENT_DISTANCE_CLAMPED = new CEGUI::ListboxTextItem("EXPONENT_DISTANCE_CLAMPED");
	itemEXPONENT_DISTANCE_CLAMPED->setSelectionBrushImage("LandsLook","TextSelectionBrush");
	distMod->addItem(itemEXPONENT_DISTANCE_CLAMPED);

	createCompass();

	//infofeld
	hideScreenText();


}



bool GameApplication::handleQuit(const CEGUI::EventArgs& e)
{
	mFrameListener->requestShutdown();
	return true;
}

void GameApplication::resetApp()
{
	if(inventoryOpen)
		toggleInventory();
	if(spellbookOpen)
		toggleSpellbook();
	if(otherInvOpen)
		hideOtherInventory();

	enemyList.clear();

	curHolding.item = NULL;
	curHolding.count = 0;
	curHoldingSpell = NULL;
	curHoldingContainer = NULL;
	curHoldingInventory = NULL;
	battleMusic = false;
	

	curOpenInventory = NULL;


	playerInventorySize = 0;
	setDisplayedSpell(NULL);
	setDisplayedWeapon(NULL);
	setSpellBorder(NULL);

	//SoundManager::getSingletonPtr()->destroyBuffersByType(btLevel);
}

bool GameApplication::startGameClick(const CEGUI::EventArgs& e)
{
	
	tempSaveGame = new ZipSaveFile("tempsave",tempPath,SAVEGAME_EXTENSION);
	tempSaveGame->removeFile();
	CEGUI::PushButton* resumeButton = (CEGUI::PushButton*)wmgr->getWindow((CEGUI::utf8*)"resume");
	resumeButton->enable();
	resetApp();
	loadLevel(startingLevel,startingEntrance,true);
	setMenu(GUI_INGAME,false);
	/*if(soundMgr)
		soundMgr->destroyAllSounds();*/
	return true;
}
bool GameApplication::resumeClick(const CEGUI::EventArgs& e)
{
//	log("resumeClick");
	if(currentLevel)
	{
		setMenu(GUI_INGAME);
	}
	return true;
}

Ogre::String GameApplication::getSavegameSettings()
{
	TiXmlDocument doc;
	//now try to write all the stuff to it...
	
	TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );


	TiXmlElement * mainElem = new TiXmlElement( "savegame" );
	mainElem->SetAttribute("name","erstmal test");
	mainElem->SetAttribute("playerlevel",currentLevel->getFileName().c_str());
	//erstmal spielersettings
	TiXmlElement * playerElem = currentLevel->getPlayer()->getXmlElem();
	playerElem->SetValue("player");
	mainElem->LinkEndChild(playerElem);
	//jetzt theoretisch noch so zeug wie globale vars...
	doc.LinkEndChild( decl );
	doc.LinkEndChild( mainElem );
	
	std::stringstream st;
	st << doc;
	return st.str();
	
	
}


bool GameApplication::saveLoadClick(const CEGUI::EventArgs& e)
{
	setMenu(GUI_SAVEGAME);
	//updateSaveLoadWnd();
	//if(!currentLevel)
	//	return true;
	//ZipSaveFile sav("test",saveGamePath,"zip");
	//sav.openZipFile(true);
	//sav.addTextFile(currentLevel->getSavegameXML(),"level/"+currentLevel->getFileName(),false);
	//sav.addTextFile(getSavegameSettings(),"savegame.xml",false);

	////jetzt zeug aus dem tempsave adden
	//
	//sav.copyFilesFrom(tempSaveGame);
	//
	//
	//sav.closeZipFile();
	
	return true;
}

bool GameApplication::optionsClick(const CEGUI::EventArgs& e)
{
	setMenu(GUI_OPTIONS);
	return true;
}


void GameApplication::setMenu(MenuMode mId,bool changePausedState)
{
	//if(mEditorGuiSheet)
	//	CEGUI::WindowManager::getSingleton().destroyWindow(mEditorGuiSheet);
	menuMode = mId;
	switch(mId)
	{
	case GUI_DISABLE:
		if(useMouseLooking)
			setCursorMode(false);
		if(changePausedState)
			setPaused(false);
		
		break;
	case GUI_INGAME:

		mGUISystem->setGUISheet(mIngameGui);
		if(changePausedState)
			setPaused(false);
		if(useMouseLooking)
			setCursorMode(false);
		break;
	case GUI_MAINMENU:

		mGUISystem->setGUISheet(mMainMenuGui);
		if(changePausedState)
			setPaused(true);
		setCursorMode(true);
		break;
	case GUI_OPTIONS:
		updateOptionsWindow();
		mGUISystem->setGUISheet(mOptionsGui);
		if(changePausedState)
			setPaused(true);
		setCursorMode(true);
		break;
	case GUI_SAVEGAME:
		updateSaveLoadWnd();
		mGUISystem->setGUISheet(mSavegameGui);
		if(changePausedState)
			setPaused(true);
		setCursorMode(true);
		break;


	}
	

	//if(mId == 0)
	//{
	//	menuMode = mId;
	//	cursorMode = false;
	//	paused = false;
	//}
	//else if(mId == GUI_INGAME)
	//{
	//	////Load a XML file
	//	//mEditorGuiSheet = CEGUI::WindowManager::getSingleton().loadWindowLayout((CEGUI::utf8*)"mainmenu.xml");
	//	//mGUISystem->setGUISheet(mEditorGuiSheet);
	//	menuMode = 1;
	//	mGUISystem->setGUISheet(mIngameGui);
	//	paused = false;
	//	cursorMode = false;
	//	//	CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
	//	////wmgr.getWindow((CEGUI::utf8*)"Quit")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameApplication::handleQuit, this));
	//	//	wmgr.getWindow((CEGUI::utf8*)"Quit")->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&GameApplication::handleQuit,this));

	//}
	//else if(mId == GUI_MAINMENU)
	//{
	//	////Load a XML file
	//	//mEditorGuiSheet = CEGUI::WindowManager::getSingleton().loadWindowLayout((CEGUI::utf8*)"mainmenu.xml");
	//	//mGUISystem->setGUISheet(mEditorGuiSheet);
	//	menuMode = mId;
	//	mGUISystem->setGUISheet(mMainMenuGui);
	//	paused = true;
	//	cursorMode = true;

	//	//			CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
	//	////wmgr.getWindow((CEGUI::utf8*)"Quit")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameApplication::handleQuit, this));
	//	//	wmgr.getWindow((CEGUI::utf8*)"Quit")->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&GameApplication::handleQuit,this));

	//}
}





void GameApplication::setDebugText(Ogre::String str,bool add)
{
	CEGUI::Window *muhtest = wmgr->getWindow((CEGUI::utf8*)"TextOut");
	CEGUI::String test = "";
	if(add)
		test += muhtest->getText();
	test += str.c_str();


	muhtest->setText(test);
}

gamedata_char GameApplication::getGameCharData(Ogre::String Id)
{
//	log("--getGameCharData begin");
	if(datamap_char.find(Id) != datamap_char.end())
	{
//		log("--getGameCharData gut");
		return datamap_char[Id];

	}
	else
	{
//		log("--getGameCharData schlecht");
		gamedata_char temp;
		temp.id="";
		return temp;
	}
//	log("--getGameCharData end");
}


gamedata_char *GameApplication::getGameCharDataPtr(Ogre::String Id)
{

	if(datamap_char.find(Id) != datamap_char.end())
	{
		return &datamap_char[Id];
	}
	else
	{
		return NULL;
	}

}
void GameApplication::showInfoText(Ogre::String txt,Ogre::MovableObject *mObj)
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

void GameApplication::setLoadLevel(Ogre::String filename, Ogre::String entrance)
{
	sllLevel = filename;
	sllEntrance = entrance;
	sll = true;
}
void GameApplication::loadLevel(Ogre::String filename, Ogre::String entrance,bool reload,bool placePlayer)
{
//	unpauseWaitTime = 0;

	paused = true;
	unpauseNextFrame = true;
	waitingUntilUnpause = 0;
	if(!placePlayer)
		reload = true;

	GameChar *oldPlayer = NULL;

	//newton debuger stuff
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
 			ZipSaveFile *temp2sav = new ZipSaveFile("tempsave2",tempPath,SAVEGAME_EXTENSION);
			temp2sav->openZipFile(true);
			temp2sav->addTextFile(currentLevel->getSavegameXML(),"level/"+currentLevel->getFileName());
			temp2sav->copyFilesFrom(tempSaveGame);
			temp2sav->closeZipFile();
			tempSaveGame->removeFile();
			temp2sav->renameFile(tempSaveGame->getZipFileName());
			delete tempSaveGame;
			tempSaveGame = temp2sav;

//			tempSaveGame->addTextFile(currentLevel->saveToFile("",true),"level/"+currentLevel->getFileName());
			OgreNewt::Debugger::getSingleton().deInit();
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
	Debugger::getSingleton().init( newLevel->getSceneManager() );

	Camera *newCam = newLevel->getMainCam();
	Viewport* vp = mWindow->getViewport(0);
	vp->setCamera(newCam);

	// Alter the camera aspect ratio to match the viewport
	newCam->setAspectRatio(Real(vp->getActualWidth()) / Real(vp->getActualHeight()));

	if(placePlayer)
		newLevel->placePlayer(entrance,oldPlayer);
	//jetzt kann sich das alte Lvl auf seinen Tod vorbereiten
	/*if(currentLevel)
		currentLevel->destroyAllObjects();*/
	mGUIRenderer->setTargetSceneManager(newLevel->getSceneManager());

	oldLevel = currentLevel;




	//nicht jetzt, erst wenn die update() schleife durch ist
	//delete currentLevel;

	currentLevel = newLevel;
	//unpaused wird es im framelistener
//	log("neues level eigentlich geladen");
	//newLevel->placeObject(GameChar

	//SNDTEST
	
	
	


}

void GameApplication::showScreenText(Ogre::String str)
{
	CEGUI::Window *textOut = wmgr->getWindow((CEGUI::utf8*)"TextOut");
	CEGUI::String test = str.c_str();


	textOut->setText(test);
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


void GameApplication::frameEnded(Real time)
{
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
	if(otherInvOpen)
	{
		//schauen ob man nicht zu weit vom offenen objekt gegangen ist
		
		
		
		if(currentLevel->getPlayer()->getPosition().squaredDistance(curOpenInvOwner->getPosition()) 
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

void GameApplication::createDefaultSceneMgr()
{
	noLevelMgr = mRoot->createSceneManager(ST_GENERIC);
	noLevelCam = noLevelMgr->createCamera("main");


    Viewport* vp = mWindow->addViewport(noLevelCam);
	vp->setBackgroundColour(ColourValue(0,0,0));
	noLevelCam->setAspectRatio(Real(vp->getActualWidth()) / Real(vp->getActualHeight()));

}

CEGUI::OgreCEGUIRenderer *GameApplication::getCEGUIRenderer()
{
	return mGUIRenderer;
}

bool GameApplication::optionsCancelClick(const CEGUI::EventArgs& e)
{
	setMenu(GUI_MAINMENU);
	return true;
}
bool GameApplication::optionsOKClick(const CEGUI::EventArgs& e)
{
	acceptSettings();
	setMenu(GUI_MAINMENU);
	return true;
}
bool GameApplication::optionsAcceptClick(const CEGUI::EventArgs& e)
{
	acceptSettings();
	return true;
}

void GameApplication::acceptSettings()
{
	//CEGUI::WindowManager *wmgr = CEGUI::WindowManager::getSingletonPtr();
	//OGRE
	CEGUI::ScrollablePane *videoOptionTab = static_cast<ScrollablePane*>(wmgr->getWindow("VideoOptionsScroll"));
	CEGUI::Combobox *renderSelect = static_cast<CEGUI::Combobox*>(wmgr->getWindow("renderSelect"));
	newRenderer = renderSelect->getText().c_str();
	RenderSystem *rs = mRoot->getRenderSystemByName(newRenderer);
	for(unsigned int i=0;i<videoOptionTab->getContentPane()->getChildCount();i++)
	{
		Window *curElem = videoOptionTab->getContentPane()->getChildAtIdx(i);
		CEGUI::String cType = curElem->getType();
		if(cType == CEGUI::String("LandsLook/Combobox"))
		{
			CEGUI::String name = curElem->getName();
			CEGUI::String value = curElem->getText();

			if(name != CEGUI::String("renderSelect") && name != CEGUI::String(""))
			{
				rs->setConfigOption(name.c_str(),value.c_str());
			}



		}
	}
	saveOgreConfig();
	//audio

	gainAll = getScrollbarPos((CEGUI::Scrollbar*)(wmgr->getWindow("options/gainAll")));

	gainMusic = getScrollbarPos((CEGUI::Scrollbar*)(wmgr->getWindow("options/gainMusic")));
	gainSFX = getScrollbarPos((CEGUI::Scrollbar*)(wmgr->getWindow("options/gainSFX")));
	gainSFXPlayer = getScrollbarPos((CEGUI::Scrollbar*)(wmgr->getWindow("options/gainSFXPlayer")));
	gainSpeech = getScrollbarPos((CEGUI::Scrollbar*)(wmgr->getWindow("options/gainSpeech")));

	
	soundMgr->setTypeGain(gainMusic,stMusic);
	
	soundMgr->setTypeGain(gainSFX,stSfxLevel);
	soundMgr->setTypeGain(gainSFXPlayer,stSfxPlayer);
	soundMgr->setTypeGain(gainSpeech,stSpeech);

	soundMgr->getListener()->setGain(gainAll);

	
	distModel = parseDistanceModel( wmgr->getWindow("options/distance_model")->getText().c_str() );
	soundMgr->setDistanceModel(distModel);

	maxDistance = StringConverter::parseReal(wmgr->getWindow("options/maxdist")->getText().c_str());
	rollofFactor = StringConverter::parseReal(wmgr->getWindow("options/rolloff")->getText().c_str());
	refDistance = StringConverter::parseReal(wmgr->getWindow("options/refdist")->getText().c_str());
	soundMgr->setDefaultMaxDist(maxDistance);
	soundMgr->setDefaultRefDistance(refDistance);
	soundMgr->setDefaultRolloffFactor(rollofFactor);
	//gamesettings
	sensibility_x = getScrollbarPos((CEGUI::Scrollbar*)(wmgr->getWindow("options/sens_x")))*2;
	sensibility_y = getScrollbarPos((CEGUI::Scrollbar*)(wmgr->getWindow("options/sens_y")))*2;
	
	useMouseLooking = static_cast<CEGUI::Checkbox*>(wmgr->getWindow("options/mouselooking"))->isSelected();
	

}

void GameApplication::saveOgreConfig()
{

	std::ofstream of(mConfigFileName.c_str());
	if (!of)
		OGRE_EXCEPT(Ogre::Exception::ERR_CANNOT_WRITE_TO_FILE, "Cannot create settings file.",
            "GameApplication::saveConfigFile");
	of << "Render System=" << newRenderer << std::endl;

	for (RenderSystemList::const_iterator pRend = mRoot->getAvailableRenderers()->begin(); pRend != mRoot->getAvailableRenderers()->end(); ++pRend)
    {
        RenderSystem* rs = *pRend;
        of << std::endl;
        of << "[" << rs->getName() << "]" << std::endl;
        const ConfigOptionMap& opts = rs->getConfigOptions();
        for (ConfigOptionMap::const_iterator pOpt = opts.begin(); pOpt != opts.end(); ++pOpt)
        {
			of << pOpt->first << "=" << pOpt->second.currentValue << std::endl;
        }
    }

    of.close();

/****** stolen from OgreRoot.cpp

	void Root::saveConfig(void)
    {
		std::ofstream of(mConfigFileName.c_str());

        if (!of)
            OGRE_EXCEPT(Exception::ERR_CANNOT_WRITE_TO_FILE, "Cannot create settings file.",
            "Root::saveConfig");

        if (mActiveRenderer)
        {
            of << "Render System=" << mActiveRenderer->getName() << std::endl;
        }
        else
        {
            of << "Render System=" << std::endl;
        }

        for (RenderSystemList::const_iterator pRend = getAvailableRenderers()->begin(); pRend != getAvailableRenderers()->end(); ++pRend)
        {
            RenderSystem* rs = *pRend;
            of << std::endl;
            of << "[" << rs->getName() << "]" << std::endl;
            const ConfigOptionMap& opts = rs->getConfigOptions();
            for (ConfigOptionMap::const_iterator pOpt = opts.begin(); pOpt != opts.end(); ++pOpt)
            {
				of << pOpt->first << "=" << pOpt->second.currentValue << std::endl;
            }
        }

        of.close();

    }
*/
	////restore config:
	/*
        // Restores configuration from saved state
        // Returns true if a valid saved configuration is
        //   available, and false if no saved config is
        //   stored, or if there has been a problem
        ConfigFile cfg;
        //RenderSystemList::iterator pRend;

        try {
            // Don't trim whitespace
            cfg.load(mConfigFileName, "\t:=", false);
        }
        catch (Exception& e)
        {
            if (e.getNumber() == Exception::ERR_FILE_NOT_FOUND)
            {
                return false;
            }
            else
            {
                throw;
            }
        }

        ConfigFile::SectionIterator iSection = cfg.getSectionIterator();
        while (iSection.hasMoreElements())
        {
            const String& renderSystem = iSection.peekNextKey();
            const ConfigFile::SettingsMultiMap& settings = *iSection.getNext();

            RenderSystem* rs = getRenderSystemByName(renderSystem);
            if (!rs)
            {
                // Unrecognised render system
                continue;
            }

            ConfigFile::SettingsMultiMap::const_iterator i;
            for (i = settings.begin(); i != settings.end(); ++i)
            {
                rs->setConfigOption(i->first, i->second);
            }
        }

        RenderSystem* rs = getRenderSystemByName(cfg.getSetting("Render System"));
        if (!rs)
        {
            // Unrecognised render system
            return false;
        }

        setRenderSystem(rs);

        // Successful load
        return true;
		*/


}

void GameApplication::updateVideoWindow()
{
			//TabControl* optionTabs = static_cast<TabControl*>(wmgr.getWindow("OptionTabs"));
//	log("updateOptionsWindow begin WTF");
	//CEGUI::WindowManager *wmgr = CEGUI::WindowManager::getSingletonPtr();
//	log("nach singleton ptr");
	CEGUI::ScrollablePane *videoOptionTab = static_cast<ScrollablePane*>(wmgr->getWindow("VideoOptionsScroll"));
//	log("childcount ="+StringConverter::toString(videoOptionTab->getChildCount()));
//	log("childcount contentpane="+StringConverter::toString(videoOptionTab->getContentPane()->getChildCount()));
	size_t cCount = videoOptionTab->getContentPane()->getChildCount();
	size_t delIndex = 0;
	if(cCount > 0)
	{
		for(size_t i=0;i<cCount;i++)
		{
			CEGUI::String curName = videoOptionTab->getContentPane()->getChildAtIdx(delIndex)->getName();
			if(curName != CEGUI::String("renderSelectLabel") && curName != CEGUI::String("renderSelect"))
			{
//				log(Ogre::String("lösche ")+curName.c_str());
				/*videoOptionTab->getContentPane()->getChildAtIdx(0)->destroy();*/
				wmgr->destroyWindow(curName);
			}
			else
			{
//				log(Ogre::String("lösche NICHT ")+curName.c_str());
				delIndex++;

			}
		}
	}


	//add the renderer dropdown menu
	bool hadRenderMenu = true;

	if(!wmgr->isWindowPresent("renderSelectLabel"))
	{

		CEGUI::Window *rendererText =wmgr->createWindow("LandsLook/StaticText","renderSelectLabel");
		videoOptionTab->addChildWindow(rendererText);
		rendererText->setArea(URect(UDim(0,20),UDim(0,5),UDim(0.5,-20),UDim(0,30)));
		//curText->setArea(URect(UDim(0,20),UDim(0,70+40*hOffset),UDim(0.5,-20),UDim(0,100+40*hOffset)));
		rendererText->setText("Renderer:");
	}
	CEGUI::Combobox *rendererDropdown = NULL;
	if(!wmgr->isWindowPresent("renderSelect"))
	{
		rendererDropdown = static_cast<CEGUI::Combobox*>(wmgr->createWindow("LandsLook/Combobox","renderSelect"));
		videoOptionTab->addChildWindow(rendererDropdown);
		rendererDropdown->setReadOnly(true);
		rendererDropdown->setArea(URect(UDim(0.5,20),UDim(0,5),UDim(1,-20),UDim(0,175)));
		//quitButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameApplication::handleQuit, this));
		hadRenderMenu = false;
		rendererDropdown->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted,Event::Subscriber(&GameApplication::optionsRendererChanged, this));
	}
	else
	{
		rendererDropdown = static_cast<CEGUI::Combobox*>(wmgr->getWindow("renderSelect"));
	}
	Ogre::RenderSystem *cRS = NULL;

	if(!hadRenderMenu)
	{
		cRS = mRoot->getRenderSystem();

		rendererDropdown->setText(cRS->getName().c_str());

		RenderSystemList *rlist = mRoot->getAvailableRenderers();
//		log("vor schleife");
		for(RenderSystemList::iterator itr = rlist->begin();itr!=rlist->end();itr++)
		{
			RenderSystem *rs = *itr;
			Ogre::String name = rs->getName();
			CEGUI::ListboxTextItem *renderItem = new ListboxTextItem(name.c_str());
			renderItem->setSelectionBrushImage("LandsLook","TextSelectionBrush");
			rendererDropdown->addItem(renderItem);
		}
//		log("nach renderer schleife");
	}
	else
	{
		Ogre::String curSelName =  rendererDropdown->getText().c_str();
//		log("auswahl: "+curSelName);
		cRS = mRoot->getRenderSystemByName(curSelName);

	}
	ConfigOptionMap options = cRS->getConfigOptions();
	int hOffset = 0;
	for(ConfigOptionMap::iterator it2 = options.begin();it2!=options.end();it2++)
	{
		//create dropdown menus for all the options


		Ogre::String name = it2->first;
		ConfigOption opt = it2->second;

//		log("-option "+name);
//		log("--currentVal="+opt.currentValue);
//		log("--immutable="+StringConverter::toString(opt.immutable));
//		log("--name="+opt.name);
//		log("--possibleValues=");

		CEGUI::Window *curText =wmgr->createWindow("LandsLook/StaticText");
		videoOptionTab->addChildWindow(curText);
		curText->setArea(URect(UDim(0,20),UDim(0,70+40*hOffset),UDim(0.5,-20),UDim(0,95+40*hOffset)));
		curText->setText(name.c_str());

//		log("vor curCB");
		if(wmgr->isWindowPresent(name.c_str()))
		{
//			log("mwaha");
			wmgr->destroyWindow(name.c_str());
		}
		CEGUI::Combobox *curCB = static_cast<CEGUI::Combobox*>(wmgr->createWindow("LandsLook/Combobox",name.c_str()));
//		log("nach curCB");
		videoOptionTab->addChildWindow(curCB);
		curCB->setReadOnly(true);
		curCB->setArea(URect(UDim(0.5,20),UDim(0,70+40*hOffset),UDim(1,-20),UDim(0,200+50*hOffset)));
		curCB->setText(opt.currentValue.c_str());

		//adding values





		for(StringVector::iterator it3 = opt.possibleValues.begin();it3 != opt.possibleValues.end();it3++)
		{
			Ogre::String val = *it3;
			CEGUI::ListboxTextItem *item = new ListboxTextItem(val.c_str());
			item->setSelectionBrushImage("LandsLook","TextSelectionBrush");
			curCB->addItem(item);
//			log("---"+val);

		}
		hOffset++;
	}

}

bool GameApplication::optionsRendererChanged(const CEGUI::EventArgs& e)
{
	/*CEGUI::Combobox *ren = static_cast<CEGUI::Combobox*>(CEGUI::WindowManager::getSingletonPtr()->getWindow("renderSelect"));
	Ogre::String rs = ren->getName().c_str();
	if(rs != curSelectedRenderer)
	{
		updateOptionsWindow(mRoot->getRenderSystemByName(curSelectedRenderer.c_str()));
	}*/
	updateVideoWindow();
	return true;
}
void GameApplication::toggleSpellbook()
{
	spellbookOpen = !(this->playerSpellbookWnd->isVisible());
	if(spellbookOpen)
	{
		//geht nicht wenn kontainer offen ist
		if(otherInvOpen)
		{
			spellbookOpen = false;
			return;
		}
		hideInfoText();
		unsigned int rowLength = INV_ROW_LENGTH;
		Spellbook *sb = currentLevel->getPlayer()->getSpellbook();
		if(sb)
		{
			setCursorMode(true);

			playerSpellbookWnd->enable();
			playerSpellbookWnd->setVisible(true);
			updateSpellbook(sb);
		}//ende von if(inv)
		else
		{
			//kein spellbook!
			if(spellbookSize != 0)
				updateSpellbookWindow();

		}
	}
	else
	{
		if(useMouseLooking && !inventoryOpen)
		{
			setCursorMode(false);
			holdingItemWnd->disable();
			holdingItemWnd->setVisible(false);			
		}
		playerSpellbookWnd->disable();
		playerSpellbookWnd->setVisible(false);
	}


}

void GameApplication::updatePlayerInventory(Inventory *inv)
{
	if(inv)
	{
		unsigned int invSize = inv->getMaxSize();
		if(playerInventorySize != invSize)
			updatePlayerInventoryWindow();
		
		

		unsigned int x = 0;
		unsigned int y = 0;
		unsigned int rowLength = INV_ROW_LENGTH;
		//CEGUI::ImagesetManager *imgr = CEGUI::ImagesetManager::getSingletonPtr();
		//std::vector<Inventory::invItem>::iterator itr;
		//for(itr = inv->itemVector.begin();itr != inv->itemVector.end();itr++)
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//!!!!!!DAS GEGEN ITERATOR MIT i++ IN DER SCHLEFE ERSETZEN!!!!!!
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		for(size_t i = 0;i < invSize;i++)
		{
			
			//alle items anzeigen
			Inventory::invDataItem cur = inv->getItemAt(i);//*itr;
			
			updateInventoryField(x,y,cur);
			x++;
			if(x % rowLength == 0)
			{
				x = 0;
				y++;
			}
		}
	}
	else
	{
		if(playerInventorySize != 0)		
			updatePlayerInventoryWindow();
		
	}
}
void GameApplication::updateOtherInventory(Inventory *inv)
{
	if(inv)
	{
		unsigned int invSize = inv->getMaxSize();
		if(invSize != otherInventorySize)
			updateOtherInventoryWindow();
		
		

		

		unsigned int x = 0;
		unsigned int y = 0;

		//CEGUI::ImagesetManager *imgr = CEGUI::ImagesetManager::getSingletonPtr();
		//std::vector<Inventory::invItem>::iterator itr;
		//for(itr = inv->itemVector.begin();itr != inv->itemVector.end();itr++)
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//!!!!!!DAS GEGEN ITERATOR MIT i++ IN DER SCHLEFE ERSETZEN!!!!!!
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		unsigned int rowLength = INV_ROW_LENGTH;
		for(size_t i = 0;i < invSize;i++)
		{
			
			//alle items anzeigen
			Inventory::invDataItem cur = inv->getItemAt(i);//*itr;
			
			updateOtherInvField(x,y,cur);
			x++;
			if(x % rowLength == 0)
			{
				x = 0;
				y++;
			}
		}
		
	}
	else
	{
		if(otherInventorySize != 0)
			updateOtherInventoryWindow();

	}
}
void GameApplication::updateSpellbook(Spellbook *sb)
{
	if(sb)
	{
		unsigned int size = sb->getSpellCnt();
		if(spellbookSize != size)
			updateSpellbookWindow();
		
		

		unsigned int x = 0;
		unsigned int y = 0;
		
		//CEGUI::ImagesetManager *imgr = CEGUI::ImagesetManager::getSingletonPtr();
		//std::vector<Inventory::invItem>::iterator itr;
		//for(itr = inv->itemVector.begin();itr != inv->itemVector.end();itr++)
		unsigned int rowLength = INV_ROW_LENGTH;
		for(size_t i = 0;i < spellbookSize;i++)
		{
			
			
			gamedata_spell* cur = sb->getAt(i);
			
			updateSpellbookField(x,y,cur);
			x++;
			if(x % rowLength == 0)
			{
				x = 0;
				y++;
			}
		}
	}
	else
	{
		if(spellbookSize != 0)
			updateSpellbookWindow();
	}
}

void GameApplication::toggleInventory()
{
	inventoryOpen = !(playerInventoryWnd->isVisible());
	//toggle machen
	if(inventoryOpen)
	{
		
		
		hideInfoText();
		setCursorMode(true);

		playerEquipWnd->enable();
		playerEquipWnd->setVisible(true);

		playerInventoryWnd->enable();
		playerInventoryWnd->setVisible(true);
		//CEGUI::WindowManager *wmgr = CEGUI::WindowManager::getSingletonPtr();

		updateDisplayedStats();

		Inventory *inv = currentLevel->getPlayer()->getInventory();
		if(inv)
		{
			
			updatePlayerInventory(inv);
		}//ende von if(inv)
		else
		{

			//kein inventar!
			if(playerInventorySize != 0)
				updatePlayerInventoryWindow();

		}
		//hier ausrüstung anzeigen

		GameChar *player = currentLevel->getPlayer();
		//alle felder holen
		CEGUI::Window *melee = wmgr->getWindow("PlayerInventory/melee");
		CEGUI::Window *ranged = wmgr->getWindow("PlayerInventory/ranged");
		CEGUI::Window *armor = wmgr->getWindow("PlayerInventory/armor");
		CEGUI::Window *shield = wmgr->getWindow("PlayerInventory/shield");

		CEGUI::Window *else1 = wmgr->getWindow("PlayerInventory/else1");
		CEGUI::Window *else2 = wmgr->getWindow("PlayerInventory/else2");
		CEGUI::Window *else3 = wmgr->getWindow("PlayerInventory/else3");
		CEGUI::Window *else4 = wmgr->getWindow("PlayerInventory/else4");

		CEGUI::Window *bag   = wmgr->getWindow("PlayerInventory/bag");

		//CEGUI::ImagesetManager *imgr = CEGUI::ImagesetManager::getSingletonPtr();

		//bildnamen aller items setzen
		//melee
		CEGUI::String meleeFileName = "";
		CEGUI::String meleeToolTip = "";
		gamedata_item *item = player->getMelee();
		if(item)
		{
			meleeFileName = item->inv_image.c_str();
			meleeToolTip = item->name.c_str();
		}
		else
		{
			meleeFileName = ICON_MELEE;
		}	
		//ranged
		CEGUI::String rangedFileName = "";
		CEGUI::String rangedToolTip = "";
		item = player->getRanged();
		if(item)
		{
			rangedFileName = item->inv_image.c_str();
			rangedToolTip = item->name.c_str();
		}
		else
		{
			rangedFileName = ICON_RANGED;
		}	
		//armor
		CEGUI::String armorFileName = "";
		CEGUI::String armorToolTip = "";
		item = player->getArmor();
		if(item)
		{
			armorFileName = item->inv_image.c_str();
			armorToolTip = item->name.c_str();
		}
		else
		{
			armorFileName = ICON_ARMOR;
		}		
		//shield
		CEGUI::String shieldFileName = "";
		CEGUI::String shieldToolTip = "";
		item = player->getShield();
		if(item)
		{
			shieldFileName = item->inv_image.c_str();
			shieldToolTip = item->name.c_str();
		}
		else
		{
			shieldFileName = ICON_SHIELD;
		}		
		//else1
		CEGUI::String else1FileName = "";
		CEGUI::String else1ToolTip = "";
		item = player->getElse1();
		if(item)
		{
			else1FileName = item->inv_image.c_str();
			else1ToolTip = item->name.c_str();
		}
		else
		{
			else1FileName = ICON_ELSE;
		}		
		//else2
		CEGUI::String else2FileName = "";
		CEGUI::String else2ToolTip = "";
		item = player->getElse2();
		if(item)
		{
			else2FileName = item->inv_image.c_str();
			else2ToolTip = item->name.c_str();
		}
		else
		{
			else2FileName = ICON_ELSE;
		}		
		//else3
		CEGUI::String else3FileName = "";
		CEGUI::String else3ToolTip = "";
		item = player->getElse3();
		if(item)
		{
			else3FileName = item->inv_image.c_str();
			else3ToolTip = item->name.c_str();
		}
		else
		{
			else3FileName = ICON_ELSE;
		}		
		//else4
		CEGUI::String else4FileName = "";
		CEGUI::String else4ToolTip = "";
		item = player->getElse4();
		if(item)
		{
			else4FileName = item->inv_image.c_str();
			else4ToolTip = item->name.c_str();
		}
		else
		{
			else4FileName = ICON_ELSE;
		}		
		//tasche
		CEGUI::String bagFileName = "";
		CEGUI::String bagToolTip = "";
		gamedata_container *bag_item = player->getBag();
		if(bag_item)
		{
			bagFileName = bag_item->inv_image.c_str();
			bagToolTip = bag_item->name.c_str();
		}
		else
		{
			bagFileName = ICON_BAG;
		}		
		//bilder setzen
				ceguiSetImage(melee,meleeFileName);
		melee->setTooltipText(meleeToolTip);
		ceguiSetImage(ranged,rangedFileName);
		ranged->setTooltipText(rangedToolTip);
		ceguiSetImage(armor,armorFileName);
		armor->setTooltipText(armorToolTip);
		ceguiSetImage(shield,shieldFileName);
		shield->setTooltipText(shieldToolTip);
		ceguiSetImage(else1,else1FileName);
		else1->setTooltipText(else1ToolTip);
		ceguiSetImage(else2,else2FileName);
		else2->setTooltipText(else2ToolTip);
		ceguiSetImage(else3,else3FileName);
		else3->setTooltipText(else3ToolTip);
		ceguiSetImage(else4,else4FileName);
		else4->setTooltipText(else4ToolTip);
		ceguiSetImage(bag,bagFileName);
		bag->setTooltipText(bagToolTip);

		


		//if(!imgr->isImagesetPresent(imgFileName))
		//{
		//	imgr->createImagesetFromImageFile(imgFileName,imgFileName);
		//}		
		//melee->setProperty("Image", "set:"+imgFileName+" image:full_image");


		//itemID = player->eqArmor.c_str();
		//if(itemID != "")
		//{
		//	imgFileName = getItemDataPtr(itemID)->inv_image.c_str();
		//}
		//else
		//{
		//	imgFileName = ICON_ARMOR;
		//}	
		//if(!imgr->isImagesetPresent(imgFileName))
		//{
		//	imgr->createImagesetFromImageFile(imgFileName,imgFileName);
		//}		
		//armor->setProperty("Image", "set:"+imgFileName+" image:full_image");


		//itemID = player->eqShield.c_str();
		//if(itemID != "")
		//{
		//	imgFileName = getItemDataPtr(itemID)->inv_image.c_str();
		//}
		//else
		//{
		//	imgFileName = ICON_SHIELD;
		//}
		//if(!imgr->isImagesetPresent(imgFileName))
		//{
		//	imgr->createImagesetFromImageFile(imgFileName,imgFileName);
		//}		
		//shield->setProperty("Image", "set:"+imgFileName+" image:full_image");


		//
		//itemID = player->eqRanged.c_str();
		//if(itemID != "")
		//{
		//	imgFileName = getItemDataPtr(itemID)->inv_image.c_str();
		//}
		//else
		//{
		//	imgFileName = ICON_RANGED;
		//}
		//if(!imgr->isImagesetPresent(imgFileName))
		//{
		//	imgr->createImagesetFromImageFile(imgFileName,imgFileName);
		//}		
		//ranged->setProperty("Image", "set:"+imgFileName+" image:full_image");


		//itemID = player->eqElse1.c_str();
		//if(itemID != "")
		//{
		//	imgFileName = getItemDataPtr(itemID)->inv_image.c_str();
		//}
		//else
		//{
		//	imgFileName = ICON_RANGED;
		//}
		//imgFileName = ICON_ELSE;
		//if(!imgr->isImagesetPresent(imgFileName))
		//{
		//	imgr->createImagesetFromImageFile(imgFileName,imgFileName);
		//}		
		//else1->setProperty("Image", "set:"+imgFileName+" image:full_image");
		//else2->setProperty("Image", "set:"+imgFileName+" image:full_image");
		//else3->setProperty("Image", "set:"+imgFileName+" image:full_image");
		//else4->setProperty("Image", "set:"+imgFileName+" image:full_image");
		
		
		
		
//		
//			/* StaticImage */
//		ImagesetManager::getSingleton().createImagesetFromImageFile("testimg", "crown_armor.png");
//
//		//CEGUI::DefaultWindow* staticImage = static_cast<CEGUI::DefaultWindow*>(itest);
//		itest->setProperty("Image", "set:testimg image:full_image"); // "full_image" is a default name from CEGUIImageset::Imageset() 
		
	}
	else
	{
		//wenn das 2. inv offen ist, es mitschließen
		if(otherInvOpen)
		{
			hideOtherInventory();
		}
		playerEquipWnd->disable();
		playerEquipWnd->setVisible(false);
		if(useMouseLooking && !spellbookOpen)
		{
			setCursorMode(false);
			holdingItemWnd->disable();
			holdingItemWnd->setVisible(false);
		}
		playerInventoryWnd->disable();
		playerInventoryWnd->setVisible(false);

		

	}
}

void GameApplication::updateDisplayedStats()
{
	GameChar *player = currentLevel->getPlayer();
	Stats st = player->getStats();
	wmgr->getWindow("stat_strength")->setText(str(st.mStats[st_strength]).c_str()); 
	wmgr->getWindow("stat_agility")->setText(str(st.mStats[st_agility]).c_str());
	wmgr->getWindow("stat_magic")->setText(str(st.mStats[st_magic]).c_str());
	wmgr->getWindow("stat_maxHP")->setText(str(st.mStats[st_maxHP]).c_str());
	wmgr->getWindow("stat_maxMP")->setText(str(st.mStats[st_maxMP]).c_str());	
	wmgr->getWindow("stat_resistBlunt")->setText(str(st.mStats[st_resistBlunt]).c_str());	
	wmgr->getWindow("stat_resistCut")->setText(str(st.mStats[st_resistCut]).c_str());		
	wmgr->getWindow("stat_resistPierce")->setText(str(st.mStats[st_resistPierce]).c_str());	
	wmgr->getWindow("stat_resistFire")->setText(str(st.mStats[st_resistFire]).c_str());	
	wmgr->getWindow("stat_resistIce")->setText(str(st.mStats[st_resistIce]).c_str());		
	wmgr->getWindow("stat_resistSpark")->setText(str(st.mStats[st_resistSpark]).c_str());
	wmgr->getWindow("stat_resistPoison")->setText(str(st.mStats[st_resistPoison]).c_str());
	wmgr->getWindow("stat_resistDark")->setText(str(st.mStats[st_resistDark]).c_str());
	wmgr->getWindow("stat_resistLight")->setText(str(st.mStats[st_resistLight]).c_str());
	wmgr->getWindow("stat_hpRegenRate")->setText(str(st.mStats[st_hpRegenRate]).c_str());
	wmgr->getWindow("stat_manaRegenRate")->setText(str(st.mStats[st_manaRegenRate]).c_str());
}

void GameApplication::setDisplayedWeapon(gamedata_item* item)
{
	if(item == curDisplayedWeapon)
		return;

	curDisplayedWeapon = item;
	if(item)
		ceguiSetImage(wmgr->getWindow("currentweapon"),item->inv_image.c_str());
	else
		ceguiSetImage(wmgr->getWindow("currentweapon"),"");

}

void GameApplication::setDisplayedSpell(gamedata_spell* spell)
{
	if(spell == curDisplayedSpell)
		return;

	curDisplayedSpell = spell;
	if(spell)
		ceguiSetImage(wmgr->getWindow("currentspell"),spell->inv_image.c_str());
	else
		ceguiSetImage(wmgr->getWindow("currentspell"),"");

}

void GameApplication::gameOver()
{
	CEGUI::PushButton* resumeButton = (CEGUI::PushButton*)wmgr->getWindow((CEGUI::utf8*)"resume");
	resumeButton->disable();
	if(inventoryOpen)
		toggleInventory();
	if(spellbookOpen)
		toggleSpellbook();
	if(otherInvOpen)
		hideOtherInventory();

	curHolding.item = NULL;
	curHolding.count = 0;
	curHoldingSpell = NULL;
	curHoldingContainer = NULL;
	curHoldingInventory = NULL;

	curOpenInventory = NULL;


	playerInventorySize = 0;
	setDisplayedSpell(NULL);
	setDisplayedWeapon(NULL);
	setSpellBorder(NULL);
	
	setMenu(GUI_MAINMENU);
}

void GameApplication::setCursorMode(bool enable)
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
		
	}
	else
	{
		
		mGUISystem->getDefaultTooltip()->hide();
		mouse->setPosition(CEGUI::Point(mWindow->getWidth()/2,mWindow->getHeight()/2));
		mGUISystem->setDefaultMouseCursor((CEGUI::utf8*)"LandsLook", (CEGUI::utf8*)"MouseTarget");
		mouse->setImage("LandsLook", "MouseTarget");
	}
	//CEGUI::MouseCursor::getSingleton().setVisible(enable);
	if(cursorMode && (curHolding.count != 0 || curHoldingSpell))
	{
		holdingItemWnd->enable();
		holdingItemWnd->setVisible(true);
	}
}

void GameApplication::updateSpellbookWindow()
{
	unsigned int rowLength = INV_ROW_LENGTH;
	Spellbook *sb = currentLevel->getPlayer()->getSpellbook();
	unsigned int sbSize = 0;
	if(sb)
	{
		sbSize = sb->getSpellCnt();
	}
		
	
	unsigned int x = 0;
	unsigned int y = 0;
	//CEGUI::WindowManager *wmgr = CEGUI::WindowManager::getSingletonPtr();
	CEGUI::Window *invpane = wmgr->getWindow((CEGUI::utf8*)"PlayerSpellbook/contentPane");
	if(wmgr->isWindowPresent("playerSpell0_0"))
	{
		CEGUI::Window *badchild = wmgr->getWindow("playerSpell0_0");
		while(badchild)
		{
			
			wmgr->destroyWindow(badchild);
			x++;
			if(x % rowLength == 0)
			{
				x = 0;
				y++;
			}
			Ogre::String cname = "playerSpell"+str(x)+"_"+str(y);

			if(wmgr->isWindowPresent(cname.c_str()))
			{
				badchild = wmgr->getWindow(cname.c_str());
			}
			else
			{
				badchild = NULL;
			}


		}

	}

	if(sb)
	{
		
		
		
		//das erzeugt erstmal alle felder
		x = y = 0;
		Real imgsize = INV_IMAGESIZE;
		for(unsigned int i=0;i<sbSize;i++)
		{
			Ogre::String fieldname = "playerSpell"+str(x)+"_"+str(y);
			CEGUI::Window *itest = wmgr->createWindow("LandsLook/StaticImage",(CEGUI::utf8*)fieldname.c_str());
			itest->setSize(CEGUI::UVector2(CEGUI::UDim(0,imgsize),CEGUI::UDim(0,imgsize)));
			invpane->addChildWindow(itest);
			itest->setPosition(CEGUI::UVector2(CEGUI::UDim(0,imgsize*x),CEGUI::UDim(0,imgsize*y)));
			itest->setID(i);
			itest->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&GameApplication::playerSpellbookClick, this));
			
			x++;
			if(x % rowLength == 0)
			{
				x = 0;
				y++;
			}
		}
		spellbookSize = sbSize;
	}//von if(inv)
	else
	{
		spellbookSize = 0;
	}
	
}
//--------------------------------------------------------------------------------------------------------
void GameApplication::updatePlayerInventoryWindow()
{
	unsigned int rowLength = INV_ROW_LENGTH;
	Inventory *inv = currentLevel->getPlayer()->getInventory();
	unsigned int invSize = 0;
	if(inv)
	{
		invSize = inv->getMaxSize();
	}
	
	
	unsigned int x = 0;
	unsigned int y = 0;
	//CEGUI::WindowManager *wmgr = CEGUI::WindowManager::getSingletonPtr();
	CEGUI::Window *invpane = wmgr->getWindow((CEGUI::utf8*)"PlayerInventory/contentPane");
	if(wmgr->isWindowPresent("playerInv0_0"))
	{
		CEGUI::Window *badchild = wmgr->getWindow("playerInv0_0");
		while(badchild)
		{
			
			wmgr->destroyWindow(badchild);
			x++;
			if(x % rowLength == 0)
			{
				x = 0;
				y++;
			}
			Ogre::String cname = "playerInv"+str(x)+"_"+str(y);

			if(wmgr->isWindowPresent(cname.c_str()))
			{
				badchild = wmgr->getWindow(cname.c_str());
			}
			else
			{
				badchild = NULL;
			}


		}

	}

	if(inv)
	{
		
		
		
		//das erzeugt erstmal alle felder
		x = y = 0;
		Real imgsize = INV_IMAGESIZE;
		for(unsigned int i=0;i<invSize;i++)
		{
			Ogre::String fieldname = "playerInv"+str(x)+"_"+str(y);
			CEGUI::Window *itest = wmgr->createWindow("LandsLook/StaticImage",(CEGUI::utf8*)fieldname.c_str());
			itest->setSize(CEGUI::UVector2(CEGUI::UDim(0,imgsize),CEGUI::UDim(0,imgsize)));
			invpane->addChildWindow(itest);
			itest->setPosition(CEGUI::UVector2(CEGUI::UDim(0,imgsize*x),CEGUI::UDim(0,imgsize*y)));
			itest->setID(i);
			itest->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&GameApplication::playerInventoryClick, this));
			
			fieldname = "playerInvCnt"+str(x)+"_"+str(y);
			CEGUI::Window *cntfield = wmgr->createWindow("LandsLook/StaticText",(CEGUI::utf8*)fieldname.c_str());
			cntfield->setSize(CEGUI::UVector2(CEGUI::UDim(0,32),CEGUI::UDim(0,16)));
			cntfield->setText("");
			itest->addChildWindow(cntfield);
			cntfield->setPosition(CEGUI::UVector2(CEGUI::UDim(1,-34),CEGUI::UDim(1,-16)));
			cntfield->setProperty("FrameEnabled","False");
			cntfield->setProperty("BackgroundEnabled","False");
			cntfield->setProperty("VertFormatting","TopAligned");
			cntfield->setProperty("HorzFormatting","RightAligned");
			
			

			x++;
			if(x % rowLength == 0)
			{
				x = 0;
				y++;
			}
		}
		playerInventorySize = invSize;
	}//von if(inv)
	else
	{
		playerInventorySize = 0;
	}
	
}



void GameApplication::updateOtherInventoryWindow()
{
	unsigned int rowLength = INV_ROW_LENGTH;
	//Inventory *inv = currentLevel->getPlayer()->getInventory();
	unsigned int invSize = 0;
	if(curOpenInventory)
	{
		invSize = curOpenInventory->getMaxSize();
	}
	
	
	unsigned int x = 0;
	unsigned int y = 0;
	//CEGUI::WindowManager *wmgr = CEGUI::WindowManager::getSingletonPtr();
	CEGUI::Window *invpane = wmgr->getWindow((CEGUI::utf8*)"OtherInventory/contentPane");
	if(wmgr->isWindowPresent("otherInv0_0"))
	{
		CEGUI::Window *badchild = wmgr->getWindow("otherInv0_0");
		while(badchild)
		{
			
			wmgr->destroyWindow(badchild);
			x++;
			if(x % rowLength == 0)
			{
				x = 0;
				y++;
			}
			Ogre::String cname = "otherInv"+str(x)+"_"+str(y);

			if(wmgr->isWindowPresent(cname.c_str()))
			{
				badchild = wmgr->getWindow(cname.c_str());
			}
			else
			{
				badchild = NULL;
			}


		}

	}

	if(curOpenInventory)
	{
		
		
		
		//das erzeugt erstmal alle felder
		x = y = 0;
		Real imgsize = INV_IMAGESIZE;
		for(unsigned int i=0;i<invSize;i++)
		{
			Ogre::String fieldname = "otherInv"+str(x)+"_"+str(y);
			CEGUI::Window *itest = wmgr->createWindow("LandsLook/StaticImage",(CEGUI::utf8*)fieldname.c_str());
			itest->setSize(CEGUI::UVector2(CEGUI::UDim(0,imgsize),CEGUI::UDim(0,imgsize)));
			invpane->addChildWindow(itest);
			itest->setPosition(CEGUI::UVector2(CEGUI::UDim(0,imgsize*x),CEGUI::UDim(0,imgsize*y)));
			itest->setID(i);
			itest->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&GameApplication::playerInventoryClick, this));
			
			fieldname = "otherInvCnt"+str(x)+"_"+str(y);
			CEGUI::Window *cntfield = wmgr->createWindow("LandsLook/StaticText",(CEGUI::utf8*)fieldname.c_str());
			cntfield->setSize(CEGUI::UVector2(CEGUI::UDim(0,32),CEGUI::UDim(0,16)));
			cntfield->setText("");
			itest->addChildWindow(cntfield);
			cntfield->setPosition(CEGUI::UVector2(CEGUI::UDim(1,-34),CEGUI::UDim(1,-16)));
			cntfield->setProperty("FrameEnabled","False");
			cntfield->setProperty("BackgroundEnabled","False");
			cntfield->setProperty("VertFormatting","TopAligned");
			cntfield->setProperty("HorzFormatting","RightAligned");
			
			

			x++;
			if(x % rowLength == 0)
			{
				x = 0;
				y++;
			}
		}
		otherInventorySize = invSize;
	}//von if(inv)
	else
	{
		otherInventorySize = 0;
	}
	
}

bool GameApplication::playerSpellbookClick(const CEGUI::EventArgs &e)
{
	if(curHolding.count != 0)
		return true;

	bool shouldPlaySound = false;
	const CEGUI::MouseEventArgs& me = 
		static_cast<const CEGUI::MouseEventArgs&>(e);
	
	UniKey curKey = convertCeguiMouseButtonToOis(me.button);	
	if(me.sysKeys & CEGUI::Control)
	{
		curKey.useModifier = true;
		curKey.modifier = OIS::Keyboard::Ctrl;
	}
	else if(me.sysKeys & CEGUI::Alt)
	{
		curKey.useModifier = true;
		curKey.modifier = OIS::Keyboard::Alt;
	}
	else if(me.sysKeys & CEGUI::Shift)
	{
		curKey.useModifier = true;
		curKey.modifier = OIS::Keyboard::Shift;
	}
	
	Ogre::String name = me.window->getName().c_str();

	gamedata_spell* playerSpell = currentLevel->getPlayer()->getSpell();

	if(StringUtil::startsWith(name,"playerspell"))
	{
		//das ist die vorhin zugewiesene ID des fensters. entspricht der nr im inventar
		unsigned int wid = me.window->getID();
		int x = Math::IFloor(wid % INV_ROW_LENGTH);
		int y = Math::IFloor(wid / INV_ROW_LENGTH);
		Spellbook *sb = currentLevel->getPlayer()->getSpellbook();
		gamedata_spell* cur = sb->getAt(wid);
		if(curKey == mFrameListener->mainKeys[CFrameListener::keyInvTakeOne] || curKey == mFrameListener->altKeys[CFrameListener::keyInvTakeOne])
		{
			//nehmen-taste:
			if(cur)
			{
				//da ist was drin
				if(!curHoldingSpell)
				{				
					//ich halte nichts
					if(curKey == mFrameListener->mainKeys[CFrameListener::keyInvTakeOne] || curKey == mFrameListener->altKeys[CFrameListener::keyInvTakeOne])
					{
						//aufnehmen...
						sb->setAt(wid,NULL);
						
						setHoldingSpell(cur);
						updateSpellbookField(x,y,NULL);
						if(cur == playerSpell)
						{
							setSpellBorder(holdingItemWnd);
						}
						shouldPlaySound = true;
					}
					
				}
				else
				{
					//ich halte was. tauschen
					gamedata_spell* old = sb->getAt(wid);
					if(cur == playerSpell)
					{
						//dann wird der aktive spell grad aufgenommen
						setSpellBorder(holdingItemWnd);
					}
					shouldPlaySound = true;
					//if(curHoldingSpell == playerSpell)
					//{
					//	//dann wirds grad abgelegt. updateSpellbookField sollte sich darum kümmern


					//}
					sb->setAt(wid,curHoldingSpell);
					updateSpellbookField(x,y,curHoldingSpell);
					setHoldingSpell(cur);
					//prüfen, ob eins davon aktiv ist


				}
			}
			else
			{
				//da ist nix drin
				if(curHoldingSpell)
				{				
					//ich halte was
					if(curKey == mFrameListener->mainKeys[CFrameListener::keyInvTakeOne] || curKey == mFrameListener->altKeys[CFrameListener::keyInvTakeOne])
					{
						//ablegen...
						sb->setAt(wid,curHoldingSpell);					
						
						updateSpellbookField(x,y,curHoldingSpell);
						setHoldingSpell(NULL);
						shouldPlaySound = true;
					}
				}

			}

		}
		else if(curKey == mFrameListener->mainKeys[CFrameListener::keyInvUse] || curKey == mFrameListener->altKeys[CFrameListener::keyInvUse])
		{
			//rechtsklicken bzw aktivieren
			//nur wenn dort ein Spell ist
			if(cur)
			{
				setSpellBorder(me.window);
				currentLevel->getPlayer()->setSpell(cur);
				setDisplayedSpell(cur);

			}			

		}

		
	}

	if(shouldPlaySound)
	{
		playPlayerSFX(clickSnd);
	}
	return true;//OMFG
}

void GameApplication::setSpellBorder(Window *field)
{
	
	CEGUI::Window *border;
	//"anlegen"
	if(!wmgr->isWindowPresent(NAME_SPELLBORDER))
	{
		border = wmgr->createWindow("LandsLook/StaticImage",NAME_SPELLBORDER);
		border->setProperty("FrameEnabled","False");
		border->setProperty("BackgroundEnabled","False");
		//border->setProperty("VertFormatting","TopAligned");
		//border->setProperty("HorzFormatting","RightAligned");
		ceguiSetImage(border,ICON_SELECTBORDER);
		/*border->setSize(CEGUI::UVector2( 
			CEGUI::UDim(0,INV_IMAGESIZE-8),
			CEGUI::UDim(0,INV_IMAGESIZE-8) 
		));*/
	}
	else
	{
		border = wmgr->getWindow(NAME_SPELLBORDER);
		if(border->getParent())
			border->getParent()->removeChildWindow(border);
	}
	if(field)
	{
		border->setVisible(true);
		field->addChildWindow(border);
		border->setArea(UDim(0,0),UDim(0,0),UDim(1,0),UDim(1,0));
	}
	else
	{
		border->setVisible(false);
		if(border->getParent())
		{
			border->getParent()->removeChildWindow(border);
		}
		
	}
}

//
//bool GameApplication::otherInventoryClick(const CEGUI::EventArgs &e)
//{
//	if(curHoldingSpell != "" || curHoldingContainer != "")
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
//	if(StringUtil::startsWith(name,"playerinv"))
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
//			if(curHolding.count == 0)
//			{
//				//nehmen, wenn ich nix halte
//				//switch(me.button)
//				//{
//				if(curKey == mFrameListener->mainKeys[CFrameListener::keyInvTakeOne] || curKey == mFrameListener->altKeys[CFrameListener::keyInvTakeOne])
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
//				else if(curKey == mFrameListener->mainKeys[CFrameListener::keyInvTakeAll] || curKey == mFrameListener->altKeys[CFrameListener::keyInvTakeAll])
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
//				else if(curKey == mFrameListener->mainKeys[CFrameListener::keyInvUse] || curKey == mFrameListener->altKeys[CFrameListener::keyInvUse])
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
//					if(curKey == mFrameListener->mainKeys[CFrameListener::keyInvTakeOne]	|| curKey == mFrameListener->altKeys[CFrameListener::keyInvTakeOne])
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
//					else if(curKey == mFrameListener->mainKeys[CFrameListener::keyInvTakeAll] || curKey == mFrameListener->altKeys[CFrameListener::keyInvTakeAll])
//					{
//					//	break;
//					//case CEGUI::MiddleButton:
//						placed = inv->addItemAt(wid,curHolding.itemID,curHolding.count);
//						neu.itemID = curHolding.itemID;
//						neu.count  = intToByte(placed+cur.count);
//						updateInventoryField(x,y,neu);
//						//newHolding.itemID = neu.itemID;
//						newHolding.count  = curHolding.count - intToByte(placed);
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
//			if(curHolding.count != 0)
//			{
//				Inventory::invItem neu = cur;
//				//...und wir halten was zum ablegen
//				unsigned int placed;
//				//switch(me.button)
//				//{
//				//case CEGUI::LeftButton:
//				if(curKey == mFrameListener->mainKeys[CFrameListener::keyInvTakeOne] || curKey == mFrameListener->altKeys[CFrameListener::keyInvTakeOne])
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
//				else if(curKey == mFrameListener->mainKeys[CFrameListener::keyInvTakeAll] || curKey == mFrameListener->altKeys[CFrameListener::keyInvTakeAll])
//				{	
//					//break;
//				//case CEGUI::MiddleButton:
//					//jetzt alles ablegen. oder zumindest so viel wie geht
//					placed = inv->addItemAt(wid,curHolding.itemID,curHolding.count);
//					neu.itemID = curHolding.itemID;
//					neu.count  = intToByte(placed);
//					updateInventoryField(x,y,neu);
//					//newHolding.itemID = neu.itemID;
//					newHolding.count  = curHolding.count - intToByte(placed);
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
bool GameApplication::playerInventoryClick(const CEGUI::EventArgs &e)
{
	if(curHoldingSpell || curHoldingContainer)
		//ich halte einen spell oder tasche. somit nix inventar
		return true;

	const CEGUI::MouseEventArgs& me = 
		static_cast<const CEGUI::MouseEventArgs&>(e);
	
	UniKey curKey = convertCeguiMouseButtonToOis(me.button);	
	curKey.ignoreModifier = false;
	if(me.sysKeys & CEGUI::Control)
	{
		curKey.useModifier = true;
		curKey.modifier = OIS::Keyboard::Ctrl;
	}
	else if(me.sysKeys & CEGUI::Alt)
	{
		curKey.useModifier = true;
		curKey.modifier = OIS::Keyboard::Alt;
	}
	else if(me.sysKeys & CEGUI::Shift)
	{
		curKey.useModifier = true;
		curKey.modifier = OIS::Keyboard::Shift;
	}
	
	Ogre::String name = me.window->getName().c_str();
	//diese funktion macht sowohl inventar des players als auch kontainer, der nachfolgende bool entscheidet
	//drüber, welches
	bool playerInv;
	bool shouldPlaySound = false;
	if(StringUtil::startsWith(name,"playerinv"))
		playerInv = true;
	else if(StringUtil::startsWith(name,"otherinv"))
		playerInv = false;
	else
		return true;

	
	//das ist die vorhin zugewiesene ID des fensters. entspricht der nr im inventar
	unsigned int wid = me.window->getID();
	int x = Math::IFloor(wid % INV_ROW_LENGTH);
	int y = Math::IFloor(wid / INV_ROW_LENGTH);
	Inventory *inv;
	if(playerInv)
		inv = currentLevel->getPlayer()->getInventory();
	else
		inv = curOpenInventory;
	Inventory::invDataItem cur = inv->getItemAt(wid);

	Inventory::invDataItem neu = cur;
	Inventory::invDataItem newHolding = curHolding;

	if(cur.count != 0)
	{
		//dh da liegt was drin
		
		/*ich machs mal so: 
		linksklick: eines nehmen/eines ablegen
		mittelklick: alle nehmen/alle ablegen
		rechtsklick: benutzen		
		*/
		if(curHolding.count == 0)
		{
			//nehmen, wenn ich nix halte
			//switch(me.button)
			//{
			if(curKey == mFrameListener->mainKeys[CFrameListener::keyInvTakeOne] || curKey == mFrameListener->altKeys[CFrameListener::keyInvTakeOne])
			//case CEGUI::LeftButton:
			{
				//eines nehmen

				inv->removeItemAt(wid,1);
				neu.count -= 1;
				if(playerInv)
					updateInventoryField(x,y,neu);
				else
					updateOtherInvField(x,y,neu);
				newHolding.item = cur.item;
				newHolding.count = 1;
				setHoldingItem(newHolding);
				shouldPlaySound = true;
			}
			else if(curKey == mFrameListener->mainKeys[CFrameListener::keyInvTakeAll] || curKey == mFrameListener->altKeys[CFrameListener::keyInvTakeAll])
				//break;
			//case CEGUI::MiddleButton:
			{
				//alle nehmen
				newHolding = cur;				
				inv->removeItemAt(wid,cur.count);
				neu.count = 0;
				neu.item = NULL;
				if(playerInv)
					updateInventoryField(x,y,neu);
				else
					updateOtherInvField(x,y,neu);
				setHoldingItem(newHolding);
				shouldPlaySound = true;
				//break;
			}
			else if(curKey == mFrameListener->mainKeys[CFrameListener::keyInvUse] || curKey == mFrameListener->altKeys[CFrameListener::keyInvUse])
			{
				//benutzen, was drin liegt

			}
			//case CEGUI::RightButton:
			//	break;
			
		}
		else
		{
			//dh ich klicke auf ein feld wo was drin ist, und halte dabei schon was
			if(cur.item == curHolding.item)
			{
				//wenn ich das gleiche halte wie das was im feld liegt
				unsigned int placed;
				//switch(me.button)
				//{
				//case CEGUI::LeftButton:
				if(curKey == mFrameListener->mainKeys[CFrameListener::keyInvTakeOne]	|| curKey == mFrameListener->altKeys[CFrameListener::keyInvTakeOne])
				{
					//eines dazutun
					placed = inv->addItemAt(wid,curHolding.item,1);
					if(placed == 1)
					{
						shouldPlaySound = true;
						neu.count = cur.count+1;
						neu.item = curHolding.item;
						if(playerInv)
							updateInventoryField(x,y,neu);
						else
							updateOtherInvField(x,y,neu);

						newHolding.count -= 1;						
						setHoldingItem(newHolding);
					}
				}
				else if(curKey == mFrameListener->mainKeys[CFrameListener::keyInvTakeAll] || curKey == mFrameListener->altKeys[CFrameListener::keyInvTakeAll])
				{
				//	break;
				//case CEGUI::MiddleButton:
					//soviel dazutun wie geht
					placed = inv->addItemAt(wid,curHolding.item,curHolding.count);
					neu.item = curHolding.item;
					neu.count  = intToByte(placed+cur.count);
					if(playerInv)
						updateInventoryField(x,y,neu);
					else
						updateOtherInvField(x,y,neu);
					//newHolding.itemID = neu.itemID;
					newHolding.count  = curHolding.count - intToByte(placed);
					shouldPlaySound = true;
					setHoldingItem(newHolding);

					//break;	
				}
			}//von if(cur.item == curHolding.item
			else
			{
				//mit etwas auf etwas anderes klicken
				//zuerst misc attempt
				//wenn linksklick->jeweils eines mischen
				// wenn cur oder curHolding 1 waren, kommt es dahin
				// sonst: einfach ins inv stecken
				//wenn mittelklick:
				//versuch, alle gehaltenen mit allen im feld zu mischen
				//resultat kommt dahin, wo vorher weniger waren
				//am besten function miscItems(invDataItem &educt1, invDataItem &educt2, invDataItem &procut, bool miscAll  = false)
				//die sich gleich um alles kümmert
				//map von struct oder array mit 2 pointern, die dort nach größe sortiert vorkommen
					
				//sonst, wenn beide jeweils 1 sind -> vertauschen
				
			}
			

		}

	}
	else
	{
		//klick auf leeres feld
		if(curHolding.count != 0)
		{
			Inventory::invDataItem neu = cur;
			//...und wir halten was zum ablegen
			unsigned int placed;
			//switch(me.button)
			//{
			//case CEGUI::LeftButton:
			if(curKey == mFrameListener->mainKeys[CFrameListener::keyInvTakeOne] || curKey == mFrameListener->altKeys[CFrameListener::keyInvTakeOne])
			{
				//nur eines
				placed = inv->addItemAt(wid,curHolding.item,1);
				if(placed == 1)
				{
					neu.count = 1;
					neu.item = curHolding.item;
					if(playerInv)
						updateInventoryField(x,y,neu);
					else
						updateOtherInvField(x,y,neu);
					newHolding.count -= 1;						
					setHoldingItem(newHolding);
					shouldPlaySound = true;
				}
			}
			else if(curKey == mFrameListener->mainKeys[CFrameListener::keyInvTakeAll] || curKey == mFrameListener->altKeys[CFrameListener::keyInvTakeAll])
			{	
				//break;
			//case CEGUI::MiddleButton:
				//jetzt alles ablegen. oder zumindest so viel wie geht
				placed = inv->addItemAt(wid,curHolding.item,curHolding.count);
				neu.item = curHolding.item;
				neu.count  = intToByte(placed);
				if(playerInv)
					updateInventoryField(x,y,neu);
				else
					updateOtherInvField(x,y,neu);
				//newHolding.itemID = neu.itemID;
				newHolding.count  = curHolding.count - intToByte(placed);
				shouldPlaySound = true;

				setHoldingItem(newHolding);
				//break;
			}
		}
		
	}//ende von curCount != 0
	

	if(shouldPlaySound)
	{
		playPlayerSFX(clickSnd);
	}
	


	return true;
	//Ogre::String sX = name.substr(
}
//--------------------------------------------------------------------------------------------------------
bool GameApplication::equipmentClick(const CEGUI::EventArgs &e)
{
	if(curHoldingInventory || curHoldingSpell)
		return true;
	const CEGUI::MouseEventArgs& me = 
		static_cast<const CEGUI::MouseEventArgs&>(e);
	GameChar *player = currentLevel->getPlayer();
	CEGUI::String wname = me.window->getName();
	bool shouldPlaySound = false;

	if(wname == "PlayerInventory/melee")
	{
		gamedata_item *cur = player->getMelee();
		if(curHolding.count != 0 && cur == NULL)
		{
			//ich halte was, und der slot ist leer
			if(player->setMelee(curHolding.item))
			{			
				curHolding.count -= 1;
				setHoldingItem(curHolding);
				CEGUI::String invimage = curHolding.item->inv_image.c_str();
				me.window->setTooltipText(curHolding.item->name);
				ceguiSetImage(me.window,invimage);
				shouldPlaySound = true;
			}
		}
		else if(curHolding.count == 0 && cur)
		{
			//ich halte nichts und im slot ist was drin
			if(player->setMelee(NULL))
			{
				curHolding.count = 1;
				curHolding.item = cur;
				setHoldingItem(curHolding);
				me.window->setTooltipText("");
				ceguiSetImage(me.window,ICON_MELEE);
				shouldPlaySound = true;
			}
		}
	}
	else if(wname == "PlayerInventory/armor")
	{
		gamedata_item *cur = player->getArmor();
		if(curHolding.count != 0 && !cur)
		{
			//ich halte was, und der slot ist leer
			if(player->setArmor(curHolding.item))
			{			
				curHolding.count -= 1;
				setHoldingItem(curHolding);
				CEGUI::String invimage = curHolding.item->inv_image.c_str();
				me.window->setTooltipText(curHolding.item->name);
				ceguiSetImage(me.window,invimage);
				shouldPlaySound = true;
			}
		}
		else if(curHolding.count == 0 && cur)
		{
			//ich halte nichts und im slot ist was drin
			if(player->setArmor(NULL))
			{
				curHolding.count = 1;
				curHolding.item = cur;
				setHoldingItem(curHolding);
				me.window->setTooltipText("");
				ceguiSetImage(me.window,ICON_ARMOR);	
				shouldPlaySound = true;
			}
		}
	}
	else if(wname == "PlayerInventory/ranged")
	{
		gamedata_item *cur = player->getRanged();
		if(curHolding.count != 0 && !cur)
		{
			//ich halte was, und der slot ist leer
			if(player->setRanged(curHolding.item))
			{			
				curHolding.count -= 1;
				setHoldingItem(curHolding);
				CEGUI::String invimage = curHolding.item->inv_image.c_str();
				me.window->setTooltipText(curHolding.item->name);
				ceguiSetImage(me.window,invimage);
				shouldPlaySound = true;
			}
		}
		else if(curHolding.count == 0 && cur)
		{
			//ich halte nichts und im slot ist was drin
			if(player->setRanged(NULL))
			{
				curHolding.count = 1;
				curHolding.item = cur;
				setHoldingItem(curHolding);
				me.window->setTooltipText("");
				ceguiSetImage(me.window,ICON_RANGED);	
				shouldPlaySound = true;
			}
		}
	}
	else if(wname == "PlayerInventory/shield")
	{
		gamedata_item *cur = player->getShield();
		if(curHolding.count != 0 && !cur)
		{
			//ich halte was, und der slot ist leer
			if(player->setShield(curHolding.item))
			{			
				curHolding.count -= 1;
				setHoldingItem(curHolding);
				CEGUI::String invimage = curHolding.item->inv_image.c_str();
				me.window->setTooltipText(curHolding.item->name);
				ceguiSetImage(me.window,invimage);
				shouldPlaySound = true;
			}
		}
		else if(curHolding.count == 0 && cur)
		{
			//ich halte nichts und im slot ist was drin
			if(player->setShield(NULL))
			{
				curHolding.count = 1;
				curHolding.item = cur;
				setHoldingItem(curHolding);
				me.window->setTooltipText("");
				ceguiSetImage(me.window,ICON_SHIELD);		
				shouldPlaySound = true;
			}
		}
	}
	else if(wname == "PlayerInventory/else4")
	{
		gamedata_item *cur = player->getElse4();
		if(curHolding.count != 0 && !cur)
		{
			//ich halte was, und der slot ist leer
			if(player->setElse4(curHolding.item))
			{			
				curHolding.count -= 1;
				setHoldingItem(curHolding);
				CEGUI::String invimage = curHolding.item->inv_image.c_str();
				ceguiSetImage(me.window,invimage);
				me.window->setTooltipText(curHolding.item->name);
				shouldPlaySound = true;
			}
		}
		else if(curHolding.count == 0 && cur)
		{
			//ich halte nichts und im slot ist was drin
			if(player->setElse4(NULL))
			{
				curHolding.count = 1;
				curHolding.item = cur;
				setHoldingItem(curHolding);
				ceguiSetImage(me.window,ICON_ELSE);	
				me.window->setTooltipText("");
				shouldPlaySound = true;
			}
		}
	}
	else if(wname == "PlayerInventory/else3")
	{
		gamedata_item *cur = player->getElse3();
		if(curHolding.count != 0 && !cur)
		{
			//ich halte was, und der slot ist leer
			if(player->setElse3(curHolding.item))
			{			
				curHolding.count -= 1;
				setHoldingItem(curHolding);
				CEGUI::String invimage = curHolding.item->inv_image.c_str();
				ceguiSetImage(me.window,invimage);
				me.window->setTooltipText(curHolding.item->name);
				shouldPlaySound = true;
			}
		}
		else if(curHolding.count == 0 && cur)
		{
			//ich halte nichts und im slot ist was drin
			if(player->setElse3(NULL))
			{
				curHolding.count = 1;
				curHolding.item = cur;
				setHoldingItem(curHolding);
				ceguiSetImage(me.window,ICON_ELSE);		
				me.window->setTooltipText("");
				shouldPlaySound = true;
			}
		}
	}
	else if(wname == "PlayerInventory/else2")
	{
		gamedata_item *cur = player->getElse2();
		if(curHolding.count != 0 && !cur)
		{
			//ich halte was, und der slot ist leer
			if(player->setElse2(curHolding.item))
			{			
				curHolding.count -= 1;
				setHoldingItem(curHolding);
				CEGUI::String invimage = curHolding.item->inv_image.c_str();
				ceguiSetImage(me.window,invimage);
				me.window->setTooltipText(curHolding.item->name);
				shouldPlaySound = true;
			}
		}
		else if(curHolding.count == 0 && cur)
		{
			//ich halte nichts und im slot ist was drin
			if(player->setElse2(NULL))
			{
				curHolding.count = 1;
				curHolding.item = cur;
				setHoldingItem(curHolding);
				ceguiSetImage(me.window,ICON_ELSE);		
				me.window->setTooltipText("");
				shouldPlaySound = true;
			}
		}
	}
	else if(wname == "PlayerInventory/else1")
	{
		gamedata_item *cur = player->getElse1();
		if(curHolding.count != 0 && !cur)
		{
			//ich halte was, und der slot ist leer
			if(player->setElse1(curHolding.item))
			{			
				curHolding.count -= 1;
				setHoldingItem(curHolding);
				CEGUI::String invimage = curHolding.item->inv_image.c_str();
				ceguiSetImage(me.window,invimage);
				me.window->setTooltipText(curHolding.item->name);
				shouldPlaySound = true;
			}
		}
		else if(curHolding.count == 0 && cur)
		{
			//ich halte nichts und im slot ist was drin
			if(player->setElse1(NULL))
			{
				curHolding.count = 1;
				curHolding.item = cur;
				setHoldingItem(curHolding);
				ceguiSetImage(me.window,ICON_ELSE);		
				me.window->setTooltipText(curHolding.item->name);
				shouldPlaySound = true;
			}
		}
	}
	if(shouldPlaySound)
	{
		playPlayerSFX(clickSnd);
	}
	updateDisplayedStats();
	return true;

}


//---------------------------------------------------------------------------------------------------------
void GameApplication::updateSpellbookField(unsigned int x, unsigned int y,gamedata_spell* spell)
{
	//CEGUI::ImagesetManager *imgr = CEGUI::ImagesetManager::getSingletonPtr();
	//CEGUI::WindowManager *wmgr = CEGUI::WindowManager::getSingletonPtr();
	Ogre::String iname = "playerSpell"+str(x)+"_"+str(y);
	CEGUI::Window *imgfield = wmgr->getWindow(iname.c_str());
	if(!spell)
	{
		ceguiSetImage(imgfield,"");	
		imgfield->setTooltipText("");
	}
	else
	{
		ceguiSetImage(imgfield,spell->inv_image.c_str());
		imgfield->setTooltipText(spell->name.c_str());
		/*gamedata_spell curData = getSpellData(spell);
		CEGUI::String imgFileName = curData.inv_image.c_str();
		if(!imgr->isImagesetPresent(imgFileName))
		{
			imgr->createImagesetFromImageFile(imgFileName,imgFileName);
		}
		
		
		imgfield->setProperty("Image", "set:"+imgFileName+" image:full_image");*/

		if(spell && spell == currentLevel->getPlayer()->getSpell())
		{
			setSpellBorder(imgfield);
		}
		
		
		//cntfield->setText(str(cur.count).c_str());
	}
}
//--------------------------------------------------------------------------------------------------------
void GameApplication::updateInventoryField(unsigned int x, unsigned int y,Inventory::invDataItem cur)
{
	//CEGUI::ImagesetManager *imgr = CEGUI::ImagesetManager::getSingletonPtr();
	//CEGUI::WindowManager *wmgr = CEGUI::WindowManager::getSingletonPtr();
	Ogre::String iname = "playerInv"+str(x)+"_"+str(y);
	Ogre::String cname = "playerInvCnt"+str(x)+"_"+str(y);
	CEGUI::Window *imgfield = wmgr->getWindow(iname.c_str());
	CEGUI::Window *cntfield = wmgr->getWindow(cname.c_str());
	if(cur.count == 0)
	{
		imgfield->setProperty("Image", "False");		
		cntfield->setText("");
		imgfield->setTooltipText("");

	}
	else
	{
		
		/*CEGUI::String imgFileName = cur.item->inv_image.c_str();
		if(!imgr->isImagesetPresent(imgFileName))
		{
			imgr->createImagesetFromImageFile(imgFileName,imgFileName);
		}
		
		
		imgfield->setProperty("Image", "set:"+imgFileName+" image:full_image");*/
		ceguiSetImage(imgfield,cur.item->inv_image.c_str());
		imgfield->setTooltipText(cur.item->name.c_str());
		if(cur.count != 1)
			cntfield->setText(str(cur.count).c_str());
		else
			cntfield->setText("");
	}
}
void GameApplication::updateOtherInvField(unsigned int x, unsigned int y,Inventory::invDataItem cur)
{
	//CEGUI::ImagesetManager *imgr = CEGUI::ImagesetManager::getSingletonPtr();
	//CEGUI::WindowManager *wmgr = CEGUI::WindowManager::getSingletonPtr();
	Ogre::String iname = "otherInv"+str(x)+"_"+str(y);
	Ogre::String cname = "otherInvCnt"+str(x)+"_"+str(y);
	CEGUI::Window *imgfield = wmgr->getWindow(iname.c_str());
	CEGUI::Window *cntfield = wmgr->getWindow(cname.c_str());
	if(cur.count == 0)
	{
		imgfield->setProperty("Image", "False");		
		cntfield->setText("");
		imgfield->setTooltipText("");

	}
	else
	{
		
		/*CEGUI::String imgFileName = cur.item->inv_image.c_str();
		if(!imgr->isImagesetPresent(imgFileName))
		{
			imgr->createImagesetFromImageFile(imgFileName,imgFileName);
		}

		
		
		
		imgfield->setProperty("Image", "set:"+imgFileName+" image:full_image");*/
		ceguiSetImage(imgfield,cur.item->inv_image.c_str());
		imgfield->setTooltipText(cur.item->name.c_str());
		if(cur.count != 1)
			cntfield->setText(str(cur.count).c_str());
		else
			cntfield->setText("");
	}
}
//--------------------------------------------------------------------------------------------------------
void GameApplication::setHoldingDisplay(Ogre::String imgName, Ogre::String subtext)
{
	if(imgName != "")
	{
		holdingItemWnd->enable();
		holdingItemWnd->setVisible(true);
		CEGUI::Point mp = MouseCursor::getSingletonPtr()->getPosition();					
		holdingItemWnd->setPosition(CEGUI::UVector2(CEGUI::UDim(0,mp.d_x+8),CEGUI::UDim(0,mp.d_y+8) ));
					

		//CEGUI::ImagesetManager *imgr = CEGUI::ImagesetManager::getSingletonPtr();
		
		/*if(!imgr->isImagesetPresent(imgName))
		{
			imgr->createImagesetFromImageFile(imgName,imgName);
		}	
		holdingItemWnd->setProperty("Image", "set:"+imgName+" image:full_image");*/
		ceguiSetImage(holdingItemWnd,imgName);

		CEGUI::Window *cntfield = wmgr->getWindow("CurrentHoldingCount");
		cntfield->setText(subtext.c_str());
	}
	else
	{
		holdingItemWnd->disable();
		holdingItemWnd->setVisible(false);
	}

}
//--------------------------------------------------------------------------------------------------------
void GameApplication::setHoldingSpell(gamedata_spell* spell)
{
	curHoldingSpell = spell;
	if(curHoldingSpell)
	{
		setHoldingDisplay(spell->inv_image);
		
	}
	else
	{
		setHoldingDisplay("");
		//holdingItemWnd->disable();
		//holdingItemWnd->setVisible(false);
	}
}
//--------------------------------------------------------------------------------------------------------
void GameApplication::setHoldingItem(Inventory::invDataItem item)
{
	curHolding = item;
	if(curHolding.count != 0)
	{
		//holdingItemWnd->enable();
		//holdingItemWnd->setVisible(true);
		//CEGUI::Point mp = CEGUI::MouseCursor::getSingletonPtr()->getPosition();					
		//holdingItemWnd->setPosition(CEGUI::UVector2(CEGUI::UDim(0,mp.d_x+8),CEGUI::UDim(0,mp.d_y+8) ));
					

		//CEGUI::ImagesetManager *imgr = CEGUI::ImagesetManager::getSingletonPtr();
		Ogre::String cntstr = "";
		if(item.count != 1)
			cntstr = str(item.count);
		setHoldingDisplay(curHolding.item->inv_image,cntstr);
		/*CEGUI::String imgFileName = curData.inv_image.c_str();
		if(!imgr->isImagesetPresent(imgFileName))
		{
			imgr->createImagesetFromImageFile(imgFileName,imgFileName);
		}	
		holdingItemWnd->setProperty("Image", "set:"+imgFileName+" image:full_image");
		CEGUI::Window *cntfield = wmgr->getWindow("CurrentHoldingCount");
		cntfield->setText(str(curHolding.count).c_str());*/
	}
	else
	{
		setHoldingDisplay("");
		//holdingItemWnd->disable();
		//holdingItemWnd->setVisible(false);
	}
}
void GameApplication::setHoldingContainer(gamedata_container* cont,Inventory *inv)
{
	if(cont == NULL || inv == NULL)
	{
		curHoldingInventory = NULL;
		curHoldingContainer = NULL;		
		setHoldingDisplay("");
	}
	else
	{
		curHoldingInventory = inv;
		curHoldingContainer = cont;
		setHoldingDisplay(cont->inv_image);
	}
}
//----------------------------------------------------------------------------------
bool GameApplication::bagEquipClick(const CEGUI::EventArgs& e)
{
	const CEGUI::MouseEventArgs& me = 
		static_cast<const CEGUI::MouseEventArgs&>(e);
	GameChar *player = currentLevel->getPlayer();
	bool shouldPlaySound = false;
	gamedata_container *curBag = player->getBag();
	//Ogre::String curID = player->getBag();
	if(curHoldingInventory && !curBag)
	{
		//ich halte was, und der slot ist leer
		if(player->setBag(curHoldingContainer,curHoldingInventory))
		{			
			//setHoldingItem(curHolding);
			CEGUI::String invimage = curHoldingContainer->inv_image.c_str();
			ceguiSetImage(me.window,invimage);
			me.window->setTooltipText(curHoldingContainer->name.c_str());
			updatePlayerInventory(curHoldingInventory);
			setHoldingContainer(NULL);
			shouldPlaySound = true;
			
		}
	}
	else if(!curHoldingInventory && curBag)
	{
		//ich halte nichts und im slot ist was drin
		Inventory *pInv = player->getInventory();
		if(player->setBag(NULL))
		{
			setHoldingContainer(curBag,pInv);
			ceguiSetImage(me.window,ICON_BAG);	
			me.window->setTooltipText("");
			updatePlayerInventory();
			shouldPlaySound = true;
		}
	}
	if(shouldPlaySound)
	{
		playPlayerSFX(clickSnd);
	}
	return true;

}
//bool GameApplication::spellEquipClick(const EventArgs &e)
//{
//	/*if(curHolding.count != 0 || curHoldingInventory)
//		return true;
//
//	currentLevel->getPlayer()->setSpell(curHoldingSpell);
//	CEGUI::String fname = getSpellData(curHoldingSpell).inv_image.c_str();
//	ceguiSetImage(wmgr->getWindow("PlayerSpellbook/activespell"),fname);
//	ceguiSetImage(wmgr->getWindow("currentspell"),fname);
//*/
//	return true;
//}

bool GameApplication::savegameListClick(const CEGUI::EventArgs& e)
{
	CEGUI::Editbox *editbox = static_cast<CEGUI::Editbox*>(wmgr->getWindow("saveloadwnd/namebox"));
	CEGUI::Listbox* saveList = (CEGUI::Listbox*)wmgr->getWindow("saveloadwnd/savegamelist");
	CEGUI::ListboxItem *cur = saveList->getFirstSelectedItem();
	if(!cur)
		return true;
	editbox->setText(cur->getText());
	return true;
}
bool GameApplication::savegameListDblClick(const CEGUI::EventArgs& e)
{
	return true;
}

void GameApplication::updateSaveLoadWnd()
{
	CEGUI::Window *slWnd = wmgr->getWindow("saveloadwnd");

	//loading list of saves
	StringVectorPtr savevector = ResourceGroupManager::getSingletonPtr()->findResourceNames("Savegame",Ogre::String("*.")+SAVEGAME_EXTENSION);
	//liste sortieren
	NameTimeVector timevector;
	for (StringVector::iterator i = savevector->begin(); i != savevector->end(); ++i)
	{
		NameTime cur;
		cur.name = *i;
		cur.time = getFileDate( saveGamePath+"/"+cur.name );
		timevector.push_back(cur);
	}
	sortNameTimeVector(timevector);
	CEGUI::Listbox* saveList = (CEGUI::Listbox*)wmgr->getWindow("saveloadwnd/savegamelist");
	size_t cnt = saveList->getItemCount();		
	for(size_t i = 0;i < cnt;i++)
	{
		CEGUI::ListboxItem *listboxitem = saveList->getListboxItemFromIndex(0);
		//setDebugText(String("\nremoved:")+listboxitem->getText().c_str(),true);
		saveList->removeItem(listboxitem);
	}
	//saveList->subscribeEvent(CEGUI::Window::EventMouseButtonDown,CEGUI::Event::Subscriber(&EditorApp::meshListDown,this));
	for (NameTimeVector::iterator i = timevector.begin(); i != timevector.end(); ++i)
	{
		//Ogre::String wtf = *i;
		//getFileDate( saveGamePath+"/"+wtf );
		Ogre::String fname, fext;
		Ogre::StringUtil::splitBaseFilename((*i).name,fname,fext);
		CEGUI::ListboxTextItem *listboxitem = new CEGUI::ListboxTextItem(fname.c_str());
		listboxitem->setSelectionBrushImage(STYLE_GAME,"TextSelectionBrush");
		//listboxitem->setTextColours(clBlack);
		saveList->addItem(listboxitem);
	}
	

	//END list of saves


	
	/*slWnd->setVisible(true);
	slWnd->setEnabled(true);
	slWnd->activate();*/
	//slWnd->se
}

bool GameApplication::savegameLoadClick(const CEGUI::EventArgs& e)
{
	CEGUI::Editbox *editbox = static_cast<CEGUI::Editbox*>(wmgr->getWindow("saveloadwnd/namebox"));
	Ogre::String name = editbox->getText().c_str();
	if(name == "")
		return true;
	
	ZipSaveFile *cur = new ZipSaveFile(name,saveGamePath,SAVEGAME_EXTENSION);
	Ogre::String sgConfigFile = cur->getFileAsString("savegame.xml");
	if(sgConfigFile == "")
		return true;
	//ab diesem Punkt wird versucht, zu laden
	resetApp();
	if(tempSaveGame)
	{
		//jetzt das tempsavegame löschen und ersetzen
		tempSaveGame->removeFile();
		delete tempSaveGame;
	}
	cur->copyFile("tempsave",tempPath,SAVEGAME_EXTENSION,false);
	
	tempSaveGame = cur;

	//jetzt das xml parsen
	TiXmlDocument *doc = new TiXmlDocument();
	doc->Parse(sgConfigFile.c_str());
	/*<savegame name="erstmal test" playerlevel="newstart.lvl">*/
	TiXmlElement *savelem = doc->FirstChildElement("savegame");
	Ogre::String lvlname = savelem ->Attribute("playerlevel");
	loadLevel(lvlname,"",true,false);
	GameChar *player = new GameChar(savelem->FirstChildElement("player"),currentLevel,Ogre::Vector3::ZERO,true);
	currentLevel->setPlayerControl(player);
	setDisplayedSpell(player->getSpell());
	//...
	cur->closeZipFile();

	delete doc;
	
	
	setMenu(GUI_INGAME,false);
	CEGUI::PushButton* resumeButton = (CEGUI::PushButton*)wmgr->getWindow((CEGUI::utf8*)"resume");
	resumeButton->enable();
	return true;
}
bool GameApplication::savegameSaveClick(const CEGUI::EventArgs& e)
{
	if(!currentLevel)
		return true;
	
	CEGUI::Editbox *editbox = static_cast<CEGUI::Editbox*>(wmgr->getWindow("saveloadwnd/namebox"));
	Ogre::String name = editbox->getText().c_str();
	if(name == "")
		return true;
	ZipSaveFile sav(name,saveGamePath,SAVEGAME_EXTENSION);
	sav.openZipFile(true);
	sav.addTextFile(currentLevel->getSavegameXML(),"level/"+currentLevel->getFileName());
	sav.addTextFile(getSavegameSettings(),"savegame.xml");

	//jetzt zeug aus dem tempsave adden
	
	sav.copyFilesFrom(tempSaveGame);
	
	
	sav.closeZipFile();
	updateSaveLoadWnd();

	return true;
}
bool GameApplication::savegameDeleteClick(const CEGUI::EventArgs& e)
{
	CEGUI::ListboxItem* cur = static_cast<CEGUI::Listbox*>(wmgr->getWindow("saveloadwnd/savegamelist"))->getFirstSelectedItem();
	if(!cur)
		return true;
	ZipSaveFile toDel(cur->getText().c_str(),saveGamePath,SAVEGAME_EXTENSION);
	
	toDel.removeFile();
	updateSaveLoadWnd();
	return true;
}

bool GameApplication::hideSaveLoadWnd(const CEGUI::EventArgs& e)
{
	setMenu(GUI_MAINMENU);
	/*CEGUI::Window *slWnd = wmgr->getWindow("saveloadwnd");
	slWnd->setEnabled(false);
	slWnd->setVisible(false);*/
	return true;
}

void GameApplication::showOtherInventory(Inventory *inv,GameObject *owner)
{
	if(!inventoryOpen)
		toggleInventory();
	curOpenInvOwner = owner;
	curOpenInventory = inv;
	otherInventoryWnd->setVisible(true);
	otherInventoryWnd->enable();
	otherInvOpen = true;
	setCursorMode(true);
	updateOtherInventory(inv);
	otherInvSquaredDist = owner->getPosition().squaredDistance(currentLevel->getPlayer()->getPosition());
	
}
void GameApplication::hideOtherInventory()
{
	curOpenInvOwner = NULL;
	curOpenInventory = NULL;
	otherInventoryWnd->setVisible(false);
	otherInventoryWnd->disable();
	otherInvOpen = false;
}

void GameApplication::setBattleMusic(bool set)
{
	if(set != battleMusic)
	{
		currentLevel->setMusic(set);
		battleMusic = set;
	}
	
}