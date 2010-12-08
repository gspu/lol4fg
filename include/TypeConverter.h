#ifndef _typeConverter
#define _typeConverter

#include <Ogre.h>
#ifndef __editor
#include <CEGUI.h>
#else
#include <QImage>
#endif
#include <OIS.h>

//CEGUI::PropertyHelper::

namespace TypeConverter
{
	//converter to ogre strings
	inline Ogre::String ogre_str(Ogre::Real val, unsigned short precision = 6, 
		unsigned short width = 0, char fill = ' ', 
		std::ios::fmtflags flags = std::ios::fmtflags(0) )
	{
		return Ogre::StringConverter::toString(val,precision,width,fill,flags);
	}

	/** Converts a Ogre::Radian to a Ogre::String. */
	inline Ogre::String ogre_str(Ogre::Radian val, unsigned short precision = 6, 
		unsigned short width = 0, char fill = ' ', 
		std::ios::fmtflags flags = std::ios::fmtflags(0) )
	{
		return Ogre::StringConverter::toString(val.valueAngleUnits(), precision, width, fill, flags);
	}
	/** Converts a Ogre::Degree to a Ogre::String. */
	inline Ogre::String ogre_str(Ogre::Degree val, unsigned short precision = 6, 
		unsigned short width = 0, char fill = ' ', 
		std::ios::fmtflags flags = std::ios::fmtflags(0) )
	{
		return Ogre::StringConverter::toString(val.valueAngleUnits(), precision, width, fill, flags);
	}
	/** Converts an int to a Ogre::String. */
	inline Ogre::String ogre_str(int val, unsigned short width = 0, 
		char fill = ' ', 
		std::ios::fmtflags flags = std::ios::fmtflags(0) )
	{
		return Ogre::StringConverter::toString(val,width,fill,flags);
	}

	/** Converts a size_t to a Ogre::String. */
	inline Ogre::String ogre_str(size_t val, unsigned short width = 0, 
		char fill = ' ', 
		std::ios::fmtflags flags = std::ios::fmtflags(0) )
	{
		return Ogre::StringConverter::toString(val,width,fill,flags);
	}

	/** Converts an unsigned long to a Ogre::String. */
	inline Ogre::String ogre_str(unsigned long val, 
		unsigned short width = 0, char fill = ' ', 
		std::ios::fmtflags flags = std::ios::fmtflags(0) )
	{
		return Ogre::StringConverter::toString(val,width,fill,flags);
	}

	/** Converts a long to a Ogre::String. */
	inline Ogre::String ogre_str(long val, 
		unsigned short width = 0, char fill = ' ', 
		std::ios::fmtflags flags = std::ios::fmtflags(0) )
	{
		return Ogre::StringConverter::toString(val,width,fill,flags);
	}

	/** Converts a boolean to a Ogre::String. 
	@param yesNo If set to true, result is 'yes' or 'no' instead of 'true' or 'false'
	*/
	inline Ogre::String ogre_str(bool val, bool yesNo = false)
	{
		return Ogre::StringConverter::toString(val,yesNo);
	}
	/** Converts a Ogre::Vector2 to a Ogre::String. 
	@remarks
		Format is "x y" (i.e. 2x Ogre::Real values, space delimited)
	*/
	inline Ogre::String ogre_str(const Ogre::Vector2& val)
	{
		return Ogre::StringConverter::toString(val);
	}
	/** Converts a Ogre::Vector3 to a Ogre::String. 
	@remarks
		Format is "x y z" (i.e. 3x Ogre::Real values, space delimited)
	*/

	inline Ogre::String ogre_str(const Ogre::Vector3& val)
	{	
		return Ogre::StringConverter::toString(val);
	}


	/** Converts a Ogre::Vector4 to a Ogre::String. 
	@remarks
		Format is "x y z w" (i.e. 4x Ogre::Real values, space delimited)
	*/
	inline Ogre::String ogre_str(const Ogre::Vector4& val)
	{
		return Ogre::StringConverter::toString(val);
	}
	/** Converts a Ogre::Matrix3 to a Ogre::String. 
	@remarks
		Format is "00 01 02 10 11 12 20 21 22" where '01' means row 0 column 1 etc.
	*/
	inline Ogre::String ogre_str(const Ogre::Matrix3& val)
	{
		return Ogre::StringConverter::toString(val);
	}
	/** Converts a Ogre::Matrix4 to a Ogre::String. 
	@remarks
		Format is "00 01 02 03 10 11 12 13 20 21 22 23 30 31 32 33" where 
		'01' means row 0 column 1 etc.
	*/
	inline Ogre::String ogre_str(const Ogre::Matrix4& val)
	{
		return Ogre::StringConverter::toString(val);
	}
	/** Converts a Ogre::Quaternion to a Ogre::String. 
	@remarks
		Format is "w x y z" (i.e. 4x Ogre::Real values, space delimited)
	*/
	inline Ogre::String ogre_str(const Ogre::Quaternion& val)
	{
		return Ogre::StringConverter::toString(val);
	}
	/** Converts a Ogre::ColourValue to a Ogre::String. 
	@remarks
		Format is "r g b a" (i.e. 4x Ogre::Real values, space delimited). 
	*/
	inline Ogre::String ogre_str(const Ogre::ColourValue& val)
	{
		return Ogre::StringConverter::toString(val);
	}
	/** Converts a Ogre::StringVector to a string.
	@remarks
		Strings must not contain spaces since space is used as a delimeter in
		the output.
	*/
	inline Ogre::String ogre_str(const Ogre::StringVector& val)
	{
		return Ogre::StringConverter::toString(val);
	}
	
///////////////////////////////////////CEGUI-RELEVANT STUFF///////////////////////////////////////
#ifndef __editor
	inline Ogre::String orge_str(const CEGUI::String str)
	{
		return Ogre::String(str.c_str());
	}
	inline Ogre::String orge_str_TEST(const CEGUI::String str)
	{
		return Ogre::String(str.c_str());
	}
	


