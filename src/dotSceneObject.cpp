#include "dotSceneObject.h"
#include "tinyxml.h"
#include "functions.h"

dotSceneObject::dotSceneObject(Ogre::String filename, Level *lvl, Ogre::Vector3 pos, Ogre::Quaternion orient, Ogre::Vector3 scale)
{
	TiXmlDocument   *XMLDoc = 0;
	TiXmlElement   *XMLRoot;
	mLevel = lvl;
	ID = filename;

	baseScale = Vector3::UNIT_SCALE;
	positionOffset = Vector3::ZERO;
	//OrientationOffset = Quatermion::
//	collisionParams = Vector3::UNIT_SCALE;
	colType = CT_TREECOLLISIONSCENE;
	m_sGroupName = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;

	
	type = GameObject::otDotScene;
	/*String uName;*/
	

	try
	{
		DataStreamPtr pStream = ResourceGroupManager::getSingleton().
			openResource( filename,m_sGroupName );

		String data = pStream->getAsString();
		// Open the .scene File
		XMLDoc = new TiXmlDocument();
		XMLDoc->Parse( data.c_str() );
		pStream->close();
		pStream.setNull();

		if( XMLDoc->Error() )
		{
			//We'll just log, and continue on gracefully
			LogManager::getSingleton().logMessage("[dotSceneObject] The TiXmlDocument reported an error");
			delete XMLDoc;
			//return;
			throw EX_CONSTR_FAILED;
		}
	}
	catch(...)
	{
		//We'll just log, and continue on gracefully
		LogManager::getSingleton().logMessage("[dotSceneObject] Error creating TiXmlDocument");
		delete XMLDoc;
		//return;
		throw EX_CONSTR_FAILED;
	}
	// Validate the File
	XMLRoot = XMLDoc->RootElement();
	if( String( XMLRoot->Value()) != "scene"  ) {
		LogManager::getSingleton().logMessage( "[dotSceneObject] Error: Invalid .scene File. Missing <scene>" );
		delete XMLDoc;      
		//return;
		throw EX_CONSTR_FAILED;
	}

	mNode = mLevel->getSceneManager()->getRootSceneNode()->createChildSceneNode();
	

	// Process the scene
	processScene(XMLRoot);

	mNode->scale(scale);	



	// Close the XML File
	delete XMLDoc;
	mBody = makeBody(mLevel->getWorld(),colType,0, Vector3::ZERO,mNode);
	//if(colType != CT_TREECOLLISION && colType != CT_TREECOLLISIONSCENE)
	//	mBody->setCustomForceAndTorqueCallback<Level>(&Level::LevelForceCallback,mLevel);  // add the previous defined callback function as the body custom force and torque callback
	mBody->setPositionOrientation(pos,orient);
	mLevel->registerObject(this);
}

void dotSceneObject::finishConstructor()
{
	
}


dotSceneObject::~dotSceneObject()
{
	mLevel->unregisterObject(this);
	delete mBody;
	//destroyNode(mLevel->getSceneManager(),mNode);
	if(mArt)
		delete mArt;
	mSceneMgr->destroySceneNode(mNode->getName());
}

void dotSceneObject::processScene(TiXmlElement *XMLRoot)
{
	// Process the scene parameters
	String version = getAttrib(XMLRoot, "formatVersion", "unknown");

	String message = "[dotSceneObject] Parsing dotScene file with version " + version;
	if(XMLRoot->Attribute("ID"))
		message += ", id " + String(XMLRoot->Attribute("ID"));
	if(XMLRoot->Attribute("sceneManager"))
		message += ", scene manager " + String(XMLRoot->Attribute("sceneManager"));
	if(XMLRoot->Attribute("minOgreVersion"))
		message += ", min. Ogre version " + String(XMLRoot->Attribute("minOgreVersion"));
	if(XMLRoot->Attribute("author"))
		message += ", author " + String(XMLRoot->Attribute("author"));

	LogManager::getSingleton().logMessage(message);

	TiXmlElement *pElement;

	// Process nodes (?)
	pElement = XMLRoot->FirstChildElement("nodes");
	if(pElement)
		processNodes(pElement);

	// Process externals (?)
	pElement = XMLRoot->FirstChildElement("externals");
	if(pElement)
		processExternals(pElement);

	// Process environment (?)
	pElement = XMLRoot->FirstChildElement("environment");
	if(pElement)
		processEnvironment(pElement);

	// Process terrain (?)
	pElement = XMLRoot->FirstChildElement("terrain");
	if(pElement)
		processTerrain(pElement);

	// Process userDataReference (?)
	pElement = XMLRoot->FirstChildElement("userDataReference");
	if(pElement)
		processUserDataReference(pElement);

	// Process octree (?)
	pElement = XMLRoot->FirstChildElement("octree");
	if(pElement)
		processOctree(pElement);

	// Process light (?)
	pElement = XMLRoot->FirstChildElement("light");
	if(pElement)
		processLight(pElement);

	// Process camera (?)
	pElement = XMLRoot->FirstChildElement("camera");
	if(pElement)
		processCamera(pElement);
}


