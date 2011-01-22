#include "unikey.h"
#include <OIS/OIS.h>
#include "functions.h"
#ifndef __editor
#include "GameSettings.h"
#endif
//helper function
std::map<OIS::KeyCode,Ogre::String> UniKey::initKeyMap()
{
	std::map<OIS::KeyCode,Ogre::String> res;

	res[OIS::KC_UNASSIGNED] = Ogre::String("KC_UNASSIGNED");
	res[OIS::KC_ESCAPE] = "KC_ESCAPE";
	res[OIS::KC_1] = "KC_1";
	res[OIS::KC_2] = "KC_2";
	res[OIS::KC_3] = "KC_3";
	res[OIS::KC_4] = "KC_4";
	res[OIS::KC_5] = "KC_5";
	res[OIS::KC_6] = "KC_6";
	res[OIS::KC_7] = "KC_7";
	res[OIS::KC_8] = "KC_8";
	res[OIS::KC_9] = "KC_9";
	res[OIS::KC_0] = "KC_0";
	res[OIS::KC_MINUS] = "KC_MINUS";    // - on main keyboard
	res[OIS::KC_EQUALS] = "KC_EQUALS";
	res[OIS::KC_BACK] = "KC_BACK";    // backspace
	res[OIS::KC_TAB] = "KC_TAB";
	res[OIS::KC_Q] = "KC_Q";
	res[OIS::KC_W] = "KC_W";
	res[OIS::KC_E] = "KC_E";
	res[OIS::KC_R] = "KC_R";
	res[OIS::KC_T] = "KC_T";
	res[OIS::KC_Y] = "KC_Y";
	res[OIS::KC_U] = "KC_U";
	res[OIS::KC_I] = "KC_I";
	res[OIS::KC_O] = "KC_O";
	res[OIS::KC_P] = "KC_P";
	res[OIS::KC_LBRACKET] = "KC_LBRACKET";
	res[OIS::KC_RBRACKET] = "KC_RBRACKET";
	res[OIS::KC_RETURN] = "KC_RETURN";    // Enter on main keyboard
	res[OIS::KC_LCONTROL] = "KC_LCONTROL";
	res[OIS::KC_A] = "KC_A";
	res[OIS::KC_S] = "KC_S";
	res[OIS::KC_D] = "KC_D";
	res[OIS::KC_F] = "KC_F";
	res[OIS::KC_G] = "KC_G";
	res[OIS::KC_H] = "KC_H";
	res[OIS::KC_J] = "KC_J";
	res[OIS::KC_K] = "KC_K";
	res[OIS::KC_L] = "KC_L";
	res[OIS::KC_SEMICOLON] = "KC_SEMICOLON";
	res[OIS::KC_APOSTROPHE] = "KC_APOSTROPHE";
	res[OIS::KC_GRAVE] = "KC_GRAVE";    // accent
	res[OIS::KC_LSHIFT] = "KC_LSHIFT";
	res[OIS::KC_BACKSLASH] = "KC_BACKSLASH";
	res[OIS::KC_Z] = "KC_Z";
	res[OIS::KC_X] = "KC_X";
	res[OIS::KC_C] = "KC_C";
	res[OIS::KC_V] = "KC_V";
	res[OIS::KC_B] = "KC_B";
	res[OIS::KC_N] = "KC_N";
	res[OIS::KC_M] = "KC_M";
	res[OIS::KC_COMMA] = "KC_COMMA";
	res[OIS::KC_PERIOD] = "KC_PERIOD";    // . on main keyboard
	res[OIS::KC_SLASH] = "KC_SLASH";    // / on main keyboard
	res[OIS::KC_RSHIFT] = "KC_RSHIFT";
	res[OIS::KC_MULTIPLY] = "KC_MULTIPLY";    // * on numeric keypad
	res[OIS::KC_LMENU] = "KC_LMENU";    // left Alt
	res[OIS::KC_SPACE] = "KC_SPACE";
	res[OIS::KC_CAPITAL] = "KC_CAPITAL";
	res[OIS::KC_F1] = "KC_F1";
	res[OIS::KC_F2] = "KC_F2";
	res[OIS::KC_F3] = "KC_F3";
	res[OIS::KC_F4] = "KC_F4";
	res[OIS::KC_F5] = "KC_F5";
	res[OIS::KC_F6] = "KC_F6";
	res[OIS::KC_F7] = "KC_F7";
	res[OIS::KC_F8] = "KC_F8";
	res[OIS::KC_F9] = "KC_F9";
	res[OIS::KC_F10] = "KC_F10";
	res[OIS::KC_NUMLOCK] = "KC_NUMLOCK";
	res[OIS::KC_SCROLL] = "KC_SCROLL";    // Scroll Lock
	res[OIS::KC_NUMPAD7] = "KC_NUMPAD7";
	res[OIS::KC_NUMPAD8] = "KC_NUMPAD8";
	res[OIS::KC_NUMPAD9] = "KC_NUMPAD9";
	res[OIS::KC_SUBTRACT] = "KC_SUBTRACT";    // - on numeric keypad
	res[OIS::KC_NUMPAD4] = "KC_NUMPAD4";
	res[OIS::KC_NUMPAD5] = "KC_NUMPAD5";
	res[OIS::KC_NUMPAD6] = "KC_NUMPAD6";
	res[OIS::KC_ADD] = "KC_ADD";    // + on numeric keypad
	res[OIS::KC_NUMPAD1] = "KC_NUMPAD1";
	res[OIS::KC_NUMPAD2] = "KC_NUMPAD2";
	res[OIS::KC_NUMPAD3] = "KC_NUMPAD3";
	res[OIS::KC_NUMPAD0] = "KC_NUMPAD0";
	res[OIS::KC_DECIMAL] = "KC_DECIMAL";    // . on numeric keypad
	res[OIS::KC_OEM_102] = "KC_OEM_102";    // < > | on UK/Germany keyboards
	res[OIS::KC_F11] = "KC_F11";
	res[OIS::KC_F12] = "KC_F12";
	res[OIS::KC_F13] = "KC_F13";    //                     (NEC PC98)
	res[OIS::KC_F14] = "KC_F14";    //                     (NEC PC98)
	res[OIS::KC_F15] = "KC_F15";    //                     (NEC PC98)
	res[OIS::KC_KANA] = "KC_KANA";    // (Japanese keyboard)
	res[OIS::KC_ABNT_C1] = "KC_ABNT";    // / ? on Portugese (Brazilian) keyboards
	res[OIS::KC_CONVERT] = "KC_CONVERT";    // (Japanese keyboard)
	res[OIS::KC_NOCONVERT] = "KC_NOCONVERT";    // (Japanese keyboard)
	res[OIS::KC_YEN] = "KC_YEN";    // (Japanese keyboard)
	res[OIS::KC_ABNT_C2] = "KC_ABNT_C2";    // Numpad . on Portugese (Brazilian) keyboards
	res[OIS::KC_NUMPADEQUALS] = "KC_NUMPADEQUALS";    // = on numeric keypad (NEC PC98)
	res[OIS::KC_PREVTRACK] = "KC_PREVTRACK";    // Previous Track (KC_CIRCUMFLEX on Japanese keyboard)
	res[OIS::KC_AT] = "KC_AT";    //                     (NEC PC98)
	res[OIS::KC_COLON] = "KC_COLON";    //                     (NEC PC98)
	res[OIS::KC_UNDERLINE] = "KC_UNDERLINE";    //                     (NEC PC98)
	res[OIS::KC_KANJI] = "KC_KANJI";    // (Japanese keyboard)
	res[OIS::KC_STOP] = "KC_STOP";    //                     (NEC PC98)
	res[OIS::KC_AX] = "KC_AX";    //                     (Japan AX)
	res[OIS::KC_UNLABELED] = "KC_UNLABELED";    //                        (J3100)
	res[OIS::KC_NEXTTRACK] = "KC_NEXTTRACK";    // Next Track
	res[OIS::KC_NUMPADENTER] = "KC_NUMPADENTER";    // Enter on numeric keypad
	res[OIS::KC_RCONTROL] = "KC_RCONTROL";
	res[OIS::KC_MUTE] = "KC_MUTE";    // Mute
	res[OIS::KC_CALCULATOR] = "KC_CALCULATOR";    // Calculator
	res[OIS::KC_PLAYPAUSE] = "KC_PLAYPAUSE";    // Play / Pause
	res[OIS::KC_MEDIASTOP] = "KC_MEDIASTOP";    // Media Stop
	res[OIS::KC_VOLUMEDOWN] = "KC_VOLUMEDOWN";    // Volume -
	res[OIS::KC_VOLUMEUP] = "KC_VOLUMEUP";    // Volume +
	res[OIS::KC_WEBHOME] = "KC_WEBHOME";    // Web home
	res[OIS::KC_NUMPADCOMMA] = "KC_NUMPADCOMMA"; //on numeric keypad (NEC PC98)
	res[OIS::KC_DIVIDE] = "KC_DIVIDE";    // / on numeric keypad
	res[OIS::KC_SYSRQ] = "KC_SYSRQ";
	res[OIS::KC_RMENU] = "KC_RMENU";    // right Alt
	res[OIS::KC_PAUSE] = "KC_PAUSE";    // Pause
	res[OIS::KC_HOME] = "KC_HOME";    // Home on arrow keypad
	res[OIS::KC_UP] = "KC_UP";    // UpArrow on arrow keypad
	res[OIS::KC_PGUP] = "KC_PGUP";    // PgUp on arrow keypad
	res[OIS::KC_LEFT] = "KC_LEFT";    // LeftArrow on arrow keypad
	res[OIS::KC_RIGHT] = "KC_RIGHT";    // RightArrow on arrow keypad
	res[OIS::KC_END] = "KC_END";    // End on arrow keypad
	res[OIS::KC_DOWN] = "KC_DOWN";    // DownArrow on arrow keypad
	res[OIS::KC_PGDOWN] = "KC_PGDOWN";    // PgDn on arrow keypad
	res[OIS::KC_INSERT] = "KC_INSERT";    // Insert on arrow keypad
	res[OIS::KC_DELETE] = "KC_DELETE";    // Delete on arrow keypad
	res[OIS::KC_LWIN] = "KC_LWIN";    // Left Windows key
	res[OIS::KC_RWIN] = "KC_RWIN";    // Right Windows key
	res[OIS::KC_APPS] = "KC_APPS";    // AppMenu key
	res[OIS::KC_POWER] = "KC_POWER";    // System Power
	res[OIS::KC_SLEEP] = "KC_SLEEP";    // System Sleep
	res[OIS::KC_WAKE] = "KC_WAKE";    // System Wake
	res[OIS::KC_WEBSEARCH] = "KC_WEBSEARCH";    // Web Search
	res[OIS::KC_WEBFAVORITES] = "KC_WEBFAVORITES";    // Web Favorites
	res[OIS::KC_WEBREFRESH] = "KC_WEBREFRESH";    // Web Refresh
	res[OIS::KC_WEBSTOP] = "KC_WEBSTOP";    // Web Stop
	res[OIS::KC_WEBFORWARD] = "KC_WEBFORWARD";    // Web Forward
	res[OIS::KC_WEBBACK] = "KC_WEBBACK";    // Web Back
	res[OIS::KC_MYCOMPUTER] = "KC_MYCOMPUTER";    // My Computer
	res[OIS::KC_MAIL] = "KC_MAIL";    // Mail
	res[OIS::KC_MEDIASELECT] = "KC_MEDIASELECT";     // Media Select
	return res;
}

