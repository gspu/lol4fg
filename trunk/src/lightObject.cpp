#include "LightObject.h"
#include "StandardApplication.h"
#include "functions.h"
#include "Level.h"
using namespace Ogre;
void LightObject::init( Level *lvl, Ogre::Vector3 pos, Ogre::Quaternion ornt)//=Ogre::Quaternion::IDENTITY, Ogre::Vector3 _scale = Ogre::Vector3::UNIT_SCALE)
{
	isInLevelFile = true;
	SGID = 0;
//	meshEnt = NULL;
	mNode = NULL;
	mBody = NULL;
	mArt = NULL;
//	colType = 0;
//	collisionParams = Ogre::Vector3::UNIT_SCALE;
	ID = "";
	type = otLight; 
	
	mLevel = lvl;
	Ogre::String uName = "LevelLight";
	size_t i = 0;
	Ogre::SceneManager *mSceneMgr = mLevel->getSceneManager();
	while(mSceneMgr->hasLight(uName))
	{
		uName = "LevelLight"+Ogre::StringConverter::toString(i++);

	}
	mNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();

	mLight = mSceneMgr->createLight(uName);
	mLight->setType(Ogre::Light::LT_POINT);
	mLight->setUserAny(Any(this));
	
	/*mLight->setDiffuseColour(diffuse);
	mLight->setSpecularColour(specular);*/
	Ogre::ColourValue def = Ogre::ColourValue::White;
	mLight->setDiffuseColour(def);
	mNode->attachObject(mLight);





	mNode->setPosition(pos);
	mNode->setOrientation(ornt);

	mLevel->registerObject(this);	
}


LightObject::~LightObject()
{
	//mLevel->unregisterObject(this);
	////destroyNode(mLevel->getSceneManager(),mNode);
	//if(mArt)
	//	delete mArt;
	//destroyNode(mLevel->getSceneManager(),mNode);
	////mLevel->getSceneManager()->destroySceneNode(mNode->getName());
}
TiXmlElement* LightObject::getXmlElem(Ogre::Vector3 startPosition,bool savegame)
{
	Ogre::String strtype = objectTypeToString(type);
	if(strtype == "")
		return NULL;
	TiXmlElement* xmlElem = new TiXmlElement(strtype.c_str());
	Ogre::Vector3 pos, scale;
	Ogre::Quaternion ornt;
		
	scale = getScale();
	getPositionOrientation(pos,ornt);
		
	pos -= startPosition;
	xmlElem->SetAttribute("pos",Ogre::StringConverter::toString(pos).c_str());
	if(ornt != Ogre::Quaternion::IDENTITY)
	{
		xmlElem->SetAttribute("orient",Ogre::StringConverter::toString(ornt).c_str());
	}
	if(SGID)
	{
		xmlElem->SetAttribute("SGID",Ogre::StringConverter::toString(SGID).c_str());
	}
	
	
	
	LightObject *lo = (LightObject*)this;
	Ogre::Light *light = lo->getLight();
	Ogre::String strlighttype = "point";
	
	switch(light->getType())
	{					
	case Ogre::Light::LT_POINT:
		strlighttype = "point";
		break;
	case Ogre::Light::LT_DIRECTIONAL:
		strlighttype = "directional";
		break;
	case Ogre::Light::LT_SPOTLIGHT:
		strlighttype = "spotlight";
		break;
	}
	xmlElem->SetValue("light");
	xmlElem->SetAttribute("type",strlighttype.c_str());
	
	
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
	return xmlElem;
}

