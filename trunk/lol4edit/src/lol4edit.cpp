/*
#include <Ogre.h>

#include <OgreFrameListener.h>
#include <OgreEventListeners.h>
#include <OgreKeyEvent.h>


#include <OgreTextAreaOverlayElement.h>
#include <OgreFontManager.h>
#include <vector>
#include <OgreNewt.h>
#include "tinyxml.h"


//mem probs without this next one
#include <OgreNoMemoryMacros.h>
#include <CEGUI/CEGUIImageset.h>
#include <CEGUI/CEGUISystem.h>
#include <CEGUI/CEGUILogger.h>
#include <CEGUI/CEGUISchemeManager.h>
#include <CEGUI/CEGUIWindowManager.h>
#include <CEGUI/CEGUIWindow.h>
#include "OgreCEGUIRenderer.h"
#include "OgreCEGUIResourceProvider.h"
#include <CEGUI/elements/CEGUIPushButton.h>

//regular mem handler
#include <OgreMemoryMacros.h>
*/
//#include "editFWDec.h"
//#include "FwDec.h"
#include "EditorApp.h"
#include <locale.h> 
#include "QtEditorApp.h"
//#include "EditGameLogic.h"


#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif
#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
   INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
   {
//	INT __stdcall wWinMain ( HINSTANCE hInst, HINSTANCE, LPWSTR strCmdLine, INT  )
#else
   int main(int argc, char *argv[])
   {
       /*std::string strCmdLine = "";
       for(int i=0;i<argc;i++)
       {
           if(i != 0)
               strCmdLine += " ";
           strCmdLine += std::string(argv[i]);
           
       }*/
#endif
       

//	 // setlocale( LC_NUMERIC, "English" ); 
//		
      // Create application object
      EditorApp editApp(strCmdLine);

      int returnCode = 0;

      try
      {
          returnCode = editApp.go_qt();        
      }
      catch (Ogre::Exception& e)
      {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
         MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
         std::cerr << "An exception has occured: " <<
            e.getFullDescription().c_str() << std::endl;
#endif
      }

      


      return returnCode;
//
//
//      //SET_TERM_HANDLER;
//
      /*try {
         app.go();
      } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
         MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
         std::cerr << "An exception has occured: " <<
            e.getFullDescription().c_str() << std::endl;
#endif
      }*/
//
//      return 0;
	  
   }

#ifdef __cplusplus
}
#endif

