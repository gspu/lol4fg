#ifndef __praLightObject
#define __praLightObject

#include "Ogre.h"
#include "FwDec.h"
#include "GameObject.h"
//#include "StandardApplication.h"



class LightObject: public GameObject
{
public:
	//LightObject( Level *lvl, Ogre::Light::LightTypes lType,/*,Ogre::ColourValue specular, Ogre::ColourValue diffuse,*/ Ogre::Vector3 pos = Ogre::Vector3::ZERO, Ogre::Vector3 dir = Ogre::Vector3::NEGATIVE_UNIT_Z);//, Ogre::Quaternion ornt=Ogre::Quaternion::IDENTITY, Ogre::Vector3 _scale = Ogre::Vector3::UNIT_SCALE);
	//constructor for creating editable object from scenenode part
	LightObject( Level *lvl, Ogre::Vector3 pos, Ogre::Quaternion ornt = Ogre::Quaternion::IDENTITY)
	{
		init(lvl,pos,ornt);
	}
	LightObject( LightObject *src, Level *targetLvl = NULL);
	LightObject( TiXmlElement *XMLNode, Level *lvl, Ogre::Vector3 positionOffset = Ogre::Vector3::ZERO );
	GameObject* clone(Level *targetLvl = NULL)
	{
		return static_cast<GameObject*>(new LightObject(this,targetLvl));
	}
	virtual ~LightObject();

	/*void update(Ogre::Real time);*/
	//sets position
	void setPosition(Ogre::Vector3 pos)
	{
		mNode->setPosition(pos);		
	}
	//sets orientation
	void setOrientation(Ogre::Quaternion ornt)
	{
		mNode->setOrientation(ornt);
		//mLight->setDirection(ornt*Ogre::Vector3::NEGATIVE_UNIT_Z);
	}
	virtual TiXmlElement* getXmlElem(Ogre::Vector3 startPosition = Ogre::Vector3::ZERO,bool savegame=false);
	//retrieves body position
	Ogre::Vector3 getPosition()
	{
		return mNode->getPosition();	
	}
	//retr. body orient
	Ogre::Quaternion getOrientation()
	{
		return mNode->getOrientation();
	}

	void rotate(const Ogre::Quaternion& q, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_LOCAL)
	{
		mNode->rotate(q,relativeTo);
	}

	void setPositionOrientation(Ogre::Vector3 pos,Ogre::Quaternion ornt)
	{
		setPosition(pos);
		setOrientation(ornt);
	}
	bool getPositionOrientation( Ogre::Vector3& pos, Ogre::Quaternion& orient )
	{
		orient = getOrientation();
		pos = getPosition();
		return true;
	}
	void setScale(Ogre::Vector3 scale){}
	void modScale(Ogre::Vector3 scale){}
	Ogre::Vector3 getScale()
	{
		return Ogre::Vector3::UNIT_SCALE;
	}

	//lichtspezifisches zeug
	//eigentlich nur für editor wichtig
	void setLightType(Ogre::Light::LightTypes lt);
	Ogre::Light::LightTypes getLightType()
	{
		return mLight->getType();
	}
	
	Ogre::String getDisplayName()
	{
		Ogre::String name = "(li)";
		switch(getLightType())
		{
		case Ogre::Light::LT_DIRECTIONAL:
			name += "directional";
			break;
		case Ogre::Light::LT_POINT:
			name += "point";
			break;
		case Ogre::Light::LT_SPOTLIGHT:
			name += "spotlight";
			break;
		}
		
		name += ("("+Ogre::StringConverter::toString(getLight()->getDiffuseColour())+")"); //+name;
		return name;
	}

	//pointer to the level this obj is currently in
	Level *getLevel()
	{
		return mLevel;
	}
	ObjType getType()
	{
		return otLight;
	}



	Ogre::Light *getLight()
	{
		return mLight;
	}

	void showMesh();
	void hideMesh();

	gamedata *getData()
	{
		return NULL;
	}
	//Ogre::Entity *meshEnt;



protected:
	//gamedata_obj_static *myData;
	//OgreNewt::Body *mBody;
	//Ogre::SceneNode *mNode;
	//ObjType type; //<- später evtl als enum
	//Ogre::String mID;
	//Ogre::String uName;
	//Level *mLevel;
	Ogre::Light *mLight;
	void init( Level *lvl, Ogre::Vector3 pos, Ogre::Quaternion ornt = Ogre::Quaternion::IDENTITY);


};

#endif