#include "WorldArt.h"
#include "functions.h"
#include "level.h"
#include "GameObject.h"

using namespace Ogre;

WorldArt::WorldArt(GameObject *par,WorldArtData wData, Ogre::SceneNode *nod,Ogre::Vector3 scale)
{
	mData = wData;
	type = mData.type;
	parent = par;
	if(nod)
		mNode = nod;
	else
	{
		if(!parent->mNode)
		{
			parent->mNode = parent->mLevel->getSceneManager()->getRootSceneNode()->createChildSceneNode();
			
		}
		mNode = parent->mNode->createChildSceneNode();
	}
	
	m_sGroupName = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;
	//scale *= wData.scale;
	mData.scale *= scale;
	processWorldArtData(mData);

}

void WorldArt::loadPlane(Ogre::String material, PlaneData pData)
{
	Ogre::String name = "!plane:"+str(pData.width)+"/"+str(pData.height)
        +"/"+str(pData.xSeg)+"/"+str(pData.ySeg)
        +"/"+str(pData.normal)+"/"+str(pData.up)
        +"/"+str(pData.constant)+"/"+str(pData.numTexCoordSets)
        +"/"+str(pData.uTile)+"/"+str(pData.vTile);
	if(!Ogre::MeshManager::getSingleton().resourceExists(name))
	{

		Ogre::Plane pl(pData.normal,pData.constant);
		MeshPtr planeMeshPtr = Ogre::MeshManager::getSingleton().createPlane(name,
				   Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, pl,
				   pData.width,pData.height,pData.xSeg,pData.ySeg,
				   true,pData.numTexCoordSets,pData.uTile,pData.vTile,pData.up);
		planeMeshPtr.getPointer()->buildTangentVectors();
	}
	loadMesh(name,material);
#ifdef __editor
	//das ist 2 plane für den Editor


	name = "!plane:"+str(pData.width)+"/"+str(pData.height)
        +"/"+str(pData.xSeg)+"/"+str(pData.ySeg)
        +"/"+str(-pData.normal)+"/"+str(pData.up)
        +"/"+str(pData.constant)+"/"+str(pData.numTexCoordSets)
        +"/"+str(pData.uTile)+"/"+str(pData.vTile);
	if(!Ogre::MeshManager::getSingleton().resourceExists(name))
	{

		Ogre::Plane pl(-pData.normal,pData.constant);
		MeshPtr planeMeshPtr = Ogre::MeshManager::getSingleton().createPlane(name,
				   Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, pl,
				   pData.width,pData.height,pData.xSeg,pData.ySeg,
				   true,pData.numTexCoordSets,pData.uTile,pData.vTile,pData.up);
		planeMeshPtr.getPointer()->buildTangentVectors();
	}
	loadMesh(name,MAT_REV_PLANE);
#endif
}

WorldArt::~WorldArt()
{
	//hier detachen. die node deleten muss das obj selber
	mNode->removeAndDestroyAllChildren();
	Ogre::SceneManager *mSceneMgr = parent->mLevel->getSceneManager();
	while(mNode->numAttachedObjects())
	{

		MovableObject *m = mNode->detachObject((unsigned short)0);
		//Ogre::LogManager::getSingletonPtr()->logMessage("destroyNode: "+m->getName());
		mSceneMgr->destroyMovableObject(m);

	}
}

WorldArt::WorldArt(WorldArt *src, GameObject *target, Ogre::SceneNode *nod)
{
	mData = src->mData;
	type = src->type;
	parent = target;
	if(nod)
		mNode = nod;
	else
	{
		if(!parent->mNode)
		{
			parent->mNode = parent->mLevel->getSceneManager()->getRootSceneNode()->createChildSceneNode();
			
		}
		mNode = parent->mNode->createChildSceneNode();
	}
	
	m_sGroupName = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;
//	Ogre::Vector3 lol = src->mNode->getScale();
	mData.scale = src->mNode->getScale(); //für alle fälle
	processWorldArtData(mData);
	/*Ogre::Vector3 scale = src->mNode->getScale();
	switch(mData.type)
	{
	case WT_MESH:
		loadMesh(mData.filename,mData.material,scale,mData.positionOffset,mData.orientationOffset);
		break;
	case WT_BILLBOARD:
		loadBillboard(mData.material,mData.bbColor,1,1,mData.positionOffset);
		break;
	case WT_DOTSCENE:
		loadDotScene(mData.filename,scale,mData.positionOffset,mData.orientationOffset);
		break;
	case WT_PLANE:
		loadPlane(mData.material,mData.pData,scale,mData.positionOffset,mData.orientationOffset);
		break;
	case WT_PARTICLESYSTEM:
		loadParticleSystem(mData.material,mData.scale,mData.positionOffset,mData.orientationOffset);
		break;
	case WT_BILLBOARDCHAIN:
		throw Ogre::Exception(WT_BILLBOARDCHAIN,"WT_BILLBOARDCHAIN noch nicht implementiert!","WorldArt constructor, im switch()");
		break;
	case WT_RIBBONTRAIL:
		throw Ogre::Exception(WT_RIBBONTRAIL,"WT_RIBBONTRAIL noch nicht implementiert!","WorldArt constructor, im switch()");
		break;
	default:
		throw Ogre::Exception(type,"<-- so nen WorldArt-typen gibts nicht","WorldArt constructor, im switch()");
	}
	mNode->setScale(scale);
	mNode->setPosition(src->mNode->getPosition());
	mNode->setOrientation(src->mNode->getOrientation());
*/

	//mNode = cloneSceneNode(target->mLevel->getSceneManager(),src->mNode,NULL,Any(target));
//	Ogre::Vector3 wtf = mNode->getScale();
//	mNode->setScale(Ogre::Vector3(10,10,10));
}

