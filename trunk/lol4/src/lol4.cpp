

//#include "FwDec.h"

//#include "GameChar.h"
//#include "global_class.h"
//#include "GameFrameListener.h"
#include "GameApp.h"
//#include "contactCallback.h"



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
#else
   int main(int argc, char *argv[])
   {
       std::string strCmdLine = "";
       for(int i=0;i<argc;i++)
       {
           if(i != 0)
               strCmdLine += " ";
           strCmdLine += std::string(argv[i]);
           
       }
#endif
   
		
       
       

      // Create application object
      GameApp app(strCmdLine);

      //SET_TERM_HANDLER;

      try {
         app.go();
      } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
         MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
         std::cerr << "An exception has occured: " <<
            e.getFullDescription().c_str() << std::endl;
#endif
      }

      return 0;
	  
   }

#ifdef __cplusplus
}
#endif