void dotSceneObject::processNodes(TiXmlElement *XMLNode)
{
	TiXmlElement *pElement;

	// Process node (*)
	pElement = XMLNode->FirstChildElement("node");
	while(pElement)
	{
		processNode(pElement);
		pElement = pElement->NextSiblingElement("node");
	}

	// Process position (?)
	pElement = XMLNode->FirstChildElement("position");
	if(pElement)
	{
		//mAttachNode->setPosition(parseVector3(pElement));
		positionOffset = parseVector3(pElement);
		//mAttachNode->setInitialState();
		//mNode->setInitialState();
	}

	// Process rotation (?)
	pElement = XMLNode->FirstChildElement("rotation");
	if(pElement)
	{
		//mAttachNode->setOrientation(parseQuaternion(pElement));
		//mAttachNode->setInitialState();
		mNode->setOrientation(parseQuaternion(pElement));
		mNode->setInitialState();
	}

	// Process scale (?)
	pElement = XMLNode->FirstChildElement("scale");
	if(pElement)
	{
		mNode->setScale(parseVector3(pElement));
		mNode->setInitialState();
	}
}

void dotSceneObject::processExternals(TiXmlElement *XMLNode)
{
	//! @todo Implement this
}

void dotSceneObject::processEnvironment(TiXmlElement *XMLNode)
{
	TiXmlElement *pElement;

	// Process fog (?)
	pElement = XMLNode->FirstChildElement("fog");
	if(pElement)
		processFog(pElement);

	// Process skyBox (?)
	pElement = XMLNode->FirstChildElement("skyBox");
	if(pElement)
		processSkyBox(pElement);

	// Process skyDome (?)
	pElement = XMLNode->FirstChildElement("skyDome");
	if(pElement)
		processSkyDome(pElement);

	// Process skyPlane (?)
	pElement = XMLNode->FirstChildElement("skyPlane");
	if(pElement)
		processSkyPlane(pElement);

	// Process clipping (?)
	pElement = XMLNode->FirstChildElement("clipping");
	if(pElement)
		processClipping(pElement);

	// Process colourAmbient (?)
	pElement = XMLNode->FirstChildElement("colourAmbient");
	if(pElement)
		mLevel->getSceneManager()->setAmbientLight(parseColour(pElement));

	//// Process colourBackground (?)
	////! @todo Set the background colour of all viewports (RenderWindow has to be provided then)
	//pElement = XMLNode->FirstChildElement("colourBackground");
	//if(pElement)
	//	;//mLevel->getSceneManager()->set(parseColour(pElement));

	// Process userDataReference (?)
	pElement = XMLNode->FirstChildElement("userDataReference");
	if(pElement)
		processUserDataReference(pElement);
}

void dotSceneObject::processTerrain(TiXmlElement *XMLNode)
{
	//! @todo Implement this
}

void dotSceneObject::processUserDataReference(TiXmlElement *XMLNode, SceneNode *pParent)
{
	//! @todo Implement this
}

void dotSceneObject::processOctree(TiXmlElement *XMLNode)
{
	//! @todo Implement this
}