WorldArt *WorldArt::clone(GameObject *targetObj)
{
	
	WorldArt *res = new WorldArt(this,targetObj);
	return res;
}

void WorldArt::processWorldArtData(WorldArtData data)
{
	mData = data;
	switch(data.type)
	{
	case WT_MESH:
		loadMesh(data.filename,data.material);
		break;
	case WT_BILLBOARD:
		loadBillboard(data.material);
		break;
	case WT_DOTSCENE:
		loadDotScene(data.filename);
		break;
	case WT_PLANE:
		loadPlane(data.material,data.pData);
		break;
	case WT_PARTICLESYSTEM:
		loadParticleSystem(data.material);
		break;
	case WT_BILLBOARDCHAIN:
		throw Ogre::Exception(WT_BILLBOARDCHAIN,"WT_BILLBOARDCHAIN noch nicht implementiert!","WorldArt constructor, im switch()");
		break;
	case WT_RIBBONTRAIL:
		throw Ogre::Exception(WT_RIBBONTRAIL,"WT_RIBBONTRAIL noch nicht implementiert!","WorldArt constructor, im switch()");
		break;
	case WT_NONE:
		throw etGenericError;
		break;
	default:
		throw Ogre::Exception(type,"<-- so nen WorldArt-typen gibts nicht","WorldArt constructor, im switch()");
	}
	
	
	mNode->setPosition(data.positionOffset);
	mNode->setOrientation(data.orientationOffset);
	mNode->setScale(data.orientationOffset*data.scale);
	

}

void WorldArt::loadBillboard(Ogre::String material,Ogre::ColourValue cl)
{
	
	Ogre::SceneManager *mSceneMgr = parent->mLevel->getSceneManager();
	Ogre::BillboardSet *bill = mSceneMgr->createBillboardSet(getUniqueBBSetName(mSceneMgr),1);
	Ogre::Billboard *b = bill->createBillboard(Ogre::Vector3::ZERO,cl);
	bill->setMaterialName(material);
	/*if(width != 0 && height != 0)
		bill->setDefaultDimensions(width,height);*/
	bill->setDefaultDimensions(1,1);
    bill->MovableObject::setUserAny(Any((GameObject*)parent));
	
	mNode->attachObject(bill);
	mNode->setInitialState();
#ifdef __editor
	Ogre::SceneNode *dNode = mNode->createChildSceneNode();
	dNode->setInheritScale(false);
	Ogre::String entName  = getUniqueEntityName(mSceneMgr);
	Ogre::Entity *ent = mSceneMgr->createEntity(entName,MESH_BOX);

	ent->setMaterialName(MAT_BBDUMMY);		
	
	ent->setUserAny(Any((GameObject*)parent));
	
	dNode->attachObject(ent);
#endif
}

void WorldArt::markSelected(bool mark)
{
	mNode->showBoundingBox(mark);
	
	Ogre::SceneNode::ChildNodeIterator nodei = mNode->getChildIterator();
	while (nodei.hasMoreElements())
	{
	   Ogre::SceneNode* childNode =     static_cast<Ogre::SceneNode*>(nodei.getNext());
	   childNode->showBoundingBox(mark);
	}
}

void WorldArt::loadParticleSystem(Ogre::String name)
{
	Ogre::SceneManager *mSceneMgr = parent->mLevel->getSceneManager();
	//===========================================
	//das evtl noch beim objekte einlesen machen
	/*if(Ogre::StringUtil::startsWith(mesh,"!plane"))
	{
		if(!createPlaneMesh(mesh))
		{
			throw Ogre::Exception(0,"could not create plane mesh "+mesh,"GameObject constructor");
		}

	}*/
	//===========================================
	if(name != "")
	{
		Ogre::String psName  = getUniquePSName(mSceneMgr);
		ParticleSystem *ps = mSceneMgr->createParticleSystem(psName,name);

		
		ps->setUserAny(Any((GameObject*)parent));
		ps->setKeepParticlesInLocalSpace(true);
		
		mNode->attachObject(ps);
		ps->setCastShadows(mData.shadow);
	}

#ifdef __editor
	Ogre::SceneNode *dNode = mNode->createChildSceneNode();
	dNode->setInheritScale(false);
	Ogre::String entName  = getUniqueEntityName(mSceneMgr);
	Ogre::Entity *ent = mSceneMgr->createEntity(entName,MESH_PSDUMMY);

	ent->setMaterialName(MAT_PSDUMMY);		
	
	ent->setUserAny(Any((GameObject*)parent));
	
	dNode->attachObject(ent);
#endif

	
	
	

	
}

