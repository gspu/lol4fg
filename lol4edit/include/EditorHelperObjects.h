#ifndef __editorHelperObjects
#define __editorHelperObjects
//class for some stuff which the editor needs
#include "FwDec.h"
#include <Ogre.h>
#include <DynamicLines.h>

namespace Ogre
{
	class BorderPanelOverlayElement;
}
	//Ogre::Overlay* overlay;

//class for displaying something as a decal on the terrain
class TerrainDecal
{
public:
	TerrainDecal();
	~TerrainDecal();

	typedef std::list<Ogre::TextureUnitState*> TexStateList;



	void setLevel(Level *lvl);

	//the position is in WORLD coordinates
	void update(Ogre::Real x, Ogre::Real z, Ogre::Real decalSize);

	//shows the decal by attaching it to the current level
	void show();

	//hides the decal by de-attaching it from the current level
	void hide();
private:
	//backupping vals
	/*Ogre::Real prev_x;
	Ogre::Real prev_z; 
	Ogre::Real prev_rad;*/

	TexStateList mTexStates;

	//float accuracy;


	Level *mLevel;

	//Ogre::SceneNode *mMainNode;

	Ogre::Frustum *mDecalFrustum;
	Ogre::SceneNode *mProjectorNode;

	/*Ogre::Frustum *mFilterFrustum;
	Ogre::SceneNode *mFilterNode;*/
	/*Ogre::ManualObject *mMeshDecal;
	int x_size;
	int z_size;*/
	bool isShown;

	
	void addDecalPasses();

	//void generateNodeStructure(Ogre::SceneManager *mgr);
};

//a selection border for the editor
class SelectionBorder
{
public:
	SelectionBorder();
	~SelectionBorder();

	//begins the drawing of the border
	void begin(int mouseX, int mouseY);
	//updades it, so that it follows the mouse
	void update(int mouseX, int mouseY);
	//ends it. expects current mouse coordinates and a group object to add the objects to
	//if none passed, it will make a new one
	//returns a groupobject, newly made or the addToGroup
	GroupObject *end(int mouseX, int mouseY, GroupObject *addToGroup = NULL);

	//just cancels selection and hides the border
	void cancel();
private:
	Ogre::BorderPanelOverlayElement *panel;
	Ogre::Overlay* overlay;
	int firstPointX;
	int firstPointY;

//DEBUG:
	//Ogre::SceneNode *debugNode;
 //   //linien
 //   DynamicLines *lines;
 //
 //   
 //   Ogre::Entity *e1;
 //   Ogre::Entity *e2; 
 //   Ogre::Entity *e3;
 //   Ogre::Entity *e4;
 //   Ogre::Entity *e5;

	//Ogre::SceneNode *camHolder;
	//Ogre::Entity *fakeCam;

	//Ogre::SceneNode *h1;
	//Ogre::SceneNode *h2;
	//Ogre::SceneNode *h3;
	//Ogre::SceneNode *h4;
};

////the transformation axis object
//class ObjectManipulator
//{
//public:
//	enum TransformMode
//    {
//	    tmMove,
//	    tmRotate,
//	    tmScale
//    };
//    enum TransformAxis
//    {
//	    //nix
//	    tNone,
//	    //alles gleichzeitig, nur bei scale relevant
//	    tAll,
//	    //X-Achse
//	    tAxisX,
//	    //Y-achse
//	    tAxisY,
//	    //Z-Achse
//	    tAxisZ,
//	    //XY-ebene
//	    tPlaneXY,
//	    //XZ-ebene
//	    tPlaneXZ,
//	    //YZ-ebene
//	    tPlaneYZ
//    };
//
//	ObjectManipulator();
//	~ObjectManipulator();
//
//	void update();
//
//	void show(bool set)
//	{
//		isVisible = set;
//	}
//
//	void createAxes();
//
//	//these are supposed to be called each frame, when mouse is down
//	void objectRotate();
//    void objectMove();
//    void objectScale();
//
//	bool getLocalMode()
//	{
//		return isLocalMode;
//	}
//	void setLocalMode(bool set)
//	{
//		isLocalMode = set;
//	}
//private:
//	Ogre::SceneNode *axesNode;
//    Ogre::SceneNode *axe_X;
//    Ogre::SceneNode *axe_Y;
//    Ogre::SceneNode *axe_Z;
//    Ogre::SceneNode *plane_XZ;
//    Ogre::SceneNode *plane_XY;
//    Ogre::SceneNode *plane_YZ;
//    Ogre::SceneNode *boxAll;
//
//	TransformAxis curTransform;
//	TransformMode mTMode;
//
//	bool isLocalMode;
//	bool isVisible;
//	void createAxes();
//	
//	
//	Ogre::Vector3 ObjectManipulator::snapVector(Ogre::Degree angle,Ogre::Vector3 src,Ogre::Vector3 axis);
//
//	
//};

#endif