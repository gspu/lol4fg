//------- BEGIN OGRE & stuff includes ----------
#include <Ogre.h>

#include <OgreFrameListener.h>




//#include <OgreFontManager.h>
#include <vector>
#include <OgreNewt.h>
#include "tinyxml.h"


//mem probs without this next one
////#include <OgreNoMemoryMacros.h>
//#include <CEGUI/CEGUIImageset.h>
//#include <CEGUI/CEGUISystem.h>
//#include <CEGUI/CEGUILogger.h>
//#include <CEGUI/CEGUISchemeManager.h>
//#include <CEGUI/CEGUIWindowManager.h>
//#include <CEGUI/CEGUIWindow.h>
//#include "OgreCEGUIRenderer.h"
//#include "OgreCEGUIResourceProvider.h"
//#include <CEGUI/elements/CEGUIPushButton.h>
//#include <CEGUI/elements/CEGUICheckbox.h>
//#include <CEGUI/elements/CEGUIListbox.h>
//#include <CEGUI/elements/CEGUIListboxTextItem.h>
//#include <CEGUI/elements/CEGUITabControl.h>
//#include <CEGUI/elements/CEGUICombobox.h>
//#include <CEGUI/elements/CEGUIMenubar.h>
//#include <CEGUI/elements/CEGUIMenuItem.h>
//#include <CEGUI/elements/CEGUIPopupMenu.h>
//#include <CEGUI/elements/CEGUIFrameWindow.h>
//regular mem handler
//#include <OgreMemoryMacros.h>
//------- END OGRE & stuff includes ----------
//#include "editFWDec.h"
#include "FwDec.h"
#include "EditorApp.h"
//#include "editorDefines.h"
#include "defines.h"
#include "EditGameLogic.h"
#include "Level.h"
#include "GameObject.h"
#include "LightObject.h"
#include "entranceObject.h"
#include "DoorObject.h"
#include "functions.h"
#include "../res/resource.h"
#include "StaticObject.h"
#include "ContainerObject.h"
#include "GameChar.h"
#include "Inventory.h"
//#include "DecalCursor.h"
#include "SoundManager.h"
#include "tinyxml.h"
#include "xmlwrapper.h"
#include "waterplane.h"
#include "groupObject.h"
#include "QtEditorApp.h"
using namespace std;
//using namespace Ogre;


Qt::MouseButton convertOisMouseToQt(OIS::MouseButtonID mouse)
{
	/*qt: enum MouseButton {
        NoButton         = 0x00000000,
        LeftButton       = 0x00000001,
        RightButton      = 0x00000002,
        MidButton        = 0x00000004,
        XButton1         = 0x00000008,
        XButton2         = 0x00000010,
        MouseButtonMask  = 0x000000ff
    };
	ois: MB_Left = 0, MB_Right, MB_Middle,
		MB_Button3, MB_Button4,	MB_Button5, MB_Button6,	MB_Button7
	*/
	switch(mouse)
	{
	case OIS::MB_Left:
		return Qt::LeftButton;
	case OIS::MB_Right:
		return Qt::RightButton;		
	case OIS::MB_Middle:
		return Qt::MidButton;
	//ich bin mir nicht sicher, ob dieses mapping nun stimmt....
	case OIS::MB_Button3:
		return Qt::XButton1;
	case OIS::MB_Button4:
		return Qt::XButton2;
	default:
		return Qt::NoButton;
	}
}
OIS::MouseButtonID convertQtMouseToOis(Qt::MouseButton mouse)
{
	//hier ist es scheiße, weil OIS kein NoButton kennt. entweder ich nehm button7 oder so, oder leftbutton...
	//aber solche spezialfälle sollt man eh abfangen
	switch(mouse)
	{
	case Qt::LeftButton:
		return OIS::MB_Left;
	case Qt::RightButton:
		return OIS::MB_Right;		
	case Qt::MidButton:
		return OIS::MB_Middle;
	//ich bin mir nicht sicher, ob dieses mapping nun stimmt....
	case Qt::XButton1:
		return OIS::MB_Button3;
	case Qt::XButton2:
		return OIS::MB_Button4;
	default:
		return OIS::MB_Left;
	}
}


OIS::KeyCode convertQtKeyToOis(Qt::Key key)
{
    switch(key){                
	case Qt::Key_Escape: return OIS::KC_ESCAPE;
	case Qt::Key_1: return OIS::KC_1;
	case Qt::Key_2: return OIS::KC_2;
	case Qt::Key_3: return OIS::KC_3;
	case Qt::Key_4: return OIS::KC_4;
	case Qt::Key_5: return OIS::KC_5;
	case Qt::Key_6: return OIS::KC_6;
	case Qt::Key_7: return OIS::KC_7;
	case Qt::Key_8: return OIS::KC_8;
	case Qt::Key_9: return OIS::KC_9;
	case Qt::Key_0: return OIS::KC_0;
	case Qt::Key_Minus: return OIS::KC_MINUS;    // - on main keyboard
	case Qt::Key_Equal: return OIS::KC_EQUALS;
	case Qt::Key_Backspace: return OIS::KC_BACK;    // backspace
	case Qt::Key_Tab: return OIS::KC_TAB;
	case Qt::Key_Q: return OIS::KC_Q;
	case Qt::Key_W: return OIS::KC_W;
	case Qt::Key_E: return OIS::KC_E;
	case Qt::Key_R: return OIS::KC_R;
	case Qt::Key_T: return OIS::KC_T;
	case Qt::Key_Y: return OIS::KC_Y;
	case Qt::Key_U: return OIS::KC_U;
	case Qt::Key_I: return OIS::KC_I;
	case Qt::Key_O: return OIS::KC_O;
	case Qt::Key_P: return OIS::KC_P;
	case Qt::Key_BracketLeft: return OIS::KC_LBRACKET;
	case Qt::Key_BracketRight: return OIS::KC_RBRACKET;
	case Qt::Key_Return: return OIS::KC_RETURN;    // Enter on main keyboard
	case Qt::Key_Control: return OIS::KC_LCONTROL;
	case Qt::Key_A: return OIS::KC_A;
	case Qt::Key_S: return OIS::KC_S;
	case Qt::Key_D: return OIS::KC_D;
	case Qt::Key_F: return OIS::KC_F;
	case Qt::Key_G: return OIS::KC_G;
	case Qt::Key_H: return OIS::KC_H;
	case Qt::Key_J: return OIS::KC_J;
	case Qt::Key_K: return OIS::KC_K;
	case Qt::Key_L: return OIS::KC_L;
	case Qt::Key_Semicolon: return OIS::KC_SEMICOLON;
	case Qt::Key_Apostrophe: return OIS::KC_APOSTROPHE;
	case Qt::Key_Dead_Grave: return OIS::KC_GRAVE;    // IS THIS RIGHT?!!?!?!
	case Qt::Key_Shift: return OIS::KC_LSHIFT;
	case Qt::Key_Backslash: return OIS::KC_BACKSLASH;
	case Qt::Key_Z: return OIS::KC_Z;
	case Qt::Key_X: return OIS::KC_X;
	case Qt::Key_C: return OIS::KC_C;
	case Qt::Key_V: return OIS::KC_V;
	case Qt::Key_B: return OIS::KC_B;
	case Qt::Key_N: return OIS::KC_N;
	case Qt::Key_M: return OIS::KC_M;
	case Qt::Key_Comma: return OIS::KC_COMMA;
	case Qt::Key_Period: return OIS::KC_PERIOD;    // . on main keyboard
	case Qt::Key_Slash: return OIS::KC_SLASH;    // / on main keyboard
//	case Qt::Key_Shift: return OIS::KC_RSHIFT;
	case Qt::Key_Asterisk: return OIS::KC_MULTIPLY;    // * on numeric keypad
	case Qt::Key_Alt: return OIS::KC_LMENU;    // left Alt
	case Qt::Key_Space: return OIS::KC_SPACE;
	case Qt::Key_CapsLock: return OIS::KC_CAPITAL;
	case Qt::Key_F1: return OIS::KC_F1;
	case Qt::Key_F2: return OIS::KC_F2;
	case Qt::Key_F3: return OIS::KC_F3;
	case Qt::Key_F4: return OIS::KC_F4;
	case Qt::Key_F5: return OIS::KC_F5;
	case Qt::Key_F6: return OIS::KC_F6;
	case Qt::Key_F7: return OIS::KC_F7;
	case Qt::Key_F8: return OIS::KC_F8;
	case Qt::Key_F9: return OIS::KC_F9;
	case Qt::Key_F10: return OIS::KC_F10;
	case Qt::Key_NumLock: return OIS::KC_NUMLOCK;
	case Qt::Key_ScrollLock: return OIS::KC_SCROLL;    // Scroll Lock
//	case Qt::Key_7: return OIS::KC_NUMPAD7;
//	case Qt::Key_8: return OIS::KC_NUMPAD8;
//	case Qt::Key_9: return OIS::KC_NUMPAD9;
//	case Qt::Key_Minus: return OIS::KC_SUBTRACT;    // - on numeric keypad
//	case Qt::Key_4: return OIS::KC_NUMPAD4;
//	case Qt::Key_5: return OIS::KC_NUMPAD5;
//	case Qt::Key_6: return OIS::KC_NUMPAD6;
	case Qt::Key_Plus: return OIS::KC_ADD;    // + on numeric keypad
//	case Qt::Key_1: return OIS::KC_NUMPAD1;
//	case Qt::Key_2: return OIS::KC_NUMPAD2;
//	case Qt::Key_3: return OIS::KC_NUMPAD3;
//	case Qt::Key_0: return OIS::KC_NUMPAD0;
//	case Qt::Key_Period: return OIS::KC_DECIMAL;    // . on numeric keypad
	
	case Qt::Key_F11: return OIS::KC_F11;
	case Qt::Key_F12: return OIS::KC_F12;
	case Qt::Key_F13: return OIS::KC_F13;    //                     (NEC PC98)
	case Qt::Key_F14: return OIS::KC_F14;    //                     (NEC PC98)
	case Qt::Key_F15: return OIS::KC_F15;    //                     (NEC PC98)
	case Qt::Key_Katakana: return OIS::KC_KANA; //NO IDEA IF IT'S CORRECT    // (Japanese keyboard)
	
	case Qt::Key_Kanji: return OIS::KC_CONVERT;    // (Japanese keyboard)
	case Qt::Key_Muhenkan: return OIS::KC_NOCONVERT;    // (Japanese keyboard)
	case Qt::Key_yen: return OIS::KC_YEN;    // (Japanese keyboard)
	
//	case Qt::Key_Equal: return OIS::KC_NUMPADEQUALS;    // = on numeric keypad (NEC PC98)
	case Qt::Key_MediaPrevious: return OIS::KC_PREVTRACK;    // Previous Track (OIS::KC_CIRCUMFLEX on Japanese keyboard)
	case Qt::Key_At: return OIS::KC_AT;    //                     (NEC PC98)
	case Qt::Key_Colon: return OIS::KC_COLON;    //                     (NEC PC98)
	case Qt::Key_Underscore: return OIS::KC_UNDERLINE;    //                     (NEC PC98)
//	case Qt::Key_Kanji: return OIS::KC_KANJI;    // (Japanese keyboard)
	case Qt::Key_Stop: return OIS::KC_STOP;    //                     (NEC PC98)
	
	
	case Qt::Key_MediaNext: return OIS::KC_NEXTTRACK;    // Next Track
	case Qt::Key_Enter: return OIS::KC_NUMPADENTER;    //OR IS IT Key_Return?! Enter on numeric keypad
//	case Qt::Key_Control: return OIS::KC_RCONTROL;
	case Qt::Key_VolumeMute: return OIS::KC_MUTE;    // Mute
	
	case Qt::Key_Pause: return OIS::KC_PLAYPAUSE;    // Play / Pause
//	case Qt::Key_Stop: return OIS::KC_MEDIASTOP;    // Media Stop
	case Qt::Key_VolumeDown: return OIS::KC_VOLUMEDOWN;    // Volume -
	case Qt::Key_VolumeUp: return OIS::KC_VOLUMEUP;    // Volume +
	case Qt::Key_HomePage: return OIS::KC_WEBHOME;    // Web home
//	case Qt::Key_Comma: return OIS::KC_NUMPADCOMMA; //on numeric keypad (NEC PC98)
//	case Qt::Key_Slash: return OIS::KC_DIVIDE;    // / on numeric keypad
	case Qt::Key_SysReq: return OIS::KC_SYSRQ;
//	case Qt::Key_Alt: return OIS::KC_RMENU;    // right Alt
//	case Qt::Key_Pause: return OIS::KC_PAUSE;    // Pause
	case Qt::Key_Home: return OIS::KC_HOME;    // Home on arrow keypad
	case Qt::Key_Up: return OIS::KC_UP;    // UpArrow on arrow keypad
	case Qt::Key_PageUp: return OIS::KC_PGUP;    // PgUp on arrow keypad
	case Qt::Key_Left: return OIS::KC_LEFT;    // LeftArrow on arrow keypad
	case Qt::Key_Right: return OIS::KC_RIGHT;    // RightArrow on arrow keypad
	case Qt::Key_End: return OIS::KC_END;    // End on arrow keypad
	case Qt::Key_Down: return OIS::KC_DOWN;    // DownArrow on arrow keypad
	case Qt::Key_PageDown: return OIS::KC_PGDOWN;    // PgDn on arrow keypad
	case Qt::Key_Insert: return OIS::KC_INSERT;    // Insert on arrow keypad
	case Qt::Key_Delete: return OIS::KC_DELETE;    // Delete on arrow keypad
	case Qt::Key_Super_L: return OIS::KC_LWIN;    // Left Windows key I HOPE THIS IS RIGHT
	case Qt::Key_Super_R: return OIS::KC_RWIN;    // Right Windows key
	
	
	case Qt::Key_Sleep: return OIS::KC_SLEEP;    // System Sleep
	
	case Qt::Key_Search: return OIS::KC_WEBSEARCH;    // Web Search
	case Qt::Key_Favorites: return OIS::KC_WEBFAVORITES;    // Web Favorites
	case Qt::Key_Refresh: return OIS::KC_WEBREFRESH;    // Web Refresh
//	case Qt::Key_Stop: return OIS::KC_WEBSTOP;    // Web Stop
	case Qt::Key_Forward: return OIS::KC_WEBFORWARD;    // Web Forward
	case Qt::Key_Back: return OIS::KC_WEBBACK;    // Web Back
	
	case Qt::Key_LaunchMail: return OIS::KC_MAIL;    // Mail
    case Qt::Key_Select: return OIS::KC_MEDIASELECT;     // Media Select
    default: return OIS::KC_UNASSIGNED;
    }
}
Qt::Key convertOisKeyToQt(OIS::KeyCode key)
{
    switch(key){
//    case OIS::KC_UNASSIGNED  : return 0;
	case OIS::KC_ESCAPE      : return Qt::Key_Escape;
	case OIS::KC_1           : return Qt::Key_1;
	case OIS::KC_2           : return Qt::Key_2;
	case OIS::KC_3           : return Qt::Key_3;
	case OIS::KC_4           : return Qt::Key_4;
	case OIS::KC_5           : return Qt::Key_5;
	case OIS::KC_6           : return Qt::Key_6;
	case OIS::KC_7           : return Qt::Key_7;
	case OIS::KC_8           : return Qt::Key_8;
	case OIS::KC_9           : return Qt::Key_9;
	case OIS::KC_0           : return Qt::Key_0;
	case OIS::KC_MINUS       : return Qt::Key_Minus;    // - on main keyboard
	case OIS::KC_EQUALS      : return Qt::Key_Equal;
	case OIS::KC_BACK        : return Qt::Key_Backspace;    // backspace
	case OIS::KC_TAB         : return Qt::Key_Tab;
	case OIS::KC_Q           : return Qt::Key_Q;
	case OIS::KC_W           : return Qt::Key_W;
	case OIS::KC_E           : return Qt::Key_E;
	case OIS::KC_R           : return Qt::Key_R;
	case OIS::KC_T           : return Qt::Key_T;
	case OIS::KC_Y           : return Qt::Key_Y;
	case OIS::KC_U           : return Qt::Key_U;
	case OIS::KC_I           : return Qt::Key_I;
	case OIS::KC_O           : return Qt::Key_O;
	case OIS::KC_P           : return Qt::Key_P;
	case OIS::KC_LBRACKET    : return Qt::Key_BracketLeft;
	case OIS::KC_RBRACKET    : return Qt::Key_BracketRight;
	case OIS::KC_RETURN      : return Qt::Key_Return;    // Enter on main keyboard
	case OIS::KC_LCONTROL    : return Qt::Key_Control;
	case OIS::KC_A           : return Qt::Key_A;
	case OIS::KC_S           : return Qt::Key_S;
	case OIS::KC_D           : return Qt::Key_D;
	case OIS::KC_F           : return Qt::Key_F;
	case OIS::KC_G           : return Qt::Key_G;
	case OIS::KC_H           : return Qt::Key_H;
	case OIS::KC_J           : return Qt::Key_J;
	case OIS::KC_K           : return Qt::Key_K;
	case OIS::KC_L           : return Qt::Key_L;
	case OIS::KC_SEMICOLON   : return Qt::Key_Semicolon;
	case OIS::KC_APOSTROPHE  : return Qt::Key_Apostrophe;
	case OIS::KC_GRAVE       : return Qt::Key_Dead_Grave;    // IS THIS RIGHT?!!?!?!
	case OIS::KC_LSHIFT      : return Qt::Key_Shift;
	case OIS::KC_BACKSLASH   : return Qt::Key_Backslash;
	case OIS::KC_Z           : return Qt::Key_Z;
	case OIS::KC_X           : return Qt::Key_X;
	case OIS::KC_C           : return Qt::Key_C;
	case OIS::KC_V           : return Qt::Key_V;
	case OIS::KC_B           : return Qt::Key_B;
	case OIS::KC_N           : return Qt::Key_N;
	case OIS::KC_M           : return Qt::Key_M;
	case OIS::KC_COMMA       : return Qt::Key_Comma;
	case OIS::KC_PERIOD      : return Qt::Key_Period;    // . on main keyboard
	case OIS::KC_SLASH       : return Qt::Key_Slash;    // / on main keyboard
	case OIS::KC_RSHIFT      : return Qt::Key_Shift;
	case OIS::KC_MULTIPLY    : return Qt::Key_Asterisk;    // * on numeric keypad
	case OIS::KC_LMENU       : return Qt::Key_Alt;    // left Alt
	case OIS::KC_SPACE       : return Qt::Key_Space;
	case OIS::KC_CAPITAL     : return Qt::Key_CapsLock;
	case OIS::KC_F1          : return Qt::Key_F1;
	case OIS::KC_F2          : return Qt::Key_F2;
	case OIS::KC_F3          : return Qt::Key_F3;
	case OIS::KC_F4          : return Qt::Key_F4;
	case OIS::KC_F5          : return Qt::Key_F5;
	case OIS::KC_F6          : return Qt::Key_F6;
	case OIS::KC_F7          : return Qt::Key_F7;
	case OIS::KC_F8          : return Qt::Key_F8;
	case OIS::KC_F9          : return Qt::Key_F9;
	case OIS::KC_F10         : return Qt::Key_F10;
	case OIS::KC_NUMLOCK     : return Qt::Key_NumLock;
	case OIS::KC_SCROLL      : return Qt::Key_ScrollLock;    // Scroll Lock
	case OIS::KC_NUMPAD7     : return Qt::Key_7;
	case OIS::KC_NUMPAD8     : return Qt::Key_8;
	case OIS::KC_NUMPAD9     : return Qt::Key_9;
	case OIS::KC_SUBTRACT    : return Qt::Key_Minus;    // - on numeric keypad
	case OIS::KC_NUMPAD4     : return Qt::Key_4;
	case OIS::KC_NUMPAD5     : return Qt::Key_5;
	case OIS::KC_NUMPAD6     : return Qt::Key_6;
	case OIS::KC_ADD         : return Qt::Key_Plus;    // + on numeric keypad
	case OIS::KC_NUMPAD1     : return Qt::Key_1;
	case OIS::KC_NUMPAD2     : return Qt::Key_2;
	case OIS::KC_NUMPAD3     : return Qt::Key_3;
	case OIS::KC_NUMPAD0     : return Qt::Key_0;
	case OIS::KC_DECIMAL     : return Qt::Key_Period;    // . on numeric keypad
//	case OIS::KC_OEM_102     : return 0;             //no f*cking clue....    // < > | on UK/Germany keyboards
	case OIS::KC_F11         : return Qt::Key_F11;
	case OIS::KC_F12         : return Qt::Key_F12;
	case OIS::KC_F13         : return Qt::Key_F13;    //                     (NEC PC98)
	case OIS::KC_F14         : return Qt::Key_F14;    //                     (NEC PC98)
	case OIS::KC_F15         : return Qt::Key_F15;    //                     (NEC PC98)
	case OIS::KC_KANA        : return Qt::Key_Katakana; //NO IDEA IF IT'S CORRECT    // (Japanese keyboard)
	//case OIS::KC_ABNT_C1     : return 0;    //NO IDEA... / ? on Portugese (Brazilian) keyboards
	case OIS::KC_CONVERT     : return Qt::Key_Kanji;    // (Japanese keyboard)
	case OIS::KC_NOCONVERT   : return Qt::Key_Muhenkan;    // (Japanese keyboard)
	case OIS::KC_YEN         : return Qt::Key_yen;    // (Japanese keyboard)
	//case OIS::KC_ABNT_C2     : return 0;    // NO IDEA Numpad . on Portugese (Brazilian) keyboards
	case OIS::KC_NUMPADEQUALS: return Qt::Key_Equal;    // = on numeric keypad (NEC PC98)
	case OIS::KC_PREVTRACK   : return Qt::Key_MediaPrevious;    // Previous Track (OIS::KC_CIRCUMFLEX on Japanese keyboard)
	case OIS::KC_AT          : return Qt::Key_At;    //                     (NEC PC98)
	case OIS::KC_COLON       : return Qt::Key_Colon;    //                     (NEC PC98)
	case OIS::KC_UNDERLINE   : return Qt::Key_Underscore;    //                     (NEC PC98)
	case OIS::KC_KANJI       : return Qt::Key_Kanji;    // (Japanese keyboard)
	case OIS::KC_STOP        : return Qt::Key_Stop;    //                     (NEC PC98)
	//case OIS::KC_AX          : return 0;    //NO IDEA....                     (Japan AX)
	//case OIS::KC_UNLABELED   : return 0;    //NO IDEA....                        (J3100)
	case OIS::KC_NEXTTRACK   : return Qt::Key_MediaNext;    // Next Track
	case OIS::KC_NUMPADENTER : return Qt::Key_Enter;    //OR IS IT Key_Return?! Enter on numeric keypad
	case OIS::KC_RCONTROL    : return Qt::Key_Control;
	case OIS::KC_MUTE        : return Qt::Key_VolumeMute;    // Mute
	//case OIS::KC_CALCULATOR  : return 0;    // Calculator
	case OIS::KC_PLAYPAUSE   : return Qt::Key_Pause;    // Play / Pause
	case OIS::KC_MEDIASTOP   : return Qt::Key_Stop;    // Media Stop
	case OIS::KC_VOLUMEDOWN  : return Qt::Key_VolumeDown;    // Volume -
	case OIS::KC_VOLUMEUP    : return Qt::Key_VolumeUp;    // Volume +
	case OIS::KC_WEBHOME     : return Qt::Key_HomePage;    // Web home
	case OIS::KC_NUMPADCOMMA : return Qt::Key_Comma;    // ; on numeric keypad (NEC PC98)
	case OIS::KC_DIVIDE      : return Qt::Key_Slash;    // / on numeric keypad
	case OIS::KC_SYSRQ       : return Qt::Key_SysReq;
	case OIS::KC_RMENU       : return Qt::Key_Alt;    // right Alt
	case OIS::KC_PAUSE       : return Qt::Key_Pause;    // Pause
	case OIS::KC_HOME        : return Qt::Key_Home;    // Home on arrow keypad
	case OIS::KC_UP          : return Qt::Key_Up;    // UpArrow on arrow keypad
	case OIS::KC_PGUP        : return Qt::Key_PageUp;    // PgUp on arrow keypad
	case OIS::KC_LEFT        : return Qt::Key_Left;    // LeftArrow on arrow keypad
	case OIS::KC_RIGHT       : return Qt::Key_Right;    // RightArrow on arrow keypad
	case OIS::KC_END         : return Qt::Key_End;    // End on arrow keypad
	case OIS::KC_DOWN        : return Qt::Key_Down;    // DownArrow on arrow keypad
	case OIS::KC_PGDOWN      : return Qt::Key_PageDown;    // PgDn on arrow keypad
	case OIS::KC_INSERT      : return Qt::Key_Insert;    // Insert on arrow keypad
	case OIS::KC_DELETE      : return Qt::Key_Delete;    // Delete on arrow keypad
	case OIS::KC_LWIN        : return Qt::Key_Super_L;    // Left Windows key I HOPE THIS IS RIGHT
	case OIS::KC_RWIN        : return Qt::Key_Super_R;    // Right Windows key
	//case OIS::KC_APPS        : return 0;    // AppMenu key
	//case OIS::KC_POWER       : return 0;    // System Power
	case OIS::KC_SLEEP       : return Qt::Key_Sleep;    // System Sleep
	//case OIS::KC_WAKE        : return 0;    // System Wake
	case OIS::KC_WEBSEARCH   : return Qt::Key_Search;    // Web Search
	case OIS::KC_WEBFAVORITES: return Qt::Key_Favorites;    // Web Favorites
	case OIS::KC_WEBREFRESH  : return Qt::Key_Refresh;    // Web Refresh
	case OIS::KC_WEBSTOP     : return Qt::Key_Stop;    // Web Stop
	case OIS::KC_WEBFORWARD  : return Qt::Key_Forward;    // Web Forward
	case OIS::KC_WEBBACK     : return Qt::Key_Back;    // Web Back
	//case OIS::KC_MYCOMPUTER  : return 0;    // My Computer
	case OIS::KC_MAIL        : return Qt::Key_LaunchMail;    // Mail
	case OIS::KC_MEDIASELECT : return Qt::Key_Select;     // Media Select
    default:return Qt::Key_unknown;
    }
}

