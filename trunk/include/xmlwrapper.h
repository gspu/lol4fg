#ifndef _mxmlwrapper
#define _mxmlwrapper

#include "tinyxml.h"
#include "Ogre.h"


class OgreXmlElement: public TiXmlElement
{
public:
    OgreXmlElement(Ogre::String val): TiXmlElement(val){}
	Ogre::Vector3 AttributeAsVector3(Ogre::String attrName,Ogre::Vector3 def = Ogre::Vector3::ZERO,bool toLowerCase = true);
	Ogre::Real AttributeAsReal(Ogre::String attrName,Ogre::Real def = 0,bool toLowerCase = true);
	Ogre::String AttributeAsString(Ogre::String attrName,Ogre::String def = "",bool toLowerCase = true);
	int AttributeAsInt(Ogre::String attrName,int def = 0,bool toLowerCase = true);
	unsigned int AttributeAsUnsignedInt(Ogre::String attrName,unsigned int def = 0,bool toLowerCase = true);
	Ogre::Quaternion AttributeAsQuaternion(Ogre::String attrName,Ogre::Quaternion def = Ogre::Quaternion::IDENTITY,bool toLowerCase = true);
	Ogre::ColourValue AttributeAsColour(Ogre::String attrName,Ogre::ColourValue def = Ogre::ColourValue::White,bool toLowerCase = true);
	bool AttributeAsBool(Ogre::String attrName,bool def = true,bool toLowerCase = true);
	Ogre::Degree AttributeAsDegree(Ogre::String attrName,Ogre::Degree def = Ogre::Degree(0),bool toLowerCase = true)
	{
		return Ogre::Degree(AttributeAsRadian(attrName,Ogre::Degree(def),toLowerCase));
	}
	Ogre::Radian AttributeAsRadian(Ogre::String attrName,Ogre::Radian def = Ogre::Radian(0),bool toLowerCase = true);
	long AttributeAsLong(Ogre::String attrName,long def = 0,bool toLowerCase = true);
	unsigned long AttributeAsUnsignedLong(Ogre::String attrName,unsigned long def = 0,bool toLowerCase = true);


	inline void SetOgreAttribute(Ogre::String name,Ogre::Vector3 value)
	{
		SetAttribute(name,Ogre::StringConverter::toString(value));
	}
	inline void SetOgreAttribute(Ogre::String name,Ogre::Quaternion value)
	{
		SetAttribute(name,Ogre::StringConverter::toString(value));
	}
	inline void SetOgreAttribute(Ogre::String name,Ogre::String value)
	{
		SetAttribute(name,value);
	}
	inline void SetOgreAttribute(Ogre::String name,int value)
	{
		SetAttribute(name,Ogre::StringConverter::toString(value));
	}
	inline void SetOgreAttribute(Ogre::String name,unsigned int value)
	{
		SetAttribute(name,Ogre::StringConverter::toString(value));
	}
	inline void SetOgreAttribute(Ogre::String name,Ogre::ColourValue value)
	{
		SetAttribute(name,Ogre::StringConverter::toString(value));
	}
	inline void SetOgreAttribute(Ogre::String name,Ogre::Real value)
	{
		SetAttribute(name,Ogre::StringConverter::toString(value));
	}
	inline void SetOgreAttribute(Ogre::String name,bool value)
	{
		SetAttribute(name,Ogre::StringConverter::toString(value));
	}
	inline void SetOgreAttribute(Ogre::String name,Ogre::Degree value)
	{
		SetAttribute(name,Ogre::StringConverter::toString(value));
	}
	inline void SetOgreAttribute(Ogre::String name,Ogre::Radian value)
	{
		SetAttribute(name,Ogre::StringConverter::toString(value));
	}
	inline void SetOgreAttribute(Ogre::String name,long value)
	{
		SetAttribute(name,Ogre::StringConverter::toString(value));
	}
	inline void SetOgreAttribute(Ogre::String name,unsigned long value)
	{
		SetAttribute(name,Ogre::StringConverter::toString(value));
	}

	inline OgreXmlElement *FirstChildElement(Ogre::String name)
	{
		return static_cast<OgreXmlElement*>(TiXmlElement::FirstChildElement(name));
	}
	inline OgreXmlElement *FirstChildElement()
	{
		return static_cast<OgreXmlElement*>(TiXmlElement::FirstChildElement());
	}
	inline OgreXmlElement *NextSiblingElement(Ogre::String name)
	{
		return static_cast<OgreXmlElement*>(TiXmlElement::NextSiblingElement(name));
	}
	inline OgreXmlElement *NextSiblingElement()
	{
		return static_cast<OgreXmlElement*>(TiXmlElement::NextSiblingElement());
	}
    inline TiXmlElement *asTiXmlElement()
    {
        return static_cast<TiXmlElement*>(this);
    }
};

#endif