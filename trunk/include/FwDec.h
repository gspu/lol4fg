#ifndef __praClassForwardDecl
#define __praClassForwardDecl

class StandardApplication;
class GameObject;
class DynamicObject;
class ZipSaveFile;
class GameChar;
class UniKey;
//class global_class;

//class objCont;
class SpellEffect;
class ContCallback;
class AI;
class Level;
class GameItem;
class DoorObject;
class WorldArt;
class MissileObject;
class StaticObject;
class ExplosionObject;
class BeamObject;
//class StaticObject;
class Inventory;
//class dotSceneObject;
class Spellbook;
class SlotWindow;

#ifndef __editor
//for the game
class GameApp;
class GameFrameListener;
class InventoryWindow;
class SpellbookWindow;
class HoldingSlotWindow;
class MainMenu;
class OptionsWindow;
class SaveLoadMenu;
class HUD;
class ContainerWindow;
#else
//for the editor
class entranceObject;
class EditorApp;
//class EditFrameListener;
namespace QtOgre
{
class EditGameLogic;
}
#endif

class Alchemy;
class gamedata;
class gamedata_char;
class gamedata_container;
class gamedata_door;
class gamedata_item;
class gamedata_dynamic;
class gamedata_spell;
class SpellObject;
class WaterPlane;


class OgreXmlElement;

class ObjectPtr;

#endif // #ifndef __lol4_h_






//Ogre::SceneNode *charNode, *camNode;







//game_char *player;