//
//CEGUI::String ceguiScanToString(CEGUI::Key::Scan s)
//{
//    switch(s)
//    {        
//        case CEGUI::Key::One: return "1"; 
//        case CEGUI::Key::Two: return "2"; 
//        case CEGUI::Key::Three: return "3"; 
//        case CEGUI::Key::Four: return "4"; 
//        case CEGUI::Key::Five: return "5"; 
//        case CEGUI::Key::Six: return "6"; 
//        case CEGUI::Key::Seven: return "7"; 
//        case CEGUI::Key::Eight: return "8"; 
//        case CEGUI::Key::Nine: return "9"; 
//        case CEGUI::Key::Zero: return "0"; 
//        case CEGUI::Key::Minus: return "-";     /* - on main keyboard */
//        case CEGUI::Key::Equals: return "="; 
//       
//        case CEGUI::Key::Tab: return "  "; 
//        case CEGUI::Key::Q: return "q"; 
//        case CEGUI::Key::W: return "w"; 
//        case CEGUI::Key::E: return "e"; 
//        case CEGUI::Key::R: return "r"; 
//        case CEGUI::Key::T: return "t"; 
//        case CEGUI::Key::Y: return "y"; 
//        case CEGUI::Key::U: return "u"; 
//        case CEGUI::Key::I: return "i"; 
//        case CEGUI::Key::O: return "o"; 
//        case CEGUI::Key::P: return "p"; 
//        case CEGUI::Key::LeftBracket: return "("; 
//        case CEGUI::Key::RightBracket: return ")"; 
//        
//        
//        case CEGUI::Key::A: return "a"; 
//        case CEGUI::Key::S: return "s"; 
//        case CEGUI::Key::D: return "d"; 
//        case CEGUI::Key::F: return "f"; 
//        case CEGUI::Key::G: return "g"; 
//        case CEGUI::Key::H: return "h"; 
//        case CEGUI::Key::J: return "j"; 
//        case CEGUI::Key::K: return "k"; 
//        case CEGUI::Key::L: return "l"; 
//        case CEGUI::Key::Semicolon: return ";"; 
//        case CEGUI::Key::Apostrophe: return "'"; 
//        case CEGUI::Key::Grave: return "`";     /* accent grave */
//         
//        case CEGUI::Key::Backslash: return "\\"; 
//        case CEGUI::Key::Z: return "z"; 
//        case CEGUI::Key::X: return "x"; 
//        case CEGUI::Key::C: return "c"; 
//        case CEGUI::Key::V: return "v"; 
//        case CEGUI::Key::B: return "b"; 
//        case CEGUI::Key::N: return "n"; 
//        case CEGUI::Key::M: return "m"; 
//        case CEGUI::Key::Comma: return ","; 
//        case CEGUI::Key::Period: return ".";     /* . on main keyboard */
//        case CEGUI::Key::Slash: return "/";     /* '/' on main keyboard */
//        
//        case CEGUI::Key::Multiply: return "*";     /* * on numeric keypad */
//        
//        case CEGUI::Key::Space: return " "; 
//        //case CEGUI::Key::Capital: return ""; 
//        
//       
//        case CEGUI::Key::Numpad7: return "7"; 
//        case CEGUI::Key::Numpad8: return "8"; 
//        case CEGUI::Key::Numpad9: return "9"; 
//        case CEGUI::Key::Subtract: return "-";     /* - on numeric keypad */
//        case CEGUI::Key::Numpad4: return "4"; 
//        case CEGUI::Key::Numpad5: return "5"; 
//        case CEGUI::Key::Numpad6: return "6"; 
//        case CEGUI::Key::Add: return "+";     /* + on numeric keypad */
//        case CEGUI::Key::Numpad1: return "1"; 
//        case CEGUI::Key::Numpad2: return "2"; 
//        case CEGUI::Key::Numpad3: return "3"; 
//        case CEGUI::Key::Numpad0: return "0"; 
//        case CEGUI::Key::Decimal: return ",";     /* . on numeric keypad */
//        //case CEGUI::Key::OEM: return "";     /* < > | on UK/Germany keyboards */
//        
//        
//
//        case CEGUI::Key::NumpadEquals: return "=";     /* = on numeric keypad (NEC PC98) */
//        
//        
//        case CEGUI::Key::NumpadComma: return ",";  
//        case CEGUI::Key::Divide: return "/";     /* / on numeric keypad */
//        default: return "";
//    };
//}

EditorApp::EditorApp(Ogre::String cmdLine):
	configXml(NULL),
		mTerrainGlobals(NULL)
{
    tEditMode = teDeform;
//    flatteningToClick = false;
//   brushScalingInProgress = false;
    intensityChangeInProgress = false;
    showConfigDialog = false;
    listboxTypeText = "";//wenn man im listbox tippt
    timeSinceLastLBDown = 0;
//   lastLB = NULL;
//    terrainFlattening = false;
    terrainIntensity = 50;
//   brushScale = 8;
	currentTexture = 0;
//	paintingTextures = false;
//wmgr = NULL;
	editingMode = emLevel;
	savingMapImageState = 0;
//	dotSceneMode = false;
	snapOnGrid = false;
	snapOnAngle = false;
	snapOnScale = false;
//	loadTerrainCollision = false;
	showFog = false;
	showDebugWnd = true;
	gridSnap = 0.5f;
	scaleSnap = 0.01;
	angleSnap= 45;//15;

	axesScaleFactor = 0.1;
	windowName = "LoL 4 Leveleditor";
	camStartPos = Ogre::Vector3(0,30,0);
	

    paramList = Ogre::StringUtil::split(cmdLine," ");

	curEditing = NULL;

    currentLevel = NULL;
	oldLevel = NULL;
    //mFrameListener = NULL;
    mRoot = 0;
	sensibility_x = 0.25;
	sensibility_y = 0.25;
//mEditorGuiSheet = NULL;
	mWindow = NULL;
//detailswnd = NULL;


	conf_lookspeed = 75;
//mGUISystem = NULL;
//mGUIRenderer = NULL;

//fpsBox = NULL;


	menuMode = 0;//GUI_DISABLE;
	paused = true;
	shutDown = false;
	cursorMode = false;
//	okToProcessEvent = true;

	/*dragItemType = 0;
	dragItemID   = "";*/

	//barDimensions = CEGUI::UVector2(CEGUI::UDim(1,0),CEGUI::UDim(0,35));//was war das nochmal....?


    qtApp = NULL;
//    /*** initializing the QT app ***/
//    //@todo: do this directly with the cmdLine
//
//    char *command_line = GetCommandLine();
//
//    //argc/argv code
//    int    argc;
//    char** argv;
//
//    char*  arg;
//    int    index;
////        int    result;
//
//    // count the arguments
//
//    argc = 1;
//    arg  = command_line;
//
//    while (arg[0] != 0) {
//
//        while (arg[0] != 0 && arg[0] == ' ') {
//            arg++;
//        }
//
//        if (arg[0] != 0) {
//            argc++;
//            while (arg[0] != 0 && arg[0] != ' ') {
//                arg++;
//            }
//        }
//    }    
//
//    // tokenize the arguments
//
//    argv = (char**)malloc(argc * sizeof(char*));
//
//    arg = command_line;
//    index = 1;
//
//    while (arg[0] != 0) {
//        while (arg[0] != 0 && arg[0] == ' ') {
//            arg++;
//        }
//        if (arg[0] != 0) {
//
//            argv[index] = arg;
//            index++;
//
//            while (arg[0] != 0 && arg[0] != ' ') {
//                arg++;
//            }
//
//            if (arg[0] != 0) {
//                arg[0] = 0;    
//                arg++;
//            }
//        }
//    }   
//
//    // put the program name into argv[0]
//
//    char filename[_MAX_PATH];
//
//    GetModuleFileName(NULL, filename, _MAX_PATH);
//    argv[0] = filename;
//
//    // call the user specified main function    
//
//    
//
//   qtApp = new QtOgre::QtEditorApp(argc, argv, new QtOgre::EditGameLogic);
//   //result = main(argc, argv);
//
//   free(argv);

}

void EditorApp::initializeQt()
{
    char** argv;
    int argc = paramList.size()+1;//paramList enthält NICHT den programmnamen!
    
    //das initiiert also das array von c-strings
    argv = (char**)malloc(argc * sizeof(char*));
    
    for(int i=0;i<argc;i++)
    {
        char* curString;
        if(i == 0)
        {
            char filename[_MAX_PATH];

            GetModuleFileName(NULL, filename, _MAX_PATH);
            curString = filename;             
        }
        else
        {
            //DIES KÖNNTE DER GRUND FÜR IRGENDWELCHE FEHLER SEIN!!
            //wenn es das ist, durch strcpy ersetzen
            curString = const_cast<char*>(paramList[i-1].c_str());
        }
        argv[i] = curString;
    }
    //nun sollte ich argc und argv richtig haben...
    qtApp = new QtOgre::QtEditorApp(argc, argv, new QtOgre::EditGameLogic);
    free(argv);
}

EditorApp::~EditorApp()
{

	
	if(mTerrainGlobals)
	{
		OGRE_DELETE mTerrainGlobals;
	}

	if(mWindow)
	{
		if(mWindow->getNumViewports())
			mWindow->removeAllViewports();
	}
	//mLog("ok, aus");
    
	
	
	if (qtApp && qtApp->getGameLogic())
	{
		saveConfig();
		//mRoot->removeFrameListener(mFrameListener);
        //delete mFrameListener;
	}


	//delete mKeyListener;
	/*if(mEventProcessor)
		delete mEventProcessor;*/
	//delete mOgreNewtListener;

	/*if(mWorld)
		delete mWorld;*/
	//log("ok, aus2");

	
	


	
//OgreNewt::Debugger::getSingleton().deInit();
	if(currentLevel)
		delete currentLevel;

	/*if(plsm2SceneMgr)
		mRoot->destroySceneManager(plsm2SceneMgr);*/
	
//QTAPP MACHT DAS
	//if (mRoot)
 //       delete mRoot;

}

//void EditorApp::go()
//{
//
//    if (!setup())
//        return;
//
//    mRoot->startRendering();
//
//    // clean up
//    destroyScene();
//}

//
//bool EditorApp::setup(void)
//{	
// //   TiXmlDocument *doc = loadConfigXml();
// //   setupPlugins(doc->FirstChildElement("Plugins"));
// //   ////game und editor können damit verschiedene renderer settings haben, zB für mehr Leistung beim Editieren
// //   ////zugunsten der Grafik
// //   setupRenderSystem(doc->FirstChildElement("editor")->FirstChildElement("Renderer"));
// //   //NOT YET. this has to be stolen form QtEditorApp!
//
// //   setupResources(doc->FirstChildElement("Resources"));
//
// //   
//	////HIER muss das root initialisiert werden!
//	//qtApp->initialise();
//	//
//
// //   //bool carryOn = configure();
// //   //if (!carryOn) return false;
//
//	//createResourceListener();
//	//// Load resources
//	//loadResources();
//
// //   loadManualLoD();
//
//	//loadGameData();
//
//
//	//createDefaultSceneMgr();
//
//
//
//
//	//createGUI();
//	//
//
//	//createTerrainBrushes();
//
//
// //   setupConfig(doc->FirstChildElement("editor"));
//	////loadConfig();
//
// //   initSound();
//
//	//qtApp->postInit();
//
//	//loadLevel("collisionOffsetTest.zip");//TEST
// //   delete doc;
//    return true;
//}

/*

bool EditorApp::setup(void)
{
	//mRoot = new Ogre::Root();
	mRoot = new Ogre::Root("","",mLogFileName);
	for(vector<Ogre::String>::iterator itr = paramList.begin();itr != paramList.end();itr++)
	{
		Ogre::String param = *itr;
		if(param == "-config")
		{
			showConfigDialog = true;
		}
		Ogre::LogManager::getSingleton().logMessage("param="+param);
	}
     //ok, let's XML-parse
   
    TiXmlDocument *doc = loadConfigXml();
    setupPlugins(doc->FirstChildElement("Plugins"));
    //game und editor können damit verschiedene renderer settings haben, zB für mehr Leistung beim Editieren
    //zugunsten der Grafik
    setupRenderSystem(doc->FirstChildElement("editor")->FirstChildElement("Renderer"));
    setupResources(doc->FirstChildElement("Resources"));

    

	

    bool carryOn = configure();
    if (!carryOn) return false;

	createResourceListener();
	// Load resources
	loadResources();

    loadManualLoD();

	loadGameData();


	createDefaultSceneMgr();




	createGUI();
	createTerrainBrushes();
	//log("nach createGUI();");
	//setMenu(GUI_MAINMENU);
    createFrameListener();

    setupConfig(doc->FirstChildElement("editor"));
	//loadConfig();

    initSound();


	loadLevel("");
    delete doc;
    return true;
}
*/




void EditorApp::createTerrainBrushes()
{
//    Ogre::StringVectorPtr brushes = Ogre::ResourceGroupManager::getSingletonPtr()->findResourceNames("ETBrushes","*");
//	//CEGUI::Listbox* dsList = (CEGUI::Listbox*)wmgr->getWindow((CEGUI::utf8*)LIST_STATIC_DOTSCENES);
//	//dsList->subscribeEvent(CEGUI::Window::EventMouseButtonDown,CEGUI::Event::Subscriber(&EditorApp::sceneListDown,this));
////   mBrushes.clear();
////   mBrushNames.clear();
//    for (Ogre::StringVector::iterator i = brushes->begin(); i != brushes->end(); ++i)
//	{
//		Ogre::String filename = *i;
//        Ogre::Image image;
//	    image.load(filename, "ETBrushes");
//        image.resize(brushScale,brushScale);
//        ET::Brush brush = ET::loadBrushFromImage(image);
//        mBrushes.push_back(brush);
//        mBrushNames.push_back(filename);
//        //currentBrush = ET::loadBrushFromImage(image);
//		//CEGUI::ListboxTextItem *listboxitem = new CEGUI::ListboxTextItem(wtf.c_str());
//		//listboxitem->setSelectionBrushImage(STYLE_EDITOR,"DefaultSelectionBrush");
//		//listboxitem->setTextColours(clBlack);
//		//dsList->addItem(listboxitem);
//	}
//	currentBrushIndex = 0;
}

	/** Configures the application - returns false if the user chooses to abandon configuration. */
bool EditorApp::configure(void)
{
   // Show the configuration dialog and initialise the system
    // You can skip this and use root.restoreConfig() to load configuration
    // settings if you were sure there are valid ones saved in ogre.cfg
	//if(showConfigDialog)    
	//	if(!mRoot->showConfigDialog())
	//	{
	//		return false;
	//	}
	
    // If returned true, user clicked OK so initialise
    // Here we choose to let the system create a default rendering window by passing 'true'
    // mWindow = mRoot->initialise(true, WINDOW_NAME);
    mWindow = qtApp->ogreRenderWindow();
//#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
//	HWND hwnd;
//	mWindow->getCustomAttribute("WINDOW", (void*)&hwnd);
//	LONG iconID   = (LONG)LoadIcon( GetModuleHandle(0), MAKEINTRESOURCE(IDI_ICON1) );
//	SetClassLong( hwnd, GCL_HICON, iconID );
//#endif
//    return true;
//    
   return true;
}
void EditorApp::createDefaultSceneMgr(void)
{
// Create the Ogre::SceneManager, in this case a generic one
//-----------------------------------------------------
// 4 Create the Ogre::SceneManager
//
//		ST_GENERIC = octree
//		ST_EXTERIOR_CLOSE = simple terrain
//		ST_EXTERIOR_FAR = nature terrain (depreciated)
//		ST_EXTERIOR_REAL_FAR = paging landscape
//		ST_INTERIOR = Quake3 BSP
//-----------------------------------------------------
	mDefaultSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC,"DefaultSceneManager");//, "ExampleSMInstance");
	
	//((PCZSceneManager*)mDefaultSceneMgr)->

	mDefaultCam = mDefaultSceneMgr->createCamera("main");
	//mDefaultCam->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
	mDefaultCamNode = mDefaultSceneMgr->getRootSceneNode()->createChildSceneNode();
	mDefaultCamNode->attachObject(mDefaultCam);
	
    Ogre::Viewport* vp = mWindow->addViewport(mDefaultCam);
	vp->setBackgroundColour(Ogre::ColourValue(0,0,0));
	mDefaultCam->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));


}

void EditorApp::initTerrainGlobals()
{
	mTerrainGlobals = OGRE_NEW Ogre::TerrainGlobalOptions();

	mTerrainGlobals->setDefaultResourceGroup("Terrain");


	// this should be defineable in the game itself, I think
    mTerrainGlobals->setMaxPixelError(8);
    // this too, probaby
    mTerrainGlobals->setCompositeMapDistance(3000);
 
	Ogre::Vector3 lightMapDir(-1,-1,0);
	lightMapDir.normalise();

    // Important to set these so that the terrain knows what to use for derived (non-realtime) data
	//I think these can be overridden
    mTerrainGlobals->setLightMapDirection(lightMapDir);
    mTerrainGlobals->setCompositeMapAmbient(Ogre::ColourValue::White);
    mTerrainGlobals->setCompositeMapDiffuse(Ogre::ColourValue::White);

}

void EditorApp::createFrameListener(void)
{
    //should alerady exist...
	
	//mFrameListener = new EditFrameListener();//this,mEventProcessor->getInputReader(),mGUIRenderer,mWindow, mSceneMgr, mWorld, 120 );

	//mRoot->addFrameListener(mFrameListener);
	//
	//log("frame listener online");

	//zwar etwas unsauber, aber hier config laden
	



	
}


	/// Optional override method where you can create resource listeners (e.g. for loading screens)
void EditorApp::createResourceListener(void)
{

}

	/// Optional override method where you can perform resource group loading
	/// Must at least do Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups(){}