void WorldArt::loadMesh(Ogre::String mesh,Ogre::String material)
{
	Ogre::SceneManager *mSceneMgr = parent->mLevel->getSceneManager();
	//===========================================
	//das evtl noch beim objekte einlesen machen
	/*if(Ogre::StringUtil::startsWith(mesh,"!plane"))
	{
		if(!createPlaneMesh(mesh))
		{
			throw Ogre::Exception(0,"could not create plane mesh "+mesh,"GameObject constructor");
		}

	}*/
	//===========================================
	
	Ogre::String entName  = getUniqueEntityName(mSceneMgr);
	Ogre::Entity *ent = mSceneMgr->createEntity(entName,mesh);

	if(material != "")
	{
		ent->setMaterialName(material);	
	}
	ent->setUserAny(Any((GameObject*)parent));
	mNode->attachObject(ent);
	
	ent->setCastShadows(mData.shadow);
//	ent->setNormaliseNormals(true);
}

void WorldArt::loadDotScene(Ogre::String filename)
{
	TiXmlDocument   *XMLDoc = 0;
	TiXmlElement   *XMLRoot;
	
	//mLevel = parent->getLevel();
	try
	{
		Ogre::DataStreamPtr pStream = Ogre::ResourceGroupManager::getSingleton().
			openResource( filename,m_sGroupName );

		Ogre::String data = pStream->getAsString();
		// Open the .scene File
		XMLDoc = new TiXmlDocument();
		XMLDoc->Parse( data.c_str() );
		pStream->close();
		pStream.setNull();

		if( XMLDoc->Error() )
		{
			//We'll just log, and continue on gracefully
			Ogre::LogManager::getSingleton().logMessage("[dotSceneObject] The TiXmlDocument reported an error");
			delete XMLDoc;
			//return;
			throw etCorruptXml;
		}
	}
	catch(...)
	{
		//We'll just log, and continue on gracefully
		Ogre::LogManager::getSingleton().logMessage("[dotSceneObject] Error creating TiXmlDocument");
		delete XMLDoc;
		//return;
		throw etCorruptXml;
	}	
	// Validate the File
	XMLRoot = XMLDoc->RootElement();
	if( Ogre::String( XMLRoot->Value()) != "scene"  ) {
		Ogre::LogManager::getSingleton().logMessage( "[dotSceneObject] Error: Invalid .scene File. Missing <scene>" );
		delete XMLDoc;      
		//return;
		throw etCorruptXml;
	}
	


	
	

	// Process the scene
	ds_processScene(XMLRoot);


	


	// Close the XML File
	delete XMLDoc;	

}

void WorldArt::ds_finishConstructor()
{
	mNode->setInitialState();
}


void WorldArt::ds_processScene(TiXmlElement *XMLRoot)
{
	// Process the scene parameters
	Ogre::String version = getAttrib(XMLRoot, "formatVersion", "unknown");

	Ogre::String message = "[dotSceneObject] Parsing dotScene file with version " + version;
	if(XMLRoot->Attribute("ID"))
		message += ", id " + Ogre::String(XMLRoot->Attribute("ID"));
	if(XMLRoot->Attribute("sceneManager"))
		message += ", scene manager " + Ogre::String(XMLRoot->Attribute("sceneManager"));
	if(XMLRoot->Attribute("minOgreVersion"))
		message += ", min. Ogre version " + Ogre::String(XMLRoot->Attribute("minOgreVersion"));
	if(XMLRoot->Attribute("author"))
		message += ", author " + Ogre::String(XMLRoot->Attribute("author"));

	Ogre::LogManager::getSingleton().logMessage(message);

	TiXmlElement *pElement;

	// Process nodes (?)
	pElement = XMLRoot->FirstChildElement("nodes");
	if(pElement)
		ds_processNodes(pElement);

	//// Process externals (?)
	//pElement = XMLRoot->FirstChildElement("externals");
	//if(pElement)
	//	ds_processExternals(pElement);

	//// Process environment (?)
	//pElement = XMLRoot->FirstChildElement("environment");
	//if(pElement)
	//	ds_processEnvironment(pElement);

	//// Process terrain (?)
	//pElement = XMLRoot->FirstChildElement("terrain");
	//if(pElement)
	//	ds_processTerrain(pElement);

	//// Process userDataReference (?)
	//pElement = XMLRoot->FirstChildElement("userDataReference");
	//if(pElement)
	//	ds_processUserDataReference(pElement);

	//// Process octree (?)
	//pElement = XMLRoot->FirstChildElement("octree");
	//if(pElement)
	//	processOctree(pElement);

	// Process light (?)
	pElement = XMLRoot->FirstChildElement("light");
	if(pElement)
		ds_processLight(pElement);

	//// Process camera (?)
	//pElement = XMLRoot->FirstChildElement("camera");
	//if(pElement)
	//	processCamera(pElement);
}


