#include <EditorHelperObjects.h>
#include <Level.h>
#include <Ogre.h>
#include <EditorApp.h>
#include <OgreBorderPanelOverlayElement.h>
#include <OgreOverlayElement.h>
#include <GroupObject.h>
#include <QueryListener.h>
#include <DynamicLines.h>
#include <TypeConverter.h>


TerrainDecal::TerrainDecal():
	mLevel(NULL),
	/*mMeshDecal(NULL),
	x_size(4),
	z_size(4),
	isShown(false),*/
	mDecalFrustum(NULL),
	mProjectorNode(NULL),
	/*prev_x(0),
	prev_z(0), 
	prev_rad(0),
	mFilterNode(NULL),
	mFilterFrustum(NULL),*/
	isShown(false)
	//,
	//accuracy(35)
{
	
	using namespace Ogre;
	//creating the frustum	
	mDecalFrustum = new Frustum();
	//mFilterFrustum = new Frustum();
	//now create the node using the default scene manager
	//now, new stuff
	//mMainNode is the node which is relocated and stuff
	//mProjectorNode is the node which projects the actual decal
	//mFilterNode is the node containing the filter, to remove the decal
   
	mProjectorNode = EditorApp::getSingletonPtr()->getDefaultSceneManager()->getRootSceneNode()->createChildSceneNode("DecalProjectorNode");


	/*mProjectorNode = mMainNode->createChildSceneNode();
	mFilterNode = mMainNode->createChildSceneNode();*/

	mProjectorNode->setOrientation(Ogre::Quaternion(Ogre::Degree(90),Ogre::Vector3::NEGATIVE_UNIT_X));

	//mFilterNode->setOrientation(Ogre::Quaternion(Ogre::Degree(180),Ogre::Vector3::UNIT_X));
	mProjectorNode->attachObject(mDecalFrustum);
	//mFilterNode->attachObject(mFilterFrustum);


	//now detatch it
	EditorApp::getSingletonPtr()->getDefaultSceneManager()->getRootSceneNode()->removeChild(mProjectorNode);

	
	mDecalFrustum->setAspectRatio(1);
	mDecalFrustum->setProjectionType(PT_ORTHOGRAPHIC);
    mDecalFrustum->setOrthoWindowHeight(100);
	mDecalFrustum->setNearClipDistance(0.1);
	mDecalFrustum->setFarClipDistance(10000);

	/*mFilterFrustum->setAspectRatio(1);
	mFilterFrustum->setProjectionType(PT_ORTHOGRAPHIC);
    mFilterFrustum->setOrthoWindowHeight(100);*/
	//mProjectorNode->setOrientation(Ogre::Quaternion(Ogre::Degree(-90),Ogre::Vector3::UNIT_X));
	/*mDecalFrustum->setNearClipDistance(1);
	mDecalFrustum->setFarClipDistance(10000);*/

	
	

}

TerrainDecal::~TerrainDecal()
{
	if(mDecalFrustum)
	{
		delete mDecalFrustum;
	}
	/*if(mFilterFrustum)
	{
		delete mFilterFrustum;
	}*/
	if(mProjectorNode)
	{
		if(mLevel)
			mLevel->getSceneManager()->destroySceneNode(mProjectorNode);
		else
			EditorApp::getSingletonPtr()->getDefaultSceneManager()->destroySceneNode(mProjectorNode);
	}
}


void TerrainDecal::addDecalPasses()
{
	mTexStates.clear();
	using namespace Ogre;
	
	if(!mLevel || !mLevel->mTerrainGroup)
		return;

	Ogre::TerrainGroup::TerrainIterator ti = mLevel->mTerrainGroup->getTerrainIterator();
    while(ti.hasMoreElements())
    {
        Ogre::Terrain* t = ti.getNext()->instance;			
		MaterialPtr mat = t->getMaterial();		


        Pass *pass =  mat->getTechnique(0)->createPass();
		pass->setSceneBlending(SBT_TRANSPARENT_ALPHA);
		//pass->setAlphaRejectSettings(Ogre::CMPF_GREATER,250);
        pass->setDepthBias(1,5);
		pass->setDepthCheckEnabled(true);
        pass->setLightingEnabled(false);
		TextureUnitState *texState = pass->createTextureUnitState("decal.png");
        texState->setProjectiveTexturing(true, mDecalFrustum);
        texState->setTextureAddressingMode(TextureUnitState::TAM_CLAMP);
        texState->setTextureFiltering(FO_POINT, FO_LINEAR, FO_NONE);

		
		mTexStates.push_back(texState);

		/*texState = pass->createTextureUnitState("decal_filter.png");
        texState->setProjectiveTexturing(true, mFilterFrustum);
        texState->setTextureAddressingMode(TextureUnitState::TAM_CLAMP);
        texState->setTextureFiltering(TFO_NONE);*/
    }
	isShown = true;//if this was called, then we are visible
}


//void TerrainDecal::generateNodeStructure(Ogre::SceneManager *mgr)
//{
//
//}

void TerrainDecal::setLevel(Level *lvl)
{
	using namespace Ogre;
	if(mLevel)
	{		
		//mProjectorNode->detachObject(mDecalFrustum);
		mLevel->getSceneManager()->getRootSceneNode()->removeChild(mProjectorNode);
		lvl->getSceneManager()->getRootSceneNode()->addChild(mProjectorNode);
		//hide();
		mLevel = lvl;		
	}
	else
	{		
		lvl->getSceneManager()->getRootSceneNode()->addChild(mProjectorNode);	
		mLevel = lvl;
	}
	addDecalPasses();
	//mProjectorNode->setOrientation(Ogre::Quaternion::IDENTITY);
	/*mProjectorNode->setOrientation(Ogre::Quaternion(Ogre::Degree(-90),Ogre::Vector3::UNIT_X));
	mFilterNode->setOrientation(Ogre::Quaternion(Ogre::Degree(90),Ogre::Vector3::UNIT_X));*/
	//mProjectorNode->attachObject(mDecalFrustum);
	mProjectorNode->setPosition(0,100,0);
	//mProjectorNode->setOrientation(Ogre::Quaternion(Ogre::Degree(-90),Ogre::Vector3::UNIT_X));

	//show();
}


void TerrainDecal::show()
{
	if(!isShown)
	{
		for(TexStateList::iterator itr = mTexStates.begin();itr!=mTexStates.end();itr++)
		{
			(*itr)->setProjectiveTexturing(true,mDecalFrustum);
			(*itr)->setTextureName("decal.png");
		}
		

		//mFilterNode->setOrientation(Ogre::Quaternion(Ogre::Degree(180),Ogre::Vector3::UNIT_X));
		//mProjectorNode->setVisible(true);
		//mLevel->getSceneManager()->getRootSceneNode()->attachObject(mMeshDecal);
		isShown = true;
	}
}


void TerrainDecal::hide()
{
	if(isShown)
	{
		for(TexStateList::iterator itr = mTexStates.begin();itr!=mTexStates.end();itr++)
		{
			(*itr)->setProjectiveTexturing(false);
			//(*itr)->setBlank();
			(*itr)->setTextureName("decal_filter.png");
		}
		//mFilterNode->setOrientation(Ogre::Quaternion(Ogre::Degree(90),Ogre::Vector3::UNIT_X));
		//mProjectorNode->setVisible(false);
		//mLevel->getSceneManager()->getRootSceneNode()->detachObject(mMeshDecal);
		isShown = false;
	}
}

void TerrainDecal::update(Ogre::Real x, Ogre::Real z, Ogre::Real decalSize)
{
	if(!isShown)
		return;
	mDecalFrustum->setOrthoWindowHeight(decalSize*20);
	//mFilterFrustum->setOrthoWindowHeight(decalSize*20);
	//mProjectorNode->setOrientation(Ogre::Quaternion(Ogre::Degree(x),Ogre::Vector3::UNIT_X));
	//mProjectorNode->pitch(Ogre::Degree(x));
	mProjectorNode->setPosition(x,100,z);
}

////SELECTION BORDER////
SelectionBorder::SelectionBorder()/*:
	debugNode(NULL)*/
{
	using namespace Ogre;
	OverlayManager& overlayManager = OverlayManager::getSingleton();
 
	// Create a panel
	/*OverlayContainer* panel = static_cast<OverlayContainer*>(
		overlayManager.createOverlayElement("Panel", "PanelName"));
	panel->setMetricsMode(Ogre::GMM_PIXELS);
	panel->setPosition(10, 10);
	panel->setDimensions(100, 100);*/
	
	//panel->setMaterialName("transform/redplane"); // Optional background material
	 
	panel = static_cast<BorderPanelOverlayElement*>(
		overlayManager.createOverlayElement("BorderPanel", "SelectionBorder"));
	panel->setBorderSize(1);
	panel->setBorderMaterialName("SelBorder");
	panel->setMetricsMode(Ogre::GMM_PIXELS);
	panel->setPosition(0, 0);
	panel->setDimensions(100, 100);


	 
	// Create an overlay, and add the panel
	overlay = overlayManager.create("SelectionBorderOverlay");
	overlay->add2D(panel);//panel);
	 
	// Add the text area to the panel
	//panel->addChild(test);
	 
	
}

