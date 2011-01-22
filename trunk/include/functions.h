#ifndef __praFunctions
#define __praFunctions
#include "Ogre.h"
#include "OgreNewt.h"
#include "StandardApplication.h"

#ifndef __editor
#include "CEGUI.h"
#else
#include <QPixmap>
#endif

#include "OIS/OIS.h"

//#include "GameObject.h"
//#include "GameChar.h"
#include "tinyxml.h"
#include "data.h"
/*general utility functions*/


//Damage *damagePtrFromXml(TiXmlElement *elem);
//Stats *statsPtrFromXml(TiXmlElement *elem);


inline void mDebugText(Ogre::String string,bool add = false)
{
	StandardApplication::getSingletonPtr()->setDebugText(string,add);
}



#ifndef __editor
CEGUI::MouseButton convertOISMouseButtonToCegui(OIS::MouseButtonID buttonID);

OIS::MouseButtonID convertCeguiMouseButtonToOis(CEGUI::MouseButton buttonID);




inline CEGUI::Window *getActiveWindow()
{
	return CEGUI::System::getSingletonPtr()->getGUISheet()->getActiveChild();
}
inline void deactivateWindow()
{
	using namespace CEGUI;
	Window *win = getActiveWindow();	
	while(win->getType() != (utf8*)"DefaultWindow")
	{
		win->deactivate();
		win = win->getParent();
	}
}

inline Ogre::String str(const CEGUI::Point pt)
{
	return Ogre::StringConverter::toString(pt.d_x)+" "+Ogre::StringConverter::toString(pt.d_y);
}
bool isMouseOverGui();//float const& x, float const& y);

void mLog(CEGUI::String text);
#endif

inline Ogre::String ptrToString(void *ptr)
{
	long address = reinterpret_cast<long>(ptr);
	char buffer[33];
	//int base = 16;
	ltoa(address,buffer,16);
	return Ogre::String(buffer);
}

ObjType stringToObjectType(Ogre::String str);
Ogre::String objectTypeToString(ObjType type);

void destroyNode(Ogre::SceneManager *mSceneMgr,Ogre::SceneNode *mNode);

//void testLoopNode(Ogre::SceneNode *nod, Ogre::Vector3 scale = Ogre::Vector3::UNIT_SCALE, Ogre::Vector3 pos = Ogre::Vector3::ZERO);

OgreNewt::Collision *makeCollision(Ogre::Vector3& inertia,OgreNewt::World *mWorld,short colType, Ogre::Real mass, Ogre::Vector3 CollisionParams, Ogre::SceneNode *nod, Ogre::Vector3 scale = Ogre::Vector3::UNIT_SCALE);
OgreNewt::Body *makeBody(OgreNewt::World *mWorld,short colType, Ogre::Real mass, Ogre::Vector3 CollisionParams, Ogre::SceneNode *nod, Ogre::Vector3 scale = Ogre::Vector3::UNIT_SCALE);

Ogre::SceneNode *getMostParentNode(Ogre::SceneManager *mSceneMgr,Ogre::SceneNode *child);

/*
letzter parameter legt fest, ob bis zum polygon oder nicht
ich sollte diese func auch so ändern, dass es ein Movable zurückgibt
ignore ist eine zu ignorierende entity, zB die des casters selbst
*/
//Ogre::Entity *getClosestEntity(Ogre::SceneManager *mSceneMgr,Ogre::Ray mRay,Ogre::Real maxDistance = 0,bool fullRaycast = false,Ogre::Entity *ignore = NULL);
Ogre::RaySceneQueryResultEntry getClosest(Ogre::SceneManager *mSceneMgr,Ogre::Ray mRay,Ogre::Real maxDistance = 0,Ogre::Real minDistance = 0, bool fullRaycast = false, Ogre::String movableType = "all", Ogre::uint32 queryFlags = ~0);