void dotSceneObject::processLight(TiXmlElement *XMLNode, SceneNode *pParent)
{
	// Process attributes
	String name = getAttrib(XMLNode, "name");
	String id = getAttrib(XMLNode, "id");

	// Create the light
	Light *pLight = mLevel->getSceneManager()->createLight(name);
	if(pParent)
		pParent->attachObject(pLight);
	else
		mNode->attachObject(pLight);

	String sValue = getAttrib(XMLNode, "type");
	if(sValue == "point")
		pLight->setType(Light::LT_POINT);
	else if(sValue == "directional")
		pLight->setType(Light::LT_DIRECTIONAL);
	else if(sValue == "spot")
		pLight->setType(Light::LT_SPOTLIGHT);
	else if(sValue == "radPoint")
		pLight->setType(Light::LT_POINT);

	pLight->setVisible(getAttribBool(XMLNode, "visible", true));
	pLight->setCastShadows(getAttribBool(XMLNode, "castShadows", true));

	TiXmlElement *pElement;

	// Process position (?)
	pElement = XMLNode->FirstChildElement("position");
	if(pElement)
		pLight->setPosition(parseVector3(pElement));

	// Process normal (?)
	pElement = XMLNode->FirstChildElement("normal");
	if(pElement)
		pLight->setDirection(parseVector3(pElement));

	// Process colourDiffuse (?)
	pElement = XMLNode->FirstChildElement("colourDiffuse");
	if(pElement)
		pLight->setDiffuseColour(parseColour(pElement));

	// Process colourSpecular (?)
	pElement = XMLNode->FirstChildElement("colourSpecular");
	if(pElement)
		pLight->setSpecularColour(parseColour(pElement));

	// Process lightRange (?)
	pElement = XMLNode->FirstChildElement("lightRange");
	if(pElement)
		processLightRange(pElement, pLight);

	// Process lightAttenuation (?)
	pElement = XMLNode->FirstChildElement("lightAttenuation");
	if(pElement)
		processLightAttenuation(pElement, pLight);

	pLight->setUserAny(Any((GameObject*)this));

	//// Process userDataReference (?)
	//pElement = XMLNode->FirstChildElement("userDataReference");
	//if(pElement)
	//	;//processUserDataReference(pElement, pLight);
}

void dotSceneObject::processCamera(TiXmlElement *XMLNode, SceneNode *pParent)
{
	//! @todo Implement this
}

void dotSceneObject::processNode(TiXmlElement *XMLNode, SceneNode *pParent)
{
	// Construct the node's name
	String name = m_sPrependNode + getAttrib(XMLNode, "name");

	// Create the scene node
	SceneNode *pNode;
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
	String id = getAttrib(XMLNode, "id");
	bool isTarget = getAttribBool(XMLNode, "isTarget");

	TiXmlElement *pElement;

	// Process position (?)
	pElement = XMLNode->FirstChildElement("position");
	if(pElement)
	{
		pNode->setPosition(parseVector3(pElement));
		pNode->setInitialState();
	}

	// Process rotation (?)
	pElement = XMLNode->FirstChildElement("rotation");
	if(pElement)
	{
		pNode->setOrientation(parseQuaternion(pElement));
		pNode->setInitialState();
	}

	// Process scale (?)
	pElement = XMLNode->FirstChildElement("scale");
	if(pElement)
	{
		pNode->setScale(parseVector3(pElement));
		pNode->setInitialState();
	}

	// Process lookTarget (?)
	pElement = XMLNode->FirstChildElement("lookTarget");
	if(pElement)
		processLookTarget(pElement, pNode);

	// Process trackTarget (?)
	pElement = XMLNode->FirstChildElement("trackTarget");
	if(pElement)
		processTrackTarget(pElement, pNode);

	// Process node (*)
	pElement = XMLNode->FirstChildElement("node");
	while(pElement)
	{
		processNode(pElement, pNode);
		pElement = pElement->NextSiblingElement("node");
	}

	// Process entity (*)
	pElement = XMLNode->FirstChildElement("entity");
	while(pElement)
	{
		processEntity(pElement, pNode);
		pElement = pElement->NextSiblingElement("entity");
	}

	// Process light (*)
	pElement = XMLNode->FirstChildElement("light");
	while(pElement)
	{
		processLight(pElement, pNode);
		pElement = pElement->NextSiblingElement("light");
	}

	// Process camera (*)
	pElement = XMLNode->FirstChildElement("camera");
	while(pElement)
	{
		processCamera(pElement, pNode);
		pElement = pElement->NextSiblingElement("camera");
	}

	// Process particleSystem (*)
	pElement = XMLNode->FirstChildElement("particleSystem");
	while(pElement)
	{
		processParticleSystem(pElement, pNode);
		pElement = pElement->NextSiblingElement("particleSystem");
	}

	// Process billboardSet (*)
	pElement = XMLNode->FirstChildElement("billboardSet");
	while(pElement)
	{
		processBillboardSet(pElement, pNode);
		pElement = pElement->NextSiblingElement("billboardSet");
	}

	// Process plane (*)
	pElement = XMLNode->FirstChildElement("plane");
	while(pElement)
	{
		processPlane(pElement, pNode);
		pElement = pElement->NextSiblingElement("plane");
	}

	// Process userDataReference (?)
	pElement = XMLNode->FirstChildElement("userDataReference");
	if(pElement)
		processUserDataReference(pElement, pNode);
}

