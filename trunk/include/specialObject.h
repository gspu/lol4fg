#ifndef __praSpecialObject
#define __praSpecialObject

#include "Ogre.h"

#include "FwDec.h"

#include "GameObject.h"
#include "StandardApplication.h"

using namespace Ogre;

class SpecialObject: public GameObject
{
public:
	//default constructor
	SpecialObject(TiXmlElement *XMLNode,Level *lvl, Vector3 positionOffset = Vector3::ZERO);
	SpecialObject(gamedata_special *data, Level *lvl, Vector3 pos, Quaternion ornt=Quaternion::IDENTITY, Vector3 _scale = Vector3::UNIT_SCALE)
	{
		init(data,lvl,pos,ornt,_scale);
	}
	SpecialObject(Ogre::String id, Level *lvl, Vector3 pos, Quaternion ornt=Quaternion::IDENTITY, Vector3 _scale = Vector3::UNIT_SCALE)
	{		
		gamedata_special *myData = StandardApplication::getSingletonPtr()->getSpecialDataPtr(id);
		if(!myData || myData->id == "")
		{
			throw etInvalidId;
		}
		init(myData,lvl,pos,ornt,_scale);
	}
	//copy constructor
	//if lvl = NULL, same level as src will be used
	SpecialObject(SpecialObject *src, Level *lvl = NULL);
	GameObject* clone(Level *targetLvl = NULL)
	{
		return static_cast<GameObject*>(new SpecialObject(this,targetLvl));
	}
	virtual ~SpecialObject();

	/*void update(Real time);*/
	//sets position
	void setPosition(Vector3 pos);
	void setOrientation(Quaternion ornt);
	void setPositionOrientation(Vector3 pos,Quaternion ornt);

	void setScale(Vector3 scale);
	void modScale(Vector3 scale);	
	virtual TiXmlElement* getXmlElem(Vector3 startPosition = Vector3::ZERO,bool savegame=false);
	void rotate(const Quaternion& q, Node::TransformSpace relativeTo = Node::TS_LOCAL);
	void rotate(const Vector3& axis, const Radian& angle, Node::TransformSpace relativeTo = Node::TS_LOCAL);
	////sets orientation
	//void setOrientation(Quaternion ornt);
	////retrieves body position
	//Vector3 getPosition();
	////retr. body orient
	//Quaternion getOrientation();
	//void setScale(Vector3 scale);
	//void modScale(Vector3 scale);
	//Vector3 getScale();
	//OgreNewt::Body *getBody();
	//SceneNode *getSceneNode();
	//pointer to the level this obj is currently in
	/*Level *getLevel();
	ObjType getType();
	String getID();
	String getUName();*/
	/*String getName();*/
	void updateBody();


protected:
	void init(gamedata_special *data, Level *lvl, Vector3 pos, Quaternion ornt=Quaternion::IDENTITY, Vector3 _scale = Vector3::UNIT_SCALE,int SaveGameID = 0);
	void processSubObject(gamedata_special data, OgreNewt::Body *parent,Vector3 scale = Vector3::UNIT_SCALE);
	//gamedata_special *myData;
	struct SubObject
	{
		OgreNewt::Body* mBody;
		WorldArt* mArt;
		CollisionData colData;
		//ein pointer auf einen eintrag im "subObjects" vector seines parents
		joint_data jData;
		OgreNewt::Body *parent;
	};
	//typedef std::list<OgreNewt::Body*> BodyList;
	typedef std::vector<SubObject> SubObjectList;
	typedef std::vector<OgreNewt::Joint*> JointList;
	SubObjectList subObjectList;
	//BodyList subBodyList;
	JointList jointList;
	
	//gamedata_obj_static *myData;
	//OgreNewt::Body *mBody;
	//SceneNode *mNode;
	//ObjType type; //<- später evtl als enum
	//String mID;
	//String uName;
	//Level *mLevel;

};

#endif