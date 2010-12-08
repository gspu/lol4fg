#include "entranceObject.h"
#include "functions.h"
#include "Level.h"

entranceObject::entranceObject(TiXmlElement *XMLNode,Level *lvl, Ogre::Vector3 positionOffset)
{
	//<entrance pos="-18.125 14.5154 -3.25" orient="0.707107 0 -0.707107 0" name="GameStartPosition" />
	
	Ogre::Vector3 pos = Ogre::StringConverter::parseVector3(XMLNode->Attribute("pos"))+positionOffset;
	Ogre::String name = XMLNode->Attribute("name");

	Ogre::Quaternion ornt = Ogre::Quaternion::IDENTITY;
			
	
	if(XMLNode->Attribute("orient"))
	{
		ornt = Ogre::StringConverter::parseQuaternion(XMLNode->Attribute("orient"));
	}
	init(name,lvl,pos,ornt);
	
}

void entranceObject::init(Ogre::String name, Level *lvl, Ogre::Vector3 pos, Ogre::Quaternion ornt)
{
	SGID = 0;
	//die variable ID wird hier für den namen missbraucht
	ID = name;
	mLevel = lvl;
	type = otEntrance;
	WorldArtData d;
	d.type = WT_MESH;
	d.filename = ENTR_MESH;
	d.material = "transform/red";
	mNode = NULL;
	mArt = new WorldArt(this,d);
	//mArt->loadMesh(ENTR_MESH,"colorRed");
	//	createNodeAndEntity(ENTR_MESH,"colorRed");
	mBody = NULL;
	mNode->setPosition(pos);
	mNode->setOrientation(ornt);
	Level::OrientedPoint entr;
	entr.orient = ornt;
	entr.pos = pos;
	mLevel->entrances[name] = entr;
	/*colType = 0;
	collisionParams = Ogre::Vector3::UNIT_SCALE;*/
	
	mLevel->registerObject(this);
	mColData.colType = CT_NOCOLLISION;
	
}


TiXmlElement* entranceObject::getXmlElem(Ogre::Vector3 startPosition,bool savegame)
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
	
	Ogre::String name = getName();
	xmlElem->SetValue("entrance");
	xmlElem->SetAttribute("name",name.c_str());
	return xmlElem;
	
}

entranceObject::~entranceObject()
{
	/*mLevel->unregisterObject(this);
	if(mLevel->entrances.find(ID) != mLevel->entrances.end())
	{
		mLevel->entrances.erase(ID);
	}
	destroyNode(mLevel->getSceneManager(),mNode);*/
}
Ogre::String entranceObject::getName()
{
	return ID;
}
void entranceObject::setName(Ogre::String str)
{
	ID = str;
}


Ogre::Vector3 entranceObject::getScale()
{
	return Ogre::Vector3::UNIT_SCALE;

}