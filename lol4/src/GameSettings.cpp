#include "GameSettings.h"
#include "xmlwrapper.h"
#include "GameApp.h"

//
//void GameSettings::processKeyElem(TiXmlElement *elem)
//{
//	using namespace Ogre;
//	
//    keys = elem->FirstChildElement("key");
//    while(keys)
//    {
//        Ogre::String varName = keys->Attribute("name");
//        Ogre::String value= keys->Attribute("value");
//        Ogre::StringUtil::toLowerCase(varName);
//        if(varName == "MoveFoward")				
//			keys[keyMoveForward].alt = UniKey(value);
//		else if(varName == "MoveBack")				
//			keys[keyMoveBack].alt = UniKey(value); 
//		else if(varName == "MoveRight")				
//			keys[keyMoveRight].alt = UniKey(value); 
//		else if(varName == "MoveLeft")				
//			keys[keyMoveLeft].alt = UniKey(value);
//		else if(varName == "Jump")				
//			keys[keyJump].alt = UniKey(value); 
//		else if(varName == "Attack")				
//			keys[keyAttack].alt = UniKey(value);
//		else if(varName == "Spellcast")				
//			keys[keySpellcast].alt = UniKey(value); 
//		else if(varName == "Run")				
//			keys[keyRun].alt = UniKey(value); 
//		else if(varName == "Action")				
//			keys[keyAction].alt = UniKey(value); 
//		else if(varName == "Inventory")				
//			keys[keyInventory].alt = UniKey(value); 
//		else if(varName == "Spellbook")				
//			keys[keySpellbook].alt = UniKey(value); 
//		else if(varName == "InvTakeOne")				
//			keys[keyInvTakeOne].alt = UniKey(value); 
//		else if(varName == "InvTakeAll")				
//			keys[keyInvTakeAll].alt = UniKey(value); 
//		else if(varName == "InvUse")				
//			keys[keyInvUse].alt = UniKey(value); 
//		else if(varName == "Screenshot")				
//			keys[keyScreenshot].alt = UniKey(value); 
//		else if(varName == "Mainmenu")				
//			keys[keyMainmenu].alt = UniKey(value); 
//		else if(varName == "LookUp")				
//			keys[keyLookUp].alt = UniKey(value); 
//		else if(varName == "LookDown")				
//			keys[keyLookDown].alt = UniKey(value); 
//		else if(varName == "LookLeft")				
//			keys[keyLookLeft].alt = UniKey(value); 
//		else if(varName == "LookRight")				
//			keys[keyLookRight].alt = UniKey(value); 
//		else if(varName == "ToggleMouselooking")				
//			keys[keyToggleMouselooking].alt = UniKey(value);
//		else if(varName == "Debug3rdp")				
//			keys[keyDebug3rdp].alt = UniKey(value);
//		else if(varName == "Debugshowlines")				
//			keys[keyDebugshowlines].alt = UniKey(value);
//		else if(varName == "DragItem")				
//			keys[keyDragItem].main = UniKey(value);
//        keys = keys->NextSiblingElement("key");
//    }
//}