void WorldArt::ds_processNodes(TiXmlElement *XMLNode)
{
	TiXmlElement *pElement;

	// Process node (*)
	pElement = XMLNode->FirstChildElement("node");
	while(pElement)
	{
		ds_processNode(pElement);
		pElement = pElement->NextSiblingElement("node");
	}

	// Process position (?)
	pElement = XMLNode->FirstChildElement("position");
	if(pElement)
	{
		//atm kA wie ich das benutzen soll
		//positionOffset = ds_parseVector3(pElement);



		//mAttachNode->setInitialState();
		//mNode->setInitialState();
	}

	// Process rotation (?)
	pElement = XMLNode->FirstChildElement("rotation");
	if(pElement)
	{
		//mAttachNode->setOrientation(parseQuaternion(pElement));
		//mAttachNode->setInitialState();
		mNode->setOrientation(ds_parseQuaternion(pElement));
		mNode->setInitialState();
	}

	// Process scale (?)
	pElement = XMLNode->FirstChildElement("scale");
	if(pElement)
	{
		mNode->setScale(ds_parseVector3(pElement));
		mNode->setInitialState();
	}
}


void WorldArt::ds_processLight(TiXmlElement *XMLNode, Ogre::SceneNode *pParent)
{
	// Process attributes
	Ogre::String name = getAttrib(XMLNode, "name");
	Ogre::String id = getAttrib(XMLNode, "id");

	// Create the light
	Ogre::Light *pLight = parent->mLevel->getSceneManager()->createLight(name);
	if(pParent)
		pParent->attachObject(pLight);
	else
		mNode->attachObject(pLight);

	Ogre::String sValue = getAttrib(XMLNode, "type");
	if(sValue == "point")
		pLight->setType(Ogre::Light::LT_POINT);
	else if(sValue == "directional")
		pLight->setType(Ogre::Light::LT_DIRECTIONAL);
	else if(sValue == "spot")
		pLight->setType(Ogre::Light::LT_SPOTLIGHT);
	else if(sValue == "radPoint")
		pLight->setType(Ogre::Light::LT_POINT);

	pLight->setVisible(getAttribBool(XMLNode, "visible", true));
	pLight->setCastShadows(getAttribBool(XMLNode, "castShadows", true));

	TiXmlElement *pElement;

	// Process position (?)
	pElement = XMLNode->FirstChildElement("position");
	if(pElement)
		pLight->setPosition(ds_parseVector3(pElement));

	// Process normal (?)
	pElement = XMLNode->FirstChildElement("normal");
	if(pElement)
		pLight->setDirection(ds_parseVector3(pElement));

	// Process colourDiffuse (?)
	pElement = XMLNode->FirstChildElement("colourDiffuse");
	if(pElement)
		pLight->setDiffuseColour(ds_parseColour(pElement));

	// Process colourSpecular (?)
	pElement = XMLNode->FirstChildElement("colourSpecular");
	if(pElement)
		pLight->setSpecularColour(ds_parseColour(pElement));

	//// Process lightRange (?)
	//pElement = XMLNode->FirstChildElement("lightRange");
	//if(pElement)
	//	processLightRange(pElement, pLight);

	//// Process lightAttenuation (?)
	//pElement = XMLNode->FirstChildElement("lightAttenuation");
	//if(pElement)
	//	processLightAttenuation(pElement, pLight);

	pLight->setUserAny(Any((GameObject*)parent));

	//// Process userDataReference (?)
	//pElement = XMLNode->FirstChildElement("userDataReference");
	//if(pElement)
	//	;//processUserDataReference(pElement, pLight);
}

