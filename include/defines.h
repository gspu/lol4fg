#ifndef __praDefines
#define __praDefines



////body type defines
//#define BT_STATIC 0  //part of the level
//#define BT_MOVABLE 1 //stuff to push around
//#define BT_CHAR 2    //living stuff
//#define BT_ITEM 3

//collision type defines
//jetzt in StandardApplication




//errors
#define ERROR_MESH "box.mesh"
#define ERROR_INVIMAGE "error.png"
#define ERROR_IMAGE "error.png"
#define ERROR_NONAME "-no name defined-" 
#define ERROR_MATERIAL "nothing"

////obj types
////0 is reserved
//#define OT_STATIC 1
//#define OT_ITEM 2
//#define OT_CHAR 3
//#define OT_ACT 4


//stuff
#define CAM_NAME "mainCam"
#define ENTR_MESH "arrow.mesh"
#define MESH_MOVE "pfeil1.mesh"
#define MESH_ROTATE "Ring2.mesh"
#define MESH_SCALE "pfeil2.mesh"
#define MESH_BOX "box.mesh"
#define MESH_PSDUMMY "particledummy.mesh"
#define MAT_X "transform/red"
#define MAT_Y "transform/blue"
#define MAT_Z "transform/green"
#define MATP_X "transform/redplane"
#define MATP_Y "transform/blueplane"
#define MATP_Z "transform/greenplane"
#define MAT_ALL "transform/white"
#define MAT_REV_PLANE "editor/planebackside"
#define MAT_PSDUMMY "editor/psdummy"
#define MAT_BBDUMMY "editor/bbdummy"

//default axis
#define AXIS_X 1
#define AXIS_Y 2
#define AXIS_Z 3


//some exceltion constants
#define EX_CONSTR_FAILED 1

//stuff for rayqueries
#define QM_ALL ~0
#define QM_OBJECTS 1
#define QM_TRANSFORMAXES 1<<1
//#define QM_irgendwas 1<<2

#define UPDATE_FRAMERATE float(60.0f)
#define UPDATE_FRAMERATE_MIN float(1.0f/600.f)
#define UPDATE_FRAMERATE_MAX float(10.f/600.0f)

//some strings
#define OGRECONFIG "ogre.cfg"
#define LEVELEXT ".lvl"
#define ZLEVELEXT ".zip"
#define DOTSCENEEXT ".scene"
#define ICON_MELEE  "sword-white-50opacity.png"
#define ICON_ARMOR  "shirt-white-50opacity.png"
#define ICON_SHIELD "shield-white-50opacity.png"
#define ICON_RANGED "bow-white-50opacity.png"
#define ICON_ELSE   "star-white-50opacity.png"
#define ICON_BAG   "star-white-50opacity.png"
#define ICON_SELECTBORDER "redborder.png"

//GUI
#define LIST_ITEMS "items_list"
#define LIST_STATIC_MESHES "staticmeshes_list"
#define LIST_STATIC_DOTSCENES "Dotscenes_list"
#define LIST_SPECIAL "special_list"
#define LIST_CHARACTERS "chars_list"
#define LIST_DOORS "doors_list"
#define LIST_ELSE "else_list"
#define LIST_CONTAINERS "Containers_list"
#define STYLE_EDITOR "WindowsLook"
#define STYLE_GAME "LandsLook"
#define NAME_SPELLBORDER "spell/selectborder"


//some constants
#define GRAVITY -9.81
#define WORLD_MIN_Y -1000
#define WORLD_MAX_Y 1000
#define MAX_JUMPTIME 0.1
#define MIN_JUMPPAUSE 0.5
#define SCREENTEXT_TIME 10
//#define NEWTON_UPDATE (float)(1.0f / 200.0f)
//geschwindigkeit, unterhalb der man wieder springen kann
#define MIN_JUMP_VEL 0.01
//anzahl items pro zeile im inventar
#define INV_ROW_LENGTH 6 
#define INV_IMAGESIZE 36
#define SLOT_MAX 200
#define PICKUP_RANGE 2
//#define MELEE_RANGE 2
#define MAX_RANGED_RAYCAST 50
#define TEST_COOLDOWN 0.5
#define AXES_SCALE_FACTOR 5
#define CUR_LEVEL_VERSION 3
//lebensdauer in Sekunden
#define MISSILE_LIFESPAN 10
#define AMBIENTSND_MAX_WAIT 10

#define STEPSOUND_DEFAULT "default_step.wav"
#define SAVEGAME_EXTENSION "zip"
//inventar



//object flags
#define OF_INVISIBLE 1

#ifndef __editor

#define WINDOW_NAME "LoL4"

#else

#define WINDOW_NAME "LoL4 Editor"

//KEY defines
#define KEY_VROTATE OIS::KC_LCONTROL
#define KEY_VMOVE	OIS::KC_SPACE
//moveMode
#define MM_MOVE_XZ 1
#define MM_MOVE_XY 2
#define MM_MOVE_ZY 3
#define MM_MOVE_X 4
#define MM_MOVE_Y 5
#define MM_MOVE_Z 6




#endif

#endif