std::map<OIS::KeyCode,Ogre::String> UniKey::keyMap = UniKey::initKeyMap();

//#ifndef __editor
//Ogre::String UniKey::keyNameToString(unsigned int kn)
//{
//	switch(kn)
//	{
//		//case keyNone: return "keyNone";
//		case keyMoveForward:	return "Move Forwards";
//		case keyMoveBack:		return "Move Backwards";
//		case keyMoveRight:		return "Move Right";
//		case keyMoveLeft:		return "Move Left";
//		case keyJump:			return "Jump";
//		case keyAttack:			return "Attack";
//		case keySpellcast:		return "Cast Spell";
//		case keyRun:			return "Run";
//		case keyAction:			return "Action";
//		case keyInventory:		return "Inventory";
//		case keyInvTakeOne:		return "Take One";
//		case keyInvTakeAll:		return "Take All";
//		case keyInvUse:			return "Use";
//		case keySpellbook:		return "Open Spellbook";
//		case keyScreenshot:		return "Make Screenshot";
//		case keyMainmenu:		return "Main Menu";
//		case keyLookUp:			return "Look Up";
//		case keyLookDown:		return "Look Down";
//		case keyLookLeft:		return "Look Left";
//		case keyLookRight:		return "Look Right";
//		case keyToggleMouselooking: return "Toggle Mouselooking";		
//	
//		//case keyDebug3rdp: return "keyDebug3rdp";
//		//case keyDebugshowlines: return "keyDebugshowlines";
//		
//		case keyDragItem:		return "Drag Item";
//	};	
//	return "";//"Key "+ogre_str(kn);
//}
//#endif

