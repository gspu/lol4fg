#include "Level.h"

#include "contactCallback.h"

#include "GameObject.h"
#include "GameChar.h"
#include "DynamicObject.h"
#include "GameItem.h"
#include "DoorObject.h"
#include "LightObject.h"
#include "functions.h"
#include "ContainerObject.h"
#include "StaticObject.h"
#include "MissileObject.h"
#include "ExplosionObject.h"
#include "Source.h"
#include "SoundManager.h"
#include "Buffer.h"
#include "ZipSaveFile.h"
#include "xmlwrapper.h"
#include "waterplane.h"
#include <OgreTerrain.h>
#include <OgreTerrainPaging.h>
#include <OgreTerrainGroup.h>
#include <OgrePage.h>
#ifdef __editor
#include "EditorApp.h"
#include "QtEditorApp.h"
#endif
#include <TypeConverter.h>


#include "LevelPageManager.h"
#include "LevelTerrainGroup.h"
#include "LevelPagedWorld.h"
#include "LevelPaging.h"
#include "OgreTerrainGroup.h"
using namespace TypeConverter;



#ifdef __editor
#include "entranceObject.h"
#endif

//TEMP!!!
void getTerrainImage(bool flipX, bool flipY, Ogre::Image& img)
{
    img.load("terrain.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    if (flipX)
        img.flipAroundY();
    if (flipY)
        img.flipAroundX();
 
}

Source *Level::createSource(SimpleSound::Buffer *buffer, int type, bool loop, bool relativeToListener, bool stream)
{
	//if(!(loop & 
	Source *res = NULL;
#ifdef __editor
#else
	res = GameApp::getSingletonPtr()->createSource(type,loop,relativeToListener,stream);
#endif
	if(res)
	{
		mSources.push_back(res);
		res->setBuffer(buffer);

	}
	return res;
}

Buffer *Level::getBuffer(Ogre::String filename)
{
	//so. level hat ab jetzt immer alle seinen buffer bei sich!
	//in eigener Liste suchen
	for(SoundManager::BufferList::iterator itr = mBuffers.begin();itr!=mBuffers.end();itr++)
	{
		if((*itr)->getFileName() == filename)
			return *itr;
	}
	////jetzt in globaler 
	//Buffer *res = SoundManager::getSingletonPtr()->getBuffer(filename,false);
	//if(res)
	//	return res;
	//else
	//{
	//neuen erstellen
	Buffer *res = SoundManager::getSingletonPtr()->createBuffer(filename);
	res->setType(btLevel);
	mBuffers.push_back(res);
	return res;
	//}
}

void Level::debugShowPoint(Ogre::Vector3 pos)
{
	if(!debugNode)
	{
		Ogre::Entity *ent = mSceneMgr->createEntity("levelDebugENtity","axes.mesh");
		debugNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		debugNode->attachObject(ent);
		debugNode->setScale(Ogre::Vector3(0.01));
	}
	debugNode->setPosition(pos);
}

void Level::debugShowLine(Ogre::Vector3 p1,Ogre::Vector3 p2)
{
	Ogre::ManualObject* myManualObject;
	if(!debugNode3)
	{
		
		Ogre::MaterialPtr myManualObjectMaterial = Ogre::MaterialManager::getSingleton().create("manual1Material","debugger"); 
		myManualObjectMaterial->setReceiveShadows(false); 
		myManualObjectMaterial->getTechnique(0)->setLightingEnabled(true); 
		myManualObjectMaterial->getTechnique(0)->getPass(0)->setDiffuse(0,0,1,0); 
		myManualObjectMaterial->getTechnique(0)->getPass(0)->setAmbient(0,0,1); 
		myManualObjectMaterial->getTechnique(0)->getPass(0)->setSelfIllumination(0,0,1); 

		
		debugNode3 = mSceneMgr->getRootSceneNode()->createChildSceneNode("manual1_node"); 
		

		
	}
	else
	{
		mSceneMgr->destroyManualObject("debugLine1");
	}
	myManualObject =  mSceneMgr->createManualObject("debugLine1"); 
	myManualObject->begin("manual1Material", Ogre::RenderOperation::OT_LINE_LIST); 
	myManualObject->position(p1); 
	myManualObject->position(p2); 
	// etc 
	myManualObject->end(); 

	debugNode3->attachObject(myManualObject);	
}


void Level::debugShowPlane(Ogre::Plane pl)
{
	if(!debugNode2)
	{
        Ogre::MeshManager::getSingleton().createPlane("debugPlane1",
	    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, pl,
	    50,50,1,1,true,1,1,1,pl.normal.perpendicular());
		Ogre::Entity *ent = mSceneMgr->createEntity("debugent1","debugPlane1");
	    ent->setMaterialName("Examples/RustySteel");
		debugNode2 = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		debugNode2->attachObject(ent);
		
	}
	
}


Level::Level(Ogre::String filename,ZipSaveFile *sav)
:splatChannels(3),
mTerrainGroup(NULL),
mTerrainPaging(NULL),
mPageManager(NULL),
mPageProvider(NULL),
has_terrain(false)
{
	mTerrainData.terrainSize = 129;
	mTerrainData.worldSize = 600;
 /*   terrainCollision = NULL;
    terrainBody = NULL;
    terrainMaterial.setNull();
    terrainLightmap.setNull();
    mTerrainMgr = NULL;
    mSplatMgr = NULL;
    mTerrainInfo = NULL;
    etLightColor = Ogre::ColourValue::White;
    etAmbientColor = Ogre::ColourValue(0.3,0.3,0.3);
    etLightDirection = Ogre::Vector3(1,-1,1);
    terrainExtends = Ogre::AxisAlignedBox(-150,-10,-150,150,90,150);*/
    levelSGfile =NULL;
	splattingBasename = "";
	/*mTerrainMgr = NULL;
	mSplatMgr = NULL;*/
	lastSGID = 1;
	northOrientation = Ogre::Quaternion::IDENTITY;
	backgroundColour=Ogre::ColourValue::Black;
	lastExIndex = 0;
	lastBaIndex = 0;
	objectListenerUse = false;
	timeSinceLastAmbient = 0;
	musicPreloaded = false;

//pagingBorder = 0.3;
	debugNode = NULL;
    debugNode2 = NULL;
	debugNode3 = NULL;
	isConstructing = true;

	
	wait_min = 0;
	wait_max = AMBIENTSND_MAX_WAIT;
	hasMusicExplore = false;
	hasMusicBattle = false;
	hasAmbientLoop = false;
	hasAmbientRandom = false;

//	loadedPages.setEmptyVal(false);
	
	//DEBUG
//	pgLoaded = false;
	//ENDDEBUG
	/*mWorldUpdate = 1.0f / 60.0f;//NEWTON_UPDATE;
	mWorldElapsed = 0.0f;
	mWorldWaitTime = 0;*/
	app = StandardApplication::getSingletonPtr();
	skyPlaneDist = 3000;
	skyBoxDist   = 5000;
	skyDomeDist  = 4000;
	destructionPrepared = false;

	mLvlType = lvtGeneric;
	worldGeometryFile = "";
//	plsm2MapName = "";
	//plsm2Bodies.
//paging = false;
//curPage_x = 0; //X-wert der aktuellen page, wo der spieler drin ist
//curPage_z = 0; //Z-wert der aktuellen page, da in der XZ-ebene
//pageStartOffset = Ogre::Vector3::ZERO;
//pageSize = 100;

	//for(int i=0;i<9;i++)
	//{
	//	Page cur;
	//	
	//	cur.x = 0;
	//	cur.z = 0;
	//	cur.loaded = false;
	//	mLoadedPages[i] = cur;
	//		
	//}


	 
	mFileName = filename;
//	dotSceneFileName = filename+".scene";
//	dotScenePos = Ogre::Vector3::ZERO;
//	dotSceneScale = Ogre::Vector3::UNIT_SCALE;
//	dotSceneOrient = Ogre::Quaternion::IDENTITY;
	mName = "";
	//loadPageDelegate = NULL;
	//unLoadPageDelegate = NULL;
	
	gravity = -9.81;
	skyBoxMaterial = "";
	skyPlaneMaterial = "";
	skyDomeMaterial = "";
	//BEGIN load level

	
	

	loadFile(sav);


	//jetzt die musik setzen
	//setMusic();
#ifndef __editor
	preloadMusic();
#endif
	musicPreloaded = true;



	//  XML Stuff END
	////app->log("end loading lvl");

	//CREATE PLAYER
	//player = new GameChar(mSceneMgr,mWorld,charMaterial,this,Ogre::Vector3(0.0,15.0,0.0));
	player = NULL;
	origPlayer = NULL;
	destructorCalled = false;
	isConstructing = false;
	//destructionPrepared = true;

	//////////////////////////
	//TEST STUFF
	//mSceneMgr->setShadowTechnique(
	
	//mWorld->setMinimumFrameRate(1/120);
	
	

}

void Level::setAmbientRandom()
{
#ifndef __editor
	if(hasAmbientRandom)
	{
		GameApp *ga = static_cast<GameApp*>(StandardApplication::getSingletonPtr());
		if(ambientRandom.size() == 1)
		{
			//nur 1. das loopen.
			ga->setAmbient1(ambientRandomPreloaded[0]);
		}
		else
		{
			
			size_t cnt = ambientRandom.size();
			size_t rand = Ogre::Math::Floor(Ogre::Math::UnitRandom()*(cnt));
			
			ga->setAmbient1(ambientRandomPreloaded[rand]);

		}
	}
	timeSinceLastAmbient = 0;
	timeAmbientWait = Ogre::Math::RangeRandom(wait_min,wait_max);
#endif
}

void Level::setAmbientLoop()
{
#ifndef __editor
	if(hasAmbientLoop)
	{
		GameApp *ga = static_cast<GameApp*>(StandardApplication::getSingletonPtr());
		if(ambientLoop.size() == 1)
		{
			//nur 1. das loopen.
			ga->setAmbient2(ambientLoopPreloaded[0],true);
		}
		else
		{
			
			size_t cnt = ambientLoop.size();
			size_t rand = Ogre::Math::Floor(Ogre::Math::UnitRandom()*(cnt));
			
			ga->setAmbient2(ambientLoopPreloaded[rand]);

		}
	}
	
#endif
}
	

void Level::setMusic(bool battle)
{
	//mLog("setMusic("+ogre_str(battle)+": "+ptrToString(this));
	
#ifndef __editor
	GameApp *ga = static_cast<GameApp*>(StandardApplication::getSingletonPtr());
	if(!battle)
	{
		if(hasMusicExplore)
		{
			
			if(musicExpPreloaded.size() == 1)
			{
				//nur 1. das loopen.
				ga->setMusic(musicExpPreloaded[0],true);
			}
			else
			{
				
				size_t cnt = musicExpPreloaded.size();
				size_t rand = lastExIndex;
				while(rand == lastExIndex)
				{
					rand = Ogre::Math::Floor(Ogre::Math::UnitRandom()*(cnt));				
				}
				ga->setMusic(musicExpPreloaded[rand]);
				lastExIndex = rand;

			}
		}

	}
	else
	{
		//kampfmusik
		if(hasMusicBattle)
		{
			
			if(musicBatPreloaded.size() == 1)
			{
				//nur 1. das loopen.
				ga->setMusic(musicBatPreloaded[0],true);
			}
			else
			{
				
				size_t cnt = musicBatPreloaded.size();
				size_t rand = lastBaIndex;
				while(rand == lastBaIndex)
				{
					rand = Ogre::Math::Floor(Ogre::Math::UnitRandom()*(cnt));				
				}
				ga->setMusic(musicBatPreloaded[rand]);
				lastBaIndex = rand;

			}
		}
	}
	
#endif
}

void Level::loadFile(ZipSaveFile *sav)
{
	unsigned int levelVersion = CUR_LEVEL_VERSION;
	if(mFileName != "")
	{
        Ogre::String data = "";
        if(Ogre::StringUtil::endsWith(mFileName,ZLEVELEXT))
        {
            //es ist gezippt!
            Ogre::String path = StandardApplication::getSingletonPtr()->levelPath;
            levelSGfile = new ZipSaveFile(path+"/"+mFileName);
            data = levelSGfile->getFileAsString("level.lvl");
        }
        else
        {
		    Ogre::DataStreamPtr pStream = Ogre::ResourceGroupManager::getSingleton().openResource(mFileName,"Level");

		    data = pStream->getAsString();
            pStream->close();
		    pStream.setNull();
        }
//		////app->log("level laden: 2");
		

		TiXmlDocument *LevelDoc;
		LevelDoc = new TiXmlDocument();
		LevelDoc->Parse( data.c_str() );



		//prüfen obs passt&version holen
		TiXmlElement *curXml = LevelDoc ->FirstChildElement("level");
		if(curXml)
		{
			
			if(curXml->Attribute("version"))
			{
				levelVersion = Ogre::StringConverter::parseUnsignedInt(curXml->Attribute("version"));
			}	
			else
			{
				levelVersion = 1;
			}
		}
		else
		{
			throw Ogre::Exception(0,"'"+mFileName+"' is not a valid level file. <level> tag missing.","Level constructor");
		}
		switch(levelVersion)
		{
		case 1:
			processLevelSettings(curXml);			
			createStandardMaterials();	
			loadVersion1(LevelDoc);
			break;
		case 2:
			processLevelSettings(curXml);			
			createStandardMaterials();	
			if(curXml->Attribute("last_sgid"))
				lastSGID = Ogre::StringConverter::parseInt(curXml->Attribute("last_sgid"));
			processAudioSettings(LevelDoc->FirstChildElement("sound"));
			processTerrainSettings(LevelDoc->FirstChildElement("terrain"));
			if(!sav)
				loadVersion2(LevelDoc);
			else
			{
				TiXmlElement   *curXml;
				curXml = LevelDoc ->FirstChildElement("objects");
                //das bereitet die objekte des tatsächlichen Levels vor, nicht des levels im SG
                //und dieses XML enthält auch statics, im gegensatz zu dem aus dem SG
				loadSavegamePrepare(curXml); 
				loadFromSavegame(sav);
			}
			break;
		case 3:
			//difference is: <level> is the root node, like how it should be in valid XML
			processLevelSettings(curXml);			
			createStandardMaterials();	
			if(curXml->Attribute("last_sgid"))
				lastSGID = Ogre::StringConverter::parseInt(curXml->Attribute("last_sgid"));
			processAudioSettings(curXml->FirstChildElement("sound"));
			processTerrainSettings(curXml->FirstChildElement("terrain"));

			loadObjects(curXml->FirstChildElement("objects"),Ogre::Vector3::ZERO,true);

			/*if(!sav)
				loadVersion2(curXml);
			else
			{*/
			if(sav)
			{
                //das bereitet die objekte des tatsächlichen Levels vor, nicht des levels im SG
                //und dieses XML enthält auch statics, im gegensatz zu dem aus dem SG
				loadSavegamePrepare(curXml ->FirstChildElement("objects")); 
				loadFromSavegame(sav);
			}
			//}
			break;
		}

		
		
		loadTerrain();

		


		delete LevelDoc;
		data="";
	}//if filename != ""
	else
	{
		//init empty level
		
		//mWorld->update(mWorldUpdate);	
		mSceneMgr = app->getRoot()->createSceneManager(Ogre::ST_GENERIC);
		createNewtonWorld();
		createStandardMaterials();	
		createCamera();
        app->getRenderWindow()->getViewport(0)->setBackgroundColour(backgroundColour);
		
	}
	//mWorld->update(mWorldUpdate);
    /*if(levelSGfile)
        levelSGfile->closeZipFile();*/
	
}

void Level::loadVersion1(TiXmlDocument *doc)
{
	TiXmlElement   *curXml;//, *XMLNode;

	//level-einstellungen, wie zB ambient light
	
	//Ogre::ColourValue ambient = ;
	
	/*curXml = LevelDoc ->FirstChildElement("level");
	if(curXml)
	{		
		processLevelSettings(curXml);			
		createStandardMaterials();	
	}
	else
	{
		throw Ogre::Exception(0,"'"+mFileName+"' is not a valid level file. <level> tag missing.","Level constructor");
	}
	*/

	curXml = doc ->FirstChildElement("statics");
	if(curXml)
	{
		//datamap_dynamic are meshes or dotScenes
		processStatics(curXml);

	}
	curXml = doc ->FirstChildElement("special");
	if(curXml)
	{
		//"special" are predefined objects, should later be scriptable
		processSpecial(curXml);

	}
	////--------------------------
	curXml = doc ->FirstChildElement("doors");
	if(curXml)
	{
		//doors. will teleport the player from one level to another
		processDoors(curXml);
	}
	
	
	curXml = doc ->FirstChildElement("items");
	if(curXml)
	{
		//items. later the player will be able to pick them up
		processItems(curXml);
	}

	curXml = doc ->FirstChildElement("characters");
	if(curXml)
	{
		//NPCs, creatures, and stuff like that
		processCharacters(curXml);		
	}
	curXml = doc ->FirstChildElement("entrances");
	if(curXml)
	{
		//points there a "door" can teleport you
		processEntrances(curXml);	
	}
	//lichter
	curXml = doc ->FirstChildElement("lights");
	if(curXml)
	{
		//lights. point, directional or spotlight
		processLights(curXml);
	}
	//kontainer
	curXml = doc ->FirstChildElement("containers");
	if(curXml)
	{
		//lights. point, directional or spotlight
		processContainers(curXml);
	}
}



void Level::loadVersion2(TiXmlDocument *doc)
{
	TiXmlElement   *curXml;//, *XMLNode;

	//level-einstellungen, wie zB ambient light
	
	//Ogre::ColourValue ambient = ;
	/*
	curXml = LevelDoc ->FirstChildElement("level");
	if(curXml)
	{
		processLevelSettings(curXml);			
		createStandardMaterials();	
	}
	else
	{
		throw Ogre::Exception(0,"'"+mFileName+"' is not a valid level file. <level> tag missing.","Level constructor");
	}
	*/

	curXml = doc ->FirstChildElement("objects");
	loadObjects(curXml,Ogre::Vector3::ZERO,false);
}


void Level::loadObject(TiXmlElement *XMLNode, Ogre::Vector3 positionOffset, bool fromLevelFile)
{
	ObjType type = stringToObjectType(XMLNode->Value()); 			
	GameObject *resultObject = NULL;		
	try
	{
		switch(type)
		{
			case otStatic:
				{
					resultObject = static_cast<GameObject*>(new StaticObject(XMLNode,this,positionOffset));
				}
				break;
			case otDynamic:
				{							
					resultObject = static_cast<GameObject*>(new DynamicObject(XMLNode,this,positionOffset));	
				}
				break;
			case otItem:
				{							
					resultObject = static_cast<GameObject*>(new GameItem(XMLNode,this,positionOffset));
				}
				break;
			case otChar:
				{
					resultObject = static_cast<GameObject*>(new GameChar(XMLNode,this,positionOffset,!fromLevelFile));	
				}
				break;
			case otDoor:
				{
					resultObject = static_cast<GameObject*>(new DoorObject(XMLNode,this,positionOffset));
				}
				break;
			case otLight:
				{
					LightObject *lo = new LightObject(XMLNode,this,positionOffset);
					resultObject = lo;
#ifdef __editor
					lo->showMesh();
#endif	
				}
				break;
			case otEntrance:
				{
					Ogre::String name = XMLNode->Attribute("name");
	
#ifdef __editor
					//entranceObject erstellen
	
					if(!isEntranceNameUnique(name))
					{
						mLog("entrName "+name+" nicht einzigartig! wird übersprungen");
						//continue;
					}
					else
					{			
						resultObject = static_cast<GameObject*>(new entranceObject(XMLNode,this,positionOffset));	
					}
	
#else

					Ogre::Vector3 pos = Ogre::StringConverter::parseVector3(XMLNode->Attribute("pos"))+positionOffset;
					Ogre::Quaternion ornt = Ogre::Quaternion::IDENTITY;
					if(XMLNode->Attribute("orient"))
					{
						ornt = Ogre::StringConverter::parseQuaternion(XMLNode->Attribute("orient"));
					}	
	
					OrientedPoint entr;
					entr.orient = ornt;
					entr.pos = pos;
					entrances[name] = entr; 
#endif
				}
				break;
			case otMissile:
				{
					resultObject = static_cast<GameObject*>(new MissileObject(XMLNode,this,positionOffset));
				}
				break;
			case otExplosion:
				{							
					resultObject = static_cast<GameObject*>(new ExplosionObject(XMLNode,this,positionOffset));
				}
				break;
			case otContainer:
				{
					resultObject = static_cast<GameObject*>(new ContainerObject(XMLNode,this,positionOffset));
					
				}
				break;
            case otWater:
				{
					resultObject = static_cast<GameObject*>(new WaterPlane(XMLNode,this,positionOffset));
					
				}
				break;
			case otGroup:
				//nix
				mLog("in der Level-load-funktion kommt ein gruppenobj vor. das sollte nicht passieren...");
				break;
			case otDeleted:
				{
					//toter eintrag
					int SGID = Ogre::StringConverter::parseInt(XMLNode->Attribute("SGID"));
					saveableObjects[SGID] = NULL;
				}
				break;
		}//von switch
		if(resultObject)
		{
			if(fromLevelFile)
				resultObject->isInLevelFile = true;
			//sonst könnte es in der XML stehen, das soll nicht überschrieben werden
		}
	}
	catch(ExceptionType)
	{
		//mLog("konnte "+objectTypeToString(type)+" "+id+" nicht erstellen!");
	}
}

void Level::loadObjects(TiXmlElement *curXml, Ogre::Vector3 positionOffset, bool fromLevelFile)
{
	if(curXml)
	{
		TiXmlElement *XMLNode = curXml->FirstChildElement();
		while( XMLNode )
		{
			loadObject(	XMLNode,positionOffset,fromLevelFile);		
			XMLNode = XMLNode->NextSiblingElement();

		}//----------------ENDE von if(XMLNode)				
		

	}	
}

void Level::loadSavegamePrepare(TiXmlElement *curXml)
{
	if(curXml)
	{
		TiXmlElement *XMLNode = curXml->FirstChildElement();
		while( XMLNode )
		{
			if(objectIsStatic(XMLNode->ValueStr()))
			{
                //statics direkt sofort laden
				loadObject(	XMLNode,Ogre::Vector3::ZERO,true);		
			}
			else
			{
				//SGID holen
				if(XMLNode->Attribute("SGID"))
				{
					int sgid = Ogre::StringConverter::parseInt(XMLNode->Attribute("SGID"));
					preloadedObjects[sgid] = XMLNode;
				}
				//wenn keine da ist, pech gehabt
			}
			XMLNode = XMLNode->NextSiblingElement();

		}//----------------ENDE von if(XMLNode)				
		

	}	
}

void Level::loadFromSavegame(ZipSaveFile *sav)
{
	TiXmlDocument *doc = NULL;
	//erstmal mich selber aus dem SG holen
	Ogre::String sgLvl = sav->getFileAsString("level/"+getBaseFileName()+".lvl");
	if(sgLvl != "")
	{
		
		doc = new TiXmlDocument();
		doc->Parse( sgLvl.c_str() );
		TiXmlElement *objElem = doc->FirstChildElement("objects");
        //das ist das loadSavegamePrepare für das savegame. in diesem XML darf es an sich keine statics geben
		loadSavegamePrepare(objElem);
		
		
	}
	for(SaveableXmlList::iterator itr = preloadedObjects.begin();itr!=preloadedObjects.end();itr++)
	{
		loadObject(itr->second,Ogre::Vector3::ZERO,false);
	}
	preloadedObjects.clear();
	if(doc)
		delete doc;
}

void Level::destroySource(Source *src)
{
	for(SoundManager::SourceList::iterator itr = mSources.begin();itr!=mSources.end();itr++)
	{	
		if(src == *itr)
		{
			SoundManager::getSingletonPtr()->destroySource(src);
			mSources.erase(itr);
			return;
		}
	}
}

Level::~Level()
{
//terrainMaterial.setNull();
	/*if(mTerrainMgr)
		delete mTerrainMgr;*/
	/*if(mSplatMgr)
		delete mSplatMgr;*/
	/*destructorCalled = true;*/
	prepareForDestruction();
	destroyAllObjects();

	//mTerrainPagedWorld
	
	if(has_terrain)
	{
		if(mPageManager)
		{
			mPageManager->destroyWorld(static_cast<Ogre::PagedWorld*>(mTerrainPagedWorld));
			OGRE_DELETE mPageManager;
		}

		if(mPageProvider)
			delete mPageProvider;

		if(mPageManager)
			OGRE_DELETE mPageManager;

		if(mTerrainGroup)
			OGRE_DELETE mTerrainGroup;

		
	}
	

	

	for(SoundManager::SourceList::iterator itr = mSources.begin();itr!=mSources.end();itr++)
	{	
		SoundManager::getSingletonPtr()->destroySource(*itr);
	}

	/*std::vector<GameObject*>::iterator itr;
	for(itr = mObjects.begin();itr != mObjects.end();itr++)
	{
		GameObject *toDel = *itr;		
		delete toDel;
	}
	mObjects.clear();*/
	/*
	std::map<Ogre::SceneNode*, GameObject*>::iterator itr;
	//for(itr = ObjectMap.begin();itr != ObjectMap.end();itr++)
	while(ObjectMap.begin() != ObjectMap.end())
	{
		//Ogre::String thisid = itr->first;
		itr = ObjectMap.begin();
		GameObject *toDel = itr->second;
		//////app->log("lösche: "+thisid);
		
		delete toDel;
	}
	ObjectMap.clear();*/
	////app->log("level destructor 1");
	
	//mSceneMgr->destroyAllCameras();
	////app->log("level destructor 2");
	
	//mSceneMgr->clearScene(); ist schon
	////app->log("level destructor 3");
	//if(loadPageDelegate)
	//{
	//	mSceneMgr->setOption("removeLoadPageListener",loadPageDelegate);
	//	//loadPageDelegate->clear();
	//	delete loadPageDelegate;
	//}
	//if(unLoadPageDelegate)
	//{
	//	mSceneMgr->setOption("removeUnloadPageListener",unLoadPageDelegate);
	//	//loadPageDelegate->clear();
	//	delete unLoadPageDelegate;
	//}
	

	//PagingLandScapeSceneManager *plsmgr = static_cast<PagingLandScapeSceneManager*>(mSceneMgr);
	

	
	
	
	////app->log("level destructor 4");
	//if(mLvlType != Level::lvtPaginglandscape)	
	//{
	//	//wenn plsm, dann nicht zerstören
	//	app->getRoot()->destroySceneManager(mSceneMgr);
	//}
	////app->log("level destructor 5");

#ifdef _DEBUG
	OgreNewt::Debugger *dbg = &mWorld->getDebugger();
	dbg->deInit();	
#endif
	mWorld->destroyAllBodies();
	
	delete mWorld;	
	////app->log("level destructor 6");
}

void Level::setAutoPaging(bool set)
{
	mTerrainPagedWorld->autoLoadingEnabled = set;
}

bool Level::getAutoPaging()
{
	return mTerrainPagedWorld->autoLoadingEnabled;
}


void Level::showNewtonDebugLines(bool show)
{
#ifdef _DEBUG
	OgreNewt::Debugger *debug = &mWorld->getDebugger();

	if (show)
	{
		debug->showDebugInformation();
		/*debug.startRaycastRecording();
		debug.clearRaycastsRecorded();*/
	}
	else
	{
		debug->hideDebugInformation();
		/*debug.clearRaycastsRecorded();
		debug.stopRaycastRecording();*/
	}
#endif
}

GameObject* Level::placeObject(gamedata *data,Ogre::Vector3 pos,Ogre::Quaternion ornt,
							   Ogre::Vector3 modScale)
{
	switch(data->type)
	{
	case gtDynamic:		
		try
		{
			DynamicObject *thisObject = new DynamicObject(static_cast<gamedata_dynamic*>(data),this,pos,ornt,modScale);
			return static_cast<GameObject*>(thisObject);
		}
		catch(int errortype)
		{
			mLog("Error "+Ogre::StringConverter::toString(errortype)+": could not create dynamic \""+data->id+"\"");
			return NULL;
		}		
		break;
	case gtItem:
		try
		{
			GameItem *thisItem = new GameItem(static_cast<gamedata_item*>(data),this,pos,ornt,modScale);
			return static_cast<GameObject*>(thisItem);
		}
		catch(int errortype)
		{
			mLog("Error "+Ogre::StringConverter::toString(errortype)+": could not create item \""+data->id+"\"");
			return NULL;
		}
		
		break;
	case gtCharacter:
		try
		{			
			GameChar *thisChar = new GameChar(static_cast<gamedata_char*>(data),this,pos,ornt);
			return static_cast<GameObject*>(thisChar);
		}
		catch(int errortype)
		{
			mLog("Error: "+Ogre::StringConverter::toString(errortype)+" could not create character \""+data->id+"\"");
			return NULL;
		}
		break;
	case gtDoor:
		
		try
		{
			DoorObject *thisObject = new DoorObject(static_cast<gamedata_door*>(data),this,pos,ornt,modScale);
			return static_cast<GameObject*>(thisObject);
		}
		catch(int errortype)
		{
			mLog("Error "+Ogre::StringConverter::toString(errortype)+": could not create door \""+data->id+"\"");
			return NULL;
		}
		
		break;	
	case gtContainer:
		try
		{	
			ContainerObject *thisObj = new ContainerObject(static_cast<gamedata_container*>(data),this,pos,ornt,modScale);
			return static_cast<GameObject*>(thisObj);
		}
		catch(int errortype)
		{
			mLog("Error: "+Ogre::StringConverter::toString(errortype)+" could not create container \""+data->id+"\"");
			return NULL;
		}
		break;
	default:
		mLog("Invalid ObjectType: "+Ogre::StringConverter::toString(data->type));
		return NULL;
	}
}


GameObject* Level::placeObject(Ogre::String id,Ogre::Vector3 pos,Ogre::Quaternion ornt,
							   Ogre::Vector3 modScale)
{
	gamedata *data = StandardApplication::getSingletonPtr()->getDataPtr(id);
	if(!data)
		return NULL;
	return placeObject(data,pos,ornt,modScale);
}

GameObject* Level::placeObject(ObjType objectType,Ogre::String objectID,Ogre::Vector3 pos,Ogre::Quaternion ornt,
							   Ogre::Vector3 modScale,WorldArtType staticType)//,DoorData addData)//,Ogre::String plane_material,Ogre::Real plane_xseg, Ogre::Real plane_yseg)
{

	


	switch(objectType)
	{
	case otDynamic:		
	case otItem:		
	case otChar:	
	case otDoor:
	case otContainer:
		return placeObject(objectID,pos,ornt,modScale);
		break;
		
	
	case otLight:
		try
		{
			LightObject *thisObject = new LightObject(this,pos);
#ifdef __editor
			thisObject->showMesh();
#endif
			return static_cast<GameObject*>(thisObject);
		}
		catch(int errortype)
		{
			mLog("Error "+Ogre::StringConverter::toString(errortype)+": could not create light \""+objectID+"\"");
			return NULL;
		}
		break;
		
#ifdef __editor
	case otEntrance:
		try
		{
			Ogre::String entrname = objectID;
			mLog("erstelle entrance: "+objectID);
			if(!isEntranceNameUnique(objectID))
			{
				//mLog("entrName nicht unique!");
				entrname = getUniqueEntranceName(entrname);

			}
			entranceObject *thisObject = new entranceObject(entrname,this,pos,ornt);
			OrientedPoint me;
			me.pos = pos;
			me.orient = ornt;
			entrances[objectID] = me;
			return static_cast<GameObject*>(thisObject);
		}
		catch(int errortype)
		{
			mLog("Error "+Ogre::StringConverter::toString(errortype)+": could not create entrance \""+objectID+"\"");
			return NULL;
		}
		break;
#endif	
	case otStatic:
		try
		{	
			WorldArtData wData;
			switch(staticType)
			{
			case WT_PLANE:
				{
					wData.type = WT_PLANE;
					wData.material = objectID;//"colorWhite";
					wData.pData.constant = 0;
					wData.pData.normal = Ogre::Vector3::NEGATIVE_UNIT_Z;
					wData.pData.width = 1;
					wData.pData.height = 1;
					wData.pData.xSeg = 1;
					wData.pData.ySeg = 1;
					wData.pData.numTexCoordSets = 1;
					wData.pData.uTile = 1;
					wData.pData.vTile = 1;
					wData.pData.up = Ogre::Vector3::UNIT_Y;
				}
				break;
			case WT_DOTSCENE:
				{
					wData.type = WT_DOTSCENE;
					wData.filename = objectID;
				}
				break;
			case WT_MESH:
				{
					wData.type = WT_MESH;
					wData.filename = objectID;
				}
				break;
			case WT_BILLBOARD:
				{
					wData.type = WT_BILLBOARD;
					wData.bbColor = Ogre::ColourValue::White;
					wData.material = objectID;//MAT_PSDUMMY;
				}
				break;
			case WT_PARTICLESYSTEM:
				{
					wData.type = WT_PARTICLESYSTEM;
					wData.material = objectID;//"";
				}
				break;
			default:
				mLog("Error: could not create static object of type "+ogre_str(staticType));

			};
			
			/*if(objectID == "plane")
			{
				wData.type = WT_PLANE;
				wData.material = "colorWhite";
				wData.pData.constant = 0;
				wData.pData.normal = Ogre::Vector3::NEGATIVE_UNIT_Z;
				wData.pData.width = 1;
				wData.pData.height = 1;
				wData.pData.xSeg = 1;
				wData.pData.ySeg = 1;
				wData.pData.numTexCoordSets = 1;
				wData.pData.uTile = 1;
				wData.pData.vTile = 1;
				wData.pData.up = Ogre::Vector3::UNIT_Y;
			}
			else if(Ogre::StringUtil::endsWith(objectID,".scene"))
			{
				wData.type = WT_DOTSCENE;
				wData.filename = objectID;
			}
			else if(Ogre::StringUtil::endsWith(objectID,".mesh"))
			{
				wData.type = WT_MESH;
				wData.filename = objectID;
			}
			else if(objectID == "billboard")
			{
				wData.type = WT_BILLBOARD;
				wData.bbColor = Ogre::ColourValue::White;
				wData.material = MAT_PSDUMMY;
			}
			else if(objectID == "particle")
			{
				wData.type = WT_PARTICLESYSTEM;
				wData.material = "";
			}*/
			
			StaticObject *thisObj = new StaticObject(this,wData,pos,ornt,modScale);
			return static_cast<GameObject*>(thisObj);
		}
		catch(int errortype)
		{
			mLog("Error: "+Ogre::StringConverter::toString(errortype)+" could not create static \""+objectID+"\"");
			return NULL;
		}
		break;		
    case otWater:
        try
		{	
			WaterPlane *thisObj = new WaterPlane(this,pos,ornt,modScale);
			return static_cast<GameObject*>(thisObj);
		}
		catch(int errortype)
		{
			mLog("Error: "+Ogre::StringConverter::toString(errortype)+" could not create waterplane");
			return NULL;
		}
		break;


		

	default:
		mLog("Invalid ObjectType: "+Ogre::StringConverter::toString(objectType));
		//goOn = false;
		//nix
	}

	//if(!goOn)
	//{
	//	return;
	//}
	//Ogre::String nodeName = "node_"+objectID;
	//for(int i = 0;mSceneMgr->hasSceneNode(nodeName);++i)
	//{
	//	nodeName = "node_"+objectID+Ogre::StringConverter::toString(i);
	//}
	//Ogre::SceneNode *nod = mSceneMgr->getRootSceneNode()->createChildSceneNode(nodeName);//,pos);


	//Ogre::String entName = "ent_"+objectID;//+"_"+Ogre::StringConverter::toString(i);
	//for(int i = 0;mSceneMgr->hasEntity(entName);++i)
	//{
	//	entName = "ent_"+objectID+Ogre::StringConverter::toString(i);
	//}


	//
	//if(Ogre::StringUtil::startsWith(mesh,"!plane"))
	//{
	//	//ok, ein fall für den meshmanager
	//	if(Ogre::MeshManager::getSingleton().resourceExists(mesh))
	//	{
	//		//dann ist alles ok
	//	}
	//	else
	//	{
	//		Ogre::Plane pl(Ogre::Vector3(0,0,-1),0);
	//		Ogre::String xseg_s = "";
	//		Ogre::String yseg_s = "";
	//		Ogre::String temp_string= "";
	//		for(unsigned int i=7;i<mesh.length();i++)
	//		{
	//			if(mesh[i] != '/')
	//			{
	//				temp_string += mesh[i];
	//			}
	//			else
	//			{
	//				xseg_s = temp_string;
	//				temp_string="";
	//			}

	//		}
	//		yseg_s = temp_string;
	//		////app->log(xseg_s+" - "+yseg_s);
	//		Ogre::MeshManager::getSingleton().createPlane(mesh,
	//		   Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, pl,
	//		   1,1,Ogre::StringConverter::parseInt(xseg_s),Ogre::StringConverter::parseInt(yseg_s),
	//		   true,1,5,5,Ogre::Vector3(1,0,0));
	//	}

	//		
	//}
	//

	////if(!plane)
	////{
	//	ent = mSceneMgr->createEntity( entName, mesh.c_str() );
	//	if(material != "")
	//	{
	//		ent->setMaterialName(material);
	//		
	//	}
	//

	//nod->attachObject(ent);
	////if(!plane)
	////{
	//	nod->setScale(scale);
	////}

	////if(flags & OF_INVISIBLE)
	////{
	////	//only ingame invisible
	////	ent->setMaterialName("nothing");
	////}

	//OgreNewt::Body *bod = makeBody(collisionType,mass,collisionParams,nod);

	//////app->log("pos="+Ogre::StringConverter::toString(pos)+", ornt="+Ogre::StringConverter::toString(ornt));

	//bod->setPositionOrientation(pos,ornt);
	//////app->log("nach bod->setPositionOrientation(pos,ornt);");

	//currentContainer.NewtonBody = bod;
	//currentContainer.ObjId = objectID;

	//ObjectMap[nodeName] = currentContainer;
	//////app->log("am ende der func");
	return NULL;

}

//
//void Level::removeObject(Ogre::String nodeName)
//{
//	removeObject(mSceneMgr->getSceneNode(nodeName));	
//}
//
//void Level::removeObject(GameObject *obj)
//{
//	////app->log("in removeObject");
//	//Ogre::String uName = obj->getUName();
//	Ogre::SceneNode *node = obj->getSceneNode();
//	removeObject(node);
//	/*if(ObjectMap.count(node))
//	{
//		ObjectMap.erase(node);
//		////app->log("in removeObject nach erase");
//	}
//	delete obj;
//	////app->log("in removeObject nach delete");*/
//		
//}
//
//void Level::removeObject(Ogre::SceneNode *sNode)
//{
//
//	////app->log("in remove object");
//	//Ogre::String nodeName = sNode->getName();
//	if(ObjectMap.count(sNode))
//	{
//		
//		
//		GameObject *toDel = ObjectMap[sNode];
////#ifdef __editor
////		if(toDel->getType() == otEntrance)
////		{
////			mLog("lösche entrance");
////			entranceObject *entr = static_cast<entranceObject*>(toDel);
////			mLog(entr->getName());
////			if(entrances.find(entr->getName()) != entrances.end())
////			{
////				mLog("ja gibts");
////			}
////			
////			entrances.erase(entr->getName());
////		}
////#endif
//		//objekt soll sich selber aus allen maps löschen
//		delete toDel;
//		//ObjectMap.er
//
//
//		//if(temp.type == OT_CHAR)
//		//{
//		//	//char will destroy himself
//		//	GameChar *ch = static_cast<GameChar*>(temp.ptr);
//		//	////app->log("bevor charzerstörung");
//		//	delete ch;
//		//	////app->log("nach charzerstörung");
//		//	std::vector<GameChar*>::iterator itr;
//
//
//		//}
//		//else
//		//{
//
//
//		//	if(temp.NewtonBody)
//		//		delete temp.NewtonBody;
//
//		////	
//		////	
//		//	sNode->removeAndDestroyAllChildren();
//		//	while(sNode->numAttachedObjects())
//		//	{
//		//		
//		//		MovableObject *m = sNode->detachObject((unsigned short)0);
//		//		
//		//		mSceneMgr->destroyMovableObject(m);
//		//		
//		//	}
//		//	
//		//	mSceneMgr->destroySceneNode(nodeName);
//		//}
//		/*ObjectMap.erase(sNode);
//		////app->log("nach erase");*/
//	}
//	
//}
void Level::updateDelete()
{
	//mLog("update vectoren");
	ObjectList::iterator itr;
	if(!objectsToDelete.empty())
	{
		for(itr = objectsToDelete.begin();itr != objectsToDelete.end();itr++)
		{
			
			GameObject *cur = *itr;//->second;
			if(!cur->isStatic())
			{
				int curID = cur->getSGID();

				
			
				
				if(saveableObjects.find(curID) != saveableObjects.end())
				{
					if(cur->isInLevelFile)
					{
						saveableObjects[curID] = NULL;
					}
					else
					{
						saveableObjects.erase(cur->getSGID());
					}
				}
			}
			Ogre::String ptr = cur->getDebugInfo();
			delete cur;
			mLog("Object "+ptr+" has been deleted");
			*itr = NULL;
			//objectsToDelete.erase(itr);

			
		}
		objectsToDelete.clear();
	}

	//if(!objectsToRegister.empty())
	//{
	//	for(itr = objectsToRegister.begin();itr != objectsToRegister.end();itr++)
	//	{
	//		
	//		GameObject *cur = *itr;//->second;
	//		mLog("zu registrierendes gefunden: "+ptrToString(cur));
	//		mObjects.push_back(cur);
	//		
	//		//objectsToDelete.erase(itr);

	//		
	//	}
	//	objectsToRegister.clear();
	//}
	//mLog("vectorupdate fertig");
}

void Level::update(Ogre::Real time)
{
	if(destructionPrepared)
		return;

	if(mTerrainGroup)
		mTerrainGroup->update();

	//mLog("=== level update ===");
    SimpleSound::SoundManager *smgr = SimpleSound::SoundManager::getSingletonPtr();
    SimpleSound::SoundManager::SourceList::iterator sitr=mTempSources.begin();

    while(sitr != mTempSources.end())
    {
        SimpleSound::SoundManager::SourceList::iterator next = sitr;
        next++;
        if(!(*sitr)->isPlaying())
        {
            destroySource(*sitr);
            
            mTempSources.erase(sitr);
        }
        sitr=next;
    }
	
	
	

	//Newton-zeug

	/*init:
	mWorldUpdate = 1.0f / UPDATE_FRAMERATE;
	mWorldElapsed = 0.0f;
	
	#define UPDATE_FRAMERATE 120
	#define UPDATE_FRAMERATE_MIN 1/60
	#define UPDATE_FRAMERATE_MAX 1/600
	*/
	
	if(time != 0)
	{
		ObjectList::iterator itr;
		for(itr = mObjects.begin();itr != mObjects.end();itr++)
		{
			
			GameObject *cur = *itr;//->second;
			cur->update(time);
			
		}
		/*if(mWorldWaitTime < 0.85)
		{
			mWorldWaitTime += time;
		}
		else
		{*/
		
		//mWorld->update(time);
		//mWorldElapsed += time;
		//mWorldUpdate = (float)((1.0f)/(600.0f));
		//mDebugText("==time="+ogre_str(time)+"==");
		//mWorldUpdate = app->getPhysicsPrescision();
		mWorld->update(time);
		//while(mWorldElapsed > mWorldUpdate)
		//{
		//	
		//	mWorld->update(mWorldUpdate);
		//	mWorldElapsed -= mWorldUpdate;
		//	//mLog("mWorld->update("+ogre_str(mWorldUpdate)+"), mWorldElapsed="+ogre_str(mWorldElapsed));
		//}
		//}
	}
		
	////PAGING-zeug:
	//if(paging)
	//{
	//	int x,z;
	//	//Ogre::Vector3 pos = player->getPosition();
	//	getPlayerPage(x,z);
	//	
	//	if(curPage_x != x || curPage_z != z)
	//	{
	//		
	//		//umladen
	//		curPage_x = x;
	//		curPage_z = z;
	//		//mDebugText("x="+ogre_str(x)+" | z="+ogre_str(z));
	//		updatePaging();
	//	}
	//}
	//SOUND UPDATEN
#ifndef __editor
	if(!musicPreloaded)
	{
		preloadMusic();
		musicPreloaded = true;
	}
	GameApp *ga = static_cast<GameApp*>(StandardApplication::getSingletonPtr());
	if(hasMusicExplore)
	{
		
		if(!ga->music->isPlaying())
		{
			
			//nächste musik
			
			setMusic(false);
		}
		

	}
	if(hasAmbientRandom)
	{
		if(!ga->ambient1->isPlaying() &&  timeSinceLastAmbient>=timeAmbientWait)
		{
			//nächstes aus der schleife
			setAmbientRandom();
		}
	}
	timeSinceLastAmbient += time;
	if(hasAmbientLoop)
	{
		if(!ga->ambient2->isPlaying())
		{
			//nächstes aus der schleife
			setAmbientLoop();
		}
	}
#endif
	

}
//
//bool Level::isPageNeighbour(int main_x, int main_z,int x, int z)
//{
//	if((main_x == x+1 || main_x == x || main_x == x-1) && (main_z == z+1 || main_z == z || main_z == z-1))
//		return true;
//	return false;
//}
//
//void Level::loadPage(int x,int z)
//{
//	
//	setPageLoaded(x,z,true);
//
//}
//void Level::unloadPage(int x, int z)
//{
//	
//	setPageLoaded(x,z,false);
//
//}
//
//bool Level::isPageLoaded(int x, int z)
//{
//	for(int i=0;i<9;i++)
//	{
//		Page cur = mLoadedPages[i];
//		if(cur.x == x && cur.z == z && cur.loaded)
//			return true;
//	}
//	return false;
//}
//void Level::setPageLoaded(int x, int z,bool set)
//{
//	for(int i=0;i<9;i++)
//	{
//		Page cur = mLoadedPages[i];
//		if(set)
//		{			
//			if(!cur.loaded)
//			{
//				cur.x = x;
//				cur.z = z;
//				cur.loaded = true;
//				mLoadedPages[i] = cur;
//				return;
//			}
//		}
//		else
//		{
//			if(x == cur.x && z == cur.z)
//			{
//				cur.loaded = false;
//				mLoadedPages[i] = cur;
//				return;
//			}
//		}
//			
//	}
//	
//}
//
//void Level::updatePaging()
//{
//	
//	Page noLoad[9];
//	int endIndex = -1;
//	
//	for(int i=0;i<9;i++)
//	{
//		Page cur = mLoadedPages[i];
//		if(!isPageNeighbour(curPage_x,curPage_z,cur.x,cur.z))
//		{
//			if(cur.loaded)
//			{
//			//ist drin, soll aber nicht
//				unloadPage(cur.x,cur.z);
//				
//			}
//					
//			
//		}
//		else if(cur.loaded)
//		{
//			//passt. bleibt.
//			endIndex++;
//			noLoad[endIndex] = cur;
//			
//		}
//	}
//	for(int x=0;x<3;x++)
//	{
//		for(int z=0;z<3;z++)
//		{
//			bool mustLoad = true;
//			int curX = x+curPage_x-1;
//			int curZ = z+curPage_z-1;
//			for(int i=0;i<=endIndex;i++)
//			{
//				if(noLoad[i].x == curX && noLoad[i].z == curZ)
//				{
//					mustLoad = false;
//					break;
//				}
//			}
//			if(mustLoad)
//			{			
//				loadPage(curX,curZ);				
//			}
//		}
//	}
//	
//	
//}

GameObject *Level::findObject(Ogre::SceneNode *nod)
{
	GameObject *res = NULL;
	ObjectList::const_iterator itr;
	for(itr = mObjects.begin();itr != mObjects.end();itr++)
	{
		GameObject *cur = *itr;
		if(cur->getSceneNode() == nod)
		{
			res = cur;
			break;
		}
	}
	return res;

}
//
//GameObject *Level::getObject(Ogre::String nodeName)
//{
//	return getObject(mSceneMgr->getSceneNode(nodeName));
//	//if(ObjectMap.find(nodeName) != ObjectMap.end())
//	//{
//	//	return ObjectMap[nodeName];
//	//}
//	//else
//	//{
//	//	return NULL;
//	//}
//
//}
//GameObject *Level::getObject(Ogre::SceneNode *nod)
//{
//	if(ObjectMap.find(nod) != ObjectMap.end())
//	{
//		return ObjectMap[nod];
//	}
//	else
//	{
//		return NULL;
//	}
//	//ROFL
//	//return getObject(nod->getName());
//}


void Level::setPlayerControl(GameChar *target)
{
	//first, remove control from the current player
	if(player)
	{
		player->detachCam();
		player->setPlayerControlled(false);
	}
	//now set it to the target
	target->attachCam();
	target->setPlayerControlled(true);
	//update player var
	player=target;
	//jetzt der sound!
	//OgreAL::Listener* l =  OgreAL::SoundManager::getSingletonPtr()->getListener();
	//if(l->isAttached())
	//{
	//	l->getParentSceneNode()->detachObject(l);
	//}
	//player->getSceneNode()->attachObject(l);
}
void Level::restorePlayerControl()
{
	setPlayerControl(origPlayer);
}


void Level::createStandardMaterials()
{
	//sowhol newton als auch ogre
	const OgreNewt::MaterialID *defaultMaterialID = mWorld->getDefaultMaterialID();
	charMaterial = new OgreNewt::MaterialID(mWorld);
	projMaterial = new OgreNewt::MaterialID(mWorld);
	matGlass = new OgreNewt::MaterialID(mWorld);
	matWood = new OgreNewt::MaterialID(mWorld);
	matMetal = new OgreNewt::MaterialID(mWorld);
	matStone = new OgreNewt::MaterialID(mWorld);
	

	OgreNewt::MaterialPair* mpDefChar = new OgreNewt::MaterialPair( mWorld, charMaterial, defaultMaterialID );
	mpDefChar->setDefaultFriction(0,0);//0.0f, 0.0f);
	mpDefChar->setDefaultSoftness(0);
	mpDefChar->setDefaultElasticity(0);
	mpDefChar->setContinuousCollisionMode(0);
	

	//jetzt callbacks
	mCallback = new ContCallback();
	mpDefChar->setContactCallback(mCallback);

	OgreNewt::MaterialPair* projChar = new OgreNewt::MaterialPair( mWorld, charMaterial, projMaterial );
	projChar->setContactCallback(mCallback);

	OgreNewt::MaterialPair* projDef = new OgreNewt::MaterialPair( mWorld, defaultMaterialID, projMaterial );
	projDef->setContactCallback(mCallback);

	OgreNewt::MaterialPair* projProj = new OgreNewt::MaterialPair( mWorld, projMaterial, projMaterial );
	projProj->setContactCallback(mCallback);

	OgreNewt::MaterialPair* defDef = new OgreNewt::MaterialPair( mWorld, defaultMaterialID, defaultMaterialID );
	defDef->setContactCallback(mCallback);


	//jetzt ogre
	
	
	
	Ogre::ResourceManager::ResourceCreateOrRetrieveResult res = 
		Ogre::MaterialManager::getSingletonPtr()->createOrRetrieve("colorRed",Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	Ogre::MaterialPtr mat = res.first; //Ogre::MaterialManager::getSingletonPtr()->create("lichttest"+uName,Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);//Ogre::MaterialManager::getSingletonPtr()->createOrRetrieve("lichttest","general",true);
	mat->setAmbient(Ogre::ColourValue::Red);

	res = Ogre::MaterialManager::getSingletonPtr()->createOrRetrieve("colorGreen",Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	mat = res.first; //Ogre::MaterialManager::getSingletonPtr()->create("lichttest"+uName,Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);//Ogre::MaterialManager::getSingletonPtr()->createOrRetrieve("lichttest","general",true);
	mat->setAmbient(Ogre::ColourValue::Green);

	res = Ogre::MaterialManager::getSingletonPtr()->createOrRetrieve("colorBlue",Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	mat = res.first; //Ogre::MaterialManager::getSingletonPtr()->create("lichttest"+uName,Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);//Ogre::MaterialManager::getSingletonPtr()->createOrRetrieve("lichttest","general",true);
	mat->setAmbient(Ogre::ColourValue::Blue);

	res = Ogre::MaterialManager::getSingletonPtr()->createOrRetrieve("colorWhite",Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	mat = res.first; //Ogre::MaterialManager::getSingletonPtr()->create("lichttest"+uName,Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);//Ogre::MaterialManager::getSingletonPtr()->createOrRetrieve("lichttest","general",true);
	mat->setAmbient(Ogre::ColourValue::White);

	res = Ogre::MaterialManager::getSingletonPtr()->createOrRetrieve("noMaterial",Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	mat = res.first; //Ogre::MaterialManager::getSingletonPtr()->create("lichttest"+uName,Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);//Ogre::MaterialManager::getSingletonPtr()->createOrRetrieve("lichttest","general",true);
	Ogre::ColourValue nm;
	nm.a = 1;
	nm.r = 1;
	nm.b = 1;
	nm.g = 0;
	mat->setAmbient(nm);
	updateSceneManagersAfterMaterialsChange();

	
}

void Level::placePlayer(Ogre::Vector3 pos, Ogre::Quaternion ornt,GameChar *oldPlayer)
{
	if(!player)
	{
		GameChar *tempPlayer;

		try
		{
			if(!oldPlayer)
			{
				tempPlayer = new GameChar("player",this,pos,ornt);
			}
			else
			{
				tempPlayer = new GameChar(oldPlayer,this);
				tempPlayer->setInitPositionOrientation(pos,ornt);
			}
			origPlayer = tempPlayer;
		}
		catch(ExceptionType)
		{
			//////app->log("WTF? can't construct player");
			throw Ogre::Exception(0,"Could not construct player","Level class constructor");
		}

	//	obj_container currentContainer;
		/*Ogre::String nodeName = tempPlayer->getUName();*/
		//Ogre::SceneNode *nod = tempPlayer->getSceneNode();
		/*currentContainer.NewtonBody = tempPlayer->getChassisBody();
		currentContainer.ObjId="player";
		currentContainer.ptr = tempPlayer;
		currentContainer.type = OT_CHAR;*/
		//ObjectMap[nod] = static_cast<GameObject*>(tempPlayer);//currentContainer;
		//charVector.push_back(tempPlayer);
		setPlayerControl(tempPlayer);
	}
	else
	{
		player->setPositionOrientation(pos,ornt);
	}
	//if(paging)
	//{
	//	//initiieren
	//	getPlayerPage(curPage_x,curPage_z,false);
	//}

	//TEST!!!
	/*MissileData test;
	test.cData.collistionParams = Ogre::Vector3(0.1,0.1,0.5);
	test.cData.colType = CT_ELLIPSOID;
	test.cData.mass = 5;
	test.count = 1;
	test.multimissile_delay = 0;
	test.repeat = 1;
	test.velocity = 0;
	test.wData.filename = "ellipsoid.mesh";
	test.wData.type = WT_MESH;
	test.wData.scale = Ogre::Vector3(0.1,0.1,0.5);
	test.wData.material = "";*/
	/*new MissileObject(player,test, this, Ogre::Vector3(0,17,0), Ogre::Quaternion(Ogre::Degree(45),Ogre::Vector3::UNIT_Y), Ogre::Quaternion(Ogre::Degree(45),Ogre::Vector3::UNIT_Y)*Ogre::Vector3(0,0,1));
	new MissileObject(player,test, this, Ogre::Vector3(0,17,0), Ogre::Quaternion(Ogre::Degree(90),Ogre::Vector3::UNIT_Y), Ogre::Quaternion(Ogre::Degree(90),Ogre::Vector3::UNIT_Y)*Ogre::Vector3(0,0,1));
	*//*new MissileObject(player,test, this, Ogre::Vector3(0,17,0), Ogre::Quaternion(Ogre::Degree(135),Ogre::Vector3::UNIT_Y), Ogre::Quaternion(Ogre::Degree(135),Ogre::Vector3::UNIT_Y)*Ogre::Vector3(0,0,1));
	new MissileObject(player,test, this, Ogre::Vector3(0,17,0), Ogre::Quaternion(Ogre::Degree(180),Ogre::Vector3::UNIT_Y), Ogre::Quaternion(Ogre::Degree(180),Ogre::Vector3::UNIT_Y)*Ogre::Vector3(0,0,1));
	new MissileObject(player,test, this, Ogre::Vector3(0,17,0), Ogre::Quaternion(Ogre::Degree(225),Ogre::Vector3::UNIT_Y), Ogre::Quaternion(Ogre::Degree(225),Ogre::Vector3::UNIT_Y)*Ogre::Vector3(0,0,1));
	new MissileObject(player,test, this, Ogre::Vector3(0,17,0), Ogre::Quaternion(Ogre::Degree(270),Ogre::Vector3::UNIT_Y), Ogre::Quaternion(Ogre::Degree(270),Ogre::Vector3::UNIT_Y)*Ogre::Vector3(0,0,1));
	new MissileObject(player,test, this, Ogre::Vector3(0,17,0), Ogre::Quaternion(Ogre::Degree(315),Ogre::Vector3::UNIT_Y), Ogre::Quaternion(Ogre::Degree(315),Ogre::Vector3::UNIT_Y)*Ogre::Vector3(0,0,1));
	new MissileObject(player,test, this, Ogre::Vector3(0,17,0), Ogre::Quaternion(Ogre::Degree(360),Ogre::Vector3::UNIT_Y), Ogre::Quaternion(Ogre::Degree(360),Ogre::Vector3::UNIT_Y)*Ogre::Vector3(0,0,1));*/

}

GameObject *Level::getObjectBySGID(int sgid)
{
	
	//if(sgid == -1)
	//	return static_cast<GameObject*>(getPlayer());
	if(saveableObjects.find(sgid) != saveableObjects.end())
		return saveableObjects[sgid];
	return NULL;
	
}

void Level::placePlayer(Ogre::String entranceName,GameChar *oldPlayer)
{
	if(entrances.find(entranceName) != entrances.end())
	{
		OrientedPoint entr = entrances[entranceName];
		placePlayer(entr.pos,entr.orient,oldPlayer);
	}
	else
	{
		placePlayer(Ogre::Vector3::ZERO,Ogre::Quaternion::IDENTITY,oldPlayer);
	}
}
//
//void Level::createETMaterialPasses(Ogre::Technique *tech,unsigned int texturesPerPass)
//{
//	using namespace Ogre;
//	if(texturesPerPass > 12)
//		texturesPerPass = 12;
//	Ogre::Pass *sampleTexPass = tech->getPass(0);
//	Ogre::Pass *sampleLightPass = tech->getPass(1);
//
//	
//	
//	Ogre::String texureVertexName = sampleTexPass->getVertexProgramName();
//	Ogre::GpuProgramParametersSharedPtr textureVertexParams = sampleTexPass->getVertexProgramParameters();
//	Ogre::String lightmapVertexName = sampleLightPass->getVertexProgramName();
//	Ogre::GpuProgramParametersSharedPtr lightmapVertexParams = sampleLightPass->getVertexProgramParameters();
//	
//	Ogre::String textureFragmentName = sampleTexPass->getFragmentProgramName();//das ist das splatting program
//	Ogre::GpuProgramParametersSharedPtr textureFragmentParams = sampleTexPass->getFragmentProgramParameters();
//	Ogre::String lightmapFragmentName = sampleLightPass->getFragmentProgramName();
//	Ogre::GpuProgramParametersSharedPtr lightmapFragmentParams = sampleLightPass->getFragmentProgramParameters();
//	
//	tech->removeAllPasses();
//
//	
//
//	unsigned int numCovMaps = mSplatMgr->getNumMaps();
//	unsigned int numTextures= terrainTextures.size();
//    bool fallback = false;
//	if(texturesPerPass == 0)
//	{
//		//fallback-technick
//        fallback = true;
//		texturesPerPass = 3;
//		textureFragmentName = "ET/Programs/FallbackSplat_";//+ogre_str(texturesPerPass)+"tex";
//	}
//	else
//	{
//		if(texturesPerPass > numTextures)
//		{
//			//sonst gibt es zu viele covmaps
//			texturesPerPass = numTextures;
//		}
//		//normal
//		textureFragmentName = "ET/Programs/Splat_";//+ogre_str(texturesPerPass)+"tex";
//	}
//	
//	unsigned int covMapsPerPass = Ogre::Math::ICeil(texturesPerPass/3.0f);
//	unsigned int numTexUnitStatesPerPass = covMapsPerPass+texturesPerPass;
//	unsigned int numPasses = 0;
//    if(texturesPerPass != 0)
//       numPasses = Ogre::Math::ICeil(float(numTextures)/float(texturesPerPass));
//	
//	
//	for(unsigned int passnr = 0;passnr<numPasses;passnr++)
//	{
//		//jetzt schauen wie viele texturen ich hier adden muss		
//		unsigned int texturesLeft = numTextures-texturesPerPass*passnr;
//        unsigned int covmapsLeft = numCovMaps-covMapsPerPass*passnr;
//		unsigned int texturesThisPass;
//        unsigned int covmapsThisPass;
//		if(texturesLeft >= texturesPerPass)
//		{
//			//dann habe ich noch genug, diesmal wird noch das maximum geadded
//			texturesThisPass = texturesPerPass;
//		}
//		else
//		{
//			//es sind weniger übrig als in einen pass sollen
//			texturesThisPass = texturesLeft;
//		}
//        if(covmapsLeft >= covMapsPerPass)
//		{
//			//dann habe ich noch genug, diesmal wird noch das maximum geadded
//			covmapsThisPass = covMapsPerPass;
//		}
//		else
//		{
//			//es sind weniger übrig als in einen pass sollen
//			covmapsThisPass = covmapsLeft;
//		}
//		//neuen pass erstellen
//		Ogre::Pass *pass = tech->createPass();
//		pass->setLightingEnabled(false);
//        if(passnr != 0)
//            pass->setSceneBlending(Ogre::SBT_ADD);
//		pass->setSceneBlending(SBT_TRANSPARENT_ALPHA);
//		
//		pass->setAlphaRejectSettings(CMPF_GREATER,220);
//		pass->setVertexProgram(texureVertexName);
//		pass->setVertexProgramParameters(textureVertexParams);
//		
//		pass->setFragmentProgram(textureFragmentName+ogre_str(texturesThisPass)+"tex");
//		pass->setFragmentProgramParameters(textureFragmentParams);
//		//units adden
//		//zuerst die covmaps
//		for(unsigned int covmapnr=0;covmapnr<covmapsThisPass;covmapnr++)
//		{
//			TextureUnitState *tex = pass->createTextureUnitState();
//			tex->setTextureName(splattingBasename+ogre_str( passnr*covMapsPerPass+covmapnr ));
//		}
//		//jetzt die texturen
//		for(unsigned int texnr = 0;texnr < texturesThisPass;texnr++)
//		{
//			unsigned int index = passnr*texturesPerPass+texnr;
//			/*if(index >= numTextures)
//				break;*/
//			Ogre::String texName = terrainTextures[index];
//			TextureUnitState *tex = pass->createTextureUnitState();
//			tex->setTextureName(texName);
//		}
//	}
//	
//	//und jetzt der lightmap pass
//	
//    Ogre::Pass *lightPass = tech->createPass();
//	lightPass->setVertexProgram(lightmapVertexName);
//	lightPass->setVertexProgramParameters(lightmapVertexParams);
//	lightPass->setFragmentProgram(lightmapFragmentName);
//	lightPass->setFragmentProgramParameters(lightmapFragmentParams);
//	lightPass->setSceneBlending(SBT_MODULATE);
//	lightPass->createTextureUnitState()->setTextureName(terrainLightmap->getName());
//    
//	//texturesPerPass = textureUnitsPerPass * 3 / 4;
//
//}

void Level::createETMaterial()
{	
	//Ogre::MaterialManager	*matMgr = Ogre::MaterialManager::getSingletonPtr();
	////test
	//
	////Material klonen
	//Ogre::MaterialPtr baseMaterial = matMgr->getByName("ETTerrainMaterialBase");
	//

	//Ogre::MaterialPtr mat = matMgr->getByName("ETTerrainMaterial"+mFileName);
	//if(!mat.isNull())
	//{
	//	matMgr->remove(mat->getName());
	//}
	//if(!terrainMaterial.isNull())
	//{
	//	matMgr->remove(terrainMaterial->getName());
	//}
	//unsigned int numCovMaps = mSplatMgr->getNumMaps();
	//unsigned int numTextures= terrainTextures.size();
	////material ETTerrainMaterial
	//terrainMaterial = baseMaterial->clone("ETTerrainMaterial"+mFileName);//matMgr->create("ETTerrainMaterial"+mFileName,"ET");
	//
	//Ogre::Technique *mainTech = terrainMaterial->getTechnique(0);
	////jetzt die haupttechnik
	//createETMaterialPasses(mainTech,9);
	////und die fallback
	//Ogre::Technique *fallbackTech = terrainMaterial->getTechnique(1);
	//createETMaterialPasses(fallbackTech,0);


	//updateSceneManagersAfterMaterialsChange();
 //   Ogre::MaterialSerializer out;
 //   out.exportMaterial(terrainMaterial,"test.material");
}

//entfernt das terrain
void Level::removeTerrain()
{
	has_terrain = false;
	if (mTerrainPaging)
	{
		OGRE_DELETE mTerrainPaging;
		OGRE_DELETE mPageManager;
	}
	else
	{
		OGRE_DELETE mTerrainGroup;

		
	}
	OGRE_DELETE mTerrainGlobals;
    /*if(terrainBody)
    {
        delete terrainBody;
        terrainBody = NULL;
    }
    if(terrainCollision)
    {
        delete terrainCollision;
        terrainCollision = NULL;
    }
    if(mTerrainMgr)
    {
        delete mTerrainMgr;
        mTerrainMgr = NULL;
    }
    if(mSplatMgr)
    {
        delete mSplatMgr;
        mSplatMgr = NULL;
    }
    if(!terrainMaterial.isNull())
		Ogre::MaterialManager::getSingletonPtr()->remove((Ogre::ResourcePtr)terrainMaterial);
    if(!terrainLightmap.isNull())
        Ogre::TextureManager::getSingletonPtr()->remove((Ogre::ResourcePtr)terrainLightmap);
    mTerrainInfo = NULL;
    terrainTextures.clear();*/
    /*Ogre::MaterialPtr terrainMaterial;
	Ogre::TexturePtr terrainLightmap;*/
}
//erzeugt ein leeres terrain mit ohne gar nichts
void Level::addBlankTerrain(unsigned int heightmapWidth,unsigned int heightmapHeight,unsigned int lightmapWidth,unsigned int lightmapHeight,unsigned int splattingWidth,unsigned int splattingHeight)
{
//
//  
//    if(mTerrainMgr)
//        return;
//
//    terrainTextures.push_back("default.png");
//
//    hmWidth = heightmapWidth;
//    hmHeight= heightmapHeight;
//    lmWidth = lightmapWidth;
//    lmHeight= lightmapHeight;
//    splatHeight = splattingHeight;
//    splatWidth  = splattingWidth;
////    splatChannels= splattingChannels;
//	mTerrainMgr = new ET::TerrainManager(mSceneMgr);
//	mTerrainMgr->setLODErrorMargin(2, StandardApplication::getSingletonPtr()->getRenderWindow()->getViewport(0)->getActualHeight());
//	mTerrainMgr->setUseLODMorphing(true, 0.2, "morphFactor");
//    // create a fresh, mid-high terrain for editing
//    ET::TerrainInfo info (heightmapWidth, heightmapHeight, std::vector<float>(heightmapWidth*heightmapHeight, 0.5f));
//    // set position and size of the terrain
//    info.setExtents(terrainExtends);
//	
//	
//	
//	// now render it
//	mTerrainMgr->createTerrain(info);
//	mTerrainInfo = & mTerrainMgr->getTerrainInfo();
//	
//	
//	//gleich die texturen holen und zählen
//    //hm, erstmal keine texturen. mal sehen ob es damit klarkommt
//
//	unsigned int numTex = 1;//splatElem->AttributeAsUnsignedInt("numtextures",1);
//	
//
//	splattingBasename = "ETSplatting"+mFileName;//splatElem->Attribute("basename");
//	
//	
//	// create the splatting manager
//	mSplatMgr = new ET::SplattingManager(splattingBasename, "ET", splatWidth, splatHeight, splatChannels);
//	// specify number of splatting textures we need to handle
//    //ich hoffe das kann man ändern
//	mSplatMgr->setNumTextures(numTex);
//	
//	
//	
//	
//	//bool shadowed = lightElem->AttributeAsBool("shadowed");
//	// create a manual lightmap texture
//	terrainLightmap = Ogre::TextureManager::getSingleton().createManual(
//		"ETLightmap"+mFileName, "ET", Ogre::TEX_TYPE_2D, lmWidth, lmHeight, 1, Ogre::PF_BYTE_RGB);
//	Ogre::Image lightmap;
//	
//	
//	// load the terrain material and assign it
//	createETMaterial();
//	//terrainMaterial = Ogre::MaterialManager::getSingleton().getByName("ETTerrainMaterial");
//	mTerrainMgr->setMaterial(terrainMaterial);
//	
//
//
//
//
//	// get our dynamic texture and update its contents
//	//Ogre::TexturePtr tex = TextureManager::getSingleton().getByName("ETLightmap");
//	//ET::createTerrainLightmap(mTerrainInfo, lightmap, lmWidth, lmHeight,lightDir, etLightColor,
//	//ambient,shadowed);
//	updateTerrainLightmap();
}

void Level::processTerrainSettings(TiXmlElement *_elem)
{
    //das wird jetzt überarbeitet
    /*
    terrain kann nur noch aus zip geladen werden
    wenn levelSGfile = null, dann kein terrain.
    heightmap heißt immer heightmap.png und befindet sich im terrain ordner in der zip
    covmaps heißen coverage.x.png und sind auch dort
    splattingBasename = "ETSplatting"+mFileName;
    texturen bleiben wie bisher
    */
//	if(!_elem)
//		return;
//    if(!levelSGfile)
//    {
//        mLog("terrain-informationen in einer nichtgezippten leveldatei: "+mFileName);
//        return;
//    }
//	OgreXmlElement *elem = static_cast<OgreXmlElement*>(_elem);
//	mTerrainMgr = new ET::TerrainManager(mSceneMgr);
//	mTerrainMgr->setLODErrorMargin(2, StandardApplication::getSingletonPtr()->getRenderWindow()->getViewport(0)->getActualHeight());
//	mTerrainMgr->setUseLODMorphing(true, 0.2, "morphFactor");
//	// create a fresh, mid-high terrain for editing
//	
//	// set position and size of the terrain
//	//terrainInfo.setExtents(Ogre::AxisAlignedBox(0, 0, 0, 1500, 300, 1500));
//	//Ogre::String heightmap = elem->Attribute("heightmap");
//    Ogre::Image image = levelSGfile->getFileAsImage("terrain/heightmap.png");
//
//    hmWidth = image.getWidth();
//    hmHeight= image.getHeight();
//   
//    //image.loadRawData(
//	//image.load(heightmap, "ET");
//	ET::TerrainInfo info;
//	//extends holen
//	Ogre::Vector3 extendsMin = elem->AttributeAsVector3("extends_min",Ogre::Vector3(-100,-100,-100));
//	Ogre::Vector3 extendsMax = elem->AttributeAsVector3("extends_max",Ogre::Vector3(100,100,100));
//	Ogre::Vector3 worldMin = extendsMin;
//	Ogre::Vector3 worldMax = extendsMax;
//	if(worldMin.x > -1000)
//		worldMin.x = -1000;
//	if(worldMin.y > -1000)
//		worldMin.y = -1000;
//	if(worldMin.z > -1000)
//		worldMin.z = -1000;
//	if(worldMax.x < 1000)
//		worldMax.x = 1000;
//	if(worldMax.y < 1000)
//		worldMax.y = 1000;
//	if(worldMax.z < 1000)
//		worldMax.z = 1000;
//    terrainExtends = Ogre::AxisAlignedBox(extendsMin,extendsMax);
//	//nun ogrenewt
//	mWorld->setWorldSize(extendsMin,extendsMax);
//	ET::loadHeightmapFromImage(info, image);
//	info.setExtents(terrainExtends);
//
//	
//	// now render it
//	mTerrainMgr->createTerrain(info);
//	mTerrainInfo = & mTerrainMgr->getTerrainInfo();
//	
//	/*<terrain heightmap="ETterrain.png">
//        <splatting basename="ETSplatting" numtextures="6" width="128" height="128" channels="3">
//            <coverage image="ETcoverage.0.png" mapnum="0" />
//            <coverage image="ETcoverage.1.png" mapnum="1" />
//        </splatting>        
//        <lightmap width="128" height="128" lightdir="1, -1, 1" lightcol="1 1 1 1" ambient="0.3 0.3 0.3" shadowed="true" />     
//    </terrain>*/
//	OgreXmlElement *splatElem = elem->FirstChildElement("splatting");
//
//	//gleich die texturen holen und zählen
//	unsigned int numTex = 0;//splatElem->AttributeAsUnsignedInt("numtextures",1);
//	OgreXmlElement *texElem = splatElem->FirstChildElement("texture");
//	while(texElem)
//	{
//		Ogre::String img = texElem->Attribute("image");
//		//Ogre::uint nr = texElem->AttributeAsUnsignedInt("mapnum");
//		terrainTextures.push_back(img);
//		/*image.load(img, "ET");
//		mSplatMgr->loadMapFromImage(nr, image);*/
//		texElem = texElem->NextSiblingElement("texture");
//		numTex++;
//	}
//
//	splattingBasename = "ETSplatting"+mFileName;//splatElem->Attribute("basename");
//	mLog(splattingBasename);
//	splatWidth = splatElem->AttributeAsUnsignedInt("width",128);
//	splatHeight = splatElem->AttributeAsUnsignedInt("height",128);
////    splatChannels = splatElem->AttributeAsUnsignedInt("channels",4);
//	// create the splatting manager
//	mSplatMgr = new ET::SplattingManager(splattingBasename, "ET", splatWidth, splatHeight, splatChannels);
//	// specify number of splatting textures we need to handle
//	mSplatMgr->setNumTextures(numTex);
//	//get coverage info
//	//OgreXmlElement *covElem = splatElem->FirstChildElement("coverage");
//    for(Ogre::uint i = 0;i<mSplatMgr->getNumMaps();i++)
//    {
//        Ogre::String name = "terrain/coverage."+ogre_str(i)+".png";
//        Ogre::Image img = levelSGfile->getFileAsImage(name);
//        mSplatMgr->loadMapFromImage(i, img);
//    }
//	/*Ogre::uint i = 0;
//	while(covElem && i < mSplatMgr->getNumMaps())
//	{
//		Ogre::String img = covElem->Attribute("image");
//		Ogre::uint nr = covElem->AttributeAsUnsignedInt("mapnum");
//		image.load(img, "ET");
//		mSplatMgr->loadMapFromImage(nr, image);
//		covElem = covElem->NextSiblingElement("coverage");
//		i++;
//	}*/
//	
//	//load lightmap stuff
//	OgreXmlElement *lightElem = elem->FirstChildElement("lightmap");
//	//<lightmap width="128" height="128" lightdir="1, -1, 1" lightcol="1 1 1 1" ambient="0.3 0.3 0.3" shadowed="true" />     
//	lmWidth = lightElem->AttributeAsUnsignedInt("width");
//	lmHeight = lightElem->AttributeAsUnsignedInt("height");
//	etLightDirection = lightElem->AttributeAsVector3("lightdir");
//	etLightColor = lightElem->AttributeAsColour("lightcol");
//	etAmbientColor = lightElem->AttributeAsColour("ambient");
//	//bool shadowed = lightElem->AttributeAsBool("shadowed");
//	// create a manual lightmap texture
//	terrainLightmap = Ogre::TextureManager::getSingleton().createManual(
//		"ETLightmap"+mFileName, "ET", Ogre::TEX_TYPE_2D, lmWidth, lmHeight, 1, Ogre::PF_BYTE_RGB);
//	
//	
//	// load the terrain material and assign it
//	createETMaterial();
//	//terrainMaterial = Ogre::MaterialManager::getSingleton().getByName("ETTerrainMaterial");
//	mTerrainMgr->setMaterial(terrainMaterial);
//	
//
//	//load or create lightmap
//	Ogre::Image lightmap;	
//	if(levelSGfile->hasFile("terrain/lightmap.png"))
//	{
//		lightmap = levelSGfile->getFileAsImage("terrain/lightmap.png");
//		terrainLightmap->getBuffer(0, 0)->blitFromMemory(lightmap.getPixelBox(0, 0));
//	}	
//	else
//	{
//		
//		// get our dynamic texture and update its contents
//		//Ogre::TexturePtr tex = TextureManager::getSingleton().getByName("ETLightmap");
//		//ET::createTerrainLightmap(mTerrainInfo, lightmap, lmWidth, lmHeight,lightDir, etLightColor,
//		//ambient,shadowed);
//		updateTerrainLightmap();
//	}
//	
//    //generateTerrainCollision();
//    //collision laden
//    
//    OgreNewt::CollisionPrimitives::TreeCollision *col = levelSGfile->getTreeCollision("collision/terrain.col",mWorld);
//    if(col)
//    {
//        terrainBody = new OgreNewt::Body(mWorld,col);
//        terrainCollision=col;
//    }

}

Ogre::String Level::objectToXml(GameObject *obj)
{
	TiXmlDocument doc;
	TiXmlElement *elem = obj->getXmlElem();//objectToXmlElem(obj);
	if(!elem)
		return "";
	doc.LinkEndChild(elem);
	
	
	TiXmlPrinter *test = new TiXmlPrinter();
	
	doc.Accept(test);
	Ogre::String muh = test->CStr();
	return muh;
}

void Level::processSGID(GameObject *obj)
{
	if(obj->isStatic())
		return;
	int curId = obj->getSGID();
    //objekt will eine best. ID haben
	if(curId)
	{
		if(saveableObjects.find(curId) != saveableObjects.end())
		{
			mLog("Warnung: SGID "+ogre_str(curId)+" wird bereits verwendet!");
            //es kriegt aber doch ne andere
		}
		else
		{
			saveableObjects[curId] = obj;
			
			return;
		}
	}
	//letzte SGID validieren und ggf. resynchen 
	while(saveableObjects.find(lastSGID) != saveableObjects.end())
	{
		lastSGID++;
	}
	obj->setSGID(lastSGID);
	saveableObjects[lastSGID] = obj;
	
	

}

Ogre::String Level::getSavegameXML()
{
	//	StandardApplication::getSingletonPtr()->setDebugText("save to "+filename);
	//Ogre::String saveTo = levelPath+"/"+currentLevel->getFileName();
	TiXmlDocument doc;
	//now try to write all the stuff to it...
	
	TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );


	TiXmlElement * levelElement = new TiXmlElement( "level" );
	//TiXmlElement * soundElement = new TiXmlElement( "sound" );
	TiXmlElement * objectElement = new TiXmlElement( "objects" );
	
	levelElement->SetAttribute("version",ogre_str(CUR_LEVEL_VERSION).c_str());
	levelElement->SetAttribute("last_sgid",ogre_str(lastSGID).c_str());

	
	for(SaveableObjectList::iterator itr = saveableObjects.begin();
		itr != saveableObjects.end();itr++)
	{
		
		GameObject *obj= itr->second;
		if(!obj)
		{
			TiXmlElement* xmlElem = new TiXmlElement("deleted");
			xmlElem->SetAttribute("SGID",ogre_str(itr->first));
			objectElement->LinkEndChild(xmlElem);
			continue;
		}
		
		if(obj->isStatic())
			continue;
		
		
		TiXmlElement *objelem = obj->getXmlElem(Ogre::Vector3::ZERO,true);//objectToXmlElem(obj);
		if(objelem)
			objectElement->LinkEndChild(objelem);
		
		
	}

	


	

	
	doc.LinkEndChild( decl );
	doc.LinkEndChild( levelElement );
	//doc.LinkEndChild( soundElement );
	doc.LinkEndChild( objectElement );
	
	std::stringstream st;
	st << doc;
	return st.str();
	
}



void Level::updateTextureList(Ogre::StringVector newList)
{
    ////zu jeder neuen textur schauen, ob es schon einen covmapchannel gibt
    ////wenn ja, diesen holen
    ////erstmal, wie viele covmaps brauchen wir?
    //size_t newTexCount = newList.size();

    ////mSplatMgr->setNumTextures(newCount);
    //unsigned int numCovMaps = Ogre::Math::ICeil(float(newTexCount) / float(splatChannels));
    ////images für die neuen covmaps
    //std::vector<Ogre::Image> newCovmaps;
    //newCovmaps.reserve(numCovMaps);
    //unsigned int curCovmap = 0;
    //unsigned int curChannel= 0;
    //unsigned int bufSize = splatChannels*splatWidth*splatHeight;
    //for(unsigned int i=0;i<numCovMaps;i++)
    //{
    //    //buffer für diese map vorbereiten
    //    
    //    Ogre::Image curImage;
    //    Ogre::uchar *buffer = new Ogre::uchar[bufSize];
    //    initBuffer(buffer,bufSize,0);
    //    //jetzt die [splatChannels] texturen durchgehen
    //    for(unsigned int j=0;j<splatChannels;j++)
    //    {
    //        unsigned int curTexIndex = i*numCovMaps+j;
    //        if(curTexIndex>=newList.size())
    //            break;
    //        Ogre::String curTex = newList[curTexIndex];
    //        int oldIndex = getOldTextureIndex(curTex);
    //        if(oldIndex != -1)
    //        {
    //            //existiert schon, ich muss den channel klauen
    //            //erstmal das zugehörige image holen
    //            Ogre::Image img;
    //            mSplatMgr->saveMapToImage(covmapNrFromTextureIndex(oldIndex,splatChannels),img);
    //            //jetzt den richtigen kanal davon
    //            //und davon aus dem buffer den richtigen channel. entspricht dem wert, um den erhöht werden soll
    //            //wenn negativ, dann ist das Offset = Abs(imgChannelCnt-1-Offset);
    //            //int imgChannelCnt = pixelFormatToNumChannels(img.getFormat());//das ist wie viele channel es hat. negativ=umgekehrte reihenfolge
    //            //jetzt den kanal im image. entspricht dem Offset
    //            int channelInImg = oldIndex % splatChannels;//channelNrFromTextureIndex(oldIndex,Ogre::Math::Abs(imgChannelCnt));
    //            /*if(imgChannelCnt < 0)
    //            {
    //                imgChannelCnt *= -1;
    //                channelInImg = imgChannelCnt-1-channelInImg;
    //            }
    //            */
    //            //und jetzt endlich channel klauen...
    //            Ogre::uchar *oldBuffer = img.getData();
    //            for(unsigned int k=0;k<bufSize;k += splatChannels)
    //            {
    //                buffer[k+j] = oldBuffer[k+channelInImg];
    //                //[k+channelInImg] <- für den alten
    //                //[k+j] <- für den neuen

    //                //das k ist jetzt das Index für den alten buffer
    //                //j ist die nr. des neuen channels, im neuen buffer
    //            }
    //            
    //        }
    //    }
    //    curImage.loadDynamicImage(buffer,splatWidth,splatHeight,1,numChannelsToPixelFormat(splatChannels),true);
    //    newCovmaps.push_back(curImage);        
    //}
    //
    ////und jetzt nochmal wg der löcher
    //for(size_t i=0;i<bufSize;i+=3)
    //{
    //    bool isHole = true;
    //    Ogre::uchar p1 = 0;
    //    Ogre::uchar p2 = 0;
    //    Ogre::uchar p3 = 0;
    //    for(size_t j=1;j<newCovmaps.size();j++)
    //    {
    //        p1 += newCovmaps[j].getData()[i];
    //        p2 += newCovmaps[j].getData()[i+1];
    //        p3 += newCovmaps[j].getData()[i+2];
    //        

    //    }
    //    if(p1+p2+p3+newCovmaps[0].getData()[i]
    //        +newCovmaps[0].getData()[i+1]
    //        +newCovmaps[0].getData()[i+2] < 255)
    //    {
    //        newCovmaps[0].getData()[i] = 255-p1-p2-p3-newCovmaps[0].getData()[i+1]-newCovmaps[0].getData()[i+2];
    //    }
    //   
    //    

    //}
    //terrainTextures = newList;
    //mSplatMgr->setNumTextures(newTexCount);     
    //for(Ogre::uint i = 0;i<mSplatMgr->getNumMaps();i++)
    //{       
    //    mSplatMgr->loadMapFromImage(i, newCovmaps[i]);
    //}
    //createETMaterial();
    //updateTerrainLightmap();
    //mTerrainMgr->setMaterial(terrainMaterial);
}

//Ogre::Vector3 Level::getTerrainVertex(size_t x,size_t z)
//{
//   
//    //Ogre::Vector3 res;
//    //
//    //res.x = mTerrainInfo->getOffset().x + mTerrainInfo->getScaling().x * x;
//    //res.y = mTerrainInfo->getOffset().y + mTerrainInfo->at(x,z) * mTerrainInfo->getScaling().y;
//
//    //res.z = mTerrainInfo->getOffset().z + mTerrainInfo->getScaling().z * z;
//    //return res;
//    ////mTerrainInfo->getScaling
//}
//

//void Level::generateTerrainCollision()
//{
//	////covmap und channel der lochmap holen
//	//int texIndex = -1;
//	//for(unsigned int i=0;i<terrainTextures.size();i++)
//	//{
//	//	if(terrainTextures[i] == "hole.png")
//	//	{
//	//		texIndex = i;
//	//		break;
//	//	}
//	//}
//	//Ogre::Image holesCovmap;
//	//int covmapNr = Ogre::Math::IFloor(float(texIndex)/float(splatChannels));
//	//int channelNr= Ogre::Math::IFloor(texIndex % splatChannels);
//	//if(texIndex != -1)
//	//	mSplatMgr->saveMapToImage(covmapNr,holesCovmap);
//
//	////Ogre::ColourValue ist r g b a
// //   OgreNewt::CollisionPrimitives::TreeCollision *col = new OgreNewt::CollisionPrimitives::TreeCollision(mWorld);
// //   col->start();
// //   for(size_t x=0;x<hmWidth-1;x++)
// //   {
// //       for(size_t z=0;z<hmHeight-1;z++)
// //       {
// //           //|   |
// //           //4---3-
// //           //| \ |
// //           //1---2-
// //                       
// //           //1:(x,z)
// //           //2:(x+1,z)
// //           //3:(x+1,z+1)
// //           //4:(x,z+1)
// //           Ogre::Vector3 v1 = getTerrainVertex(x,z);
// //           Ogre::Vector3 v2 = getTerrainVertex(x+1,z);
// //           Ogre::Vector3 v3 = getTerrainVertex(x+1,z+1);
// //           Ogre::Vector3 v4 = getTerrainVertex(x,z+1);
//	//		Ogre::Vector3 poly_verts[3];
//	//		int nrOfHoles = 0;
//	//		if(texIndex != -1)
//	//		{
//	//			//koordinaten in holemapcoords transformieren
//	//			
//	//			/*int hole_x = round ( float(x)/float(hmWidth)  * float(holesCovmap.getWidth())  );
//	//			int hole_z = round ( float(z)/float(hmHeight) * float(holesCovmap.getHeight()) );*/
//	//			//mLog("hole_x="+ogre_str(hole_x)+", hole_z="+ogre_str(hole_z)+", x="+ogre_str(x)+", z="+ogre_str(z));
//	//			Ogre::Real val = holesCovmap.getColourAt(x,z,0)[channelNr];
//	//			//Ogre::Real val = holesCovmap.getColourAt(hole_x,hole_z,0)[channelNr];
//	//			Ogre::Real cmpval = 0.01;
//	//			if(val > cmpval)
//	//				continue;
//	//			//Ogre::Real v1val = holesCovmap.getColourAt(hole_x,hole_z,0)[channelNr];
//	//			//Ogre::Real v2val = holesCovmap.getColourAt(hole_x+1,hole_z,0)[channelNr];
//	//			//Ogre::Real v3val = holesCovmap.getColourAt(hole_x+1,hole_z+1,0)[channelNr];
//	//			//Ogre::Real v4val = holesCovmap.getColourAt(hole_x,hole_z+1,0)[channelNr];
//	//			//
//	//			//
//	//			//if(v1val > cmpval)
//	//			//	nrOfHoles++;
//	//			//if(v2val > cmpval)
//	//			//	nrOfHoles++;
//	//			//if(v3val > cmpval)
//	//			//	nrOfHoles++;
//	//			//if(v4val > cmpval)
//	//			//	nrOfHoles++;
//	//			//if(nrOfHoles >= 2)
//	//			//	continue;
//	//			//if(v1val > cmpval)
//	//			//{
//	//			//	//|   |
//	//			//	//4---3-
//	//			//	//| \x|
//	//			//	// ---2-
//	//			//	poly_verts[2] = v2;
//	//			//	poly_verts[1] = v3;
//	//			//	poly_verts[0] = v4;
//	//			//	col->addPoly( poly_verts, 0 ); 
//
//	//			//}
//	//			//else if(v2val > cmpval)
//	//			//{
//	//			//	//|   |
//	//			//	//4---3-
//	//			//	//|x/ |
//	//			//	//1--- -
//	//			//	poly_verts[2] = v3;
//	//			//	poly_verts[1] = v4;
//	//			//	poly_verts[0] = v1;
//	//			//	col->addPoly( poly_verts, 0 ); 
//	//			//	
//	//			//}
//	//			//else if(v3val > cmpval)
//	//			//{
//	//			//	//|   |
//	//			//	//4--- -
//	//			//	//|x\ |
//	//			//	//1---2-
//	//			//	poly_verts[2] = v2;
//	//			//	poly_verts[1] = v4;
//	//			//	poly_verts[0] = v1;
//	//			//	col->addPoly( poly_verts, 0 ); 
//	//			//	
//	//			//}
//	//			//else if(v4val > cmpval)
//	//			//{
//	//			//	//|   |
//	//			//	// ---3-
//	//			//	//| /x|
//	//			//	//1---2-
//	//			//	poly_verts[2] = v3;
//	//			//	poly_verts[1] = v1;
//	//			//	poly_verts[0] = v2;
//	//			//	col->addPoly( poly_verts, 0 ); 
//	//			//	
//	//			//}
//	//		}            
//	//		if(nrOfHoles == 0)
//	//		{
//	//			poly_verts[2] = v1;
//	//			poly_verts[1] = v2;
//	//			poly_verts[0] = v4;
//	//			col->addPoly( poly_verts, 0 ); 
//
//	//			poly_verts[2] = v2;
//	//			poly_verts[1] = v3;
//	//			poly_verts[0] = v4;
//	//			col->addPoly( poly_verts, 0 ); 
//	//		}
// //           
// //       }
// //   }
// //   col->finish(false);
// //   if(terrainCollision)
// //       delete terrainCollision;
// //   if(terrainBody)
// //       delete terrainBody;
// //   terrainCollision = col;
// //   terrainBody = new OgreNewt::Body(mWorld,col);
// //   //OgreNewt::CollisionPrimitives::TreeCollision *col = new OgreNewt::CollisionPrimitives::TreeCollision(mWorld);
// //   //col->a
// //   //mTerrainMgr->createTerrain(
// //   //const std::vector<float> heightmap = mTerrainInfo->getHeightmapData();
//
// //   //OgreNewt::TreeCollisionSerializer *ser = new OgreNewt::TreeCollisionSerializer();
// //   //ser->exportTreeCollision
//}
//

void Level::setTerrainExtends(Ogre::AxisAlignedBox extends)
{
    //if(terrainExtends == extends)
    //    return;
    //ET::TerrainInfo info = *mTerrainInfo;
    //info.setExtents(extends);
    //mTerrainMgr->destroyTerrain();
    //mTerrainMgr->createTerrain(info);
    //mTerrainInfo = &mTerrainMgr->getTerrainInfo();
    ////const_cast<ET::TerrainInfo*>(mTerrainInfo)->setExtents(extends);
    ////getTerrainInfo()->setExtents(&extends);
    //terrainExtends = extends;
}

void Level::updateTerrainLightmap()
{
	//Ogre::Image lightmap;
	////mTerrainInfo = mTerrainMgr
 //   
 //   ET::createTerrainLightmap(*mTerrainInfo, lightmap, lmWidth, lmHeight, etLightDirection, etLightColor,
	//etAmbientColor);
	////Ogre::ResourceManager::ResourceCreateOrRetrieveResult texRes = TextureManager::getSingleton().createOrRetrieve("ETLightmap",Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,true);
 //   
	////nun die lightmap den löchern entsprechend updaten
	//int texIndex = -1;
	//if(hasHoleMap())
	//{
	//	texIndex = 0;
	//}
	//
	//Ogre::Image holesCovmap;
	//int covmapNr = Ogre::Math::IFloor(float(texIndex)/float(splatChannels));
	//int channelNr= Ogre::Math::IFloor(texIndex % splatChannels);
	//if(texIndex != -1)
	//{
	//	mSplatMgr->saveMapToImage(covmapNr,holesCovmap);
	//	/*Ogre::Real val = holesCovmap.getColourAt(x,z,0)[channelNr];
	//				
	//				if(val > cmpval)
	//					continue;*/
	//	Ogre::Real cmpval = 0;//.01;
	//	
	//	for(size_t x=0;x<lightmap.getWidth();x++)
	//		for(size_t z=0;z<lightmap.getHeight();z++)
	//		{
	//			Ogre::Real val = holesCovmap.getColourAt(x,z,0)[channelNr];
	//			if(val > cmpval)
	//			{
	//				lightmap.getData()[splatChannels*(z*lightmap.getWidth() + x)] = 255;
	//				lightmap.getData()[splatChannels*(z*lightmap.getWidth() + x)+1] = 255;
	//				lightmap.getData()[splatChannels*(z*lightmap.getWidth() + x)+2] = 255;
	//				//mLog("loch at "+ogre_str(x)+"/"+ogre_str(z));
	//			}
	//		}
	//}

	//// get our dynamic texture and update its contents
	////Ogre::TexturePtr tex = TextureManager::getSingleton().getByName("ETLightmap");
	//terrainLightmap->getBuffer(0, 0)->blitFromMemory(lightmap.getPixelBox(0, 0));
	
}

void Level::saveToFile(Ogre::String filename)
{
	//eventuell ist es eine gute idee, erstmal das archiv neuzubauen
    //zip vorbereiten
    
    ZipSaveFile zip(filename,true);
  
    //zip.removeFile();
	
//	StandardApplication::getSingletonPtr()->setDebugText("save to "+filename);
	//Ogre::String saveTo = levelPath+"/"+currentLevel->getFileName();
	TiXmlDocument doc;
	//now try to write all the stuff to it...
	
	TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );


	TiXmlElement * levelElement = new TiXmlElement( "level" );
	TiXmlElement * soundElement = new TiXmlElement( "sound" );
	TiXmlElement * objectElement = new TiXmlElement( "objects" );
	
	levelElement->SetAttribute("version",ogre_str(CUR_LEVEL_VERSION).c_str());
	levelElement->SetAttribute("last_sgid",ogre_str(lastSGID).c_str());

	/*
	switch(mLvlType)
	{
	case lvtPaginglandscape:
		levelElement->SetAttribute("type","paginglandscape");
		levelElement->SetAttribute("src",worldGeometryFile.c_str());		
		levelElement->SetAttribute("mapName",plsm2MapName.c_str());
		
		break;
	//default:
	//	//erstmal gar nix
	}
	*/

    

	TiXmlElement *xmlElem = NULL;
	if(mName != "")
	{
		xmlElem = new TiXmlElement( "name" );
		xmlElem->SetAttribute("value",mName.c_str());
		levelElement->LinkEndChild(xmlElem);

	}


	if(gravity != GRAVITY)
	{
		xmlElem = new TiXmlElement( "gravity" );
		xmlElem->SetAttribute("value",Ogre::StringConverter::toString(gravity).c_str());
		levelElement->LinkEndChild(xmlElem);

	}
	Ogre::ColourValue ambient = mSceneMgr->getAmbientLight();
	xmlElem = new TiXmlElement("ambientcolour");
	xmlElem->SetAttribute("value",Ogre::StringConverter::toString(ambient).c_str());
	levelElement->LinkEndChild(xmlElem);

	xmlElem = new TiXmlElement("backgroundcolour");
	xmlElem->SetAttribute("value",Ogre::StringConverter::toString(backgroundColour).c_str());
	levelElement->LinkEndChild(xmlElem);

	xmlElem = new TiXmlElement("north");
	xmlElem->SetAttribute("orientation",Ogre::StringConverter::toString(northOrientation).c_str());
	levelElement->LinkEndChild(xmlElem);

	if(mSceneMgr->isSkyBoxEnabled())
	{
		if(skyBoxMaterial != "")
		{
		
			xmlElem = new TiXmlElement("skybox");
			xmlElem->SetAttribute("material",skyBoxMaterial.c_str());
			levelElement->LinkEndChild(xmlElem);
			xmlElem->SetAttribute("distance",Ogre::StringConverter::toString(skyBoxParams.skyBoxDistance).c_str());	
		}
		
	}
	//Ogre::String skyBoxMaterial, skyDomeMaterial, skyPlaneMaterial;
	if(mSceneMgr->isSkyDomeEnabled())
	{
		/*mSceneMgr->getSkyBoxGenParameters().
		Ogre::String mat = mSceneMgr->getSkyDomeNode()->getMaterial()->getName();*/
		if(skyDomeMaterial != "")
		{
			xmlElem = new TiXmlElement("skydome");
			xmlElem->SetAttribute("material",skyDomeMaterial.c_str());
			//Ogre::SceneManager::SkyDomeGenParameters sdp = mSceneMgr->getSkyDomeGenParameters();
			xmlElem->SetAttribute("curvature",Ogre::StringConverter::toString(skyDomeParams.skyDomeCurvature).c_str());
			xmlElem->SetAttribute("tiling",Ogre::StringConverter::toString(skyDomeParams.skyDomeTiling).c_str());
			xmlElem->SetAttribute("xsegments",Ogre::StringConverter::toString(skyDomeParams.skyDomeXSegments).c_str());
			xmlElem->SetAttribute("ysegments",Ogre::StringConverter::toString(skyDomeParams.skyDomeYSegments).c_str());
			xmlElem->SetAttribute("distance",Ogre::StringConverter::toString(skyDomeParams.skyDomeDistance).c_str());	
			levelElement->LinkEndChild(xmlElem);
		}
	}
	if(mSceneMgr->isSkyPlaneEnabled())
	{
		//Ogre::String mat = mSceneMgr->getSkyPlaneNode()->getMaterial()->getName();
		if(skyPlaneMaterial != "")
		{
			xmlElem = new TiXmlElement("skyplane");
			xmlElem->SetAttribute("material",skyPlaneMaterial.c_str());
			//Ogre::SceneManager::SkyPlaneGenParameters spp = mSceneMgr->getSkyPlaneGenParameters();
			xmlElem->SetAttribute("bow",Ogre::StringConverter::toString(skyPlaneParams.skyPlaneBow).c_str());
			xmlElem->SetAttribute("scale",Ogre::StringConverter::toString(skyPlaneParams.skyPlaneScale).c_str());
			xmlElem->SetAttribute("tiling",Ogre::StringConverter::toString(skyPlaneParams.skyPlaneTiling).c_str());
			xmlElem->SetAttribute("xsegments",Ogre::StringConverter::toString(skyPlaneParams.skyPlaneXSegments).c_str());
			xmlElem->SetAttribute("ysegments",Ogre::StringConverter::toString(skyPlaneParams.skyPlaneYSegments).c_str());
			xmlElem->SetAttribute("distance",Ogre::StringConverter::toString(skyPlaneDist).c_str());			
			levelElement->LinkEndChild(xmlElem);
		}
	}

	
	
	if(mFogMode != Ogre::FOG_NONE)
	{
		xmlElem = new TiXmlElement("fog");
		switch(mFogMode)
		{
		case Ogre::FOG_LINEAR:
			xmlElem->SetAttribute("type","linear");
			break;
		case Ogre::FOG_EXP:
			xmlElem->SetAttribute("type","exp");
			break;
		case Ogre::FOG_EXP2:
			xmlElem->SetAttribute("type","exp2");
			break;
		}
		xmlElem->SetAttribute("colour",ogre_str(mFogColor).c_str());
		xmlElem->SetAttribute("expDensity",ogre_str(mFogDensity).c_str());
		xmlElem->SetAttribute("linearStart",ogre_str(mFogStart).c_str());
		xmlElem->SetAttribute("linearEnd",ogre_str(mFogEnd).c_str());
		levelElement->LinkEndChild(xmlElem);
	}

	
	xmlElem = new TiXmlElement("music");
	for(Ogre::StringVector::iterator itr = musicExplore.begin();itr!=musicExplore.end();itr++)
	{
		TiXmlElement *music = new TiXmlElement("explore");
		Ogre::String file = *itr;
		music->SetAttribute("file",file.c_str());
		xmlElem->LinkEndChild(music);
	}
	for(Ogre::StringVector::iterator itr = musicBattle.begin();itr!=musicBattle.end();itr++)
	{
		TiXmlElement *music = new TiXmlElement("battle");
		Ogre::String file = *itr;
		music->SetAttribute("file",file.c_str());
		xmlElem->LinkEndChild(music);
	}
	soundElement->LinkEndChild(xmlElem);

	xmlElem = new TiXmlElement("ambient");
	xmlElem->SetAttribute("wait_min",ogre_str(wait_min).c_str());
	xmlElem->SetAttribute("wait_max",ogre_str(wait_max).c_str());
	for(Ogre::StringVector::iterator itr = ambientRandom.begin();itr!=ambientRandom.end();itr++)
	{
		TiXmlElement *amb = new TiXmlElement("sound");
		Ogre::String file = *itr;
		amb->SetAttribute("file",file.c_str());
		xmlElem->LinkEndChild(amb);
	}
	soundElement->LinkEndChild(xmlElem);
	xmlElem = new TiXmlElement("ambientloop");
	for(Ogre::StringVector::iterator itr = ambientLoop.begin();itr!=ambientLoop.end();itr++)
	{
		TiXmlElement *amb = new TiXmlElement("sound");
		Ogre::String file = *itr;
		amb->SetAttribute("file",file.c_str());
		xmlElem->LinkEndChild(amb);
	}
	soundElement->LinkEndChild(xmlElem);
	

	
	for(ObjectList::iterator itr = mObjects.begin();
		itr != mObjects.end();itr++)
	{
		
		GameObject *obj= *itr;
		if(obj->getType() != otGroup)
		{
		
			TiXmlElement *objelem = obj->getXmlElem();//objectToXmlElem(obj);
			
			if(objelem)
			{
				objelem->SetAttribute("inlevelfile","true");
				objectElement->LinkEndChild(objelem);
			}

		}
		
		
	}

	

	//"special"
	/*for(std::map<Ogre::String, OrientedPoint>::iterator itr = entrances.begin();itr != entrances.end();itr++)
	{
		Ogre::String name = itr->first;
		OrientedPoint psor = itr->second;
		TiXmlElement *xmlElem = new TiXmlElement( "entrance" );
		xmlElem->SetAttribute("pos",Ogre::StringConverter::toString(psor.pos).c_str());
		xmlElem->SetAttribute("name",name.c_str());
		if(psor.orient != Ogre::Quaternion::IDENTITY)
		{
			xmlElem->SetAttribute("orient",Ogre::StringConverter::toString(psor.orient).c_str());
		}
		specialElement->LinkEndChild(xmlElem);
		
	}*/
	
	
	//level
	
	

	
	doc.LinkEndChild( decl );

	levelElement->LinkEndChild(soundElement);
	levelElement->LinkEndChild(objectElement);

	doc.LinkEndChild( levelElement );

	//doc.LinkEndChild( soundElement );
	//doc.LinkEndChild( objectElement );
	//und jetzt zippen

	//Ogre::String tesssst = test->Value();
    std::stringstream st;
	st << doc;
    zip.addTextFile(st.str(),"level.lvl");
    //zip.closeZipFile();
	//doc.SaveFile(filename.c_str());
	
}


void Level::saveToFileOld(Ogre::String filename)
{
	StandardApplication::getSingletonPtr()->setDebugText("save to "+filename);
	//Ogre::String saveTo = levelPath+"/"+currentLevel->getFileName();
	TiXmlDocument doc;
	//now try to write all the stuff to it...
	
	TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );


	TiXmlElement * levelElement = new TiXmlElement( "level" );
	TiXmlElement * specialElement = new TiXmlElement( "special" );
	TiXmlElement * staticElement = new TiXmlElement( "statics" );	
	TiXmlElement * itemElement = new TiXmlElement( "items" );
	TiXmlElement * lightElement = new TiXmlElement( "lights" );
	TiXmlElement * doorElement = new TiXmlElement( "doors" );
	TiXmlElement * entrElement = new TiXmlElement( "entrances" );
	TiXmlElement * charElement = new TiXmlElement( "characters" );
	TiXmlElement * containerElement = new TiXmlElement( "containers" );
	

/*	
	switch(mLvlType)
	{
	case lvtPaginglandscape:
		levelElement->SetAttribute("type","paginglandscape");
		levelElement->SetAttribute("src",worldGeometryFile.c_str());		
		levelElement->SetAttribute("mapName",plsm2MapName.c_str());
		
		break;
	//default:
	//	//erstmal gar nix
	}
	
*/
	
	for(ObjectList::iterator itr = mObjects.begin();
		itr != mObjects.end();itr++)
	{
		
		GameObject *obj= *itr;//->second;

		TiXmlElement *xmlElem = new TiXmlElement( obj->getID().c_str() );
		
		Ogre::Vector3 pos, scale;
		Ogre::Quaternion ornt;
		
		scale = obj->getScale();
		obj->getPositionOrientation(pos,ornt);
		
		xmlElem->SetAttribute("pos",Ogre::StringConverter::toString(pos).c_str());
		if(ornt != Ogre::Quaternion::IDENTITY)
		{
			xmlElem->SetAttribute("orient",Ogre::StringConverter::toString(ornt).c_str());
		}
		//scale = nod->getScale();
		
		switch(obj->getType())
		{
		
		case otItem:
			{
				//gamedata_item it = app->getItemData(obj->getID());
				if(scale != Ogre::Vector3::UNIT_SCALE)
				{
					xmlElem->SetAttribute("scale",Ogre::StringConverter::toString(scale).c_str());
				}
				itemElement->LinkEndChild( xmlElem );

			}			
			break;
		case otDynamic:
			{
				//gamedata_dynamic ob = app->getDynamicData(obj->getID());
				if(scale != Ogre::Vector3::UNIT_SCALE)
				{
					xmlElem->SetAttribute("scale",Ogre::StringConverter::toString(scale).c_str());
				}
				specialElement->LinkEndChild( xmlElem );

			}			
			break;
		case otChar:
			{
				//gamedata_char ch = app->getCharData(obj->getID());
				/*if(ch.scale != scale)
				{
					xmlElem->SetAttribute("scale",Ogre::StringConverter::toString(scale).c_str());
				}*/
				GameChar *c = static_cast<GameChar*>(obj);
				if(c->getInventory())
				{
					TiXmlElement *invnode = new TiXmlElement( "inventory" );
					app->inventoryToXml(invnode,c->getInventory());
					xmlElem->LinkEndChild(invnode);
				}

				charElement->LinkEndChild( xmlElem );
				
				//Ogre::LogManager::getSingletonPtr()->logMessage("speichere char="+obj->getID());

			}			
			break;
		case otDoor:
			{
				//gamedata_door ac = app->getDoorData(obj->getID());
				if(scale != Ogre::Vector3::UNIT_SCALE)
				{
					xmlElem->SetAttribute("scale",Ogre::StringConverter::toString(scale).c_str());
				}
				DoorData ad = static_cast<DoorObject*>(obj)->getDoorData();
				xmlElem->SetAttribute("targetLevel",ad.targetLevel.c_str());
				xmlElem->SetAttribute("targetEntrance",ad.targetEntrance.c_str());
				doorElement->LinkEndChild( xmlElem );

			}			
			break;
		case otLight:
			{
				LightObject *lo = (LightObject*)obj;
				Ogre::Light *light = lo->getLight();
				Ogre::String strtype = "point";
				
				switch(light->getType())
				{					
				case Ogre::Light::LT_POINT:
					strtype = "point";
					break;
				case Ogre::Light::LT_DIRECTIONAL:
					strtype = "directional";
					break;
				case Ogre::Light::LT_SPOTLIGHT:
					strtype = "spotlight";
					break;
				}
				xmlElem->SetValue(strtype.c_str());
				
				
				Ogre::ColourValue diffuse, specular;
				diffuse = light->getDiffuseColour();
				specular = light->getSpecularColour();
				xmlElem->SetAttribute("diffuse",Ogre::StringConverter::toString(diffuse).c_str());
				xmlElem->SetAttribute("specular",Ogre::StringConverter::toString(specular).c_str());
				
				Ogre::Real aRange  = light->getAttenuationRange();
				Ogre::Real aConst  = light->getAttenuationConstant();
				Ogre::Real aLinear = light->getAttenuationLinear();
				Ogre::Real aQuad   = light->getAttenuationQuadric();

				xmlElem->SetAttribute("aRange",Ogre::StringConverter::toString(aRange).c_str());
				xmlElem->SetAttribute("aConst",Ogre::StringConverter::toString(aConst).c_str());
				xmlElem->SetAttribute("aLinear",Ogre::StringConverter::toString(aLinear).c_str());
				xmlElem->SetAttribute("aQuad",Ogre::StringConverter::toString(aQuad).c_str());

				if(light->getType() == Ogre::Light::LT_SPOTLIGHT)
				{
					Ogre::Radian sInner = light->getSpotlightInnerAngle();
					Ogre::Radian sOuter = light->getSpotlightOuterAngle();
					Ogre::Real falloff  = light->getSpotlightFalloff();

					xmlElem->SetAttribute("sInner",Ogre::StringConverter::toString(sInner).c_str());
					xmlElem->SetAttribute("sOuter",Ogre::StringConverter::toString(sOuter).c_str());
					xmlElem->SetAttribute("falloff",Ogre::StringConverter::toString(falloff).c_str());


				}

			
				
				lightElement->LinkEndChild( xmlElem );
			}
			break;
#ifdef __editor
		case otEntrance:
			{
//				//mLog("speichere entrance");
				entranceObject *eo = (entranceObject*)obj;
				Ogre::String name = eo->getName();
				xmlElem->SetValue("entrance");
				xmlElem->SetAttribute("name",name.c_str());
				entrElement->LinkEndChild(xmlElem);
			}
			break;
#endif
		case otContainer:
			{
				//xmlElem->SetValue("dotScene");
				if(scale != Ogre::Vector3::UNIT_SCALE)
				{
					xmlElem->SetAttribute("scale",Ogre::StringConverter::toString(scale).c_str());
				}

				ContainerObject *c = static_cast<ContainerObject*>(obj);
				if(c->mInv)
				{
					TiXmlElement *invnode = new TiXmlElement( "inventory" );
					app->inventoryToXml(invnode,c->mInv);
					xmlElem->LinkEndChild(invnode);
				}
				
				containerElement->LinkEndChild(xmlElem);
			}
			break;
		case otStatic:
			{
//				!le compile preventer
				WorldArt *w = obj->getWorldArt();
				/*Ogre::String type;
				switch(w->type)
				{
				case WT_MESH:
					type = "mesh";
					break;
				case WT_DOTSCENE:
					type = "dotscene";
					break;
				case WT_BILLBOARD:
					type = "billboard";
					break;
				case WT_PLANE:
					type = "plane";
					break;
				}*/
				app->worldArtToXml(w->mData,xmlElem);//->SetValue(type.c_str());
				if(scale != Ogre::Vector3::UNIT_SCALE)
				{
					xmlElem->SetAttribute("scale",Ogre::StringConverter::toString(scale).c_str());
				}

				/*if(scale != Ogre::Vector3::UNIT_SCALE)
				{
					xmlElem->SetAttribute("scale",Ogre::StringConverter::toString(scale).c_str());
				}*/
				//Ogre::String fName = w->data_Filename;
				//Ogre::String matName = w->data_Material;//((StaticObject*)obj)->getMaterial();
				//if(matName != "")
				//	xmlElem->SetAttribute("material",matName.c_str());
				//if(fName != "")
				//	xmlElem->SetAttribute("filename",fName.c_str());
				/*xmlElem->SetAttribute("pos",Ogre::StringConverter::toString(pos).c_str());
				if(ornt != Ogre::Quaternion::IDENTITY)
				{
					xmlElem->SetAttribute("orient",Ogre::StringConverter::toString(ornt).c_str());
				}*/
				staticElement->LinkEndChild(xmlElem);
				

			}
			break;
			
		}

		/*std::map<Ogre::String, game_obj_static> objects;
std::map<Ogre::String, game_item> datamap_item;*/


		
		
	}

	

	//"special"
	/*for(std::map<Ogre::String, OrientedPoint>::iterator itr = entrances.begin();itr != entrances.end();itr++)
	{
		Ogre::String name = itr->first;
		OrientedPoint psor = itr->second;
		TiXmlElement *xmlElem = new TiXmlElement( "entrance" );
		xmlElem->SetAttribute("pos",Ogre::StringConverter::toString(psor.pos).c_str());
		xmlElem->SetAttribute("name",name.c_str());
		if(psor.orient != Ogre::Quaternion::IDENTITY)
		{
			xmlElem->SetAttribute("orient",Ogre::StringConverter::toString(psor.orient).c_str());
		}
		specialElement->LinkEndChild(xmlElem);
		
	}*/
	TiXmlElement *xmlElem = NULL;
	
	//level
	if(mName != "")
	{
		xmlElem = new TiXmlElement( "name" );
		xmlElem->SetAttribute("value",mName.c_str());
		levelElement->LinkEndChild(xmlElem);

	}


	if(gravity != GRAVITY)
	{
		xmlElem = new TiXmlElement( "gravity" );
		xmlElem->SetAttribute("value",Ogre::StringConverter::toString(gravity).c_str());
		levelElement->LinkEndChild(xmlElem);

	}
	Ogre::ColourValue ambient = mSceneMgr->getAmbientLight();
	xmlElem = new TiXmlElement("ambientcolour");
	xmlElem->SetAttribute("value",Ogre::StringConverter::toString(ambient).c_str());
	levelElement->LinkEndChild(xmlElem);

	if(mSceneMgr->isSkyBoxEnabled())
	{
		if(skyBoxMaterial != "")
		{
		
			xmlElem = new TiXmlElement("skybox");
			xmlElem->SetAttribute("material",skyBoxMaterial.c_str());
			levelElement->LinkEndChild(xmlElem);
		}
		
	}
	if(mSceneMgr->isSkyDomeEnabled())
	{
	//	Ogre::String mat = mSceneMgr->getSkyDomeNode()->getMaterial()->getName();
		if(skyDomeMaterial != "")
		{
			xmlElem = new TiXmlElement("skydome");
			xmlElem->SetAttribute("material",skyDomeMaterial.c_str());
			Ogre::SceneManager::SkyDomeGenParameters sdp = mSceneMgr->getSkyDomeGenParameters();
			xmlElem->SetAttribute("curvature",Ogre::StringConverter::toString(sdp.skyDomeCurvature).c_str());
			xmlElem->SetAttribute("tiling",Ogre::StringConverter::toString(sdp.skyDomeTiling).c_str());
			xmlElem->SetAttribute("xsegments",Ogre::StringConverter::toString(sdp.skyDomeXSegments).c_str());
			xmlElem->SetAttribute("ysegments",Ogre::StringConverter::toString(sdp.skyDomeYSegments).c_str());
			levelElement->LinkEndChild(xmlElem);
		}
	}
	if(mSceneMgr->isSkyPlaneEnabled())
	{
//	Ogre::String mat = mSceneMgr->getSkyPlaneNode()->getMaterial()->getName();
		if(skyPlaneMaterial != "")
		{
			xmlElem = new TiXmlElement("skyplane");
			xmlElem->SetAttribute("material",skyPlaneMaterial.c_str());
			Ogre::SceneManager::SkyPlaneGenParameters spp = mSceneMgr->getSkyPlaneGenParameters();
			xmlElem->SetAttribute("bow",Ogre::StringConverter::toString(spp.skyPlaneBow).c_str());
			xmlElem->SetAttribute("scale",Ogre::StringConverter::toString(spp.skyPlaneScale).c_str());
			xmlElem->SetAttribute("tiling",Ogre::StringConverter::toString(spp.skyPlaneTiling).c_str());
			xmlElem->SetAttribute("xsegments",Ogre::StringConverter::toString(spp.skyPlaneXSegments).c_str());
			xmlElem->SetAttribute("ysegments",Ogre::StringConverter::toString(spp.skyPlaneYSegments).c_str());
			levelElement->LinkEndChild(xmlElem);
		}
	}

	
	


	



	
	doc.LinkEndChild( decl );
	doc.LinkEndChild( levelElement );
	doc.LinkEndChild( staticElement );
	doc.LinkEndChild( itemElement );
	doc.LinkEndChild( entrElement );
	doc.LinkEndChild( lightElement );
	doc.LinkEndChild( doorElement );
	doc.LinkEndChild( charElement );
	doc.LinkEndChild( specialElement );
	doc.LinkEndChild( containerElement );
	
	doc.SaveFile(filename.c_str());
}

void Level::preloadMusic()
{
	//mLog("preloadMusic: "+ptrToString(this));
#ifndef __editor
	GameApp *app = static_cast<GameApp*>(StandardApplication::getSingletonPtr());
	SimpleSound::SoundManager* soundMgr = SimpleSound::SoundManager::getSingletonPtr();
	if(hasMusicExplore)
	{
		for(Ogre::StringVector::const_iterator itr = musicExplore.begin();itr!= musicExplore.end();itr++)
		{
			SimpleSound::Buffer *buff = getBuffer(*itr);//soundMgr->getBuffer(*itr);
			buff->setType(btLevel);
			musicExpPreloaded.push_back(buff);
		}
	}
	if(hasMusicBattle)
	{
		for(Ogre::StringVector::const_iterator itr = musicBattle.begin();itr!= musicBattle.end();itr++) 
		{
			SimpleSound::Buffer *buff = getBuffer(*itr);//soundMgr->getBuffer(*itr);
			buff->setType(btLevel);
			musicBatPreloaded.push_back(buff);
		}
	}
	if(hasAmbientLoop)
	{
		for(Ogre::StringVector::const_iterator itr = ambientLoop.begin();itr!= ambientLoop.end();itr++) 
		{
			SimpleSound::Buffer *buff = getBuffer(*itr);//soundMgr->getBuffer(*itr);
			buff->setType(btLevel);
			ambientLoopPreloaded.push_back(buff);
		}
	}
	if(hasAmbientRandom)
	{
		for(Ogre::StringVector::const_iterator itr = ambientRandom.begin();itr!= ambientRandom.end();itr++) 
		{
			SimpleSound::Buffer *buff = getBuffer(*itr);//soundMgr->getBuffer(*itr);
			buff->setType(btLevel);
			ambientRandomPreloaded.push_back(buff);
		}
	}
#endif
}

void Level::processAudioSettings(TiXmlElement *elem)
{
	if(!elem)
		return;
	
	//erstmal musik
	TiXmlElement *music = elem->FirstChildElement("music");
	if(music)
	{
		TiXmlElement *child = music->FirstChildElement();
		while(child)
		{
			if(Ogre::String(child->Value()) == "explore")
			{				
				musicExplore.push_back(child->Attribute("file"));
			}
			else if(Ogre::String(child->Value()) == "battle")
			{
				musicBattle.push_back(child->Attribute("file"));
			}
			child = child->NextSiblingElement();
		}
	}
	

	// die "last" indizes setzen. x.size() ist ein wert, den sie normal nie erreichen werden, ergo guter startwert
	lastExIndex = musicExplore.size()+1;
	lastBaIndex = musicBattle.size()+1;
	//jetzt das normale ambient
	TiXmlElement *ambient_r = elem->FirstChildElement("ambient");
	if(ambient_r)
	{
		
		if(ambient_r->Attribute("wait_min"))
			wait_min = Ogre::StringConverter::parseReal(ambient_r->Attribute("wait_min"));
		if(ambient_r->Attribute("wait_max"))
			wait_max = Ogre::StringConverter::parseReal(ambient_r->Attribute("wait_max"));
		if(wait_min >= wait_max)
		{
			wait_max = wait_min;
			timeAmbientWait = wait_min;
		}
		else
			timeAmbientWait = Ogre::Math::RangeRandom(wait_min,wait_max);
		TiXmlElement *ambElem = ambient_r->FirstChildElement();
		while(ambElem)
		{
			ambientRandom.push_back(ambElem->Attribute("file"));
			ambElem = ambElem->NextSiblingElement();
		}
	}
	//und jetzt das immer loopende 
	TiXmlElement *ambient_l = elem->FirstChildElement("ambientloop");
	if(ambient_l)
	{
		
		TiXmlElement *ambElem = ambient_l->FirstChildElement();
		while(ambElem)
		{
			ambientLoop.push_back(ambElem->Attribute("file"));
			ambElem = ambElem->NextSiblingElement();
		}
	}
	//die bools updaten
	if(musicExplore.size() > 0)
		hasMusicExplore = true;
	if(musicBattle.size() > 0)
		hasMusicBattle = true;
	if(ambientRandom.size() > 0)
		hasAmbientRandom = true;
	if(ambientLoop.size() > 0)
		hasAmbientLoop = true;
}

void Level::processLevelSettings(TiXmlElement *elem)
{
	
	/*TiXmlElement*/
	OgreXmlElement *XMLNode = static_cast<OgreXmlElement *>(elem->FirstChildElement());
	//ZUERST "type" holen!
	Ogre::String lvlType = "";
	if(elem->Attribute("type"))
	{
		lvlType = elem->Attribute("type");
	}
	
	//nebel muss vor setWorldGeometry gesetzt werden
	TiXmlElement *fogElem = elem->FirstChildElement("fog");
	mFogMode = Ogre::FOG_NONE;
	mFogColor = Ogre::ColourValue::White;
	mFogDensity = 0.001;
	mFogStart = 0.0;
	mFogEnd = 1.0;
	if(fogElem)
	{
		if( fogElem->Attribute("type") )
		{
			Ogre::String fType = fogElem->Attribute("type");
			Ogre::StringUtil::toLowerCase(fType);
			if(fType == "linear")
				mFogMode = Ogre::FOG_LINEAR;
			else if(fType == "exp")
				mFogMode = Ogre::FOG_EXP;
			else if(fType == "exp2")
				mFogMode = Ogre::FOG_EXP2;
		}
		if( fogElem->Attribute("colour") )
		{
			mFogColor = Ogre::StringConverter::parseColourValue( fogElem->Attribute("colour") );
		}
		if( fogElem->Attribute("expDensity") )
		{
			mFogDensity = Ogre::StringConverter::parseReal( fogElem->Attribute("expDensity") );
		}
		if( fogElem->Attribute("linearStart") )
		{
			mFogStart = Ogre::StringConverter::parseReal( fogElem->Attribute("linearStart") );
		}
		if( fogElem->Attribute("linearEnd") )
		{
			mFogEnd = Ogre::StringConverter::parseReal( fogElem->Attribute("linearEnd") );
		}
	}
	//mSceneMgr->setFog(Ogre::FOG_LINEAR, fadeColour, 0.0, 50, 500);
	//world, scene manager and stuff
	//später aus XML lesen
	// Create the Ogre::SceneManager, in this case a generic one
	//-----------------------------------------------------
	// 4 Create the Ogre::SceneManager
	//
	//		Ogre::ST_GENERIC = octree
	//		ST_EXTERIOR_CLOSE = simple terrain
	//		ST_EXTERIOR_FAR = nature terrain (depreciated)
	//		ST_EXTERIOR_REAL_FAR = paging landscape
	//		ST_INTERIOR = Quake3 BSP
	//-----------------------------------------------------
	
	//, "ExampleSMInstance");
	//mSceneMgr->setAmbientLight(Ogre::ColourValue(1, 1, 1));
	
//	mWorld->update(mWorldUpdate);
	/*lvlType = Ogre::StringUtil::toLowerCase(lvlType);*/

	mSceneMgr = app->getRoot()->createSceneManager(Ogre::ST_GENERIC);
	createNewtonWorld();
	enableFog(app->showFog);
	//mSceneMgr->setFog(fogMode,fogColor,fogExpDensity,fogLinearStart,fogLinearEnd);
	createCamera();
	//}
	

	
		


	////i = 0;
	while( XMLNode )
	{
		/*if(XMLNode->Value() == Ogre::String("paging"))
		{
			paging = Ogre::StringConverter::parseBool(XMLNode->Attribute("use"));
			if(XMLNode->Attribute("pagesize"))
			{
				pageSize = Ogre::StringConverter::parseReal(XMLNode->Attribute("pagesize"));
			}
			if(XMLNode->Attribute("offset"))
			{
				pageStartOffset = Ogre::StringConverter::parseVector3(XMLNode->Attribute("offset"));
			}

		}*/
		
		if(XMLNode->Value() == Ogre::String("ambientcolour"))
		{
			Ogre::ColourValue col = Ogre::StringConverter::parseColourValue(XMLNode->Attribute("value"));
			mSceneMgr->setAmbientLight(col);
		}
		else if(XMLNode->Value() == Ogre::String("backgroundcolour"))
		{
			backgroundColour = Ogre::StringConverter::parseColourValue(XMLNode->Attribute("value"));
			
			
		}
		else if(XMLNode->Value() == Ogre::String("north"))
		{
			if(XMLNode->Attribute("orientation"))
			{
				northOrientation = Ogre::StringConverter::parseQuaternion(XMLNode->Attribute("orientation"));
			}
			else if(XMLNode->Attribute("direction"))
			{
				Ogre::Vector3 northDir = Ogre::StringConverter::parseVector3(XMLNode->Attribute("direction"));
				northOrientation = Ogre::Vector3::NEGATIVE_UNIT_Z.getRotationTo(northDir);
			}		
		}
		else if(XMLNode->Value() == Ogre::String("gravity"))
		{
			gravity = Ogre::StringConverter::parseReal(XMLNode->Attribute("value"));
			if(gravity == 0)
			{
				gravity = -9.81;
			}
		}
		else if(XMLNode->Value() == Ogre::String("name"))
		{
			mName = XMLNode->Attribute("value");			
		}
		else if(XMLNode->Value() == Ogre::String("skybox"))
		{
			Ogre::String material = XMLNode->Attribute("material");
			Ogre::Real dist = XMLNode->AttributeAsReal("distance",5000);
			setSkyBox(material,Ogre::Quaternion::IDENTITY,dist);
			/*mSceneMgr->setSkyBox(true,material);
			skyBoxParams=mSceneMgr->getSkyBoxGenParameters();
			skyBoxMaterial = material;*/
		}
		else if(XMLNode->Value() == Ogre::String("skyplane"))
		{
			Ogre::String material = XMLNode->Attribute("material");
			Ogre::Real dist = XMLNode->AttributeAsReal("distance",3000);
			// material="Examples/CloudySky"  scale="1000" tiling="10" bow="0" xsegments="1" ysegments="1" /-->
			Ogre::Real scale = 1000;
			Ogre::Real tiling = 8;
			Ogre::Real bow = 0;
			int xseg = 16;
			int yseg = 16;
			if(XMLNode->Attribute("scale"))
			{
				scale = Ogre::StringConverter::parseReal(XMLNode->Attribute("scale"));
			}
			if(XMLNode->Attribute("tiling"))
			{
				tiling = Ogre::StringConverter::parseReal(XMLNode->Attribute("tiling"));
			}
			if(XMLNode->Attribute("xsegments"))
			{
				xseg = Ogre::StringConverter::parseInt(XMLNode->Attribute("xsegments"));
			}
			if(XMLNode->Attribute("ysegments"))
			{
				yseg = Ogre::StringConverter::parseInt(XMLNode->Attribute("ysegments"));
			}
			if(XMLNode->Attribute("bow"))
			{
				bow = Ogre::StringConverter::parseReal(XMLNode->Attribute("bow"));
			}
			setSkyPlane(material,scale,tiling,bow,xseg,yseg,dist);
			/*Ogre::Plane plane;
			plane.d = 1000;
			plane.normal = Ogre::Vector3::NEGATIVE_UNIT_Y;
			mSceneMgr->setSkyPlane(true,plane,material,scale,tiling,true,bow,xseg,yseg);
			skyPlaneParams=mSceneMgr->getSkyPlaneGenParameters();
			skyPlaneMaterial = material;*/
		}
		else if(XMLNode->Value() == Ogre::String("skydome"))
		{
			Ogre::String material = XMLNode->Attribute("material");
			Ogre::Real dist = XMLNode->AttributeAsReal("distance",4000);
			// curvature="20" tiling="8" xsegments="16" ysegments="16" />
			Ogre::Real curvature = 20;
			Ogre::Real tiling = 8;
			int xseg = 16;
			int yseg = 16;
			if(XMLNode->Attribute("curvature"))
			{
				curvature = Ogre::StringConverter::parseReal(XMLNode->Attribute("curvature"));
			}
			if(XMLNode->Attribute("tiling"))
			{
				tiling = Ogre::StringConverter::parseReal(XMLNode->Attribute("tiling"));
			}
			if(XMLNode->Attribute("xsegments"))
			{
				xseg = Ogre::StringConverter::parseInt(XMLNode->Attribute("xsegments"));
			}
			if(XMLNode->Attribute("ysegments"))
			{
				yseg = Ogre::StringConverter::parseInt(XMLNode->Attribute("ysegments"));
			}

			setSkyDome(material,curvature,tiling,xseg,yseg,Ogre::Quaternion::IDENTITY,dist);
			/*mSceneMgr->setSkyDome(true,material,curvature,tiling,4000,true,Ogre::Quaternion::IDENTITY,xseg,yseg);
			skyDomeParams=mSceneMgr->getSkyDomeGenParameters();
			skyDomeMaterial = material;*/
		}
		//else if(XMLNode->Value() == Ogre::String("dotScene"))
		//{
		//	loadDotScene(XMLNode);
		//	/*Ogre::String scenefile = XMLNode->Attribute("src");
		//	Ogre::Vector3 pos = Ogre::Vector3::ZERO;
		//	Ogre::Vector3 scale = Ogre::Vector3::UNIT_SCALE;
		//	Ogre::Quaternion orient = Ogre::Quaternion::IDENTITY;

		//	if(XMLNode->Attribute("pos"))
		//	{
		//		pos = Ogre::StringConverter::parseVector3(XMLNode->Attribute("pos"));
		//		if(pos == Ogre::Vector3::ZERO)
		//		{
		//			pos = Ogre::StringConverter::parseReal(XMLNode->Attribute("pos"));
		//		}
		//	}
		//	if(XMLNode->Attribute("scale"))
		//	{
		//		scale = Ogre::StringConverter::parseVector3(XMLNode->Attribute("scale"));
		//		if(scale == Ogre::Vector3::ZERO)
		//		{
		//			scale = Ogre::StringConverter::parseReal(XMLNode->Attribute("scale"));
		//		}
		//	}
		//	if(XMLNode->Attribute("orient"))
		//	{
		//		orient = Ogre::StringConverter::parseQuaternion(XMLNode->Attribute("orient"));
		//	}
		//	


		//	dotSceneObject *obj = new dotSceneObject(scenefile,this,pos,orient,scale);
		//	ObjectMap[obj->getSceneNode()] = static_cast<GameObject*>(obj);*/

		//}
		/*else if(XMLNode->Value() == Ogre::String("skybox"))
		{
			Ogre::String material = XMLNode->Attribute("material");
			mSceneMgr->setSkyBox(true,material);
		}*/
		

		XMLNode = XMLNode->NextSiblingElement();
	}
	app->getRenderWindow()->getViewport(0)->setBackgroundColour(backgroundColour);
}

void Level::createNewtonWorld()
{
	mWorld = new OgreNewt::World();
	mWorld->setUpdateFPS(60,5);
	mWorld->setWorldSize(Ogre::Vector3(-1000,-1000,-1000),Ogre::Vector3(1000,1000,1000));
#ifdef _DEBUG
	OgreNewt::Debugger *dbg = &mWorld->getDebugger();
	dbg->init(mSceneMgr);
#endif
}

void Level::processStatics(TiXmlElement *elem)
{
	TiXmlElement *XMLNode = elem->FirstChildElement();


	//int i = 0;
	while( XMLNode )
	{
		
		Ogre::String type = XMLNode->Value();
		Ogre::StringUtil::toLowerCase(type);
		Ogre::Vector3 pos = Ogre::StringConverter::parseVector3(XMLNode->Attribute("pos"));
		Ogre::Vector3 scale = Ogre::Vector3::UNIT_SCALE;
		Ogre::Quaternion ornt = Ogre::Quaternion::IDENTITY;
		/*Ogre::String filename = XMLNode->Attribute("filename");
		Ogre::String material = "";*/
		bool hasCollision = true;
		

		
		
		if(XMLNode->Attribute("orient"))
		{
			ornt = Ogre::StringConverter::parseQuaternion(XMLNode->Attribute("orient"));
		}
		
		/*if(XMLNode->Attribute("scale"))
		{
			scale = Ogre::StringConverter::parseVector3(XMLNode->Attribute("scale"));
		}*/
		if(XMLNode->Attribute("collision"))
		{
			hasCollision = Ogre::StringConverter::parseBool(XMLNode->Attribute("collision"));
		}
		/*
		if(XMLNode->Attribute("material"))
		{
			material = XMLNode->Attribute("material");
		}*/
		
		WorldArtData wd = app->parseWorldArtData(XMLNode);
		try
		{			
			StaticObject *obj = new StaticObject(this,wd,pos,ornt,scale,hasCollision);	
			
		}
		catch(ExceptionType)
		{
			mLog("konnte static nicht erstellen!");
		}
		

		



		XMLNode = XMLNode->NextSiblingElement();

	}//----------------ENDE von if(XMLNode)

}
void Level::processSpecial(TiXmlElement *elem)
{
		
		TiXmlElement *XMLNode = elem->FirstChildElement();


		//int i = 0;
		while( XMLNode )
		{
			
			Ogre::String ID = XMLNode->Value();
			Ogre::Vector3 pos = Ogre::StringConverter::parseVector3(XMLNode->Attribute("pos"));
			Ogre::Vector3 scale = Ogre::Vector3::UNIT_SCALE;
			Ogre::Quaternion ornt = Ogre::Quaternion::IDENTITY;
			

			if(XMLNode->Attribute("rotVector"))
			{
				
				ornt = Ogre::Vector3::NEGATIVE_UNIT_Z.getRotationTo(Ogre::StringConverter::parseVector3(XMLNode->Attribute("rotVector")));
			}
			
			if(XMLNode->Attribute("orient"))
			{
				ornt = Ogre::StringConverter::parseQuaternion(XMLNode->Attribute("orient"));
			}
			
			if(XMLNode->Attribute("scale"))
			{
				scale = Ogre::StringConverter::parseVector3(XMLNode->Attribute("scale"));
			}
			try
			{
				new DynamicObject(ID,this,pos,ornt,scale);				
			}
			catch(ExceptionType)
			{
				mLog("konnte special "+ID+" nicht erstellen!");
			}
			//placeObject(otDynamic,ID,pos,ornt,scale);
			//new DynamicObject(ID,this,pos,ornt,scale);



			XMLNode = XMLNode->NextSiblingElement();

		}//----------------ENDE von if(XMLNode)
}
void Level::processDoors(TiXmlElement *elem)
{
	TiXmlElement *XMLNode = elem->FirstChildElement();


	
	while( XMLNode )
	{
		
		Ogre::String ID = XMLNode->Value();
		Ogre::Vector3 pos = Ogre::StringConverter::parseVector3(XMLNode->Attribute("pos"));
		Ogre::Vector3 scale = Ogre::Vector3::UNIT_SCALE;
		Ogre::Quaternion ornt = Ogre::Quaternion::IDENTITY;
		DoorData mData;
		mData.targetLevel = "";
		mData.targetEntrance = "";

		

		if(XMLNode->Attribute("rotVector"))
		{
			
			ornt = Ogre::Vector3::NEGATIVE_UNIT_Z.getRotationTo(Ogre::StringConverter::parseVector3(XMLNode->Attribute("rotVector")));
		}
		
		if(XMLNode->Attribute("orient"))
		{
			ornt = Ogre::StringConverter::parseQuaternion(XMLNode->Attribute("orient"));
		}

		if(XMLNode->Attribute("targetEntrance"))
		{
			mData.targetEntrance = XMLNode->Attribute("targetEntrance");
		}
		
		if(XMLNode->Attribute("targetLevel"))
		{
			mData.targetLevel = XMLNode->Attribute("targetLevel");
		}

		if(XMLNode->Attribute("scale"))
		{
			scale = Ogre::StringConverter::parseVector3(XMLNode->Attribute("scale"));
		}
		
		//DoorObject *obj = (DoorObject*)placeObject(otDoor,ID,pos,ornt,scale);
		//DoorObject *obj = new DoorObject(ID,this,pos,ornt,scale);

		try
		{
			DoorObject *obj = new DoorObject(ID,this,pos,ornt,scale);
			obj->setDoorData(mData);
		}
		catch(ExceptionType)
		{
			////app->log("Error "+Ogre::StringConverter::toString(errortype)+": could not create door \""+objectID+"\"");
			mLog("konnte Door nicht erstellen");
		}

		



		XMLNode = XMLNode->NextSiblingElement();

	}//----------------ENDE von if(XMLNode)
}
void Level::processItems(TiXmlElement *elem)
{
	TiXmlElement *XMLNode = elem->FirstChildElement();


	////i = 0;
	while( XMLNode )
	{
		Ogre::String ID = XMLNode->Value();
		Ogre::Vector3 pos = Ogre::StringConverter::parseVector3(XMLNode->Attribute("pos"));
		Ogre::Vector3 scale = Ogre::Vector3::UNIT_SCALE;
		Ogre::Quaternion ornt = Ogre::Quaternion::IDENTITY;
		

		if(XMLNode->Attribute("orient"))
		{
			ornt = Ogre::StringConverter::parseQuaternion(XMLNode->Attribute("orient"));
		}
		if(XMLNode->Attribute("scale"))
		{
			scale = Ogre::StringConverter::parseVector3(XMLNode->Attribute("scale"));
		}

		try
		{
			new GameItem(ID,this,pos,ornt,scale);
			
		}
		catch(ExceptionType)
		{
			////app->log("Error "+Ogre::StringConverter::toString(errortype)+": could not create item \""+objectID+"\"");
			mLog("konnte Item "+ID+"nicht erstellen!");
		}

		//placeObject(otItem,ID,pos,ornt,scale);
		//new GameItem(ID,this,pos,ornt,scale);

		XMLNode = XMLNode->NextSiblingElement();
	}
}
void Level::processContainers(TiXmlElement *elem)
{
	TiXmlElement *XMLNode = elem->FirstChildElement();


	////i = 0;
	while( XMLNode )
	{
		Ogre::String ID = XMLNode->Value();
		Ogre::Vector3 pos = Ogre::StringConverter::parseVector3(XMLNode->Attribute("pos"));
		Ogre::Vector3 scale = Ogre::Vector3::UNIT_SCALE;
		Ogre::Quaternion ornt = Ogre::Quaternion::IDENTITY;
		

		if(XMLNode->Attribute("orient"))
		{
			ornt = Ogre::StringConverter::parseQuaternion(XMLNode->Attribute("orient"));
		}
		if(XMLNode->Attribute("scale"))
		{
			scale = Ogre::StringConverter::parseVector3(XMLNode->Attribute("scale"));
		}
		//Ogre::LogManager::getSingletonPtr()->logMessage("Erstelle char: id="+ID);
		try
		{			
			ContainerObject *cur = new ContainerObject(ID,this,pos,ornt,scale);
			if(XMLNode->FirstChildElement("inventory"))
			{
				app->parseInventory(XMLNode->FirstChildElement("inventory"),cur->mInv);
				//cur->mInv
			}
			

		}
		catch(ExceptionType)
		{
			mLog("Could not create container "+ID);
		}
		//placeObject(otChar,ID,pos,ornt,scale);

		XMLNode = XMLNode->NextSiblingElement();
	}
		
}

void Level::processCharacters(TiXmlElement *elem)
{
	TiXmlElement *XMLNode = elem->FirstChildElement();


	////i = 0;
	while( XMLNode )
	{
		Ogre::String ID = XMLNode->Value();
		Ogre::Vector3 pos = Ogre::StringConverter::parseVector3(XMLNode->Attribute("pos"));
		Ogre::Vector3 scale = Ogre::Vector3::UNIT_SCALE;
		Ogre::Quaternion ornt = Ogre::Quaternion::IDENTITY;
		

		if(XMLNode->Attribute("orient"))
		{
			ornt = Ogre::StringConverter::parseQuaternion(XMLNode->Attribute("orient"));
		}
		if(XMLNode->Attribute("scale"))
		{
			scale = Ogre::StringConverter::parseVector3(XMLNode->Attribute("scale"));
		}
		//Ogre::LogManager::getSingletonPtr()->logMessage("Erstelle char: id="+ID);
		try
		{			
			new GameChar(ID,this,pos,ornt);			
		}
		catch(ExceptionType)
		{
			mLog("Could not create character "+ID);
		}
		//placeObject(otChar,ID,pos,ornt,scale);

		XMLNode = XMLNode->NextSiblingElement();
	}
		
}

void Level::processLights(TiXmlElement *elem)
{
	TiXmlElement *XMLNode = elem->FirstChildElement();
		
	while( XMLNode )
	{
		Ogre::ColourValue diffuse = Ogre::ColourValue::White;
		Ogre::ColourValue specular = Ogre::ColourValue::White;
		Ogre::Quaternion orient = Ogre::Quaternion::IDENTITY;
		Ogre::Light::LightTypes type = Ogre::Light::LT_POINT;
		

		
		
		//Ogre::LogManager::getSingleton().logMessage(Ogre::String("in lights, Value=")+XMLNode->Value());
		Ogre::String strtype = XMLNode->Value();
		if(strtype == Ogre::String("point"))
		{			
			//dann ist ok	
		}
		else if(strtype == "directional")
		{
			type = Ogre::Light::LT_DIRECTIONAL;
		}
		else if(strtype == "spotlight")
		{
			type = Ogre::Light::LT_SPOTLIGHT;
		}
		else
		{
			//zomg
			mLog("Licht mit "+strtype+" als Typ gefunden. wird als \"point\" angesehen");
			
		}
		//hier brauchen wir position
		if(XMLNode->Attribute("pos"))
		{
			
			Ogre::Vector3 pos = Ogre::StringConverter::parseVector3(XMLNode->Attribute("pos"));
			//Ogre::LogManager::getSingleton().logMessage(Ogre::String("in lights, pos="+Ogre::StringConverter::toString(pos)));
			if(XMLNode->Attribute("diffuse"))
				diffuse = Ogre::StringConverter::parseColourValue(XMLNode->Attribute("diffuse"));
			if(XMLNode->Attribute("specular"))
				specular = Ogre::StringConverter::parseColourValue(XMLNode->Attribute("specular"));

			if(XMLNode->Attribute("orient"))
				orient = Ogre::StringConverter::parseQuaternion(XMLNode->Attribute("orient"));
		
			try
			{
				LightObject *lo = new LightObject(this,pos,orient);

				Ogre::Light *l = lo->getLight();
				lo->setLightType(type);
				l->setDiffuseColour(diffuse);
				l->setSpecularColour(specular);
				

				Ogre::Real aRange  = 100000;
				Ogre::Real aConst  = 1;
				Ogre::Real aLinear = 0;
				Ogre::Real aQuad   = 0;

				if(XMLNode->Attribute("aRange"))
				{
					aRange = Ogre::StringConverter::parseReal(XMLNode->Attribute("aRange"));				
				}
				if(XMLNode->Attribute("aConst"))
				{
					aConst = Ogre::StringConverter::parseReal(XMLNode->Attribute("aConst"));				
				}
				if(XMLNode->Attribute("aLinear"))
				{
					aLinear = Ogre::StringConverter::parseReal(XMLNode->Attribute("aLinear"));				
				}
				if(XMLNode->Attribute("aQuad"))
				{
					aQuad = Ogre::StringConverter::parseReal(XMLNode->Attribute("aQuad"));				
				}
				l->setAttenuation(aRange,aConst,aLinear,aQuad);

				if(XMLNode->Attribute("powerScale"))
				{
					Ogre::Real powerScale = Ogre::StringConverter::parseReal(XMLNode->Attribute("powerScale"));
					l->setPowerScale(powerScale);
				}
				if(type == Ogre::Light::LT_SPOTLIGHT)
				{
					//spotlight params
					if(XMLNode->Attribute("sInner"))
					{
						Ogre::Radian sInner = Ogre::StringConverter::parseAngle(XMLNode->Attribute("sInner"));
						l->setSpotlightInnerAngle(sInner);
					}
					if(XMLNode->Attribute("sOuter"))
					{
						Ogre::Radian sOuter = Ogre::StringConverter::parseAngle(XMLNode->Attribute("sOuter"));
						l->setSpotlightOuterAngle(sOuter);
					}
					if(XMLNode->Attribute("falloff"))
					{
						Ogre::Real falloff = Ogre::StringConverter::parseReal(XMLNode->Attribute("falloff"));
						l->setSpotlightFalloff(falloff);
					}
						
				}	
#ifdef __editor
				lo->showMesh();
#endif
				
			}
			catch(ExceptionType)
			{
				mLog("Could not create Ogre::Light");
				
			}			
			//LightObject *lo = (LightObject*)placeObject(otLight,"",pos,orient);//new LightObject(this,pos,orient);
			
					
			//Ogre::Light *light = lo->getLight();//mSceneMgr->createLight("LevelLight"+Ogre::StringConverter::toString(i++));
			//light->setPosition(pos);
			/*light->setDiffuseColour(diffuse);
			light->setSpecularColour(specular);*/
			
			//Ogre::SceneNode *node = lo->getSceneNode();
			//ObjectMap[node] = static_cast<GameObject*>(lo);

			

		}
		

		XMLNode = XMLNode->NextSiblingElement();
	}
}
void Level::processEntrances(TiXmlElement *elem)
{
	TiXmlElement *XMLNode = elem->FirstChildElement();


	////i = 0;
	while( XMLNode )
	{
		Ogre::String ID = XMLNode->Value();
		
		Ogre::Vector3 pos = Ogre::StringConverter::parseVector3(XMLNode->Attribute("pos"));
		Ogre::Vector3 scale = Ogre::Vector3::UNIT_SCALE;
		Ogre::Quaternion ornt = Ogre::Quaternion::IDENTITY;	

		if(XMLNode->Attribute("orient"))
		{
			ornt = Ogre::StringConverter::parseQuaternion(XMLNode->Attribute("orient"));
		}
		
		if(ID == "entrance")
		{
			Ogre::String name = XMLNode->Attribute("name");
			
#ifdef __editor
			//entranceObject erstellen
			try
			{
				if(!isEntranceNameUnique(name))
				{
					mLog("entrName "+name+" nicht einzigartig! wird übersprungen");
					//continue;
				}
				else
				{
					new entranceObject(name,this,pos,ornt);				
				}
			}
			catch(ExceptionType)
			{
				mLog("konnte entrance "+name+" nicht erstellen");
				
			}
			//placeObject(otEntrance,name,pos,ornt);
			//new entranceObject(name,this,pos,ornt);
#else


			
			OrientedPoint entr;
			entr.orient = ornt;
			entr.pos = pos;
			entrances[name] = entr; 
#endif
		}

		XMLNode = XMLNode->NextSiblingElement();
	}
}

Ogre::String Level::getUniqueEntranceName(Ogre::String base)
{
	
	int i = 0;
	while(entrances.find(base+ogre_str(i)) != entrances.end())
	{
		i++;
	}
	return base+ogre_str(i);
	
}

bool Level::isEntranceNameUnique(Ogre::String name)
{
	return (entrances.find(name) == entrances.end());
}

void Level::setSkyBox(Ogre::String material, Ogre::Quaternion orientation, Ogre::Real distance)
{
	mSceneMgr->setSkyBox(true,material,distance,true,orientation);
	skyBoxMaterial = material;
	skyBoxParams=mSceneMgr->getSkyBoxGenParameters();
}
void Level::removeSkyBox()
{
	mSceneMgr->setSkyBox(false,"");
	//skyBoxMaterial = "";
}
void Level::setSkyDome(Ogre::String material, Ogre::Real curvature, Ogre::Real tiling, int xseg,int yseg, Ogre::Quaternion orientation, Ogre::Real distance)
{
	mSceneMgr->setSkyDome(true,material,curvature,tiling,distance,true,orientation,xseg,yseg);
	skyDomeMaterial = material;
	skyDomeParams   = mSceneMgr->getSkyDomeGenParameters();

}
void Level::removeSkyDome()
{
	mSceneMgr->setSkyDome(false,"");
	//skyDomeMaterial = "";
}
void Level::setSkyPlane(Ogre::String material, Ogre::Real scale, Ogre::Real tiling, Ogre::Real bow, int xseg,int yseg,Ogre::Real distance)
{
	Ogre::Plane plane;
	plane.d = distance;
	plane.normal = Ogre::Vector3::NEGATIVE_UNIT_Y;
	mSceneMgr->setSkyPlane(true,plane,material,scale,tiling,true,bow,xseg,yseg);
	skyPlaneParams=mSceneMgr->getSkyPlaneGenParameters();
	skyPlaneMaterial = material;
	skyPlaneDist = distance;
}
void Level::removeSkyPlane()
{
	Ogre::Plane plane;
	plane.d = 1;
	plane.normal = Ogre::Vector3::NEGATIVE_UNIT_Y;
	mSceneMgr->setSkyPlane(false,plane,"");
	//skyPlaneMaterial="";
}

void Level::loadDotScene(TiXmlElement *elem)
{
	////test: editor-dotscene-aufspaltung


	//dotSceneFileName = elem->Attribute("src");


	//if(elem->Attribute("pos"))
	//{
	//	dotScenePos = Ogre::StringConverter::parseVector3(elem->Attribute("pos"));
	//	if(dotScenePos == Ogre::Vector3::ZERO)
	//	{
	//		dotScenePos = Ogre::StringConverter::parseReal(elem->Attribute("pos"));
	//	}
	//}
	//if(elem->Attribute("scale"))
	//{
	//	dotSceneScale = Ogre::StringConverter::parseVector3(elem->Attribute("scale"));
	//	if(dotSceneScale == Ogre::Vector3::ZERO)
	//	{
	//		dotSceneScale = Ogre::StringConverter::parseReal(elem->Attribute("scale"));
	//	}
	//}
	//if(elem->Attribute("orient"))
	//{
	//	dotSceneOrient = Ogre::StringConverter::parseQuaternion(elem->Attribute("orient"));
	//}

	////if editor dann
	////TiXmlDocument   *XMLDoc = 0;
	////TiXmlElement   *XMLRoot;
	////
	////Ogre::String m_sGroupName = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;
	////try
	////{
	////	Ogre::DataStreamPtr pStream = Ogre::ResourceGroupManager::getSingleton().
	////		openResource( dotSceneFileName,m_sGroupName );

	////	Ogre::String data = pStream->getAsString();
	////	// Open the .scene File
	////	XMLDoc = new TiXmlDocument();
	////	XMLDoc->Parse( data.c_str() );
	////	pStream->close();
	////	pStream.setNull();

	////	if( XMLDoc->Error() )
	////	{
	////		//We'll just log, and continue on gracefully
	////		Ogre::LogManager::getSingleton().logMessage("[dotSceneObject] The TiXmlDocument reported an error");
	////		delete XMLDoc;
	////		//return;
	////		throw etInvalidId;
	////	}
	////}
	////catch(...)
	////{
	////	//We'll just log, and continue on gracefully
	////	Ogre::LogManager::getSingleton().logMessage("[dotSceneObject] Error creating TiXmlDocument");
	////	delete XMLDoc;
	////	//return;
	////	throw etInvalidId;
	////}
	////// Validate the File
	////XMLRoot = XMLDoc->RootElement();
	////if( Ogre::String( XMLRoot->Value()) != "scene"  ) {
	////	Ogre::LogManager::getSingleton().logMessage( "[dotSceneObject] Error: Invalid .scene File. Missing <scene>" );
	////	delete XMLDoc;      
	////	//return;
	////	throw etInvalidId;
	////}
	////TiXmlElement *pElement;

	////// Process nodes (?)
	////pElement = XMLRoot->FirstChildElement("nodes");
	////pElement = pElement->FirstChildElement("node");
	////while(pElement)
	////{
	////	Ogre::Vector3 childPos;
	////	Ogre::Vector3 childScale;
	////	Ogre::Quaternion childOrient;
	////	new dotSceneObject(pElement,this,childPos,childOrient,childScale);
	////	
	////	pElement = pElement->NextSiblingElement("node");
	////}
	////
	////endif editor
	//

	//dotSceneObject *obj = new dotSceneObject(dotSceneFileName,this,dotScenePos,dotSceneOrient,dotSceneScale);
	////ObjectMap[obj->getSceneNode()] = static_cast<GameObject*>(obj);
}

void Level::saveDotScene()
{
}

void Level::registerObject(GameObject *obj)
{
	mLog("REGISTER OBJECT "+obj->getDebugInfo());
	mObjects.push_back(obj);
	if(objectListenerUse)
	{
		objectListenerList.push_back(obj);
	}

	processSGID(obj);

//#ifdef __editor
//	mObjects.push_back(obj);
//#else
//		if(isConstructing)
//			
//		else
//			objectsToRegister.push_back(obj);
//#endif
}

void Level::unregisterObject(GameObject *obj)
{
	if(destructorCalled)
		return;
	//DEBUG
	mLog("UNREGISTER OBJECT "+obj->getDebugInfo());
	ObjectList::iterator itr;
	//omfg... durchgehen und suchen
	for(itr = mObjects.begin();itr != mObjects.end();itr++)
	{
		GameObject *cur = *itr;
		
		if(cur == obj)
		{
			mObjects.erase(itr);
			break;
		}
	}
	if(!obj->isStatic())
	{
		if(obj->isInLevelFile)
		{
			saveableObjects[obj->getSGID()] = NULL;
		}
		else
		{
			for(SaveableObjectList::iterator itr = saveableObjects.begin();itr!=saveableObjects.end();itr++)
			{
				if(itr->second == obj)
				{
					saveableObjects.erase(itr);
					break;
				}
			}
		}
	}
	
}

void Level::LevelForceCallback( OgreNewt::Body* bod )
{
    Ogre::Vector3 intertia;
    Ogre::Real mass;
    bod->getMassMatrix(mass,intertia);
    
	bod->addForce(Ogre::Vector3(0,gravity*mass,0));
    //jetzt auftriebskraft
	GameObject *obj = Ogre::any_cast<GameObject *>(bod->getUserData());
    
    if(obj)
    {
        if(obj->isInWater())
        {
            //mDebugText("adding buoyancy");
            bod->addBouyancyForce<GameObject>( 0.7, 0.5, 0.5, Ogre::Vector3(0.0f,gravity,0.0f), &GameObject::buoyancyCallback, obj );
            //obj->onWaterSurfaceState = 0;
        }
    }
}

//void Level::tileLoaded(PagingLandscapeEvent* e)
//{
//
//
//
//	//return;
//	//if(pgLoaded)
//	//	return;
//	//pgLoaded = true;
//	///***CODE ZUM IMPORTIEREN
//	//unsigned int page_x = e->mPagex;
//	//unsigned int page_z = e->mPagez;
//	//unsigned int tile_x = e->mTilex;
//	//unsigned int tile_z = e->mTilez;
//
//	////PagingLandScapeSceneManager *plsmMgr = static_cast<PagingLandScapeSceneManager*>(mSceneMgr);
//	////PagingLandScapeOptions *opt = plsmMgr->getOptions();
//	////unsigned int TileSize = opt->TileSize;
//	////unsigned int PageSize = opt->PageSize;
//
//	////unsigned int height = opt->world_height;
//	////unsigned int width  = opt->world_width;
//	//				
//	////unsigned int NumTiles = opt->NumTiles;
//	////unsigned int NumPages = opt->NumPages;
//
//	////mLog("tileLoaded: x="+ogre_str(page_x)+", Z="+ogre_str(page_z));
//	////Ogre::ResourceManager()::getsing
//	////Ogre::String filename = "plsm2_"+worldGeometryFile+"_"+ogre_str(page_x)+"_"+ogre_str(page_z)+"_"+ogre_str(tile_x)+"_"+ogre_str(tile_z)+".collision";
//	//
//	//
//	//Ogre::ResourceGroupManager *rgm = Ogre::ResourceGroupManager::getSingletonPtr();
//	//TreeCollisionSerializer* serializer=new TreeCollisionSerializer();
//	//
//	//Ogre::String filename = "plsm2_"+worldGeometryFile+"_"+ogre_str(page_x)+"_"+ogre_str(page_z)+"_"+ogre_str(tile_x)+"_"+ogre_str(tile_z)+".collision";
//	//if(rgm->resourceExists("Collision",filename))
//	//{
//	//	//ok, col kann geladen werden
//	//	Ogre::DataStreamPtr ptr = rgm->openResource(filename,"Collision");
//	//	CollisionPrimitives::TreeCollision *collision=new CollisionPrimitives::TreeCollision(mWorld);
//	//	//FILE* file=fopen("SomeFileName.collision","rb");
//	//	//Ogre::DataStreamPtr ptr(new FileHandleDataStream(file));
//	//	serializer->importTreeCollision(ptr,collision);
//	//	OgreNewt::Body *bod = new OgreNewt::Body(mWorld,collision);
//	//}
//
//	//delete serializer;
//
//	//*/
//	//
//	//
//
//	////recover PLSM2's tile object
//	//size_t pageX=e->mPagex;
//	//size_t pageZ=e->mPagez;
//	//size_t tileX=e->mTilex;
//	//size_t tileZ=e->mTilez;
//	//
//
//	//mLog("tileLoaded:\npageX="+ogre_str(pageX)+
//	//				"\npageZ="+ogre_str(pageZ)+
//	//				"\ntileX="+ogre_str(tileX)+
//	//				"\ntileZ="+ogre_str(tileZ));
//
//
//	////ask reference to geometry to PLSM2
//	//std::vector<void*> params;
//	//int renderLevel=0;
//	//params.push_back(&pageX);
//	//params.push_back(&pageZ);
//	//params.push_back(&tileX);
//	//params.push_back(&tileZ);
//	//params.push_back(&renderLevel);
//	//mSceneMgr->getOption("PageGetTileVertexData_2",&params);
//
//	////recover data at the end of the vector and send it to our physics class for the next step
//	//int* numVtx=((int*)params[5]);
//	//Ogre::Vector3* vertices=((Ogre::Vector3*)params[6]);
//	//IndexData* indexData=((IndexData*)params[7]);
//
//	//Ogre::SceneNode *sn;
//	//sn = mSceneMgr->getRootSceneNode()->createChildSceneNode();
//	//
//	//sn->setPosition(e->mBbox.getCenter());
//	//
//
//	//loadTerrainGeometry(sn, vertices, *numVtx, indexData);
//
//
//
//	////cleanup
//	//delete[] vertices;
//	//delete numVtx;
//}






//
//void Level::pageLoaded(PagingLandscapeEvent *e)
//{
//#ifdef __editor
//	if(!((EditorApp*)app)->loadTerrainCollision)
//		return;
//#endif
////#ifdef _DEBUG
////	mLog("im debugmodus funzt das PagingLandScapeOptions leider nicht");
////	return;
////#endif
//	
//	unsigned int page_x = e->mPagex;
//	unsigned int page_z = e->mPagez;
//
//
//	
//
//	//PagingLandScapeSceneManager *plsmMgr = static_cast<PagingLandScapeSceneManager*>(mSceneMgr);
//	//PagingLandScapeOptions *opt = plsmMgr->getOptions();
//	//unsigned int TileSize = opt->TileSize;
//	//unsigned int PageSize = opt->PageSize;
//
//	//unsigned int height = opt->world_height;
//	//unsigned int width  = opt->world_width;
//					
//	//unsigned int NumTiles = opt->NumTiles;
//	//unsigned int NumPages = opt->NumPages;
//
////	mLog("pageLoaded: x="+ogre_str(page_x)+", Z="+ogre_str(page_z));
//	//Ogre::ResourceManager()::getsing
//	//Ogre::String filename = "plsm2_"+worldGeometryFile+"_"+ogre_str(page_x)+"_"+ogre_str(page_z)+"_"+ogre_str(tile_x)+"_"+ogre_str(tile_z)+".collision";
//	Ogre::ResourceGroupManager *rgm = Ogre::ResourceGroupManager::getSingletonPtr();
//	TreeCollisionSerializer* serializer=new TreeCollisionSerializer();
//
//
//	Ogre::String filename = "plsm2_"+plsm2MapName+"_"+ogre_str(page_x)+"_"+ogre_str(page_z)+".collision";
//	if(rgm->resourceExists("Collision",filename))
//	{
//		//ok, col kann geladen werden
//		Ogre::DataStreamPtr ptr = rgm->openResource(filename,"Collision");
//		CollisionPrimitives::TreeCollision *collision=new CollisionPrimitives::TreeCollision(mWorld);
//		//FILE* file=fopen("SomeFileName.collision","rb");
//		//Ogre::DataStreamPtr ptr(new FileHandleDataStream(file));
//		serializer->importTreeCollision(ptr,collision);
//		OgreNewt::Body *bod = new OgreNewt::Body(mWorld,collision);
//
//
//		//bod->attachToNode(NULL);
//		plsm2Bodies.setAt(page_x,page_z,bod);
//		//plsm2Bodies[page_x][page_z] = bod;
//		//Ogre::Matrix4
//	}
//	else
//	{
//		mLog("WARNUNG: für Page ("+ogre_str(page_x)+","+ogre_str(page_z)+") ist keine Collisionsdatei vorhanden");
//	}
//
//	//for(unsigned int tile_x=0;tile_x<NumTiles;tile_x++)
//	//	for(unsigned int tile_z=0;tile_z<NumTiles;tile_z++)
//	//	{
//	//		Ogre::String filename = "plsm2_"+worldGeometryFile+"_"+ogre_str(page_x)+"_"+ogre_str(page_z)+"_"+ogre_str(tile_x)+"_"+ogre_str(tile_z)+".collision";
//	//		if(rgm->resourceExists("Collision",filename))
//	//		{
//	//			//ok, col kann geladen werden
//	//			Ogre::DataStreamPtr ptr = rgm->openResource(filename,"Collision");
//	//			CollisionPrimitives::TreeCollision *collision=new CollisionPrimitives::TreeCollision(mWorld);
//	//			//FILE* file=fopen("SomeFileName.collision","rb");
//	//			//Ogre::DataStreamPtr ptr(new FileHandleDataStream(file));
//	//			serializer->importTreeCollision(ptr,collision);
//	//			OgreNewt::Body *bod = new OgreNewt::Body(mWorld,collision);
//	//			bod->attachToNode(NULL);
//	//		}
//
//	//	}
//
//	delete serializer;
////	mLog("pageloaded done");
//	
//
//	//e->
//}
//
//void Level::pageUnLoaded(PagingLandscapeEvent *e)
//{
////	mLog("OMFG page unLoad");
//	OgreNewt::Body* cur = plsm2Bodies.getAt(e->mPagex,e->mPagez);//plsm2Bodies[e->mPagex][e->mPagez];
//	if(cur)
//	{
//		delete cur;
//		//moar code to go
//		plsm2Bodies.setAt(e->mPagex,e->mPagez,NULL);
//		//plsm2Bodies[e->mPagex][e->mPagez] = NULL;
//
//	}
//}
void Level::destroyAllObjects()
{
//	if(mObjects.empty())
//		return;
	//ObjectList::iterator itr;
	for(ObjectList::iterator itr = mObjects.begin();itr != mObjects.end();itr++)
	{
		GameObject *toDel = *itr;		

		delete toDel;
		//mLog("wtf");
	}
	mObjects.clear();
	mSceneMgr->clearScene();
	mWorld->destroyAllBodies();
}
void Level::prepareForDestruction(bool keepBuffers)
{
	//mLog("prepareForDestruction: "+ptrToString(this));

	if(destructionPrepared)
		return;
	/*if(has_terrain)
		removeTerrain();*/

	destructorCalled = true;


	//sounds...
#ifndef __editor
	GameApp *app = (GameApp*)StandardApplication::getSingletonPtr();
	
	app->setMusic(NULL);
	app->setAmbient1(NULL);
	app->setAmbient2(NULL);
	//mgr->setBufferByType(NULL,stOwnerIsLevel);
#endif
    SimpleSound::SoundManager *mgr = SimpleSound::SoundManager::getSingletonPtr();
	for(SoundManager::SourceList::iterator itr = mSources.begin();itr!=mSources.end();itr++)
	{		
		(*itr)->setBuffer(NULL);	
	}

	for(SoundManager::BufferList::iterator itr = mBuffers.begin();itr!=mBuffers.end();itr++)
	{		
		mgr->destroyBuffer(*itr);		
	}
	//mgr->destroyBuffersByType(btLevel);

	

	//OgreAL::SoundManager *sMgr = OgreAL::SoundManager::getSingletonPtr();
	//for(SoundVector::iterator itr = musicExpPreloaded.begin();itr!=musicExpPreloaded.end();itr++)
	//{
	//	OgreAL::Sound *snd = *itr;
	//	snd->stop();
	//	sMgr->destroySound(snd);		
	//}
	//for(SoundVector::iterator itr = musicBatPreloaded.begin();itr!=musicBatPreloaded.end();itr++)
	//{
	//	OgreAL::Sound *snd = *itr;
	//	snd->stop();
	//	sMgr->destroySound(snd);		
	//}

	//if(mLvlType == lvtPaginglandscape)
	//{
	//	/*bool tr = true;
	//	mSceneMgr->setOption("pause",&tr);*/
	//	//PLSM stuff
	//	//if(loadTileDelegate)
	//	//{
	//	//	mSceneMgr->setOption("removeLoadTileListener",loadTileDelegate);
	//	//	//loadPageDelegate->clear();
	//	//	delete loadTileDelegate;
	//	//}
	//	if(loadPageDelegate)
	//	{
	//		mSceneMgr->setOption("removeLoadPageListener",loadPageDelegate);
	//		//loadPageDelegate->clear();
	//		delete loadPageDelegate;
	//	}
	//	if(unLoadPageDelegate)
	//	{
	//		mSceneMgr->setOption("removeUnloadPageListener",unLoadPageDelegate);
	//		//loadPageDelegate->clear();
	//		delete unLoadPageDelegate;
	//	}
	//	
	//	
	//	//mSceneMgr->setOption("ResetScene",&tr);
	//	
	//}

	
	destructionPrepared = true;

}
//----------------------------------------------------------------------------------------------------------
void Level::createCamera()
{
	if(!mSceneMgr->hasCamera(CAM_NAME))
	{
		mCam = mSceneMgr->createCamera(CAM_NAME);
		mCam->setNearClipDistance(0.01);
	}
	else
	{
		mCam = mSceneMgr->getCamera(CAM_NAME);
	}
#ifdef __editor
	createCameraNodes();
#endif

}

void Level::createCameraNodes()
{
	/*Ogre::SceneNode *CamNode, *CamHeadNode;*/
	
	if(mSceneMgr->hasSceneNode("CamNode")) 
	{
		
		CamNode = mSceneMgr->getSceneNode("CamNode");	
		/**/
		if(mSceneMgr->hasSceneNode("CamHeadNode"))
		{
			CamHeadNode = (Ogre::SceneNode*)CamNode->getChild("CamHeadNode");
		}
		else
		{
			CamHeadNode = CamNode->createChildSceneNode("CamHeadNode");

		}
		
		
		

		//mSceneMgr->destroySceneNode("CamNode");
	}
	else
	{	  
		CamNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("CamNode");	
		CamHeadNode = CamNode->createChildSceneNode("CamHeadNode",Ogre::Vector3(0,0,0));
		
	}
	/*PCZSceneManager *pMgr = dynamic_cast<PCZSceneManager*>(mSceneMgr);
	if(pMgr)
	{
		PCZSceneNode *pNode = static_cast<PCZSceneNode*>(CamHeadNode);
		pNode->anchorToHomeZone(pMgr->getDefaultZone());
		pNode->setHomeZone(pMgr->getDefaultZone());
	}*/
	CamHeadNode->attachObject(mCam);
}

void Level::playSound(Ogre::Vector3 position, Ogre::String filename)
{
    SimpleSound::Buffer *buf = getBuffer(filename);
    SimpleSound::Source *src = createSource(buf, stSfxLevel);
    mTempSources.push_back(src);
    src->setPosition(position);
    src->play();
}

void Level::deleteObject(GameObject *obj)
{

	mLog(obj->getDebugInfo()+" requests deletion");
	//das verhindert doppeleinträge
	ObjectList::iterator itr;
	if(!objectsToDelete.empty())
	{
		for(itr = objectsToDelete.begin();itr != objectsToDelete.end();itr++)
		{		
			GameObject *cur = *itr;//->second;

			if(cur == obj)
				return;			
		}
	}
	//unregisterObject(obj);
	mLog("object "+obj->getDebugInfo()+" added to delete list");
	objectsToDelete.push_back(obj);//pushBack(obj);
}
//
//void Level::getPlayerPage(int &x, int &z,bool useBorders)
//{
//	Ogre::Vector3 playerPos = player->getPosition()-pageStartOffset;
//  /*            /#\
//     |     |     #     |     |     |   
//	-+-----+-----#-----+-----+-----+---
//	 |     |     #0/+1 |     |     |
//	-+-----+-----#-----+-----+-----+---
//	 |     |-1/0 #  0  |+1/0 |     |   X
//	-+-----+-----X==========================>
//	 |     |     |0/-1 |     |     |
//    -+-----+-----+-----+-----+-----+---
//	 |     |     |     |     |     |
//	 wenn der wert negativ ist, muss ich hinterher noch -1 addieren
//	*/
//	//(x-a/2)/a = x/a - 0.5
//	Ogre::Real relX = playerPos.x / pageSize + 0.5; //X-position in Pages
//	Ogre::Real relZ = playerPos.z / pageSize + 0.5;
//
//	Ogre::Real actualX = Ogre::Math::Floor( relX ); //tatsächliches page
//	Ogre::Real actualZ = Ogre::Math::Floor( relZ );
//	if(!useBorders)
//	{
//		x = actualX;
//		z = actualZ;
//		return;
//	}
//
//	
//
//	if(actualX + pagingBorder < relX && actualX+1 - pagingBorder > relX)
//	{
//		x = actualX;		
//	}
//	else
//	{
//		//X ist noch in der Unschärfe
//		x = curPage_x;
//	}
//	if(actualZ + pagingBorder < relZ && actualZ+1 - pagingBorder > relZ)
//	{
//		z = actualZ;		
//	}
//	else
//	{
//		//X ist noch in der Unschärfe
//		z = curPage_z;
//	}
//	//if(relX < curPage_x+pagingBorder && relX > curPage_x-pagingBorder)
//	//{
//	//	//X ist noch in der Unschärfe
//	//	x = curPage_x;
//	//}
//	//else
//	//{
//	//	x = Ogre::Math::Floor( relX );		
//	//}
//
//	//if(relZ < curPage_z+pagingBorder && relZ > curPage_z-pagingBorder)
//	//{
//	//	z = curPage_z;
//	//}
//	//else
//	//{
//	//	z = Ogre::Math::Floor( relZ );
//	//	if(curPage_z != z)
//	//	{
//	//		int d = 0;
//	//	}
//	//}
//	
//
//
//}

Ogre::DataStreamPtr Level::getTerrainForPage(long x, long y)
{
	Ogre::DataStreamPtr result;
	result.setNull();
	//check here if we have the page
	return result;
}

void Level::defineTerrainForSection(LevelPagedWorldSection* section, long x, long y)
{
	//mTerrainGroup->saveAllTerrains
	//mTerrainGroup->getTerrainIterator(); might be usable for saving
	//mTerrainGroup->defineTerrain
}


void Level::loadTerrain()
{ 
	has_terrain = true;
    /*Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TFO_ANISOTROPIC);
    Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(7);*/
 
	//this is the light direction for the terrain. it will need to be user-defineable
    Ogre::Vector3 lightdir(0.55, -0.3, 0.75);
    lightdir.normalise();
 
    terrainLight = mSceneMgr->createLight("terrainLight");
    terrainLight->setType(Ogre::Light::LT_DIRECTIONAL);
    terrainLight->setDirection(lightdir);
	//these two colors as well
    terrainLight->setDiffuseColour(Ogre::ColourValue::White);
    terrainLight->setSpecularColour(Ogre::ColourValue(0.4, 0.4, 0.4));

	
 
	mTerrainGlobals = Ogre::TerrainGlobalOptions::getSingletonPtr();//OGRE_NEW Ogre::TerrainGlobalOptions();

	//mTerrainGlobals->setDefaultResourceGroup("Terrain");
 
	//maybe the last two params, too?
    //mTerrainGroup = OGRE_NEW Ogre::TerrainGroup(mSceneMgr, Ogre::Terrain::ALIGN_X_Z, 513, 12000.0f);
	mTerrainGroup = OGRE_NEW LevelTerrainGroup(mSceneMgr, Ogre::Terrain::ALIGN_X_Z, mTerrainData.terrainSize, mTerrainData.worldSize);
	//maybe no need at all...
    mTerrainGroup->setFilenameConvention(Ogre::String("Terrain"), Ogre::String("dat"));
    //always zero?
	mTerrainGroup->setOrigin(Ogre::Vector3(0,-100,0));//Ogre::Vector3::ZERO);

	
 
	
    configureTerrainDefaults();
 
    /*for (long x = 0; x <= 0; ++x)
        for (long y = 0; y <= 0; ++y)
            defineTerrain(x, y);*/

	
	// Paging setup
	mPageManager = OGRE_NEW LevelPageManager(this);//Ogre::PageManager();
		
	mPageProvider = new LevelPageProvider(this);
	mPageManager->setPageProvider(mPageProvider);
	mPageManager->addCamera(getMainCam());
	mTerrainPaging = OGRE_NEW LevelPaging(mPageManager);//Ogre::TerrainPaging(mPageManager);
	mTerrainPagedWorld = mPageManager->createLevelPagedWorld();
	//the numbers are loadRadius and holdRadius
	mTerrainPaging->createLevelWorldSection(mTerrainPagedWorld, mTerrainGroup, 600, 700,
			-10, -10, 
			10, 10);
	
	//OLD BEGIN
	//defineTerrain(0, 0); 
 //   // sync load since we want everything in place when we start
    //mTerrainGroup->loadAllTerrains(true);
	//OLD END
 
    if (mTerrainsImported)
    {
        Ogre::TerrainGroup::TerrainIterator ti = mTerrainGroup->getTerrainIterator();
        while(ti.hasMoreElements())
        {
            Ogre::Terrain* t = ti.getNext()->instance;			
            initBlendMaps(t);
        }
    }
 
    mTerrainGroup->freeTemporaryResources();

	//mTerrainGroup->saveAllTerrains(
	//addDecalPasses();

}

void Level::configureTerrainDefaults()
{
	//// this should be defineable in the game itself, I think
 //   mTerrainGlobals->setMaxPixelError(8);
 //   // this too, probaby
	Ogre::TerrainGlobalOptions *mTerrainGlobals = Ogre::TerrainGlobalOptions::getSingletonPtr();
 //
 //   // Important to set these so that the terrain knows what to use for derived (non-realtime) data
    mTerrainGlobals->setLightMapDirection(terrainLight->getDerivedDirection());
    mTerrainGlobals->setCompositeMapAmbient(mSceneMgr->getAmbientLight());
    mTerrainGlobals->setCompositeMapDiffuse(terrainLight->getDiffuseColour());
 
	
    // Configure default import settings for if we use imported image
    Ogre::Terrain::ImportData& defaultimp = mTerrainGroup->getDefaultImportSettings();
	defaultimp.terrainSize = mTerrainData.terrainSize;//513;
	defaultimp.worldSize = mTerrainData.worldSize;//12000.0f;
    defaultimp.inputScale = 600;
    defaultimp.minBatchSize = 33;
    defaultimp.maxBatchSize = 65;
    // textures. this has to be set in level file
    defaultimp.layerList.resize(3);
    defaultimp.layerList[0].worldSize = 100;//something about the scaling of the image, I think
    defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_diffusespecular.dds");
    defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_normalheight.dds");
	//nr. of these is defined in defaultimp.layerDeclaration, somehow
	//a certain TerrainMaterialGenerator has something to do with it
    defaultimp.layerList[1].worldSize = 30;
    defaultimp.layerList[1].textureNames.push_back("grass_green-01_diffusespecular.dds");
    defaultimp.layerList[1].textureNames.push_back("grass_green-01_normalheight.dds");
    defaultimp.layerList[2].worldSize = 200;
    defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_diffusespecular.dds");
    defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_normalheight.dds");

	

	
	

}

void Level::defineTerrain(long x, long y)
{
    Ogre::String filename = mTerrainGroup->generateFilename(x, y);
    if (Ogre::ResourceGroupManager::getSingleton().resourceExists(mTerrainGroup->getResourceGroup(), filename))
    {
		
        mTerrainGroup->defineTerrain(x, y);
    }
    else
    {
        /*Ogre::Image img;
        getTerrainImage(x % 2 != 0, y % 2 != 0, img);
        mTerrainGroup->defineTerrain(x, y, &img);*/
		mTerrainGroup->defineTerrain(x,y,float(0.0f));
        mTerrainsImported = true;
    }
}

Ogre::Real Level::getCombinedBlendValue(Ogre::Terrain* terrain, Ogre::uint8 layer, size_t x, size_t y)
{
	float baseVal = 1;//if layer == 0, then baseVal is always 1
	if(layer > 0)
	{
		Ogre::TerrainLayerBlendMap *mLayer = terrain->getLayerBlendMap(layer);
		baseVal = mLayer->getBlendValue(x,y);
	}
		
	for(Ogre::uint8 i = layer+1;i<terrain->getLayerCount();i++)
	{
		Ogre::TerrainLayerBlendMap *cur = terrain->getLayerBlendMap(i);
		float curVal = cur->getBlendValue(x,y);
		baseVal = baseVal * (1-curVal); 
	}
	return baseVal;
}

void Level::setCombinedBlendValue(Ogre::Terrain* terrain, Ogre::uint8 layer, size_t x, size_t y, Ogre::Real val)
{
	float baseVal = 1;
	if(layer > 0)
	{
		Ogre::TerrainLayerBlendMap *mLayer = terrain->getLayerBlendMap(layer);
		baseVal = mLayer->getBlendValue(x,y);
		mLayer->setBlendValue(x,y,val);
	}
	
	float valFactor = val/baseVal;

	Ogre::uint8 numLayersAbove = terrain->getLayerCount()-layer;

	//now update the layers above
	for(Ogre::uint8 i = layer+1;i<terrain->getLayerCount();i++)
	{
		Ogre::TerrainLayerBlendMap *cur = terrain->getLayerBlendMap(i);
		Ogre::Real curVal = (1-cur->getBlendValue(x,y))/valFactor;
		curVal = Ogre::Math::Clamp(curVal,0.f,1.0f);
		cur->setBlendValue(x,y,curVal);
	}
}

void Level::initBlendMaps(Ogre::Terrain* terrain)
{
    Ogre::TerrainLayerBlendMap* blendMap0 = terrain->getLayerBlendMap(1);
    Ogre::TerrainLayerBlendMap* blendMap1 = terrain->getLayerBlendMap(2);
    Ogre::Real minHeight0 = 1900;
    Ogre::Real fadeDist0 = 40;
    Ogre::Real minHeight1 = 70;
    Ogre::Real fadeDist1 = 15;
    float* pBlend1 = blendMap1->getBlendPointer();
    for (Ogre::uint16 y = 0; y < terrain->getLayerBlendMapSize(); ++y)
    {
        for (Ogre::uint16 x = 0; x < terrain->getLayerBlendMapSize(); ++x)
        {
            Ogre::Real tx, ty;
 
            blendMap0->convertImageToTerrainSpace(x, y, &tx, &ty);
            Ogre::Real height = terrain->getHeightAtTerrainPosition(tx, ty);
            Ogre::Real val = (height - minHeight0) / fadeDist0;
            val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
 
            val = (height - minHeight1) / fadeDist1;
            val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
            *pBlend1++ = val;
        }
    }
    blendMap0->dirty();
    blendMap1->dirty();
    blendMap0->update();
    blendMap1->update();
}

//converts a length in world dimensions to terrain dimensions
Ogre::Real Level::lengthWorldToTerrain(Ogre::Real worldLength)
{
	Ogre::Terrain::ImportData& defaultimp = mTerrainGroup->getDefaultImportSettings();
	Ogre::Real size = defaultimp.terrainSize;
	Ogre::Real mBase = -mTerrainGroup->getTerrainWorldSize() * 0.5;
	Ogre::Real mScale = mTerrainGroup->getTerrainWorldSize() / (Ogre::Real)(size-1);
	

	
	return (worldLength) / ((size - 1) * mScale);
}
//converts a length in terrain dimensions to world dimensions
Ogre::Real Level::lengthTerrainToWorld(Ogre::Real terrainLength)
{
	Ogre::Terrain::ImportData& defaultimp = mTerrainGroup->getDefaultImportSettings();
	Ogre::Real size = defaultimp.terrainSize;
	Ogre::Real mBase = -mTerrainGroup->getTerrainWorldSize() * 0.5;
	Ogre::Real mScale = mTerrainGroup->getTerrainWorldSize() / (Ogre::Real)(size-1);
	
	/*// centre the terrain on local origin
		mBase = -mWorldSize * 0.5; 
		// scale determines what 1 unit on the grid becomes in world space
		mScale =  mWorldSize / (Real)(mSize-1);*/
	
	
	return terrainLength * (size - 1) * mScale;
}


bool Level::LevelPageProvider::prepareProceduralPage(Ogre::Page* page, Ogre::PagedWorldSection* section) 
{ 

	////unpack
	long x;
	long y;
	mLevel->mTerrainGroup->unpackIndex(page->getID(),&x,&y);
	StandardApplication::getSingletonPtr()->pagePreparingCallback(x,y);
	
	return true; 
}

bool Level::LevelPageProvider::loadProceduralPage(Ogre::Page* page, Ogre::PagedWorldSection* section) 
{ 
	long x;
	long y;
	mLevel->mTerrainGroup->unpackIndex(page->getID(),&x,&y);	

	StandardApplication::getSingletonPtr()->pageLoadingCallback(x,y);
	//mLog("loading page "+ogre_str(x)+" / "+ogre_str(y));
	////load
	//mLevel->mTerrainGroup->loadTerrain(x,y,true);
	//
	//	

	////Ogre::Terrain *terr = mLevel->mTerrainGroup->getTerrain(x,y);

	////mLevel->initBlendMaps(terr);

	return true; 
}

bool Level::LevelPageProvider::unloadProceduralPage(Ogre::Page* page, Ogre::PagedWorldSection* section) 
{ 
	long x;
	long y;
	mLevel->mTerrainGroup->unpackIndex(page->getID(),&x,&y);	
	StandardApplication::getSingletonPtr()->pageUnloadingCallback(x,y);

	return true; 
}

bool Level::LevelPageProvider::unprepareProceduralPage(Ogre::Page* page, Ogre::PagedWorldSection* section) 
{ 
	long x;
	long y;
	mLevel->mTerrainGroup->unpackIndex(page->getID(),&x,&y);	
	StandardApplication::getSingletonPtr()->pageUnpreparingCallback(x,y);
	return true; 
}