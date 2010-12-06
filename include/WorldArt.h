#ifndef __praWorldArt
#define __praWorldArt
#include "FwDec.h"
#include "Ogre.h"
//#include "GameObject.h"
#include "tinyxml.h"
#include "OgreNewt.h"
#include "data.h"
//diese Klasse ist für die Darstellung von Objekten zuständig
//ob mesh, billboard oder dotscene

class WorldArt
{
public:
	friend class Level;

	
	/*enum WorldArtType
	{
		Mesh,
		DotScene,
		Billboard,
		Ogre::Plane
	};*/
	WorldArt(GameObject *par,WorldArtData wData, Ogre::SceneNode *nod = NULL , Ogre::Vector3 scale = Ogre::Vector3::UNIT_SCALE);
	//wenn nod == NULL, wird bei parent->mNode eine neue node erstellt 
	////mesh constructor
	//WorldArt(GameObject *par, Ogre::String mesh,Ogre::String material = "",Ogre::Vector3 scale = Ogre::Vector3::UNIT_SCALE, Ogre::Vector3 positionOffset = Ogre::Vector3::ZERO, Ogre::Quaternion rotationOffset = Ogre::Quaternion::IDENTITY, Ogre::SceneNode *nod = NULL);
	////billboardConstructor
	//WorldArt(GameObject *par, Ogre::String material, Ogre::Real width = 0, Ogre::Real height = 0,  Ogre::ColourValue cl = Ogre::ColourValue::White, Ogre::Vector3 positionOffset = Ogre::Vector3::ZERO,  Ogre::SceneNode *nod = NULL);
	////dotscene konstruktor
	//WorldArt(GameObject *par, Ogre::String filename,Ogre::Vector3 scale = Ogre::Vector3::UNIT_SCALE, Ogre::Vector3 positionOffset = Ogre::Vector3::ZERO, Ogre::Quaternion rotationOffset = Ogre::Quaternion::IDENTITY, Ogre::SceneNode *nod = NULL);
	////plane konstruktor
	//WorldArt(GameObject *par, Ogre::String material,PlaneData pData, Ogre::Vector3 scale = Ogre::Vector3::UNIT_SCALE, Ogre::Vector3 positionOffset = Ogre::Vector3::ZERO, Ogre::Quaternion rotationOffset = Ogre::Quaternion::IDENTITY, Ogre::SceneNode *nod = NULL);

	WorldArt(WorldArt *src, GameObject *target, Ogre::SceneNode *nod = NULL);
	//WorldArt(GameObject *par, Ogre::SceneNode *nod = NULL);
	~WorldArt();
	
	void loadXML(TiXmlElement *elem);

	void markSelected(bool mark);

	inline void setScale(Ogre::Vector3 scale)
	{
		mNode->setScale(getRotationOffset()*scale);
	}
	inline Ogre::Vector3 getScale()
	{
		return getRotationOffset().Inverse()*mNode->getScale();
	}
	inline void modScale(Ogre::Vector3 scale)
	{
		mNode->scale(getRotationOffset()*scale);
	}
	inline void setPositionOffset(Ogre::Vector3 pos)
	{
		mNode->setPosition(pos);
	}
	inline Ogre::Vector3 getPositionOffset()
	{
		return mNode->getPosition();
	}
	inline void setRotationOffset(Ogre::Quaternion ornt)
	{
		mNode->setOrientation(ornt);
	}
	inline Ogre::Quaternion getRotationOffset()
	{
		return mNode->getOrientation();
	}
	inline WorldArtType getType()
	{
		return type;
	}
	Ogre::AxisAlignedBox getBoundingBox();

	WorldArt *clone(GameObject *targetObj);

	OgreNewt::CollisionPtr getTreeCollision();
	//das holt die animstate der entity. später auch evtl custom
	Ogre::AnimationState *getAnimState(Ogre::String name);
	Ogre::AnimationStateSet *getAllAnimStates();//<- scheint verbuggt zu sein
	Ogre::SceneNode *getSceneNode()
	{
		return mNode;
	}
	WorldArtData getData()
	{
		return mData;
	}
private:
	GameObject *parent;
	Ogre::SceneNode *mNode;
	Ogre::String m_sGroupName;

	//später braucht das nur das game
	WorldArtData mData;
	