#ifndef __editor
UniKey::UniKey(CEGUI::MouseButton ceguiMouse, CEGUI::uint ceguiMod)
{
	isKeyboard = false;
	mouseID = convertCeguiMouseButtonToOis(ceguiMouse);
	modifier = 0;

	if(ceguiMod & CEGUI::Control)
	{	
		setModifier(OIS::Keyboard::Ctrl);
	}
	else if(ceguiMod & CEGUI::Alt)
	{
		setModifier(OIS::Keyboard::Alt);
	}
	else if(ceguiMod & CEGUI::Shift)
	{
		setModifier(OIS::Keyboard::Shift);
	}	
}
#endif

Ogre::String UniKey::toString()
{
	Ogre::String res = "";
    if(hasModifier())
	{
		switch(modifier)
		{
		case OIS::Keyboard::Ctrl:
			res = "Ctrl+";
			break;
		case OIS::Keyboard::Shift:
			res = "Shift+";
			break;
		case OIS::Keyboard::Alt:
			res = "Alt+";
			break;
		}
	}
	if(isKeyboard)
	{
		res += keyMap[keyboardID];
	}
	else
	{
		switch(mouseID)
		{
		case MB_Left:
			res += "MB_Left";
			break;
		case MB_Right:
			res += "MB_Right";
			break;
		case MB_Middle:
			res += "MB_Middle";
			break;
		case MB_Button3:
			res += "MB_Button3";
			break;
		case MB_Button4:
			res += "MB_Button4";
			break;
		case MB_Button5:
			res += "MB_Button5";
			break;
		case MB_Button6:
			res += "MB_Button6";
			break;
		case MB_Button7:
			res += "MB_Button7";
			break;
		}
	}
	return res;
}