void dotSceneObject::processLookTarget(TiXmlElement *XMLNode, SceneNode *pParent)
{
	//! @todo Is this correct? Cause I don't have a clue actually

	// Process attributes
	String nodeName = getAttrib(XMLNode, "nodeName");

	Node::TransformSpace relativeTo = Node::TS_PARENT;
	String sValue = getAttrib(XMLNode, "relativeTo");
	if(sValue == "local")
		relativeTo = Node::TS_LOCAL;
	else if(sValue == "parent")
		relativeTo = Node::TS_PARENT;
	else if(sValue == "world")
		relativeTo = Node::TS_WORLD;

	TiXmlElement *pElement;

	// Process position (?)
	Vector3 position;
	pElement = XMLNode->FirstChildElement("position");
	if(pElement)
		position = parseVector3(pElement);

	// Process localDirection (?)
	Vector3 localDirection = Vector3::NEGATIVE_UNIT_Z;
	pElement = XMLNode->FirstChildElement("localDirection");
	if(pElement)
		localDirection = parseVector3(pElement);

	// Setup the look target
	try
	{
		if(!nodeName.empty())
		{
			SceneNode *pLookNode = mLevel->getSceneManager()->getSceneNode(nodeName);
			position = pLookNode->_getDerivedPosition();
		}

		pParent->lookAt(position, relativeTo, localDirection);
	}
	catch(Ogre::Exception &/*e*/)
	{
		LogManager::getSingleton().logMessage("[dotSceneObject] Error processing a look target!");
	}
}

void dotSceneObject::processTrackTarget(TiXmlElement *XMLNode, SceneNode *pParent)
{
	// Process attributes
	String nodeName = getAttrib(XMLNode, "nodeName");

	TiXmlElement *pElement;

	// Process localDirection (?)
	Vector3 localDirection = Vector3::NEGATIVE_UNIT_Z;
	pElement = XMLNode->FirstChildElement("localDirection");
	if(pElement)
		localDirection = parseVector3(pElement);

	// Process offset (?)
	Vector3 offset = Vector3::ZERO;
	pElement = XMLNode->FirstChildElement("offset");
	if(pElement)
		offset = parseVector3(pElement);

	// Setup the track target
	try
	{
		SceneNode *pTrackNode = mLevel->getSceneManager()->getSceneNode(nodeName);
		pParent->setAutoTracking(true, pTrackNode, localDirection, offset);
	}
	catch(Ogre::Exception &/*e*/)
	{
		LogManager::getSingleton().logMessage("[dotSceneObject] Error processing a track target!");
	}
}

void dotSceneObject::processEntity(TiXmlElement *XMLNode, SceneNode *pParent)
{
	// Process attributes
	String name = getAttrib(XMLNode, "name");
	String id = getAttrib(XMLNode, "id");
	String meshFile = getAttrib(XMLNode, "meshFile");
	String materialFile = getAttrib(XMLNode, "materialFile");
	bool isStatic = getAttribBool(XMLNode, "static", false);;
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
	Entity *pEntity = 0;
	try
	{
		MeshManager::getSingleton().load(meshFile , m_sGroupName);
		pEntity = mLevel->getSceneManager()->createEntity(name, meshFile);
		pEntity->setCastShadows(castShadows);
		pParent->attachObject(pEntity);

		pEntity->setUserAny(Any((GameObject*)this));
		
		if(!materialFile.empty())
			pEntity->setMaterialName(materialFile);
	}
	catch(Ogre::Exception &/*e*/)
	{
		LogManager::getSingleton().logMessage("[dotSceneObject] Error loading an entity!");
	}

	// Process userDataReference (?)
	pElement = XMLNode->FirstChildElement("userDataReference");
	if(pElement)
		processUserDataReference(pElement, pEntity);
	pEntity->setUserAny(Any((GameObject*)this));

	
}

void dotSceneObject::processParticleSystem(TiXmlElement *XMLNode, SceneNode *pParent)
{
	// Process attributes
	String name = getAttrib(XMLNode, "name");
	String id = getAttrib(XMLNode, "id");
	String file = getAttrib(XMLNode, "file");

	// Create the particle system
	try
	{
		ParticleSystem *pParticles = mLevel->getSceneManager()->createParticleSystem(name, file);
		pParent->attachObject(pParticles);
		pParticles->setUserAny(Any((GameObject*)this));
	}
	catch(Ogre::Exception &/*e*/)
	{
		LogManager::getSingleton().logMessage("[dotSceneObject] Error creating a particle system!");
	}
}