void WorldArt::ds_processNode(TiXmlElement *XMLNode, Ogre::SceneNode *pParent)
{
	// Construct the node's name
	Ogre::String name = getUniqueNodeName(parent->mLevel->getSceneManager(),getAttrib(XMLNode, "name"));

	// Create the scene node
	Ogre::SceneNode *pNode;
	if(name.empty())
	{
		// Let Ogre choose the name
		if(pParent)
			pNode = pParent->createChildSceneNode();
		else
			pNode = mNode->createChildSceneNode();
	}
	else
	{
		// Provide the name
		if(pParent)
			pNode = pParent->createChildSceneNode(name);
		else
			pNode = mNode->createChildSceneNode(name);
	}

	// Process other attributes
	Ogre::String id = getAttrib(XMLNode, "id");

	TiXmlElement *pElement;

	// Process position (?)
	pElement = XMLNode->FirstChildElement("position");
	if(pElement)
	{
		pNode->setPosition(ds_parseVector3(pElement));
		pNode->setInitialState();
	}

	// Process rotation (?)
	pElement = XMLNode->FirstChildElement("rotation");
	if(pElement)
	{
		pNode->setOrientation(ds_parseQuaternion(pElement));
		pNode->setInitialState();
	}

	// Process scale (?)
	pElement = XMLNode->FirstChildElement("scale");
	if(pElement)
	{
		pNode->setScale(ds_parseVector3(pElement));
		pNode->setInitialState();
	}

	//// Process lookTarget (?)
	//pElement = XMLNode->FirstChildElement("lookTarget");
	//if(pElement)
	//	processLookTarget(pElement, pNode);

	//// Process trackTarget (?)
	//pElement = XMLNode->FirstChildElement("trackTarget");
	//if(pElement)
	//	processTrackTarget(pElement, pNode);

	// Process node (*)
	pElement = XMLNode->FirstChildElement("node");
	while(pElement)
	{
		ds_processNode(pElement, pNode);
		pElement = pElement->NextSiblingElement("node");
	}

	// Process entity (*)
	pElement = XMLNode->FirstChildElement("entity");
	while(pElement)
	{
		ds_processEntity(pElement, pNode);
		pElement = pElement->NextSiblingElement("entity");
	}

	// Process light (*)
	pElement = XMLNode->FirstChildElement("light");
	while(pElement)
	{
		ds_processLight(pElement, pNode);
		pElement = pElement->NextSiblingElement("light");
	}

	//// Process camera (*)
	//pElement = XMLNode->FirstChildElement("camera");
	//while(pElement)
	//{
	//	processCamera(pElement, pNode);
	//	pElement = pElement->NextSiblingElement("camera");
	//}

	// Process particleSystem (*)
	pElement = XMLNode->FirstChildElement("particleSystem");
	while(pElement)
	{
		ds_processParticleSystem(pElement, pNode);
		pElement = pElement->NextSiblingElement("particleSystem");
	}

	// Process billboardSet (*)
	pElement = XMLNode->FirstChildElement("billboardSet");
	while(pElement)
	{
		ds_processBillboardSet(pElement, pNode);
		pElement = pElement->NextSiblingElement("billboardSet");
	}

	// Process plane (*)
	pElement = XMLNode->FirstChildElement("plane");
	while(pElement)
	{
		ds_processPlane(pElement, pNode);
		pElement = pElement->NextSiblingElement("plane");
	}

	//// Process userDataReference (?)
	//pElement = XMLNode->FirstChildElement("userDataReference");
	//if(pElement)
	//	processUserDataReference(pElement, pNode);
}
//
//void WorldArt::ds_processLookTarget(TiXmlElement *XMLNode, Ogre::SceneNode *pParent)
//{
//	//! @todo Is this correct? Cause I don't have a clue actually
//
//	// Process attributes
//	Ogre::String nodeName = getAttrib(XMLNode, "nodeName");
//
//	Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_PARENT;
//	Ogre::String sValue = getAttrib(XMLNode, "relativeTo");
//	if(sValue == "local")
//		relativeTo = Ogre::Node::TS_LOCAL;
//	else if(sValue == "parent")
//		relativeTo = Ogre::Node::TS_PARENT;
//	else if(sValue == "world")
//		relativeTo = Ogre::Node::TS_WORLD;
//
//	TiXmlElement *pElement;
//
//	// Process position (?)
//	Ogre::Vector3 position;
//	pElement = XMLNode->FirstChildElement("position");
//	if(pElement)
//		position = parseVector3(pElement);
//
//	// Process localDirection (?)
//	Ogre::Vector3 localDirection = Ogre::Vector3::NEGATIVE_UNIT_Z;
//	pElement = XMLNode->FirstChildElement("localDirection");
//	if(pElement)
//		localDirection = parseVector3(pElement);
//
//	// Setup the look target
//	try
//	{
//		if(!nodeName.empty())
//		{
//			Ogre::SceneNode *pLookNode = mLevel->getSceneManager()->getSceneNode(nodeName);
//			position = pLookNode->_getDerivedPosition();
//		}
//
//		pParent->lookAt(position, relativeTo, localDirection);
//	}
//	catch(Ogre::Exception &/*e*/)
//	{
//		Ogre::LogManager::getSingleton().logMessage("[dotSceneObject] Error processing a look target!");
//	}
//}
//
//void dotSceneObject::processTrackTarget(TiXmlElement *XMLNode, Ogre::SceneNode *pParent)
//{
//	// Process attributes
//	Ogre::String nodeName = getAttrib(XMLNode, "nodeName");
//
//	TiXmlElement *pElement;
//
//	// Process localDirection (?)
//	Ogre::Vector3 localDirection = Ogre::Vector3::NEGATIVE_UNIT_Z;
//	pElement = XMLNode->FirstChildElement("localDirection");
//	if(pElement)
//		localDirection = parseVector3(pElement);
//
//	// Process offset (?)
//	Ogre::Vector3 offset = Ogre::Vector3::ZERO;
//	pElement = XMLNode->FirstChildElement("offset");
//	if(pElement)
//		offset = parseVector3(pElement);
//
//	// Setup the track target
//	try
//	{
//		Ogre::SceneNode *pTrackNode = mLevel->getSceneManager()->getSceneNode(nodeName);
//		pParent->setAutoTracking(true, pTrackNode, localDirection, offset);
//	}
//	catch(Ogre::Exception &/*e*/)
//	{
//		Ogre::LogManager::getSingleton().logMessage("[dotSceneObject] Error processing a track target!");
//	}
//}