Ogre::Vector3 getWorldScale(Ogre::SceneManager *mSceneMgr, Ogre::SceneNode *nod);

Ogre::String getUniqueNodeName(Ogre::SceneManager *mgr, Ogre::String base = "");
Ogre::String getUniqueEntityName(Ogre::SceneManager *mgr, Ogre::String base = "");
Ogre::String getUniquePSName(Ogre::SceneManager *mgr, Ogre::String base = "ps");

Ogre::String getUniqueBBSetName(Ogre::SceneManager *mgr, Ogre::String base = "");
Ogre::String getUniqueLightName(Ogre::SceneManager *mgr, Ogre::String base = "");

void getMeshInformation( const Ogre::Mesh* const mesh, size_t &vertex_count,
                                 Ogre::Vector3* &vertices,
                                 size_t &index_count, unsigned long* &indices,
                                 const Ogre::Vector3 &position,
                                 const Ogre::Quaternion &orient,
                                 const Ogre::Vector3 &scale);

short StringToColType(Ogre::String mystring);



//creates a plane from a string like: !plane:1/1 (!plane:xseg/yseg)
//returns true if the mesh has been created or does exist, false if it failed
bool createPlaneMesh(Ogre::String name);

Ogre::Real getFullYaw(Ogre::Quaternion q);
Ogre::Real getFullRoll(Ogre::Quaternion q);
Ogre::Real getFullPitch(Ogre::Quaternion q);

void mLog(const char* text);
void mLog(Ogre::String text);


int round(Ogre::Real x);

