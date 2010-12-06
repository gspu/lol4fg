#ifndef __praUniKey
#define __praUniKey

#include "OIS/OIS.h"
#include "Ogre.h"
#ifndef __editor
#include "CEGUI.h"
#else
#endif

using namespace OIS;

//das muss lernen, mit Qt-KeyCodes umzugehen...
//und mit denen von cegui?
//am besten 2 abgeleitete klassen, eine für game, eine für editor

class UniKey
	{
	public:
#ifndef __editor
		UniKey(CEGUI::MouseButton ceguiMouse, CEGUI::uint ceguiMod = 0);
		//converts the key name to a readable string. returns "" if the key name is not to be set by the user
		/*static Ogre::String keyNameToString(unsigned int kn);*/
#else
#endif
		UniKey(OIS::MouseButtonID ID, int mod = 0)
		{
			isKeyboard = false;
			mouseID = ID;
			modifier = mod;
		}
		UniKey(OIS::KeyCode ID, int mod = 0)
		{
			isKeyboard = true;
			keyboardID = ID;
			modifier = mod;			
		}
		UniKey()
		{
			isKeyboard = true;
			keyboardID = OIS::KC_UNASSIGNED;
			mouseID = OIS::MB_Left;			
			modifier = 0;			
		}
		UniKey(Ogre::String str)
		{
			loadFromString(str);
		}
		UniKey operator = (OIS::KeyCode kc)
		{
			isKeyboard = true;
			keyboardID = kc;
		
			modifier = 0;
			
			return *this;
		}
		UniKey operator = (OIS::MouseButtonID id)
		{
			isKeyboard = false;
			mouseID = id;
			modifier = 0;
			keyboardID = OIS::KC_UNASSIGNED;
			//return UniKey(id);
			return *this;
		}
		
		bool operator == (UniKey k);

		/*bool operator == (Qt::Key qtKey)
		{
			OIS::KeyCode kc = convertQtKeyToOis(qtKey);
			return (*this) == kc;
		}*/

		bool operator == (OIS::KeyCode kc)
		{
            if(isEmpty() && kc != OIS::KC_UNASSIGNED)
				return false;
			if(modifier != 0 || !isKeyboard)//wenn modifier gesetzt ist, oder es keine Taste ist
			{
				return false;				
			}
			else
			{
				return (keyboardID == kc);				
			}
		}
		bool operator == (OIS::MouseButtonID mb)
		{			
			if(isEmpty())
				return false;
			if(modifier != 0 || isKeyboard)
			{
				return false;
			}
			else
			{
				return (mouseID == mb);
			}
		}
		bool isEmpty()
		{
			return (isKeyboard && keyboardID == KC_UNASSIGNED);
		}
        /**
         * returns the modifier as OIS::Keyboard::Modifier. 
         * if modifier is 0 or any invalid value, Modifier::Ctrl is returned!
        */
        OIS::Keyboard::Modifier getModifier()
        {
            switch(modifier){
            case OIS::Keyboard::Shift:
                return OIS::Keyboard::Shift;
            case OIS::Keyboard::Alt:
                return OIS::Keyboard::Alt;
            default:
                return OIS::Keyboard::Ctrl;
            }
        }

		//holt den modifier als keycode. es werden immer die linken shift-, alt- und ctrltasten zurückgegeben
		OIS::KeyCode getModifierAsKeyCode()
		{
			switch(modifier){
            case OIS::Keyboard::Shift:
				return OIS::KC_LSHIFT;
            case OIS::Keyboard::Alt:
                return OIS::KC_LMENU;
            default:
				return OIS::KC_LCONTROL;
            }
		}

        bool hasModifier(){
            switch(modifier){            
            case OIS::Keyboard::Shift:                
            case OIS::Keyboard::Alt:
            case OIS::Keyboard::Ctrl:
                return true;
            default:
                return false;
            }
        }
        inline void setModifier(OIS::Keyboard::Modifier mod){
            modifier = mod;
        }
        inline void clearModifier(){
            modifier = 0;
        }
		Ogre::String toString();
		void loadFromString(Ogre::String str);

		

		bool isKeyboard;
		
		OIS::MouseButtonID mouseID;
		OIS::KeyCode keyboardID;
		//OIS::Keyboard::Modifier modifier;
        int modifier;//should be OIS::Keyboard::Modifier, but i need 0 as value sometimes

		
        //MAYBE: 
        /*
        enum KeyType{
            ktNotSet = 0,
            ktOisKeyboard = 1,
            ktQtKeyboard = 2,
            ktOisMouse = 3,
            ktQtMouse = 4
        };
        anstatt bool isKeyboard?
        */
	private:
		static std::map<OIS::KeyCode,Ogre::String> keyMap;
		static std::map<OIS::KeyCode,Ogre::String> initKeyMap();
	};

#endif