//void EditorApp::loadResources(void)
//{
//Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
//	log("res init done");
//}
//void EditorApp::loadGameData(void)
//{
////------------------------------------------------
//	//PRA: readng object config file
//	//------------------------------------------------
//	Ogre::ConfigFile cf;
//	cf.loadFromResourceSystem("objects.cfg","game_data");//,"\t:=");
//
//
//	// Go through all sections & settings in the file
//	Ogre::ConfigFile::SectionIterator seci2 = cf.getSectionIterator();
//
//	Ogre::String c_name, c_value, secName;
//	//alerady defined
//	while (seci2.hasMoreElements())
//	{
//		secName = seci2.peekNextKey();
//
//		game_obj_static temp;
//		temp.id=secName;
//		temp.material = "";
//		temp.mesh = ERROR_MESH;
//		temp.scale = Ogre::Vector3::UNIT_SCALE;
//		temp.mass = 0;
//		temp.collisionParams = Ogre::Vector3(1,1,1);
//		temp.collisionType = 0;
//
//		bool valid = true;
//
//		Ogre::ConfigFile::SettingsMultiMap *settings = seci2.getNext();
//		Ogre::ConfigFile::SettingsMultiMap::iterator i;
//		for (i = settings->begin(); i != settings->end(); ++i)
//		{
//			/*-----------------
//			This loads the global static definition file
//			[secName]
//			c_name=c_value
//			------------------*/
//			if(secName != "")
//			{
//				c_name = i->first;
//				Ogre::StringUtil::toLowerCase(c_name);
//				c_value = i->second;
//
//				//Ogre::StringUtil::toLowerCase(c_name);
//				log("c_name="+c_name);
//				if(c_name == Ogre::String("mesh"))
//				{
//					temp.mesh = c_value;
//					//settext("juhu");
//				}
//				else if(c_name == Ogre::String("material"))
//				{
//					temp.material = c_value;
//					//settext("juhu");
//				}
//				else if(c_name== Ogre::String("scale"))
//				{
//					temp.scale = Ogre::StringConverter::parseVector3(c_value);
//					if(temp.scale == Ogre::Vector3::ZERO)
//					{
//						//perhaps it's just one number?
//						Ogre::Real scale_1 = Ogre::StringConverter::parseReal(c_value);
//						if(scale_1 == 0.0f)
//						{
//							//invalid => set it to 1
//							temp.scale = Ogre::Vector3(1,1,1);
//						}
//						else
//						{
//							temp.scale = Ogre::Vector3(scale_1,scale_1,scale_1);
//						}
//					}
//				}
//				else if(c_name==Ogre::String("mass"))
//				{
//					temp.mass=Ogre::StringConverter::parseReal(c_value);
//				}
//				else if(c_name==Ogre::String("collisionparams"))
//				{
//					temp.collisionParams=Ogre::StringConverter::parseVector3(c_value);
//				}
//				else if(c_name==Ogre::String("collisiontype"))
//				{
//					temp.collisionType=StringToColType(c_value);
//					//perhaps no collision at all?
//
//					if (!temp.collisionType || temp.collisionType == CT_TREECOLLISION)
//					{
//						temp.mass=0;
//						//no need of a mass here
//					}
//				}
//			}
//		}
//		if(valid)
//		{
//			objects[secName] = temp;
//		}
//		log("nächstes");
//	}
//
//	cf.clear();
//	cf.loadFromResourceSystem("datamap_item.cfg","game_data");//,"\t:=");
//
//
//	// Go through all sections & settings in the file
//	seci2 = cf.getSectionIterator();
//    //Ogre::ConfigFile::SectionIterator seci3 = cf.getSectionIterator();
//	//Ogre::String c_name, c_value, secName;
//	//alerady defined
//	while (seci2.hasMoreElements())
//	{
//		secName = seci2.peekNextKey();
//
//
//		/*struct game_item
//{
//	Ogre::String id;
//	Ogre::String name;
//	Ogre::String mesh;
//	Ogre::String material;
//	Ogre::Vector3 scale;
//	Ogre::String inv_image;
//	Ogre::Real mass;
//	short collisionType;
//	Ogre::Vector3 collisionParams;
//};*/
//		game_item temp;
//		temp.id=secName;
//		temp.material = "";
//		temp.name = ERROR_NONAME;
//		temp.mesh = ERROR_MESH;
//		temp.scale = Ogre::Vector3::UNIT_SCALE;
//		temp.inv_image = ERROR_INVIMAGE;
//		temp.mass = 1;
//		temp.collisionParams = Ogre::Vector3(1,1,1);
//		temp.collisionType = CT_BOX;
//
//		bool valid = true;
//
//
//		Ogre::ConfigFile::SettingsMultiMap *settings = seci2.getNext();
//		Ogre::ConfigFile::SettingsMultiMap::iterator i;
//		for (i = settings->begin(); i != settings->end(); ++i)
//		{
//			/*-----------------
//			This loads the global static definition file
//			[secName]
//			c_name=c_value
//			------------------*/
//			c_name = i->first;
//			Ogre::StringUtil::toLowerCase(c_name);
//			c_value = i->second;
//
//			//Ogre::StringUtil::toLowerCase(c_name);
//			if(c_name == Ogre::String("mesh"))
//			{
//				temp.mesh = c_value;
//			}
//			else if(c_name == Ogre::String("inv_image"))
//			{
//				temp.inv_image = c_value;
//			}
//			else if(c_name == Ogre::String("name"))
//			{
//				temp.name = c_value;
//			}
//			else if(c_name == Ogre::String("material"))
//			{
//				temp.material = c_value;
//			}
//			else if(c_name== Ogre::String("scale"))
//			{
//				temp.scale = Ogre::StringConverter::parseVector3(c_value);
//				if(temp.scale == Ogre::Vector3::ZERO)
//				{
//					//perhaps it's just one number?
//					Ogre::Real scale_1 = Ogre::StringConverter::parseReal(c_value);
//					if(scale_1 == 0.0f)
//					{
//						//invalid => set it to 1
//						temp.scale = Ogre::Vector3(1,1,1);
//					}
//					else
//					{
//						temp.scale = Ogre::Vector3(scale_1,scale_1,scale_1);
//					}
//				}
//			}
//			else if(c_name==Ogre::String("mass"))
//			{
//				temp.mass=Ogre::StringConverter::parseReal(c_value);
//			}
//			else if(c_name==Ogre::String("collisionparams"))
//			{
//				temp.collisionParams=Ogre::StringConverter::parseVector3(c_value);
//			}
//			else if(c_name==Ogre::String("collisiontype"))
//			{
//				temp.collisionType=StringToColType(c_value);
//				if (!temp.collisionType || temp.collisionType == CT_TREECOLLISION)
//				{
//					valid = false;
//					//this is not a valid item
//				}
//			}
//
//		}
//		if(!valid)
//		{
//			log("ERROR: item "+temp.id+" is not valid. skipping");
//		}
//		else
//		{
//			datamap_item[secName] = temp;
//		}
//		log("nächstes");
//	}
//	//game_obj asdf = objects["robo"];
//	//settext(asdf.name+" / "+asdf.mesh);
//	//END---------------------------------------------
//	log("game data geladen");
//
//}
//
//
//
void EditorApp::setObjectInfo(Ogre::String text)
{
//	objInfo->setText(text.c_str());
}
	
void EditorApp::loadLevel(Ogre::String filename)
{
    
	//zipTest();
	/*******************************************************************************************
	***************                     BEGIN OLD CODE                    ***********************/
	if(!paused)
		paused = true;
    
	if(currentLevel)
	{
        if(editingMode == emTerrain)
            setEditorMode(emLevel);
            //setTerrainEditing(false);
		qtApp->getGameLogic()->hideAxes();
		//mFrameListener->hideAxes();
		//auf jeden Fall VOR dem neuen level erledigen!
//	OgreNewt::Debugger::getSingleton().deInit();
		currentLevel->prepareForDestruction();
        
        
		
	}

	Level *newLevel = new Level(filename);

	qtApp->getGameLogic()->levelChangeBegin(newLevel);
	//newton debuger stuff
	
		

	Ogre::SceneManager *mSceneMgr = newLevel->getSceneManager();

//OgreNewt::Debugger::getSingleton().init( mSceneMgr );

	Ogre::Camera *newCam = newLevel->getMainCam();
	Ogre::Viewport* vp = mWindow->getViewport(0);
	vp->setCamera(newCam);
	
	
	// Alter the camera aspect ratio to match the viewport
    Ogre::Real actualW = vp->getActualWidth();
    Ogre::Real actualH = vp->getActualHeight();
	newCam->setAspectRatio(actualW / actualH);



	//mGUIRenderer->setTargetSceneManager(mSceneMgr);

	oldLevel = currentLevel;

	//nicht jetzt, erst wenn die update() schleife durch ist
	//delete currentLevel;
	
	//das hier ist der editor. mal testen...
	delete currentLevel;
	oldLevel = NULL;

	currentLevel = newLevel;



	Ogre::SceneNode *CamNode = NULL;	
	Ogre::SceneNode *CamHeadNode = NULL;

	if(mSceneMgr->hasSceneNode("CamNode")) 
	{
		
		CamNode = mSceneMgr->getSceneNode("CamNode");	
		/**/
		if(mSceneMgr->hasSceneNode("CamHeadNode"))
		{
			CamHeadNode = (Ogre::SceneNode*)CamNode->getChild("CamHeadNode");
		}
		else
		{
			CamHeadNode = CamNode->createChildSceneNode("CamHeadNode");

		}
		if(newCam->getParentSceneNode() != CamHeadNode)		
		{
			if(newCam->getParentSceneNode())
			{
				newCam->getParentSceneNode()->detachObject(newCam);
			}
			CamHeadNode->attachObject(newCam);

		}
		
		

		//mSceneMgr->destroySceneNode("CamNode");
	}
	else
	{	  
		CamNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("CamNode");	
		CamHeadNode = CamNode->createChildSceneNode("CamHeadNode",Ogre::Vector3(0,0,0));
		CamHeadNode->attachObject(newCam);
	}
	
	
	
	CamNode->setPosition(camStartPos);
	CamHeadNode->pitch(Ogre::Degree(-90));
	qtApp->getGameLogic()->levelChangeEnd();
	updateCurObjectsList();
	//titel setzen
	Ogre::String newTitle;
	if(currentLevel->getName() != "")
	{
		newTitle = currentLevel->getName()+" ("+currentLevel->getFileName()+") - "+windowName;
	}
	else
	{
		if(currentLevel->getFileName() != "")
		{
			newTitle = currentLevel->getFileName()+" - "+windowName;
		}
		else
		{
			newTitle = "new level - "+windowName;
		}
	}
	setWindowTitle(newTitle);

	terrainDeformed = false;

	
	/************************        END OLD CODE            ******************************
	**************************************************************************************/

}

void EditorApp::updateCurObjectsList(GameObject *changed, bool added)
{
	//ERSTMAL NIX
}
//
//void EditorApp::updateTerrainWindow()
//{
//   // CEGUI::Window *terrainwnd = wmgr->getWindow("editterrain");
//	//CEGUI::Window *objectwnd = wmgr->getWindow("objects");
//	//CEGUI::Window *curwnd = wmgr->getWindow("curobjects");
//    
//    
//
//    //scale fenster updaten
//    static_cast<CEGUI::Spinner*>(wmgr->getWindow("editterrain/brushscale_value"))->setCurrentValue(brushScale);
//
//	updateTexturesList();
//	//for(unsigned int i = 0;i<texcnt;i++)
//	//{
//	//	//CEGUI::ListboxTextItem *it = new CEGUI::ListboxTextItem(CEGUI::String("texture")+str(i).c_str());
//	//	CEGUI::String name = Ogre::String("editterrain/texturewnd"+str(i)).c_str();
//	//	CEGUI::Window *texfield;
//	//	
//	//	texfield = wmgr->createWindow("WindowsLook/StaticImage",name);
//	//	
//	//	texfield->subscribeEvent(CEGUI::Window::EventMouseClick,CEGUI::Event::Subscriber(&EditorApp::textureSelectClick,this));
//
//	//	texList->addChildWindow(texfield);
//	//	if(currentTexture != i)
//	//		texfield->setAlpha(0.5);
//	//	texfield->setID(i);
//	//	texfield->setProperty("HorzFormatting", "Tiled");
//	//	texfield->setProperty("VertFormatting", "Tiled");
//	//	texfield->setArea(CEGUI::UDim(0,0),CEGUI::UDim(0,i*52),CEGUI::UDim(1,-20),CEGUI::UDim(0,50));
//
//	//	//// Set the CEGUI Button's image to our new RTT
//	//	//
//	//	//Ogre::TexturePtr ogreTex = Ogre::TextureManager::getSingletonPtr()->getByName(texName);
//	//	//CEGUI::Texture* ceguiTex = mGUIRenderer->createTexture(ogreTex);
//
//	//	//CEGUI::String imageSetName = "compassimgset";
//	//	//Imageset* textureImageSet = ImagesetManager::getSingleton().createImageset(imageSetName, ceguiTex);
//
//	//	//textureImageSet->defineImage("compassneedle", Point(0.0f, 0.0f), Size(ceguiTex->getWidth(), ceguiTex->getHeight()), Point(0.0f,0.0f));
//	//	////imageSetName = "set:compassimgset image:compassneedle";
//
//	//	//compassWnd->setProperty("Image", "set:"+imageSetName+" image:compassneedle");
//
//	//	Ogre::String texname = terrainMat->getTechnique(0)->getPass(0)->getTextureUnitState(i+mapCnt)->getTextureName();//currentLevel->getSplattingBasename()+str(i)+".png";
//	//	ceguiSetImage(texfield,texname,"ET");
//	//	//Ogre::ResourceGroupManager::
//	//	/*it->setTextColours(CEGUI::colour(0,0,0));
//	//	it->setSelectionBrushImage(STYLE_EDITOR,"DefaultSelectionBrush");
//	//	texList->addItem(it);*/
//	//}
//    //jetzt brushes
//    updateBrushesWindow();
//
//    //die werte setzen
//    Ogre::AxisAlignedBox box = currentLevel->getTerrainExtends();
//    wmgr->getWindow("editterrain/minbounding")->setText(str(box.getMinimum()).c_str());
//    wmgr->getWindow("editterrain/maxbounding")->setText(str(box.getMaximum()).c_str());
//
//    Ogre::ColourValue etLightColor, etAmbientColor;
//    Ogre::Vector3 etLightDirection;
//    currentLevel->getTerrainLightSettings(etLightColor, etAmbientColor, etLightDirection);
//
//    wmgr->getWindow("editterrain/lightdir")->setText(str(etLightDirection).c_str());
//    wmgr->getWindow("editterrain/lightcolor")->setText(str(etLightColor).c_str());
//    wmgr->getWindow("editterrain/ambientcolor")->setText(str(etAmbientColor).c_str());
//
//}

//bool EditorApp::terrainsettingsApply(const CEGUI::EventArgs& e)
//{
//    //die werte setzen
//    
//    Ogre::Vector3 min = Ogre::StringConverter::parseVector3(wmgr->getWindow("editterrain/minbounding")->getText().c_str());
//    Ogre::Vector3 max = Ogre::StringConverter::parseVector3(wmgr->getWindow("editterrain/maxbounding")->getText().c_str());
//    Ogre::AxisAlignedBox box(min,max);
//
//  
//
//    Ogre::Vector3 etLightDirection = Ogre::StringConverter::parseVector3(wmgr->getWindow("editterrain/lightdir")->getText().c_str());
//    Ogre::ColourValue etLightColor = Ogre::StringConverter::parseColourValue(wmgr->getWindow("editterrain/lightcolor")->getText().c_str());
//    Ogre::ColourValue etAmbientColor = Ogre::StringConverter::parseColourValue(wmgr->getWindow("editterrain/ambientcolor")->getText().c_str());
//
//    currentLevel->setTerrainExtends(box);
//    currentLevel->setTerrainLightSettings(etLightColor,etAmbientColor,etLightDirection);
//    //nun das decal anpassen
//
//    qtApp->getGameLogic()->setDecalScale(brushScale);
//    return true;
//}

void EditorApp::updateTexturesList()
{
    
 //   CEGUI::ScrollablePane *texList = (CEGUI::ScrollablePane*)wmgr->getWindow("editterrain/texlist");
 //   //erstmal aufräumen
	//unsigned int b_i = 0;
	//Ogre::String name = Ogre::String("editterrain/texturewnd"+str(b_i));
 //   if(!wmgr->isWindowPresent(name.c_str()))
 //   {
 //       //das nächste versuchen. evtl hatte das keine holemap und ist bei 1 begonnen
 //       b_i = 1;
 //       name = Ogre::String("editterrain/texturewnd"+str(b_i));
 //   }
	////for(unsigned int i = 0;wmgr->isWindowPresent(Ogre::String("editterrain/texturewnd"+str(i));i++)
	//while(wmgr->isWindowPresent(name.c_str()))
	//{			
	//	wmgr->destroyWindow(name.c_str());
	//	b_i++;
	//	name = Ogre::String("editterrain/texturewnd"+str(b_i));
	//}

	//currentTexture = 0;

 //   unsigned int i = 0;
 //   for(Ogre::StringVector::iterator itr = currentLevel->terrainTextures.begin();itr!= currentLevel->terrainTextures.end();itr++)
 //   {
	//	if(i == 0 && currentLevel->hasHoleMap())
	//	{
	//		i++;
	//		continue;//die lochmap rausfiltern
	//	}
 //       CEGUI::String name = Ogre::String("editterrain/texturewnd"+str(i)).c_str();
	//	CEGUI::Window *texfield;
	//	
	//	texfield = wmgr->createWindow("WindowsLook/StaticImage",name);
	//	
	//	texfield->subscribeEvent(CEGUI::Window::EventMouseClick,CEGUI::Event::Subscriber(&EditorApp::textureSelectClick,this));

	//	texList->addChildWindow(texfield);
	//	if(currentTexture != i)
	//		texfield->setAlpha(0.5);
	//	texfield->setID(i);
	//	texfield->setProperty("HorzFormatting", "Tiled");
	//	texfield->setProperty("VertFormatting", "Tiled");
	//	texfield->setArea(CEGUI::UDim(0,0),CEGUI::UDim(0,i*52),CEGUI::UDim(1,-20),CEGUI::UDim(0,50));

	//	

	//	Ogre::String texname = *itr;
	//	ceguiSetImage(texfield,texname,"ETTextures");
 //       i++;
 //   }
}

//void EditorApp::updateBrushesWindow()
//{
//    CEGUI::ScrollablePane *brushList = (CEGUI::ScrollablePane*)wmgr->getWindow("editterrain/brushes_list");
//    //erstmal aufräumen
//	unsigned int b_i = 0;
//    Ogre::String name = Ogre::String("editterrain/brushwnd"+str(b_i));
//	//for(unsigned int i = 0;wmgr->isWindowPresent(Ogre::String("editterrain/texturewnd"+str(i));i++)
//	while(wmgr->isWindowPresent(name.c_str()))
//	{			
//		wmgr->destroyWindow(name.c_str());
//		b_i++;
//		name = Ogre::String("editterrain/brushwnd"+str(b_i));
//	}
//
//    for(unsigned int i = 0;i<mBrushes.size();i++)
//	{
//		//CEGUI::ListboxTextItem *it = new CEGUI::ListboxTextItem(CEGUI::String("texture")+str(i).c_str());
//		CEGUI::String name = Ogre::String("editterrain/brushwnd"+str(i)).c_str();
//		CEGUI::Window *texfield;
//		
//		texfield = wmgr->createWindow("WindowsLook/StaticImage",name);
//		
//		texfield->subscribeEvent(CEGUI::Window::EventMouseClick,CEGUI::Event::Subscriber(&EditorApp::brushSelectClick,this));
//
//		brushList->addChildWindow(texfield);
//		if(currentBrushIndex != i)
//			texfield->setAlpha(0.5);
//		texfield->setID(i);
//		texfield->setProperty("HorzFormatting", "Stretched");
//		texfield->setProperty("VertFormatting", "Stretched");
//		texfield->setArea(CEGUI::UDim(0,0),CEGUI::UDim(0,i*52),CEGUI::UDim(0,40),CEGUI::UDim(0,40));
//
//		//// Set the CEGUI Button's image to our new RTT
//		//
//		//Ogre::TexturePtr ogreTex = Ogre::TextureManager::getSingletonPtr()->getByName(texName);
//		//CEGUI::Texture* ceguiTex = mGUIRenderer->createTexture(ogreTex);
//
//		//CEGUI::String imageSetName = "compassimgset";
//		//Imageset* textureImageSet = ImagesetManager::getSingleton().createImageset(imageSetName, ceguiTex);
//
//		//textureImageSet->defineImage("compassneedle", Point(0.0f, 0.0f), Size(ceguiTex->getWidth(), ceguiTex->getHeight()), Point(0.0f,0.0f));
//		////imageSetName = "set:compassimgset image:compassneedle";
//
//		//compassWnd->setProperty("Image", "set:"+imageSetName+" image:compassneedle");
//
//		Ogre::String texname = mBrushNames[i];
//		ceguiSetImage(texfield,texname,"ETBrushes");
//		//Ogre::ResourceGroupManager::
//		/*it->setTextColours(CEGUI::colour(0,0,0));
//		it->setSelectionBrushImage(STYLE_EDITOR,"DefaultSelectionBrush");
//		texList->addItem(it);*/
//	}
//}
//

void EditorApp::setEditorMode(EditorMode mode)
{    
    editingMode = mode;
	if(mode == emTerrain)
		qtApp->getGameLogic()->mTerrainDecal->show();
	else
		qtApp->getGameLogic()->mTerrainDecal->hide();
}
//
void EditorApp::setTerrainEditing(bool enable)
{
}
//    if(!currentLevel->getTerrainInfo())
//        return;
//	editingTerrain = enable;
//	CEGUI::Window *terrainwnd = wmgr->getWindow("editterrain");
//	CEGUI::Window *objectwnd = wmgr->getWindow("objects");
//	CEGUI::Window *curwnd = wmgr->getWindow("curobjects");
//	//objects
//	//curobjects
//	if(!enable)
//	{
//		terrainwnd->disable();
//		terrainwnd->setVisible(false);
//		objectwnd->enable();
//		objectwnd->setVisible(true);
//		curwnd->enable();
//		curwnd->setVisible(true);
//        //das decal entfernen
//        mFrameListener->deleteDecal();
//
//        //collision neu erzeugen
//        if(terrainDeformed)
//            currentLevel->generateTerrainCollision();
//        terrainDeformed = false;
//	}
//	else
//	{
//		
//		terrainwnd->enable();
//		terrainwnd->setVisible(true);
//		objectwnd->disable();
//		objectwnd->setVisible(false);
//		curwnd->disable();
//		curwnd->setVisible(false);
//		//jetzt mal die listen füllen, mehr oder weniger
//		//textures_list
//		updateTerrainWindow();
//        //das decal erstellen
//        mFrameListener->createDecal();
//        
//	}
//}