LightObject::LightObject( TiXmlElement *XMLNode, Level *lvl, Ogre::Vector3 positionOffset )
{
	
    Ogre::String id = "";
	Ogre::Vector3 pos = Ogre::StringConverter::parseVector3(XMLNode->Attribute("pos"))+positionOffset;
	Ogre::Vector3 scale = Ogre::Vector3::UNIT_SCALE;
	Ogre::Quaternion ornt = Ogre::Quaternion::IDENTITY;

			
	
	if(XMLNode->Attribute("orient"))
	{
		ornt = Ogre::StringConverter::parseQuaternion(XMLNode->Attribute("orient"));
	}
	
	if(XMLNode->Attribute("id"))
	{
		id = XMLNode->Attribute("id");
	}
	if(XMLNode->Attribute("scale"))
	{
		scale = Ogre::StringConverter::parseVector3(XMLNode->Attribute("scale"));
	}
	
	

	Ogre::ColourValue diffuse = Ogre::ColourValue::White;
	Ogre::ColourValue specular = Ogre::ColourValue::White;

	Ogre::Light::LightTypes type = Ogre::Light::LT_POINT;

	Ogre::String strtype = XMLNode->Attribute("type");
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


	if(XMLNode->Attribute("diffuse"))
		diffuse = Ogre::StringConverter::parseColourValue(XMLNode->Attribute("diffuse"));
	if(XMLNode->Attribute("specular"))
		specular = Ogre::StringConverter::parseColourValue(XMLNode->Attribute("specular"));


	init(lvl,pos,ornt);
	

	
	setLightType(type);
	mLight->setDiffuseColour(diffuse);
	mLight->setSpecularColour(specular);


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
	mLight->setAttenuation(aRange,aConst,aLinear,aQuad);

	if(XMLNode->Attribute("powerScale"))
	{
		Ogre::Real powerScale = Ogre::StringConverter::parseReal(XMLNode->Attribute("powerScale"));
		mLight->setPowerScale(powerScale);
	}
	if(type == Ogre::Light::LT_SPOTLIGHT)
	{
		//spotlight params
		if(XMLNode->Attribute("sInner"))
		{
			Ogre::Radian sInner = Ogre::StringConverter::parseAngle(XMLNode->Attribute("sInner"));
			mLight->setSpotlightInnerAngle(sInner);
		}
		if(XMLNode->Attribute("sOuter"))
		{
			Ogre::Radian sOuter = Ogre::StringConverter::parseAngle(XMLNode->Attribute("sOuter"));
			mLight->setSpotlightOuterAngle(sOuter);
		}
		if(XMLNode->Attribute("falloff"))
		{
			Ogre::Real falloff = Ogre::StringConverter::parseReal(XMLNode->Attribute("falloff"));
			mLight->setSpotlightFalloff(falloff);
		}
			
	}
}

LightObject::LightObject(LightObject *src,Level *lvl)
{
	SGID = 0;
	if(lvl)
	{
		mLevel = lvl;
	}
	else
	{
		mLevel = src->mLevel;
	}
	mNode = NULL;
	mBody = NULL;
	mArt = NULL;

	ID = "";
	type = otLight; 
	mColData.colType = CT_NOCOLLISION;
	
	Ogre::SceneManager *mgr = mLevel->getSceneManager();

	mNode = mgr->getRootSceneNode()->createChildSceneNode();
	
	Ogre::String name = getUniqueLightName(mgr);

	mLight = mgr->createLight(name);

	mNode->attachObject(mLight);
	mLight->setType(src->mLight->getType());

    mLight->setVisible(src->mLight->getVisible());
    mLight->setCastShadows(src->mLight->getCastShadows());
    mLight->setPosition(src->mLight->getPosition());
    mLight->setDirection(src->mLight->getDirection());

    mLight->setDiffuseColour(src->mLight->getDiffuseColour());
    mLight->setSpecularColour(src->mLight->getSpecularColour());

	Ogre::Real aRange  = src->mLight->getAttenuationRange();
	Ogre::Real aConst  = src->mLight->getAttenuationConstant();
	Ogre::Real aLinear = src->mLight->getAttenuationLinear();
	Ogre::Real aQuad   = src->mLight->getAttenuationQuadric();

	mLight->setAttenuation(aRange,aConst,aLinear,aQuad);

	mLight->setPowerScale(src->mLight->getPowerScale());

	if(mLight->getType() == Ogre::Light::LT_SPOTLIGHT)
	{
	    mLight->setSpotlightInnerAngle(src->mLight->getSpotlightInnerAngle());
	    mLight->setSpotlightOuterAngle(src->mLight->getSpotlightOuterAngle());
	    mLight->setSpotlightFalloff(src->mLight->getSpotlightFalloff());
	}

	mLight->setUserAny(Any(this));
	
	/*mLight->setDiffuseColour(diffuse);
	mLight->setSpecularColour(specular);*/

	if(src->mArt)
		showMesh();

	mNode->setPosition(src->mNode->getPosition());
	mNode->setOrientation(src->mNode->getOrientation());
	

	mLevel->registerObject(this);	

}