void dotSceneObject::processBillboardSet(TiXmlElement *XMLNode, SceneNode *pParent)
{
	//! @todo Implement this
}

void dotSceneObject::processPlane(TiXmlElement *XMLNode, SceneNode *pParent)
{
	//! @todo Implement this
}

void dotSceneObject::processFog(TiXmlElement *XMLNode)
{
	//// Process attributes
	//Real expDensity = getAttribReal(XMLNode, "expDensity", 0.001);
	//Real linearStart = getAttribReal(XMLNode, "linearStart", 0.0);
	//Real linearEnd = getAttribReal(XMLNode, "linearEnd", 1.0);

	//FogMode mode = FOG_NONE;
	//String sMode = getAttrib(XMLNode, "mode");
	//if(sMode == "none")
	//	mode = FOG_NONE;
	//else if(sMode == "exp")
	//	mode = FOG_EXP;
	//else if(sMode == "exp2")
	//	mode = FOG_EXP2;
	//else if(sMode == "linear")
	//	mode = FOG_LINEAR;

	//TiXmlElement *pElement;

	//// Process colourDiffuse (?)
	//ColourValue colourDiffuse = ColourValue::White;
	//pElement = XMLNode->FirstChildElement("colourDiffuse");
	//if(pElement)
	//	colourDiffuse = parseColour(pElement);

	//// Setup the fog
	//mLevel->getSceneManager()->setFog(mode, colourDiffuse, expDensity, linearStart, linearEnd);
}

void dotSceneObject::processSkyBox(TiXmlElement *XMLNode)
{
	//// Process attributes
	//String material = getAttrib(XMLNode, "material");
	//Real distance = getAttribReal(XMLNode, "distance", 5000);
	//bool drawFirst = getAttribBool(XMLNode, "drawFirst", true);

	//TiXmlElement *pElement;

	//// Process rotation (?)
	//Quaternion rotation = Quaternion::IDENTITY;
	//pElement = XMLNode->FirstChildElement("rotation");
	//if(pElement)
	//	rotation = parseQuaternion(pElement);

	//// Setup the sky box
	//mLevel->getSceneManager()->setSkyBox(true, material, distance, drawFirst, rotation, m_sGroupName);
}

void dotSceneObject::processSkyDome(TiXmlElement *XMLNode)
{
	//// Process attributes
	//String material = XMLNode->Attribute("material");
	//Real curvature = getAttribReal(XMLNode, "curvature", 10);
	//Real tiling = getAttribReal(XMLNode, "tiling", 8);
	//Real distance = getAttribReal(XMLNode, "distance", 4000);
	//bool drawFirst = getAttribBool(XMLNode, "drawFirst", true);

	//TiXmlElement *pElement;

	//// Process rotation (?)
	//Quaternion rotation = Quaternion::IDENTITY;
	//pElement = XMLNode->FirstChildElement("rotation");
	//if(pElement)
	//	rotation = parseQuaternion(pElement);

	//// Setup the sky dome
	//mLevel->getSceneManager()->setSkyDome(true, material, curvature, tiling, distance, drawFirst, rotation, 16, 16, -1, m_sGroupName);
}

void dotSceneObject::processSkyPlane(TiXmlElement *XMLNode)
{
	//// Process attributes
	//String material = getAttrib(XMLNode, "material");
	//Real planeX = getAttribReal(XMLNode, "planeX", 0);
	//Real planeY = getAttribReal(XMLNode, "planeY", -1);
	//Real planeZ = getAttribReal(XMLNode, "planeX", 0);
	//Real planeD = getAttribReal(XMLNode, "planeD", 5000);
	//Real scale = getAttribReal(XMLNode, "scale", 1000);
	//Real bow = getAttribReal(XMLNode, "bow", 0);
	//Real tiling = getAttribReal(XMLNode, "tiling", 10);
	//bool drawFirst = getAttribBool(XMLNode, "drawFirst", true);

	//// Setup the sky plane
	//Plane plane;
	//plane.normal = Vector3(planeX, planeY, planeZ);
	//plane.d = planeD;
	//mLevel->getSceneManager()->setSkyPlane(true, plane, material, scale, tiling, drawFirst, bow, 1, 1, m_sGroupName);
}

void dotSceneObject::processClipping(TiXmlElement *XMLNode)
{
	//! @todo Implement this

	// Process attributes
	Real fNear = getAttribReal(XMLNode, "near", 0);
	Real fFar = getAttribReal(XMLNode, "far", 1);
}