//-----------------------------------------------------------------------------------------------------------
void EditorApp::createGUI()
{
//
//
//	// Set up GUI system
//	mGUIRenderer = new CEGUI::OgreCEGUIRenderer(mWindow, Ogre::RENDER_QUEUE_OVERLAY, false, 3000, mDefaultSceneMgr);
//	mGUISystem = new CEGUI::System(mGUIRenderer);
//	CEGUI::Logger::getSingleton().setLoggingLevel(CEGUI::Informative);
//
//	//load schemes
//	CEGUI::SchemeManager::getSingleton().loadScheme((CEGUI::utf8*)"TaharezLookSkin.scheme");
//	CEGUI::SchemeManager::getSingleton().loadScheme((CEGUI::utf8*)"WindowsLook.scheme");
//	//setup mouse
//	mGUISystem->setDefaultMouseCursor((CEGUI::utf8*)STYLE_EDITOR, (CEGUI::utf8*)"MouseArrow");
//	CEGUI::MouseCursor::getSingleton().setImage(STYLE_EDITOR, "MouseMoveCursor");
//	mGUISystem->setDefaultFont((CEGUI::utf8*)"BlueHighway-12");
//
//	/*mEditorGuiSheet= CEGUI::WindowManager::getSingleton().createWindow((CEGUI::utf8*)"DefaultWindow", (CEGUI::utf8*)"Sheet");
//	mGUISystem->setGUISheet(mEditorGuiSheet);*/
//
//	//get wmgr pointer
//	wmgr = CEGUI::WindowManager::getSingletonPtr();
//
//	//load XML files
//	mEditorGuiSheet = wmgr->loadWindowLayout((CEGUI::utf8*)"editor-main.layout");
//	CEGUI::Window *id = wmgr->loadWindowLayout((CEGUI::utf8*)"editor-inputdialog.layout");
//	CEGUI::Window *dw = wmgr->loadWindowLayout((CEGUI::utf8*)"editor-detailswnd.layout");
//	CEGUI::Window *esw = wmgr->loadWindowLayout((CEGUI::utf8*)"editor-settings.layout");
//	CEGUI::Window *lsw = wmgr->loadWindowLayout((CEGUI::utf8*)"editor-levelsettings.layout");
//
//
//	mGUISystem->setGUISheet(mEditorGuiSheet);
//
//	mEditorGuiSheet->addChildWindow(dw);	
//	mEditorGuiSheet->addChildWindow(id);
//	mEditorGuiSheet->addChildWindow(esw);
//	mEditorGuiSheet->addChildWindow(lsw);
//
//	//CEGUI::PushButton* quitButton = (CEGUI::PushButton*)CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Quit");
//
//
//	//get important windows
//	fpsBox = wmgr->getWindow("showFPS");	
//	objInfo = wmgr->getWindow("objectInfo");
//	levelsettings = (CEGUI::FrameWindow*)lsw;
//	inputdialog = (CEGUI::FrameWindow*)id;
//	detailswnd = (CEGUI::FrameWindow*)dw;
//	editorsettings = (CEGUI::FrameWindow*)esw;
//	selectionBorder = wmgr->getWindow("selectionborder");
//	selectionBorder->setMousePassThroughEnabled(true);
//	selectionBorder->hide();
//	selectionBorder->disable();
//	//END get important windows
//
//	//init debug wnd
//	CEGUI::Window *muhtest = CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"TextOut");
//	muhtest->setText((CEGUI::utf8*)"muhpra");
//	//END debug wnd
//
//	
//	//subscribe MAIN MENU
//	wmgr->getWindow((CEGUI::utf8*)"Quit")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&EditorApp::handleQuit, this));
//	wmgr->getWindow((CEGUI::utf8*)"Open")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&EditorApp::showOpenLevel, this));
////	wmgr->getWindow((CEGUI::utf8*)"SaveAsDotscene")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&EditorApp::saveGroup, this));
//	wmgr->getWindow((CEGUI::utf8*)"Save")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&EditorApp::saveBtnClick, this));
//	wmgr->getWindow((CEGUI::utf8*)"SaveAs")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&EditorApp::saveAsClick, this));
//
//	wmgr->getWindow((CEGUI::utf8*)"newlvl")->subscribeEvent(CEGUI::Window::EventMouseClick,CEGUI::Event::Subscriber(&EditorApp::newLevel,this));
//	wmgr->getWindow((CEGUI::utf8*)"editorsettings_start")->subscribeEvent(CEGUI::Window::EventMouseClick,CEGUI::Event::Subscriber(&EditorApp::showEditorSettings,this));
//	wmgr->getWindow((CEGUI::utf8*)"levelsettings_start")->subscribeEvent(CEGUI::Window::EventMouseClick,CEGUI::Event::Subscriber(&EditorApp::showLevelSettings,this));
//	
//
//	wmgr->getWindow((CEGUI::utf8*)"GridSnap")->subscribeEvent(CEGUI::Window::EventMouseClick,CEGUI::Event::Subscriber(&EditorApp::checkSnapGridClick,this));
//	wmgr->getWindow((CEGUI::utf8*)"AngleSnap")->subscribeEvent(CEGUI::Window::EventMouseClick,CEGUI::Event::Subscriber(&EditorApp::checkSnapAngleClick,this));
//	wmgr->getWindow((CEGUI::utf8*)"ScaleSnap")->subscribeEvent(CEGUI::Window::EventMouseClick,CEGUI::Event::Subscriber(&EditorApp::checkSnapScaleClick,this));
//	wmgr->getWindow((CEGUI::utf8*)"ProcessPhysics")->subscribeEvent(CEGUI::Window::EventMouseClick,CEGUI::Event::Subscriber(&EditorApp::checkPhysicsClick,this));
//	wmgr->getWindow((CEGUI::utf8*)"seteditterrain")->subscribeEvent(CEGUI::Window::EventMouseClick,CEGUI::Event::Subscriber(&EditorApp::checkEditTerrainClick,this));
//    wmgr->getWindow((CEGUI::utf8*)"seteditobjects")->subscribeEvent(CEGUI::Window::EventMouseClick,CEGUI::Event::Subscriber(&EditorApp::checkEditObjectsClick,this));
//	//die 3 checkboxen
//    
//    //das auswahldingens
//    wmgr->getWindow((CEGUI::utf8*)"editterrain/terrainDeform")->subscribeEvent(CEGUI::Window::EventMouseClick,CEGUI::Event::Subscriber(&EditorApp::terrainDeformClick,this));
//    wmgr->getWindow((CEGUI::utf8*)"editterrain/terrainPaint")->subscribeEvent(CEGUI::Window::EventMouseClick,CEGUI::Event::Subscriber(&EditorApp::paintTexturesClick,this));
//    wmgr->getWindow((CEGUI::utf8*)"editterrain/terrainFlatten")->subscribeEvent(CEGUI::Window::EventMouseClick,CEGUI::Event::Subscriber(&EditorApp::terrainFlattenClick,this));
//    wmgr->getWindow((CEGUI::utf8*)"editterrain/terrainSoften")->subscribeEvent(CEGUI::Window::EventMouseClick,CEGUI::Event::Subscriber(&EditorApp::terrainSoftenClick,this));
//	wmgr->getWindow((CEGUI::utf8*)"editterrain/terrainMakeholes")->subscribeEvent(CEGUI::Window::EventMouseClick,CEGUI::Event::Subscriber(&EditorApp::paintHolesClick,this));
//    //wmgr->getWindow((CEGUI::utf8*)"editterrain/painttextures")->subscribeEvent(CEGUI::Window::EventMouseClick,CEGUI::Event::Subscriber(&EditorApp::paintTexturesClick,this));
//    //wmgr->getWindow((CEGUI::utf8*)"editterrain/flatten")->subscribeEvent(CEGUI::Window::EventMouseClick,CEGUI::Event::Subscriber(&EditorApp::terrainFlattenClick,this));
//    //wmgr->getWindow((CEGUI::utf8*)"editterrain/flattenclick")->subscribeEvent(CEGUI::Window::EventMouseClick,CEGUI::Event::Subscriber(&EditorApp::terrainSoftenClick,this));
//    
//    wmgr->getWindow((CEGUI::utf8*)"editterrain/edittextures")->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&EditorApp::editTexturesShow,this));
//    wmgr->getWindow((CEGUI::utf8*)"editterrain/accept")->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&EditorApp::terrainsettingsApply,this));
//	wmgr->getWindow((CEGUI::utf8*)"ShowFog")->subscribeEvent(CEGUI::Window::EventMouseClick,CEGUI::Event::Subscriber(&EditorApp::checkFogClick,this));
//
//	wmgr->getWindow((CEGUI::utf8*)"modType")->subscribeEvent(CEGUI::Window::EventMouseClick,CEGUI::Event::Subscriber(&EditorApp::mModeChange,this));
//	wmgr->getWindow((CEGUI::utf8*)"modType")->setText("move");
//	//subscribing main menu END
//
//	//INIT input dialog..		
//	inputdialog->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked,CEGUI::Event::Subscriber(&EditorApp::defaultCloseBtn,this));
//	wmgr->getWindow((CEGUI::utf8*)"InputDialog/Cancel")->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&EditorApp::defaultCloseBtn,this));
//	wmgr->getWindow((CEGUI::utf8*)"InputDialog/OK")->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&EditorApp::inputDialogOK,this));
//
//	hideInputDialog();
//	//input dialog END
//
//	
//	
//
//	//INIT details wnd	
//	detailswnd->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked,CEGUI::Event::Subscriber(&EditorApp::defaultCloseBtn,this));
//	wmgr->getWindow((CEGUI::utf8*)"editObjWnd/cancelbtn")->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&EditorApp::defaultCloseBtn,this));
//	wmgr->getWindow((CEGUI::utf8*)"editObjWnd/okbtn")->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&EditorApp::detailsWndOk,this));
//	wmgr->getWindow((CEGUI::utf8*)"editObjWnd/setdir")->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&EditorApp::resetOrient,this));
//	wmgr->getWindow((CEGUI::uint8*)"editObjWnd/waterOptions/browsemat")->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&EditorApp::browseMaterialsBtn,this));
//	wmgr->getWindow((CEGUI::uint8*)"editObjWnd/staticOptions/browsemat")->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&EditorApp::browseMaterialsBtn,this));
//	wmgr->getWindow((CEGUI::uint8*)"editObjWnd/staticOptions/browseps")->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&EditorApp::browsePSBtn,this));
//	wmgr->getWindow((CEGUI::uint8*)"editObjWnd/contOptions/editInv")->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&EditorApp::editInv,this));
//	wmgr->getWindow((CEGUI::uint8*)"editObjWnd/charOptions/editInv")->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&EditorApp::editInv,this));
//	wmgr->getWindow((CEGUI::uint8*)"editObjWnd/charOptions/eqmelee")->subscribeEvent(CEGUI::Editbox::EventMouseButtonUp,CEGUI::Event::Subscriber(&EditorApp::charEqMouseReleased,this));
//	wmgr->getWindow((CEGUI::uint8*)"editObjWnd/charOptions/eqranged")->subscribeEvent(CEGUI::Editbox::EventMouseButtonUp,CEGUI::Event::Subscriber(&EditorApp::charEqMouseReleased,this));
//	wmgr->getWindow((CEGUI::uint8*)"editObjWnd/charOptions/eqshield")->subscribeEvent(CEGUI::Editbox::EventMouseButtonUp,CEGUI::Event::Subscriber(&EditorApp::charEqMouseReleased,this));
//	wmgr->getWindow((CEGUI::uint8*)"editObjWnd/charOptions/eqarmor")->subscribeEvent(CEGUI::Editbox::EventMouseButtonUp,CEGUI::Event::Subscriber(&EditorApp::charEqMouseReleased,this));
//	wmgr->getWindow((CEGUI::uint8*)"editObjWnd/charOptions/eqelse1")->subscribeEvent(CEGUI::Editbox::EventMouseButtonUp,CEGUI::Event::Subscriber(&EditorApp::charEqMouseReleased,this));
//	wmgr->getWindow((CEGUI::uint8*)"editObjWnd/charOptions/eqelse2")->subscribeEvent(CEGUI::Editbox::EventMouseButtonUp,CEGUI::Event::Subscriber(&EditorApp::charEqMouseReleased,this));
//	wmgr->getWindow((CEGUI::uint8*)"editObjWnd/charOptions/eqelse3")->subscribeEvent(CEGUI::Editbox::EventMouseButtonUp,CEGUI::Event::Subscriber(&EditorApp::charEqMouseReleased,this));
//	wmgr->getWindow((CEGUI::uint8*)"editObjWnd/charOptions/eqelse4")->subscribeEvent(CEGUI::Editbox::EventMouseButtonUp,CEGUI::Event::Subscriber(&EditorApp::charEqMouseReleased,this));
//	wmgr->getWindow((CEGUI::uint8*)"editObjWnd/charOptions/eqbag")->subscribeEvent(CEGUI::Editbox::EventMouseButtonUp,CEGUI::Event::Subscriber(&EditorApp::charEqMouseReleased,this));
//	
//	wmgr->getWindow((CEGUI::uint8*)"debugWnd")->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked, CEGUI::Event::Subscriber(&EditorApp::defaultCloseBtn, this));
//
//	CEGUI::Combobox *lightType =(CEGUI::Combobox*)wmgr->getWindow((CEGUI::utf8*)"editObjWnd/lightOptions/type");
//	CEGUI::ListboxTextItem *itemPoint = new CEGUI::ListboxTextItem("point");
//	itemPoint->setTextColours(CEGUI::colour(0,0,0));
//	itemPoint->setSelectionBrushImage(STYLE_EDITOR,"DefaultSelectionBrush");
//	CEGUI::ListboxTextItem *itemSpotlight = new CEGUI::ListboxTextItem("spotlight");
//	itemSpotlight->setTextColours(CEGUI::colour(0,0,0));
//	itemSpotlight->setID(1);
//	itemSpotlight->setSelectionBrushImage(STYLE_EDITOR,"DefaultSelectionBrush");
//	CEGUI::ListboxTextItem *itemDirectional = new CEGUI::ListboxTextItem("directional");
//	itemDirectional->setTextColours(CEGUI::colour(0,0,0));
//	itemDirectional->setID(0);
//	itemDirectional->setSelectionBrushImage(STYLE_EDITOR,"DefaultSelectionBrush");
//	lightType->addItem(itemPoint);
//	lightType->addItem(itemSpotlight);
//	lightType->addItem(itemDirectional);
//
//
//	hideDetailsDialog();
//	//END details wnd
//
//	//browse material
//	CEGUI::Window *br = wmgr->getWindow("browsematerialwnd");
//	br->disable();
//	br->setVisible(false);
//	br->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked, CEGUI::Event::Subscriber(&EditorApp::defaultCloseBtn, this));
//	wmgr->getWindow((CEGUI::utf8*)"browsematerialwnd/cancel")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&EditorApp::defaultCloseBtn, this));
//	wmgr->getWindow((CEGUI::utf8*)"browsematerialwnd/ok")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&EditorApp::browseMaterialsOK, this));
//
//
//	//browse particle system
//	CEGUI::Window *brps = wmgr->getWindow("browsepswnd");
//	brps->disable();
//	brps->setVisible(false);
//	brps->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked, CEGUI::Event::Subscriber(&EditorApp::defaultCloseBtn, this));
//	wmgr->getWindow((CEGUI::utf8*)"browsepswnd/cancel")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&EditorApp::defaultCloseBtn, this));
//	wmgr->getWindow((CEGUI::utf8*)"browsepswnd/ok")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&EditorApp::browsePSOK, this));
//
//
//	//edior settings wnd	
//	editorsettings->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked,CEGUI::Event::Subscriber(&EditorApp::defaultCloseBtn,this));
//	wmgr->getWindow((CEGUI::utf8*)"editorsettings/okbtn")->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&EditorApp::editorsettingsWndOk,this));
//	wmgr->getWindow((CEGUI::utf8*)"editorsettings/cancelbtn")->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&EditorApp::defaultCloseBtn,this));
//	hideEditorSettings();
//	//editor settings wnd end
//
//	//"edit inventory" wnd
//	CEGUI::Window *invwnd = wmgr->getWindow("EditInventory");
//	invwnd->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked,CEGUI::Event::Subscriber(&EditorApp::defaultCloseBtn,this));
//	wmgr->getWindow("EditInventory/ok")->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&EditorApp::inventoryWndOK,this));
//	wmgr->getWindow("EditInventory/cancel")->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&EditorApp::defaultCloseBtn,this));
//	CEGUI::Window *invlist = wmgr->getWindow("EditInventory/listbox");
//	invlist->subscribeEvent(CEGUI::Listbox::EventMouseButtonUp,CEGUI::Event::Subscriber(&EditorApp::inventoryWndMouseReleased,this));
//	invlist->subscribeEvent(CEGUI::Listbox::EventKeyUp,CEGUI::Event::Subscriber(&EditorApp::inventoryWndKeyReleased,this));
//	invwnd->disable();		
//	invwnd->setVisible(false);
//	//inventory wnd END
//
//	//"edit spellbook" wnd
//	CEGUI::Window *spwnd = wmgr->getWindow("EditSpellbook");
//	spwnd->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked,CEGUI::Event::Subscriber(&EditorApp::defaultCloseBtn,this));
//	wmgr->getWindow("EditSpellbook/ok")->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&EditorApp::spellbookWndOK,this));
//	wmgr->getWindow("EditSpellbook/cancel")->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&EditorApp::defaultCloseBtn,this));
//	CEGUI::Window *splist = wmgr->getWindow("EditSpellbook/listbox");
//	splist->subscribeEvent(CEGUI::Listbox::EventMouseButtonUp,CEGUI::Event::Subscriber(&EditorApp::spellbookWndMouseReleased,this));
//	splist->subscribeEvent(CEGUI::Listbox::EventKeyUp,CEGUI::Event::Subscriber(&EditorApp::spellbookWndKeyReleased,this));
//	spwnd->disable();		
//	spwnd->setVisible(false);
//	//inventory wnd END
//
//	//edit terrain wnd
//	CEGUI::Window *terrainwnd = wmgr->getWindow("editterrain");
//	terrainwnd->disable();
//	terrainwnd->setVisible(false);
//	//resolution select combobox
//	CEGUI::Combobox *terrainResSelect =(CEGUI::Combobox*)wmgr->getWindow((CEGUI::utf8*)"levelsettings/terrain/resSelect");
//	terrainResSelect->subscribeEvent(CEGUI::Combobox::EventTextSelectionChanged/*CEGUI::Combobox::EventMouseButtonUp*/,CEGUI::Event::Subscriber(&EditorApp::selectTerrainResolution,this));
//	for(int i=0;i<6;i++)
//	{
//		int curVal = Ogre::Math::IFloor(Ogre::Math::Pow(2,i+5));
//		CEGUI::String strVal = str(curVal).c_str();
//		CEGUI::ListboxTextItem *item = new CEGUI::ListboxTextItem(strVal);
//		item->setTextColours(CEGUI::colour(0,0,0));
//		item->setSelectionBrushImage(STYLE_EDITOR,"DefaultSelectionBrush");
//		terrainResSelect->addItem(item);
//	}
//	
//	/*fogNone->setID(0);
//	fogNone->setSelectionBrushImage(STYLE_EDITOR,"DefaultSelectionBrush");
//	fogNone->setTextColours(clBlack);
//	CEGUI::ListboxTextItem *fogLinear = new CEGUI::ListboxTextItem("linear");
//	fogLinear->setID(1);
//	fogLinear->setTextColours(clBlack);
//	fogLinear->setSelectionBrushImage(STYLE_EDITOR,"DefaultSelectionBrush");
//	CEGUI::ListboxTextItem *fogExp = new CEGUI::ListboxTextItem("exp");
//	fogExp->setID(2);
//	fogExp->setTextColours(clBlack);
//	fogExp->setSelectionBrushImage(STYLE_EDITOR,"DefaultSelectionBrush");
//	CEGUI::ListboxTextItem *fogExp2 = new CEGUI::ListboxTextItem("exp2");
//	fogExp2->setID(3);
//	fogExp2->setTextColours(clBlack);
//	fogExp2->setSelectionBrushImage(STYLE_EDITOR,"DefaultSelectionBrush");*/
//
//    CEGUI::Spinner *spScale = static_cast<CEGUI::Spinner*>(wmgr->getWindow((CEGUI::utf8*)"editterrain/brushscale_value"));
//    spScale->subscribeEvent(CEGUI::Spinner::EventValueChanged,CEGUI::Event::Subscriber(&EditorApp::brushSetScale,this));
//    CEGUI::Spinner *spIntens = static_cast<CEGUI::Spinner*>(wmgr->getWindow((CEGUI::utf8*)"editterrain/brushintensity"));
//    spIntens->subscribeEvent(CEGUI::Spinner::EventValueChanged,CEGUI::Event::Subscriber(&EditorApp::changeBrushIntensity,this));
//    //die scrollbars
//    CEGUI::Scrollbar *sbIntens = static_cast<CEGUI::Scrollbar *>(wmgr->getWindow((CEGUI::utf8*)"editterrain/brushintensity_sb"));
//    Ogre::Real pos = spIntens->getCurrentValue();
//    pos = pos/99;
//    setScrollbarPos(sbIntens,pos);
//    sbIntens->setStepSize(0.01);
//    sbIntens->setPageSize(0.1);
//    sbIntens->subscribeEvent(CEGUI::Scrollbar::EventScrollPositionChanged,CEGUI::Event::Subscriber(&EditorApp::changeBrushIntensity,this));
//    CEGUI::Scrollbar *sbScale = static_cast<CEGUI::Scrollbar *>(wmgr->getWindow((CEGUI::utf8*)"editterrain/brushscale_sb"));
//    sbScale->setStepSize(0.01);
//    sbScale->setPageSize(0.1);
//    pos = spScale->getCurrentValue();
//    pos = pos/49;
//    setScrollbarPos(sbScale,pos);
//    sbScale->subscribeEvent(CEGUI::Scrollbar::EventScrollPositionChanged,CEGUI::Event::Subscriber(&EditorApp::brushSetScale,this));
//	
//    
//
//	//terrain wind END
//
//	//lvl settings wnd	
//	levelsettings->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked,CEGUI::Event::Subscriber(&EditorApp::defaultCloseBtn,this));
//	wmgr->getWindow((CEGUI::utf8*)"levelsettings/okbtn")->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&EditorApp::lvlsettingsWndOk,this));
//	wmgr->getWindow((CEGUI::utf8*)"levelsettings/cancelbtn")->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&EditorApp::defaultCloseBtn,this));
//	wmgr->getWindow((CEGUI::uint8*)"levelsettings/skyboxmaterialbrowsebtn")->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&EditorApp::browseMaterialsBtn,this));
//	wmgr->getWindow((CEGUI::uint8*)"levelsettings/skydomematerialbrowsebtn")->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&EditorApp::browseMaterialsBtn,this));
//	wmgr->getWindow((CEGUI::uint8*)"levelsettings/skyplanematerialbrowsebtn")->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&EditorApp::browseMaterialsBtn,this));
//	wmgr->getWindow((CEGUI::uint8*)"levelsettings/skyplanematerialbrowsebtn")->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&EditorApp::browseMaterialsBtn,this));
//
//	hideLevelSettings();
//	//lvl settings wnd end
//
//	//------------------------------------------------------------
//	//populating the listboxes
//	//"special"-object
//	CEGUI::Listbox* specialList = (CEGUI::Listbox*)wmgr->getWindow((CEGUI::utf8*)LIST_SPECIAL);
//	specialList->subscribeEvent(CEGUI::Window::EventMouseButtonDown,CEGUI::Event::Subscriber(&EditorApp::specialListDown,this));
//    specialList->subscribeEvent(CEGUI::Window::EventMouseDoubleClick,CEGUI::Event::Subscriber(&EditorApp::objectListDoubleclick,this));
//
//	CEGUI::colour clBlack(0,0,0);
//	for(std::map<Ogre::String, gamedata_dynamic>::iterator itr =datamap_dynamic.begin();
//		itr != datamap_dynamic.end();itr++)
//	{
//
//		CEGUI::String obj_id = itr->first.c_str();
//		if(obj_id != "")
//		{
//			CEGUI::ListboxTextItem *listboxitem = new CEGUI::ListboxTextItem(obj_id);
//			listboxitem->setTextColours(clBlack);
//			listboxitem->setSelectionBrushImage(STYLE_EDITOR,"DefaultSelectionBrush");
//			specialList->addItem(listboxitem);
//		}
//	}
//    specialList->setSortingEnabled(true);
//    
//    specialList->subscribeEvent(CEGUI::Listbox::EventCharacterKey,CEGUI::Event::Subscriber(&EditorApp::listboxTypeFind,this));
//	//END special-object
//
//	//BEGIN items
//	CEGUI::Listbox* itemList = (CEGUI::Listbox*)wmgr->getWindow((CEGUI::utf8*)LIST_ITEMS);
//	itemList->subscribeEvent(CEGUI::Window::EventMouseButtonDown,CEGUI::Event::Subscriber(&EditorApp::itemListDown,this));
//	itemList->subscribeEvent(CEGUI::Window::EventMouseDoubleClick,CEGUI::Event::Subscriber(&EditorApp::objectListDoubleclick,this));
//    for(std::map<Ogre::String, gamedata_item>::iterator itr =datamap_item.begin();
//		itr != datamap_item.end();itr++)
//	{
//		CEGUI::String obj_id = itr->first.c_str();
//		if(obj_id != "" && !(itr->second.propertyFlags & PF_SECONDARY))
//		{
//			CEGUI::ListboxTextItem *listboxitem = new CEGUI::ListboxTextItem(obj_id);
//			listboxitem->setSelectionBrushImage(STYLE_EDITOR,"DefaultSelectionBrush");
//			listboxitem->setTextColours(clBlack);
//			itemList->addItem(listboxitem);
//		}
//	}
//    itemList->setSortingEnabled(true);
//    itemList->subscribeEvent(CEGUI::Listbox::EventCharacterKey,CEGUI::Event::Subscriber(&EditorApp::listboxTypeFind,this));
//	//END items
//
//	//characters
//	CEGUI::Listbox* charList = (CEGUI::Listbox*)wmgr->getWindow((CEGUI::utf8*)LIST_CHARACTERS);
//	charList->subscribeEvent(CEGUI::Window::EventMouseButtonDown,CEGUI::Event::Subscriber(&EditorApp::charListDown,this));
//	charList->subscribeEvent(CEGUI::Window::EventMouseDoubleClick,CEGUI::Event::Subscriber(&EditorApp::objectListDoubleclick,this));
//    for(std::map<Ogre::String, gamedata_char>::iterator itr =datamap_char.begin();
//		itr != datamap_char.end();itr++)
//	{
//
//		CEGUI::String obj_id = itr->first.c_str();
//		if(obj_id != "")
//		{
//			CEGUI::ListboxTextItem *listboxitem = new CEGUI::ListboxTextItem(obj_id);
//			listboxitem->setSelectionBrushImage(STYLE_EDITOR,"DefaultSelectionBrush");
//			listboxitem->setTextColours(clBlack);
//			charList->addItem(listboxitem);
//		}
//	}
//    charList->setSortingEnabled(true);
//    charList->subscribeEvent(CEGUI::Listbox::EventCharacterKey,CEGUI::Event::Subscriber(&EditorApp::listboxTypeFind,this));
//	//END characters
//
//	//doors
//	CEGUI::Listbox* actList = (CEGUI::Listbox*)wmgr->getWindow((CEGUI::utf8*)LIST_DOORS);
//	actList->subscribeEvent(CEGUI::Window::EventMouseButtonDown,CEGUI::Event::Subscriber(&EditorApp::actListDown,this));
//	actList->subscribeEvent(CEGUI::Window::EventMouseDoubleClick,CEGUI::Event::Subscriber(&EditorApp::objectListDoubleclick,this));
//    for(std::map<Ogre::String, gamedata_door>::iterator itr =datamap_door.begin();
//		itr != datamap_door.end();itr++)
//	{
//
//		CEGUI::String obj_id = itr->first.c_str();
//		if(obj_id != "")
//		{
//			CEGUI::ListboxTextItem *listboxitem = new CEGUI::ListboxTextItem(obj_id);
//			listboxitem->setSelectionBrushImage(STYLE_EDITOR,"DefaultSelectionBrush");
//			listboxitem->setTextColours(clBlack);
//			actList->addItem(listboxitem);
//		}
//	}
//    actList->setSortingEnabled(true);
//    actList->subscribeEvent(CEGUI::Listbox::EventCharacterKey,CEGUI::Event::Subscriber(&EditorApp::listboxTypeFind,this));
//	//END doors
//
//	//containers
//	CEGUI::Listbox* cntList = (CEGUI::Listbox*)wmgr->getWindow((CEGUI::utf8*)LIST_CONTAINERS);
//	cntList->subscribeEvent(CEGUI::Window::EventMouseButtonDown,CEGUI::Event::Subscriber(&EditorApp::containerListDown,this));
//	cntList->subscribeEvent(CEGUI::Window::EventMouseDoubleClick,CEGUI::Event::Subscriber(&EditorApp::objectListDoubleclick,this));
//    for(std::map<Ogre::String, gamedata_container>::iterator itr = datamap_container.begin();
//		itr != datamap_container.end();itr++)
//	{
//
//		CEGUI::String obj_id = itr->first.c_str();
//		if(obj_id != "")
//		{
//			CEGUI::ListboxTextItem *listboxitem = new CEGUI::ListboxTextItem(obj_id);
//			listboxitem->setSelectionBrushImage(STYLE_EDITOR,"DefaultSelectionBrush");
//			listboxitem->setTextColours(clBlack);
//			cntList->addItem(listboxitem);
//		}
//	}
//    cntList->setSortingEnabled(true);
//    cntList->subscribeEvent(CEGUI::Listbox::EventCharacterKey,CEGUI::Event::Subscriber(&EditorApp::listboxTypeFind,this));
//	//END containers
//
//	//else/stuff
//	CEGUI::Listbox* elseList = (CEGUI::Listbox*)wmgr->getWindow((CEGUI::utf8*)LIST_ELSE);
//	elseList->subscribeEvent(CEGUI::Window::EventMouseButtonDown,CEGUI::Event::Subscriber(&EditorApp::elseListDown,this));
//	elseList->subscribeEvent(CEGUI::Window::EventMouseDoubleClick,CEGUI::Event::Subscriber(&EditorApp::objectListDoubleclick,this));
//	CEGUI::ListboxTextItem *listboxitem = new CEGUI::ListboxTextItem("Entrance");
//	listboxitem->setSelectionBrushImage(STYLE_EDITOR,"DefaultSelectionBrush");
//	listboxitem->setTextColours(clBlack);
//	listboxitem->setID(0);
//	elseList->addItem(listboxitem);
//
//	listboxitem = new CEGUI::ListboxTextItem("Ogre::Light");
//	listboxitem->setSelectionBrushImage(STYLE_EDITOR,"DefaultSelectionBrush");
//	listboxitem->setTextColours(clBlack);
//	listboxitem->setID(1);
//	elseList->addItem(listboxitem);
//
//	listboxitem = new CEGUI::ListboxTextItem("Ogre::Plane");
//	listboxitem->setSelectionBrushImage(STYLE_EDITOR,"DefaultSelectionBrush");
//	listboxitem->setTextColours(clBlack);
//	listboxitem->setID(2);
//	elseList->addItem(listboxitem);
//
//	listboxitem = new CEGUI::ListboxTextItem("Static Billboard");
//	listboxitem->setSelectionBrushImage(STYLE_EDITOR,"DefaultSelectionBrush");
//	listboxitem->setTextColours(clBlack);
//	listboxitem->setID(3);
//	elseList->addItem(listboxitem);
//
//	listboxitem = new CEGUI::ListboxTextItem("Static Particle System");
//	listboxitem->setSelectionBrushImage(STYLE_EDITOR,"DefaultSelectionBrush");
//	listboxitem->setTextColours(clBlack);
//	listboxitem->setID(4);
//	elseList->addItem(listboxitem);
//
//    listboxitem = new CEGUI::ListboxTextItem("Waterpane");
//	listboxitem->setSelectionBrushImage(STYLE_EDITOR,"DefaultSelectionBrush");
//	listboxitem->setTextColours(clBlack);
//	listboxitem->setID(5);
//	elseList->addItem(listboxitem);
//    elseList->subscribeEvent(CEGUI::Listbox::EventCharacterKey,CEGUI::Event::Subscriber(&EditorApp::listboxTypeFind,this));
//	//END else/stuff
//
//	//"Open Level"-Window
//	CEGUI::Listbox* lvlList = (CEGUI::Listbox*)wmgr->getWindow("levelview/levels_list");
//    lvlList->setSortingEnabled(true);
//    lvlList->subscribeEvent(CEGUI::Listbox::EventCharacterKey,CEGUI::Event::Subscriber(&EditorApp::listboxTypeFind,this));
//	lvlList->subscribeEvent(CEGUI::Window::EventMouseDoubleClick,CEGUI::Event::Subscriber(&EditorApp::lvlListDbl,this));
//	wmgr->getWindow((CEGUI::utf8*)"levelview/ok")->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&EditorApp::lvlListDbl,this));
//	wmgr->getWindow((CEGUI::utf8*)"levelview/cancel")->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&EditorApp::defaultCloseBtn,this));
//	CEGUI::Window *lvlwnd = wmgr->getWindow((CEGUI::utf8*)"levelview");
//	lvlwnd->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked,CEGUI::Event::Subscriber(&EditorApp::defaultCloseBtn,this));
//	updateLevelList();
//	lvlwnd->disable();
//	lvlwnd->setVisible(false);
//	//level-window END
//
//	wmgr->getWindow((CEGUI::utf8*)"curobjects_list")->subscribeEvent(CEGUI::Window::EventMouseDoubleClick,CEGUI::Event::Subscriber(&EditorApp::curObjDblClick,this));
//	
//	
//	
//	
//
//	//filling list of materials
//#ifdef _DEBUG
//	int debug_limiter = 0;
//#endif
//	CEGUI::Listbox* matList = (CEGUI::Listbox*)wmgr->getWindow((CEGUI::utf8*)"browsematerialwnd/list");
//	Ogre::ResourceManager::ResourceMapIterator itr = Ogre::MaterialManager::getSingletonPtr()->getResourceIterator();
//	while(itr.hasMoreElements())
//	{
//		Ogre::String name = itr.peekNextValue()->getName();
//		CEGUI::ListboxTextItem *listboxitem = new CEGUI::ListboxTextItem(name.c_str());
//		listboxitem->setSelectionBrushImage(STYLE_EDITOR,"DefaultSelectionBrush");
//		listboxitem->setTextColours(clBlack);
//		matList->addItem(listboxitem);
//		itr.moveNext();
//#ifdef _DEBUG
//		debug_limiter++;
//		if(debug_limiter > 20)
//			break;
//#endif
//        
//
//	}
//    matList->setSortingEnabled(true);
//    matList->subscribeEvent(CEGUI::Listbox::EventCharacterKey,CEGUI::Event::Subscriber(&EditorApp::listboxTypeFind,this));
//	//list of materials END
//
//	//list of Particle Systems
//	
////#ifdef _DEBUG
////	int debug_limiter = 0;
////#endif
//	CEGUI::Listbox* psList = (CEGUI::Listbox*)wmgr->getWindow((CEGUI::utf8*)"browsepswnd/list");
//    Ogre::ParticleSystemManager::ParticleSystemTemplateIterator psitr = Ogre::ParticleSystemManager::getSingletonPtr()->getTemplateIterator();
//	while(psitr.hasMoreElements())
//	{
//		
//		Ogre::String name = psitr.peekNextValue()->getName();
//		CEGUI::ListboxTextItem *listboxitem = new CEGUI::ListboxTextItem(name.c_str());
//		listboxitem->setSelectionBrushImage(STYLE_EDITOR,"DefaultSelectionBrush");
//		listboxitem->setTextColours(clBlack);
//		psList->addItem(listboxitem);
//		psitr.moveNext();
////#ifdef _DEBUG
////		debug_limiter++;
////		if(debug_limiter > 20)
////			break;
////#endif
//
//	}
//    psList->setSortingEnabled(true);
//    psList->subscribeEvent(CEGUI::Listbox::EventCharacterKey,CEGUI::Event::Subscriber(&EditorApp::listboxTypeFind,this));
//	//particle systems END
//	
//	//loading list of meshes
//	Ogre::StringVectorPtr meshvector = Ogre::ResourceGroupManager::getSingletonPtr()->findResourceNames(Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,"*.mesh");
//	CEGUI::Listbox* meshList = (CEGUI::Listbox*)wmgr->getWindow((CEGUI::utf8*)LIST_STATIC_MESHES);
//	meshList->subscribeEvent(CEGUI::Window::EventMouseButtonDown,CEGUI::Event::Subscriber(&EditorApp::meshListDown,this));
//    meshList->subscribeEvent(CEGUI::Window::EventMouseDoubleClick,CEGUI::Event::Subscriber(&EditorApp::objectListDoubleclick,this));
//	for (Ogre::StringVector::iterator i = meshvector->begin(); i != meshvector->end(); ++i)
//	{
//		Ogre::String wtf = *i;
//		CEGUI::ListboxTextItem *listboxitem = new CEGUI::ListboxTextItem(wtf.c_str());
//		listboxitem->setSelectionBrushImage(STYLE_EDITOR,"DefaultSelectionBrush");
//		listboxitem->setTextColours(clBlack);
//		meshList->addItem(listboxitem);
//	}
//    meshList->setSortingEnabled(true);
//    meshList->subscribeEvent(CEGUI::Listbox::EventCharacterKey,CEGUI::Event::Subscriber(&EditorApp::listboxTypeFind,this));
//	//END list of meshes
//
//	//loading list of dotscenes
//	Ogre::StringVectorPtr dsvector = Ogre::ResourceGroupManager::getSingletonPtr()->findResourceNames(Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,"*.scene");
//	CEGUI::Listbox* dsList = (CEGUI::Listbox*)wmgr->getWindow((CEGUI::utf8*)LIST_STATIC_DOTSCENES);
//	dsList->subscribeEvent(CEGUI::Window::EventMouseButtonDown,CEGUI::Event::Subscriber(&EditorApp::sceneListDown,this));
//	dsList->subscribeEvent(CEGUI::Window::EventMouseDoubleClick,CEGUI::Event::Subscriber(&EditorApp::objectListDoubleclick,this));
//    for (Ogre::StringVector::iterator i = dsvector->begin(); i != dsvector->end(); ++i)
//	{
//		Ogre::String wtf = *i;
//		CEGUI::ListboxTextItem *listboxitem = new CEGUI::ListboxTextItem(wtf.c_str());
//		listboxitem->setSelectionBrushImage(STYLE_EDITOR,"DefaultSelectionBrush");
//		listboxitem->setTextColours(clBlack);
//		dsList->addItem(listboxitem);
//	}
//    dsList->setSortingEnabled(true);
//    dsList->subscribeEvent(CEGUI::Listbox::EventCharacterKey,CEGUI::Event::Subscriber(&EditorApp::listboxTypeFind,this));
//	//END list of dotscenes
//
//	//dropdown-menüs in den levelsettings füllen
//	CEGUI::Combobox *fogType =(CEGUI::Combobox*)wmgr->getWindow((CEGUI::utf8*)"levelsettings/fog_type");
//	CEGUI::ListboxTextItem *fogNone = new CEGUI::ListboxTextItem("none");
//	fogNone->setID(0);
//	fogNone->setSelectionBrushImage(STYLE_EDITOR,"DefaultSelectionBrush");
//	fogNone->setTextColours(clBlack);
//	CEGUI::ListboxTextItem *fogLinear = new CEGUI::ListboxTextItem("linear");
//	fogLinear->setID(1);
//	fogLinear->setTextColours(clBlack);
//	fogLinear->setSelectionBrushImage(STYLE_EDITOR,"DefaultSelectionBrush");
//	CEGUI::ListboxTextItem *fogExp = new CEGUI::ListboxTextItem("exp");
//	fogExp->setID(2);
//	fogExp->setTextColours(clBlack);
//	fogExp->setSelectionBrushImage(STYLE_EDITOR,"DefaultSelectionBrush");
//	CEGUI::ListboxTextItem *fogExp2 = new CEGUI::ListboxTextItem("exp2");
//	fogExp2->setID(3);
//	fogExp2->setTextColours(clBlack);
//	fogExp2->setSelectionBrushImage(STYLE_EDITOR,"DefaultSelectionBrush");
//	
//	fogType->addItem(fogNone);
//	fogType->addItem(fogLinear);
//	fogType->addItem(fogExp);
//	fogType->addItem(fogExp2);
//	fogNone->setSelected(true);
//
//    //texturen-bearbeitungsfenster vorbereiten und verstecken
//    Ogre::FileInfoListPtr textures = Ogre::ResourceGroupManager::getSingleton().listResourceFileInfo("ETTextures");
//	CEGUI::Listbox* texList = (CEGUI::Listbox*)wmgr->getWindow((CEGUI::utf8*)"editterraintextures/alltexturelist");
//	for(Ogre::FileInfoList::iterator it = textures->begin();it != textures->end();it++)
//	{
//
//		CEGUI::String obj_id = it->filename;
//		if(obj_id != "" && obj_id != "hole.png")
//		{
//			CEGUI::ListboxTextItem *listboxitem = new CEGUI::ListboxTextItem(obj_id);
//			//listboxitem->setSelectionColours(CEGUI::colour(11000000));
//			listboxitem->setSelectionBrushImage(STYLE_EDITOR,"DefaultSelectionBrush");
//			listboxitem->setTextColours(CEGUI::colour(0,0,0));
//			listboxitem->setAutoDeleted(true);
//			texList->addItem(listboxitem);
//		}
//	}
//    texList->setSortingEnabled(true);
//    texList->subscribeEvent(CEGUI::Listbox::EventCharacterKey,CEGUI::Event::Subscriber(&EditorApp::listboxTypeFind,this));
//
//    CEGUI::Window *editTexWnd = wmgr->getWindow("editterraintextures");
//    editTexWnd->setVisible(false);
//    editTexWnd->disable();
//
//    editTexWnd->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked,CEGUI::Event::Subscriber(&EditorApp::defaultCloseBtn,this));
//	wmgr->getWindow((CEGUI::utf8*)"editterraintextures/cancel")->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&EditorApp::defaultCloseBtn,this));
//
//    wmgr->getWindow((CEGUI::utf8*)"editterraintextures/ok")->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&EditorApp::editTexturesOkClick,this));
//    wmgr->getWindow((CEGUI::utf8*)"editterraintextures/up")->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&EditorApp::editTexturesUpClick,this));
//    wmgr->getWindow((CEGUI::utf8*)"editterraintextures/down")->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&EditorApp::editTexturesDownClick,this));
//    wmgr->getWindow((CEGUI::utf8*)"editterraintextures/add")->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&EditorApp::editTexturesAddClick,this));
//    wmgr->getWindow((CEGUI::utf8*)"editterraintextures/remove")->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&EditorApp::editTexturesRemoveClick,this));
//	
//    wmgr->getWindow((CEGUI::utf8*)"editterraintextures/alltexturelist")->subscribeEvent(CEGUI::Listbox::EventSelectionChanged,CEGUI::Event::Subscriber(&EditorApp::textureListboxClick,this));
//    wmgr->getWindow((CEGUI::utf8*)"editterraintextures/leveltexturelist")->subscribeEvent(CEGUI::Listbox::EventSelectionChanged,CEGUI::Event::Subscriber(&EditorApp::textureListboxClick,this));
    
}