void GameSettings::loadSettings(TiXmlElement *elem)
{
	using namespace Ogre;
	TiXmlElement *option = elem->FirstChildElement("options")->FirstChildElement("option");
	
    while(option)
    {
        Ogre::String varName = option->Attribute("name");
        Ogre::String value= option->Attribute("value");
        Ogre::StringUtil::toLowerCase(varName);
        if(varName == "mouselooking")
			mouselooking = StringConverter::parseBool(value);
		else if(varName == "sensibility_x_mouse")
			sensibility_x_mouse = StringConverter::parseReal(value);
		else if(varName == "sensibility_y_mouse")
			sensibility_y_mouse = StringConverter::parseReal(value);
		else if(varName == "physics_fps")
			physics_fps = StringConverter::parseReal(value);
		else if(varName == "sensibility_x_keyboard")
			sensibility_x_keyboard = StringConverter::parseReal(value);
		else if(varName == "sensibility_y_keyboard")
			sensibility_y_keyboard = StringConverter::parseReal(value);
		else if(varName == "gain_all")
			gain_all = StringConverter::parseReal(value);
		else if(varName == "gain_music")
			gain_music = StringConverter::parseReal(value);
		else if(varName == "gain_sfx")
			gain_sfx = StringConverter::parseReal(value);
		else if(varName == "gain_player_sfx")
			gain_player_sfx = StringConverter::parseReal(value);
		else if(varName == "gain_speech")
			gain_speech = StringConverter::parseReal(value);
		else if(varName == "rolloff_factor")
			rolloff_factor = StringConverter::parseReal(value);
		else if(varName == "ref_distance")
			ref_distance = StringConverter::parseReal(value);
		else if(varName == "max_distance")
			max_distance = StringConverter::parseReal(value);      
		else if(varName == "distance_model")
			distance_model = SimpleSound::SoundManager::intToDistanceModel(StringConverter::parseInt(value));    
        else if(varName == "starting_level")				
			starting_level = value;
		else if(starting_level == "starting_entrance")				
			starting_entrance = value;
		else if(varName == "default_music")
			default_music = value;
		else if(varName == "default_combat_music")
			default_combat_music = value;
        option = option->NextSiblingElement("option");
    }
	if(elem->FirstChildElement("keys"))
	{
		TiXmlElement *keysElem = elem->FirstChildElement("keys")->FirstChildElement("key");
		GameApp *app = GameApp::getSingletonPtr();
		while(keysElem)
		{
			Ogre::String name = keysElem->Attribute("name");
			Ogre::String main = keysElem->Attribute("main");
			Ogre::String alt = keysElem->Attribute("alt");
			
			
			KeyName curKey = app->parseKeyName(name);
			if(curKey != keyNone)
			{
				//no need for defining keyNone
				keys[curKey].main = UniKey(main);
				keys[curKey].alt = UniKey(alt);//oO

				keysElem = keysElem->NextSiblingElement("key");
			}
		}
	}
    //keys_main
    //TiXmlElement *keysElem = elem->FirstChildElement("keys_main")->FirstChildElement("key");

  //  while(keysElem)
  //  {
  //      Ogre::String varName = keysElem->Attribute("name");
  //      Ogre::String value= keysElem->Attribute("value");
  //      Ogre::StringUtil::toLowerCase(varName);
  //      if(varName == "MoveFoward")				
		//	keys[keyMoveForward].main = UniKey(value);
		//else if(varName == "MoveBack")				
		//	keys[keyMoveBack].main = UniKey(value); 
		//else if(varName == "MoveRight")				
		//	keys[keyMoveRight].main = UniKey(value); 
		//else if(varName == "MoveLeft")				
		//	keys[keyMoveLeft].main = UniKey(value);
		//else if(varName == "Jump")				
		//	keys[keyJump].main = UniKey(value); 
		//else if(varName == "Attack")				
		//	keys[keyAttack].main = UniKey(value);
		//else if(varName == "Spellcast")				
		//	keys[keySpellcast].main = UniKey(value); 
		//else if(varName == "Run")				
		//	keys[keyRun].main = UniKey(value); 
		//else if(varName == "Action")				
		//	keys[keyAction].main = UniKey(value); 
		//else if(varName == "Inventory")				
		//	keys[keyInventory].main = UniKey(value); 
		//else if(varName == "Spellbook")				
		//	keys[keySpellbook].main = UniKey(value); 
		//else if(varName == "InvTakeOne")				
		//	keys[keyInvTakeOne].main = UniKey(value); 
		//else if(varName == "InvTakeAll")				
		//	keys[keyInvTakeAll].main = UniKey(value); 
		//else if(varName == "InvUse")				
		//	keys[keyInvUse].main = UniKey(value); 
		//else if(varName == "Screenshot")				
		//	keys[keyScreenshot].main = UniKey(value); 
		//else if(varName == "Mainmenu")				
		//	keys[keyMainmenu].main = UniKey(value); 
		//else if(varName == "LookUp")				
		//	keys[keyLookUp].main = UniKey(value); 
		//else if(varName == "LookDown")				
		//	keys[keyLookDown].main = UniKey(value); 
		//else if(varName == "LookLeft")				
		//	keys[keyLookLeft].main = UniKey(value); 
		//else if(varName == "LookRight")				
		//	keys[keyLookRight].main = UniKey(value); 
		//else if(varName == "ToggleMouselooking")				
		//	keys[keyToggleMouselooking].main = UniKey(value);
		//else if(varName == "Debug3rdp")				
		//	keys[keyDebug3rdp].main = UniKey(value);
		//else if(varName == "Debugshowlines")				
		//	keys[keyDebugshowlines].main = UniKey(value);
		//else if(varName == "DragItem")				
		//	keys[keyDragItem].main = UniKey(value);
  //      keysElem = keysElem->NextSiblingElement("key");
  //  }
  //  //keys_alt
  //  keysElem = elem->FirstChildElement("keys_alt")->FirstChildElement("key");
  //  while(keysElem)
  //  {
  //      Ogre::String varName = keysElem->Attribute("name");
  //      Ogre::String value= keysElem->Attribute("value");
  //      Ogre::StringUtil::toLowerCase(varName);
  //      if(varName == "MoveFoward")				
		//	keys[keyMoveForward].alt = UniKey(value);
		//else if(varName == "MoveBack")				
		//	keys[keyMoveBack].alt = UniKey(value); 
		//else if(varName == "MoveRight")				
		//	keys[keyMoveRight].alt = UniKey(value); 
		//else if(varName == "MoveLeft")				
		//	keys[keyMoveLeft].alt = UniKey(value);
		//else if(varName == "Jump")				
		//	keys[keyJump].alt = UniKey(value); 
		//else if(varName == "Attack")				
		//	keys[keyAttack].alt = UniKey(value);
		//else if(varName == "Spellcast")				
		//	keys[keySpellcast].alt = UniKey(value); 
		//else if(varName == "Run")				
		//	keys[keyRun].alt = UniKey(value); 
		//else if(varName == "Action")				
		//	keys[keyAction].alt = UniKey(value); 
		//else if(varName == "Inventory")				
		//	keys[keyInventory].alt = UniKey(value); 
		//else if(varName == "Spellbook")				
		//	keys[keySpellbook].alt = UniKey(value); 
		//else if(varName == "InvTakeOne")				
		//	keys[keyInvTakeOne].alt = UniKey(value); 
		//else if(varName == "InvTakeAll")				
		//	keys[keyInvTakeAll].alt = UniKey(value); 
		//else if(varName == "InvUse")				
		//	keys[keyInvUse].alt = UniKey(value); 
		//else if(varName == "Screenshot")				
		//	keys[keyScreenshot].alt = UniKey(value); 
		//else if(varName == "Mainmenu")				
		//	keys[keyMainmenu].alt = UniKey(value); 
		//else if(varName == "LookUp")				
		//	keys[keyLookUp].alt = UniKey(value); 
		//else if(varName == "LookDown")				
		//	keys[keyLookDown].alt = UniKey(value); 
		//else if(varName == "LookLeft")				
		//	keys[keyLookLeft].alt = UniKey(value); 
		//else if(varName == "LookRight")				
		//	keys[keyLookRight].alt = UniKey(value); 
		//else if(varName == "ToggleMouselooking")				
		//	keys[keyToggleMouselooking].alt = UniKey(value);
		//else if(varName == "Debug3rdp")				
		//	keys[keyDebug3rdp].alt = UniKey(value);
		//else if(varName == "Debugshowlines")				
		//	keys[keyDebugshowlines].alt = UniKey(value);
		//else if(varName == "DragItem")				
		//	keys[keyDragItem].alt = UniKey(value);
  //      keysElem = keysElem->NextSiblingElement("key");
  //  }
}