void LightObject::showMesh()
{
	hideMesh();
	
	Ogre::SceneManager *mSceneMgr = mLevel->getSceneManager();
	
	
	Ogre::String meshName;
	switch(mLight->getType())
	{
	case Ogre::Light::LT_DIRECTIONAL:
		meshName = "MultiPfeilMitBirne.mesh";
		break;
	case Ogre::Light::LT_SPOTLIGHT:
		meshName = "PfeilMitBirne.mesh";
		break;
	default:
		meshName = "Birne.mesh";	
	}
	

	Ogre::ColourValue def = mLight->getDiffuseColour();
	//def.a = 0.5;
	Ogre::String matName = "lightcolor"+Ogre::StringConverter::toString(def);
	ResourceManager::ResourceCreateOrRetrieveResult res = Ogre::MaterialManager::getSingletonPtr()->createOrRetrieve(matName,Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	Ogre::MaterialPtr mat = res.first; //Ogre::MaterialManager::getSingletonPtr()->create("lichttest"+uName,Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);//Ogre::MaterialManager::getSingletonPtr()->createOrRetrieve("lichttest","general",true);
	
	/*mat->getTechnique(0)->getPass(0)->setSceneBlending(SBT_MODULATE);
	mat->getTechnique(0)->getPass(0)->setAlphaRejectSettings(CMPF_GREATER,128);
	mat->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false); */
	
	if(res.second)
	{
		mat->setAmbient(def);
		mat->setDiffuse(def);
		mat->setSpecular(def);
		mat->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBT_REPLACE);
		mat->getTechnique(0)->getPass(0)->setSelfIllumination(def);
		updateSceneManagersAfterMaterialsChange();
	}
	
	WorldArtData wData;
	wData.filename = meshName;
	wData.material = matName;
	wData.shadow = false;
	wData.type = WT_MESH;
	mArt = new WorldArt(this,wData);
//	meshEnt->setMaterialName(matName);
//	meshEnt->setUserAny(Any((GameObject*)this));
	//Ogre::Material *mat = Ogre::MaterialManager::getSingletonPtr()->
	//ent->setMaterialName("Examples/RustySteel");
	
//	mNode->attachObject(meshEnt);
	mNode->setScale(Ogre::Vector3(0.5));
}
void LightObject::hideMesh()
{
	if(mArt)
	{
		delete mArt;
		//mNode->detachObject(meshEnt);
		//mLevel->getSceneManager()->destroyEntity(meshEnt);
	}
}

//void LightObject::update(Ogre::Real time)
//{
//}
//sets position

//sets orientation

//retrieves body position

//retr. body orient

//bool LightObject::getPositionOrientation(Ogre::Vector3 &pos, Ogre::Quaternion &orient)
//{
//	orient = getOrientation();
//	pos = getPosition();
//	return true;
//}
//


//void LightObject::setPositionOrientation(Ogre::Vector3 pos, Ogre::Quaternion ornt)
//{
//	setPosition(pos);
//	setOrientation(ornt);
//}

//void LightObject::setScale(Ogre::Vector3 scale)
//{
//}
//void LightObject::modScale(Ogre::Vector3 scale)
//{
//}
//Ogre::Vector3 LightObject::getScale()
//{
//	return Ogre::Vector3::UNIT_SCALE;
//}

//pointer to the level this obj is currently in
//Level *LightObject::getLevel()
//{
//	return mLevel;
//}
//ObjType LightObject::getType()
//{
//	return otLight;
//}
//
//Ogre::String LightObject::getID()
//{
//	return "";
//}
//Ogre::String LightObject::getUName()
//{
//	return uName;
//}
//Ogre::String LightObject::getName()
//{
//	return "";
//}
//Ogre::Light *LightObject::getLight()
//{
//	return mLight;
//}

void LightObject::setLightType(Ogre::Light::LightTypes lt)
{
	mLight->setType(lt);
	if(lt != Ogre::Light::LT_POINT)
	{
		mLight->setDirection(Ogre::Vector3::NEGATIVE_UNIT_Z);
	}
	
}
//Ogre::Light::LightTypes LightObject::getLightType()
//{
//	return mLight->getType();
//}

