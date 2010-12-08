#ifndef __praDotSceneObject
#define __praDotSceneObject
#include "Ogre.h"
#include "FwDec.h"
#include "GameObject.h"
#include "Level.h"

using namespace Ogre;

class dotSceneObject: public GameObject
{
public:
	//default constructor. entire file, one node, one body
	dotSceneObject(String filename, Level *lvl,Vector3 pos,Quaternion orient = Quaternion::IDENTITY,Vector3 scale = Vector3::UNIT_SCALE);
	//alternative constructor. one <node> attribute. intended for the editor
	//dotSceneObject(TiXmlElement *XMLNode, Level *lvl,Vector3 pos,Quaternion orient = Quaternion::IDENTITY,Vector3 scale = Vector3::UNIT_SCALE);
	//constructor 3... ob das ganze so ne gute Idee ist?
	//dotSceneObject(Level *lvl,String mesh,Vector3 pos,String material = "",Quaternion orient = Quaternion::IDENTITY,Vector3 scale = Vector3::UNIT_SCALE);
	~dotSceneObject();
	GameObject* clone(Level *targetLvl = NULL){return NULL;}
private:
	//Level *mLevel;
	//OgreNewt::Body *mBody;
	//SceneNode *mSceneNode;
	String m_sPrependNode;
	Vector3 positionOffset;
	String m_sGroupName;
	//Quaternion orientationOffset;
protected:
	void finishConstructor();
	void processScene(TiXmlElement *XMLRoot);

	void processNodes(TiXmlElement *XMLNode);
	void processExternals(TiXmlElement *XMLNode);
	void processEnvironment(TiXmlElement *XMLNode);
	void processTerrain(TiXmlElement *XMLNode);
	void processUserDataReference(TiXmlElement *XMLNode, SceneNode *pParent = 0);
	void processUserDataReference(TiXmlElement *XMLNode, Entity *pEntity);
	void processOctree(TiXmlElement *XMLNode);
	void processLight(TiXmlElement *XMLNode, SceneNode *pParent = 0);
	void processCamera(TiXmlElement *XMLNode, SceneNode *pParent = 0);

	void processNode(TiXmlElement *XMLNode, SceneNode *pParent = 0);
	void processLookTarget(TiXmlElement *XMLNode, SceneNode *pParent);
	void processTrackTarget(TiXmlElement *XMLNode, SceneNode *pParent);
	void processEntity(TiXmlElement *XMLNode, SceneNode *pParent);
	void processParticleSystem(TiXmlElement *XMLNode, SceneNode *pParent);
	void processBillboardSet(TiXmlElement *XMLNode, SceneNode *pParent);
	void processPlane(TiXmlElement *XMLNode, SceneNode *pParent);

	void processFog(TiXmlElement *XMLNode);
	void processSkyBox(TiXmlElement *XMLNode);
	void processSkyDome(TiXmlElement *XMLNode);
	void processSkyPlane(TiXmlElement *XMLNode);
	void processClipping(TiXmlElement *XMLNode);

	void processLightRange(TiXmlElement *XMLNode, Light *pLight);
	void processLightAttenuation(TiXmlElement *XMLNode, Light *pLight);

	String getAttrib(TiXmlElement *XMLNode, const String &parameter, const String &defaultValue = "");
	Real getAttribReal(TiXmlElement *XMLNode, const String &parameter, Real defaultValue = 0);
	bool getAttribBool(TiXmlElement *XMLNode, const String &parameter, bool defaultValue = false);

	Vector3 parseVector3(TiXmlElement *XMLNode);
	Quaternion parseQuaternion(TiXmlElement *XMLNode);
	ColourValue parseColour(TiXmlElement *XMLNode);

};




#endif