TiXmlElement *GameSettings::getAsXml()
{	
	TiXmlElement *elem = new TiXmlElement("settings");
	OgreXmlElement *optionsElem = new OgreXmlElement("options");
	elem->LinkEndChild(optionsElem);



	//<settings>
	

    OgreXmlElement *option = new OgreXmlElement("option");
	optionsElem->LinkEndChild(option);
	option->SetAttribute("name","mouselooking");
	option->SetOgreAttribute("value",mouselooking);

	option = new OgreXmlElement("option");
	optionsElem->LinkEndChild(option);
	option->SetAttribute("name","sensibility_x_mouse");
	option->SetOgreAttribute("value",sensibility_x_mouse);

	option = new OgreXmlElement("option");
	optionsElem->LinkEndChild(option);
	option->SetAttribute("name","sensibility_y_mouse");
	option->SetOgreAttribute("value",sensibility_y_mouse);

	option = new OgreXmlElement("option");
	optionsElem->LinkEndChild(option);
	option->SetAttribute("name","physics_fps");
	option->SetOgreAttribute("value",physics_fps);

	option = new OgreXmlElement("option");
	optionsElem->LinkEndChild(option);
	option->SetAttribute("name","sensibility_x_keyboard");
	option->SetOgreAttribute("value",sensibility_x_keyboard);

	option = new OgreXmlElement("option");
	optionsElem->LinkEndChild(option);
	option->SetAttribute("name","gain_all");
	option->SetOgreAttribute("value",gain_all);

	option = new OgreXmlElement("option");
	optionsElem->LinkEndChild(option);
	option->SetAttribute("name","gain_sfx");
	option->SetOgreAttribute("value",gain_sfx);

	option = new OgreXmlElement("option");
	optionsElem->LinkEndChild(option);
	option->SetAttribute("name","gain_speech");
	option->SetOgreAttribute("value",gain_speech);

	option = new OgreXmlElement("option");
	optionsElem->LinkEndChild(option);
	option->SetAttribute("name","gain_music");
	option->SetOgreAttribute("value",gain_music);

	option = new OgreXmlElement("option");
	optionsElem->LinkEndChild(option);
	option->SetAttribute("name","gain_player_sfx");
	option->SetOgreAttribute("value",gain_player_sfx);

	option = new OgreXmlElement("option");
	optionsElem->LinkEndChild(option);
	option->SetAttribute("name","rolloff_factor");
	option->SetOgreAttribute("value",rolloff_factor);

	option = new OgreXmlElement("option");
	optionsElem->LinkEndChild(option);
	option->SetAttribute("name","ref_distance");
	option->SetOgreAttribute("value",ref_distance);

	option = new OgreXmlElement("option");
	optionsElem->LinkEndChild(option);
	option->SetAttribute("name","max_distance");
	option->SetOgreAttribute("value",max_distance);

	option = new OgreXmlElement("option");
	optionsElem->LinkEndChild(option);
	option->SetAttribute("name","distance_model");
	option->SetOgreAttribute("value",distance_model);

    option = new OgreXmlElement("option");
	optionsElem->LinkEndChild(option);
	option->SetAttribute("name","starting_level");
	option->SetOgreAttribute("value",starting_level);

	option = new OgreXmlElement("option");
	optionsElem->LinkEndChild(option);
	option->SetAttribute("name","starting_entrance");
	option->SetOgreAttribute("value",starting_entrance);

	option = new OgreXmlElement("option");
	optionsElem->LinkEndChild(option);
	option->SetAttribute("name","default_music");
	option->SetOgreAttribute("value",default_music);
	
	option = new OgreXmlElement("option");
	optionsElem->LinkEndChild(option);
	option->SetAttribute("name","default_combat_music");
	option->SetOgreAttribute("value",default_combat_music);
	//

	//keys. looping and stuff
	//NEW system: <key name="MoveFoward" main="KC_W" alt="KC_NUMPAD8" />
	OgreXmlElement *keyElem = new OgreXmlElement("keys");
	elem->LinkEndChild(keyElem);  
	GameApp *app = GameApp::getSingletonPtr();
	for(unsigned int i=1;i<_num_keys;i++)
	{		
		//begin at 1, we do not need to define keyNone
		OgreXmlElement *key = new OgreXmlElement("key");
		keyElem->LinkEndChild(key);
		key->SetAttribute("name",app->keyNameToString(i));
		key->SetAttribute("main",keys[i].main.toString());
		key->SetAttribute("alt",keys[i].alt.toString());
	}

	////<keys_main>
	//OgreXmlElement *keyElem = new OgreXmlElement("keys_main");
 //   elem->LinkEndChild(keyElem);  

	//OgreXmlElement *key = new OgreXmlElement("key");
	//keyElem->LinkEndChild(key);
	//key->SetAttribute("name","MoveFoward");
	//key->SetAttribute("value",keys[keyMoveForward].main.toString());

	//key = new OgreXmlElement("key");
	//keyElem->LinkEndChild(key);
	//key->SetAttribute("name","MoveBack");
	//key->SetAttribute("value",keys[keyMoveBack].main.toString()); 

	//key = new OgreXmlElement("key");
	//keyElem->LinkEndChild(key);
	//key->SetAttribute("name","MoveRight");
	//key->SetAttribute("value",keys[keyMoveRight].main.toString()); 

	//key = new OgreXmlElement("key");
	//keyElem->LinkEndChild(key);
	//key->SetAttribute("name","MoveLeft");
	//key->SetAttribute("value",keys[keyMoveLeft].main.toString());

	//key = new OgreXmlElement("key");
	//keyElem->LinkEndChild(key);
	//key->SetAttribute("name","Jump");
	//key->SetAttribute("value",keys[keyJump].main.toString()); 

	//key = new OgreXmlElement("key");
	//keyElem->LinkEndChild(key);
	//key->SetAttribute("name","Attack");
	//key->SetAttribute("value",keys[keyAttack].main.toString());

	//key = new OgreXmlElement("key");
	//keyElem->LinkEndChild(key);
	//key->SetAttribute("name","Spellcast");
	//key->SetAttribute("value",keys[keySpellcast].main.toString()); 

	//key = new OgreXmlElement("key");
	//keyElem->LinkEndChild(key);
	//key->SetAttribute("name","Run");
	//key->SetAttribute("value",keys[keyRun].main.toString()); 

	//key = new OgreXmlElement("key");
	//keyElem->LinkEndChild(key);
	//key->SetAttribute("name","Action");
	//key->SetAttribute("value",keys[keyAction].main.toString()); 

	//key = new OgreXmlElement("key");
	//keyElem->LinkEndChild(key);
	//key->SetAttribute("name","Inventory");
	//key->SetAttribute("value",keys[keyInventory].main.toString()); 

	//key = new OgreXmlElement("key");
	//keyElem->LinkEndChild(key);
	//key->SetAttribute("name","InvTakeOne");
	//key->SetAttribute("value",keys[keyInvTakeOne].main.toString()); 

	//key = new OgreXmlElement("key");
	//keyElem->LinkEndChild(key);
	//key->SetAttribute("name","InvTakeAll");
	//key->SetAttribute("value",keys[keyInvTakeAll].main.toString()); 

	//key = new OgreXmlElement("key");
	//keyElem->LinkEndChild(key);
	//key->SetAttribute("name","InvUse");
	//key->SetAttribute("value",keys[keyInvUse].main.toString()); 

	//key = new OgreXmlElement("key");
	//keyElem->LinkEndChild(key);
	//key->SetAttribute("name","Screenshot");
	//key->SetAttribute("value",keys[keyScreenshot].main.toString()); 

	//key = new OgreXmlElement("key");
	//keyElem->LinkEndChild(key);
	//key->SetAttribute("name","Mainmenu");
	//key->SetAttribute("value",keys[keyMainmenu].main.toString()); 

	//key = new OgreXmlElement("key");
	//keyElem->LinkEndChild(key);
	//key->SetAttribute("name","LookUp");
	//key->SetAttribute("value",keys[keyLookUp].main.toString()); 

	//key = new OgreXmlElement("key");
	//keyElem->LinkEndChild(key);
	//key->SetAttribute("name","LookDown");
	//key->SetAttribute("value",keys[keyLookDown].main.toString()); 

	//key = new OgreXmlElement("key");
	//keyElem->LinkEndChild(key);
	//key->SetAttribute("name","LookLeft");
	//key->SetAttribute("value",keys[keyLookLeft].main.toString()); 

	//key = new OgreXmlElement("key");
	//keyElem->LinkEndChild(key);
	//key->SetAttribute("name","LookRight");
	//key->SetAttribute("value",keys[keyLookRight].main.toString()); 

	//key = new OgreXmlElement("key");
	//keyElem->LinkEndChild(key);
	//key->SetAttribute("name","ToggleMouselooking");
	//key->SetAttribute("value",keys[keyToggleMouselooking].main.toString());

	//key = new OgreXmlElement("key");
	//keyElem->LinkEndChild(key);
	//key->SetAttribute("name","Debug3rdp");
	//key->SetAttribute("value",keys[keyDebug3rdp].main.toString());

	//key = new OgreXmlElement("key");
	//keyElem->LinkEndChild(key);
	//key->SetAttribute("name","Debugshowlines");
	//key->SetAttribute("value",keys[keyDebugshowlines].main.toString());

	//key = new OgreXmlElement("key");
	//keyElem->LinkEndChild(key);
	//key->SetAttribute("name","DragItem");
	//key->SetAttribute("value",keys[keyDragItem].main.toString());

	////<keys_alt>
	//keyElem = new OgreXmlElement("keys_alt");
 //   elem->LinkEndChild(keyElem);  
	//	
	//key = new OgreXmlElement("key");
	//keyElem->LinkEndChild(key);
	//key->SetAttribute("name","MoveFoward");
	//key->SetAttribute("value",keys[keyMoveForward].alt.toString());
	//	
	//key = new OgreXmlElement("key");
	//keyElem->LinkEndChild(key);
	//key->SetAttribute("name","MoveBack");
	//key->SetAttribute("value",keys[keyMoveBack].alt.toString()); 
	//	
	//key = new OgreXmlElement("key");
	//keyElem->LinkEndChild(key);
	//key->SetAttribute("name","MoveRight");
	//key->SetAttribute("value",keys[keyMoveRight].alt.toString()); 
	//	
	//key = new OgreXmlElement("key");
	//keyElem->LinkEndChild(key);
	//key->SetAttribute("name","MoveLeft");
	//key->SetAttribute("value",keys[keyMoveLeft].alt.toString());
	//	
	//key = new OgreXmlElement("key");
	//keyElem->LinkEndChild(key);
	//key->SetAttribute("name","Jump");
	//key->SetAttribute("value",keys[keyJump].alt.toString()); 
	//	
	//key = new OgreXmlElement("key");
	//keyElem->LinkEndChild(key);
	//key->SetAttribute("name","Attack");
	//key->SetAttribute("value",keys[keyAttack].alt.toString());
	//	
	//key = new OgreXmlElement("key");
	//keyElem->LinkEndChild(key);
	//key->SetAttribute("name","Spellcast");
	//key->SetAttribute("value",keys[keySpellcast].alt.toString()); 
	//	
	//key = new OgreXmlElement("key");
	//keyElem->LinkEndChild(key);
	//key->SetAttribute("name","Run");
	//key->SetAttribute("value",keys[keyRun].alt.toString()); 
	//	
	//key = new OgreXmlElement("key");
	//keyElem->LinkEndChild(key);
	//key->SetAttribute("name","Action");
	//key->SetAttribute("value",keys[keyAction].alt.toString()); 
	//	
	//key = new OgreXmlElement("key");
	//keyElem->LinkEndChild(key);
	//key->SetAttribute("name","Inventory");
	//key->SetAttribute("value",keys[keyInventory].alt.toString()); 
	//	
	//key = new OgreXmlElement("key");
	//keyElem->LinkEndChild(key);
	//key->SetAttribute("name","InvTakeOne");
	//key->SetAttribute("value",keys[keyInvTakeOne].alt.toString()); 
	//	
	//key = new OgreXmlElement("key");
	//keyElem->LinkEndChild(key);
	//key->SetAttribute("name","InvTakeAll");
	//key->SetAttribute("value",keys[keyInvTakeAll].alt.toString()); 
	//	
	//key = new OgreXmlElement("key");
	//keyElem->LinkEndChild(key);
	//key->SetAttribute("name","InvUse");
	//key->SetAttribute("value",keys[keyInvUse].alt.toString()); 
	//	
	//key = new OgreXmlElement("key");
	//keyElem->LinkEndChild(key);
	//key->SetAttribute("name","Screenshot");
	//key->SetAttribute("value",keys[keyScreenshot].alt.toString()); 
	//	
	//key = new OgreXmlElement("key");
	//keyElem->LinkEndChild(key);
	//key->SetAttribute("name","Mainmenu");
	//key->SetAttribute("value",keys[keyMainmenu].alt.toString()); 
	//	
	//key = new OgreXmlElement("key");
	//keyElem->LinkEndChild(key);
	//key->SetAttribute("name","LookUp");
	//key->SetAttribute("value",keys[keyLookUp].alt.toString()); 
	//	
	//key = new OgreXmlElement("key");
	//keyElem->LinkEndChild(key);
	//key->SetAttribute("name","LookDown");
	//key->SetAttribute("value",keys[keyLookDown].alt.toString()); 
	//	
	//key = new OgreXmlElement("key");
	//keyElem->LinkEndChild(key);
	//key->SetAttribute("name","LookLeft");
	//key->SetAttribute("value",keys[keyLookLeft].alt.toString()); 
	//	
	//key = new OgreXmlElement("key");
	//keyElem->LinkEndChild(key);
	//key->SetAttribute("name","LookRight");
	//key->SetAttribute("value",keys[keyLookRight].alt.toString()); 
	//	
	//key = new OgreXmlElement("key");
	//keyElem->LinkEndChild(key);
	//key->SetAttribute("name","ToggleMouselooking");
	//key->SetAttribute("value",keys[keyToggleMouselooking].alt.toString());
	//	
	//key = new OgreXmlElement("key");
	//keyElem->LinkEndChild(key);
	//key->SetAttribute("name","Debug3rdp");
	//key->SetAttribute("value",keys[keyDebug3rdp].alt.toString());
	//	
	//key = new OgreXmlElement("key");
	//keyElem->LinkEndChild(key);
	//key->SetAttribute("name","Debugshowlines");
	//key->SetAttribute("value",keys[keyDebugshowlines].alt.toString());

	//key = new OgreXmlElement("key");
	//keyElem->LinkEndChild(key);
	//key->SetAttribute("name","DragItem");
	//key->SetAttribute("value",keys[keyDragItem].alt.toString());

	return elem;
}