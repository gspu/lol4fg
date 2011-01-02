

//------- BEGIN OGRE & stuff includes ----------
#include <Ogre.h>

#include <OgreFrameListener.h>
//#include <OgreEventListeners.h>
//#include <OgreKeyEvent.h>


#include <OgreTextAreaOverlayElement.h>
#include <OgreFontManager.h>
#include <vector>
#include <OgreNewt.h>
#include "tinyxml.h"
#include "functions.h"

#include "SpellEffect.h"
#include "Alchemy.h"
#include <TypeConverter.h>

using namespace TypeConverter;


////mem probs without this next one
////#include <OgreNoMemoryMacros.h>
//#include <CEGUIImageset.h>
//#include <CEGUISystem.h>
//#include <CEGUILogger.h>
//#include <CEGUISchemeManager.h>
//#include <CEGUIWindowManager.h>
//#include <CEGUIWindow.h>
//#include <RendererModules/Ogre/CEGUIOgreRenderer.h>
//#include <RendererModules/Ogre/CEGUIOgreResourceProvider.h>
//#include <elements/CEGUIPushButton.h>
////regular mem handler
////#include <OgreMemoryMacros.h>
//------- END OGRE & stuff includes ----------

#include "FwDec.h"

#include "GameChar.h"
//#include "global_class.h"
//#include "GameFrameListener.h"

#include "StandardApplication.h"
//#include "contactCallback.h"

#include "Sound.h"
#include "SoundManager.h"
#include "Listener.h"
#include "Buffer.h"
#include "Source.h"
#include "defines.h"
#include "xmlwrapper.h"
using namespace std;
//using namespace Ogre;

//OgreAL workaround class
//class SoundBufferLoader: public OgreAL::Sound
//{
//public:
//	bool loadSoundBuffers()
//	{
//		if(!mBuffersLoaded)
//			return loadBuffers();
//		return false;
//	}
//};
//
//bool soundLoadBuffer(OgreAL::Sound *snd)
//{
//	return static_cast<SoundBufferLoader*>(snd)->loadSoundBuffers();
//}

//workaround class end

template<> StandardApplication* Ogre::Singleton<StandardApplication>::ms_Singleton = 0;

StandardApplication::StandardApplication():
	alchemy(NULL),
	showFog(true),
	currentLevel(NULL),
	oldLevel(NULL),
	mWindow(NULL),
	
	mRoot(0)
{
	soundinc = 0;
	clipboard = "";
	mPluginFileName = "plugins.cfg";
	mConfigFileName = "ogre.cfg";
	mLogFileName = "Ogre.log";
    configFilename = "config.xml";
	soundMgr = NULL;


	levelPath = "./level"; //enthält levels und pages
	collisionPath = "./level/collision"; //enthält kollisionsdateien
	saveGamePath = "./savegame"; //enthält spielstände
	tempPath = "./temp"; //enthält savegame-versionen
	
	//rollofFactor= 1;
	//refDistance = 150;
	//maxDistance = 3400;
	//distModel = SimpleSound::SoundManager::INVERSE_DISTANCE_CLAMPED;

	//gainAll			= 1;
	//gainMusic		= 1;
	//gainSFX			= 1;       //schritte usw
	//gainSpeech		= 1;    //sprache
	//gainSFXPlayer	= 1;

	//mOgreNewtListener = 0;
//	mKeyListener = 0;

//	mEventProcessor = 0;
    mRoot = 0;

//	menuMode = GUI_DISABLE;

	//TASTEN FÜR KEY<-->string umwandlung
	keys[OIS::KC_UNASSIGNED] = "KC_UNASSIGNED";
	keys[OIS::KC_ESCAPE] = "KC_ESCAPE";
	keys[OIS::KC_1] = "KC_1";
	keys[OIS::KC_2] = "KC_2";
	keys[OIS::KC_3] = "KC_3";
	keys[OIS::KC_4] = "KC_4";
	keys[OIS::KC_5] = "KC_5";
	keys[OIS::KC_6] = "KC_6";
	keys[OIS::KC_7] = "KC_7";
	keys[OIS::KC_8] = "KC_8";
	keys[OIS::KC_9] = "KC_9";
	keys[OIS::KC_0] = "KC_0";
	keys[OIS::KC_MINUS] = "KC_MINUS";    // - on main keyboard
	keys[OIS::KC_EQUALS] = "KC_EQUALS";
	keys[OIS::KC_BACK] = "KC_BACK";    // backspace
	keys[OIS::KC_TAB] = "KC_TAB";
	keys[OIS::KC_Q] = "KC_Q";
	keys[OIS::KC_W] = "KC_W";
	keys[OIS::KC_E] = "KC_E";
	keys[OIS::KC_R] = "KC_R";
	keys[OIS::KC_T] = "KC_T";
	keys[OIS::KC_Y] = "KC_Y";
	keys[OIS::KC_U] = "KC_U";
	keys[OIS::KC_I] = "KC_I";
	keys[OIS::KC_O] = "KC_O";
	keys[OIS::KC_P] = "KC_P";
	keys[OIS::KC_LBRACKET] = "KC_LBRACKET";
	keys[OIS::KC_RBRACKET] = "KC_RBRACKET";
	keys[OIS::KC_RETURN] = "KC_RETURN";    // Enter on main keyboard
	keys[OIS::KC_LCONTROL] = "KC_LCONTROL";
	keys[OIS::KC_A] = "KC_A";
	keys[OIS::KC_S] = "KC_S";
	keys[OIS::KC_D] = "KC_D";
	keys[OIS::KC_F] = "KC_F";
	keys[OIS::KC_G] = "KC_G";
	keys[OIS::KC_H] = "KC_H";
	keys[OIS::KC_J] = "KC_J";
	keys[OIS::KC_K] = "KC_K";
	keys[OIS::KC_L] = "KC_L";
	keys[OIS::KC_SEMICOLON] = "KC_SEMICOLON";
	keys[OIS::KC_APOSTROPHE] = "KC_APOSTROPHE";
	keys[OIS::KC_GRAVE] = "KC_GRAVE";    // accent
	keys[OIS::KC_LSHIFT] = "KC_LSHIFT";
	keys[OIS::KC_BACKSLASH] = "KC_BACKSLASH";
	keys[OIS::KC_Z] = "KC_Z";
	keys[OIS::KC_X] = "KC_X";
	keys[OIS::KC_C] = "KC_C";
	keys[OIS::KC_V] = "KC_V";
	keys[OIS::KC_B] = "KC_B";
	keys[OIS::KC_N] = "KC_N";
	keys[OIS::KC_M] = "KC_M";
	keys[OIS::KC_COMMA] = "KC_COMMA";
	keys[OIS::KC_PERIOD] = "KC_PERIOD";    // . on main keyboard
	keys[OIS::KC_SLASH] = "KC_SLASH";    // / on main keyboard
	keys[OIS::KC_RSHIFT] = "KC_RSHIFT";
	keys[OIS::KC_MULTIPLY] = "KC_MULTIPLY";    // * on numeric keypad
	keys[OIS::KC_LMENU] = "KC_LMENU";    // left Alt
	keys[OIS::KC_SPACE] = "KC_SPACE";
	keys[OIS::KC_CAPITAL] = "KC_CAPITAL";
	keys[OIS::KC_F1] = "KC_F1";
	keys[OIS::KC_F2] = "KC_F2";
	keys[OIS::KC_F3] = "KC_F3";
	keys[OIS::KC_F4] = "KC_F4";
	keys[OIS::KC_F5] = "KC_F5";
	keys[OIS::KC_F6] = "KC_F6";
	keys[OIS::KC_F7] = "KC_F7";
	keys[OIS::KC_F8] = "KC_F8";
	keys[OIS::KC_F9] = "KC_F9";
	keys[OIS::KC_F10] = "KC_F10";
	keys[OIS::KC_NUMLOCK] = "KC_NUMLOCK";
	keys[OIS::KC_SCROLL] = "KC_SCROLL";    // Scroll Lock
	keys[OIS::KC_NUMPAD7] = "KC_NUMPAD7";
	keys[OIS::KC_NUMPAD8] = "KC_NUMPAD8";
	keys[OIS::KC_NUMPAD9] = "KC_NUMPAD9";
	keys[OIS::KC_SUBTRACT] = "KC_SUBTRACT";    // - on numeric keypad
	keys[OIS::KC_NUMPAD4] = "KC_NUMPAD4";
	keys[OIS::KC_NUMPAD5] = "KC_NUMPAD5";
	keys[OIS::KC_NUMPAD6] = "KC_NUMPAD6";
	keys[OIS::KC_ADD] = "KC_ADD";    // + on numeric keypad
	keys[OIS::KC_NUMPAD1] = "KC_NUMPAD1";
	keys[OIS::KC_NUMPAD2] = "KC_NUMPAD2";
	keys[OIS::KC_NUMPAD3] = "KC_NUMPAD3";
	keys[OIS::KC_NUMPAD0] = "KC_NUMPAD0";
	keys[OIS::KC_DECIMAL] = "KC_DECIMAL";    // . on numeric keypad
	keys[OIS::KC_OEM_102] = "KC_OEM_102";    // < > | on UK/Germany keyboards
	keys[OIS::KC_F11] = "KC_F11";
	keys[OIS::KC_F12] = "KC_F12";
	keys[OIS::KC_F13] = "KC_F13";    //                     (NEC PC98)
	keys[OIS::KC_F14] = "KC_F14";    //                     (NEC PC98)
	keys[OIS::KC_F15] = "KC_F15";    //                     (NEC PC98)
	keys[OIS::KC_KANA] = "KC_KANA";    // (Japanese keyboard)
	keys[OIS::KC_ABNT_C1] = "KC_ABNT";    // / ? on Portugese (Brazilian) keyboards
	keys[OIS::KC_CONVERT] = "KC_CONVERT";    // (Japanese keyboard)
	keys[OIS::KC_NOCONVERT] = "KC_NOCONVERT";    // (Japanese keyboard)
	keys[OIS::KC_YEN] = "KC_YEN";    // (Japanese keyboard)
	keys[OIS::KC_ABNT_C2] = "KC_ABNT_C2";    // Numpad . on Portugese (Brazilian) keyboards
	keys[OIS::KC_NUMPADEQUALS] = "KC_NUMPADEQUALS";    // = on numeric keypad (NEC PC98)
	keys[OIS::KC_PREVTRACK] = "KC_PREVTRACK";    // Previous Track (KC_CIRCUMFLEX on Japanese keyboard)
	keys[OIS::KC_AT] = "KC_AT";    //                     (NEC PC98)
	keys[OIS::KC_COLON] = "KC_COLON";    //                     (NEC PC98)
	keys[OIS::KC_UNDERLINE] = "KC_UNDERLINE";    //                     (NEC PC98)
	keys[OIS::KC_KANJI] = "KC_KANJI";    // (Japanese keyboard)
	keys[OIS::KC_STOP] = "KC_STOP";    //                     (NEC PC98)
	keys[OIS::KC_AX] = "KC_AX";    //                     (Japan AX)
	keys[OIS::KC_UNLABELED] = "KC_UNLABELED";    //                        (J3100)
	keys[OIS::KC_NEXTTRACK] = "KC_NEXTTRACK";    // Next Track
	keys[OIS::KC_NUMPADENTER] = "KC_NUMPADENTER";    // Enter on numeric keypad
	keys[OIS::KC_RCONTROL] = "KC_RCONTROL";
	keys[OIS::KC_MUTE] = "KC_MUTE";    // Mute
	keys[OIS::KC_CALCULATOR] = "KC_CALCULATOR";    // Calculator
	keys[OIS::KC_PLAYPAUSE] = "KC_PLAYPAUSE";    // Play / Pause
	keys[OIS::KC_MEDIASTOP] = "KC_MEDIASTOP";    // Media Stop
	keys[OIS::KC_VOLUMEDOWN] = "KC_VOLUMEDOWN";    // Volume -
	keys[OIS::KC_VOLUMEUP] = "KC_VOLUMEUP";    // Volume +
	keys[OIS::KC_WEBHOME] = "KC_WEBHOME";    // Web home
	keys[OIS::KC_NUMPADCOMMA] = "KC_NUMPADCOMMA"; //on numeric keypad (NEC PC98)
	keys[OIS::KC_DIVIDE] = "KC_DIVIDE";    // / on numeric keypad
	keys[OIS::KC_SYSRQ] = "KC_SYSRQ";
	keys[OIS::KC_RMENU] = "KC_RMENU";    // right Alt
	keys[OIS::KC_PAUSE] = "KC_PAUSE";    // Pause
	keys[OIS::KC_HOME] = "KC_HOME";    // Home on arrow keypad
	keys[OIS::KC_UP] = "KC_UP";    // UpArrow on arrow keypad
	keys[OIS::KC_PGUP] = "KC_PGUP";    // PgUp on arrow keypad
	keys[OIS::KC_LEFT] = "KC_LEFT";    // LeftArrow on arrow keypad
	keys[OIS::KC_RIGHT] = "KC_RIGHT";    // RightArrow on arrow keypad
	keys[OIS::KC_END] = "KC_END";    // End on arrow keypad
	keys[OIS::KC_DOWN] = "KC_DOWN";    // DownArrow on arrow keypad
	keys[OIS::KC_PGDOWN] = "KC_PGDOWN";    // PgDn on arrow keypad
	keys[OIS::KC_INSERT] = "KC_INSERT";    // Insert on arrow keypad
	keys[OIS::KC_DELETE] = "KC_DELETE";    // Delete on arrow keypad
	keys[OIS::KC_LWIN] = "KC_LWIN";    // Left Windows key
	keys[OIS::KC_RWIN] = "KC_RWIN";    // Right Windows key
	keys[OIS::KC_APPS] = "KC_APPS";    // AppMenu key
	keys[OIS::KC_POWER] = "KC_POWER";    // System Power
	keys[OIS::KC_SLEEP] = "KC_SLEEP";    // System Sleep
	keys[OIS::KC_WAKE] = "KC_WAKE";    // System Wake
	keys[OIS::KC_WEBSEARCH] = "KC_WEBSEARCH";    // Web Search
	keys[OIS::KC_WEBFAVORITES] = "KC_WEBFAVORITES";    // Web Favorites
	keys[OIS::KC_WEBREFRESH] = "KC_WEBREFRESH";    // Web Refresh
	keys[OIS::KC_WEBSTOP] = "KC_WEBSTOP";    // Web Stop
	keys[OIS::KC_WEBFORWARD] = "KC_WEBFORWARD";    // Web Forward
	keys[OIS::KC_WEBBACK] = "KC_WEBBACK";    // Web Back
	keys[OIS::KC_MYCOMPUTER] = "KC_MYCOMPUTER";    // My Computer
	keys[OIS::KC_MAIL] = "KC_MAIL";    // Mail
	keys[OIS::KC_MEDIASELECT] = "KC_MEDIASELECT";     // Media Select
	//ENDE tasten



}