//
//inline Ogre::String str(Ogre::Real val, unsigned short precision = 6, 
//    unsigned short width = 0, char fill = ' ', 
//    std::ios::fmtflags flags = std::ios::fmtflags(0) )
//{
//	return Ogre::StringConverter::toString(val,precision,width,fill,flags);
//}
//
///** Converts a Ogre::Radian to a Ogre::String. */
//inline Ogre::String str(Ogre::Radian val, unsigned short precision = 6, 
//    unsigned short width = 0, char fill = ' ', 
//    std::ios::fmtflags flags = std::ios::fmtflags(0) )
//{
//    return Ogre::StringConverter::toString(val.valueAngleUnits(), precision, width, fill, flags);
//}
///** Converts a Ogre::Degree to a Ogre::String. */
//inline Ogre::String str(Ogre::Degree val, unsigned short precision = 6, 
//    unsigned short width = 0, char fill = ' ', 
//    std::ios::fmtflags flags = std::ios::fmtflags(0) )
//{
//    return Ogre::StringConverter::toString(val.valueAngleUnits(), precision, width, fill, flags);
//}
///** Converts an int to a Ogre::String. */
//inline Ogre::String str(int val, unsigned short width = 0, 
//    char fill = ' ', 
//    std::ios::fmtflags flags = std::ios::fmtflags(0) )
//{
//	return Ogre::StringConverter::toString(val,width,fill,flags);
//}
//
///** Converts a size_t to a Ogre::String. */
//inline Ogre::String str(size_t val, unsigned short width = 0, 
//    char fill = ' ', 
//    std::ios::fmtflags flags = std::ios::fmtflags(0) )
//{
//	return Ogre::StringConverter::toString(val,width,fill,flags);
//}
//
///** Converts an unsigned long to a Ogre::String. */
//inline Ogre::String str(unsigned long val, 
//    unsigned short width = 0, char fill = ' ', 
//    std::ios::fmtflags flags = std::ios::fmtflags(0) )
//{
//	return Ogre::StringConverter::toString(val,width,fill,flags);
//}
//
///** Converts a long to a Ogre::String. */
//inline Ogre::String str(long val, 
//    unsigned short width = 0, char fill = ' ', 
//    std::ios::fmtflags flags = std::ios::fmtflags(0) )
//{
//	return Ogre::StringConverter::toString(val,width,fill,flags);
//}
//
///** Converts a boolean to a Ogre::String. 
//@param yesNo If set to true, result is 'yes' or 'no' instead of 'true' or 'false'
//*/
//inline Ogre::String str(bool val, bool yesNo = false)
//{
//	return Ogre::StringConverter::toString(val,yesNo);
//}
///** Converts a Ogre::Vector2 to a Ogre::String. 
//@remarks
//    Format is "x y" (i.e. 2x Ogre::Real values, space delimited)
//*/
//inline Ogre::String str(const Ogre::Vector2& val)
//{
//	return Ogre::StringConverter::toString(val);
//}
///** Converts a Ogre::Vector3 to a Ogre::String. 
//@remarks
//    Format is "x y z" (i.e. 3x Ogre::Real values, space delimited)
//*/
//
//inline Ogre::String str(const Ogre::Vector3& val)
//{	
//	return Ogre::StringConverter::toString(val);
//}
//
//
///** Converts a Ogre::Vector4 to a Ogre::String. 
//@remarks
//    Format is "x y z w" (i.e. 4x Ogre::Real values, space delimited)
//*/
//inline Ogre::String str(const Ogre::Vector4& val)
//{
//	return Ogre::StringConverter::toString(val);
//}
///** Converts a Ogre::Matrix3 to a Ogre::String. 
//@remarks
//    Format is "00 01 02 10 11 12 20 21 22" where '01' means row 0 column 1 etc.
//*/
//inline Ogre::String str(const Ogre::Matrix3& val)
//{
//	return Ogre::StringConverter::toString(val);
//}
///** Converts a Ogre::Matrix4 to a Ogre::String. 
//@remarks
//    Format is "00 01 02 03 10 11 12 13 20 21 22 23 30 31 32 33" where 
//    '01' means row 0 column 1 etc.
//*/
//inline Ogre::String str(const Ogre::Matrix4& val)
//{
//	return Ogre::StringConverter::toString(val);
//}
///** Converts a Ogre::Quaternion to a Ogre::String. 
//@remarks
//    Format is "w x y z" (i.e. 4x Ogre::Real values, space delimited)
//*/
//inline Ogre::String str(const Ogre::Quaternion& val)
//{
//	return Ogre::StringConverter::toString(val);
//}
///** Converts a Ogre::ColourValue to a Ogre::String. 
//@remarks
//    Format is "r g b a" (i.e. 4x Ogre::Real values, space delimited). 
//*/
//inline Ogre::String str(const Ogre::ColourValue& val)
//{
//	return Ogre::StringConverter::toString(val);
//}
///** Converts a Ogre::StringVector to a string.
//@remarks
//    Strings must not contain spaces since space is used as a delimeter in
//    the output.
//*/
//inline Ogre::String str(const Ogre::StringVector& val)
//{
//	return Ogre::StringConverter::toString(val);
//}


 
Ogre::SceneNode *cloneSceneNode(Ogre::SceneManager *targetMgr, Ogre::SceneNode* orig, Ogre::SceneNode *parent = NULL, Ogre::Any cloneAny = Ogre::Any());

//inline BYTE intToByte(int x)
//{
//	if(x<=255)
//		return static_cast<BYTE>(x);
//	else
//		return 255;
//}

Ogre::AxisAlignedBox getSceneNodeBoundingBox(Ogre::SceneNode *node,bool ignoreParticleEffects=false);

//testfunktion
void updateSceneManagersAfterMaterialsChange();

time_t getFileDate( Ogre::String fileName );
bool FileExists( Ogre::String fileName );

bool objectIsStatic(Ogre::String strType);
#ifdef __editor
//tries to create a Qt Pixmap from a given resource name (and resource group). returns true on success, false on failure
bool createQtPixmapFromResource(QPixmap &resultPixmap, Ogre::String resName, Ogre::String resGroup = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,bool onlySquare = true, size_t resultSize = 64);
#endif

#endif