void WorldArt::ds_processEntity(TiXmlElement *XMLNode, Ogre::SceneNode *pParent)
{
	// Process attributes
	Ogre::String name = getAttrib(XMLNode, "name");
	Ogre::String id = getAttrib(XMLNode, "id");
	Ogre::String meshFile = getAttrib(XMLNode, "meshFile");
	Ogre::String materialFile = getAttrib(XMLNode, "materialFile");
	//bool isStatic = getAttribBool(XMLNode, "static", false); //<-- ka wie ich das nutzen soll...
	bool castShadows = getAttribBool(XMLNode, "castShadows", true);

	// TEMP: Maintain a list of static and dynamic objects
	/*if(isStatic)
		staticObjects.push_back(name);
	else
		dynamicObjects.push_back(name);*/

	TiXmlElement *pElement;

	// Process vertexBuffer (?)
	//pElement = XMLNode->FirstChildElement("vertexBuffer");
	//if(pElement)
	//	;//processVertexBuffer(pElement);

	//// Process indexBuffer (?)
	//pElement = XMLNode->FirstChildElement("indexBuffer");
	//if(pElement)
	//	;//processIndexBuffer(pElement);

	// Create the entity
	Ogre::Entity *pEntity = 0;
	try
	{
		Ogre::MeshManager::getSingleton().load(meshFile , m_sGroupName);
		pEntity = parent->mLevel->getSceneManager()->createEntity(name, meshFile);
		pEntity->setCastShadows(castShadows);
		pParent->attachObject(pEntity);

		//pEntity->setUserAny(Any((GameObject*)parent));
		
		if(!materialFile.empty())
			pEntity->setMaterialName(materialFile);
	}
	catch(Ogre::Exception &/*e*/)
	{
		Ogre::LogManager::getSingleton().logMessage("[dotSceneObject] Error loading an entity!");
	}

	// Process userDataReference (?)
	pElement = XMLNode->FirstChildElement("userDataReference");
	/*if(pElement)
		processUserDataReference(pElement, pEntity);*/
	pEntity->setUserAny(Any((GameObject*)parent));

	
}

void WorldArt::ds_processParticleSystem(TiXmlElement *XMLNode, Ogre::SceneNode *pParent)
{
	// Process attributes
	Ogre::String name = getAttrib(XMLNode, "name");
	Ogre::String id = getAttrib(XMLNode, "id");
	Ogre::String file = getAttrib(XMLNode, "file");

	// Create the particle system
	try
	{
		ParticleSystem *pParticles = parent->mLevel->getSceneManager()->createParticleSystem(name, file);
		pParent->attachObject(pParticles);
		pParticles->setUserAny(Any((GameObject*)parent));
	}
	catch(Ogre::Exception &/*e*/)
	{
		Ogre::LogManager::getSingleton().logMessage("[dotSceneObject] Error creating a particle system!");
	}
}

void WorldArt::ds_processBillboardSet(TiXmlElement *XMLNode, Ogre::SceneNode *pParent)
{
	//! @todo Implement this
	//hab leider keine Ahung wie so etwas in einer dotscene aussieht,
	//muss nachsehen...
}