Ogre::String StandardApplication::distanceModelToString(SimpleSound::SoundManager::DistanceModel mod)
{
	switch(mod)
	{
	case SimpleSound::SoundManager::INVERSE_DISTANCE:
		return "INVERSE_DISTANCE";
	case SimpleSound::SoundManager::INVERSE_DISTANCE_CLAMPED:
		return "INVERSE_DISTANCE_CLAMPED";
	case SimpleSound::SoundManager::LINEAR_DISTANCE:
		return "LINEAR_DISTANCE";
	case SimpleSound::SoundManager::LINEAR_DISTANCE_CLAMPED:
		return "LINEAR_DISTANCE_CLAMPED";
	case SimpleSound::SoundManager::EXPONENT_DISTANCE:
		return "EXPONENT_DISTANCE";
	case SimpleSound::SoundManager::EXPONENT_DISTANCE_CLAMPED:
		return "EXPONENT_DISTANCE_CLAMPED";
	};
	return "INVERSE_DISTANCE_CLAMPED";
}
//
//typedef struct
//{
//    HANDLE hf;
//    int error;
//} WIN32FILE_IOWIN;
//
//voidpf win32_open_file_func (voidpf opaque,
//   const char* filename,
//   int mode)
//{
//    const char* mode_fopen = NULL;
//    DWORD dwDesiredAccess,dwCreationDisposition,dwShareMode,dwFlagsAndAttributes ;
//    HANDLE hFile = 0;
//    voidpf ret=NULL;
//
//    dwDesiredAccess = dwShareMode = dwFlagsAndAttributes = 0;
//
//    if ((mode & ZLIB_FILEFUNC_MODE_READWRITEFILTER)==ZLIB_FILEFUNC_MODE_READ)
//    {
//        dwDesiredAccess = GENERIC_READ;
//        dwCreationDisposition = OPEN_EXISTING;
//        dwShareMode = FILE_SHARE_READ;
//    }
//    else
//    if (mode & ZLIB_FILEFUNC_MODE_EXISTING)
//    {
//        dwDesiredAccess = GENERIC_WRITE | GENERIC_READ;
//        dwCreationDisposition = OPEN_EXISTING;
//    }
//    else
//    if (mode & ZLIB_FILEFUNC_MODE_CREATE)
//    {
//        dwDesiredAccess = GENERIC_WRITE | GENERIC_READ;
//        dwCreationDisposition = CREATE_ALWAYS;
//    }
//	dwCreationDisposition = CREATE_ALWAYS;
//    if ((filename!=NULL) && (dwDesiredAccess != 0))
//	{
//		/*hFile = CreateFile(filename, GENERIC_WRITE, FILE_SHARE_WRITE,
//                   NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);*/
//		/* if there is a problem opening the file, a call to
//		 GetLastError will tell you what it is */
//        hFile = CreateFile((LPCTSTR)filename, dwDesiredAccess, dwShareMode, NULL,
//                      dwCreationDisposition, dwFlagsAndAttributes, NULL);
//	}
//	DWORD test = GetLastError();
//
//    if (hFile == INVALID_HANDLE_VALUE)
//        hFile = NULL;
//
//    if (hFile != NULL)
//    {
//        WIN32FILE_IOWIN w32fiow;
//        w32fiow.hf = hFile;
//        w32fiow.error = 0;
//        ret = malloc(sizeof(WIN32FILE_IOWIN));
//        if (ret==NULL)
//            CloseHandle(hFile);
//        else *((WIN32FILE_IOWIN*)ret) = w32fiow;
//    }
//    return ret;
//}
//
//
//uLong win32_read_file_func (voidpf opaque, voidpf stream, void* buf, uLong size)
//{
//    uLong ret=0;
//    HANDLE hFile = NULL;
//    if (stream!=NULL)
//        hFile = ((WIN32FILE_IOWIN*)stream) -> hf;
//    if (hFile != NULL)
//        if (!ReadFile(hFile, buf, size, &ret, NULL))
//        {
//            DWORD dwErr = GetLastError();
//            if (dwErr == ERROR_HANDLE_EOF)
//                dwErr = 0;
//            ((WIN32FILE_IOWIN*)stream) -> error=(int)dwErr;
//        }
//
//    return ret;
//}
//
//
//uLong win32_write_file_func (voidpf opaque,voidpf stream,const void* buf,uLong size)
//{
//    uLong ret=0;
//    HANDLE hFile = NULL;
//    if (stream!=NULL)
//        hFile = ((WIN32FILE_IOWIN*)stream) -> hf;
//
//    if (hFile !=NULL)
//        if (!WriteFile(hFile, buf, size, &ret, NULL))
//        {
//            DWORD dwErr = GetLastError();
//            if (dwErr == ERROR_HANDLE_EOF)
//                dwErr = 0;
//            ((WIN32FILE_IOWIN*)stream) -> error=(int)dwErr;
//        }
//
//    return ret;
//}
//
//long win32_tell_file_func (voidpf opaque,voidpf stream)
//{
//    long ret=-1;
//    HANDLE hFile = NULL;
//    if (stream!=NULL)
//        hFile = ((WIN32FILE_IOWIN*)stream) -> hf;
//    if (hFile != NULL)
//    {
//        DWORD dwSet = SetFilePointer(hFile, 0, NULL, FILE_CURRENT);
//        if (dwSet == INVALID_SET_FILE_POINTER)
//        {
//            DWORD dwErr = GetLastError();
//            ((WIN32FILE_IOWIN*)stream) -> error=(int)dwErr;
//            ret = -1;
//        }
//        else
//            ret=(long)dwSet;
//    }
//    return ret;
//}
//
//long win32_seek_file_func (voidpf opaque,voidpf stream,uLong offset,int origin)
//{
//    DWORD dwMoveMethod=0xFFFFFFFF;
//    HANDLE hFile = NULL;
//
//    long ret=-1;
//    if (stream!=NULL)
//        hFile = ((WIN32FILE_IOWIN*)stream) -> hf;
//    switch (origin)
//    {
//    case ZLIB_FILEFUNC_SEEK_CUR :
//        dwMoveMethod = FILE_CURRENT;
//        break;
//    case ZLIB_FILEFUNC_SEEK_END :
//        dwMoveMethod = FILE_END;
//        break;
//    case ZLIB_FILEFUNC_SEEK_SET :
//        dwMoveMethod = FILE_BEGIN;
//        break;
//    default: return -1;
//    }
//
//    if (hFile != NULL)
//    {
//        DWORD dwSet = SetFilePointer(hFile, offset, NULL, dwMoveMethod);
//        if (dwSet == INVALID_SET_FILE_POINTER)
//        {
//            DWORD dwErr = GetLastError();
//            ((WIN32FILE_IOWIN*)stream) -> error=(int)dwErr;
//            ret = -1;
//        }
//        else
//            ret=0;
//    }
//    return ret;
//}
//
//int win32_close_file_func (voidpf opaque,voidpf stream)
//{
//    int ret=-1;
//
//    if (stream!=NULL)
//    {
//        HANDLE hFile;
//        hFile = ((WIN32FILE_IOWIN*)stream) -> hf;
//        if (hFile != NULL)
//        {
//            CloseHandle(hFile);
//            ret=0;
//        }
//        free(stream);
//    }
//    return ret;
//}
//
//int win32_error_file_func (voidpf opaque,voidpf stream)
//{
//    int ret=-1;
//    if (stream!=NULL)
//    {
//        ret = ((WIN32FILE_IOWIN*)stream) -> error;
//    }
//    return ret;
//}
//
//void fill_win32_filefunc (zlib_filefunc_def* pzlib_filefunc_def)
//{
//    pzlib_filefunc_def->zopen_file = win32_open_file_func;
//    pzlib_filefunc_def->zread_file = win32_read_file_func;
//    pzlib_filefunc_def->zwrite_file = win32_write_file_func;
//    pzlib_filefunc_def->ztell_file = win32_tell_file_func;
//    pzlib_filefunc_def->zseek_file = win32_seek_file_func;
//    pzlib_filefunc_def->zclose_file = win32_close_file_func;
//    pzlib_filefunc_def->zerror_file = win32_error_file_func;
//    pzlib_filefunc_def->opaque=NULL;
//}



SimpleSound::SoundManager::DistanceModel StandardApplication::parseDistanceModel(Ogre::String ogre_str)
{
	if(ogre_str == "INVERSE_DISTANCE")
	{
		return SimpleSound::SoundManager::INVERSE_DISTANCE;
	}
	else if(ogre_str == "INVERSE_DISTANCE_CLAMPED")
	{
		return SimpleSound::SoundManager::INVERSE_DISTANCE_CLAMPED;
	}
	else if(ogre_str == "LINEAR_DISTANCE")
	{
		return SimpleSound::SoundManager::LINEAR_DISTANCE;
	}
	else if(ogre_str == "LINEAR_DISTANCE_CLAMPED")
	{
		return SimpleSound::SoundManager::LINEAR_DISTANCE_CLAMPED;
	}
	else if(ogre_str == "EXPONENT_DISTANCE")
	{
		return SimpleSound::SoundManager::EXPONENT_DISTANCE;
	}
	else if(ogre_str == "EXPONENT_DISTANCE_CLAMPED")
	{
		return SimpleSound::SoundManager::EXPONENT_DISTANCE_CLAMPED;
	}
	return SimpleSound::SoundManager::INVERSE_DISTANCE_CLAMPED;
}

void StandardApplication::setWindowTitle(Ogre::String title)
{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	HWND hwnd;
	mWindow->getCustomAttribute("WINDOW", (void*)&hwnd);
	SetWindowText(hwnd,title.c_str());
	/*LONG iconID   = (LONG)LoadIcon( GetModuleHandle(0), MAKEINTRESOURCE(IDI_ICON1) );
	SetClassLong( hwnd, GCL_HICON, iconID );*/
#endif
}


void StandardApplication::setClipboardText(Ogre::String txt) 
{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	HWND hwnd;
	mWindow->getCustomAttribute("WINDOW", (void*)&hwnd);
	if(!OpenClipboard(hwnd))
	{
		clipboard = txt;
	}
	else
	{
		
		if(EmptyClipboard())
		{
			// Allocate a global memory object for the text. 
	 
			HGLOBAL hglbCopy = GlobalAlloc(GMEM_MOVEABLE, 
				(txt.length() + 1) * sizeof(TCHAR)); 
			if (hglbCopy == NULL) 
			{ 
				CloseClipboard(); 
				return; 
			} 
	 
			// Lock the handle and copy the text to the buffer. 
			 
			
			
			

			LPTSTR lptstrCopy = (LPTSTR)GlobalLock(hglbCopy); 
			memcpy(lptstrCopy, txt.c_str(), 
				txt.length() * sizeof(TCHAR)); 
			lptstrCopy[txt.length()] = (TCHAR) 0;    // null character 
			GlobalUnlock(hglbCopy); 
	 
			// Place the handle on the clipboard. 
	 
			SetClipboardData(CF_TEXT, hglbCopy); 
			//	SetClipboardData(CF_UNICODETEXT,txt.c_str());
		}
		
		CloseClipboard();
	}
#else
	clipboard = txt;
#endif
}
Ogre::String StandardApplication::getClipboardText()
{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	HWND hwnd;
	mWindow->getCustomAttribute("WINDOW", (void*)&hwnd);
	if (!IsClipboardFormatAvailable(CF_TEXT)) 
		return ""; 
	if (!OpenClipboard(hwnd)) 
		return ""; 
	Ogre::String result;
    HGLOBAL	hglb = GetClipboardData(CF_TEXT); 
    if (hglb != NULL) 
    { 
        LPTSTR lptstr = (LPTSTR)GlobalLock(hglb); 
        if (lptstr != NULL) 
        { 
            // Call the application-defined ReplaceSelection 
            // function to insert the text and repaint the 
            // window. 

            result = lptstr;
            GlobalUnlock(hglb); 
        } 
    } 
    CloseClipboard(); 
	return result;
#else
	return clipboard;
#endif
}


StandardApplication::~StandardApplication()
{



	GameDataMap::iterator itr;
	for(itr = gameDataMap.begin();itr!= gameDataMap.end();itr++)
	{
		gamedata *data = itr->second;
		data->prepareFinalRemoval();
		//if(data->type == gtCharacter)
		//{
		//	gamedata_char *cur = static_cast<gamedata_char*>(data);
		//	if(cur->defaultInventory)
		//		delete cur->defaultInventory;
		//	if(cur->defaultSpellbook)
		//		delete cur->defaultSpellbook;
		//}
		////TEST
		//if(data->type == gtSpell)
		//{
		//	gamedata_spell *test = static_cast<gamedata_spell*>(data);
		//	mLog(test->id);
		//}
		delete data;
		itr->second = NULL;
	}
	
	if(alchemy)
		delete alchemy;
}

void StandardApplication::go(void)
{

    if (!setup())
        return;

    mRoot->startRendering();

    // clean up
//    destroyScene();
}
//
//bool StandardApplication::setup(void)
//{
////	
////    mRoot = new Ogre::Root();
////log("setup in SA");
////	//mWorld = new OgreNewt::World();
////
////    setupResources();
////	log("resourcen gesetzt");
////
////    bool carryOn = configure();
////    if (!carryOn) return false;
////
////    chooseSceneManager();
////    createCamera();
////
////    createViewports();
////
////    // Set default mipmap level (NB some APIs ignore this)
////    //TextureManager::getSingleton().setDefaultNumMipmaps(5);
////	//global = new global_class(mRoot, mGUIRenderer, mGUISystem, mEditorGuiSheet, mSceneMgr, mWindow);
////	// Create any resource listeners (for loading screens)
////	createResourceListener();
////	// Load resources
////	loadResources();
////
//////	showtextpanel();
//////	app->init(mRoot);
////
////	createMaterials();
////	loadGameData();
////
////	
////
////
////
////	// Create the scene
////    createScene();
////	createGUI();
////	//if(!mGUIRenderer)
////	//{
////	//	log("!mGUIRenderer");
////	//}
////	log("scene created");
////
////
//////	settext("asdf");
////    createFrameListener();
//////	settext("framelisteners set");
//    return true;
//
//}

bool StandardApplication::configure(void)
{
	
    // Show the configuration dialog and initialise the system
    // You can skip this and use root.restoreConfig() to load configuration
    // settings if you were sure there are valid ones saved in ogre.cfg
    if(mRoot->showConfigDialog())
    {
        // If returned true, user clicked OK so initialise
        // Here we choose to let the system create a default rendering window by passing 'true'
        mWindow = mRoot->initialise(true, "LoL4 test app");
        return true;
    }
    else
    {
        return false;
    }
}
//void StandardApplication::chooseSceneManager(void)
//{
//// Create the Ogre::SceneManager, in this case a generic one
////-----------------------------------------------------
//// 4 Create the Ogre::SceneManager
////
////		ST_GENERIC = octree
////		ST_EXTERIOR_CLOSE = simple terrain
////		ST_EXTERIOR_FAR = nature terrain (depreciated)
////		ST_EXTERIOR_REAL_FAR = paging landscape
////		ST_INTERIOR = Quake3 BSP
//////-----------------------------------------------------
////mSceneMgr = mRoot->createSceneManager(ST_GENERIC);//, "ExampleSMInstance");
//
//}
//
//void StandardApplication::createCamera(void)
//{
//    //// Create the camera
//    //mCamera = mSceneMgr->createCamera("PlayerCam");
//
//    //// Position it at 500 in Z direction
//    ////mCamera->setPosition(Ogre::Vector3(0,0,500));
//    //// Look back along -Z
//    ////mCamera->lookAt(Ogre::Vector3(0,0,-300));
//    //mCamera->setNearClipDistance(0.01);
//
////}
////void StandardApplication::createMaterials(void)
////{
////	//const OgreNewt::MaterialID *defaultMaterialID;
////	//defaultMaterialID = mWorld->getDefaultMaterialID();
////
////	//OgreNewt::MaterialPair* default_default = new OgreNewt::MaterialPair( mWorld, defaultMaterialID, defaultMaterialID );
////	//default_default->setDefaultFriction(1.0f, 0.5f);
////	//default_default->setDefaultSoftness(0.05f);
////	//default_default->setDefaultElasticity(0.1f);
////
////	//charMaterial = new OgreNewt::MaterialID(mWorld);
////	////mBody->setMaterialGroupID(charMaterial);
////	////mBody->setLinearDamping(10);
////
////	//OgreNewt::MaterialPair* default_char = new OgreNewt::MaterialPair( mWorld, charMaterial, defaultMaterialID );
////
////
////	////cc_static_char = new ContCallback();
////
////
////	////material_pair2->setDefaultFriction(0.0f, 0.0f);
////	//default_char->setDefaultFriction(0,0);
////	//default_char->setDefaultSoftness(1);
////	//default_char->setDefaultElasticity(0);
////	//default_char->setContinuousCollisionMode(1);
////	////default_char->setContactCallback(cc_static_char);
////
//}
//
//void StandardApplication::createFrameListener(void)
//{
//	//override this
////    //mFrameListener= new GameFrameListener(mWindow, mCamera);
////    //mFrameListener->showDebugOverlay(true);
////
////
////	mEventProcessor = new EventProcessor();
////	mEventProcessor->initialise(mWindow);
////	mEventProcessor->startProcessingEvents();
////	// event engine is running, now we listen for keys and frames (replaces while loops)
////	//mKeyListener = new CKeyListener(mGUIRenderer);
////	//mKeyListener->setGlobal(global);
////	//mEventProcessor->addKeyListener(mKeyListener);
////	////eventProcessor->addCursorMoveListener(keyListener);
////	//mEventProcessor->addMouseMotionListener(mKeyListener);
////	//mEventProcessor->addMouseListener(mKeyListener);
////	//mLog("key listener online");
////
////	mFrameListener = new GameFrameListener(this,mEventProcessor->getInputReader(),mGUIRenderer,mWindow, mSceneMgr, mWorld, 120 );
//////	mFrameListener->setGlobal(global);
////	mRoot->addFrameListener(mFrameListener);
////	mEventProcessor->addKeyListener(mFrameListener);
////
////	mEventProcessor->addMouseMotionListener(mFrameListener);
////	mEventProcessor->addMouseListener(mFrameListener);
////	log("frame listener online");
////
////	//mOgreNewtListener = new OgreNewt::BasicFrameListener( mWindow, mSceneMgr, mWorld, 120 );
////	//mRoot->addFrameListener( mOgreNewtListener );
////	//mLog("ogrenewt listener online");
//}

//void StandardApplication::createScene(void)
//{
//	//mSceneMgr->setAmbientLight(Ogre::ColourValue(1, 1, 1));
//	//log("licht->kk");
//	//loadLvl("start.xml");
//}

//void StandardApplication::destroyScene(void){}    // Optional to override this

//void StandardApplication::createViewports(void)
//{
//    //// Create one viewport, entire window
//    //Ogre::Viewport* vp = mWindow->addViewport(mCamera);
//    //vp->setBackgroundColour(Ogre::ColourValue(0,0,0));
//
//    //// Alter the camera aspect ratio to match the viewport
//    //mCamera->setAspectRatio(
//    //    Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
//}

//erwartet den Pointer auf das <Resources> Element
void StandardApplication::setupResources(TiXmlElement *elem)
{
    Ogre::ResourceGroupManager *resMgr = Ogre::ResourceGroupManager::getSingletonPtr();
    TiXmlElement *groupElem = elem->FirstChildElement("Group");
    
    while(groupElem)
    {
        Ogre::String groupName = groupElem->Attribute("name");
        TiXmlElement *resElem = groupElem->FirstChildElement("Resource");
        while(resElem)
        {
            OgreXmlElement *elem = static_cast<OgreXmlElement*>(resElem);
            Ogre::String typeName = elem->AttributeAsString("type","FileSystem");
            Ogre::String location = elem->AttributeAsString("location",".");
            bool recursive  = elem->AttributeAsBool("recursive",false);
            resMgr->addResourceLocation(location,typeName,groupName,recursive);
            if(groupName == "Level" && typeName == "FileSystem")
		    {
			    levelPath = location;
		    }
		    else if(groupName == "Collision" && typeName == "FileSystem")
		    {
			    collisionPath = location;
		    }
		    else if(groupName == "Savegame" && typeName == "FileSystem")
		    {
			    saveGamePath = location;
		    }
		    else if(groupName == "Temp" && typeName == "FileSystem")
		    {
			    tempPath = location;
		    }
            resElem = resElem->NextSiblingElement("Resource");
        }
        
        groupElem = groupElem->NextSiblingElement("Group");
    }
}
//erwartet den Pointer auf das <Plugins> Element
void StandardApplication::setupPlugins(TiXmlElement *elem)
{
    Ogre::String pluginDir = elem->Attribute("folder");
            
    char last_char = pluginDir[pluginDir.length()-1];
    if (last_char != '/' && last_char != '\\')
    {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        pluginDir += "\\";
#elif OGRE_PLATFORM == OGRE_PLATFORM_LINUX
        pluginDir += "/";
#endif
    }
    TiXmlElement* pluginElem = elem->FirstChildElement("Plugin");
    while(pluginElem)
    {
        Ogre::String name = pluginElem->Attribute("name");
		/*if(!Ogre::StringUtil::endsWith(name,".dll"))
		{
			name += ".dll"; 
		}*/
        mRoot->loadPlugin(pluginDir+name);
        pluginElem = pluginElem->NextSiblingElement("Plugin");
    } 
}
 
//lädt die config.xml
TiXmlDocument *StandardApplication::loadConfigXml()
{
    const char* test = configFilename.c_str();
    std::ifstream fp;
    // Always open in binary mode
    fp.open(configFilename.c_str(), std::ios::in | std::ios::binary);
	if(!fp)
		OGRE_EXCEPT(
        Ogre::Exception::ERR_FILE_NOT_FOUND, "'"+configFilename+"' file not found!", "EditorApp::setup" );

	// Wrap as a stream
	Ogre::DataStreamPtr stream(new Ogre::FileStreamDataStream(configFilename, &fp, false));
    Ogre::String data = stream->getAsString();
    TiXmlDocument *doc;
    doc = new TiXmlDocument();
    //doc->LoadFile("config.xml");
    doc->Parse( data.c_str() );
    //erstmal die resources
    stream->close();
    stream.setNull();
    return doc;
}
//speichert doc und löscht das objekt
void StandardApplication::saveConfigXml(TiXmlDocument *doc)
{
    doc->SaveFile(configFilename);
    delete doc;
}