	WorldArtType type;
	/*Ogre::String data_Filename;
	Ogre::String data_Material;*/
protected:
	void loadMesh(Ogre::String mesh,Ogre::String material = ""/*,Ogre::Vector3 scale = Ogre::Vector3::UNIT_SCALE, Ogre::Vector3 positionOffset = Ogre::Vector3::ZERO, Ogre::Quaternion rotationOffset = Ogre::Quaternion::IDENTITY*/);
	void loadBillboard(Ogre::String material, Ogre::ColourValue cl = Ogre::ColourValue::White/*, Ogre::Real width = 0, Ogre::Real height = 0, Ogre::Vector3 positionOffset = Ogre::Vector3::ZERO*/);
	void loadDotScene(Ogre::String filename/*,Ogre::Vector3 scale = Ogre::Vector3::UNIT_SCALE, Ogre::Vector3 positionOffset = Ogre::Vector3::ZERO, Ogre::Quaternion rotationOffset = Ogre::Quaternion::IDENTITY*/);
	void loadPlane(Ogre::String material,PlaneData pData/*, Ogre::Vector3 scale = Ogre::Vector3::UNIT_SCALE, Ogre::Vector3 positionOffset = Ogre::Vector3::ZERO, Ogre::Quaternion rotationOffset = Ogre::Quaternion::IDENTITY*/);
	void loadParticleSystem(Ogre::String name/*, Ogre::Vector3 scale = Ogre::Vector3::UNIT_SCALE, Ogre::Vector3 positionOffset = Ogre::Vector3::ZERO, Ogre::Quaternion rotationOffset = Ogre::Quaternion::IDENTITY*/);
	//void load
	void processWorldArtData(WorldArtData data);


	void ds_finishConstructor();
	void ds_processScene(TiXmlElement *XMLRoot);

	void ds_processNodes(TiXmlElement *XMLNode);
	//void ds_processExternals(TiXmlElement *XMLNode);
	//void ds_processEnvironment(TiXmlElement *XMLNode);
	//void ds_processTerrain(TiXmlElement *XMLNode);
	//void ds_processUserDataReference(TiXmlElement *XMLNode, Ogre::SceneNode *pParent = 0);
	//void ds_processUserDataReference(TiXmlElement *XMLNode, Ogre::Entity *pEntity);
	//void ds_processOctree(TiXmlElement *XMLNode);
	void ds_processLight(TiXmlElement *XMLNode, Ogre::SceneNode *pParent = 0);
	//void ds_processCamera(TiXmlElement *XMLNode, Ogre::SceneNode *pParent = 0);

	void ds_processNode(TiXmlElement *XMLNode, Ogre::SceneNode *pParent = 0);
	//void ds_processLookTarget(TiXmlElement *XMLNode, Ogre::SceneNode *pParent);
	//void ds_processTrackTarget(TiXmlElement *XMLNode, Ogre::SceneNode *pParent);
	void ds_processEntity(TiXmlElement *XMLNode, Ogre::SceneNode *pParent);
	void ds_processParticleSystem(TiXmlElement *XMLNode, Ogre::SceneNode *pParent);
	void ds_processBillboardSet(TiXmlElement *XMLNode, Ogre::SceneNode *pParent);
	void ds_processPlane(TiXmlElement *XMLNode, Ogre::SceneNode *pParent);

	//void ds_processFog(TiXmlElement *XMLNode);
	//void ds_processSkyBox(TiXmlElement *XMLNode);
	//void ds_processSkyDome(TiXmlElement *XMLNode);
	//void ds_processSkyPlane(TiXmlElement *XMLNode);
	//void ds_processClipping(TiXmlElement *XMLNode);

	void ds_processLightRange(TiXmlElement *XMLNode, Ogre::Light *pLight);
	void ds_processLightAttenuation(TiXmlElement *XMLNode, Ogre::Light *pLight);

	Ogre::String getAttrib(TiXmlElement *XMLNode, const Ogre::String &parameter, const Ogre::String &defaultValue = "");
	Ogre::Real getAttribReal(TiXmlElement *XMLNode, const Ogre::String &parameter, Ogre::Real defaultValue = 0);
	bool getAttribBool(TiXmlElement *XMLNode, const Ogre::String &parameter, bool defaultValue = false);

	Ogre::Vector3 ds_parseVector3(TiXmlElement *XMLNode);
	Ogre::Quaternion ds_parseQuaternion(TiXmlElement *XMLNode);
	Ogre::ColourValue ds_parseColour(TiXmlElement *XMLNode);
};

#endif