void EditorApp::updateLevelList()
{
	//Ogre::FileInfoListPtr levelfiles = Ogre::ResourceGroupManager::getSingleton().listResourceFileInfo("Level");

	//CEGUI::WindowManager *wmgr = CEGUI::WindowManager::getSingletonPtr();

	//CEGUI::Listbox* lvlList = (CEGUI::Listbox*)wmgr->getWindow((CEGUI::utf8*)"levelview/levels_list");
	////delete all items
	//setDebugText("\ncnt="+str(lvlList->getItemCount()),true);
	//size_t cnt = lvlList->getItemCount();
	//
	//for(size_t i = 0;i < cnt;i++)
	//{
	//	CEGUI::ListboxItem *listboxitem = lvlList->getListboxItemFromIndex(0);
	//	//setDebugText(Ogre::String("\nremoved:")+listboxitem->getText().c_str(),true);
	//	lvlList->removeItem(listboxitem);
	//}

	//
	//
	//for(Ogre::FileInfoList::iterator it = levelfiles->begin();it != levelfiles->end();it++)
	//{

	//	CEGUI::String obj_id = it->filename;
	//	if(obj_id != "" && (Ogre::StringUtil::endsWith(obj_id.c_str(),LEVELEXT) || Ogre::StringUtil::endsWith(obj_id.c_str(),ZLEVELEXT) ))
	//	{
	//		CEGUI::ListboxTextItem *listboxitem = new CEGUI::ListboxTextItem(obj_id);
	//		//listboxitem->setSelectionColours(CEGUI::colour(11000000));
	//		listboxitem->setSelectionBrushImage(STYLE_EDITOR,"DefaultSelectionBrush");
	//		listboxitem->setTextColours(CEGUI::colour(0,0,0));
	//		listboxitem->setAutoDeleted(true);
	//		lvlList->addItem(listboxitem);
	//	}
	//}
}


void EditorApp::initSound()
{
	//SOUND 
	soundMgr = new SimpleSound::SoundManager();
	soundMgr->setDefaultMaxDist(settings.max_distance);
	soundMgr->setDefaultRefDistance(settings.ref_distance);
	soundMgr->setDefaultRolloffFactor(settings.rolloff_factor);

	soundMgr->setDistanceModel(settings.distance_model);
	soundMgr->getListener()->setGain(settings.gain_all);

	
}


//bool EditorApp::rootWindowDrop(const CEGUI::EventArgs &e)
//{
//
//
//	if(dragItemType != 0)
//	{
//		setDebugText("drop="+dragItemID);
//		CEGUI::Point mousepos =	CEGUI::MouseCursor::getSingleton().getPosition();
//		Ogre::Camera *mCamera = currentLevel->getMainCam();//mSceneMgr->getCamera("PlayerCam");
//		Ogre::Real mouseX = mousepos.d_x/mWindow->getWidth();
//		Ogre::Real mouseY = mousepos.d_y/mWindow->getHeight();
//		Ogre::Ray mouseRay = mCamera->getCameraToViewportRay( mouseX, mouseY );
//		
//		Ogre::Vector3 pos = mouseRay.getPoint(25); 
//		
//		currentLevel->placeObject(dragItemType,dragItemID,pos,Ogre::Quaternion::IDENTITY);
//		dragItemID="";
//		dragItemType=0;
//	}
////	okToProcessEvent = true;
//	return true;
//	
//}


//bool EditorApp::handleQuit(const CEGUI::EventArgs& e)
//{
//	//qtApp->getGameLogic()->requestShutdown();
//	//hm...
//	//requestShutdown();
//	qtApp->shutdown();
//	return true;
//}
//
//bool EditorApp::saveBtnClick(const CEGUI::EventArgs& e)
//{
//	saveCurrentLvl();
//	return true;
//}
//
//bool EditorApp::saveAsClick(const CEGUI::EventArgs& e)
//{
//	saveCurrentLvlAs();
//	return true;
//}
//
//void EditorApp::saveAsDialogConfirm(Ogre::String name)
//{
//	//hier speichert er erst
//	//Ogre::String filename = inputdialog->getChild("InputDialog/Edit")->getText().c_str();
//	currentLevel->mFileName = name;
//	saveCurrentLvl();
//	hideInputDialog();
////	return true;
//}
//--------------------------------------------------------------------------------------------------
//bool EditorApp::inputDialogOK(const CEGUI::EventArgs& e)
//{
//	if(inputDialogReturn)
//	{
//		Ogre::String filename = inputdialog->getChild("InputDialog/Edit")->getText().c_str();
//		(this->*inputDialogReturn)(filename);
//		inputDialogReturn = NULL;
//	}
//	return true;
//}
//--------------------------------------------------------------------------------------------------


void EditorApp::log(Ogre::String txt)//kleine Hilfsfunktion
{
	Ogre::LogManager::getSingleton().logMessage("PRA: "+txt);
}



//void EditorApp::setClipboardText(Ogre::String txt)
//{
//	clipboard = txt;
//}
//Ogre::String EditorApp::getClipboardText()
//{
//	return clipboard;
//}


void EditorApp::setDebugText(Ogre::String str,bool add)
{
	qtApp->mainWnd->setStatusText(str);
	//qtApp->logMessage(str);
	//CEGUI::Window *muhtest = CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"TextOut");
	//CEGUI::String test = "";
	//if(add)
	//	test += muhtest->getText();
	//test += str.c_str();


	//muhtest->setText(test);
}

//diese funktionen reagieren, wenn NICHT in die GUI geklickt wurden.
//sie reagieren, wenn der FL es injectet