//erwartet den Pointer auf das <Renderer> Element
void StandardApplication::setupRenderSystem(TiXmlElement *elem)
{
    Ogre::String selectedRenderer = elem->Attribute("selected");
    TiXmlElement *rsElem = elem->FirstChildElement("RenderSystem");
    Ogre::String rsName;
    while(rsElem)
    {
        rsName = rsElem->Attribute("name");
        Ogre::RenderSystem *rs = mRoot->getRenderSystemByName(rsName);
        if(!rs)
        {
            Ogre::LogManager::getSingletonPtr()->logMessage("Warning: Invalid Render System '"+rsName+"'! The Section will be ignored!");
            rsElem = rsElem->NextSiblingElement("RenderSystem");
            continue;
        }
        //jetzt die optionen
        TiXmlElement *option = rsElem->FirstChildElement("option");
        Ogre::ConfigOptionMap cfg = rs->getConfigOptions();
        while(option)
        {
            Ogre::String optName = option->Attribute("name");
            Ogre::String optValue= option->Attribute("value");
            Ogre::ConfigOption curOption = cfg[optName];
            //schauen, ob es das ausgewählte überhaupt gibt
            bool valueFound = false;
            for(Ogre::StringVector::iterator itr = curOption.possibleValues.begin();itr!=curOption.possibleValues.end();itr++)
            {
                if(*itr == optValue)
                {
                    rs->setConfigOption(optName,optValue);
                    valueFound = true;
                    break;
                }
            }           
            if(!valueFound)
                Ogre::LogManager::getSingletonPtr()->logMessage("Warning: Invalid value '"+optValue+
                "' for property '"+optName+"' of Render System '"+rsName+"'! Default value '"+curOption.currentValue+"' is used instead");
            //ansonsten ignorieren
            option = option->NextSiblingElement("option");
        }
        rsElem = rsElem->NextSiblingElement("RenderSystem");
    }
    //jetzt mal das aktive RS
    Ogre::RenderSystem *rs = mRoot->getRenderSystemByName(selectedRenderer);
    if(!rs)
    {
		
        Ogre::LogManager::getSingletonPtr()->logMessage("Warning: Selected Render System '"+selectedRenderer+"' is invalid!");
        rs = *mRoot->getAvailableRenderers().begin();
        //eines suchen
        for (Ogre::RenderSystemList::const_iterator pRend = mRoot->getAvailableRenderers().begin(); pRend != mRoot->getAvailableRenderers().end(); ++pRend)
        {
            Ogre::RenderSystem* cur = *pRend;
            if(Ogre::StringUtil::match(cur->getName(),"*Direct3D*"))
            {
                rs = cur;
                break;
            }
            
        }
        Ogre::LogManager::getSingletonPtr()->logMessage("Render System '"+rs->getName()+"' will be used instead.");
    }
    mRoot->setRenderSystem(rs);
}

/// Method which will define the source of resources (other than current folder)
void StandardApplication::setupResources(void)
{

	// Load resource paths from config file

	// File format is:
	//  [ResourceGroupName]
	//  ArchiveType=Path
	//  .. repeat
	// For example:
	//  [General]
	//  FileSystem=media/
	//  Zip=packages/level1.zip

	Ogre::ConfigFile cf;
	cf.load("resources.cfg");
	
	// Go through all sections & settings in the file
	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

	Ogre::String secName, typeName, archName;
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
				archName, typeName, secName);
			if(secName == "Level" && typeName == "FileSystem")
			{
				levelPath = archName;
			}
			else if(secName == "Collision" && typeName == "FileSystem")
			{
				collisionPath = archName;
			}
			else if(secName == "Savegame" && typeName == "FileSystem")
			{
				saveGamePath = archName;
			}
			else if(secName == "Temp" && typeName == "FileSystem")
			{
				tempPath = archName;
			}
			//log(archName+" - "+typeName+" - "+secName);
		}
	}
	

}

void StandardApplication::renderSettingsToXml(OgreXmlElement *elem)
{
	for (Ogre::RenderSystemList::const_iterator pRend = mRoot->getAvailableRenderers().begin(); pRend != mRoot->getAvailableRenderers().end(); ++pRend)
    {
        Ogre::RenderSystem* rs = *pRend;
        OgreXmlElement *rsElem = new OgreXmlElement("RenderSystem");
        rsElem->SetAttribute("name",rs->getName());
        elem->LinkEndChild(rsElem);
        
        const Ogre::ConfigOptionMap& opts = rs->getConfigOptions();
        for (Ogre::ConfigOptionMap::const_iterator pOpt = opts.begin(); pOpt != opts.end(); ++pOpt)
        {
            OgreXmlElement *option = new OgreXmlElement("option");
            option->SetAttribute("name",pOpt->first);
            option->SetAttribute("value",pOpt->second.currentValue);
            rsElem->LinkEndChild(option);
        }
    }
}

//void StandardApplication::createResourceListener(void)
//{
//
//}

/// Optional override method where you can perform resource group loading
/// Must at least do Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
void StandardApplication::loadResources(void)
{
	// Initialise, parse scripts etc
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	


}