void WorldArt::ds_processPlane(TiXmlElement *XMLNode, Ogre::SceneNode *pParent)
{
	//! @todo Implement this
	//hab leider keine Ahung wie so etwas in einer dotscene aussieht,
	//muss nachsehen...

}
//
//void dotSceneObject::processFog(TiXmlElement *XMLNode)
//{
//	//// Process attributes
//	//Ogre::Real expDensity = getAttribReal(XMLNode, "expDensity", 0.001);
//	//Ogre::Real linearStart = getAttribReal(XMLNode, "linearStart", 0.0);
//	//Ogre::Real linearEnd = getAttribReal(XMLNode, "linearEnd", 1.0);
//
//	//Ogre::FogMode mode = Ogre::FOG_NONE;
//	//Ogre::String sMode = getAttrib(XMLNode, "mode");
//	//if(sMode == "none")
//	//	mode = Ogre::FOG_NONE;
//	//else if(sMode == "exp")
//	//	mode = Ogre::FOG_EXP;
//	//else if(sMode == "exp2")
//	//	mode = Ogre::FOG_EXP2;
//	//else if(sMode == "linear")
//	//	mode = Ogre::FOG_LINEAR;
//
//	//TiXmlElement *pElement;
//
//	//// Process colourDiffuse (?)
//	//Ogre::ColourValue colourDiffuse = Ogre::ColourValue::White;
//	//pElement = XMLNode->FirstChildElement("colourDiffuse");
//	//if(pElement)
//	//	colourDiffuse = parseColour(pElement);
//
//	//// Setup the fog
//	//mLevel->getSceneManager()->setFog(mode, colourDiffuse, expDensity, linearStart, linearEnd);
//}
//
//void dotSceneObject::processSkyBox(TiXmlElement *XMLNode)
//{
//	//// Process attributes
//	//Ogre::String material = getAttrib(XMLNode, "material");
//	//Ogre::Real distance = getAttribReal(XMLNode, "distance", 5000);
//	//bool drawFirst = getAttribBool(XMLNode, "drawFirst", true);
//
//	//TiXmlElement *pElement;
//
//	//// Process rotation (?)
//	//Ogre::Quaternion rotation = Ogre::Quaternion::IDENTITY;
//	//pElement = XMLNode->FirstChildElement("rotation");
//	//if(pElement)
//	//	rotation = parseQuaternion(pElement);
//
//	//// Setup the sky box
//	//mLevel->getSceneManager()->setSkyBox(true, material, distance, drawFirst, rotation, m_sGroupName);
//}
//
//void dotSceneObject::processSkyDome(TiXmlElement *XMLNode)
//{
//	//// Process attributes
//	//Ogre::String material = XMLNode->Attribute("material");
//	//Ogre::Real curvature = getAttribReal(XMLNode, "curvature", 10);
//	//Ogre::Real tiling = getAttribReal(XMLNode, "tiling", 8);
//	//Ogre::Real distance = getAttribReal(XMLNode, "distance", 4000);
//	//bool drawFirst = getAttribBool(XMLNode, "drawFirst", true);
//
//	//TiXmlElement *pElement;
//
//	//// Process rotation (?)
//	//Ogre::Quaternion rotation = Ogre::Quaternion::IDENTITY;
//	//pElement = XMLNode->FirstChildElement("rotation");
//	//if(pElement)
//	//	rotation = parseQuaternion(pElement);
//
//	//// Setup the sky dome
//	//mLevel->getSceneManager()->setSkyDome(true, material, curvature, tiling, distance, drawFirst, rotation, 16, 16, -1, m_sGroupName);
//}
//
//void dotSceneObject::processSkyPlane(TiXmlElement *XMLNode)
//{
//	//// Process attributes
//	//Ogre::String material = getAttrib(XMLNode, "material");
//	//Ogre::Real planeX = getAttribReal(XMLNode, "planeX", 0);
//	//Ogre::Real planeY = getAttribReal(XMLNode, "planeY", -1);
//	//Ogre::Real planeZ = getAttribReal(XMLNode, "planeX", 0);
//	//Ogre::Real planeD = getAttribReal(XMLNode, "planeD", 5000);
//	//Ogre::Real scale = getAttribReal(XMLNode, "scale", 1000);
//	//Ogre::Real bow = getAttribReal(XMLNode, "bow", 0);
//	//Ogre::Real tiling = getAttribReal(XMLNode, "tiling", 10);
//	//bool drawFirst = getAttribBool(XMLNode, "drawFirst", true);
//
//	//// Setup the sky plane
//	//Ogre::Plane plane;
//	//plane.normal = Ogre::Vector3(planeX, planeY, planeZ);
//	//plane.d = planeD;
//	//mLevel->getSceneManager()->setSkyPlane(true, plane, material, scale, tiling, drawFirst, bow, 1, 1, m_sGroupName);
//}
//
//void dotSceneObject::processClipping(TiXmlElement *XMLNode)
//{
//	//! @todo Implement this
//
//	// Process attributes
//	Ogre::Real fNear = getAttribReal(XMLNode, "near", 0);
//	Ogre::Real fFar = getAttribReal(XMLNode, "far", 1);
//}
//
//void dotSceneObject::processLightRange(TiXmlElement *XMLNode, Ogre::Light *pLight)
//{
//	// Process attributes
//	Ogre::Real inner = getAttribReal(XMLNode, "inner");
//	Ogre::Real outer = getAttribReal(XMLNode, "outer");
//	Ogre::Real falloff = getAttribReal(XMLNode, "falloff", 1.0);
//
//	// Setup the light range
//	pLight->setSpotlightRange(Angle(inner), Angle(outer), falloff);
//}
//
//void dotSceneObject::processLightAttenuation(TiXmlElement *XMLNode, Ogre::Light *pLight)
//{
//	// Process attributes
//	Ogre::Real range = getAttribReal(XMLNode, "range");
//	Ogre::Real constant = getAttribReal(XMLNode, "constant");
//	Ogre::Real linear = getAttribReal(XMLNode, "linear");
//	Ogre::Real quadratic = getAttribReal(XMLNode, "quadratic");
//
//	// Setup the light attenuation
//	pLight->setAttenuation(range, constant, linear, quadratic);
//}
//

Ogre::String WorldArt::getAttrib(TiXmlElement *XMLNode, const Ogre::String &attrib, const Ogre::String &defaultValue)
{
	
	if(XMLNode->Attribute(attrib.c_str()))
		return XMLNode->Attribute(attrib.c_str());
	else
		return defaultValue;
}

Ogre::Real WorldArt::getAttribReal(TiXmlElement *XMLNode, const Ogre::String &attrib, Ogre::Real defaultValue)
{
	if(XMLNode->Attribute(attrib.c_str()))
		return Ogre::StringConverter::parseReal(XMLNode->Attribute(attrib.c_str()));
	else
		return defaultValue;
}

bool WorldArt::getAttribBool(TiXmlElement *XMLNode, const Ogre::String &attrib, bool defaultValue)
{
	if(!XMLNode->Attribute(attrib.c_str()))
		return defaultValue;

	if(Ogre::String(XMLNode->Attribute(attrib.c_str())) == "true")
		return true;

	return false;
}