//void EditorApp::placeObject(int objectType,Ogre::String objectID,Ogre::Vector3 pos,Ogre::Quaternion ornt,
//							  Ogre::Vector3 LVLscale)//,Ogre::String plane_material,Ogre::Real plane_xseg, Ogre::Real plane_yseg)
//{
//
//	log("ID="+objectID);
//
//	Ogre::Vector3 scale = Ogre::Vector3::UNIT_SCALE;
//	Ogre::String mesh = ERROR_MESH;
//    Ogre::String material = ERROR_MATERIAL;
//
//
//	obj_container currentContainer;
//
//	Ogre::Real mass = 0; //0 if static
//	short collisionType = 0;
//	Ogre::Vector3 collisionParams = Ogre::Vector3::UNIT_SCALE;
//	int flags = 0;
//
//
//	Ogre::Entity *ent;
//	OgreNewt::Collision* col = 0;// = new OgreNewt::CollisionPrimitives::TreeCollision(mWorld,nod,false);
//	bool goOn = true;
//
//
//
//	switch(objectType)
//	{
//	case OT_STATIC:
//		//if(objectID != "plane")
//		{
//			game_obj_static thisItem = objects[objectID];
//			scale = thisItem.scale;
//			mesh = thisItem.mesh;
//			material = thisItem.material;
//			mass = thisItem.mass;
//			collisionType = thisItem.collisionType;
//			collisionParams = thisItem.collisionParams;
//			flags = thisItem.flags;
//			currentContainer.type= OT_STATIC;
//			log("static");
//		}
//		break;
//	case OT_ITEM:
//		//if(1)
//		{
//			game_item thisItem = datamap_item[objectID];
//			scale = thisItem.scale;
//			mesh = thisItem.mesh;
//			material = thisItem.material;
//			mass = thisItem.mass;
//			collisionType = thisItem.collisionType;
//			collisionParams = thisItem.collisionParams;
//			flags = thisItem.flags;
//			currentContainer.type = OT_ITEM;
//			log("item");
//		}
//		break;
//
//	default:
//		goOn = false;
//	}
//
//	if(!goOn)
//	{
//		return;
//	}
//	Ogre::String nodeName = "node_"+objectID;
//	for(int i = 0;mSceneMgr->hasSceneNode(nodeName);++i)
//	{
//		nodeName = "node_"+objectID+Ogre::StringConverter::toString(i);
//	}
//	Ogre::SceneNode *nod = mSceneMgr->getRootSceneNode()->createChildSceneNode(nodeName);//,pos);
//
//
//	Ogre::String entName = "ent_"+objectID;//+"_"+Ogre::StringConverter::toString(i);
//	for(int i = 0;mSceneMgr->hasEntity(entName);++i)
//	{
//		entName = "ent_"+objectID+Ogre::StringConverter::toString(i);
//	}
//
//
//	
//	if(Ogre::StringUtil::startsWith(mesh,"!plane"))
//	{
//		//ok, ein fall für den meshmanager
//		if(Ogre::MeshManager::getSingleton().resourceExists(mesh))
//		{
//			//dann ist alles ok
//		}
//		else
//		{
//			Ogre::Plane pl(Ogre::Vector3(0,0,-1),0);
//			Ogre::String xseg_s = "";
//			Ogre::String yseg_s = "";
//			Ogre::String temp_string= "";
//			for(unsigned int i=7;i<mesh.length();i++)
//			{
//				if(mesh[i] != '/')
//				{
//					temp_string += mesh[i];
//				}
//				else
//				{
//					xseg_s = temp_string;
//					temp_string="";
//				}
//
//			}
//			yseg_s = temp_string;
//			log(xseg_s+" - "+yseg_s);
//			Ogre::MeshManager::getSingleton().createPlane(mesh,
//			   Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, pl,
//			   1,1,Ogre::StringConverter::parseInt(xseg_s),Ogre::StringConverter::parseInt(yseg_s),
//			   true,1,5,5,Ogre::Vector3(1,0,0));
//		}
//
//			
//	}
//	
//
//	//if(!plane)
//	//{
//		ent = mSceneMgr->createEntity( entName, mesh.c_str() );
//		if(material != "")
//		{
//			ent->setMaterialName(material);
//			
//		}
//	//}
//	//else
//	//{
//
//	//	Ogre::Real width = LVLscale.x;
//	//	Ogre::Real height = LVLscale.z;
//	//	Ogre::Plane pl(Ogre::Vector3(0,0,-1),0); //let's try this...
//
//	//	Ogre::String plName = "plane_";//+"_"+Ogre::StringConverter::toString(i);
//
//	//	for(int i = 0;Ogre::MeshManager::getSingleton().resourceExists(plName);++i)
//	//	{
//	//		plName = "ent_"+Ogre::StringConverter::toString(i);
//	//	}
//
//
//	//	/*Ogre::MeshManager::getSingleton().createPlane(plName,
//	//	   Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, pl,
//	//	   width,height,plane_xseg,plane_yseg,true,1,5,5,Ogre::Vector3(1,0,0));*/
//	//	Ogre::MeshManager::getSingleton().createPlane(plName,
//	//	   Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, pl,
//	//	   1,1,plane_xseg,plane_yseg,true,1,5,5,Ogre::Vector3(1,0,0));
//	//	//Ogre::String EntName = "ent_plane_"+Ogre::StringConverter::toString(i);
//
//	//	ent = mSceneMgr->createEntity( entName, plName );
//	//	log("plane entity: mat="+plane_material+", width="+Ogre::StringConverter::toString(width)+", height="+
//	//		Ogre::StringConverter::toString(height)+", entName="+entName);
//	//	ent->setMaterialName(plane_material);
//
//	//}
//
//	nod->attachObject(ent);
//	//if(!plane)
//	//{
//		nod->setScale(scale);
//	//}
//
//	//if(flags & OF_INVISIBLE)
//	//{
//	//	//only ingame invisible
//	//	ent->setMaterialName("nothing");
//	//}
//
//	OgreNewt::Body *bod = makeBody(mWorld,collisionType,mass,collisionParams,nod);
//
//	log("pos="+Ogre::StringConverter::toString(pos)+", ornt="+Ogre::StringConverter::toString(ornt));
//
//	bod->setPositionOrientation(pos,ornt);
//	log("nach bod->setPositionOrientation(pos,ornt);");
//
//	currentContainer.NewtonBody = bod;
//	currentContainer.ObjId = objectID;
//
//	ObjectMap[nodeName] = currentContainer;
//	log("am ende der func");
//
//
//}
//
//void EditorApp::removeObject(Ogre::SceneNode *sNode)
//{
//	Ogre::String nodeName = sNode->getName();
//	if(ObjectMap.count(nodeName))
//	{
//		obj_container temp = ObjectMap[nodeName];
//		setDebugText(nodeName+"\n"+temp.ObjId);
//		log(nodeName+"\n"+temp.ObjId);
//		if(temp.NewtonBody)
//			delete temp.NewtonBody;
//
//	//	
//	//	
//		sNode->removeAndDestroyAllChildren();
//		while(sNode->numAttachedObjects())
//		{
//			log("schleife anfang");
//			MovableObject *m = sNode->detachObject((unsigned short)0);
//			log("nach detach");
//			mSceneMgr->destroyMovableObject(m);
//			setDebugText("\n detach&destroy: done",true);
//			log("\n detach&destroy: done");
//		}
//		setDebugText("\n noch da: "+Ogre::StringConverter::toString(sNode->numAttachedObjects()));
//		log("\n noch da: "+Ogre::StringConverter::toString(sNode->numAttachedObjects()));
//		mSceneMgr->destroySceneNode(nodeName);
//		ObjectMap.erase(nodeName);
//	//	setDebugText("");
//	//	
//	//	while(itr.hasMoreElements())
//	//	{
//	//		MovableObject *mo = itr.getNext();
//	//		setDebugText(mo->getName()+"\n",true);
//	//		//mSceneMgr->destroyMovableObject(mo->getName(),mo->getMovableType());
//	//		mSceneMgr->destroyMovableObject(mo);
//	//		itr = sNode->getAttachedObjectIterator();
//	//	}
//
//	//	sNode->showBoundingBox(false); 
//	//	sNode->detachAllObjects();
//	//	mSceneMgr->destroySceneNode(nodeName);
//
//	//	//ObjectMap.erase(nodeName);
//
//	//	setDebugText("cnt="+Ogre::StringConverter::toString(ObjectMap.count(nodeName)));
//
//
//	}
//	
//}
//
//
//int EditorApp::getObjTypeBySceneNode(Ogre::SceneNode *sNode)
//{
//	Ogre::String nodeName = sNode->getName();
//	if(ObjectMap.count(nodeName))
//	{
//		obj_container temp = ObjectMap[nodeName];
//		return temp.type;
//	}
//	else
//	{
//		return 0;
//	}
//
//}
//
//Ogre::String EditorApp::getObjIdBySceneNode(Ogre::SceneNode *sNode)
//{
//	Ogre::String nodeName = sNode->getName();
//	if(ObjectMap.count(nodeName))
//	{
//		obj_container temp = ObjectMap[nodeName];
//		return temp.ObjId;
//	}
//	else
//	{
//		return "";
//	}
//}
//
//
//OgreNewt::Body *EditorApp::getBodyBySceneNode(Ogre::SceneNode *sNode)
//{
//	Ogre::String nodeName = sNode->getName();
//	if(ObjectMap.count(nodeName))
//	{
//		obj_container temp = ObjectMap[nodeName];
//		return temp.NewtonBody;
//	}
//	else
//	{
//		return 0;
//	}
//	
//}
//
//
//int EditorApp::getObjTypeBySceneNode(Ogre::String nodeName)
//{
//	if(ObjectMap.count(nodeName))
//	{
//		obj_container temp = ObjectMap[nodeName];
//		return temp.type;
//	}
//	else
//	{
//		return 0;
//	}
//
//}
//
//Ogre::String EditorApp::getObjIdBySceneNode(Ogre::String nodeName)
//{
//	if(ObjectMap.count(nodeName))
//	{
//		obj_container temp = ObjectMap[nodeName];
//		return temp.ObjId;
//	}
//	else
//	{
//		return "";
//	}
//}
//
//
//OgreNewt::Body *EditorApp::getBodyBySceneNode(Ogre::String nodeName)
//{
//	if(ObjectMap.count(nodeName))
//	{
//		obj_container temp = ObjectMap[nodeName];
//		return temp.NewtonBody;
//	}
//	else
//	{
//		return 0;
//	}
//	
//}

void EditorApp::saveCurrentLevel(Ogre::String name)
{
	if(currentLevel)
	{
		Ogre::String filename;
		if(name == "")
			filename = currentLevel->getFileName();
		else
			filename = name;
        Ogre::String base,ext;
        Ogre::StringUtil::splitBaseFilename(filename,base,ext);
		if(base != "")
		{
			Ogre::String saveTo = levelPath+"/"+base+ZLEVELEXT;
			/*if(!Ogre::StringUtil::endsWith(saveTo,LEVELEXT))
			{
				saveTo += LEVELEXT;
			}*/
			currentLevel->saveToFile(saveTo);
			//updateLevelList();
		}
		else
		{
			mLog("saveCurrentLevel(\""+name+"\") failed: basename \""+base+"\" is empty");
		}
	}

}

//
//void EditorApp::saveCurrentLvl()
//{
//	if(currentLevel)
//	{
//		Ogre::String filename = currentLevel->getFileName();
//        Ogre::String base,ext;
//        Ogre::StringUtil::splitBaseFilename(filename,base,ext);
//		if(base != "")
//		{
//			Ogre::String saveTo = levelPath+"/"+base+ZLEVELEXT;
//			/*if(!Ogre::StringUtil::endsWith(saveTo,LEVELEXT))
//			{
//				saveTo += LEVELEXT;
//			}*/
//			currentLevel->saveToFile(saveTo);
//			updateLevelList();
//		}
//		else
//		{
//			saveCurrentLvlAs();
//		}
//	}
//
//}
//
//void EditorApp::saveCurrentLvlAs()
//{
//	if(currentLevel)
//	{
//		/*Ogre::String saveTo = levelPath+"/"+currentLevel->getFileName();
//		currentLevel->saveToFile(saveTo);*/
//		showInputDialog("Input Filename","Save As...");
//		inputDialogReturn = &EditorApp::saveAsDialogConfirm;
//		//inputdialog->getChild("InputDialog/OK")->subscribeEvent(CEGUI::Window::EventMouseClick,CEGUI::Event::Subscriber(&EditorApp::saveAsDialogConfirm,this));
//		
//	}
//
//}

//CEGUI::Renderer *EditorApp::getCEGUIRenderer()
//{
//	return NULL;//mGUIRenderer;
//}

void EditorApp::cleanUp()
{
	if(oldLevel)
	{
		delete oldLevel;
		oldLevel = NULL;
		log("level gelöscht");
	}
}