void StandardApplication::loadManualLoD()
{
    Ogre::DataStreamPtr pStream = Ogre::ResourceGroupManager::getSingleton().openResource("manualLoD.xml","game_data");

	Ogre::String data = pStream->getAsString();
	TiXmlDocument *ObjDoc;
	ObjDoc = new TiXmlDocument();
	ObjDoc->Parse( data.c_str() );

	pStream->close();
	pStream.setNull();
	TiXmlElement   *XMLRoot;
	
	XMLRoot = ObjDoc->FirstChildElement("lodData");
	if(!XMLRoot)
	{
		return;
	}
    TiXmlElement *XMLNode=XMLRoot->FirstChildElement("mesh");
	while(XMLNode)
	{
        
        Ogre::String curMeshName = XMLNode->Attribute("name");
        Ogre::MeshPtr curMesh = Ogre::MeshManager::getSingletonPtr()->getByName(curMeshName);
        if(curMesh.isNull())
        {
            curMesh = Ogre::MeshManager::getSingletonPtr()->load(curMeshName,Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
            if(curMesh.isNull())
            {
                mLog("Error: LoD settings for mesh "+curMeshName+" which does not exist");
                XMLNode = XMLNode->NextSiblingElement("mesh");
                continue;
            }
        }
        //entfernt alle bereits existierenden LODs, wenn es hier manuell drinsteht
        if(curMesh->getNumLodLevels() > 1)
            curMesh->removeLodLevels();
        TiXmlElement *lodNode=XMLNode->FirstChildElement("lod");
        while(lodNode)
        {
            Ogre::Real fromDepth = Ogre::StringConverter::parseReal(lodNode->Attribute("fromDepth"));
            Ogre::String meshName = lodNode->Attribute("meshName");
            curMesh->createManualLodLevel(fromDepth,meshName);
            lodNode = lodNode->NextSiblingElement("lod");
        }
        
        XMLNode = XMLNode->NextSiblingElement("mesh");
    }
}

void StandardApplication::setDefaultData()
{
	/*player*/
	gamedata_char *player = new gamedata_char();
	player->id = "player";
	player->wData.orientationOffset = Ogre::Quaternion::IDENTITY;
	player->wData.type = WT_NONE;
//	player.mesh_orientation_offset = Ogre::Quaternion::IDENTITY;
	player->head_position = Ogre::Vector3(0,0.9,-0.4);
	player->cData.mass = 60;
	player->cData.colType = CT_ELLIPSOID;
	player->cData.collistionParams = Ogre::Vector3(0.5,1.65,0.5);
	player->cData.upVector = Ogre::Vector3::UNIT_Y;

//	player.size=Ogre::Vector3(0.5,1.65,0.5);
	player->name="Player";

	registerGameData(player);
	//gameDataMap["player"] = player;

}
//die funktion erwartet ein fertiges inventar, und schreibt da items rein, ohne Rücksicht auf Verluste
void StandardApplication::parseInventory(TiXmlElement *elem, Inventory *inv)
{
	TiXmlElement *XMLNode=elem->FirstChildElement("item");
	while(XMLNode)
	{
		Inventory::invDataItem cur;
		cur.count = 1;
		cur.item  = NULL;
		if(XMLNode->Attribute("id"))
		{
			cur.item = getItemDataPtr(XMLNode->Attribute("id"));
		}
		if(XMLNode->Attribute("count"))
		{
			cur.count=Ogre::StringConverter::parseInt(XMLNode->Attribute("count"));
		}
		if(cur.count == 0 || cur.item == NULL)
		{
			cur.count = 0;
			cur.item= NULL;
		}
		inv->itemVector.push_back(cur);
		XMLNode = XMLNode->NextSiblingElement("item");
	}
	inv->resyncItemCount();

}

void StandardApplication::parseSpellbook(TiXmlElement *elem, Spellbook *sb)
{
	TiXmlElement *XMLNode=elem->FirstChildElement("spell");
	while(XMLNode)
	{
		gamedata_spell *cur=NULL;
		if(XMLNode->Attribute("id"))
		{
			cur= getSpellDataPtr(XMLNode->Attribute("id"));
		}
		if(cur)
		{
			sb->spellVector.push_back(cur);
		}
		XMLNode = XMLNode->NextSiblingElement("spell");
	}
	sb->resyncCount();

}


void StandardApplication::spellbookToXml(TiXmlElement *elem, Spellbook *sb)
{
	for(std::vector<gamedata_spell*>::iterator itr = sb->spellVector.begin();itr!=sb->spellVector.end();itr++)
	{
		TiXmlElement *spell = new TiXmlElement("spell");
		spell->SetAttribute("id",(*itr)->id.c_str());
		elem->LinkEndChild(spell);
	}

}


void StandardApplication::spellListToXml(TiXmlElement *elem,SpellList list)
{
	for(SpellList::iterator itr=list.begin();itr!=list.end();itr++)
	{
		TiXmlElement *child = new TiXmlElement("spelleffect");
		SpellObject* cur = *itr;
		child->SetAttribute("id",cur->getData()->id.c_str());
		if(cur->caster)
			child->SetAttribute( "caster",ogre_str(cur->caster->getSGID()).c_str() );
		if(cur->target)
			child->SetAttribute("target",ogre_str(cur->target->getSGID()).c_str());
		child->SetAttribute("lifetime",ogre_str(cur->lifetime).c_str());
		
		elem->LinkEndChild(child);
	}
}

void StandardApplication::inventoryToXml(TiXmlElement *elem, Inventory *inv)
{
	TiXmlElement *XMLNode;// = new TiXmlElement( "item" );
	std::vector<Inventory::invDataItem>::const_iterator itr;
	//size_t cnt = inv->getMaxSize();

	for(itr = inv->itemVector.begin();itr != inv->itemVector.end();itr++)
	{
		XMLNode = new TiXmlElement( "item" );
		Inventory::invDataItem cur = *itr;
		if(!cur.item)
		{
			XMLNode->SetAttribute("id","");
			XMLNode->SetAttribute("count","0");
		}
		else
		{
			XMLNode->SetAttribute("id",cur.item->id.c_str());
			XMLNode->SetAttribute("count",ogre_str(cur.count).c_str());
		}
		elem->LinkEndChild(XMLNode);
		
	}
}


void StandardApplication::statsToXml(TiXmlElement *elem, Stats st)
{
	TiXmlElement *XMLNode = new TiXmlElement( "strength" );
	XMLNode->SetAttribute("value",ogre_str(st.mStats[st_strength]).c_str());
	elem->LinkEndChild(XMLNode);

	XMLNode = new TiXmlElement( "agility" );
	XMLNode->SetAttribute("value",ogre_str(st.mStats[st_agility]).c_str());
	elem->LinkEndChild(XMLNode);

	XMLNode = new TiXmlElement( "magic" );
	XMLNode->SetAttribute("value",ogre_str(st.mStats[st_magic]).c_str());
	elem->LinkEndChild(XMLNode);

	XMLNode = new TiXmlElement( "maxHP" );
	XMLNode->SetAttribute("value",ogre_str(st.mStats[st_maxHP]).c_str());
	elem->LinkEndChild(XMLNode);

	XMLNode = new TiXmlElement( "maxMP" );
	XMLNode->SetAttribute("value",ogre_str(st.mStats[st_maxMP]).c_str());
	elem->LinkEndChild(XMLNode);

	XMLNode = new TiXmlElement( "resistBlunt" );
	XMLNode->SetAttribute("value",ogre_str(st.mStats[st_resistBlunt]).c_str());
	elem->LinkEndChild(XMLNode);

	XMLNode = new TiXmlElement( "resistCut" );
	XMLNode->SetAttribute("value",ogre_str(st.mStats[st_resistCut]).c_str());
	elem->LinkEndChild(XMLNode);

	XMLNode = new TiXmlElement( "resistPierce" );
	XMLNode->SetAttribute("value",ogre_str(st.mStats[st_resistPierce]).c_str());
	elem->LinkEndChild(XMLNode);

	XMLNode = new TiXmlElement( "resistFire" );
	XMLNode->SetAttribute("value",ogre_str(st.mStats[st_resistFire]).c_str());
	elem->LinkEndChild(XMLNode);

	XMLNode = new TiXmlElement( "resistIce" );
	XMLNode->SetAttribute("value",ogre_str(st.mStats[st_resistIce]).c_str());
	elem->LinkEndChild(XMLNode);

	XMLNode = new TiXmlElement( "resistSpark" );
	XMLNode->SetAttribute("value",ogre_str(st.mStats[st_resistSpark]).c_str());
	elem->LinkEndChild(XMLNode);

	XMLNode = new TiXmlElement( "resistPoison" );
	XMLNode->SetAttribute("value",ogre_str(st.mStats[st_resistPoison]).c_str());
	elem->LinkEndChild(XMLNode);

	XMLNode = new TiXmlElement( "resistDark" );
	XMLNode->SetAttribute("value",ogre_str(st.mStats[st_resistDark]).c_str());
	elem->LinkEndChild(XMLNode);

	XMLNode = new TiXmlElement( "resistLight" );
	XMLNode->SetAttribute("value",ogre_str(st.mStats[st_resistLight]).c_str());
	elem->LinkEndChild(XMLNode);

	XMLNode = new TiXmlElement( "hpRegenRate" );
	XMLNode->SetAttribute("value",ogre_str(st.mStats[st_hpRegenRate]).c_str());
	elem->LinkEndChild(XMLNode);

	XMLNode = new TiXmlElement( "manaRegenRate" );
	XMLNode->SetAttribute("value",ogre_str(st.mStats[st_manaRegenRate]).c_str());
	elem->LinkEndChild(XMLNode);

	XMLNode = new TiXmlElement( "walkSpeed" );
	XMLNode->SetAttribute("value",ogre_str(st.mStats[st_walkSpeed]).c_str());
	elem->LinkEndChild(XMLNode);

	XMLNode = new TiXmlElement( "runSpeed" );
	XMLNode->SetAttribute("value",ogre_str(st.mStats[st_runSpeed]).c_str());
	elem->LinkEndChild(XMLNode);

	XMLNode = new TiXmlElement( "ancientMagic" );
	XMLNode->SetAttribute("value",ogre_str(st.ancientMagic).c_str());
	elem->LinkEndChild(XMLNode);

	XMLNode = new TiXmlElement( "mana" );
	XMLNode->SetAttribute("value",ogre_str(st.mana).c_str());
	elem->LinkEndChild(XMLNode);



}

void StandardApplication::processDoorData(TiXmlElement *XMLRoot)
{
	TiXmlElement *XMLNode=XMLRoot->FirstChildElement("object");
	while(XMLNode)
	{
		//<object id="ogre" mesh="ogrehead.mesh" scale="0.1 0.1 0.1" name="Ogreförmige Tür" />
		if(!XMLNode->Attribute("id"))
			break;
		gamedata_door *cur = new gamedata_door();
		cur->name="";
		
		cur->id = XMLNode->Attribute("id");
		WorldArtData myWAD;
		myWAD.filename	= ERROR_MESH;
		myWAD.scale		= Ogre::Vector3::UNIT_SCALE;
		myWAD.material	= "";
		myWAD.positionOffset = Ogre::Vector3::ZERO;
		myWAD.orientationOffset = Ogre::Quaternion::IDENTITY;
		myWAD.type = WT_NONE;
		cur->wData = myWAD;
//		cur.collisionParams=Ogre::Vector3::UNIT_SCALE;
//		cur.collisionType=0;
		cur->flags=0;
		cur->useOnCollision = false;
		cur->cData.colType = CT_TREECOLLISION;
//		cur.mass=0;
		
		if( XMLNode->FirstChildElement("collision"))
		{
			cur->cData = parseCollisionData(XMLNode->FirstChildElement("collision"));
		}
		else
		{
			cur->cData.colType = CT_BOX;
			cur->cData.collistionParams = Ogre::Vector3::UNIT_SCALE;
		}
		if( XMLNode->FirstChildElement("worldArt"))
		{
			cur->wData = parseWorldArtData(XMLNode->FirstChildElement("worldArt"));
		}
		else
		{
#ifdef __editor
			//kollisionsabhängige mesh zeigen
			cur->wData.type = WT_MESH;
			cur->wData.material = MAT_REV_PLANE;

			switch(cur->cData.colType)
			{
			case CT_BOX:
				cur->wData.filename = "box.mesh";
				cur->wData.scale = cur->cData.collistionParams;
				break;
			case CT_ELLIPSOID:
				cur->wData.filename = "ellipsoid.mesh";
				cur->wData.scale = cur->cData.collistionParams;
				break;
			default:
				//ka was
				cur->wData.filename = "box.mesh";
				cur->wData.scale = cur->cData.collistionParams;
				break;
			}
#endif
			
		}		

		
		if( XMLNode->Attribute("name"))
		{
			cur->name = XMLNode->Attribute("name");

		}
		if( XMLNode->Attribute("useOnCollision"))
		{
			cur->useOnCollision = Ogre::StringConverter::parseBool(XMLNode->Attribute("useOnCollision"));

		}
	
		if(XMLNode->Attribute("flags"))
		{
			cur->flags=Ogre::StringConverter::parseInt(XMLNode->Attribute("flags"));
		}
		
		
		registerGameData(cur);
		XMLNode = XMLNode->NextSiblingElement("object");
	}
}

WorldArtData StandardApplication::parseWorldArtData(TiXmlElement *XMLNode)
{
	WorldArtData res;
	res.type = WT_MESH;
	res.scale = Ogre::Vector3::UNIT_SCALE;
	res.orientationOffset = Ogre::Quaternion::IDENTITY;
	res.positionOffset = Ogre::Vector3::ZERO;
	res.filename = "";
	res.material = "";
	res.bbColor = Ogre::ColourValue::White;
	res.shadow = true;
	res.loopAnim = "";
	if(XMLNode->Attribute("type"))
	{
		Ogre::String type = XMLNode->Attribute("type");
		Ogre::StringUtil::toLowerCase(type);
		if(type == "none")
			res.type = WT_NONE;
		else if(type == "mesh")
			res.type = WT_MESH;
		else if(type == "billboard")
			res.type = WT_BILLBOARD;
		else if(type == "dotscene")
			res.type = WT_DOTSCENE;
		else if(type == "particle")
		{
			res.type = WT_PARTICLESYSTEM;
			if(XMLNode->Attribute("name"))
				res.material = XMLNode->Attribute("name");
		}
		
		else if(type == "plane")
		{
			res.type = WT_PLANE;
			res.pData = parsePlaneData(XMLNode);
		}
		else if(type == "billboardchain")
		{
			res.type = WT_BILLBOARDCHAIN;
			res.bbcData = parseBBChainData(XMLNode);
		}
		else if(type == "ribbontrail")
		{
			res.type = WT_RIBBONTRAIL;
			res.bbcData = parseBBChainData(XMLNode);
		}
	}
	if(XMLNode->Attribute("loopanim"))
		res.loopAnim = XMLNode->Attribute("loopanim");
	if(XMLNode->Attribute("filename"))
		res.filename = XMLNode->Attribute("filename");
	if(XMLNode->Attribute("shadow"))
		res.shadow = Ogre::StringConverter::parseBool(XMLNode->Attribute("shadow"));
	if(XMLNode->Attribute("material"))
		res.material = XMLNode->Attribute("material");
	if(XMLNode->Attribute("billboard_color"))
		res.bbColor = Ogre::StringConverter::parseColourValue(XMLNode->Attribute("billboard_color"));
	if(XMLNode->Attribute("scale"))
	{
		res.scale = Ogre::StringConverter::parseVector3(XMLNode->Attribute("scale"));
		if(res.scale == Ogre::Vector3::ZERO)
			res.scale = Ogre::Vector3(Ogre::StringConverter::parseReal(XMLNode->Attribute("scale")));
	}
	if(XMLNode->Attribute("position_offset"))
		res.positionOffset = Ogre::StringConverter::parseVector3(XMLNode->Attribute("position_offset"));
	if(XMLNode->Attribute("orientation_offset"))
		res.orientationOffset = Ogre::StringConverter::parseQuaternion(XMLNode->Attribute("orientation_offset"));
	

	return res;
}

billboardChainData StandardApplication::parseBBChainData(TiXmlElement *elem)
{
	billboardChainData res;
	/*size_t maxElements;//=20, 
		size_t numberOfChains;//=1, 
		bool useTextureCoords;//=true, 
		bool useColours;//=true, 
		bool dynamic;//=true)*/
	if(elem->Attribute("maxElements"))
	{
		res.maxElements = Ogre::StringConverter::parseUnsignedInt(elem->Attribute("maxElements"));
	}
	if(elem->Attribute("numberOfChains"))
	{
		res.numberOfChains = Ogre::StringConverter::parseUnsignedInt(elem->Attribute("numberOfChains"));
	}
	if(elem->Attribute("useTextureCoords"))
	{
		res.useTextureCoords = Ogre::StringConverter::parseBool(elem->Attribute("useTextureCoords"));
	}
	if(elem->Attribute("useColours"))
	{
		res.useColours = Ogre::StringConverter::parseBool(elem->Attribute("useColours"));
	}
	if(elem->Attribute("dynamic"))
	{
		res.useTextureCoords = Ogre::StringConverter::parseBool(elem->Attribute("dynamic"));
	}
	
	return res;
}

PlaneData StandardApplication::parsePlaneData(TiXmlElement *elem)
{
	PlaneData res;
	res.constant = 0;
	res.normal = Ogre::Vector3::NEGATIVE_UNIT_Z;
	res.width = 1;
	res.height = 1;
	res.xSeg = 1;
	res.ySeg = 1;
	res.numTexCoordSets = 1;
	res.uTile = 1;
	res.vTile = 1;
	res.up = Ogre::Vector3::UNIT_Y;
	if(elem->Attribute("normal"))
	{
			res.normal = Ogre::StringConverter::parseVector3(elem->Attribute("normal"));
	}
	if(elem->Attribute("constant"))
	{
			res.constant = Ogre::StringConverter::parseReal(elem->Attribute("constant"));
	}
	if(elem->Attribute("width"))
	{
			res.width = Ogre::StringConverter::parseReal(elem->Attribute("width"));
	}
	if(elem->Attribute("height"))
	{
			res.height = Ogre::StringConverter::parseReal(elem->Attribute("height"));
	}
	if(elem->Attribute("xSeg"))
	{
			res.xSeg = Ogre::StringConverter::parseInt(elem->Attribute("xSeg"));
	}
	if(elem->Attribute("ySeg"))
	{
			res.ySeg = Ogre::StringConverter::parseInt(elem->Attribute("ySeg"));
	}
	if(elem->Attribute("numTexCoordSets"))
	{
			res.numTexCoordSets = Ogre::StringConverter::parseInt(elem->Attribute("numTexCoordSets"));
	}
	if(elem->Attribute("uTile"))
	{
			res.uTile = Ogre::StringConverter::parseReal(elem->Attribute("uTile"));
	}
	if(elem->Attribute("vTile"))
	{
			res.vTile = Ogre::StringConverter::parseReal(elem->Attribute("vTile"));
	}
	if(elem->Attribute("upVector"))
	{
			res.up = Ogre::StringConverter::parseVector3(elem->Attribute("upVector"));
			if(res.up.dotProduct(res.normal) != 0)
			{
				//dh steht nicht senkrecht -> würfeln
				res.up = res.normal.perpendicular();
			}
	}
	return res;

}

CollisionData StandardApplication::parseCollisionData(TiXmlElement *XMLNode,bool processCompound)
{
	CollisionData res;
	res.collistionParams = Ogre::Vector3::UNIT_SCALE;
	res.colType = CT_NOCOLLISION;
//	res.jointType = JT_NONE;
	res.mass = 0;
	res.subBodyData.clear();
	res.upVector = Ogre::Vector3::ZERO;
	if(XMLNode->Attribute("type"))
	{
		Ogre::String type = XMLNode->Attribute("type");
		res.colType = stringToColType(type);
		//Ogre::StringUtil::toLowerCase(type);
		//if(type == "box")
		//	res.colType = CT_BOX;
		//else if(type == "capsule")
		//	res.colType = CT_CAPSULE;
		//else if(type == "champfercylinder")
		//	res.colType = CT_CHAMFERCYLINDER;
		//else if(type == "cone")
		//	res.colType = CT_CONE;
		//else if(type == "convexhull")
		//	res.colType = CT_CONVEXHULL;
		//else if(type == "cylinder")
		//	res.colType = CT_CYLINDER;
		//else if(type == "ellipsoid")
		//	res.colType = CT_ELLIPSOID;
		//else if(type == "pyramid")
		//	res.colType = CT_PYRAMID;
		//else if(type == "treecollision")
		//	res.colType = CT_TREECOLLISION;
		///*else if(type == "treecollisionscene")
		//	res.colType = CT_TREECOLLISIONSCENE;*/
		//else if(type == "compound")
		//	res.colType = CT_COMPOUND;
		//else if(type == "file")
		//	res.colType = CT_EXTERNAL;
		
		

		if(XMLNode->Attribute("params"))
		{
			res.collistionParams = Ogre::StringConverter::parseVector3(XMLNode->Attribute("params"));
			if(res.collistionParams == Ogre::Vector3::ZERO)
				res.collistionParams = Ogre::Vector3(Ogre::StringConverter::parseReal(XMLNode->Attribute("params")));
		}
		if(XMLNode->Attribute("mass"))
		{
			res.mass = Ogre::StringConverter::parseReal(XMLNode->Attribute("mass"));			
		}
		if(XMLNode->Attribute("filename"))
		{
			res.collisionFile = XMLNode->Attribute("filename");			
		}
		if(XMLNode->Attribute("orientation_offset"))
		{
			res.orientationOffset = Ogre::StringConverter::parseQuaternion(XMLNode->Attribute("orientation_offset"));			
		}
		if(XMLNode->Attribute("position_offset"))
		{
			res.positionOffset = Ogre::StringConverter::parseVector3(XMLNode->Attribute("position_offset"));			
		}
		if(processCompound)
		{
			TiXmlElement *sub = XMLNode->FirstChildElement("compound");
			while(sub)
			{
				res.subBodyData.push_back(parseCollisionData(sub,false));
				sub = sub->NextSiblingElement("compound");
			}
		}
		if(XMLNode->Attribute("upVector"))
		{
			res.upVector = Ogre::StringConverter::parseVector3(XMLNode->Attribute("upVector"));			
		}
	}
	return res;
}



void StandardApplication::collisionDataToXml(TiXmlElement *elem,CollisionData data)
{
	elem->SetAttribute("type",colTypeToString(data.colType).c_str());

	elem->SetAttribute("params",ogre_str(data.collistionParams).c_str());
	elem->SetAttribute("mass",ogre_str(data.mass).c_str());
	elem->SetAttribute("filename",data.collisionFile.c_str());
	elem->SetAttribute("orientationOffset",ogre_str(data.orientationOffset).c_str());
	elem->SetAttribute("positionOffset",ogre_str(data.positionOffset).c_str());
	elem->SetAttribute("upVector",ogre_str(data.upVector).c_str());
		

	if(!data.subBodyData.empty())
	{
		for(std::vector<CollisionData>::iterator itr = data.subBodyData.begin();itr!=data.subBodyData.end();itr++)
		{
			TiXmlElement *sub = new TiXmlElement("compound");
			collisionDataToXml(sub,*itr);
		}

	}
}

void StandardApplication::processCharData(TiXmlElement *XMLRoot)
{
 	TiXmlElement *XMLNode=XMLRoot->FirstChildElement("object");
	while(XMLNode)
	{
//		log("character parser: "+Ogre::String(XMLNode->Value()));
		if(!XMLNode->Attribute("id"))
			break;

		gamedata_char *cur = new gamedata_char();
		
		
		cur->id = XMLNode->Attribute("id");
		CollisionData myCD;
		myCD.collisionFile = "";
		myCD.collistionParams = Ogre::Vector3::UNIT_SCALE;
		myCD.colType = CT_ELLIPSOID;
		myCD.mass = 75;
		myCD.orientationOffset = Ogre::Quaternion::IDENTITY;
		myCD.positionOffset = Ogre::Vector3::ZERO;
		myCD.upVector = Ogre::Vector3::UNIT_Y;
		cur->cData = myCD;
		WorldArtData myWAD;
		myWAD.type = WT_NONE;
		myWAD.filename	= ERROR_MESH;
		myWAD.scale		= Ogre::Vector3::UNIT_SCALE;
		myWAD.material	= "";
		myWAD.positionOffset = Ogre::Vector3::ZERO;
		myWAD.orientationOffset = Ogre::Quaternion::IDENTITY;
		cur->wData = myWAD;
		
		//cur->collisionParams=Ogre::Vector3::UNIT_SCALE;
		//cur->collisionType=0;
		cur->flags=0;
		//cur->mass=0;
		cur->head_position=Ogre::Vector3::ZERO;

		
//		cur->size = Ogre::Vector3(0.2,1.75,0.2);
		cur->name=ERROR_NONAME;
		/*cur->run_factor=1;
		cur->walk_factor=1;*/

		/*filling the animation lists*/
		if(XMLNode->FirstChildElement("animation"))
		{
			TiXmlElement *anim = XMLNode->FirstChildElement("animation")->FirstChildElement("anim");
			while(anim)
			{
				if(!anim->Attribute("type") || !anim->Attribute("name"))
				{
					break;
				}
				if(Ogre::String(anim->Attribute("type")) == "attackmelee")
				{				
					AnimData curData;
					curData.animName = anim->Attribute("name");
					if(anim->Attribute("factor"))
						curData.factor = Ogre::StringConverter::parseReal(anim->Attribute("factor"));
					cur->anim_attackmelee.push_back(curData);
				}
				else if(Ogre::String(anim->Attribute("type")) == "attackranged")
				{				
					AnimData curData;
					curData.animName = anim->Attribute("name");
					if(anim->Attribute("factor"))
						curData.factor = Ogre::StringConverter::parseReal(anim->Attribute("factor"));
					cur->anim_attackranged.push_back(curData);
				}
				else if(Ogre::String(anim->Attribute("type")) == "cast")
				{
					AnimData curData;
					curData.animName = anim->Attribute("name");
					if(anim->Attribute("factor"))
						curData.factor = Ogre::StringConverter::parseReal(anim->Attribute("factor"));
					cur->anim_cast.push_back(curData);
				}
				else if(Ogre::String(anim->Attribute("type")) == "death")
				{
					AnimData curData;
					curData.animName = anim->Attribute("name");
					if(anim->Attribute("factor"))
						curData.factor = Ogre::StringConverter::parseReal(anim->Attribute("factor"));
					cur->anim_death.push_back(curData);
				}
				else if(Ogre::String(anim->Attribute("type")) == "fall")
				{
					AnimData curData;
					curData.animName = anim->Attribute("name");
					if(anim->Attribute("factor"))
						curData.factor = Ogre::StringConverter::parseReal(anim->Attribute("factor"));
					cur->anim_fall.push_back(curData);
				}
				else if(Ogre::String(anim->Attribute("type")) == "jump")
				{
					AnimData curData;
					curData.animName = anim->Attribute("name");
					if(anim->Attribute("factor"))
						curData.factor = Ogre::StringConverter::parseReal(anim->Attribute("factor"));
					cur->anim_jump.push_back(curData);
				}
				else if(Ogre::String(anim->Attribute("type")) == "idle")
				{
					AnimData curData;
					curData.animName = anim->Attribute("name");
					if(anim->Attribute("factor"))
						curData.factor = Ogre::StringConverter::parseReal(anim->Attribute("factor"));
					cur->anim_idle.push_back(curData);
				}
				else if(Ogre::String(anim->Attribute("type")) == "run")
				{
					AnimData curData;
					curData.animName = anim->Attribute("name");
					if(anim->Attribute("factor"))
						curData.factor = Ogre::StringConverter::parseReal(anim->Attribute("factor"));
					cur->anim_run.push_back(curData);
				}
				else if(Ogre::String(anim->Attribute("type")) == "walk")
				{
					AnimData curData;
					curData.animName = anim->Attribute("name");
					if(anim->Attribute("factor"))
						curData.factor = Ogre::StringConverter::parseReal(anim->Attribute("factor"));
					cur->anim_walk.push_back(curData);
				}
				
				anim = anim->NextSiblingElement("anim");
			}
		}

		if( XMLNode->FirstChildElement("worldArt"))
		{
			cur->wData = parseWorldArtData(XMLNode->FirstChildElement("worldArt"));
			//cur->mesh = XMLNode->Attribute("mesh");
			//settext("juhu");
		}
		if( XMLNode->FirstChildElement("collision"))
		{
			cur->cData = parseCollisionData(XMLNode->FirstChildElement("collision"));
			//cur->mesh = XMLNode->Attribute("mesh");
			//settext("juhu");
		}
		if( XMLNode->FirstChildElement("inventory"))
		{
			cur->defaultInventory = new Inventory(1);
			parseInventory(XMLNode->FirstChildElement("inventory"),cur->defaultInventory );
			//cur->mesh = XMLNode->Attribute("mesh");
			//settext("juhu");
		}
		if( XMLNode->FirstChildElement("spellbook"))
		{
			cur->defaultSpellbook = new Spellbook();
			parseSpellbook(XMLNode->FirstChildElement("spellbook"),cur->defaultSpellbook);
			
		}
		if( XMLNode->FirstChildElement("stats"))
		{
			cur->stats = parseStats(XMLNode->FirstChildElement("stats")->FirstChildElement());
			
		}
		if( XMLNode->FirstChildElement("equipment"))
		{
			TiXmlElement *eq = XMLNode->FirstChildElement("equipment");
			if(eq)
			{
				Ogre::String melee = "";
				Ogre::String ranged = "";
				Ogre::String armor = "";
				Ogre::String shield = "";
				Ogre::String else1 = "";
				Ogre::String else2 = "";
				Ogre::String else3 = "";
				Ogre::String else4 = "";
				Ogre::String bag   = "";
				if(eq->Attribute("melee"))
				{
					melee = eq->Attribute("melee");
				}				
				if(eq->Attribute("ranged"))
				{
					ranged = eq->Attribute("ranged");
				}				
				if(eq->Attribute("armor"))
				{
					armor = eq->Attribute("armor");
				}				
				if(eq->Attribute("shield"))
				{
					shield = eq->Attribute("shield");
				}				
				if(eq->Attribute("else1"))
				{
					else1 = eq->Attribute("else1");
				}				
				if(eq->Attribute("else2"))
				{
					else2 = eq->Attribute("else2");
				}				
				if(eq->Attribute("else3"))
				{
					else3 = eq->Attribute("else3");
				}				
				if(eq->Attribute("else4"))
				{
					else4 = eq->Attribute("else4");
				}		
				if(eq->Attribute("bag"))
				{
					bag = eq->Attribute("bag");					
				}
						
				cur->defaultMelee= getItemDataPtr(melee);
				cur->defaultArmor= getItemDataPtr(armor);
				cur->defaultRanged= getItemDataPtr(ranged);
				cur->defaultShield= getItemDataPtr(shield);
				cur->defaultElse1= getItemDataPtr(else1);
				cur->defaultElse2= getItemDataPtr(else2);
				cur->defaultElse3= getItemDataPtr(else3);
				cur->defaultElse4= getItemDataPtr(else4);
				cur->defaultBag	= getItemDataPtr("container_"+bag);
				

			}
		}

	
		if(XMLNode->Attribute("flags"))
		{
			cur->flags=Ogre::StringConverter::parseInt(XMLNode->Attribute("flags"));
		}
		if(XMLNode->Attribute("head_position"))
		{
			cur->head_position=Ogre::StringConverter::parseVector3(XMLNode->Attribute("head_position"));
		}
		if(XMLNode->Attribute("move_acceleration"))
		{
			cur->movAccel=Ogre::StringConverter::parseReal(XMLNode->Attribute("move_acceleration"));
		}
		if(XMLNode->Attribute("stepSound"))
		{
			cur->stepSound=XMLNode->Attribute("stepSound");
		}
		if(XMLNode->Attribute("jumpSound"))
		{
			cur->jumpSound=XMLNode->Attribute("jumpSound");
		}
		if(XMLNode->Attribute("landSound"))
		{
			cur->landSound=XMLNode->Attribute("landSound");
		}
		if(XMLNode->Attribute("stepSoundWaitWalk"))
		{
			cur->stepSoundWaitWalk= Ogre::StringConverter::parseReal(XMLNode->Attribute("stepSoundWaitWalk"));
		}
		if(XMLNode->Attribute("stepSoundWaitRun"))
		{
			cur->stepSoundWaitRun= Ogre::StringConverter::parseReal(XMLNode->Attribute("stepSoundWaitRun"));
		}
	
		if( XMLNode->Attribute("name"))
		{
			cur->name = XMLNode->Attribute("name");
			//settext("juhu");
		}
	
		
		registerGameData(cur);
		XMLNode = XMLNode->NextSiblingElement("object");
	}
}

void StandardApplication::processItemData(TiXmlElement *XMLRoot)
{
	TiXmlElement *XMLNode=XMLRoot->FirstChildElement("object");
	while(XMLNode)
	{
		if(!XMLNode->Attribute("id"))
			break;
		gamedata_item *cur = new gamedata_item();
		
		cur->id = XMLNode->Attribute("id");
		cur->name=ERROR_NONAME;
		cur->inv_image=ERROR_INVIMAGE;
		cur->Explosion = NULL;

		CollisionData myCD;
		myCD.collisionFile = "";
		myCD.collistionParams = Ogre::Vector3::UNIT_SCALE;
		myCD.colType = CT_NOCOLLISION;
		myCD.mass = 0;
		myCD.orientationOffset = Ogre::Quaternion::IDENTITY;
		myCD.positionOffset = Ogre::Vector3::ZERO;
		myCD.upVector = Ogre::Vector3::ZERO;

		WorldArtData myWAD;
		myWAD.filename	= ERROR_MESH;
		myWAD.scale		= Ogre::Vector3::UNIT_SCALE;
		myWAD.material	= "";
		myWAD.positionOffset = Ogre::Vector3::ZERO;
		myWAD.orientationOffset = Ogre::Quaternion::IDENTITY;
		myWAD.type = WT_MESH;
		cur->wData = myWAD;
		cur->meleeSound = "";
		cur->rangedSound = "";
		/*cur->collisionParams=Ogre::Vector3::UNIT_SCALE;
		cur->collisionType=0;
		cur->flags=0;
		cur->mass=0;*/
		if( XMLNode->FirstChildElement("worldArt"))
		{
			cur->wData = parseWorldArtData(XMLNode->FirstChildElement("worldArt"));
		}
		if( XMLNode->FirstChildElement("collision"))
		{
			cur->cData = parseCollisionData(XMLNode->FirstChildElement("collision"));
		}
		
	
		if( XMLNode->Attribute("inv_image"))
		{
			cur->inv_image = XMLNode->Attribute("inv_image");				
		}
		if( XMLNode->Attribute("name"))
		{
			cur->name = XMLNode->Attribute("name");				
		}
		if( XMLNode->Attribute("meleeCooldown"))
		{
			cur->meleeCooldown =  Ogre::StringConverter::parseReal(XMLNode->Attribute("meleeCooldown"));				
		}
		if( XMLNode->Attribute("meleeSound"))
		{
			cur->meleeSound =  XMLNode->Attribute("meleeSound");
		}
		if( XMLNode->Attribute("rangedSound"))
		{
			cur->rangedSound =  XMLNode->Attribute("rangedSound");				
		}
		if( XMLNode->Attribute("useSound"))
		{
			cur->useSound =  XMLNode->Attribute("useSound");				
		}
		if( XMLNode->Attribute("rangedCooldown"))
		{
			cur->rangedCooldown =  Ogre::StringConverter::parseReal(XMLNode->Attribute("rangedCooldown"));			
		}
		
		TiXmlElement *flags = XMLNode->FirstChildElement();
		while(flags)
		{
			Ogre::String type = flags->Value();
			Ogre::StringUtil::toLowerCase(type);
			//Ogre::String name = flags->Attribute("name");
			//Ogre::String value= flags->Attribute("value");
			if(type == "equip")
			{
				if(flags->Attribute("melee"))
				{
					if(Ogre::StringConverter::parseBool(flags->Attribute("melee")))					
						cur->equipFlags += EF_MELEE;
					
				}
				if(flags->Attribute("ranged"))
				{
					if(Ogre::StringConverter::parseBool(flags->Attribute("ranged")))					
						cur->equipFlags += EF_RANGED;
					
				}
				if(flags->Attribute("armor"))
				{
					if(Ogre::StringConverter::parseBool(flags->Attribute("armor")))					
						cur->equipFlags += EF_ARMOR;
					
				}
				if(flags->Attribute("shield"))
				{
					if(Ogre::StringConverter::parseBool(flags->Attribute("shield")))					
						cur->equipFlags += EF_SHIELD;
					
				}
				if(flags->Attribute("else"))
				{
					if(Ogre::StringConverter::parseBool(flags->Attribute("else")))					
						cur->equipFlags += EF_ELSE;
					
				}
			}
			else if(type == "property")
			{
				if(flags->Attribute("nodrop"))
				{
					if(Ogre::StringConverter::parseBool(flags->Attribute("nodrop")))					
						cur->propertyFlags += PF_NODROP;
					
				}
				if(flags->Attribute("mixable"))
				{
					if(Ogre::StringConverter::parseBool(flags->Attribute("mixable")))					
						cur->propertyFlags += PF_MIXABLE;
					
				}
				if(flags->Attribute("usable"))
				{
					if(Ogre::StringConverter::parseBool(flags->Attribute("usable")))					
						cur->propertyFlags += PF_USABLE;
					
				}
				if(flags->Attribute("secondary"))
				{
					if(Ogre::StringConverter::parseBool(flags->Attribute("secondary")))					
						cur->propertyFlags += PF_SECONDARY;
					
				}
				if(flags->Attribute("lose_on_ranged"))
				{
					if(Ogre::StringConverter::parseBool(flags->Attribute("lose_on_ranged")))					
						cur->propertyFlags += PF_LOSE_ON_RANGED;
					
				}
				if(flags->Attribute("lose_on_melee"))
				{
					if(Ogre::StringConverter::parseBool(flags->Attribute("lose_on_melee")))					
						cur->propertyFlags += PF_LOSE_ON_MELEE;
					
				}
				if(flags->Attribute("lose_on_use"))
				{
					if(Ogre::StringConverter::parseBool(flags->Attribute("lose_on_use")))					
						cur->propertyFlags += PF_LOSE_ON_USE;
					
				}
				/*if(flags->Attribute("bag_space"))
				{					
					cur->bagSpace = Ogre::StringConverter::parseInt(flags->Attribute("bag_space"));					
				}*/
			}
			else if(type == "secondary")
			{
				cur->secondaryItem = flags->Attribute("id");
				cur->secondaryItemPtr = getItemDataPtr(cur->secondaryItem); //es gibt NULL zurück, wenn es nix findet
				Ogre::String sectype = flags->Attribute("type");
				if(sectype == "melee")
					cur->secondaryType = EF_MELEE;
				else if(sectype == "ranged")
					cur->secondaryType = EF_RANGED;
				else if(sectype == "armor")
					cur->secondaryType = EF_ARMOR;
				else if(sectype == "shield")
					cur->secondaryType = EF_SHIELD;
				else if(sectype == "else")
					cur->secondaryType = EF_ELSE;
			}
			else if(type == "equipspell")
			{
				//COMPILE PREVENTER
				//cur->equipEffects
				//<equipspell id="muh" />
				Ogre::String spellID = flags->Attribute("id");
				cur->equipSpell = getSpellDataPtr(spellID);
				
				//SpellEffectData eff = parseSpellEffectData(flags);
				//cur->equipEffects.push_back(eff);
				/*if(!cur->equipEffect)
					cur->equipEffect = new SpellEffectData();*/
				//TiXmlElement *stat = flags->FirstChildElement();
				//*cur->equipEffect = parseSpellEffectData(flags);
				
			}
			else if(type == "rangedspell")
			{
				//COMPILE PREVENTER
				//if(!cur->rangedDamage)
				//	cur->rangedDamage = new Damage();
				////TiXmlElement *stat = flags->FirstChildElement();
				//*cur->rangedDamage = parseDamageData(flags);
				//if(!cur->rangedEffect)
				//	cur->rangedEffect = new SpellEffectData();
				//TiXmlElement *stat = flags->FirstChildElement();
				//*cur->rangedEffect = parseSpellEffectData(flags);
				//SpellEffectData eff = parseSpellEffectData(flags);
				//cur->rangedEffects.push_back(eff);
				Ogre::String spellID = flags->Attribute("id");
				cur->rangedSpell = getSpellDataPtr(spellID);
				
			}
			else if(type == "meleespell")
			{
				//COMPILE PREVENTER
				//if(!cur->meleeDamage)
				//	cur->meleeDamage = new Damage();
				////TiXmlElement *stat = flags->FirstChildElement();
				//*cur->meleeDamage = parseDamageData(flags);	
				//if(!cur->meleeEffect)
				//	cur->meleeEffect = new SpellEffectData();
				//TiXmlElement *stat = flags->FirstChildElement();
				//*cur->meleeEffect = parseSpellEffectData(flags);
				//SpellEffectData eff = parseSpellEffectData(flags);
				//cur->meleeEffects.push_back(eff);
				Ogre::String spellID = flags->Attribute("id");
				cur->meleeSpell = getSpellDataPtr(spellID);
			}
			else if(type == "equipstats")
			{
				//COMPILE PREVENTER
				//if(!cur->meleeDamage)
				//	cur->meleeDamage = new Damage();
				////TiXmlElement *stat = flags->FirstChildElement();
				//*cur->meleeDamage = parseDamageData(flags);	
				//if(!cur->meleeEffect)
				//	cur->meleeEffect = new SpellEffectData();
				//TiXmlElement *stat = flags->FirstChildElement();
				//*cur->meleeEffect = parseSpellEffectData(flags);
				//SpellEffectData eff = parseSpellEffectData(flags);
				//cur->meleeEffects.push_back(eff);
				parseStatChangeList(flags,cur->equipStats);
			}
			else if(type == "usespell")
			{
				Ogre::String spellID = flags->Attribute("id");
				cur->useSpell = getSpellDataPtr(spellID);
			}
			else if(type == "meleedamage")
			{
				cur->meleeDamage = parseDamageData(flags);
			}
			else if(type == "rangeddamage")
			{
				cur->rangedDamage = parseDamageData(flags);
			}
			else if(type == "missiledata")
			{
				
				
				
				if(!cur->Missile)
				{
					MissileData *d = new MissileData();
					*d = parseMissileData(flags);
					cur->Missile = d;
					
					//cur->Missile = new MissileData();
				}
				
				//TiXmlElement *stat = flags->FirstChildElement();
				
				//*cur->Missile = parseMissileData(flags);				
			}	
			else if(type == "explosion")
			{
				if(!cur->Explosion)
				{
					ExplosionData *d = new ExplosionData();
					*d = parseExplosionData(flags);
					cur->Explosion = d;
					
					//cur->Missile = new MissileData();
				}
				
			}	
			else if(type == "beam")
			{
				if(!cur->Beam)
				{
					BeamData *d = new BeamData();
					*d = parseBeamData(flags);
					cur->Beam = d;
					
					//cur->Missile = new MissileData();
				}
				
			}
			flags = flags->NextSiblingElement();
		}
	//	mLog("item "+cur->id+" geladen. material="+cur->material);
		registerGameData(cur);
		XMLNode = XMLNode->NextSiblingElement("object");
	}
}


void StandardApplication::worldArtToXml(WorldArtData wData, TiXmlElement *nod)
{
	//TiXmlElement * res = new TiXmlElement( "worldArt" );
	Ogre::String type = "";
	switch(wData.type)
	{
	case WT_MESH:
		type = "mesh";
		nod->SetAttribute("filename",wData.filename.c_str());
		if(wData.material != "")
			nod->SetAttribute("material",wData.material.c_str());
		
		break;
	case WT_DOTSCENE:
		type = "dotscene";
		nod->SetAttribute("filename",wData.filename.c_str());
		break;
	case WT_BILLBOARD:
		type = "billboard";
		nod->SetAttribute("material",wData.material.c_str());
		if(wData.bbColor != Ogre::ColourValue::White)
			nod->SetAttribute("billboard_color",ogre_str(wData.bbColor).c_str());
		break;
	case WT_PARTICLESYSTEM:
		type = "particle";
		nod->SetAttribute("name",wData.material.c_str());
		break;
	
	case WT_PLANE:
		type = "plane";
		nod->SetAttribute("normal",ogre_str(wData.pData.normal).c_str());
		nod->SetAttribute("constant",ogre_str(wData.pData.constant).c_str());
		nod->SetAttribute("width",ogre_str(wData.pData.width).c_str());
		nod->SetAttribute("height",ogre_str(wData.pData.height).c_str());
		nod->SetAttribute("xSeg",ogre_str(wData.pData.xSeg).c_str());
		nod->SetAttribute("ySeg",ogre_str(wData.pData.ySeg).c_str());
		nod->SetAttribute("numTexCoordSets",ogre_str(wData.pData.numTexCoordSets).c_str());
		nod->SetAttribute("uTile",ogre_str(wData.pData.uTile).c_str());
		nod->SetAttribute("vTile",ogre_str(wData.pData.vTile).c_str());
		nod->SetAttribute("upVector",ogre_str(wData.pData.up).c_str());
		
		if(wData.material != "")
			nod->SetAttribute("material",wData.material.c_str());
		break;
	case WT_BILLBOARDCHAIN:
		type = "billboardchain";
	case WT_RIBBONTRAIL:
		if(type == "")
			type = "ribbontrail";
		nod->SetAttribute("dynamic",ogre_str(wData.bbcData.dynamic).c_str());
		nod->SetAttribute("maxElements",ogre_str(wData.bbcData.maxElements).c_str());
		nod->SetAttribute("numberOfChains",ogre_str(wData.bbcData.numberOfChains).c_str());
		nod->SetAttribute("useColours",ogre_str(wData.bbcData.useColours).c_str());
		nod->SetAttribute("useTextureCoords",ogre_str(wData.bbcData.useTextureCoords).c_str());
		
		
		
		nod->SetAttribute("material",wData.material.c_str());
		break;
	
	}
	nod->SetAttribute("type",type.c_str());
	if(wData.positionOffset != Ogre::Vector3::ZERO)
		nod->SetAttribute("position_offset",ogre_str(wData.positionOffset).c_str());
	if(wData.orientationOffset != Ogre::Quaternion::IDENTITY)
		nod->SetAttribute("orientation_offset",ogre_str(wData.orientationOffset).c_str());
	nod->SetAttribute("scale",ogre_str(wData.scale));
	/*if(wData.scale != Ogre::Vector3::UNIT_SCALE)
		nod->SetAttribute("scale",ogre_str(wData.scale).c_str());*/
	//nod->SetValue(type.c_str());
	//return res;
	
}


Stats StandardApplication::parseStats(TiXmlElement *elem)
{
	Stats cur;
	while(elem)
	{
		Ogre::String name = elem->Value(); //ROFL
		Ogre::String value= elem->Attribute("value");
		Ogre::StringUtil::toLowerCase(name);
		if(name == "strength")
		{

			cur.mStats[st_strength] = Ogre::StringConverter::parseInt(value);
		}
		else if(name == "agility")
		{
			cur.mStats[st_agility] = Ogre::StringConverter::parseInt(value);
		}
		else if(name == "magic")
		{
			cur.mStats[st_magic] = Ogre::StringConverter::parseInt(value);
		}
		else if(name == "maxhp")
		{
			cur.mStats[st_maxHP] = Ogre::StringConverter::parseInt(value);
		}
		else if(name == "maxmp")
		{
			cur.mStats[st_maxMP] = Ogre::StringConverter::parseInt(value);
		}
		else if(name == "hitpoints")
		{
			cur.hitpoints = Ogre::StringConverter::parseInt(value);
		}
		else if(name == "mana")
		{
			cur.mana = Ogre::StringConverter::parseInt(value);
		}
		else if(name == "ancientmagic")
		{
			cur.ancientMagic = Ogre::StringConverter::parseInt(value);
		}
		else if(name == "resistblunt")
		{
			cur.mStats[st_resistBlunt] = Ogre::StringConverter::parseReal(value);
		}
		else if(name == "resistcut")
		{
			cur.mStats[st_resistCut] = Ogre::StringConverter::parseReal(value);
		}
		else if(name == "resistpierce")
		{
			cur.mStats[st_resistPierce] = Ogre::StringConverter::parseReal(value);
		}
		else if(name == "resistfire")
		{
			cur.mStats[st_resistFire] = Ogre::StringConverter::parseReal(value);
		}
		else if(name == "resistice")
		{
			cur.mStats[st_resistIce] = Ogre::StringConverter::parseReal(value);
		}
		else if(name == "resistspark")
		{
			cur.mStats[st_resistSpark] = Ogre::StringConverter::parseReal(value);
		}
		else if(name == "resistpoison")
		{
			cur.mStats[st_resistPoison] = Ogre::StringConverter::parseReal(value);
		}
		else if(name == "resistdark")
		{
			cur.mStats[st_resistDark] = Ogre::StringConverter::parseReal(value);
		}
		else if(name == "resistlight")
		{
			cur.mStats[st_resistLight] = Ogre::StringConverter::parseReal(value);
		}
		else if(name == "hpregenrate")
		{
			cur.mStats[st_hpRegenRate] = Ogre::StringConverter::parseReal(value);
		}
		else if(name == "manaregenrate")
		{
			cur.mStats[st_manaRegenRate] = Ogre::StringConverter::parseReal(value);
		}
		else if(name == "walkspeed")
		{
			cur.mStats[st_walkSpeed] = Ogre::StringConverter::parseReal(value);
		}
		else if(name == "runspeed")
		{
			cur.mStats[st_runSpeed] = Ogre::StringConverter::parseReal(value);
		}
		/*if(name == "strength")
		{

			cur.strength = Ogre::StringConverter::parseInt(value);
		}
		else if(name == "agility")
		{
			cur.agility = Ogre::StringConverter::parseInt(value);
		}
		else if(name == "magic")
		{
			cur.magic = Ogre::StringConverter::parseInt(value);
		}
		else if(name == "maxHP")
		{
			cur.maxHP = Ogre::StringConverter::parseInt(value);
		}
		else if(name == "maxMP")
		{
			cur.maxMP = Ogre::StringConverter::parseInt(value);
		}
		else if(name == "hitpoints")
		{
			cur.hitpoints = Ogre::StringConverter::parseInt(value);
		}
		else if(name == "mana")
		{
			cur.mana = Ogre::StringConverter::parseInt(value);
		}
		else if(name == "resistBlunt")
		{
			cur.resistBlunt = Ogre::StringConverter::parseReal(value);
		}
		else if(name == "resistCut")
		{
			cur.resistCut = Ogre::StringConverter::parseReal(value);
		}
		else if(name == "resistPierce")
		{
			cur.resistPierce = Ogre::StringConverter::parseReal(value);
		}
		else if(name == "resistFire")
		{
			cur.resistFire = Ogre::StringConverter::parseReal(value);
		}
		else if(name == "resistIce")
		{
			cur.resistIce = Ogre::StringConverter::parseReal(value);
		}
		else if(name == "resistSpark")
		{
			cur.resistSpark = Ogre::StringConverter::parseReal(value);
		}
		else if(name == "resistPoison")
		{
			cur.resistPoison = Ogre::StringConverter::parseReal(value);
		}
		else if(name == "resistDark")
		{
			cur.resistDark = Ogre::StringConverter::parseReal(value);
		}
		else if(name == "resistLight")
		{
			cur.resistLight = Ogre::StringConverter::parseReal(value);
		}
		else if(name == "hpRegenRate")
		{
			cur.hpRegenRate = Ogre::StringConverter::parseReal(value);
		}
		else if(name == "manaRegenRate")
		{
			cur.manaRegenRate = Ogre::StringConverter::parseReal(value);
		}*/

		elem = elem->NextSiblingElement();
	}
	return cur;
}


ChStat StandardApplication::parseChStats(TiXmlElement *elem)
{
	
	//<resistBlunt value="0.75" mode="multiply" />
	ChStat cur;
	cur.op = opNone;
	cur.value = 0;
	Ogre::String name = elem->Value(); 
	Ogre::String value;
	if(elem->Attribute("value"))
		value = elem->Attribute("value");
	Ogre::String op;
	if(elem->Attribute("operator"))
		op = elem->Attribute("operator");
	cur.value = Ogre::StringConverter::parseReal(value);
	Ogre::StringUtil::toLowerCase(name);
	Ogre::StringUtil::toLowerCase(op);
	if(name == "strength")
	{
		cur.type = st_strength;
		cur.op = opAdd;
	}  
	else if(name == "agility")
	{
		cur.type = st_agility;
		cur.op = opAdd;
	} 
	else if(name == "magic")
	{
		cur.type = st_magic;
		cur.op = opAdd;
	} 
	else if(name == "maxhp")
	{
		cur.type = st_maxHP;
		cur.op = opAdd;
	} 
	else if(name == "maxmp")
	{
		cur.type = st_maxMP;
		cur.op = opAdd;
	} 	
	else if(name == "resistblunt")
	{
		cur.type = st_resistBlunt;
		cur.op = opMultiply;
	} 	
	else if(name == "resistcut")
	{
		cur.type = st_resistCut;
		cur.op = opMultiply;
	} 		
	else if(name == "resistpierce")
	{
		cur.type = st_resistPierce;
		cur.op = opMultiply;
	} 	
	else if(name == "resistfire")
	{
		cur.type = st_resistFire;
		cur.op = opMultiply;
	} 	
	else if(name == "resistice")
	{
		cur.type = st_resistIce;
		cur.op = opMultiply;
	} 		
	else if(name == "resistspark")
	{
		cur.type = st_resistSpark;
		cur.op = opMultiply;
	} 
	else if(name == "resistpoison")
	{
		cur.type = st_resistPoison;
		cur.op = opMultiply;
	} 
	else if(name == "resistdark")
	{
		cur.type = st_resistDark;
		cur.op = opMultiply;
	} 
	else if(name == "resistlight")
	{
		cur.type = st_resistLight;
		cur.op = opMultiply;
	} 
	else if(name == "hpregenrate")
	{
		cur.type = st_hpRegenRate;
		cur.op = opMultiply;
	} 
	else if(name == "manaregenrate")
	{
		cur.type = st_manaRegenRate;
		cur.op = opMultiply;
	} 
	else if(name == "walkspeed")
	{
		cur.type = st_walkSpeed;
		cur.op = opAdd;
	} 
	else if(name == "runspeed")
	{
		cur.type = st_runSpeed;
		cur.op = opAdd;
	} 
	//elem = elem->FirstChildElement();
	
	if(op == "add")
	{
		cur.op = opAdd;
	}
	else if(op == "multiply")
	{
		cur.op = opMultiply;
	}
	else if(op == "set")
	{
		cur.op = opSet;
	}
	
	return cur;
}


TiXmlElement *StandardApplication::chStatsToXml(ChStat stat)
{
	
	//<resistBlunt value="0.75" mode="multiply" />
	
	TiXmlElement *res = new TiXmlElement("");
	res->SetAttribute("value",ogre_str(stat.value).c_str());
	switch(stat.type)
	{
    case st_strength:
		res->SetValue("strength");
		break;
	case st_agility:
		res->SetValue("agility");
		break;
	case st_magic:
		res->SetValue("magic");
		break;
	case st_maxHP:
		res->SetValue("maxHP");
		break;
	case st_maxMP:
		res->SetValue("maxMP");
		break;
	case st_resistBlunt:
		res->SetValue("resistBlunt");
		break;
	case st_resistCut:
		res->SetValue("resistCut");
		break;
	case st_resistPierce:
		res->SetValue("resistPierce");
		break;
	case st_resistFire:
		res->SetValue("resistFire");
		break;
	case st_resistIce:
		res->SetValue("resistIce");
		break;
	case st_resistSpark:
		res->SetValue("resistSpark");
		break;
	case st_resistPoison:
		res->SetValue("resistPoison");
		break;
	case st_resistDark:
		res->SetValue("resistDark");
		break;
	case st_resistLight:
		res->SetValue("resistLight");
		break;
	case st_hpRegenRate:
		res->SetValue("hpRegenRate");
		break;
	case st_manaRegenRate:
		res->SetValue("manaRegenRate");
		break;
	case st_walkSpeed:
		res->SetValue("walkSpeed");
		break;
	case st_runSpeed:
		res->SetValue("runSpeed");
		break;
	}
	switch(stat.op)
	{
	case opAdd:
		res->SetAttribute("mode","add");
		break;
	case opMultiply:
		res->SetAttribute("mode","multiply");
		break;
	case opSet:
		res->SetAttribute("mode","set");
		break;

	}
	
	
	return res;
}

Damage StandardApplication::parseDamageData(TiXmlElement *elem)
{
	Damage cur;
	//Ogre::String name = elem->Value(); //ROFL
	//Ogre::StringUtil::toLowerCase(name);
	
	elem = elem->FirstChildElement();	
	while(elem)
	{
		Ogre::String name = elem->Value(); //ROFL
		Ogre::StringUtil::toLowerCase(name);
		Ogre::String value= elem->Attribute("value");
		if(name == "hitblunt")
		{
			cur.hitBlunt = Ogre::StringConverter::parseInt(value);
		}
		else if(name == "hitcut")
		{
			cur.hitCut = Ogre::StringConverter::parseInt(value);
		}
		else if(name == "hitpierce")
		{
			cur.hitPierce = Ogre::StringConverter::parseInt(value);
		}
		else if(name == "fire")
		{
			cur.fire = Ogre::StringConverter::parseInt(value);
		}
		else if(name == "ice")
		{
			cur.ice = Ogre::StringConverter::parseInt(value);
		}
		else if(name == "spark")
		{
			cur.spark = Ogre::StringConverter::parseInt(value);
		}
		else if(name == "poison")
		{
			cur.poison = Ogre::StringConverter::parseInt(value);
		}
		else if(name == "dark")
		{
			cur.dark = Ogre::StringConverter::parseInt(value);
		}
		else if(name == "light")
		{
			cur.light = Ogre::StringConverter::parseInt(value);
		}
		elem = elem->NextSiblingElement();
	}
	return cur;
}


void StandardApplication::damageDataToXml(TiXmlElement *elem, Damage dmg)
{
	TiXmlElement *hitBluntElem = new TiXmlElement("hitBlunt");
	hitBluntElem->SetAttribute("value",ogre_str(dmg.hitBlunt).c_str());
	elem->LinkEndChild(hitBluntElem);
	TiXmlElement *hitCutElem = new TiXmlElement("hitCut");
	hitCutElem->SetAttribute("value",ogre_str(dmg.hitCut).c_str());
	elem->LinkEndChild(hitCutElem);
	TiXmlElement *hitPierceElem = new TiXmlElement("hitPierce");
	hitPierceElem->SetAttribute("value",ogre_str(dmg.hitPierce).c_str());
	elem->LinkEndChild(hitPierceElem);
	TiXmlElement *fireElem = new TiXmlElement("fire");
	fireElem->SetAttribute("value",ogre_str(dmg.fire).c_str());
	elem->LinkEndChild(fireElem);
	TiXmlElement *iceElem = new TiXmlElement("ice");
	iceElem->SetAttribute("value",ogre_str(dmg.ice).c_str());
	elem->LinkEndChild(iceElem);
	TiXmlElement *sparkElem = new TiXmlElement("spark");
	sparkElem->SetAttribute("value",ogre_str(dmg.spark).c_str());
	elem->LinkEndChild(sparkElem);
	TiXmlElement *poisonElem = new TiXmlElement("poison");
	poisonElem->SetAttribute("value",ogre_str(dmg.poison).c_str());
	elem->LinkEndChild(poisonElem);
	TiXmlElement *darkElem = new TiXmlElement("dark");
	darkElem->SetAttribute("value",ogre_str(dmg.dark).c_str());
	elem->LinkEndChild(darkElem);
	TiXmlElement *lightElem = new TiXmlElement("light");
	lightElem->SetAttribute("value",ogre_str(dmg.light).c_str());
	elem->LinkEndChild(lightElem);
}

//SimpleSound::Source *StandardApplication::createSource(int type, bool loop, bool relativeToListener, bool stream)
//{
//	SimpleSound::Source *res = soundMgr->createSource();
//	if(!res)
//		return NULL;
//	res->setType(type);
//	res->setLoop(loop);
//	res->setRelativeToListener(relativeToListener);
//	Ogre::Real gain = gainSFX;
//	switch(type)
//	{
//	case stMusic:
//		gain = gainMusic;
//		break;
//	case stSpeech:
//		gain = gainSpeech;
//		break;
//	case stSfxPlayer:
//		gain = gainSFXPlayer;
//		break;
//	};
//	res->setGain(gain);
//	/*res->setType(type);
//	res->setMaxDistance(maxDistance);
//	res->setRolloffFactor(rollofFactor);
//	res->setReferenceDistance(refDistance);*/
//	return res;
//}

SimpleSound::Source *StandardApplication::createSound(SimpleSound::Buffer *buffer, int type, bool loop, bool relativeToListener, bool stream)
{
	if(!buffer || ! soundMgr)
		return NULL;
	SimpleSound::Source *res = createSource(type,loop,relativeToListener,stream);
	if(!res)
		return NULL;
	res->setBuffer(buffer);
	/*Ogre::Real gain = gainSFX;
	switch(type)
	{
	case stMusic:
		gain = gainMusic;
		break;
	case stSpeech:
		gain = gainSpeech;
		break;
	case stSfxPlayer:
		gain = gainSFXPlayer;
		break;
	};
	res->setGain(gain);*/
	/*res->setType(type);
	res->setMaxDistance(maxDistance);
	res->setRolloffFactor(rollofFactor);
	res->setReferenceDistance(refDistance);*/
	return res;
	//setDistanceValues(maxDistance,rollofFactor,refDistance);

}
SimpleSound::Source *StandardApplication::createSound(Ogre::String filename, int type, bool loop, bool relativeToListener, bool stream, BufferType bType)
{
	using namespace SimpleSound;
	if(filename == "" || !soundMgr)
		return NULL;
	//versuche, nen buffer zu erstellen
	Buffer *buff = soundMgr->getBuffer(filename);
	if(!buff)
		return NULL;
	buff->setType(bType);
	
	return createSound(buff,type,loop,relativeToListener,stream);
}

void StandardApplication::destroySound(SimpleSound::Source *snd)
{
	soundMgr->destroySource(snd);
}

Ogre::Vector2 StandardApplication::getScreenSize()
{
	return Ogre::Vector2(mWindow->getWidth(),mWindow->getHeight());
}

//OgreAL::Sound *StandardApplication::createSound(Ogre::String filename, SoundType type, bool loop, bool stream, bool relativeToListener)
//{
//	if(filename == "")
//		return NULL;
//	int i = 0;
//	Ogre::String name = filename;
//	while(soundMgr->hasSound(name))
//	{
//		name = filename+ogre_str(i++);
//	}
////	Ogre::String name = ogre_str(soundinc++);
//	OgreAL::Sound *res = soundMgr->createSound(name,filename,loop,stream);
////	static_cast<TestSound*>(res)->printLog();
//	Ogre::Real gain = gainSFX;
//	switch(type)
//	{
//	case stMusic:
//		gain = gainMusic;
//		break;
//	case stSpeech:
//		gain = gainSpeech;
//		break;
//	case stSfxPlayer:
//		gain = gainSFXPlayer;
//		break;
//	};
//	res->setGain(gain);
//	res->setDistanceValues(maxDistance,rollofFactor,refDistance);
//	/**
//		 * Sets the reference distance.
//		 * @param refDistance The reference distance used in attenuation calculations.
//		 * @note This is the distance under which the volume for the
//		 *     source would normally drop by half
//		 * @note Negative values will be ignored
//		 */
//	/**
//		 * Sets the rolloff factor.
//		 * @param rolloffFactor The rolloff rate for the source 
//		 * @note Used for distance attenuation
//		 * @note Negative values will be ignored
//		 */
//	/**
//		 * Sets the Max Distance.
//		 * @param maxDistance The max distance used in the Inverse Clamped Distance Model
//		 * @note This is the distance where there will be no further attenuation of the source
//		 * @note Negative values will be ignored
//		 */
//	res->setRelativeToListener(relativeToListener);
//	/*res->play();
//	res->stop();*/
//	//soundLoadBuffer(res);
//	return res;
//}

//void StandardApplication::destroySound(OgreAL::Sound *snd)
//{
//	
//	if(!snd)
//		return;
//	
//	snd->stop();
//	if(snd->isAttached())
//	{		
//		snd->getParentSceneNode()->detachObject(snd);		
//	}
//	OgreAL::SoundManager::getSingletonPtr()->destroySound(snd);
//}

ExplosionData StandardApplication::parseExplosionData(TiXmlElement *elem)
{
	ExplosionData cur;
//	cur.animToPlay = "";
//	cur.animToRepeat = 0;
	cur.lifespan = MISSILE_LIFESPAN;
	cur.sound = "";
	cur.loopSound = false;
	elem = elem->FirstChildElement();
	while(elem)
	{
		Ogre::String name = elem->Value(); //ROFL
		Ogre::StringUtil::toLowerCase(name);
		if(name != "worldart")
		{
			Ogre::String value= elem->Attribute("value");
			
			if(name == "sound")
			{
				cur.sound = value;
			}
			/*else if(name == "anim")
			{
				cur.animToPlay = value;
			}*/
			/*else if(name == "anim_repeat")
			{
				cur.animToRepeat = Ogre::StringConverter::parseUnsignedInt(value);
			}*/
			else if(name == "lifespan")
			{
				cur.lifespan = Ogre::StringConverter::parseReal(value);
			}
			else if(name == "sound_loop")
			{
				cur.loopSound = Ogre::StringConverter::parseBool(value);
			}
			
		}
		else if(name == "worldart")
		{
			cur.wData = parseWorldArtData(elem);
		}
		
		elem = elem->NextSiblingElement();
	}
	return cur;
}


void StandardApplication::explosionDataToXml(TiXmlElement *elem,ExplosionData data)
{
	
	TiXmlElement *waElem = new TiXmlElement("worldArt");
	worldArtToXml(data.wData,waElem);
	elem->LinkEndChild(waElem);

	TiXmlElement *sndElem = new TiXmlElement("sound");
	sndElem->SetAttribute("value",data.sound.c_str());
	elem->LinkEndChild(sndElem);

	TiXmlElement *lsElem = new TiXmlElement("lifespan");
	lsElem->SetAttribute("value",ogre_str(data.lifespan).c_str());
	elem->LinkEndChild(lsElem);

	TiXmlElement *slElem = new TiXmlElement("sound_loop");
	slElem ->SetAttribute("value",ogre_str(data.loopSound).c_str());
	elem->LinkEndChild(slElem );
}

MissileData StandardApplication::parseMissileData(TiXmlElement *elem)
{
	MissileData cur;
	cur.count = 1;
	cur.multimissile_angle = 0;
	cur.multimissile_delay = 0;
	cur.repeat = 1;
	cur.velocity = 10;
	elem = elem->FirstChildElement();
	while(elem)
	{
		Ogre::String name = elem->Value(); //ROFL
//		Ogre::String value = elem->Attribute("value");
		if(name != "worldArt" && name != "collision" && name != "explosion")
		{
			Ogre::String value= elem->Attribute("value");
			
			if(name == "velocity")
			{
				cur.velocity = Ogre::StringConverter::parseReal(value);
			}
			else if(name == "count")
			{
				cur.count = Ogre::StringConverter::parseInt(value);
			}
			else if(name == "sound")
			{
				cur.sound = value;
			}
			else if(name == "delay")
			{
				cur.multimissile_delay = Ogre::StringConverter::parseReal(value);
			}
			else if(name == "angle")
			{
				cur.multimissile_angle = Ogre::StringConverter::parseAngle(value);
			}
			else if(name == "repeat")
			{
				cur.repeat = Ogre::StringConverter::parseInt(value);
			}
			/*else if(name == "bbcolor")
			{
				cur.bbColor = Ogre::StringConverter::parseColourValue(value);
			}*/
		}
		else if(name == "worldArt")
		{
			cur.wData = parseWorldArtData(elem);
		}
		else if(name == "collision")
		{
			cur.cData = parseCollisionData(elem);
		}
		/*else if(name == "explosion")
		{	
			cur.eData = parseExplosionData(elem);
		}*/

		elem = elem->NextSiblingElement();
	}
	return cur;
}


BeamData StandardApplication::parseBeamData(TiXmlElement *elem)
{
	BeamData cur;
	cur.material = "";
	cur.sound = "";
	cur.lifespan = 1;
    cur.width = 1;
    cur.maxRange = 10;
    cur.texCoord = 1;
	/*MissileData cur;
	cur.count = 1;
	cur.multimissile_angle = 0;
	cur.multimissile_delay = 0;
	cur.repeat = 1;
	cur.velocity = 10;*/
	elem = elem->FirstChildElement();
	while(elem)
	{
		Ogre::String name = elem->Value(); //ROFL
        Ogre::StringUtil::toLowerCase(name);
//		Ogre::String value = elem->Attribute("value");
		
		//Ogre::String value= ;
		
		if(name == "material")
		{
			cur.material = elem->Attribute("value");
		}
		else if(name == "sound")
		{
			cur.sound = elem->Attribute("value");
		}
		else if(name == "width")
		{
            cur.width = Ogre::StringConverter::parseReal(elem->Attribute("value"));
		}
        else if(name == "texcoord")
		{
            cur.texCoord = Ogre::StringConverter::parseReal(elem->Attribute("value"));
		}
        else if(name == "maxrange")
		{
            cur.maxRange = Ogre::StringConverter::parseReal(elem->Attribute("value"));
		}
		else if(name == "lifespan")
		{
			cur.lifespan = Ogre::StringConverter::parseReal(elem->Attribute("value"));
		}
		
		elem = elem->NextSiblingElement();
	}
	return cur;
}



void StandardApplication::missileDataToXml(TiXmlElement *elem, MissileData data)
{
	TiXmlElement *velElem = new TiXmlElement("velocity");
	TiXmlElement *cntElem = new TiXmlElement("count");
	TiXmlElement *delElem = new TiXmlElement("delay");
	TiXmlElement *angElem = new TiXmlElement("angle");
	TiXmlElement *repElem = new TiXmlElement("repeat");
	TiXmlElement *sndElem = new TiXmlElement("sound");
	
	velElem->SetAttribute("value",ogre_str(data.velocity).c_str());
	cntElem->SetAttribute("value",ogre_str(data.count).c_str());
	delElem->SetAttribute("value",data.sound.c_str());
	angElem->SetAttribute("value",ogre_str(data.multimissile_delay).c_str());
	repElem->SetAttribute("value",ogre_str(data.multimissile_angle).c_str());
	sndElem->SetAttribute("value",data.sound.c_str());
	elem->LinkEndChild(velElem);
	elem->LinkEndChild(cntElem);
	elem->LinkEndChild(delElem);
	elem->LinkEndChild(angElem);
	elem->LinkEndChild(repElem);
	elem->LinkEndChild(sndElem);

	TiXmlElement *waElem = new TiXmlElement("worldArt");
	worldArtToXml(data.wData,waElem);
	elem->LinkEndChild(waElem);

	

	TiXmlElement *clElem = new TiXmlElement("collision");
	collisionDataToXml(clElem,data.cData);
	elem->LinkEndChild(clElem);
}


gamedata_dynamic StandardApplication::processDynamicSubobject(TiXmlElement *XMLNode)
{
	/*TiXmlElement *XMLNode=XMLNod->FirstChildElement("object");
	while(XMLNode)
	{*/
		
	gamedata_dynamic cur;
	
	cur.jointData.type = JT_NONE;
	cur.jointData.pin0 = Ogre::Vector3::UNIT_Z;
	cur.jointData.pin1 = Ogre::Vector3::UNIT_Y;
	cur.jointData.pos = Ogre::Vector3::ZERO;

	if(XMLNode->Attribute("jointType"))
	{
		Ogre::String joint = XMLNode->Attribute("jointType");
		Ogre::StringUtil::toLowerCase(joint);
		if(joint == "ballandsocket")
			cur.jointData.type = JT_BALL_AND_SOCKET;
		else if(joint == "hingle")
			cur.jointData.type = JT_HINGLE;
		else if(joint == "slider")
			cur.jointData.type = JT_SLIDER;
		else if(joint == "universal")
			cur.jointData.type = JT_UNIVERSAL;
	}
	if(XMLNode->Attribute("jointPos"))
	{
		cur.jointData.pos = Ogre::StringConverter::parseVector3(XMLNode->Attribute("jointPos"));
	}
	if(XMLNode->Attribute("jointPin0"))
	{
		cur.jointData.pin0 = Ogre::StringConverter::parseVector3(XMLNode->Attribute("jointPin0"));
	}
	if(XMLNode->Attribute("jointPin1"))
	{
		cur.jointData.pin1 = Ogre::StringConverter::parseVector3(XMLNode->Attribute("jointPin1"));
	}

	

	//cur.mesh=ERROR_MESH;//XMLNode->Attribute("mesh");
	//cur.scale = Ogre::Vector3::UNIT_SCALE;
	/*cur.collisionParams=Ogre::Vector3::UNIT_SCALE;
	cur.collisionType=0;*/
	CollisionData myCD;
	myCD.collisionFile = "";
	myCD.collistionParams = Ogre::Vector3::UNIT_SCALE;
	myCD.colType = CT_NOCOLLISION;
	myCD.mass = 0;
	myCD.orientationOffset = Ogre::Quaternion::IDENTITY;
	myCD.positionOffset = Ogre::Vector3::ZERO;
	myCD.upVector = Ogre::Vector3::ZERO;
	cur.flags=0;
	/*cur.mass=0;
	cur.material="";*/
	WorldArtData myWAD;
	myWAD.filename	= ERROR_MESH;
	myWAD.scale		= Ogre::Vector3::UNIT_SCALE;
	myWAD.material	= "";
	myWAD.positionOffset = Ogre::Vector3::ZERO;
	myWAD.orientationOffset = Ogre::Quaternion::IDENTITY;
	myWAD.type = WT_MESH;
	cur.wData = myWAD;
	/*cur.collisionParams=Ogre::Vector3::UNIT_SCALE;
	cur.collisionType=0;*/
	cur.flags=0;
	/*cur.mass=0;*/
	if(XMLNode->FirstChildElement("worldArt"))
	{
		cur.wData = parseWorldArtData(XMLNode->FirstChildElement("worldArt"));
	}
	if(XMLNode->FirstChildElement("collision"))
	{
		cur.cData = parseCollisionData(XMLNode->FirstChildElement("collision"));
	}
	/*if(XMLNode->FirstChildElement("object"))
	{
		cur.subObjects.push_back(processDynamicSubobject(XMLNode->FirstChildElement("object")));
	}*/
	/*if(XMLNode->FirstChildElement("object"))
	{
		cur.subObjects.push_back(processDynamicSubobject(XMLNode->FirstChildElement("object")));
	}*/
	if(XMLNode->FirstChildElement("object"))
	{
		TiXmlElement *sub = XMLNode->FirstChildElement("object");
		while(sub)
		{
			cur.subObjects.push_back(processDynamicSubobject(sub));
			sub = sub->NextSiblingElement("object");
		}
		
	}
	
	//if(XMLNode->Attribute("scale"))
	//{
	//	cur.scale = Ogre::StringConverter::parseVector3(XMLNode->Attribute("scale"));

	//	
	//	if(cur.scale == Ogre::Vector3::ZERO)
	//	{
	//		//perhaps it's just one number?
	//		Ogre::Real scale_1 = Ogre::StringConverter::parseReal(XMLNode->Attribute("scale"));
	//		if(scale_1 == 0.0f)
	//		{
	//			//invalid => set it to 1
	//			cur.scale = Ogre::Vector3::UNIT_SCALE;
	//		}
	//		else
	//		{
	//			cur.scale = Ogre::Vector3(scale_1);
	//		}
	//	}
	//}

	//
	//if( XMLNode->Attribute("material"))
	//{
	//	cur.material = XMLNode->Attribute("material");
	//	//settext("juhu");
	//}
	//if( XMLNode->Attribute("mesh"))
	//{
	//	cur.mesh = XMLNode->Attribute("mesh");
	//	
	//}
	
	
	/*if(XMLNode->Attribute("mass"))
	{
		cur.mass=Ogre::StringConverter::parseReal(XMLNode->Attribute("mass"));
	}*/
	if(XMLNode->Attribute("flags"))
	{
		cur.flags=Ogre::StringConverter::parseReal(XMLNode->Attribute("flags"));
	}
	//if(XMLNode->Attribute("collisionParams"))
	//{
	//	cur.collisionParams=Ogre::StringConverter::parseVector3(XMLNode->Attribute("collisionParams"));
	//}
	//if(XMLNode->Attribute("collisionType"))
	//{
	//	cur.collisionType=StringToColType(XMLNode->Attribute("collisionType"));
	//	//perhaps no collision at all?

	//	if (!cur.collisionType || cur.collisionType == CT_TREECOLLISION)
	//	{
	//		cur.mass=0;
	//		//no need of a mass here
	//	}
	//}
	//datamap_dynamic[cur.id]=cur;
	//XMLNode = XMLNode->NextSiblingElement("object");

	//}
	return cur;
}


void StandardApplication::processDynamicData(TiXmlElement *XMLRoot)
{
	TiXmlElement *XMLNode=XMLRoot->FirstChildElement("object");
	while(XMLNode)
	{
		if(!XMLNode->Attribute("id"))
			break;
		gamedata_dynamic *cur = new gamedata_dynamic();
		
		cur->id = XMLNode->Attribute("id");
		//cur->mesh=ERROR_MESH;//XMLNode->Attribute("mesh");
		//cur->scale = Ogre::Vector3::UNIT_SCALE;
		/*cur->collisionParams=Ogre::Vector3::UNIT_SCALE;
		cur->collisionType=0;*/
		cur->flags=0;
		/*cur->mass=0;
		cur->material="";*/
		CollisionData myCD;
		myCD.collisionFile = "";
		myCD.collistionParams = Ogre::Vector3::UNIT_SCALE;
		myCD.colType = CT_TREECOLLISION;
		myCD.mass = 0;
		myCD.orientationOffset = Ogre::Quaternion::IDENTITY;
		myCD.positionOffset = Ogre::Vector3::ZERO;
		myCD.upVector = Ogre::Vector3::ZERO;

		WorldArtData myWAD;
		myWAD.filename	= ERROR_MESH;
		myWAD.scale		= Ogre::Vector3::UNIT_SCALE;
		myWAD.material	= "";
		myWAD.positionOffset = Ogre::Vector3::ZERO;
		myWAD.orientationOffset = Ogre::Quaternion::IDENTITY;
		myWAD.type = WT_MESH;
		cur->wData = myWAD;
        cur->cData = myCD;
		/*cur->collisionParams=Ogre::Vector3::UNIT_SCALE;
		cur->collisionType=0;*/
		cur->flags=0;
		/*cur->mass=0;*/
		if(XMLNode->FirstChildElement("worldArt"))
		{
			cur->wData = parseWorldArtData(XMLNode->FirstChildElement("worldArt"));
		}
		if(XMLNode->FirstChildElement("collision"))
		{
			cur->cData = parseCollisionData(XMLNode->FirstChildElement("collision"));
		}
		if(XMLNode->FirstChildElement("object"))
		{
			TiXmlElement *sub = XMLNode->FirstChildElement("object");
			while(sub)
			{
				cur->subObjects.push_back(processDynamicSubobject(sub));
				sub = sub->NextSiblingElement("object");
			}
			
		}

		
		
		
		if(XMLNode->Attribute("flags"))
		{
			cur->flags=Ogre::StringConverter::parseReal(XMLNode->Attribute("flags"));
		}
	
		registerGameData(cur);
		XMLNode = XMLNode->NextSiblingElement("object");
	}
}


void StandardApplication::processContainerData(TiXmlElement *XMLRoot)
{
	TiXmlElement *XMLNode=XMLRoot->FirstChildElement("object");
	while(XMLNode)
	{
		if(!XMLNode->Attribute("id"))
			break;
		gamedata_container *cur = new gamedata_container();
		
        

		cur->id = XMLNode->Attribute("id");
		//cur->mesh=ERROR_MESH;//XMLNode->Attribute("mesh");
		//cur->scale = Ogre::Vector3::UNIT_SCALE;
		/*cur->collisionParams=Ogre::Vector3::UNIT_SCALE;
		cur->collisionType=0;*/
		cur->flags=0;
		cur->carryable = false;
//		cur->inv_image = ERROR_INVIMAGE;
		cur->name = ERROR_NONAME;
		/*cur->mass=0;
		cur->material="";*/
		CollisionData myCD;
		myCD.collisionFile = "";
		myCD.collistionParams = Ogre::Vector3::UNIT_SCALE;
		myCD.colType = CT_NOCOLLISION;
		myCD.mass = 0;
		myCD.orientationOffset = Ogre::Quaternion::IDENTITY;
		myCD.positionOffset = Ogre::Vector3::ZERO;
		myCD.upVector = Ogre::Vector3::ZERO;

		WorldArtData myWAD;
		myWAD.filename	= ERROR_MESH;
		myWAD.scale		= Ogre::Vector3::UNIT_SCALE;
		myWAD.material	= "";
		myWAD.positionOffset = Ogre::Vector3::ZERO;
		myWAD.orientationOffset = Ogre::Quaternion::IDENTITY;
		myWAD.type = WT_MESH;
		cur->wData = myWAD;
		/*cur->collisionParams=Ogre::Vector3::UNIT_SCALE;
		cur->collisionType=0;*/
		cur->flags=0;
		/*cur->mass=0;*/

        Ogre::String inv_image = ERROR_INVIMAGE;
		if(XMLNode->FirstChildElement("worldArt"))
		{
			cur->wData = parseWorldArtData(XMLNode->FirstChildElement("worldArt"));
		}
		if(XMLNode->FirstChildElement("collision"))
		{
			cur->cData = parseCollisionData(XMLNode->FirstChildElement("collision"));
		}
		if(XMLNode->Attribute("carryable"))
		{
			cur->carryable=Ogre::StringConverter::parseBool(XMLNode->Attribute("carryable"));
		}
		if(XMLNode->Attribute("inv_image"))
		{
			inv_image=XMLNode->Attribute("inv_image");
		}
		if(XMLNode->Attribute("name"))
		{
			cur->name=XMLNode->Attribute("name");
		}		
		if(XMLNode->Attribute("flags"))
		{
			cur->flags=Ogre::StringConverter::parseReal(XMLNode->Attribute("flags"));
		}
		if(XMLNode->Attribute("default_size"))
		{
			cur->defaultSize=Ogre::StringConverter::parseInt(XMLNode->Attribute("default_size"));
		}
        
        

		registerGameData(cur);
        if(cur->carryable)
        {
            //nun das item:
            gamedata_item *item = new gamedata_item();
            item->equipFlags = EF_BAG;
            item->Container = cur;//set the item's container to what we have done above
            item->name = cur->name;
            item->inv_image = inv_image;
            item->id = "container_"+cur->id;

            registerGameData(item);
			cur->inv_item = item;//set the container's item to this item       
           
        }
		XMLNode = XMLNode->NextSiblingElement("object");
	}
}



void StandardApplication::processAlchemyData(TiXmlElement *XMLRoot)
{
	alchemy = new Alchemy();
	alchemy->loadFromXml(XMLRoot);
	/*TiXmlElement *XMLNode=XMLRoot->FirstChildElement("object");
	while(XMLNode)
	{
		
		XMLNode = XMLNode->NextSiblingElement("object");
	}*/
}

void StandardApplication::registerGameData(gamedata *data)
{
	GameDataMap::iterator itr = gameDataMap.find(data->id);
	if(itr != gameDataMap.end())
	{
		//error! id alerady taken
		gamedata *old = itr->second;
		mLog("Registering object '"+data->id+"' of type "+gameDataTypeToString(data->type)+": the ID is alerady taken by a "+gameDataTypeToString(old->type)+" object. Deleting former definition");
		
		delete old;
	}
	//setting data, or overwriting a now invalid pointer
	gameDataMap[data->id] = data;
}

collision_type StandardApplication::stringToColType(Ogre::String ogre_str)
{
	Ogre::StringUtil::toLowerCase(ogre_str);
	if(ogre_str == "box")
		return CT_BOX;
	else if(ogre_str == "capsule")
		return CT_CAPSULE;
	else if(ogre_str == "champfercylinder")
		return CT_CHAMFERCYLINDER;
	else if(ogre_str == "cone")
		return CT_CONE;
	else if(ogre_str == "convexhull")
		return CT_CONVEXHULL;
	else if(ogre_str == "cylinder")
		return CT_CYLINDER;
	else if(ogre_str == "ellipsoid")
		return CT_ELLIPSOID;
	else if(ogre_str == "pyramid")
		return CT_PYRAMID;
	else if(ogre_str == "treecollision")
		return CT_TREECOLLISION;
	else if(ogre_str == "compound")
		return CT_COMPOUND;
	else if(ogre_str == "file")
		return CT_EXTERNAL;
    else if(ogre_str == "none")
        return CT_NOCOLLISION;
	return CT_TREECOLLISION;
}

Ogre::String StandardApplication::colTypeToString(collision_type type)
{
	switch(type)
	{
	case CT_BOX:
		return "box";
	case CT_CAPSULE:
		return "capsule";
	case CT_CHAMFERCYLINDER:
		return "champfercylinder";
	case CT_CONE:
		return "cone";
	case CT_CONVEXHULL:
		return "convexhull";
	case CT_CYLINDER:
		return "cylinder";
	case CT_ELLIPSOID:
		return "ellipsoid";
	case CT_PYRAMID:
		return "pyramid";
	case CT_TREECOLLISION:
		return "treecollision";
	case CT_COMPOUND:
		return "compound";
	case CT_EXTERNAL:
		return "file";
    case CT_NOCOLLISION:
        return "none";
	default:
		return "treecollision";
	}
}

Ogre::String StandardApplication::gameDataTypeToString(GamedataType type)
{
	switch(type)
	{
		case gtNotSet:		return "gtNotSet";
		case gtDynamic:		return "gtDynamic";
		case gtContainer:	return "gtContainer";
		case gtItem:		return "gtItem";
		case gtSpell:		return "gtSpell";
		case gtCharacter:	return "gtCharacter";
		case gtDoor:		return "gtDoor";
		case gtExtra:		return "gtExtra";
		default:			return "invalid";
	}
}

SpellHitType StandardApplication::stringToSpellHit(Ogre::String ogre_str)
{
	Ogre::StringUtil::toLowerCase(ogre_str);
	if(ogre_str=="missile")
		return shMissile;
	else if(ogre_str=="explosion")
		return shExplosion;
	else if(ogre_str=="missilerain")
		return shMissileRain;
	else if(ogre_str=="beam")
		return shBeam;
	else if(ogre_str=="self")
		return shOnSelf;
	else if(ogre_str=="script")
		return shScript;
	return shNone;
}


Ogre::String StandardApplication::spellHitToString(SpellHitType type)
{
	switch(type)
	{
	case shExplosion:
		return "explosion";
	case shMissileRain:
		return "missilerain";
	case shBeam:
		return "beam";
	case shOnSelf:
		return "self";
	case shScript:
		return "script";	
	case shMissile:
		return "missile";
	default:
		return "none";
	};
}


SpellEffectType StandardApplication::stringToSpellEffect(Ogre::String ogre_str)
{
	
	Ogre::StringUtil::toLowerCase(ogre_str);
	if(ogre_str=="damage")
		return seDamage;
	else if(ogre_str=="modstats")
		return seModStats;
	else if(ogre_str=="absorb")
		return seAbsorb;
	else if(ogre_str=="script")
		return seScript;
	
	return seNone;
}

Ogre::String StandardApplication::SpellEffectToString(SpellEffectType set)
{
	
	switch(set)
	{
	case seDamage:
		return "damage";
	case seModStats:
		return "modstats";
	case seAbsorb:
		return "absorb";
	case seScript:
		return "script";
	default:
		return "none";
	}
}

SpellEffectData StandardApplication::parseSpellEffectData(TiXmlElement *elem)
{
	/*
	SpellEffectData()
	{
		type = seNone;
		splashRadius = 0;
		explosion = NULL; 
	}
	~SpellEffectData()
	{
		if(explosion)
			delete explosion;
	}
	SpellEffectType type;
	//wie lang der effekt anhält
	Ogre::Real duration;
	ExplosionData *explosion;
	Damage dmg;
	Stats_Item stat;
	Ogre::Real splashRadius; //erstmal linear

	<effect type="muh" splashradius="x" duration="x">
		<damage...>
		<stats...>
		<explosion...>
	</effect>
	*/
	
	SpellEffectData cur;
	if(elem->Attribute("type"))
	{
		cur.type = stringToSpellEffect(elem->Attribute("type"));
	}
	if(elem->Attribute("splashradius"))
	{
		cur.splashRadius = Ogre::StringConverter::parseReal(elem->Attribute("splashradius"));
	}
	/*if(elem->Attribute("duration"))
	{
		cur.duration = Ogre::StringConverter::parseReal(elem->Attribute("duration"));
	}*/
	
	//TiXmlElement *dmgElem = elem->FirstChildElement("damage");
	//if(dmgElem)
	//{
	//	cur.dmg = parseDamageData(dmgElem);
	//}

	//TiXmlElement *statElem = elem->FirstChildElement("stats");
	//if(statElem)
	//{
	//	cur.stat = parseChStats(statElem);
	//}


	elem = elem->FirstChildElement();
	while(elem)
	{
		Ogre::String name = elem->Value(); //ROFL
		Ogre::StringUtil::toLowerCase(name);

//		Ogre::String value= elem->Attribute("value");
		if(name == "damage")
		{
			cur.dmg = parseDamageData(elem);
		}
		else if(name == "stats")
		{
			//cur.stats.push_back
			parseStatChangeList(elem,cur.stats);
			//cur.stat = parseChStats(elem);
		}
//		else if(name == "explosion")
//		{
////			cur.explosion = new ExplosionData();
//			cur.explosion = parseExplosionData(elem);
//
//		}
		elem = elem->NextSiblingElement();
	}
	return cur;
}


void StandardApplication::spellEffectDataToXml(TiXmlElement *elem,SpellEffectData data)
{	
	elem->SetAttribute("type",SpellEffectToString(data.type).c_str());
	elem->SetAttribute("splashradius",ogre_str(data.splashRadius).c_str());
	
	TiXmlElement *dmgElem = new TiXmlElement("damage");
	TiXmlElement *statElem = new TiXmlElement("stats");
	damageDataToXml(dmgElem,data.dmg);
	statChangeListToXml(statElem,data.stats);
	elem->LinkEndChild(dmgElem);
	elem->LinkEndChild(statElem);
}

void StandardApplication::parseStatChangeList(TiXmlElement *elem, StatChangeList &list)
{
	//das elem ist das <stats>..</stats>, mit den einzeleinträgen wie 	<resistBlunt value="0.75" mode="multiply" />
	elem = elem->FirstChildElement();
	while(elem)
	{
		ChStat cur = parseChStats(elem);
		if(cur.op != opNone)
		{
			if(cur.value == 0 && cur.op == opMultiply)
				cur.op = opSet;
			list.push_back(cur);
		}
		elem = elem->NextSiblingElement();
	}
}


void StandardApplication::statChangeListToXml(TiXmlElement *elem, StatChangeList list)
{
	for(StatChangeList::iterator itr = list.begin();itr != list.end();itr++)
	{
		elem->LinkEndChild( chStatsToXml(*itr));
	}
	
}

void StandardApplication::processSpellData(TiXmlElement *XMLRoot)
{
	TiXmlElement *XMLNode=XMLRoot->FirstChildElement("object");
	while(XMLNode)
	{
		if(!XMLNode->Attribute("id"))
			break;
		gamedata_spell *cur = new gamedata_spell();
		
		cur->id = XMLNode->Attribute("id");	
		
		if(XMLNode->Attribute("name"))
		{
			cur->name=XMLNode->Attribute("name");			
		}
		if(XMLNode->Attribute("inv_image"))
		{
			cur->inv_image=XMLNode->Attribute("inv_image");			
		}
		if(XMLNode->Attribute("cost"))
		{
			cur->manaCost=Ogre::StringConverter::parseReal(XMLNode->Attribute("cost"));
		}
		/*if(XMLNode->Attribute("effecttype"))
		{
			cur->spellEffectType=stringToSpellEffect(XMLNode->Attribute("effecttype"));
		}*/
		if(XMLNode->Attribute("hittype"))
		{
			cur->spellHitType=stringToSpellHit(XMLNode->Attribute("hittype"));
		}
		if(XMLNode->Attribute("sound"))
		{
			cur->sound=XMLNode->Attribute("sound");
		}
		if(XMLNode->Attribute("duration"))
		{
			cur->duration=Ogre::StringConverter::parseReal(XMLNode->Attribute("duration"));
		}
		if(XMLNode->Attribute("ancient"))
		{
			cur->duration=Ogre::StringConverter::parseBool(XMLNode->Attribute("ancient"));
		}
		if(XMLNode->Attribute("range"))
		{
			cur->range=Ogre::StringConverter::parseReal(XMLNode->Attribute("range"));
		}
		if(XMLNode->Attribute("cooldown"))
		{
			cur->cooldown= Ogre::StringConverter::parseReal( XMLNode->Attribute("cooldown") );			
		}
		TiXmlElement *flags = XMLNode->FirstChildElement();
		while(flags)
		{
			Ogre::String type = flags->Value();
			Ogre::StringUtil::toLowerCase(type);
			//Ogre::String name = flags->Attribute("name");
			//Ogre::String value= flags->Attribute("value");
			if(type == "missiledata")
			{
				if(!cur->Missile)
				{
					MissileData *d = new MissileData();
					*d = parseMissileData(flags);
					cur->Missile = d;
					
					//cur->Missile = new MissileData();
				}
				//if(!cur->Missile)
				//	cur->Missile = new MissileData();
				////TiXmlElement *stat = flags->FirstChildElement();
				//*cur->Missile = parseMissileData(flags);				
			}	
			else if(type == "explosion")
			{
				if(!cur->Explosion)
				{
					ExplosionData *d = new ExplosionData();
					*d = parseExplosionData(flags);
					cur->Explosion = d;
					
					//cur->Missile = new MissileData();
				}

				//if(!cur->Explosion)
				//	cur->Explosion = new ExplosionData();
				////TiXmlElement *stat = flags->FirstChildElement();
				//*cur->Explosion = parseExplosionData(flags);
				//
			}	
			else if(type == "beam")
			{
				if(!cur->Beam)
				{
					BeamData *d = new BeamData();
					*d = parseBeamData(flags);
					cur->Beam = d;
					
					//cur->Missile = new MissileData();
				}
				
			}
			else if(type == "effect")
			{
				/*if(!cur->Explosion)
					cur->Explosion = new ExplosionData();*/
				//TiXmlElement *stat = flags->FirstChildElement();
				cur->effects.push_back(parseSpellEffectData(flags));//parseExplosionData(flags);
				
			}	
			/*else if(type == "damage")
			{
				
				TiXmlElement *stat = flags->FirstChildElement();
				cur->dmg = parseDamageData(stat);				
			}*/
			flags = flags->NextSiblingElement();
		}
		registerGameData(cur);
		XMLNode = XMLNode->NextSiblingElement("object");
	}
}

void StandardApplication::loadGameData(void)
{
	setDefaultData();

	//AUS XML-Datei laden
	Ogre::DataStreamPtr pStream = Ogre::ResourceGroupManager::getSingleton().openResource("objects.xml","game_data");

	Ogre::String data = pStream->getAsString();
	TiXmlDocument *ObjDoc;
	ObjDoc = new TiXmlDocument();
	ObjDoc->Parse( data.c_str() );

	pStream->close();
	pStream.setNull();
	TiXmlElement   *XMLRoot;
	
	XMLRoot = ObjDoc->FirstChildElement("door");
	if(XMLRoot)
	{
		processDoorData(XMLRoot);
	}
	
	
	//parsing specials...
	XMLRoot = ObjDoc->FirstChildElement("special");
	if(XMLRoot)
	{
		processDynamicData(XMLRoot);
	}
	
	//parsing spells
	XMLRoot = ObjDoc->FirstChildElement("spell");
	if(XMLRoot)
	{
		processSpellData(XMLRoot);
	}
	//parsing items...
	//items müssen NACH spells geparst werden, denn deren effekte sind spells
	XMLRoot = ObjDoc->FirstChildElement("item");
	if(XMLRoot)
	{
		processItemData(XMLRoot);
	}
	//kontainer
	XMLRoot = ObjDoc->FirstChildElement("container");
	if(XMLRoot)
	{
		processContainerData(XMLRoot);
	}
	//parsing chars
	//GANZ WICHTIG!
	//character müssen NACH items, spells und container geparst werden,
	//denn sie können default-inventories, -spellbooks und -equipment haben!
	XMLRoot = ObjDoc->FirstChildElement("character");
	if(XMLRoot)
	{
		processCharData(XMLRoot);		
	}

	//alchemy...
	XMLRoot = ObjDoc->FirstChildElement("alchemy");
	if(XMLRoot)
	{
		processAlchemyData(XMLRoot);		
	}
	
	delete ObjDoc;

	
	mLog("game data geladen");
}





void StandardApplication::setDebugText(Ogre::String ogre_str,bool add)
{
	//CEGUI::Window *muhtest = CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"TextOut");
	//CEGUI::String test = "";
	//if(add)
	//	test += muhtest->getText();
	//test += ogre_str.c_str();


	//muhtest->setText(test);
}

Ogre::Root *StandardApplication::getRoot()
{
	return mRoot;
}

Ogre::RenderWindow *StandardApplication::getRenderWindow()
{
	return mWindow;
}




void StandardApplication::sortNameTimeVector(NameTimeVector &vec)
{
	//bubblesort
	bool done = false;
	while(!done)
	{
		done = true;
		for(size_t i = 1;i < vec.size();i++)
		{
			if(vec[i-1].time < vec[i].time)
			{
				done = false;
				NameTime temp = vec[i-1];
				vec[i-1] = vec[i];
				vec[i] = temp;
			}
		}
	}
}

//get a pointer
gamedata *StandardApplication::getDataPtr(Ogre::String id)
{
	/*if(id == "")
		return NULL*/
	GameDataMap::iterator itr = gameDataMap.find(id);
	if(itr != gameDataMap.end())
	{
		return itr->second;
	}
	return NULL;
}
//get an object
gamedata StandardApplication::getData(Ogre::String id)
{
	gamedata *ptr = getDataPtr(id);
	if(ptr)
		return *ptr;
	else
	{
		gamedata empty;
		empty.id = "";
		empty.type = gtNotSet;
		return empty;
	}
}

gamedata_dynamic StandardApplication::getDynamicData(Ogre::String id)
{
	gamedata_dynamic *ptr = getDynamicDataPtr(id);
	if(ptr)
	{		
		return *ptr;	
	}
	else
	{
		gamedata_dynamic empty;
		empty.id = "";
		return empty;
	}
}
gamedata_item StandardApplication::getItemData(Ogre::String id)
{
	gamedata_item *ptr = getItemDataPtr(id);
	if(ptr)
	{		
		return *ptr;	
	}
	else
	{
		gamedata_item empty;
		empty.id = "";
		return empty;
	}

}
gamedata_char StandardApplication::getCharData(Ogre::String id)
{

	gamedata_char *ptr = getCharDataPtr(id);
	if(ptr)
	{		
		return *ptr;	
	}
	else
	{
		gamedata_char empty;
		empty.id = "";
		return empty;
	}
}


gamedata_spell StandardApplication::getSpellData(Ogre::String id)
{
	gamedata_spell *ptr = getSpellDataPtr(id);
	if(ptr)
	{		
		return *ptr;	
	}
	else
	{
		gamedata_spell empty;
		empty.id = "";
		return empty;
	}
}

gamedata_spell *StandardApplication::getSpellDataPtr(Ogre::String id)
{
	gamedata *data = getDataPtr(id);
	if(data && data->type == gtSpell)
		return static_cast<gamedata_spell*>(data);
	return NULL;

}

//--------------------

gamedata_container StandardApplication::getContainerData(Ogre::String id)
{
	gamedata_container *ptr = getContainerDataPtr(id);
	if(ptr)
	{		
		return *ptr;	
	}
	else
	{
		gamedata_container empty;
		empty.id = "";
		return empty;
	}
}

gamedata_container *StandardApplication::getContainerDataPtr(Ogre::String id)
{
	gamedata *data = getDataPtr(id);
	if(data && data->type == gtContainer)
		return static_cast<gamedata_container*>(data);
	return NULL;
}
//--------------------------

gamedata_door StandardApplication::getDoorData(Ogre::String id)
{
	gamedata_door *ptr = getDoorDataPtr(id);
	if(ptr)
	{		
		return *ptr;	
	}
	else
	{
		gamedata_door empty;
		empty.id = "";
		return empty;
	}
}

gamedata_dynamic *StandardApplication::getDynamicDataPtr(Ogre::String id)
{
	gamedata *data = getDataPtr(id);
	if(data && data->type == gtDynamic)
		return static_cast<gamedata_dynamic*>(data);
	return NULL;
}
gamedata_item *StandardApplication::getItemDataPtr(Ogre::String id)
{
	gamedata *data = getDataPtr(id);
	if(data && data->type == gtItem)
		return static_cast<gamedata_item*>(data);
	return NULL;	
}
gamedata_char *StandardApplication::getCharDataPtr(Ogre::String id)
{
	gamedata *data = getDataPtr(id);
	if(data && data->type == gtCharacter)
		return static_cast<gamedata_char*>(data);
	return NULL;
}
gamedata_door *StandardApplication::getDoorDataPtr(Ogre::String id)
{
	gamedata *data = getDataPtr(id);
	if(data && data->type == gtDoor)
		return static_cast<gamedata_door*>(data);
	return NULL;
}

UniKey StandardApplication::stringToUniKey(Ogre::String ogre_str)
{
	UniKey res;

	
	if(Ogre::StringUtil::startsWith(ogre_str,"ctrl+"))
	{
		res.modifier = OIS::Keyboard::Ctrl;
		ogre_str = Ogre::StringUtil::split(ogre_str,"+",1)[1];
	}
	else if(Ogre::StringUtil::startsWith(ogre_str,"alt+"))
	{
		res.modifier = OIS::Keyboard::Alt;
		ogre_str = Ogre::StringUtil::split(ogre_str,"+",1)[1];
	}
	else if(Ogre::StringUtil::startsWith(ogre_str,"shift+"))
	{
		res.modifier = OIS::Keyboard::Shift;
		ogre_str = Ogre::StringUtil::split(ogre_str,"+",1)[1];
	}
	if(Ogre::StringUtil::startsWith(ogre_str,"mb"))
	{
		res.isKeyboard = false;
		if(ogre_str == "MB_Left")
			res.mouseID = MB_Left;
		else if(ogre_str == "MB_Right")
			res.mouseID = MB_Right;
		else if(ogre_str == "MB_Middle")
			res.mouseID = MB_Middle;
		else if(ogre_str == "MB_Button3")
			res.mouseID = MB_Button3;
		else if(ogre_str == "MB_Button4")
			res.mouseID = MB_Button4;
		else if(ogre_str == "MB_Button5")
			res.mouseID = MB_Button5;
		else if(ogre_str == "MB_Button6")
			res.mouseID = MB_Button6;
		else if(ogre_str == "MB_Button7")
			res.mouseID = MB_Button7;
		
		return res;
		
	}
	else if(Ogre::StringUtil::startsWith(ogre_str,"kc"))
	{
		std::map<OIS::KeyCode,Ogre::String>::iterator itr;
		for(itr = keys.begin();itr != keys.end();itr++)
		{
			OIS::KeyCode k = itr->first;
			Ogre::String s = itr->second;
			if(s == ogre_str)
			{
				res.keyboardID = k;
				return res;
			}
		}
	}
	res.keyboardID = OIS::KC_UNASSIGNED;
	res.isKeyboard = true;
	return res;
}

Ogre::String StandardApplication::uniKeyToString(UniKey key)
{
	Ogre::String res = "";
    if(key.hasModifier())
	{
		switch(key.modifier)
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
	if(key.isKeyboard)
	{
		res += keys[key.keyboardID];
	}
	else
	{
		switch(key.mouseID)
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