//begins the drawing of the border
void SelectionBorder::begin(int mouseX, int mouseY)
{
	panel->setPosition(mouseX, mouseY);
	firstPointX = mouseX;	
	firstPointY = mouseY;
	// Show the overlay
	overlay->show();
}
//updades it, so that it follows the mouse
void SelectionBorder::update(int mouseX, int mouseY)
{
	
	//now check relevant stuff
	int width  = mouseX-firstPointX;
	int height = mouseY-firstPointY;

	int x = firstPointX;
	int y = firstPointY;

	if(width < 0)
	{
		width *= -1;
		x -= width;
	}
	if(height < 0)
	{
		height *= -1;
		y -= height;
	}

	if(x<0)
		x = 0;
	if(y<0)
		y = 0;

	EditorApp *app = EditorApp::getSingletonPtr();
	
	
	
	Ogre::Real wWidth = app->getRenderWindow()->getWidth();
    Ogre::Real wHeight= app->getRenderWindow()->getHeight();

	if (width > wWidth)
		width = wWidth;
	if (height > wHeight)
		height = wHeight;

	app->setDebugText("Pos: "+TypeConverter::ogre_str(x)+"/"+TypeConverter::ogre_str(y)+
		"; Size: "+TypeConverter::ogre_str(width)+"/"+TypeConverter::ogre_str(height));
	
	
	/*selectionBorder->setPosition(pos);
	selectionBorder->setHeight(CEGUI::UDim(0,height));
	selectionBorder->setWidth(CEGUI::UDim(0,width));*/
	panel->setPosition(x,y);
	
	panel->setDimensions(width, height);
}

void SelectionBorder::cancel()
{
	overlay->hide();
}

//ends it
GroupObject *SelectionBorder::end(int mouseX, int mouseY, GroupObject *addToGroup)
{
	using namespace Ogre;
	//first, one last update
	//update(mouseX,mouseY);
	// Show the overlay
	overlay->hide();

	firstPointX = 0;	
	firstPointY = 0;
	
    //now the raycast
    /*
    1---2
    |   |
    3---4
    1 = position
    4 = pos+size
    */
	EditorApp *app = EditorApp::getSingletonPtr();
	
	
    Ogre::Real wWidth = app->getRenderWindow()->getWidth();
    Ogre::Real wHeight= app->getRenderWindow()->getHeight();




	Ogre::Real posX = panel->getLeft();
	Ogre::Real posY = panel->getTop();
	Ogre::Real width = panel->getWidth();
	Ogre::Real height= panel->getHeight();

	if(width < 5 || height < 5)
	{
		cancel();
		return NULL;
	}



    Ogre::Vector2 point1(float(posX)/float(wWidth),float(posY)/float(wHeight));
    Ogre::Vector2 point4(float(posX+width)/float(wWidth),float(posY+height)/float(wHeight));
    Ogre::Vector2 point2(point4.x,point1.y);
    Ogre::Vector2 point3(point1.x,point4.y);
    //now 4 rays using the points
    Ogre::Camera *cam = app->getCurrentLevel()->getMainCam();
    Ogre::Ray r1 = cam->getCameraToViewportRay(point1.x,point1.y);
    Ogre::Ray r2 = cam->getCameraToViewportRay(point2.x,point2.y);
    Ogre::Ray r3 = cam->getCameraToViewportRay(point3.x,point3.y);
    Ogre::Ray r4 = cam->getCameraToViewportRay(point4.x,point4.y);

    //now building the 4 planes from these rays...
    /*
	  \          /
       \   p2   /
		r1----r2
		|      |
	 p1 |  p5  | p3
		|      |
		r3----r4
       /   p4   \
      /          \

    */
	
    Ogre::Plane p1(r1.getOrigin(),r1.getPoint(100),r3.getPoint(50));
    Ogre::Plane p2(r1.getOrigin(),r1.getPoint(100),r2.getPoint(50));
    Ogre::Plane p3(r2.getOrigin(),r2.getPoint(100),r4.getPoint(50));
    Ogre::Plane p4(r3.getOrigin(),r3.getPoint(100),r4.getPoint(50));
    //p5 should be exactly in front of the cam
    Ogre::Plane p5(r1.getOrigin(),r2.getOrigin(),r3.getOrigin());
    //now check if the normals are pointing into this thing
    if(p1.getSide(r2.getPoint(10)) == Ogre::Plane::NEGATIVE_SIDE)
    {
        //this should turn the normal around and move the point to the other side
        p1.d *= -1;
        p1.normal *= -1;
    }
    if(p2.getSide(r4.getPoint(10)) == Ogre::Plane::NEGATIVE_SIDE)
    {
        p2.d *= -1;
        p2.normal *= -1;
    }
    if(p3.getSide(r1.getPoint(10)) == Ogre::Plane::NEGATIVE_SIDE)
    {
        p3.d *= -1;
        p3.normal *= -1;
    }
    if(p4.getSide(r2.getPoint(10)) == Ogre::Plane::NEGATIVE_SIDE)
    {
        p4.d *= -1;
        p4.normal *= -1;
    }
    if(p5.getSide(r2.getPoint(10)) == Ogre::Plane::NEGATIVE_SIDE)
    {
        p5.d *= -1;
        p5.normal *= -1;
    }

 //   //debugtest
 //   //DIESEN TEIL LASS ICH ERSTMAL
 //   //Man kann ihn entkommentieren, um den Rahmen zu debuggen
	//if(debugNode)
	//{
	//	
	//	//linien

	//	lines->clear();
	//	lines->addPoint(r1.getOrigin());
	//	lines->addPoint(r1.getPoint(100));
	//	lines->addPoint(r2.getOrigin());
	//	lines->addPoint(r2.getPoint(100));
	//	lines->addPoint(r4.getOrigin());
	//	lines->addPoint(r4.getPoint(100));
	//	lines->addPoint(r3.getOrigin());
	//	lines->addPoint(r3.getPoint(100));
	//      
	//	lines->update();
	//      
	//	debugNode->detachObject(e1);
	//	debugNode->detachObject(e2);
	//	debugNode->detachObject(e3);
	//	debugNode->detachObject(e4);
	//	debugNode->detachObject(e5);

	//	app->getCurrentLevel()->getSceneManager()->destroyEntity(e1);
	//	app->getCurrentLevel()->getSceneManager()->destroyEntity(e2);
	//	app->getCurrentLevel()->getSceneManager()->destroyEntity(e3);
	//	app->getCurrentLevel()->getSceneManager()->destroyEntity(e4);
	//	app->getCurrentLevel()->getSceneManager()->destroyEntity(e5);
	//	Ogre::MeshManager *mm = Ogre::MeshManager::getSingletonPtr();

	//	mm->remove("debugPlane1");
	//	mm->remove("debugPlane2");
	//	mm->remove("debugPlane3");
	//	mm->remove("debugPlane4");
	//	mm->remove("debugPlane5");
	//	
	//	
	//	//ebenen
	//	mm->createPlane("debugPlane1",
	//	   Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, p1,
	//	   50,50,1,1,true,1,1,1,p1.normal.perpendicular());
	//	mm->createPlane("debugPlane2",
	//	   Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, p2,
	//	   50,50,1,1,true,1,1,1,p2.normal.perpendicular());
	//	mm->createPlane("debugPlane3",
	//	   Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, p3,
	//	   50,50,1,1,true,1,1,1,p3.normal.perpendicular());
	//	mm->createPlane("debugPlane4",
	//	   Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, p4,
	//	   50,50,1,1,true,1,1,1,p4.normal.perpendicular());
	//	mm->createPlane("debugPlane5",
	//	   Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, p5,
	//	   50,50,1,1,true,1,1,1,p5.normal.perpendicular());

	//	e1 = app->getCurrentLevel()->getSceneManager()->createEntity("debugent1","debugPlane1");
	//	e1->setMaterialName("Examples/RustySteel");
	//	debugNode->attachObject(e1);

	//	e2 = app->getCurrentLevel()->getSceneManager()->createEntity("debugent2","debugPlane2");
	//	e2->setMaterialName("Examples/OgreLogo");
	//	debugNode->attachObject(e2);

	//	e3 = app->getCurrentLevel()->getSceneManager()->createEntity("debugent3","debugPlane3");
	//	e3->setMaterialName("Examples/Rocky");
	//	debugNode->attachObject(e3);

	//	e4 = app->getCurrentLevel()->getSceneManager()->createEntity("debugent4","debugPlane4");
	//	e4->setMaterialName("Examples/GrassFloor");
	//	debugNode->attachObject(e4);

	//	e5 = app->getCurrentLevel()->getSceneManager()->createEntity("debugent5","debugPlane5");
	//	e5->setMaterialName("Examples/Rockwall");
	//	debugNode->attachObject(e5);

	//	camHolder->setPosition(cam->getDerivedPosition());
	//	camHolder->setOrientation(cam->getDerivedOrientation());

	//	
	//	h1->setPosition(r1.getPoint(50));

	//	
	//	h2->setPosition(r2.getPoint(50));

	//	
	//	h3->setPosition(r3.getPoint(50));

	//	
	//	h4->setPosition(r4.getPoint(50));
	//}
	//else
	//{
	//	debugNode = app->getCurrentLevel()->getSceneManager()->getRootSceneNode()->createChildSceneNode();
	//	//linien

	//	lines = new DynamicLines(RenderOperation::OT_LINE_LIST);
	//	lines->addPoint(r1.getOrigin());
	//	lines->addPoint(r1.getPoint(100));
	//	lines->addPoint(r2.getOrigin());
	//	lines->addPoint(r2.getPoint(100));
	//	lines->addPoint(r4.getOrigin());
	//	lines->addPoint(r4.getPoint(100));
	//	lines->addPoint(r3.getOrigin());
	//	lines->addPoint(r3.getPoint(100));
	//      
	//	lines->update();
	//      
	//	debugNode->attachObject(lines);
	//	//ebenen
	//	Ogre::MeshManager::getSingleton().createPlane("debugPlane1",
	//	   Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, p1,
	//	   50,50,1,1,true,1,1,1,p1.normal.perpendicular());
	//	Ogre::MeshManager::getSingleton().createPlane("debugPlane2",
	//	   Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, p2,
	//	   50,50,1,1,true,1,1,1,p2.normal.perpendicular());
	//	Ogre::MeshManager::getSingleton().createPlane("debugPlane3",
	//	   Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, p3,
	//	   50,50,1,1,true,1,1,1,p3.normal.perpendicular());
	//	Ogre::MeshManager::getSingleton().createPlane("debugPlane4",
	//	   Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, p4,
	//	   50,50,1,1,true,1,1,1,p4.normal.perpendicular());
	//	Ogre::MeshManager::getSingleton().createPlane("debugPlane5",
	//	   Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, p5,
	//	   50,50,1,1,true,1,1,1,p5.normal.perpendicular());

	//	e1 = app->getCurrentLevel()->getSceneManager()->createEntity("debugent1","debugPlane1");
	//	e1->setMaterialName("Examples/RustySteel");
	//	debugNode->attachObject(e1);

	//	e2 = app->getCurrentLevel()->getSceneManager()->createEntity("debugent2","debugPlane2");
	//	e2->setMaterialName("Examples/OgreLogo");
	//	debugNode->attachObject(e2);

	//	e3 = app->getCurrentLevel()->getSceneManager()->createEntity("debugent3","debugPlane3");
	//	e3->setMaterialName("Examples/Rocky");
	//	debugNode->attachObject(e3);

	//	e4 = app->getCurrentLevel()->getSceneManager()->createEntity("debugent4","debugPlane4");
	//	e4->setMaterialName("Examples/GrassFloor");
	//	debugNode->attachObject(e4);

	//	e5 = app->getCurrentLevel()->getSceneManager()->createEntity("debugent5","debugPlane5");
	//	e5->setMaterialName("Examples/Rockwall");
	//	debugNode->attachObject(e5);

	//	
	//	camHolder = app->getCurrentLevel()->getSceneManager()->getRootSceneNode()->createChildSceneNode();
	//	fakeCam = app->getCurrentLevel()->getSceneManager()->createEntity("asdasdasd","arrow.mesh");
	//	camHolder->attachObject(fakeCam);
	//
	//	camHolder->setPosition(cam->getDerivedPosition());
	//	camHolder->setOrientation(cam->getDerivedOrientation());

	//	h1 = app->getCurrentLevel()->getSceneManager()->getRootSceneNode()->createChildSceneNode();
	//	Entity *temp = app->getCurrentLevel()->getSceneManager()->createEntity("asdasdasd1","ogrehead.mesh");
	//	h1->attachObject(temp);
	//	h1->setScale(0.05,0.05,0.05);
	//	h1->setPosition(r1.getPoint(50));

	//	h2 = app->getCurrentLevel()->getSceneManager()->getRootSceneNode()->createChildSceneNode();
	//	temp = app->getCurrentLevel()->getSceneManager()->createEntity("asdasdasd2","ogrehead.mesh");
	//	h2->attachObject(temp);
	//	h2->setScale(0.05,0.05,0.05);
	//	h2->setPosition(r2.getPoint(50));

	//	h3 = app->getCurrentLevel()->getSceneManager()->getRootSceneNode()->createChildSceneNode();
	//	temp = app->getCurrentLevel()->getSceneManager()->createEntity("asdasdasd3","ogrehead.mesh");
	//	h3->attachObject(temp);
	//	h3->setScale(0.05,0.05,0.05);
	//	h3->setPosition(r3.getPoint(50));

	//	h4 = app->getCurrentLevel()->getSceneManager()->getRootSceneNode()->createChildSceneNode();
	//	temp = app->getCurrentLevel()->getSceneManager()->createEntity("asdasdasd4","ogrehead.mesh");
	//	h4->attachObject(temp);
	//	h4->setScale(0.05,0.05,0.05);
	//	h4->setPosition(r4.getPoint(50));
	//}

 //   return NULL;

    Ogre::PlaneBoundedVolume vol; 
    vol.planes.push_back(p1);
    vol.planes.push_back(p2);
    vol.planes.push_back(p3);
    vol.planes.push_back(p4);
    vol.planes.push_back(p5);
    Ogre::PlaneBoundedVolumeList vl;
    vl.push_back(vol);

    Ogre::PlaneBoundedVolumeListSceneQuery *qry = app->getCurrentLevel()->getSceneManager()->createPlaneBoundedVolumeQuery(vl);
    
    GroupingQueryListener *listener = new GroupingQueryListener(app->getCurrentLevel(),addToGroup);
    qry->execute(listener);
    return listener->group;
}