Ogre::Vector3 WorldArt::ds_parseVector3(TiXmlElement *XMLNode)
{
	return Ogre::Vector3(
		Ogre::StringConverter::parseReal(XMLNode->Attribute("x")),
		Ogre::StringConverter::parseReal(XMLNode->Attribute("y")),
		Ogre::StringConverter::parseReal(XMLNode->Attribute("z"))
	);
}

Ogre::Quaternion WorldArt::ds_parseQuaternion(TiXmlElement *XMLNode)
{
	//! @todo Fix this shit!

	Ogre::Quaternion orientation;

	if(XMLNode->Attribute("qx"))
	{
		orientation.x = Ogre::StringConverter::parseReal(XMLNode->Attribute("qx"));
		orientation.y = Ogre::StringConverter::parseReal(XMLNode->Attribute("qy"));
		orientation.z = Ogre::StringConverter::parseReal(XMLNode->Attribute("qz"));
		orientation.w = Ogre::StringConverter::parseReal(XMLNode->Attribute("qw"));
	}
	else if(XMLNode->Attribute("axisX"))
	{
		Ogre::Vector3 axis;
		axis.x = Ogre::StringConverter::parseReal(XMLNode->Attribute("axisX"));
		axis.y = Ogre::StringConverter::parseReal(XMLNode->Attribute("axisY"));
		axis.z = Ogre::StringConverter::parseReal(XMLNode->Attribute("axisZ"));
		Ogre::Real angle = Ogre::StringConverter::parseReal(XMLNode->Attribute("angle"));;
		orientation.FromAngleAxis(Ogre::Angle(angle), axis);
	}
	else if(XMLNode->Attribute("angleX"))
	{
		Ogre::Vector3 axis;
		axis.x = Ogre::StringConverter::parseReal(XMLNode->Attribute("angleX"));
		axis.y = Ogre::StringConverter::parseReal(XMLNode->Attribute("angleY"));
		axis.z = Ogre::StringConverter::parseReal(XMLNode->Attribute("angleZ"));
		//orientation.FromAxes(&axis);
		//orientation.F
	}

	return orientation;
}

Ogre::ColourValue WorldArt::ds_parseColour(TiXmlElement *XMLNode)
{
	return Ogre::ColourValue(
		Ogre::StringConverter::parseReal(XMLNode->Attribute("r")),
		Ogre::StringConverter::parseReal(XMLNode->Attribute("g")),
		Ogre::StringConverter::parseReal(XMLNode->Attribute("b")),
		XMLNode->Attribute("a") != NULL ? Ogre::StringConverter::parseReal(XMLNode->Attribute("a")) : 1
	);
}

Ogre::AxisAlignedBox WorldArt::getBoundingBox()
{
	return Ogre::AxisAlignedBox();
}

OgreNewt::CollisionPtr WorldArt::getTreeCollision()
{
	OgreNewt::Collision *col;
	/*if(useSceneParser)
	{*/
	OgreNewt::CollisionPrimitives::TreeCollisionSceneParser *testcol = new OgreNewt::CollisionPrimitives::TreeCollisionSceneParser(parent->mLevel->getWorld());
	
	testcol->parseScene(mNode,0,false);
	col = testcol;


	//}
	//else
	//{
	//	// = new OgreNewt::Collision();
	//	col = new OgreNewt::CollisionPrimitives::TreeCollision(parent->mLevel->getWorld(),mNode,false);
	//}
	return OgreNewt::CollisionPtr(col);
}

Ogre::AnimationState *WorldArt::getAnimState(Ogre::String name)
{
	if(type == WT_MESH)
	{
		MovableObject *mov = mNode->getAttachedObject(0);
		if(mov->getMovableType() == "Ogre::Entity")
		{
			return static_cast<Ogre::Entity*>(mov)->getAnimationState(name);
		}
	}
	return NULL;
}
Ogre::AnimationStateSet *WorldArt::getAllAnimStates()
{
	/*switch(type)
	{
	};*/
	if(type == WT_MESH)
	{
		MovableObject *mov = mNode->getAttachedObject(0);
		if(mov->getMovableType() == "Ogre::Entity")
		{
			Ogre::Entity *ent = static_cast<Ogre::Entity*>(mov);
			Ogre::AnimationStateSet *res = ent->getAllAnimationStates();
			return res;
		}
	}
	return NULL;

}

//Ogre::String dotSceneObject::getProperty(const Ogre::String &ndNm, const Ogre::String &prop)
//{
//	for ( unsigned int i = 0 ; i < nodeProperties.size(); i++ )
//	{
//		if ( nodeProperties[i].nodeName == ndNm && nodeProperties[i].propertyNm == prop )
//		{
//			return nodeProperties[i].valueName;
//		}
//	}
//
//	return "";
//}

//void WorldArt::processUserDataReference(TiXmlElement *XMLNode, Ogre::Entity *pEntity)
//{
//	Ogre::String str = XMLNode->Attribute("id");
//	pEntity->setUserAny(Any(str));
//}