void dotSceneObject::processLightRange(TiXmlElement *XMLNode, Light *pLight)
{
	// Process attributes
	Real inner = getAttribReal(XMLNode, "inner");
	Real outer = getAttribReal(XMLNode, "outer");
	Real falloff = getAttribReal(XMLNode, "falloff", 1.0);

	// Setup the light range
	pLight->setSpotlightRange(Angle(inner), Angle(outer), falloff);
}

void dotSceneObject::processLightAttenuation(TiXmlElement *XMLNode, Light *pLight)
{
	// Process attributes
	Real range = getAttribReal(XMLNode, "range");
	Real constant = getAttribReal(XMLNode, "constant");
	Real linear = getAttribReal(XMLNode, "linear");
	Real quadratic = getAttribReal(XMLNode, "quadratic");

	// Setup the light attenuation
	pLight->setAttenuation(range, constant, linear, quadratic);
}


String dotSceneObject::getAttrib(TiXmlElement *XMLNode, const String &attrib, const String &defaultValue)
{
	
	if(XMLNode->Attribute(attrib.c_str()))
		return XMLNode->Attribute(attrib.c_str());
	else
		return defaultValue;
}

Real dotSceneObject::getAttribReal(TiXmlElement *XMLNode, const String &attrib, Real defaultValue)
{
	if(XMLNode->Attribute(attrib.c_str()))
		return StringConverter::parseReal(XMLNode->Attribute(attrib.c_str()));
	else
		return defaultValue;
}

bool dotSceneObject::getAttribBool(TiXmlElement *XMLNode, const String &attrib, bool defaultValue)
{
	if(!XMLNode->Attribute(attrib.c_str()))
		return defaultValue;

	if(String(XMLNode->Attribute(attrib.c_str())) == "true")
		return true;

	return false;
}


Vector3 dotSceneObject::parseVector3(TiXmlElement *XMLNode)
{
	return Vector3(
		StringConverter::parseReal(XMLNode->Attribute("x")),
		StringConverter::parseReal(XMLNode->Attribute("y")),
		StringConverter::parseReal(XMLNode->Attribute("z"))
	);
}

Quaternion dotSceneObject::parseQuaternion(TiXmlElement *XMLNode)
{
	//! @todo Fix this shit!

	Quaternion orientation;

	if(XMLNode->Attribute("qx"))
	{
		orientation.x = StringConverter::parseReal(XMLNode->Attribute("qx"));
		orientation.y = StringConverter::parseReal(XMLNode->Attribute("qy"));
		orientation.z = StringConverter::parseReal(XMLNode->Attribute("qz"));
		orientation.w = StringConverter::parseReal(XMLNode->Attribute("qw"));
	}
	else if(XMLNode->Attribute("axisX"))
	{
		Vector3 axis;
		axis.x = StringConverter::parseReal(XMLNode->Attribute("axisX"));
		axis.y = StringConverter::parseReal(XMLNode->Attribute("axisY"));
		axis.z = StringConverter::parseReal(XMLNode->Attribute("axisZ"));
		Real angle = StringConverter::parseReal(XMLNode->Attribute("angle"));;
		orientation.FromAngleAxis(Ogre::Angle(angle), axis);
	}
	else if(XMLNode->Attribute("angleX"))
	{
		Vector3 axis;
		axis.x = StringConverter::parseReal(XMLNode->Attribute("angleX"));
		axis.y = StringConverter::parseReal(XMLNode->Attribute("angleY"));
		axis.z = StringConverter::parseReal(XMLNode->Attribute("angleZ"));
		//orientation.FromAxes(&axis);
		//orientation.F
	}

	return orientation;
}

ColourValue dotSceneObject::parseColour(TiXmlElement *XMLNode)
{
	return ColourValue(
		StringConverter::parseReal(XMLNode->Attribute("r")),
		StringConverter::parseReal(XMLNode->Attribute("g")),
		StringConverter::parseReal(XMLNode->Attribute("b")),
		XMLNode->Attribute("a") != NULL ? StringConverter::parseReal(XMLNode->Attribute("a")) : 1
	);
}

//String dotSceneObject::getProperty(const String &ndNm, const String &prop)
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

void dotSceneObject::processUserDataReference(TiXmlElement *XMLNode, Entity *pEntity)
{
	String str = XMLNode->Attribute("id");
	pEntity->setUserAny(Any(str));
}