bool UniKey::operator == (UniKey k)
{	
	if(isKeyboard != k.isKeyboard || modifier != k.modifier) //wenn verschiedene inputgeräte oder modifier, dann ungleich
		return false;
    
       
	if(isKeyboard)
	{
		return (keyboardID == k.keyboardID);
	}
	else
	{
		return (mouseID == k.mouseID);
	}
}


void UniKey::loadFromString(Ogre::String str)
{	
	modifier = 0;
	if(Ogre::StringUtil::startsWith(str,"ctrl+"))
	{
		modifier = OIS::Keyboard::Ctrl;
		str = Ogre::StringUtil::split(str,"+",1)[1];
	}
	else if(Ogre::StringUtil::startsWith(str,"alt+"))
	{
		modifier = OIS::Keyboard::Alt;
		str = Ogre::StringUtil::split(str,"+",1)[1];
	}
	else if(Ogre::StringUtil::startsWith(str,"shift+"))
	{
		modifier = OIS::Keyboard::Shift;
		str = Ogre::StringUtil::split(str,"+",1)[1];
	}
	if(Ogre::StringUtil::startsWith(str,"mb"))
	{
		isKeyboard = false;
		if(str == "MB_Left")
			mouseID = MB_Left;
		else if(str == "MB_Right")
			mouseID = MB_Right;
		else if(str == "MB_Middle")
			mouseID = MB_Middle;
		else if(str == "MB_Button3")
			mouseID = MB_Button3;
		else if(str == "MB_Button4")
			mouseID = MB_Button4;
		else if(str == "MB_Button5")
			mouseID = MB_Button5;
		else if(str == "MB_Button6")
			mouseID = MB_Button6;
		else if(str == "MB_Button7")
			mouseID = MB_Button7;
		
		return;
		
	}
	else if(Ogre::StringUtil::startsWith(str,"kc"))
	{
		isKeyboard = true;	
		std::map<OIS::KeyCode,Ogre::String>::iterator itr;
		for(itr = keyMap.begin();itr != keyMap.end();itr++)
		{
			OIS::KeyCode k = itr->first;
			Ogre::String s = itr->second;
			if(s == str)
			{
				keyboardID = k;
				return;
			}
		}
	}
	keyboardID = OIS::KC_UNASSIGNED;
	isKeyboard = true;	
	
	return;
}