//
/////////////////ObjectManipulator/////////////////////////////////////
//
//ObjectManipulator::ObjectManipulator()
//{
//}
//
//ObjectManipulator::~ObjectManipulator()
//{
//}
//
//
//void ObjectManipulator::update()
//{
//
//    if(!isVisible)
//	    return;
//
//	EditorApp *app = EditorApp::getSingletonPtr();
//    
//    Ogre::Vector3 camPos = app->getCurrentLevel()->getMainCam()->getDerivedPosition();
//    Ogre::Quaternion objOrient = axesNode->_getDerivedOrientation();
//    
//
//    Ogre::Vector3 objPos = axesNode->_getDerivedPosition();
//    Ogre::Vector3 camPosRel = objOrient.Inverse()*(camPos-objPos);
//	
//    Ogre::Real scalar = Ogre::Math::Sqrt(camPosRel.squaredLength()*app->axesScaleFactor);
//    
//
//    axesNode->setScale(Ogre::Vector3(scalar));
//    prevCamPos = camPos;
//
//    //erstmal rausfinden, im welchen 3D-quadrant (relativ zum objekt) die cam sich befindet
//	//maybe change meshes and update this later, too
//    if(mTMode == tmRotate)
//	    return;
//	
//    //und jetzt hängt alles von den Vorzeichen der Komponenten von camPosRel ab
//    int quadrant = 1;
//    /*     
//	        /\ y
//            |
//        __________
//       / 5  / 6  /|
//      /____/____/ |
//     /    /    /| /
//    /____/___ / |/|   -->x
//    |    |    | /7|
//8->	| 1  | 2  |/| /
//    |---------| |/
//    | 3  | 4  | /
//    |_________|/
//
//      /
//    |/_  z
//
//    */
//
//    //das swicht die Achsenpfeile
//    if(camPosRel.x < 0)
//	    axe_X->setOrientation(Ogre::Quaternion(Ogre::Degree(90),Ogre::Vector3::UNIT_Y));
//    else
//	    axe_X->setOrientation(Ogre::Quaternion(Ogre::Degree(-90),Ogre::Vector3::UNIT_Y));
//	
//    if(camPosRel.y > 0)
//	    axe_Y->setOrientation(Ogre::Quaternion(Ogre::Degree(90),Ogre::Vector3::UNIT_X));	
//    else
//	    axe_Y->setOrientation(Ogre::Quaternion(Ogre::Degree(-90),Ogre::Vector3::UNIT_X));	
//	
//    if(camPosRel.z < 0)
//	    axe_Z->setOrientation(Ogre::Quaternion::IDENTITY);	
//    else
//	    axe_Z->setOrientation(Ogre::Quaternion(Ogre::Degree(180),Ogre::Vector3::UNIT_Y));	
//	
//
//    //und das findet nun die quadrantnummer raus, um danach die planes und evtl. das dreieck richtig zu setzen
//    if(camPosRel.x >= 0)
//    {
//	    //2,4,6,7 
//	    if(camPosRel.y >= 0)
//	    {
//		    //2,6
//		    if(camPosRel.z >= 0)
//			    quadrant = 2;
//		    else
//			    quadrant = 6;
//	    }
//	    else
//	    {
//		    //4,7
//		    if(camPosRel.z >= 0)
//			    quadrant = 4;
//		    else
//			    quadrant = 7;
//	    }
//    }
//    else
//    {
//	    //1,3,5,8
//	    if(camPosRel.y >= 0)
//	    {
//		    //1,5
//		    if(camPosRel.z >= 0)
//			    quadrant = 1;
//		    else
//			    quadrant = 5;
//	    }
//	    else
//	    {
//		    //3,8
//		    if(camPosRel.z >= 0)
//			    quadrant = 3;
//		    else
//			    quadrant = 8;
//	    }
//
//    }
//
//    switch(quadrant)
//    {
//    case 1:		
//	    plane_XZ->setPosition(Ogre::Vector3(-0.04,0,0.04));
//	    plane_XZ->setOrientation(Ogre::Quaternion(0.5,0.5,-0.5,0.5));
//		
//
//		
//	    plane_XY->setPosition(Ogre::Vector3(-0.04,0.04,0));
//	    plane_XY->setOrientation(Ogre::Quaternion(0,0,1,0));//-1.04308e-007 0 1 0
//		
//	    plane_YZ->setPosition(Ogre::Vector3(0,0.04,0.04)); //
//	    plane_YZ->setOrientation(Ogre::Quaternion(0.707107,0,-0.707107,0));
//
//	    if(boxAll)
//	    {
//		    boxAll->setPosition(Ogre::Vector3(-0.04,0.04,0.04));
//		    boxAll->setOrientation(Ogre::Quaternion(Ogre::Degree(90),Ogre::Vector3::NEGATIVE_UNIT_Y));
//	    }
//
//	    //90° an -Y
//	    break;
//    case 2:
//	    plane_XZ->setPosition(Ogre::Vector3(0.04,0,0.04));
//	    plane_XZ->setOrientation(Ogre::Quaternion(Ogre::Degree(90),Ogre::Vector3::UNIT_X));
//
//		
//	    plane_XY->setPosition(Ogre::Vector3(0.04,0.04,0));
//	    plane_XY->setOrientation(Ogre::Quaternion::IDENTITY);
//		
//	    plane_YZ->setPosition(Ogre::Vector3(0,0.04,0.04)); //
//	    plane_YZ->setOrientation(Ogre::Quaternion(0.707107,0,-0.707107,0));
//
//	    if(boxAll)
//	    {
//		    boxAll->setPosition(Ogre::Vector3(0.04,0.04,0.04));
//		    boxAll->setOrientation(Ogre::Quaternion::IDENTITY);
//	    }
//	    //identity
//	    break;
//    case 3:		
//	    plane_XZ->setPosition(Ogre::Vector3(-0.04,0,0.04));
//	    plane_XZ->setOrientation(Ogre::Quaternion(0.5,0.5,-0.5,0.5));
//
//	    plane_XY->setPosition(Ogre::Vector3(-0.04,-0.04,0));
//	    plane_XY->setOrientation(Ogre::Quaternion(0,0.707107,-0.707107,0));
//
//	    plane_YZ->setPosition(Ogre::Vector3(0,-0.04,0.04)); //
//	    plane_YZ->setOrientation(Ogre::Quaternion(-0.5,-0.5,0.5,0.5));
//
//	    if(boxAll)
//	    {
//		    boxAll->setPosition(Ogre::Vector3(-0.04,-0.04,0.04));
//		    boxAll->setOrientation(Ogre::Quaternion(0.5,0.5,-0.5,0.5));
//	    }
//		
//	    //
//	    break;
//    case 4:
//	    plane_XZ->setPosition(Ogre::Vector3(0.04,0,0.04));
//	    plane_XZ->setOrientation(Ogre::Quaternion(Ogre::Degree(90),Ogre::Vector3::UNIT_X));
//
//	    plane_XY->setPosition(Ogre::Vector3(0.04,-0.04,0));
//	    plane_XY->setOrientation(Ogre::Quaternion(0,1,0,0));
//
//	    plane_YZ->setPosition(Ogre::Vector3(0,-0.04,0.04)); //
//	    plane_YZ->setOrientation(Ogre::Quaternion(-0.5,-0.5,0.5,0.5));
//
//	    if(boxAll)
//	    {
//		    boxAll->setPosition(Ogre::Vector3(0.04,-0.04,0.04));
//		    boxAll->setOrientation(Ogre::Quaternion(Ogre::Degree(90),Ogre::Vector3::UNIT_X));
//	    }
//	    //90 an X
//	    break;
//    case 5:
//	    plane_XZ->setPosition(Ogre::Vector3(-0.04,0,-0.04));
//	    plane_XZ->setOrientation(Ogre::Quaternion(0,0,-0.707107,0.707107));
//
//	    plane_XY->setPosition(Ogre::Vector3(-0.04,0.04,0));
//	    plane_XY->setOrientation(Ogre::Quaternion(0,0,1,0));//-1.04308e-007 0 1 0
//
//	    plane_YZ->setPosition(Ogre::Vector3(0,0.04,-0.04)); //
//	    plane_YZ->setOrientation(Ogre::Quaternion(0.707107,0,0.707107,0));
//
//	    if(boxAll)
//	    {
//		    boxAll->setPosition(Ogre::Vector3(-0.04,0.04,-0.04));
//		    boxAll->setOrientation(Ogre::Quaternion(Ogre::Degree(180),Ogre::Vector3::UNIT_Y));
//	    }
//	    //180 an Y
//	    break;
//    case 6:
//	    plane_XZ->setPosition(Ogre::Vector3(0.04,0,-0.04));
//	    plane_XZ->setOrientation(Ogre::Quaternion(Ogre::Degree(-90),Ogre::Vector3::UNIT_X));
//
//	    plane_XY->setPosition(Ogre::Vector3(0.04,0.04,0));
//	    plane_XY->setOrientation(Ogre::Quaternion::IDENTITY);
//
//	    //YZ 
//	    plane_YZ->setPosition(Ogre::Vector3(0,0.04,-0.04)); //
//	    plane_YZ->setOrientation(Ogre::Quaternion(0.707107,0,0.707107,0));
//
//	    if(boxAll)
//	    {
//		    boxAll->setPosition(Ogre::Vector3(0.04,0.04,-0.04));
//		    boxAll->setOrientation(Ogre::Quaternion(Ogre::Degree(90),Ogre::Vector3::UNIT_Y));
//	    }
//	    //90 an Y
//	    break;
//    case 7:
//	    plane_XZ->setPosition(Ogre::Vector3(0.04,0,-0.04));
//	    plane_XZ->setOrientation(Ogre::Quaternion(Ogre::Degree(-90),Ogre::Vector3::UNIT_X));
//
//	    plane_XY->setPosition(Ogre::Vector3(0.04,-0.04,0));
//	    plane_XY->setOrientation(Ogre::Quaternion(0,1,0,0));
//	    //XY 
//	    plane_YZ->setPosition(Ogre::Vector3(0,-0.04,-0.04)); //
//	    plane_YZ->setOrientation(Ogre::Quaternion(0,-0.707107,0,0.707107));
//		
//	    if(boxAll)
//	    {
//		    boxAll->setPosition(Ogre::Vector3(0.04,-0.04,-0.04));
//		    boxAll->setOrientation(Ogre::Quaternion(-0.5, -0.5, -0.5, 0.5));
//	    }
//	    //-0.5 -0.5 -0.5 0.5
//	    break;
//    case 8:
//	    plane_XZ->setPosition(Ogre::Vector3(-0.04,0,-0.04));
//	    plane_XZ->setOrientation(Ogre::Quaternion(0,0,-0.707107,0.707107));
//
//	    plane_XY->setPosition(Ogre::Vector3(-0.04,-0.04,0));
//	    plane_XY->setOrientation(Ogre::Quaternion(0,0.707107,-0.707107,0));
//	    //xy 
//	    plane_YZ->setPosition(Ogre::Vector3(0,-0.04,-0.04)); //
//	    plane_YZ->setOrientation(Ogre::Quaternion(0,-0.707107,0,0.707107));
//
//	    if(boxAll)
//	    {
//		    boxAll->setPosition(Ogre::Vector3(-0.04,-0.04,-0.04));
//		    boxAll->setOrientation(Ogre::Quaternion(0, 0, -0.707107, 0.707107));
//	    }
//
//	    //3.72529e-008 1.19209e-007 -0.707107 0.707107
//		
//	    break;
//    };
//
//
//}
//
//
//void ObjectManipulator::objectScale()
//{
//	
//	Ogre::Ray mouseRay = getMouseRay();
//	Ogre::Vector3 rayOrigin = mouseRay.getOrigin();
//	Ogre::Vector3 rayDirection = mouseRay.getDirection();
//	Ogre::Vector3 newScale;
//	Ogre::Vector3 pos = selectedObject->getPosition();
//
//	
//
//	//if(oldScale == Ogre::Vector3::ZERO)
//	Ogre::Vector3 oldScale=selectedObject->getScale();
//
//	Ogre::Quaternion oldOrientation;
//	Ogre::Vector3 oldPosition;
//	selectedObject->getPositionOrientation(oldPosition,oldOrientation);
//
//	Ogre::Plane pl;	
//	Ogre::Vector3 axis; //die achse, an der ich ziehe
//	Ogre::Vector3 normal; //normale der ebene, IN der ich ziehe.
//
//	//erstmal die Achsen
//	switch(curTransform)
//	{
//	case tAxisX:
//	case tAxisY:
//	case tAxisZ:
//		{
//			std::pair<bool, Ogre::Real> res;
//			int cIndex; //das ist für direct component access, also Ogre::Vector3[i]
//			switch(curTransform)
//			{
//			case tAxisX:
//				{
//					cIndex = 0;
//			
//					axis = oldOrientation * Ogre::Vector3::UNIT_X; //die achse, an der ich ziehe
//					normal = oldOrientation * Ogre::Vector3::UNIT_Y; //normale der ebene, IN der ich ziehe.
//					pl.redefine(normal,pos);	
//					
//					//der teil prüft, ob ich die aktuelle Ebene benutzen kann
//					//res = Ogre::Math::intersects(mouseRay,pl);
//					//der nachfolgende teil prüft, ob der Mouseray nicht zu parallel zur ebene verläuft, weil dann böse
//					Ogre::Real testval = Ogre::Math::Abs(normal.dotProduct(mouseRay.getDirection())); 
//					
//					if(testval < 0.5)//!res.first)
//					{				
//						//es ist besser mit der anderen ebene			
//						normal = oldOrientation * Ogre::Vector3::UNIT_Z; //normale der ebene, IN der ich ziehe.
//						pl.redefine(normal,pos);
//					}
//					//das war die letzte achsenabhängige anweisung
//				}
//				break;
//			case tAxisY:
//				{
//					cIndex = 1;
//					axis = oldOrientation * Ogre::Vector3::UNIT_Y; //die achse, an der ich ziehe
//					normal = oldOrientation * Ogre::Vector3::UNIT_X; //normale der ebene, IN der ich ziehe.
//					pl.redefine(normal,pos);	
//
//					
//					//res = Ogre::Math::intersects(mouseRay,pl);
//					Ogre::Real testval = Ogre::Math::Abs(normal.dotProduct(mouseRay.getDirection())); 
//					
//					if(testval < 0.5)//!res.first)
//					{				
//						//es ist besser mit der anderen ebene			
//						normal = oldOrientation * Ogre::Vector3::UNIT_Z; //normale der ebene, IN der ich ziehe.
//						pl.redefine(normal,pos);					
//
//					}
//				}
//				break;
//			case tAxisZ:
//				{
//					cIndex = 2;
//					axis = oldOrientation * Ogre::Vector3::UNIT_Z; //die achse, an der ich ziehe
//					normal = oldOrientation * Ogre::Vector3::UNIT_Y; //normale der ebene, IN der ich ziehe.
//					pl.redefine(normal,pos);	
//
//					
//					//res = Ogre::Math::intersects(mouseRay,pl);
//					Ogre::Real testval = Ogre::Math::Abs(normal.dotProduct(mouseRay.getDirection())); 
//					
//					if(testval < 0.5)//!res.first)
//					{				
//						//es ist besser mit der anderen ebene			
//						normal = oldOrientation * Ogre::Vector3::UNIT_X; //normale der ebene, IN der ich ziehe.
//						pl.redefine(normal,pos);							
//
//					}
//				}
//			};
//
//
//			res = Ogre::Math::intersects(mouseRay,pl);
//			if(!res.first)
//			{
//				//o_O
//				//throw Ogre::Exception(0,"irgendwie trifft der mouseray nix","EditFrameListener::objectScale");
//				return; //keine emoware sein
//			}
//			
//
//			Ogre::Vector3 hitPoint = mouseRay.getPoint(res.second)-pos; //das ist jetzt der Punkt der ebene, den ich mit der maus getroffen habe
//			//jetzt hole ich den Abstand von hitPoint zu der ebene durch den Nullpunkt,
//			//deren Normale `axis` ist
//			if(scaleDoOnce)
//			{
//				oldHitpoint = hitPoint;
//				scaleDoOnce = false;
//			}
//			Ogre::Plane cmp(axis,0);
//			Ogre::Real dist = cmp.getDistance(hitPoint);
//			Ogre::Real dist2 = cmp.getDistance(oldHitpoint);		
//			
//
//
//			Ogre::Real ratio = Ogre::Math::Abs(dist-dist2);
//
//
//			Ogre::Vector3 testSideNor = startPosition-intersectionPoint;
//			Ogre::Plane testSide(testSideNor,0);
//			if(testSide.getDistance(hitPoint)>testSide.getDistance(oldHitpoint))
//			{
//				ratio *= -1;
//			}
//		
//			ratio /= axesNode->getScale().x;
//			ratio *= oldScale[cIndex];
//
//
//			oldHitpoint = hitPoint;
//
//			//ratio /= 2;
//			//unSnappedScale = oldScale;
//			oldScale = unSnappedScale;
//
//			//hier beginnt wieder achsenspezifisches zeug
//			//ne eigentlich nicht
//
//			
//			unSnappedScale[cIndex] = oldScale[cIndex]+ratio;
//			
//			
//			newScale = unSnappedScale;
//			if(app->snapOnScale)
//			{
//				Ogre::Real scaleSnap = app->scaleSnap;
//				//mDebugText(ogre_str(newScale.x)+"\n");
//				Ogre::Real fVal = Ogre::Math::Floor(newScale[cIndex] / scaleSnap)*scaleSnap;
//				Ogre::Real cVal = Ogre::Math::Ceil(newScale[cIndex] / scaleSnap)*scaleSnap;
//
//				if(abs(cVal - newScale[cIndex]) < abs(fVal- newScale[cIndex]))
//				{
//					//dh ceil ist näher
//					newScale[cIndex] = cVal;
//				}
//				else
//				{
//					newScale[cIndex] = fVal;
//				}
//				
//				//newScale.x = Ogre::Math::Floor(newScale.x / scaleSnap)*scaleSnap;
//				//mDebugText(ogre_str(newScale.x),true);
//			}
//		}
//		break;
//	
//	//jetzt die Ebenen:
//	case tPlaneXY:
//	case tPlaneXZ:
//	case tPlaneYZ:
//		{
//			std::pair<bool, Ogre::Real> res;
//			int index1;
//			int index2;
//			Ogre::Vector3 normal;
//			switch(curTransform)
//			{
//			case tPlaneXY:
//				index1 = 0;
//				index2 = 1;
//				normal = oldOrientation * Ogre::Vector3::UNIT_Z;
//				break;
//			case tPlaneXZ:
//				index1 = 0;
//				index2 = 2;
//				normal = oldOrientation * Ogre::Vector3::UNIT_Y;
//				break;
//			case tPlaneYZ:
//				index1 = 1;
//				index2 = 2;
//				normal = oldOrientation * Ogre::Vector3::UNIT_X;
//				break;
//			}
//			//Ogre::Vector3 axis = oldOrientation * Ogre::Vector3::UNIT_Z; //die achse, an der ich ziehe
//			//Ogre::Vector3 normal = oldOrientation * Ogre::Vector3::UNIT_Z; //normale der ebene, IN der ich ziehe.
//			Ogre::Plane pl(normal,pos);	
//
//			
//			res = Ogre::Math::intersects(mouseRay,pl);
//		
//			if(!res.first)
//			{
//				//o_O
//				//throw Ogre::Exception(0,"irgendwie trifft der mouseray nix","EditFrameListener::objectScale");
//				return;
//			}
//
//			Ogre::Vector3 hitPoint = mouseRay.getPoint(res.second)-pos;
//			if(scaleDoOnce)
//			{
//				oldHitpoint = hitPoint;
//				scaleDoOnce = false;
//			}
//			//Ogre::Plane cmp(axis,0);
//			Ogre::Real dist = hitPoint.length();//cmp.getDistance(hitPoint);
//			Ogre::Real dist2 = oldHitpoint.length();//cmp.getDistance(oldHitpoint);
//
//			Ogre::Real ratio = Ogre::Math::Abs(dist-dist2);
////			mDebugText(ogre_str(ratio));
//			Ogre::Vector3 testSideNor = startPosition-intersectionPoint;
//			Ogre::Plane testSide(testSideNor,0);
//			if(testSide.getDistance(hitPoint)>testSide.getDistance(oldHitpoint))
//			{
//				ratio *= -1;
//			}
//
//			ratio /= axesNode->getScale().x;
//			ratio *= oldScale[index1];
//
//			oldHitpoint = hitPoint;
//			oldScale = unSnappedScale;
//			
//
//			
//			unSnappedScale[index1] = oldScale[index1]+ratio;
//			unSnappedScale[index2] = oldScale[index2]+ratio;
//			/*unSnappedScale.x = oldScale.x+ratio;
//			unSnappedScale.y = oldScale.y;
//			unSnappedScale.z = oldScale.z;*/
//			
//			newScale = unSnappedScale;
//			if(app->snapOnScale)
//			{
//				Ogre::Real scaleSnap = app->scaleSnap;
//				//mDebugText(ogre_str(newScale.x)+"\n");
//				Ogre::Real fVal = Ogre::Math::Floor(newScale[index1] / scaleSnap)*scaleSnap;
//				Ogre::Real cVal = Ogre::Math::Ceil(newScale[index1] / scaleSnap)*scaleSnap;
//
//				if(abs(cVal - newScale[index1]) < abs(fVal- newScale[index1]))
//				{
//					//dh ceil ist näher
//					newScale[index1] = cVal;
//				}
//				else
//				{
//					newScale[index1] = fVal;
//				}
//				fVal = Ogre::Math::Floor(newScale[index2] / scaleSnap)*scaleSnap;
//				cVal = Ogre::Math::Ceil(newScale[index2] / scaleSnap)*scaleSnap;
//
//				if(abs(cVal - newScale[index2]) < abs(fVal- newScale[index2]))
//				{
//					//dh ceil ist näher
//					newScale[index2] = cVal;
//				}
//				else
//				{
//					newScale[index2] = fVal;
//				}
//				
//				//newScale.x = Ogre::Math::Floor(newScale.x / scaleSnap)*scaleSnap;
//				//mDebugText(ogre_str(newScale.x),true);
//			}
//		}
//		break;
//	case tAll:
//		{
//			Ogre::Quaternion camOrnt = mCamera->getDerivedOrientation();
//			Ogre::Vector3 normal = camOrnt * Ogre::Vector3::NEGATIVE_UNIT_Z; //normale der ebene, IN der ich ziehe.
//			Ogre::Plane pl(normal,pos);	
//
//			
//			std::pair<bool, Ogre::Real> res = Ogre::Math::intersects(mouseRay,pl);
//			Ogre::Real testval = Ogre::Math::Abs(normal.dotProduct(mouseRay.getDirection())); 
//			//hier kann ich alle 3 ebenen durchprobieren
////			mDebugText(ogre_str(testval));
//			if(testval < 0.2)//!res.first)
//			{				
//				//es ist besser mit der anderen ebene			
//				normal = oldOrientation * Ogre::Vector3::UNIT_Z; //normale der ebene, IN der ich ziehe.
//				pl.redefine(normal,pos);//OMFG ich n00b!
//				//hab die Zeile von da oben kopiert, wo steht `Ogre::Plane pl(normal,pos)`,
//				//und wundere mich, warum es nicht funzt -.-
//				testval = Ogre::Math::Abs(normal.dotProduct(mouseRay.getDirection())); 
//			
//				if(testval < 0.2)//!res.first)
//				{				
//					//es ist besser mit der anderen ebene			
//					normal = oldOrientation * Ogre::Vector3::UNIT_X; //normale der ebene, IN der ich ziehe.
//					pl.redefine(normal,pos);
//
//				}
//				
//
//			}
//
//			res = Ogre::Math::intersects(mouseRay,pl);
//			if(!res.first)
//			{
//				//o_O
//				//throw Ogre::Exception(0,"irgendwie trifft der mouseray nix","EditFrameListener::objectScale");
//				return;
//			}
//			
//			
//
//
//
//			Ogre::Vector3 hitPoint = mouseRay.getPoint(res.second)-pos; //das ist jetzt der Punkt der ebene, den ich mit der maus getroffen habe
//			if(scaleDoOnce)
//			{
//				oldHitpoint = hitPoint;
//				scaleDoOnce = false;
//			}
//			//jetzt hole ich den Abstand von hitPoint zu der ebene durch den Nullpunkt,
//			//deren Normale `axis` ist
////			Ogre::Plane cmp(axis,0);
//			Ogre::Real dist = hitPoint.length();
//			Ogre::Real dist2 = oldHitpoint.length();
//
//	
//			Ogre::Real ratio = Ogre::Math::Abs(dist-dist2);
//			//Ogre::Real ratio = Ogre::Math::Abs(dist/dist2);
//
//			Ogre::Vector3 testSideNor = startPosition-intersectionPoint;
//			Ogre::Plane testSide(testSideNor,0);
//			if(testSide.getDistance(hitPoint)>testSide.getDistance(oldHitpoint))
//			{
//				ratio *= -1;
//				
////				ratio = 1/ratio;
//			}
//			
//			//test
//			ratio /= axesNode->getScale().x;
//			ratio *= oldScale.x;
//		
//			oldHitpoint = hitPoint;
//			oldScale = unSnappedScale;
//
//			unSnappedScale = oldScale+Ogre::Vector3(ratio);
//			//unSnappedScale = oldScale*Ogre::Vector3(ratio);
//			/*unSnappedScale.x = oldScale.x+ratio;
//			unSnappedScale.y = oldScale.y;
//			unSnappedScale.z = oldScale.z;*/
//			
//			newScale = unSnappedScale;
//			if(app->snapOnScale)
//			{
//				Ogre::Real scaleSnap = app->scaleSnap;
//				//mDebugText(ogre_str(newScale.x)+"\n");
//				Ogre::Real fValX = Ogre::Math::Floor(newScale.x / scaleSnap)*scaleSnap;
//				Ogre::Real cValX = Ogre::Math::Ceil(newScale.x / scaleSnap)*scaleSnap;
//
//				Ogre::Real fValY = Ogre::Math::Floor(newScale.y / scaleSnap)*scaleSnap;
//				Ogre::Real cValY = Ogre::Math::Ceil(newScale.y / scaleSnap)*scaleSnap;
//
//				Ogre::Real fValZ = Ogre::Math::Floor(newScale.z / scaleSnap)*scaleSnap;
//				Ogre::Real cValZ = Ogre::Math::Ceil(newScale.z / scaleSnap)*scaleSnap;
//
//				if(abs(cValX - newScale.x) < abs(fValX- newScale.x))
//				{
//					//dh ceil ist näher
//					newScale.x = cValX;
//				}
//				else
//				{
//					newScale.x = fValX;
//				}
//				if(abs(cValY - newScale.y) < abs(fValY- newScale.y))
//				{
//					//dh ceil ist näher
//					newScale.y = cValY;
//				}
//				else
//				{
//					newScale.y = fValY;
//				}
//				if(abs(cValZ - newScale.z) < abs(fValZ- newScale.z))
//				{
//					//dh ceil ist näher
//					newScale.z = cValZ;
//				}
//				else
//				{
//					newScale.z = fValZ;
//				}
//				
//				
//				//newScale.x = Ogre::Math::Floor(newScale.x / scaleSnap)*scaleSnap;
//				//mDebugText(ogre_str(newScale.x),true);
//			}
//			//ende planecode
//
//			
//		}
//		break;
//	}
//
//
//
//	
//	//mDebugText(ogre_str(newScale));
//	selectedObject->setScale(newScale);
//}
//
//	
//void ObjectManipulator::objectRotate()
//{
//
//	Ogre::Quaternion newOrientation;
//
//
//	Ogre::Vector3 oldPosition;// = selectedObject->getPosition();
//	Ogre::Quaternion oldOrientation;// = selectedObject->getOrientation();
//	selectedObject->getPositionOrientation(oldPosition,oldOrientation);
//	//mDebugText(ogre_str(oldOrientation));
//	
//
////	CEGUI::Point mousepos =	CEGUI::MouseCursor::getSingleton().getPosition();
//
////	Ogre::Real mouseX = mousepos.d_x/mWindow->getWidth();
////	Ogre::Real mouseY = mousepos.d_y/mWindow->getHeight();
//
////	CEGUI::MouseCursor::getSingleton().setImage((CEGUI::utf8*)STYLE_EDITOR,(CEGUI::utf8*)"MouseMoveCursor");
//
/////*	app->setDebugText("cegui: x="+Ogre::StringConverter::toString()+
////						" y="+Ogre::StringConverter::toString(mousepos.d_y/mRenderWindow->getHeight())+"\n");
////	app->setDebugText("ogre: x="+Ogre::StringConverter::toString(e->getX())+
////						" y="+Ogre::StringConverter::toString(e->getY()),true);*/
//
////	Ogre::Camera *mCamera = app->getCurrentLevel()->getMainCam();//mSceneMgr->getCamera("PlayerCam");
//	Ogre::Ray mouseRay = getMouseRay();//mCamera->getCameraToViewportRay( mouseX, mouseY );
//	Ogre::Vector3 rayOrigin = mouseRay.getOrigin();
//	Ogre::Vector3 rayDirection = mouseRay.getDirection();
//
//	Ogre::Real lambda = 0;
//	Ogre::Vector3 intersectWithPlane; //der Punkt zu dem hingedreht werden soll
//
//	switch(curTransform)
//	{
//	case tAxisX:
//		lambda = (intersectionPoint.x - rayOrigin.x)/rayDirection.x;
//		intersectWithPlane = rayOrigin+rayDirection*lambda;
//		break;
//	case tAxisY:
//		lambda = (intersectionPoint.y - rayOrigin.y)/rayDirection.y;
//		intersectWithPlane = rayOrigin+rayDirection*lambda;
//		break;
//	case tAxisZ:
//		lambda = (intersectionPoint.z - rayOrigin.z)/rayDirection.z;
//		intersectWithPlane = rayOrigin+rayDirection*lambda;
//		break;
//	}
//
//	Ogre::Vector3 interSectPointRel = intersectWithPlane - oldPosition; //ist eh schon gedreht
//
//	
//	if(rotateStartDoOnce)
//	{
//		oldIntersectionPoint =holdingOffset;
//		rotateStartDoOnce = false;
//		startOrientation = oldOrientation;
//	}
//			
//	//interSectPointRel = oldOrientation*interSectPointRel;
//	switch(curTransform)
//	{
//	case tAxisX:
//		oldIntersectionPoint.x = 0;
//		interSectPointRel.x = 0;
//		break;
//	case tAxisY:
//		oldIntersectionPoint.y = 0;
//		interSectPointRel.y = 0;
//		break;
//	case tAxisZ:
//		oldIntersectionPoint.z = 0;
//		interSectPointRel.z = 0;
//	}
//
//	
//
//
//
//	newOrientation = oldIntersectionPoint.getRotationTo(interSectPointRel);
////		rotatedOrientation = newOrientation * rotatedOrientation;
//
//	
//	oldIntersectionPoint = interSectPointRel;
//
//	if(!app->snapOnAngle)
//	{
//		selectedObject->rotate(newOrientation,Ogre::Node::TS_WORLD);
//		startOrientation = newOrientation*oldOrientation;
//	}
//	else
//	{
//		//genauso, nur dass es einrastet
//		//dir wird immer gedreht, so als ob es das objekt wäre, startOrientation
//		//ist der zugehörige quaternion:
//		Ogre::Vector3 axis; //rotationsachse
//		Ogre::Vector3 basedir; //davon geht die rotation in der Ebene aus
//		Ogre::Vector3 dir;
//		Ogre::Vector3 snap;
//		switch(curTransform)
//		{
//		case tAxisX:
//			axis = Ogre::Vector3::UNIT_X;
//			basedir = Ogre::Vector3::UNIT_Y;
//			dir = (newOrientation*startOrientation)*basedir;//Ogre::Vector3::UNIT_X;
//			//dir kann nahezu null werden!
//			//das wählt die andere mögliche achse aus, wenn 
//			//nach der Rotation kaum noch was übrigbleibt
//			if(Ogre::Math::Abs(dir.y) < 0.01 && Ogre::Math::Abs(dir.z) < 0.01)
//			{
//				basedir = Ogre::Vector3::UNIT_Z;
//				dir = (newOrientation*startOrientation)*basedir;//Ogre::Vector3::UNIT_X;
//			}
//			snap = snapVector(Ogre::Degree(app->angleSnap),dir,axis);//Ogre::Vector3::UNIT_Y);
//			dir.x = 0;
//			snap.x = 0;
//			break;
//		case tAxisY:
//			axis = Ogre::Vector3::UNIT_Y;
//			basedir = Ogre::Vector3::UNIT_Z;
//			dir = (newOrientation*startOrientation)*basedir;//Ogre::Vector3::UNIT_X;
//			if(Ogre::Math::Abs(dir.x) < 0.01 && Ogre::Math::Abs(dir.z) < 0.01)
//			{
//				basedir = Ogre::Vector3::UNIT_X;
//				dir = (newOrientation*startOrientation)*basedir;//Ogre::Vector3::UNIT_X;
//			}
//			snap = snapVector(Ogre::Degree(app->angleSnap),dir,axis);//Ogre::Vector3::UNIT_Y);
//			dir.y = 0;
//			snap.y = 0;
//			break;
//		case tAxisZ:
//			axis = Ogre::Vector3::UNIT_Z;
//			basedir = Ogre::Vector3::UNIT_X;
//			dir = (newOrientation*startOrientation)*basedir;//Ogre::Vector3::UNIT_X;
//			if(Ogre::Math::Abs(dir.x) < 0.01 && Ogre::Math::Abs(dir.y) < 0.01)
//			{
//				basedir = Ogre::Vector3::UNIT_Y;
//				dir = (newOrientation*startOrientation)*basedir;//Ogre::Vector3::UNIT_X;
//				//if(Ogre::Math::Abs(dir.x) < 0.01 && Ogre::Math::Abs(dir.y) < 0.01 && Ogre::Math::Abs(dir.z) < 0.01)
//				//{
//				//	basedir = Ogre::Vector3::UNIT_Z;
//				//	dir = (newOrientation*startOrientation)*basedir;//Ogre::Vector3::UNIT_X;
//				//}
//			}
//
//			snap = snapVector(Ogre::Degree(app->angleSnap),dir,axis);//Ogre::Vector3::UNIT_Y);
//			dir.z = 0;
//			snap.z = 0;
//		}
//
//		//mDebugText("dir="+ogre_str(dir));
//		//mDebugText("\nbasedir="+ogre_str(basedir),true);
//		//mDebugText("\naxis="+ogre_str(axis),true);
//
//		
//
//		
//
//		//Ogre::Quaternion debug = Ogre::Vector3::UNIT_Z.getRotationTo(snap);
////		rotInd1->setOrientation(newOrientation);
//		//rot
//
//		//debug = Ogre::Vector3::UNIT_Z.getRotationTo(dir);
////		rotInd2->setOrientation(newOrientation*startOrientation);
//		//blau
//
//		startOrientation = newOrientation * startOrientation;
//		//selectedObject->rotate(newOrientation,Ogre::Node::TS_WORLD);
//
//		if(snap != lastSnapVector)
//		{
//			Ogre::Vector3 curdir = oldOrientation*basedir;//Ogre::Vector3::UNIT_X;
//			switch(curTransform)
//			{
//			case tAxisX:
//				curdir.x = 0;
//				break;
//			case tAxisY:
//				curdir.y = 0;
//				break;
//			case tAxisZ:
//				curdir.z = 0;
//			}
//			
//			newOrientation = curdir.getRotationTo(snap);
//			selectedObject->rotate(newOrientation,Ogre::Node::TS_WORLD);	
//		//	startOrientation = newOrientation * startOrientation;
//			lastSnapVector = snap;
//		//	
//		}	
//		//else
//		//{
//		//	//selectedObject->rotate(newOrientation,Ogre::Node::TS_WORLD);	
//		//	startOrientation = newOrientation * startOrientation;
//		//}
//		
//	}
//}
//
//	
//void ObjectManipulator::objectMove()
//{
//	/*CEGUI::Point mousepos =	CEGUI::MouseCursor::getSingleton().getPosition();
//
//	Ogre::Real mouseX = mousepos.d_x/mWindow->getWidth();
//	Ogre::Real mouseY = mousepos.d_y/mWindow->getHeight();
//
//	CEGUI::MouseCursor::getSingleton().setImage((CEGUI::utf8*)STYLE_EDITOR,(CEGUI::utf8*)"MouseMoveCursor");
//
//
//	Ogre::Camera *mCamera = app->getCurrentLevel()->getMainCam();*/
//	Ogre::Ray mouseRay = getMouseRay();//mCamera->getCameraToViewportRay( mouseX, mouseY );
//	Ogre::Vector3 rayOrigin = mouseRay.getOrigin();
//	Ogre::Vector3 rayDirection = mouseRay.getDirection();
//	Ogre::Vector3 newPosition, oldPosition;
//	oldPosition = selectedObject->getPosition();
//
//	Ogre::Vector3 pointOnPlane = intersectionPoint;
//
//	Ogre::Vector3 hOffset = holdingOffset;
//	//mDebugText(ogre_str(rayDirection));
//	switch(curTransform)
//	{
//	case tAxisX:
//		if(Ogre::Math::Abs(rayDirection.z) > 0.1)
//		{
//			//unlikely, but possible
//			Ogre::Real lambda = (intersectionPoint.z - rayOrigin.z)/rayDirection.z;
//			pointOnPlane.x=rayOrigin.x+lambda*rayDirection.x;
//			//hOffset.z = pointOnPlane.z;
//
//		}
//		else
//		{
//			//just take the other plane
//			Ogre::Real lambda = (intersectionPoint.y - rayOrigin.y)/rayDirection.y;
//			pointOnPlane.x=rayOrigin.x+lambda*rayDirection.x;
//			//hOffset.y = pointOnPlane.y;
//
//		}
//		newPosition = pointOnPlane-hOffset/*holdingOffset*/;
//		break;
//	case tAxisY:
//		/*
//		IDEA:
//		use the YX or YZ plane, and check the ray to intersect. then use only
//		the y coord of the intersection point
//		*/
//		//Ogre::Vector3 pointOnPlane = intersectionPoint;
//
//		if(Ogre::Math::Abs(rayDirection.z) > 0.1)
//		{
//			//unlikely, but possible
//			Ogre::Real lambda = (intersectionPoint.z - rayOrigin.z)/rayDirection.z;
//			pointOnPlane.y=rayOrigin.y+lambda*rayDirection.y;
//			//hOffset.z = pointOnPlane.z;
//
//
//		}
//		else
//		{
//			//just take the other plane
//			Ogre::Real lambda = (intersectionPoint.x - rayOrigin.x)/rayDirection.x;
//			pointOnPlane.y=rayOrigin.y+lambda*rayDirection.y;
//			//hOffset.x = pointOnPlane.x;
//
//		}
//		newPosition = pointOnPlane-hOffset/*holdingOffset*/;
//		break;
//	case tAxisZ:
//		if(Ogre::Math::Abs(rayDirection.y) > 0.1)
//		{
//			//unlikely, but possible
//			Ogre::Real lambda = (intersectionPoint.y - rayOrigin.y)/rayDirection.y;
//			pointOnPlane.z=rayOrigin.z+lambda*rayDirection.z;
//			//hOffset.y = pointOnPlane.y;
//
//
//		}
//		else
//		{
//			//just take the other plane
//			Ogre::Real lambda = (intersectionPoint.x - rayOrigin.x)/rayDirection.x;
//			pointOnPlane.z=rayOrigin.z+lambda*rayDirection.z;
//			//hOffset.x = pointOnPlane.x;
//
//		}
//		newPosition = pointOnPlane-hOffset/*holdingOffset*/;
//		break;
//	case tPlaneXY:
//		{
//			Ogre::Vector3 pointOnPlane = Ogre::Vector3::ZERO;
//			//ray = origin + lambda * direction
//			pointOnPlane.z=intersectionPoint.z;
//			Ogre::Real lambda = (intersectionPoint.z - rayOrigin.z)/rayDirection.z;
//			pointOnPlane.x=rayOrigin.x+lambda*rayDirection.x;
//			pointOnPlane.y=rayOrigin.y+lambda*rayDirection.y;
//
//			//hOffset.y = intersectionPoint.y;
//
//			//holdingOffset = intersectionPoint - objectPosition;
//			newPosition = pointOnPlane-hOffset/*holdingOffset*/;
//		}
//		break;
//	case tPlaneXZ:
//		{
//			//create a plane through the intersection point. erstmal y = objectINtersection.y
//			//calc the ray's intersection with this point
//			//move the first ray/obj intersection to the ray/plane intersection point
//
//
//			Ogre::Vector3 pointOnPlane = Ogre::Vector3::ZERO;
//			//ray = origin + lambda * direction
//			pointOnPlane.y=intersectionPoint.y;
//			Ogre::Real lambda = (intersectionPoint.y - rayOrigin.y)/rayDirection.y;
//			pointOnPlane.x=rayOrigin.x+lambda*rayDirection.x;
//			pointOnPlane.z=rayOrigin.z+lambda*rayDirection.z;
//
//			//hOffset.y = intersectionPoint.y;
//
//			//holdingOffset = intersectionPoint - objectPosition;
//			newPosition = pointOnPlane-hOffset/*holdingOffset*/;
//		}
//		break;
//	case tPlaneYZ:
//		{
//			Ogre::Vector3 pointOnPlane = Ogre::Vector3::ZERO;
//			//ray = origin + lambda * direction
//			pointOnPlane.x=intersectionPoint.x;
//			Ogre::Real lambda = (intersectionPoint.x - rayOrigin.x)/rayDirection.x;
//			pointOnPlane.z=rayOrigin.z+lambda*rayDirection.z;
//			pointOnPlane.y=rayOrigin.y+lambda*rayDirection.y;
//
//			//hOffset.y = intersectionPoint.y;
//
//			//holdingOffset = intersectionPoint - objectPosition;
//			newPosition = pointOnPlane-hOffset/*holdingOffset*/;
//		}
//		break;
//	}
//
//
//	if(app->snapOnGrid)
//	{
//		//anders
//		//mit floor und ceil die nächsten Punke finden, und an den näheren bewegen
//		Ogre::Vector3 newPosCeil;// = newPosition;
//		Ogre::Vector3 newPosFloor;
//		Ogre::Real gridSnap = app->gridSnap;
//
//		newPosCeil.x = ceil(newPosition.x/gridSnap)*gridSnap;
//		newPosCeil.y = ceil(newPosition.y/gridSnap)*gridSnap;
//		newPosCeil.z = ceil(newPosition.z/gridSnap)*gridSnap;
//
//		newPosFloor.x = floor(newPosition.x/gridSnap)*gridSnap;
//		newPosFloor.y = floor(newPosition.y/gridSnap)*gridSnap;
//		newPosFloor.z = floor(newPosition.z/gridSnap)*gridSnap;
//
//		if(abs(newPosCeil.x - newPosition.x) < abs(newPosFloor.x - newPosition.x))
//		{
//			//dh ceil ist näher
//			newPosition.x = newPosCeil.x;
//		}
//		else
//		{
//			newPosition.x = newPosFloor.x;
//		}
//
//		if(abs(newPosCeil.y - newPosition.y) < abs(newPosFloor.y - newPosition.y))
//		{
//			//dh ceil ist näher
//			newPosition.y = newPosCeil.y;
//		}
//		else
//		{
//			newPosition.y = newPosFloor.y;
//		}
//		if(abs(newPosCeil.z - newPosition.z) < abs(newPosFloor.z - newPosition.z))
//		{
//			//dh ceil ist näher
//			newPosition.z = newPosCeil.z;
//		}
//		else
//		{
//			newPosition.z = newPosFloor.z;
//		}
//
//	}
//	selectedObject->setPosition(newPosition);
//}
//
//	
//Ogre::Vector3 ObjectManipulator::snapVector(Ogre::Degree angle,Ogre::Vector3 src,Ogre::Vector3 axis)
//{
////	mDebugText("src="+ogre_str(src)+"\naxis="+ogre_str(axis));
//	int cnt = Ogre::Math::Floor(Ogre::Degree(360/angle).valueDegrees()); //anzahl der richtungsvektoren
//	Ogre::Quaternion rotBy; //rotation von einem r-vektor zum nächsten
//	
//	Ogre::Vector3 base;
//	if(axis == Ogre::Vector3::UNIT_X)
//	{
//		base = Ogre::Vector3::UNIT_Z; //globale achse, von der die Rotation ausgeht. also der erste mögliche snap-vektor
//		//locAxis = src		
//		if(src.directionEquals(Ogre::Vector3::UNIT_X,Ogre::Degree(10)) || src.directionEquals(Ogre::Vector3::NEGATIVE_UNIT_X,Ogre::Degree(10)))
//		{			
//			base = Ogre::Vector3::UNIT_Y;
//		}
//		src.x = 0;
//	}
//	else if(axis == Ogre::Vector3::UNIT_Y)
//	{
//		base = Ogre::Vector3::UNIT_Z; //globale achse, von der die Rotation ausgeht. also der erste mögliche snap-vektor
//		//locAxis = src		
//		if(src.directionEquals(Ogre::Vector3::UNIT_Y,Ogre::Degree(10)) || src.directionEquals(Ogre::Vector3::NEGATIVE_UNIT_Y,Ogre::Degree(10)))
//		{			
//			base = Ogre::Vector3::UNIT_X;
//		}
//		src.y = 0;
//	}
//	else if(axis == Ogre::Vector3::UNIT_Z)
//	{
//		base = Ogre::Vector3::UNIT_Y; //globale achse, von der die Rotation ausgeht. also der erste mögliche snap-vektor
//		//locAxis = src		
//		if(src.directionEquals(Ogre::Vector3::UNIT_Z,Ogre::Degree(10)) || src.directionEquals(Ogre::Vector3::NEGATIVE_UNIT_Z,Ogre::Degree(10)))
//		{			
//			base = Ogre::Vector3::UNIT_X;
//		}
//		src.z = 0;
//	}
//	else
//	{
//		//fehler
//		return Ogre::Vector3::ZERO;
//	}
//	//base.normalise();
//	src.normalise();
//	Ogre::Vector3 snap = base;
//	rotBy.FromAngleAxis(angle,axis); //rotation von einem r-vektor zum nächsten
//	for(int i=1;i<=cnt;i++)
//	{
//		
//		Ogre::Degree winkel = Ogre::Degree(Ogre::Math::ACos(src.dotProduct(snap)));
//		
//		if(winkel <= angle/2)
//		{
//			//gefunden!
//
//
//			return snap;
//			//danach gleich snap verwenden
//		}
//		//das snap weiterdrehen
//		/*mDebugText("snap "+ogre_str(i+1)+"mal gedreht");*/
//		snap = rotBy * snap;
//	}
//	return Ogre::Vector3::ZERO;
//    
//}