//
//void EditorApp::showDetailsDialog(GameObject *obj)
//{
//	qtApp->mainWnd->showObjectDetails(obj);
//	return;
//    //auswahl sperren
//    qtApp->getGameLogic()->setSelectionLocked(true);
//	
//	
//	detailswnd->enable();
//	detailswnd->activate();
//	detailswnd->setVisible(true);
//	detailswnd->setText(obj->getID().c_str());
//	CEGUI::WindowManager *wmgr = CEGUI::WindowManager::getSingletonPtr();
//	Ogre::Vector3 mPos = obj->getPosition();
//	Ogre::Quaternion q = obj->getOrientation();
//	Ogre::Vector3 scale = obj->getScale();
//	wmgr->getWindow("editObjWnd/position")->setText(str(mPos).c_str());
//	wmgr->getWindow("editObjWnd/orientation")->setText(str(q).c_str());
//	wmgr->getWindow("editObjWnd/scale")->setText(str(scale).c_str());
//	//Ogre::Vector3 dir = q*Ogre::Vector3::NEGATIVE_UNIT_Z;
//	Ogre::Degree angle;
//	Ogre::Vector3 axis;
//	q.ToAngleAxis(angle,axis);
//	wmgr->getWindow("editObjWnd/axis")->setText(str(axis).c_str());
//	wmgr->getWindow("editObjWnd/angle")->setText(str(angle).c_str());
//	wmgr->getWindow("editObjWnd/charOptions")->setVisible(false);
//	wmgr->getWindow("editObjWnd/contOptions")->setVisible(false);
//	wmgr->getWindow("editObjWnd/staticOptions")->setVisible(false);
//	wmgr->getWindow("editObjWnd/doorOptions")->setVisible(false);
//	wmgr->getWindow("editObjWnd/lightOptions")->setVisible(false);
//
//	switch(obj->getType())
//	{
//	case otDoor:
//	case otEntrance:
//		{
//			wmgr->getWindow("editObjWnd/doorOptions")->setVisible(true);
//			wmgr->getWindow("editObjWnd/doorOptions")->activate();
//			CEGUI::Window *objName = wmgr->getWindow("editObjWnd/doorOptions/name");
//			CEGUI::Window *objTargetName = wmgr->getWindow("editObjWnd/doorOptions/targetname");
//			CEGUI::Window *objTargetLevel = wmgr->getWindow("editObjWnd/doorOptions/targetlvl");
//
//			objName->setText("");
//			objTargetName->setText("");
//			objTargetLevel->setText("");
//
//			objName->disable();
//			objTargetName->disable();
//			objTargetLevel->disable();
//			
//			if(obj->getType() == otDoor)
//			{
//				//activator. hat targetLevel und targetName
//				DoorData stuff = static_cast<DoorObject*>(obj)->getDoorData();
//				objTargetName->enable();
//				objTargetName->setText(stuff.targetEntrance.c_str());
//				objTargetLevel->enable();
//				objTargetLevel->setText(stuff.targetLevel.c_str());
//			}
//			else if(obj->getType() == otEntrance)
//			{
//				
//				objName->enable();
//				objName->setText(static_cast<entranceObject*>(obj)->getName());
//				
//			}
//		}
//		break;
//	case otChar:
//		{
//			GameChar *ch = static_cast<GameChar*>(obj);
//			wmgr->getWindow("editObjWnd/charOptions")->activate();
//			wmgr->getWindow("editObjWnd/charOptions")->setVisible(true);
//			wmgr->getWindow("editObjWnd/charOptions/eqmelee")->setText(ch->getMeleeId().c_str());
//			wmgr->getWindow("editObjWnd/charOptions/eqranged")->setText(ch->getRangedId().c_str());
//			wmgr->getWindow("editObjWnd/charOptions/eqarmor")->setText(ch->getArmorId().c_str());
//			wmgr->getWindow("editObjWnd/charOptions/eqshield")->setText(ch->getShieldId().c_str());
//			wmgr->getWindow("editObjWnd/charOptions/eqelse1")->setText(ch->getElse1Id().c_str());
//			wmgr->getWindow("editObjWnd/charOptions/eqelse2")->setText(ch->getElse2Id().c_str());
//			wmgr->getWindow("editObjWnd/charOptions/eqelse3")->setText(ch->getElse3Id().c_str());
//			wmgr->getWindow("editObjWnd/charOptions/eqelse4")->setText(ch->getElse4Id().c_str());
//			wmgr->getWindow("editObjWnd/charOptions/eqbag")->setText(ch->getBagId().c_str());
//
//		}
//		break;
//	case otContainer:
//		{
//			wmgr->getWindow("editObjWnd/contOptions")->activate();
//			wmgr->getWindow("editObjWnd/contOptions")->setVisible(true);
//		}
//		break;
//	case otLight:
//		{
//			LightObject *light = static_cast<LightObject*>(obj);
//			wmgr->getWindow("editObjWnd/lightOptions")->activate();
//			wmgr->getWindow("editObjWnd/lightOptions")->setVisible(true);
//
//			Ogre::Light *l = light->getLight();
//			CEGUI::String lightType = "";
//			switch(l->getType())
//			{
//			case Ogre::Light::LT_DIRECTIONAL:
//				lightType = "directional";
//				break;
//			case Ogre::Light::LT_POINT:
//				lightType = "point";
//				break;
//			case Ogre::Light::LT_SPOTLIGHT:
//				lightType = "spotlight";
//				break;
//			}
//			wmgr->getWindow("editObjWnd/lightOptions/type")->setText(lightType);
//			//COlours
//			wmgr->getWindow("editObjWnd/lightOptions/diffuse")->setText(str(l->getDiffuseColour()).c_str());
//			wmgr->getWindow("editObjWnd/lightOptions/specular")->setText(str(l->getSpecularColour()).c_str());
//			//Attenuation
//			wmgr->getWindow("editObjWnd/lightOptions/aRange")->setText(str(l->getAttenuationRange()).c_str());
//			wmgr->getWindow("editObjWnd/lightOptions/aConst")->setText(str(l->getAttenuationConstant()).c_str());
//			wmgr->getWindow("editObjWnd/lightOptions/aLinear")->setText(str(l->getAttenuationLinear()).c_str());
//			wmgr->getWindow("editObjWnd/lightOptions/aQuad")->setText(str(l->getAttenuationQuadric()).c_str());
//			//Spotlight
//			/*if(l->getType() == Ogre::Light::LT_SPOTLIGHT)
//			{
//				wmgr->getWindow("editObjWnd/lightOptions/sInner")->setText(str(l->getSpotlightInnerAngle()).c_str());
//				wmgr->getWindow("editObjWnd/lightOptions/sOuter")->setText(str(l->getSpotlightOuterAngle()).c_str());
//				wmgr->getWindow("editObjWnd/lightOptions/sFalloff")->setText(str(l->getSpotlightFalloff()).c_str());	
//			}
//			else
//			{
//				wmgr->getWindow("editObjWnd/lightOptions/sInner")->setText("");
//				wmgr->getWindow("editObjWnd/lightOptions/sOuter")->setText("");
//				wmgr->getWindow("editObjWnd/lightOptions/sFalloff")->setText("");
//			}*/
//			wmgr->getWindow("editObjWnd/lightOptions/sInner")->setText(str(l->getSpotlightInnerAngle()).c_str());
//			wmgr->getWindow("editObjWnd/lightOptions/sOuter")->setText(str(l->getSpotlightOuterAngle()).c_str());
//			wmgr->getWindow("editObjWnd/lightOptions/sFalloff")->setText(str(l->getSpotlightFalloff()).c_str());	
//	
//			wmgr->getWindow("editObjWnd/lightOptions/powerscale")->setText(str(l->getPowerScale()).c_str());	
//			
//
//		}
//		break;
//	case otStatic:
//		{
//			wmgr->getWindow("editObjWnd/staticOptions")->activate();
//			wmgr->getWindow("editObjWnd/staticOptions")->setVisible(true);
//			WorldArtData cur = obj->getWorldArt()->getData();
//
//			CEGUI::Window *meshEdit = wmgr->getWindow("editObjWnd/staticOptions/mesh");
//			CEGUI::Window *matEdit = wmgr->getWindow("editObjWnd/staticOptions/material");
//			CEGUI::Window *browsePS = wmgr->getWindow("editObjWnd/staticOptions/browseps");
//			CEGUI::Window *browseMat = wmgr->getWindow("editObjWnd/staticOptions/browsemat");
//			meshEdit->setText("");
//			matEdit->setText("");
//			meshEdit->disable();
//			matEdit->disable();
//
//			browsePS->disable();
//			browseMat->disable();
//
//			CEGUI::Window *pl_normal = wmgr->getWindow("editObjWnd/staticOptions/plane/normal");
//			CEGUI::Window *pl_constant = wmgr->getWindow("editObjWnd/staticOptions/plane/constant");
//			CEGUI::Window *pl_width = wmgr->getWindow("editObjWnd/staticOptions/plane/width");
//			CEGUI::Window *pl_height = wmgr->getWindow("editObjWnd/staticOptions/plane/height");
//			CEGUI::Window *pl_xseg = wmgr->getWindow("editObjWnd/staticOptions/plane/xseg");
//			CEGUI::Window *pl_yseg = wmgr->getWindow("editObjWnd/staticOptions/plane/yseg");
//			CEGUI::Window *pl_upVector = wmgr->getWindow("editObjWnd/staticOptions/plane/upVector");
//			CEGUI::Window *pl_vtile = wmgr->getWindow("editObjWnd/staticOptions/plane/vtile");
//			CEGUI::Window *pl_utile = wmgr->getWindow("editObjWnd/staticOptions/plane/utile");
//			CEGUI::Window *pl_numtexcoords = wmgr->getWindow("editObjWnd/staticOptions/plane/numtexcoords");
//
//			pl_normal->disable();
//			pl_constant->disable();
//			pl_width->disable();
//			pl_height->disable();
//			pl_xseg->disable();
//			pl_yseg->disable();
//			pl_upVector->disable();
//			pl_vtile->disable();
//			pl_utile->disable();
//			pl_numtexcoords->disable();
//
//			pl_normal->setText("");
//			pl_constant->setText("");
//			pl_width->setText("");
//			pl_height->setText("");
//			pl_xseg->setText("");
//			pl_yseg->setText("");
//			pl_upVector->setText("");
//			pl_vtile->setText("");
//			pl_utile->setText("");
//			pl_numtexcoords->setText("");
//			switch(cur.type)
//			{
//			case WT_MESH:
//				meshEdit->enable();
//				matEdit->enable();
//				meshEdit->setText(cur.filename.c_str());
//				matEdit->setText(cur.material.c_str());
//				browseMat->enable();
//				break;
//			case WT_BILLBOARD:
//				matEdit->enable();
//				matEdit->setText(cur.material.c_str());
//				browseMat->enable();
//				break;
//			case WT_DOTSCENE:
//				meshEdit->enable();
//				meshEdit->setText(cur.filename.c_str());
//				break;
//			case WT_PARTICLESYSTEM:
//				meshEdit->enable();
//				meshEdit->setText(cur.material.c_str());
//				browsePS->enable();
//				break;
//			case WT_PLANE:
//				matEdit->enable();
//				matEdit->setText(cur.material.c_str());
//
//				browseMat->enable();
//
//				pl_normal->enable();
//				pl_constant->enable();
//				pl_width->enable();
//				pl_height->enable();
//				pl_xseg->enable();
//				pl_yseg->enable();
//				pl_upVector->enable();
//				pl_vtile->enable();
//				pl_utile->enable();
//				pl_numtexcoords->enable();
//
//				pl_normal->setText(str(cur.pData.normal).c_str());
//				pl_constant->setText(str(cur.pData.constant).c_str());
//				pl_width->setText(str(cur.pData.width).c_str());
//				pl_height->setText(str(cur.pData.height).c_str());
//				pl_xseg->setText(str(cur.pData.xSeg).c_str());
//				pl_yseg->setText(str(cur.pData.ySeg).c_str());
//				pl_upVector->setText(str(cur.pData.up).c_str());
//				pl_vtile->setText(str(cur.pData.vTile).c_str());
//				pl_utile->setText(str(cur.pData.uTile).c_str());
//				pl_numtexcoords->setText(str(cur.pData.numTexCoordSets).c_str());
//
//			}
//			
//			
//			bool col = static_cast<StaticObject*>(obj)->getCollision();
//			static_cast<CEGUI::Checkbox*>(wmgr->getWindow("editObjWnd/staticOptions/castShadows"))->setSelected(cur.shadow);//->setText(cur.material.c_str());
//			static_cast<CEGUI::Checkbox*>(wmgr->getWindow("editObjWnd/staticOptions/isstatic"))->setSelected(col);//->setText(cur.material.c_str());
//			/*if(cur.type == WT_PLANE)
//			{
//				wmgr->getWindow("editObjWnd/staticOptions/plane/normal")->setText(str(cur.pData.normal).c_str());
//				wmgr->getWindow("editObjWnd/staticOptions/plane/constant")->setText(str(cur.pData.constant).c_str());
//				wmgr->getWindow("editObjWnd/staticOptions/plane/width")->setText(str(cur.pData.width).c_str());
//				wmgr->getWindow("editObjWnd/staticOptions/plane/height")->setText(str(cur.pData.height).c_str());
//				wmgr->getWindow("editObjWnd/staticOptions/plane/xseg")->setText(str(cur.pData.xSeg).c_str());
//				wmgr->getWindow("editObjWnd/staticOptions/plane/yseg")->setText(str(cur.pData.ySeg).c_str());
//				wmgr->getWindow("editObjWnd/staticOptions/plane/upVector")->setText(str(cur.pData.up).c_str());
//				wmgr->getWindow("editObjWnd/staticOptions/plane/vtile")->setText(str(cur.pData.vTile).c_str());
//				wmgr->getWindow("editObjWnd/staticOptions/plane/utile")->setText(str(cur.pData.uTile).c_str());
//				wmgr->getWindow("editObjWnd/staticOptions/plane/numtexcoords")->setText(str(cur.pData.numTexCoordSets).c_str());			
//			}
//			else
//			{
//				wmgr->getWindow("editObjWnd/staticOptions/plane/normal")->setText("");
//				wmgr->getWindow("editObjWnd/staticOptions/plane/constant")->setText("");
//				wmgr->getWindow("editObjWnd/staticOptions/plane/width")->setText("");
//				wmgr->getWindow("editObjWnd/staticOptions/plane/height")->setText("");
//				wmgr->getWindow("editObjWnd/staticOptions/plane/xseg")->setText("");
//				wmgr->getWindow("editObjWnd/staticOptions/plane/yseg")->setText("");
//				wmgr->getWindow("editObjWnd/staticOptions/plane/upVector")->setText("");
//				wmgr->getWindow("editObjWnd/staticOptions/plane/vtile")->setText("");
//				wmgr->getWindow("editObjWnd/staticOptions/plane/utile")->setText("");
//				wmgr->getWindow("editObjWnd/staticOptions/plane/numtexcoords")->setText("");		
//			}
//			*/
//
//
//		}
//		break;
//	case otWater:
//		{
//			WaterPlane *water = static_cast<WaterPlane*>(obj);
//			wmgr->getWindow("editObjWnd/waterOptions")->activate();
//			wmgr->getWindow("editObjWnd/waterOptions")->setVisible(true);
//			//WorldArtData waData = obj->getWorldArt()->getData();
//			FluidData fd = water->getFluidData();
//
//			
//
//			CEGUI::Window *matEdit = wmgr->getWindow("editObjWnd/waterOptions/material");
//			//CEGUI::Window *browseMat = wmgr->getWindow("editObjWnd/waterOptions/browsemat");
//			matEdit->setText("");
//			
//
//			//CEGUI::Window *pl_normal = wmgr->getWindow("editObjWnd/waterOptions/plane/normal");
//			//CEGUI::Window *pl_constant = wmgr->getWindow("editObjWnd/waterOptions/plane/constant");
//			CEGUI::Window *pl_width = wmgr->getWindow("editObjWnd/waterOptions/plane/width");
//			CEGUI::Window *pl_height = wmgr->getWindow("editObjWnd/waterOptions/plane/height");
//			CEGUI::Window *pl_xseg = wmgr->getWindow("editObjWnd/waterOptions/plane/xseg");
//			CEGUI::Window *pl_yseg = wmgr->getWindow("editObjWnd/waterOptions/plane/yseg");
//			CEGUI::Window *pl_upVector = wmgr->getWindow("editObjWnd/waterOptions/plane/upVector");
//			CEGUI::Window *pl_vtile = wmgr->getWindow("editObjWnd/waterOptions/plane/vtile");
//			CEGUI::Window *pl_utile = wmgr->getWindow("editObjWnd/waterOptions/plane/utile");
//			CEGUI::Window *pl_numtexcoords = wmgr->getWindow("editObjWnd/waterOptions/plane/numtexcoords");
//
//			CEGUI::Window *w_density = wmgr->getWindow("editObjWnd/waterOptions/density");
//			CEGUI::Window *w_linvisc = wmgr->getWindow("editObjWnd/waterOptions/linvisc");
//			CEGUI::Window *w_angvisc = wmgr->getWindow("editObjWnd/waterOptions/angvisc");
//			CEGUI::Window *w_sound = wmgr->getWindow("editObjWnd/waterOptions/sound");
//
//			//fd.plane
//			matEdit->setText(fd.material.c_str());
//
//			
//
//			/*pl_normal->setText(str(waData.pData.normal).c_str());
//			pl_constant->setText(str(waData.pData.constant).c_str());*/
//			pl_width->setText(str(fd.plane.width).c_str());
//			pl_height->setText(str(fd.plane.height).c_str());
//			pl_xseg->setText(str(fd.plane.xSeg).c_str());
//			pl_yseg->setText(str(fd.plane.ySeg).c_str());
//			pl_upVector->setText(str(fd.plane.up).c_str());
//			pl_vtile->setText(str(fd.plane.vTile).c_str());
//			pl_utile->setText(str(fd.plane.uTile).c_str());
//			pl_numtexcoords->setText(str(fd.plane.numTexCoordSets).c_str());
//
//			w_density->setText(str(fd.density));
//			w_linvisc->setText(str(fd.linearViscosity));
//			w_angvisc->setText(str(fd.angularViscosity));
//			w_sound->setText(fd.sound);
//
//
//		}
//		break;
//	}
//
//	//
//	//wmgr->getWindow("editObjWnd/xCoord")->setText(Ogre::StringConverter::toString(mPos.x).c_str());
//	//wmgr->getWindow("editObjWnd/yCoord")->setText(Ogre::StringConverter::toString(mPos.y).c_str());
//	//wmgr->getWindow("editObjWnd/zCoord")->setText(Ogre::StringConverter::toString(mPos.z).c_str());
//
//	//wmgr->getWindow("editObjWnd/xQuat")->setText(Ogre::StringConverter::toString(q.x).c_str());
//	//wmgr->getWindow("editObjWnd/yQuat")->setText(Ogre::StringConverter::toString(q.y).c_str());
//	//wmgr->getWindow("editObjWnd/zQuat")->setText(Ogre::StringConverter::toString(q.z).c_str());
//	//wmgr->getWindow("editObjWnd/wQuat")->setText(Ogre::StringConverter::toString(q.w).c_str());
//
//	//wmgr->getWindow("editObjWnd/xScale")->setText(Ogre::StringConverter::toString(scale.x).c_str());
//	//wmgr->getWindow("editObjWnd/yScale")->setText(Ogre::StringConverter::toString(scale.y).c_str());
//	//wmgr->getWindow("editObjWnd/zScale")->setText(Ogre::StringConverter::toString(scale.z).c_str());
//
//	
//	curEditing=obj;
//
//
//
//}
//
//void EditorApp::hideDetailsDialog()
//{
//    if(qtApp->getGameLogic())
//        qtApp->getGameLogic()->setSelectionLocked(false);
//	detailswnd->disable();
//	detailswnd->setVisible(false);
//	curEditing = NULL;
//
//}
//
//void EditorApp::showInputDialog(Ogre::String text, Ogre::String title)
//{
//	inputdialog->enable();
//	inputdialog->setVisible(true);
//	inputdialog->activate();
//	inputdialog->setText(title.c_str());
//	inputdialog->getChild("InputDialog/Text")->setText(text.c_str());
//}
//void EditorApp::hideInputDialog()
//{
//	//inputdialog->getChild("InputDialog/OK")->removeAllEvents();
//	inputdialog->disable();
//	inputdialog->setVisible(false);
//}
//	
//
//
//bool EditorApp::showLevelSettings(const CEGUI::EventArgs& e)
//{
//	if(!currentLevel)
//		return true;
//	CEGUI::WindowManager *wmgr = CEGUI::WindowManager::getSingletonPtr();
//	Ogre::ColourValue amb = currentLevel->mSceneMgr->getAmbientLight();
//
//	wmgr->getWindow("levelsettings/ambientcolour")->setText(str(amb));
//	wmgr->getWindow("levelsettings/bgcolour")->setText(str(currentLevel->backgroundColour));
//	/*wmgr->getWindow("levelsettings/ambientcolourG")->setText(str(amb.g));
//	wmgr->getWindow("levelsettings/ambientcolourB")->setText(str(amb.b));
//	wmgr->getWindow("levelsettings/ambientcolourA")->setText(str(amb.a));*/
//	wmgr->getWindow("levelsettings/gravity")->setText(str(currentLevel->gravity));
//	wmgr->getWindow("levelsettings/lvlname")->setText(currentLevel->mName);
//	//wmgr->getWindow("levelsettings/gravity")->setText(str(currentLevel->gravity));
//
//	//fog
//	CEGUI::String fog = "none";
//	switch(currentLevel->mFogMode)
//	{
//	case Ogre::FOG_LINEAR:
//		fog = "linear";
//		break;
//	case Ogre::FOG_EXP:
//		fog = "exp";
//		break;
//	case Ogre::FOG_EXP2:
//		fog = "exp2";
//		break;
//	};
//	static_cast<CEGUI::Combobox*>(wmgr->getWindow("levelsettings/fog_type"))->setText(fog);
//
//	wmgr->getWindow("levelsettings/fog_density")->setText(str(currentLevel->mFogDensity).c_str());
//	wmgr->getWindow("levelsettings/fog_color")->setText(str(currentLevel->mFogColor).c_str());
//	wmgr->getWindow("levelsettings/fog_start")->setText(str(currentLevel->mFogStart).c_str());
//	wmgr->getWindow("levelsettings/fog_end")->setText(str(currentLevel->mFogEnd).c_str());
//
//	//jetzt mal sound versuchen...
//	Ogre::String explSounds = "";
//	for(Ogre::StringVector::iterator itr = currentLevel->musicExplore.begin();itr!=currentLevel->musicExplore.end();itr++)
//	{
//		Ogre::String str = *itr;
//		explSounds+=(str+"\n");
//	}
//	wmgr->getWindow("levelsettings/music_expore")->setText(explSounds.c_str());
//
//	Ogre::String battleSounds = "";
//	for(Ogre::StringVector::iterator itr = currentLevel->musicBattle.begin();itr!=currentLevel->musicBattle.end();itr++)
//	{
//		Ogre::String str = *itr;
//		battleSounds+=(str+"\n");
//	}
//	wmgr->getWindow("levelsettings/music_battle")->setText(battleSounds.c_str());
//
//	
//
//	Ogre::String ambLoop = "";
//	for(Ogre::StringVector::iterator itr = currentLevel->ambientLoop.begin();itr!=currentLevel->ambientLoop.end();itr++)
//	{
//		Ogre::String str = *itr;
//		ambLoop+=(str+"\n");
//	}
//	wmgr->getWindow("levelsettings/ambient_loop")->setText(ambLoop.c_str());
//
//	Ogre::String ambRand = "";
//	for(Ogre::StringVector::iterator itr = currentLevel->ambientRandom.begin();itr!=currentLevel->ambientRandom.end();itr++)
//	{
//		Ogre::String str = *itr;
//		ambRand+=(str+"\n");
//	}
//	wmgr->getWindow("levelsettings/ambient_random")->setText(ambRand.c_str());
//	wmgr->getWindow("levelsettings/amb_rand_min")->setText(str(currentLevel->wait_min).c_str());
//	wmgr->getWindow("levelsettings/amb_rand_max")->setText(str(currentLevel->wait_max).c_str());
//
//
//	//levelsettings/ambient_loop
//
//	if(currentLevel->mSceneMgr->isSkyBoxEnabled())
//	{
//        Ogre::SceneManager::SkyBoxGenParameters skyBoxParams;
//        Ogre::String material;
//
//        currentLevel->getSkyBoxParams(skyBoxParams,material);	
//		static_cast<CEGUI::Checkbox*>(wmgr->getWindow("levelsettings/skyboxuse"))->setSelected(true);
//		wmgr->getWindow("levelsettings/skyboxmaterial")->setText(material);
//        wmgr->getWindow("levelsettings/skyboxdist")->setText(str(skyBoxParams.skyBoxDistance));
//
//	}
//	else
//	{
//		static_cast<CEGUI::Checkbox*>(wmgr->getWindow("levelsettings/skyboxuse"))->setSelected(false);
//		wmgr->getWindow("levelsettings/skyboxmaterial")->setText("");
//        wmgr->getWindow("levelsettings/skyboxdist")->setText("5000");
//	}
//	if(currentLevel->mSceneMgr->isSkyPlaneEnabled())
//	{
//        Ogre::SceneManager::SkyPlaneGenParameters gp;
//        Ogre::Real dist;
//        Ogre::String material;
//        currentLevel->getSkyPlaneParams(gp,material,dist);
//	   
//		static_cast<CEGUI::Checkbox*>(wmgr->getWindow("levelsettings/skyplaneuse"))->setSelected(true);
//		wmgr->getWindow("levelsettings/skyplanematerial")->setText(material);
//		//Ogre::SceneManager::SkyPlaneGenParameters gp = currentLevel->mSceneMgr->getSkyPlaneGenParameters();
//
//		wmgr->getWindow("levelsettings/skyplanescale")->setText(str(gp.skyPlaneScale));
//		wmgr->getWindow("levelsettings/skyplanetiling")->setText(str(gp.skyPlaneTiling));
//		wmgr->getWindow("levelsettings/skyplanexseg")->setText(str(gp.skyPlaneXSegments));
//		wmgr->getWindow("levelsettings/skyplaneyseg")->setText(str(gp.skyPlaneYSegments));
//		wmgr->getWindow("levelsettings/skyplanebow")->setText(str(gp.skyPlaneBow));
//        wmgr->getWindow("levelsettings/skyplanedist")->setText(str(dist));
//
//
//	}
//	else
//	{
//		static_cast<CEGUI::Checkbox*>(wmgr->getWindow("levelsettings/skyplaneuse"))->setSelected(false);
//		wmgr->getWindow("levelsettings/skyplanematerial")->setText("");
//		wmgr->getWindow("levelsettings/skyplanescale")->setText("1000");
//		wmgr->getWindow("levelsettings/skyplanetiling")->setText("10");
//		wmgr->getWindow("levelsettings/skyplanexseg")->setText("1");
//		wmgr->getWindow("levelsettings/skyplaneyseg")->setText("1");
//		wmgr->getWindow("levelsettings/skyplanebow")->setText("0");
//        wmgr->getWindow("levelsettings/skyplanedist")->setText("3000");
//	}
//	if(currentLevel->mSceneMgr->isSkyDomeEnabled())
//	{
//        Ogre::SceneManager::SkyDomeGenParameters gp;
//        Ogre::String material;
//
//        currentLevel->getSkyDomeParams(gp,material);
//         
//
//		static_cast<CEGUI::Checkbox*>(wmgr->getWindow("levelsettings/skydomeuse"))->setSelected(true);
//		wmgr->getWindow("levelsettings/skydomematerial")->setText(material);
////		Ogre::SceneManager::SkyDomeGenParameters gp = currentLevel->mSceneMgr->getSkyDomeGenParameters();
//        
//		wmgr->getWindow("levelsettings/skydomecurvature")->setText(str(gp.skyDomeCurvature));
//		wmgr->getWindow("levelsettings/skydometiling")->setText(str(gp.skyDomeTiling));
//		wmgr->getWindow("levelsettings/skydomexseg")->setText(str(gp.skyDomeXSegments));
//		wmgr->getWindow("levelsettings/skydomeyseg")->setText(str(gp.skyDomeYSegments));
//
//        wmgr->getWindow("levelsettings/skydomedist")->setText(str(gp.skyDomeDistance));		
//
//
//	}
//	else
//	{
//		static_cast<CEGUI::Checkbox*>(wmgr->getWindow("levelsettings/skydomeuse"))->setSelected(false);
//		wmgr->getWindow("levelsettings/skydomematerial")->setText("");
//		
//
//		wmgr->getWindow("levelsettings/skydomecurvature")->setText("10");
//		wmgr->getWindow("levelsettings/skydometiling")->setText("8");
//		wmgr->getWindow("levelsettings/skydomexseg")->setText("16");
//		wmgr->getWindow("levelsettings/skydomeyseg")->setText("16");
//        wmgr->getWindow("levelsettings/skydomedist")->setText("4000");		
//	}
//
//    //jetzt terrain
//    if(currentLevel->getTerrainManager())
//    {
//        //
//        static_cast<CEGUI::Checkbox*>(wmgr->getWindow("levelsettings/terrain/use"))->setSelected(true);
//        unsigned int hmWidth, hmHeight, lmWidth, lmHeight, splatWidth, splatHeight;
//        currentLevel->getHeightmapDimensions(hmWidth,hmHeight);
//   
//        currentLevel->getLightmapDimensions(lmWidth,lmHeight);
//        currentLevel->getSplattingDimensions(splatWidth, splatHeight);
//        //splatChannels = currentLevel->getNumSplatChannels();
//
//        wmgr->getWindow("levelsettings/terrain/heightmapX")->setText(str(hmWidth).c_str());
//        wmgr->getWindow("levelsettings/terrain/heightmapZ")->setText(str(hmHeight).c_str());
//        wmgr->getWindow("levelsettings/terrain/lightmapX")->setText(str(lmWidth).c_str());
//        wmgr->getWindow("levelsettings/terrain/lightmapZ")->setText(str(lmHeight).c_str());
//        wmgr->getWindow("levelsettings/terrain/covmapX")->setText(str(splatWidth).c_str());
//        wmgr->getWindow("levelsettings/terrain/covmapZ")->setText(str(splatHeight).c_str());
//        //wmgr->getWindow("levelsettings/terrain/numchannels")->setText(str(splatChannels).c_str());
//    }
//    else
//    {
//        static_cast<CEGUI::Checkbox*>(wmgr->getWindow("levelsettings/terrain/use"))->setSelected(false);
//        
//
//        wmgr->getWindow("levelsettings/terrain/heightmapX")->setText("129");
//        wmgr->getWindow("levelsettings/terrain/heightmapZ")->setText("129");
//        wmgr->getWindow("levelsettings/terrain/lightmapX")->setText("128");
//        wmgr->getWindow("levelsettings/terrain/lightmapZ")->setText("128");
//        wmgr->getWindow("levelsettings/terrain/covmapX")->setText("128");
//        wmgr->getWindow("levelsettings/terrain/covmapZ")->setText("128");
//        //wmgr->getWindow("levelsettings/terrain/numchannels")->setText("3");
//    }
//
//
//
//	levelsettings->enable();
//	levelsettings->setVisible(true);
//	levelsettings->activate();
//	return true;
//}
//void EditorApp::hideLevelSettings()
//{
//	levelsettings->disable();
//	levelsettings->setVisible(false);
//}
//
//bool EditorApp::showEditorSettings(const CEGUI::EventArgs& e)
//{
//	
//	editorsettings->enable();
//	editorsettings->setVisible(true);
//	levelsettings->activate();
//	editorsettings->getChild("editorsettings/gridsnap")->setText(str(gridSnap).c_str());
//	editorsettings->getChild("editorsettings/anglesnap")->setText(str(angleSnap).c_str());
//	editorsettings->getChild("editorsettings/scalesnap")->setText(str(scaleSnap).c_str());
//
//	editorsettings->getChild("editorsettings/sens_move")->setText(str(qtApp->getGameLogic()->mMove).c_str());
//	editorsettings->getChild("editorsettings/sens_rotate")->setText(str(qtApp->getGameLogic()->mRotate).c_str());
//	editorsettings->getChild("editorsettings/sens_scroll")->setText(str(qtApp->getGameLogic()->mScroll).c_str());
//
//	editorsettings->getChild("editorsettings/taxesscale")->setText(str(axesScaleFactor).c_str());
//	editorsettings->getChild("editorsettings/wndname")->setText(windowName.c_str());
//	editorsettings->getChild("editorsettings/camstartpos")->setText(str(camStartPos).c_str());
//
////	static_cast<CEGUI::Checkbox*>(editorsettings->getChild("editorsettings/loadterrancollision"))->setSelected(loadTerrainCollision);
//	static_cast<CEGUI::Checkbox*>(editorsettings->getChild("editorsettings/showfog"))->setSelected(showFog);
//		
//
//
//	return true;
//}
//void EditorApp::hideEditorSettings()
//{
//	editorsettings->disable();
//	editorsettings->setVisible(false);
//}


//bool EditorApp::newLevel(const CEGUI::EventArgs& e)
//{
//	loadLevel("");
//	return true;
//}

void EditorApp::setupConfig(TiXmlElement *elem)
{
    
    //settings
    OgreXmlElement *settings = static_cast<OgreXmlElement*>(elem->FirstChildElement("settings"));
    OgreXmlElement *option = settings->FirstChildElement("option"); 

    //hole mir mal das gamelogic
    QtOgre::EditGameLogic* logic = qtApp->getGameLogic();

    while(option)
    {
        Ogre::String varName = option->Attribute("name");
        Ogre::String value= option->Attribute("value");
        Ogre::StringUtil::toLowerCase(varName);
        if(varName == "gridsnap")
		{
			gridSnap = Ogre::StringConverter::parseReal(value);
		}
		else if(varName == "anglesnap")
		{
			angleSnap = Ogre::StringConverter::parseReal(value);
		}
		else if(varName == "scalesnap")
		{
			scaleSnap = Ogre::StringConverter::parseReal(value);
		}
		/*else if(varName == "loadterraincollision")
		{
			loadTerrainCollision = Ogre::StringConverter::parseBool(value);
		}*/
		else if(varName == "showfog")
		{
			showFog = Ogre::StringConverter::parseBool(value);
			//static_cast<CEGUI::Checkbox*>(wmgr->getWindow("ShowFog"))->setSelected(showFog);
		}
		else if(varName == "usegridsnap")
		{
			snapOnGrid = Ogre::StringConverter::parseBool(value);
			//static_cast<CEGUI::Checkbox*>(wmgr->getWindow("GridSnap"))->setSelected(snapOnGrid);
			
		}
		else if(varName == "useanglesnap")
		{
			snapOnAngle = Ogre::StringConverter::parseBool(value);
			//static_cast<CEGUI::Checkbox*>(wmgr->getWindow("AngleSnap"))->setSelected(snapOnAngle);
		}
		else if(varName == "usescalesnap")
		{
			snapOnScale = Ogre::StringConverter::parseBool(value);
			//static_cast<CEGUI::Checkbox*>(wmgr->getWindow("ScaleSnap"))->setSelected(snapOnScale);
		}
		else if(varName == "move_sensitivity")
		{
			logic->mMove = Ogre::StringConverter::parseReal(value);					
		}
		else if(varName == "rotate_sensitivity")
		{
			logic->mRotate = Ogre::StringConverter::parseReal(value);					
		}
		else if(varName == "scroll_sensitivity")
		{
			logic->mScroll = Ogre::StringConverter::parseReal(value);					
		}
		else if(varName == "transformaxes_scale")
		{
			axesScaleFactor = Ogre::StringConverter::parseReal(value);					
		}
		else if(varName == "windowname")
		{
			windowName = value;	
			setWindowTitle(windowName);
			//mWindow->getName
		}
		else if(varName == "cam_startposition")
		{
			camStartPos = Ogre::StringConverter::parseVector3(value);					
		}
		else if(varName == "showDebugWnd")
		{
			showDebugWnd = Ogre::StringConverter::parseBool(value);	
//		CEGUI::Window *debugwnd = wmgr->getWindow("debugWnd");
			/*debugwnd->setVisible(showDebugWnd);
			debugwnd->setEnabled(showDebugWnd);*/

		}
        option = option->NextSiblingElement("option");
    }
    TiXmlElement *keys = elem->FirstChildElement("keys")->FirstChildElement("key");
    while(keys)
    {
        Ogre::String varName = keys->Attribute("name");
        Ogre::String value= keys->Attribute("value");
        Ogre::StringUtil::toLowerCase(varName);
        if(varName == "clone")
		{
			logic->key_clone = stringToUniKey(value);
		}
		else if(varName == "move_object")
		{
			logic->key_objMove = stringToUniKey(value);
		}
		

		else if(varName == "modemove")
		{
			logic->key_modeMove = stringToUniKey(value);
		}
		else if(varName == "moderotate")
		{
			logic->key_modeRotate = stringToUniKey(value);
		}
		else if(varName == "modescale")
		{
			logic->key_modeScale = stringToUniKey(value);
		}
		else if(varName == "viewrotate")
		{
			logic->key_viewRotate = stringToUniKey(value);
		}
		else if(varName == "viewmove")
		{
			logic->key_viewMove = stringToUniKey(value);
		}
		else if(varName == "showlines")
		{
			logic->key_showLines = stringToUniKey(value);
		}
		else if(varName == "screenshot")
		{
			logic->key_screenShot = stringToUniKey(value);
		}
		else if(varName == "delete")
		{
			logic->key_delete = stringToUniKey(value);
		}
		else if(varName == "showdetails")
		{
			logic->key_showDetails = stringToUniKey(value);
		}
		else if(varName == "viewrotatemove")
		{
			logic->key_viewRotateMove = stringToUniKey(value);
		}
		else if(varName == "savemap")
		{
			logic->key_savemap = stringToUniKey(value);
		}
        keys = keys->NextSiblingElement("key");
    }
    
}

