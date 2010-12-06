#include "xmlwrapper.h"

Ogre::Vector3 OgreXmlElement::AttributeAsVector3(Ogre::String attrName,Ogre::Vector3 def, bool toLowerCase)
{
	if(toLowerCase)
		Ogre::StringUtil::toLowerCase(attrName);
	if(Attribute(attrName))
	{
		return Ogre::StringConverter::parseVector3(Attribute(attrName)->c_str());
	}
	else
	{
		return def;
	}
}

Ogre::Real OgreXmlElement::AttributeAsReal(Ogre::String attrName,Ogre::Real def, bool toLowerCase)
{
	if(toLowerCase)
		Ogre::StringUtil::toLowerCase(attrName);
	if(Attribute(attrName))
	{
		return Ogre::StringConverter::parseReal(Attribute(attrName)->c_str());
	}
	else
	{
		return def;
	}
}
Ogre::String OgreXmlElement::AttributeAsString(Ogre::String attrName,Ogre::String def, bool toLowerCase)
{
	if(toLowerCase)
		Ogre::StringUtil::toLowerCase(attrName);	
	return Ogre::String(Attribute(attrName)->c_str());
	
}
int OgreXmlElement::AttributeAsInt(Ogre::String attrName,int def, bool toLowerCase)
{
	if(toLowerCase)
		Ogre::StringUtil::toLowerCase(attrName);
	if(Attribute(attrName))
	{
		return Ogre::StringConverter::parseInt(Attribute(attrName)->c_str());
	}
	else
	{
		return def;
	}
}
unsigned int OgreXmlElement::AttributeAsUnsignedInt(Ogre::String attrName,unsigned int def, bool toLowerCase)
{
	if(toLowerCase)
		Ogre::StringUtil::toLowerCase(attrName);
	if(Attribute(attrName))
	{
		return Ogre::StringConverter::parseUnsignedInt(Attribute(attrName)->c_str());
	}
	else
	{
		return def;
	}
}
Ogre::Quaternion OgreXmlElement::AttributeAsQuaternion(Ogre::String attrName,Ogre::Quaternion def, bool toLowerCase)
{
	if(toLowerCase)
		Ogre::StringUtil::toLowerCase(attrName);
	if(Attribute(attrName))
	{
		return Ogre::StringConverter::parseQuaternion(Attribute(attrName)->c_str());
	}
	else
	{
		return def;
	}
}
Ogre::ColourValue OgreXmlElement::AttributeAsColour(Ogre::String attrName,Ogre::ColourValue def, bool toLowerCase)
{
	if(toLowerCase)
		Ogre::StringUtil::toLowerCase(attrName);
	if(Attribute(attrName))
	{
		return Ogre::StringConverter::parseColourValue(Attribute(attrName)->c_str());
	}
	else
	{
		return def;
	}
}
bool OgreXmlElement::AttributeAsBool(Ogre::String attrName,bool def, bool toLowerCase)
{
	if(toLowerCase)
		Ogre::StringUtil::toLowerCase(attrName);
	if(Attribute(attrName))
	{
		return Ogre::StringConverter::parseBool(Attribute(attrName)->c_str());
	}
	else
	{
		return def;
	}
}

Ogre::Radian OgreXmlElement::AttributeAsRadian(Ogre::String attrName,Ogre::Radian def, bool toLowerCase)
{
	if(toLowerCase)
		Ogre::StringUtil::toLowerCase(attrName);
	if(Attribute(attrName))
	{
		return Ogre::StringConverter::parseAngle(Attribute(attrName)->c_str());
	}
	else
	{
		return def;
	}
}

long OgreXmlElement::AttributeAsLong(Ogre::String attrName,long def, bool toLowerCase)
{
	if(toLowerCase)
		Ogre::StringUtil::toLowerCase(attrName);
	if(Attribute(attrName))
	{
		return Ogre::StringConverter::parseLong(Attribute(attrName)->c_str());
	}
	else
	{
		return def;
	}
}
unsigned long OgreXmlElement::AttributeAsUnsignedLong(Ogre::String attrName,unsigned long def, bool toLowerCase)
{
	if(toLowerCase)
		Ogre::StringUtil::toLowerCase(attrName);
	if(Attribute(attrName))
	{
		return Ogre::StringConverter::parseUnsignedLong(Attribute(attrName)->c_str());
	}
	else
	{
		return def;
	}
}