	//to CEGUI strings
	//CEGUI::PropertyHelper
	inline CEGUI::String cegui_str(float val)
	{
		return CEGUI::PropertyHelper::floatToString(val);
	}

	inline CEGUI::String cegui_str(unsigned int val)
	{
		return CEGUI::PropertyHelper::uintToString(val);
	}

	inline CEGUI::String cegui_str(int val)
	{
		return CEGUI::PropertyHelper::intToString(val);
	}

	inline CEGUI::String cegui_str(bool val)
	{
		return CEGUI::PropertyHelper::boolToString(val);
	}

	inline CEGUI::String cegui_str(const CEGUI::Size& val)
	{
		return CEGUI::PropertyHelper::sizeToString(val);
	}

	inline CEGUI::String cegui_str(const CEGUI::Point& val)
	{
		return CEGUI::PropertyHelper::pointToString(val);
	}

	inline CEGUI::String cegui_str(const CEGUI::Vector3& val)
	{
		return CEGUI::PropertyHelper::vector3ToString(val);
	}
	    
	inline CEGUI::String cegui_str(const CEGUI::Rect& val)
	{
		return CEGUI::PropertyHelper::rectToString(val);
	}

	inline CEGUI::String cegui_str(const CEGUI::Image* const val)
	{
		return CEGUI::PropertyHelper::imageToString(val);
	}

	inline CEGUI::String cegui_str(const CEGUI::colour& val)
	{
		return CEGUI::PropertyHelper::colourToString(val);
	}

	inline CEGUI::String cegui_str(const CEGUI::ColourRect& val)
	{
		return CEGUI::PropertyHelper::colourRectToString(val);
	}

	inline CEGUI::String cegui_str(const CEGUI::UDim& val)
	{
		return CEGUI::PropertyHelper::udimToString(val);
	}

	inline CEGUI::String cegui_str(const CEGUI::UVector2& val)
	{
		return CEGUI::PropertyHelper::uvector2ToString(val);
	}

	inline CEGUI::String cegui_str(const CEGUI::URect& val)
	{
		return CEGUI::PropertyHelper::urectToString(val);
	}

	inline CEGUI::String cegui_str(Ogre::String str)
	{
		return CEGUI::String(str);
	}
	
	//Ogre-Types for CEGUI
	inline Ogre::Vector3 toOgreVector3(CEGUI::Vector3 val)
	{
		return Ogre::Vector3(val.d_x,val.d_y,val.d_z);
	}
	
	inline CEGUI::Vector3 toCeguiVector3(Ogre::Vector3 val)
	{
		return CEGUI::Vector3(val.x,val.y,val.z);
	}

	inline CEGUI::Size toCeguiSize(Ogre::Vector2 val)
	{
		return CEGUI::Size(val.x,val.y);
	}

	inline Ogre::Vector2 toOgreVector2(CEGUI::Size val)
	{
		return Ogre::Vector2(val.d_width,val.d_height);
	}
#else
///////////////////////////////////////CEGUI-RELEVANT STUFF ENDS HERE////////////////////////////
///////////////////////////////////////QT-RELEVANT STUFF BEGINS HERE////////////////////////////
//QImage ogreImageToQt(Ogre::Image *img);
QImage::Format ogreFormatToQt(Ogre::PixelFormat format);
inline QString qt_str(Ogre::String str)
{
	return QString(str.c_str());
}
inline Ogre::String ogre_str(QString str)
{
	return Ogre::String(str.toStdString());
}

///////////////////////////////////////QT-RELEVANT STUFF ENDS HERE////////////////////////////
#endif



	//else
	inline BYTE intToByte(int x)
	{
		if(x<=255)
			return static_cast<BYTE>(x);
		return 255;
	}
/*
	inline unsigned int intToUInt(int x)
	{
		if(x < 0)
			return 0;
		return static_cast<unsigned int>(x);
	}

	inline int uintToInt(unsigned int x)
	{		
		if(x > MAXINT)
			return MAXINT;
		return static_cast<int>(x);
	}*/
}

#endif