void EditorApp::loadConfig()
{
    Ogre::ConfigFile cf;
	cf.load("editor.cfg");
	
	
	// Go through all sections & settings in the file
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();


	Ogre::String secName, varName, value;
	//CEGUI::WindowManager *wmgr = CEGUI::WindowManager::getSingletonPtr();
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			varName = i->first;
			value = i->second;	
			if(secName == "settings")
			{
				if(varName == "gridsnap")
				{
					gridSnap = Ogre::StringConverter::parseReal(value);
				}
				else if(varName == "anglesnap")
				{
					angleSnap = Ogre::StringConverter::parseReal(value);
				}
				else if(varName == "scalesnap")
				{
					scaleSnap = Ogre::StringConverter::parseReal(value);
				}
				/*else if(varName == "loadterraincollision")
				{
					loadTerrainCollision = Ogre::StringConverter::parseBool(value);
				}*/
				else if(varName == "showfog")
				{
					showFog = Ogre::StringConverter::parseBool(value);
					//static_cast<CEGUI::Checkbox*>(wmgr->getWindow("ShowFog"))->setSelected(showFog);
				}
				else if(varName == "usegridsnap")
				{
					snapOnGrid = Ogre::StringConverter::parseBool(value);
					//static_cast<CEGUI::Checkbox*>(wmgr->getWindow("GridSnap"))->setSelected(snapOnGrid);
					
				}
				else if(varName == "useanglesnap")
				{
					snapOnAngle = Ogre::StringConverter::parseBool(value);
					//static_cast<CEGUI::Checkbox*>(wmgr->getWindow("AngleSnap"))->setSelected(snapOnAngle);
				}
				else if(varName == "usescalesnap")
				{
					snapOnScale = Ogre::StringConverter::parseBool(value);
					//static_cast<CEGUI::Checkbox*>(wmgr->getWindow("ScaleSnap"))->setSelected(snapOnScale);
				}
				else if(varName == "move_sensitivity")
				{
					qtApp->getGameLogic()->mMove = Ogre::StringConverter::parseReal(value);					
				}
				else if(varName == "rotate_sensitivity")
				{
					qtApp->getGameLogic()->mRotate = Ogre::StringConverter::parseReal(value);					
				}
				else if(varName == "scroll_sensitivity")
				{
					qtApp->getGameLogic()->mScroll = Ogre::StringConverter::parseReal(value);					
				}
				else if(varName == "transformaxes_scale")
				{
					axesScaleFactor = Ogre::StringConverter::parseReal(value);					
				}
				else if(varName == "windowname")
				{
					windowName = value;	
					setWindowTitle(windowName);
					//mWindow->getName
				}
				else if(varName == "cam_startposition")
				{
					camStartPos = Ogre::StringConverter::parseVector3(value);					
				}
				else if(varName == "showDebugWnd")
				{
					showDebugWnd = Ogre::StringConverter::parseBool(value);	
					/*CEGUI::Window *debugwnd = wmgr->getWindow("debugWnd");
					debugwnd->setVisible(showDebugWnd);
					debugwnd->setEnabled(showDebugWnd);*/

				}
				
				/*else if(varName == "cam_startorientation")
				{
					camStartOrient = Ogre::StringConverter::parseQuaternion(value);					
				}*/
			}
			else if(secName == "keys")
			{
				if(varName == "clone")
				{
					qtApp->getGameLogic()->key_clone = stringToUniKey(value);
				}
				else if(varName == "move_object")
				{
					qtApp->getGameLogic()->key_objMove = stringToUniKey(value);
				}
				

				else if(varName == "modeMove")
				{
					qtApp->getGameLogic()->key_modeMove = stringToUniKey(value);
				}
				else if(varName == "modeRotate")
				{
					qtApp->getGameLogic()->key_modeRotate = stringToUniKey(value);
				}
				else if(varName == "modeScale")
				{
					qtApp->getGameLogic()->key_modeScale = stringToUniKey(value);
				}
				else if(varName == "viewRotate")
				{
					qtApp->getGameLogic()->key_viewRotate = stringToUniKey(value);
				}
				else if(varName == "viewMove")
				{
					qtApp->getGameLogic()->key_viewMove = stringToUniKey(value);
				}
				else if(varName == "showLines")
				{
					qtApp->getGameLogic()->key_showLines = stringToUniKey(value);
				}
				else if(varName == "screenShot")
				{
					qtApp->getGameLogic()->key_screenShot = stringToUniKey(value);
				}
				else if(varName == "delete")
				{
					qtApp->getGameLogic()->key_delete = stringToUniKey(value);
				}
				else if(varName == "showDetails")
				{
					qtApp->getGameLogic()->key_showDetails = stringToUniKey(value);
				}
				else if(varName == "viewRotateMove")
				{
					qtApp->getGameLogic()->key_viewRotateMove = stringToUniKey(value);
				}
				else if(varName == "savemap")
				{
					qtApp->getGameLogic()->key_savemap = stringToUniKey(value);
				}
                else if(varName == "viewtop")
				{
					qtApp->getGameLogic()->key_viewtop = stringToUniKey(value);
				}
			}
		}
	}
}

void EditorApp::saveConfig()
{
    
    TiXmlDocument* doc = loadConfigXml();
    //geändert werden müssen RenderSystem, settings und keys, alles im <editor>

    OgreXmlElement *editorElem = static_cast<OgreXmlElement *>(doc->FirstChildElement("editor"));
	if(!editorElem)
	{
		editorElem = new OgreXmlElement("editor");
		doc->LinkEndChild(editorElem);
	}
    editorElem->Clear();

    OgreXmlElement *rendererElem = new OgreXmlElement("Renderer");
    rendererElem->SetAttribute("selected",mRoot->getRenderSystem()->getName());
    editorElem->LinkEndChild(rendererElem);
	renderSettingsToXml(rendererElem);

    OgreXmlElement *settingsElem = new OgreXmlElement("settings");
    editorElem->LinkEndChild(settingsElem);
        

    

    OgreXmlElement *option = new OgreXmlElement("option");
    settingsElem->LinkEndChild(option);
    option->SetAttribute("name","gridsnap");
    option->SetOgreAttribute("value",gridSnap);


    option = new OgreXmlElement("option");
    settingsElem->LinkEndChild(option);
    option->SetAttribute("name","anglesnap");
    option->SetOgreAttribute("value",angleSnap);

    option = new OgreXmlElement("option");
    settingsElem->LinkEndChild(option);
    option->SetAttribute("name","scalesnap");
    option->SetOgreAttribute("value",scaleSnap);


    option = new OgreXmlElement("option");
    settingsElem->LinkEndChild(option);
    option->SetAttribute("name","showfog");
    option->SetOgreAttribute("value",showFog);


    option = new OgreXmlElement("option");
    settingsElem->LinkEndChild(option);
    option->SetAttribute("name","usegridsnap");
    option->SetOgreAttribute("value",snapOnGrid);

    option = new OgreXmlElement("option");
    settingsElem->LinkEndChild(option);
    option->SetAttribute("name","useanglesnap");
    option->SetOgreAttribute("value",snapOnAngle);

    option = new OgreXmlElement("option");
    settingsElem->LinkEndChild(option);
    option->SetAttribute("name","usescalesnap");
    option->SetOgreAttribute("value",snapOnScale);

    option = new OgreXmlElement("option");
    settingsElem->LinkEndChild(option);
    option->SetAttribute("name","move_sensitivity");
    option->SetOgreAttribute("value",qtApp->getGameLogic()->mMove);

    option = new OgreXmlElement("option");
    settingsElem->LinkEndChild(option);
    option->SetAttribute("name","rotate_sensitivity");
    option->SetOgreAttribute("value",qtApp->getGameLogic()->mRotate);

    option = new OgreXmlElement("option");
    settingsElem->LinkEndChild(option);
    option->SetAttribute("name","scroll_sensitivity");
    option->SetOgreAttribute("value",qtApp->getGameLogic()->mScroll);

    option = new OgreXmlElement("option");
    settingsElem->LinkEndChild(option);
    option->SetAttribute("name","transformaxes_scale");
    option->SetOgreAttribute("value",axesScaleFactor);

    option = new OgreXmlElement("option");
    settingsElem->LinkEndChild(option);
    option->SetAttribute("name","windowname");
    option->SetOgreAttribute("value",windowName);

    option = new OgreXmlElement("option");
    settingsElem->LinkEndChild(option);
    option->SetAttribute("name","cam_startposition");
    option->SetOgreAttribute("value",camStartPos);

    option = new OgreXmlElement("option");
    settingsElem->LinkEndChild(option);
    option->SetAttribute("name","showDebugWnd");
    option->SetOgreAttribute("value",showDebugWnd);


    //keys
    OgreXmlElement *keyElem = new OgreXmlElement("keys");
    editorElem->LinkEndChild(keyElem);
        
    OgreXmlElement *key = new OgreXmlElement("key");
    keyElem->LinkEndChild(key);
    key->SetAttribute("name","clone");
    key->SetAttribute("value",uniKeyToString(qtApp->getGameLogic()->key_clone));
    	
    key = new OgreXmlElement("key");
    keyElem->LinkEndChild(key);
    key->SetAttribute("name","move_object");
    key->SetAttribute("value",uniKeyToString(qtApp->getGameLogic()->key_objMove));


    	
    key = new OgreXmlElement("key");
    keyElem->LinkEndChild(key);
    key->SetAttribute("name","modeMove");
    key->SetAttribute("value",uniKeyToString(qtApp->getGameLogic()->key_modeMove));
    	
    key = new OgreXmlElement("key");
    keyElem->LinkEndChild(key);
    key->SetAttribute("name","modeRotate");
    key->SetAttribute("value",uniKeyToString(qtApp->getGameLogic()->key_modeRotate));
    	
    key = new OgreXmlElement("key");
    keyElem->LinkEndChild(key);
    key->SetAttribute("name","modeScale");
    key->SetAttribute("value",uniKeyToString(qtApp->getGameLogic()->key_modeScale));

    	
    key = new OgreXmlElement("key");
    keyElem->LinkEndChild(key);
    key->SetAttribute("name","viewRotate");
    key->SetAttribute("value",uniKeyToString(qtApp->getGameLogic()->key_viewRotate));
    	
    key = new OgreXmlElement("key");
    keyElem->LinkEndChild(key);
    key->SetAttribute("name","viewMove");
    key->SetAttribute("value",uniKeyToString(qtApp->getGameLogic()->key_viewMove));
    	
    key = new OgreXmlElement("key");
    keyElem->LinkEndChild(key);
    key->SetAttribute("name","showLines");
    key->SetAttribute("value",uniKeyToString(qtApp->getGameLogic()->key_showLines));
    	
    key = new OgreXmlElement("key");
    keyElem->LinkEndChild(key);
    key->SetAttribute("name","screenShot");
    key->SetAttribute("value",uniKeyToString(qtApp->getGameLogic()->key_screenShot));
    	
    key = new OgreXmlElement("key");
    keyElem->LinkEndChild(key);
    key->SetAttribute("name","delete");
    key->SetAttribute("value",uniKeyToString(qtApp->getGameLogic()->key_delete));
    	
    key = new OgreXmlElement("key");
    keyElem->LinkEndChild(key);
    key->SetAttribute("name","showDetails");
    key->SetAttribute("value",uniKeyToString(qtApp->getGameLogic()->key_showDetails));
    	
    key = new OgreXmlElement("key");
    keyElem->LinkEndChild(key);
    key->SetAttribute("name","viewRotateMove");
    key->SetAttribute("value",uniKeyToString(qtApp->getGameLogic()->key_viewRotateMove));
    	
    key = new OgreXmlElement("key");
    keyElem->LinkEndChild(key);
    key->SetAttribute("name","savemap");
    key->SetAttribute("value",uniKeyToString(qtApp->getGameLogic()->key_savemap));

    key = new OgreXmlElement("key");
    keyElem->LinkEndChild(key);
    key->SetAttribute("name","viewtop");
    key->SetAttribute("value",uniKeyToString(qtApp->getGameLogic()->key_viewtop));
    saveConfigXml(doc);
	//std::ofstream of("editor.cfg");

 //   if (!of)
	//	OGRE_EXCEPT(Ogre::Exception::ERR_CANNOT_WRITE_TO_FILE, "Cannot create settings file.",
 //       "EditorApp::saveConfig");

	//of << "[settings]" << std::endl;
	//of << "gridsnap=" << str(gridSnap) << std::endl;
	//of << "anglesnap=" << str(angleSnap) << std::endl;
	//of << "scalesnap=" << str(scaleSnap) << std::endl;
	////of << "loadterraincollision=" << str(loadTerrainCollision) << std::endl;
	//of << "showfog=" << str(showFog) << std::endl;

	//of << "usegridsnap=" << str(snapOnGrid) << std::endl;
	//of << "useanglesnap=" << str(snapOnAngle) << std::endl;
	//of << "usescalesnap=" << str(snapOnScale) << std::endl;
	//of << "move_sensitivity=" << str(mFrameListener->mMove) << std::endl;
	//of << "rotate_sensitivity=" << str(mFrameListener->mRotate) << std::endl;
	//of << "scroll_sensitivity=" << str(mFrameListener->mScroll) << std::endl;
	//of << "transformaxes_scale=" << str(axesScaleFactor) << std::endl;
	//of << "windowname=" << windowName << std::endl;
	//of << "cam_startposition=" << str(camStartPos) << std::endl;
	//of << "showDebugWnd=" << str(showDebugWnd) << std::endl;
	//
	////of << "cam_startorientation=" << str(camStartOrient) << std::endl;

	//of << std::endl;
	//of << "[keys]" << std::endl;

	//of << "clone=" << uniKeyToString(mFrameListener->key_clone) << std::endl;
	//of << "move_object=" << uniKeyToString(mFrameListener->key_objMove) <<std::endl;


	//of << "modeMove=" << uniKeyToString(mFrameListener->key_modeMove) <<std::endl;
	//of << "modeRotate=" << uniKeyToString(mFrameListener->key_modeRotate) <<std::endl;
	//of << "modeScale=" << uniKeyToString(mFrameListener->key_modeScale) <<std::endl;

	//of << "viewRotate=" << uniKeyToString(mFrameListener->key_viewRotate) <<std::endl;
	//of << "viewMove=" << uniKeyToString(mFrameListener->key_viewMove) <<std::endl;
	//of << "showLines=" << uniKeyToString(mFrameListener->key_showLines) <<std::endl;
	//of << "screenShot=" << uniKeyToString(mFrameListener->key_screenShot) <<std::endl;
	//of << "delete=" << uniKeyToString(mFrameListener->key_delete) <<std::endl;
	//of << "showDetails=" << uniKeyToString(mFrameListener->key_showDetails) <<std::endl;
	//of << "viewRotateMove=" << uniKeyToString(mFrameListener->key_viewRotateMove) <<std::endl;
	//of << "savemap=" << uniKeyToString(mFrameListener->key_savemap) <<std::endl;
	//

	//

	//
	//


 //   /*if (mActiveRenderer)
 //   {
 //       of << "Render System=" << mActiveRenderer->getName() << std::endl;
 //   }
 //   else
 //   {
 //       of << "Render System=" << std::endl;
 //   }

 //   for (RenderSystemList::const_iterator pRend = getAvailableRenderers()->begin(); pRend != getAvailableRenderers()->end(); ++pRend)
 //   {
 //       RenderSystem* rs = *pRend;
 //       of << std::endl;
 //       of << "[" << rs->getName() << "]" << std::endl;
 //       const ConfigOptionMap& opts = rs->getConfigOptions();
 //       for (ConfigOptionMap::const_iterator pOpt = opts.begin(); pOpt != opts.end(); ++pOpt)
 //       {
	//		of << pOpt->first << "=" << pOpt->second.currentValue << std::endl;
 //       }
 //   }*/

 //   of.close();
}


void EditorApp::saveMapImageFinish()
{
	Ogre::Camera *cam = currentLevel->getSceneManager()->getCamera("xcreenshot");
	mWindow->writeContentsToFile(currentLevel->getFileName()+"_map.png");
	Ogre::Viewport* vp = mWindow->getViewport(0);
//	mGUIRenderer->setTargetSceneManager(currentLevel->getSceneManager());
	vp->setCamera(currentLevel->getMainCam());
	currentLevel->getSceneManager()->destroyCamera(cam);
	savingMapImageState = 0;
}

void EditorApp::showObjectFromTop(GameObject *obj)
{
	getQtOgreGameLogic()->showObjectFromTop(obj);
}

void EditorApp::saveMapImageInit()
{
	savingMapImageState = 5;
	//********TEST
	Ogre::Camera *cam = currentLevel->getSceneManager()->createCamera("xcreenshot");
	cam->setProjectionType(Ogre::PT_ORTHOGRAPHIC); 
	Ogre::AxisAlignedBox box = getSceneNodeBoundingBox(currentLevel->getSceneManager()->getRootSceneNode());
	Ogre::Vector3 test = box.getCenter();
	Ogre::Real dist = box.getHalfSize().x;

	Ogre::Real height = dist/Ogre::Math::Tan(Ogre::Degree(22.5))+1;

	test.y=box.getMaximum().y+height;
	
	
	//cam->getParentSceneNode()->getParentSceneNode()->setPosition(test);
	cam->setPosition(test);
	cam->pitch(Ogre::Degree(-90));
	//cam->setPosition(0,box.getMaximum().y+10,0);
	//cam->lookAt(box.getCenter());
	
    cam->setNearClipDistance(height); 
	//cam->setFarClipDistance((box.getMaximum().y-box.getMinimum().y)+10);
	Ogre::Viewport* vp = mWindow->getViewport(0);
	vp->setCamera(cam);
    
    Ogre::Real actualW = vp->getActualWidth();
    Ogre::Real actualH = vp->getActualHeight();
	cam->setAspectRatio(actualW / actualH);



//	mGUIRenderer->setTargetSceneManager(mDefaultSceneMgr);

		
	

}
//
//void EditorApp::updateSelBorder(CEGUI::Point secondPoint)
//{
//
//	//Ogre::Real width = secondPoint.d_x-selBorderFirst.d_x.d_offset;
//	//Ogre::Real height= secondPoint.d_y-selBorderFirst.d_y.d_offset;
//	//CEGUI::UVector2 pos = selBorderFirst;
//	//if(width < 0)
//	//{
//	//	width *= -1;
//	//	pos.d_x.d_offset -= width;
//	//}
//	//if(height < 0)
//	//{
//	//	height *= -1;
//	//	pos.d_y.d_offset -= height;
//	//}
//	///*CEGUI::UVector2 uPt;
//	//uPt.d_x.d_offset = secondPoint.d_x;
//	//uPt.d_y.d_offset = secondPoint.d_y;
//	//CEGUI::URect uR(selBorderFirst,uPt);*/
//	//selectionBorder->setPosition(pos);
//	//selectionBorder->setHeight(CEGUI::UDim(0,height));
//	//selectionBorder->setWidth(CEGUI::UDim(0,width));
//}
//
////"glätten" anhaken

//unsigned int EditorApp::getCurrentTexture()
//{
//	if(tEditMode == EditorApp::teHoles && currentLevel->hasHoleMap())
//		return 0;
//	else
//		return currentTexture;
//}


void EditorApp::frameEnded(Ogre::Real time)
{
    Ogre::SceneNode *mCamNode = currentLevel->getSceneManager()->getSceneNode("CamNode");
    Ogre::Vector3 pos = mCamNode->getPosition();
    Ogre::Quaternion ornt = mCamNode->getOrientation();
    soundMgr->getListener()->setPosition(pos);
    soundMgr->getListener()->setOrientation(ornt);

    if(savingMapImageState != 0)
	{
		savingMapImageState--;
		if(savingMapImageState <= 0)
			saveMapImageFinish();
	}
    /*if(lastLB)
    {
        timeSinceLastLBDown += time;
        if(timeSinceLastLBDown > 1)
        {
            listboxTypeText = "";
            timeSinceLastLBDown = 0;
            lastLB = NULL;
        }
    }*/
   
}

void EditorApp::setupFromQtApp()
{
	mRoot = Ogre::Root::getSingletonPtr();
	setupPlugins(configXml->FirstChildElement("Plugins"));
}

int EditorApp::go_qt()
{
	configXml = loadConfigXml();
	
    initializeQt();//das da erstellt "bloß" das qtApp. und damit auch Ogre::Root
	mRoot = Ogre::Root::getSingletonPtr();
    //setup();
	//SETUP BEGIN
	
    
    ////game und editor können damit verschiedene renderer settings haben, zB für mehr Leistung beim Editieren
    ////zugunsten der Grafik
    setupRenderSystem(configXml->FirstChildElement("editor")->FirstChildElement("Renderer"));
    //NOT YET. this has to be stolen form QtEditorApp!

    setupResources(configXml->FirstChildElement("Resources"));

	
    
	//HIER muss das root initialisiert werden!
	qtApp->initialise();
	

    //bool carryOn = configure();
    //if (!carryOn) return false;

	createResourceListener();
	// Load resources
	loadResources();

    loadManualLoD();

	loadGameData();


	createDefaultSceneMgr();




	createGUI();
	

	createTerrainBrushes();


    setupConfig(configXml->FirstChildElement("editor"));
	//loadConfig();

    initSound();

	qtApp->postInit();
	getQtOgreGameLogic()->createDecal();

	//now creating the terrain globals
	initTerrainGlobals();

	loadLevel("");
    delete configXml;
    
	//SETUP END
	
    return qtApp->exec(true);  

}


void EditorApp::setup_qt()
{
    configure();
}