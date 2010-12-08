#include <OptionsWindow.h>
#include "GameApp.h"
#include "CEGUI.h"
#include "TypeConverter.h"
#include <elements/CEGUIPushButton.h>
#include <elements/CEGUIScrollablePane.h>
#include <elements/CEGUICombobox.h>
#include <elements/CEGUIListboxTextItem.h>
#include <elements/CEGUITabControl.h>
#include <elements/CEGUIScrolledContainer.h>
#include "functions.h"

using namespace TypeConverter;

/****************************RendererOptionsWindow***************************/
RendererOptionsWindow::RendererOptionsWindow(Ogre::RenderSystem* renderSystem)
{
	using namespace CEGUI;

	createPrefix();
	mWnd = createWindow("ScrollablePane","mPane");
	
	mWnd->setArea(UDim(0,0),UDim(0,0),UDim(1,0),UDim(1,0));

	CEGUI::ScrollablePane *pane = static_cast<CEGUI::ScrollablePane *>(mWnd);
	//pane->setContentPaneAutoSized(false);
	//pane->setve

	Ogre::Root *mRoot = Ogre::Root::getSingletonPtr();
	
	mRenderSystem = renderSystem;

	ConfigOptionMap options = mRenderSystem->getConfigOptions();
	int hOffset = 0;
	for(ConfigOptionMap::iterator it2 = options.begin();it2!=options.end();it2++)
	{
		//create dropdown menus for all the options
		Ogre::String name = it2->first;
		ConfigOption opt = it2->second;

		UDim curY(0,40*hOffset);

		addOptionSelector(opt,curY);

		hOffset++;
	}
	
	pane->setVerticalStepSize(0.1);
	//pane->setVerticalOverlapSize(2);
	
}
RendererOptionsWindow::~RendererOptionsWindow()
{
	destroyLayout();
}

void RendererOptionsWindow::applyOptions()
{
	for(RenderOptionMap::iterator itr = mOptions.begin();itr!=mOptions.end();itr++)
	{
		Ogre::String name = itr->first;
		Ogre::String value = "";
		CEGUI::Window *wnd = itr->second;
		CEGUI::String type = getWindowType(wnd);
		if(type == "Checkbox")
		{
			CEGUI::Checkbox *box = static_cast<CEGUI::Checkbox*>(wnd);
			if(box->isSelected())
				value = "Yes";
			else
				value = "No";
		}
		else
		{
			value = wnd->getText().c_str();
		}

	}
}


void RendererOptionsWindow::addOptionSelector(Ogre::ConfigOption option, CEGUI::UDim yPos, bool useCheckboxes)
{
	using namespace CEGUI;
	//how much options do we have?
	unsigned int numValues = option.possibleValues.size();
	CEGUI::String name = TypeConverter::cegui_str(option.name);
		//maybe a checkbox?
	if(
		useCheckboxes && numValues == 2 && 
		(
			(option.possibleValues[0] == "Yes" && option.possibleValues[1] == "No") ||
			(option.possibleValues[1] == "Yes" && option.possibleValues[0] == "No")   
		))
	{
		//yes
		UDim labelX = UDim(0,20);
		UDim width = UDim(0.5,-20);
		UDim height = UDim(0,25);

		CEGUI::Checkbox *checkbox = (CEGUI::Checkbox*)createWindow("Checkbox",name+"_value");
		mWnd->addChildWindow(checkbox);
		checkbox->setPosition(UVector2(labelX,yPos));
		checkbox->setSize(UVector2(width,height));
		checkbox->setText(name.c_str());
		
		
		checkbox->setSelected(option.currentValue == "Yes");

		mOptions[option.name] = checkbox;
	}
	else
	{
		//no
		UDim labelX = UDim(0,20);
		UDim cbX = UDim(0.5,20);
		UDim width = UDim(0.5,-50);
		UDim height = UDim(0,25);
		UDim checkboxheight(0,200);

		CEGUI::Window *curText = createWindow("StaticText",name+"_label");
		mWnd->addChildWindow(curText);
		//curText->setArea(URect(labelX,yPos,UDim(0.5,-20),UDim(0,95+40*hOffset)));
		curText->setPosition(UVector2(labelX,yPos));
		curText->setSize(UVector2(width,height));
		curText->setText(name.c_str());

		CEGUI::Combobox *curCB = static_cast<CEGUI::Combobox*>(createWindow("Combobox",name+"_value"));
		mWnd->addChildWindow(curCB);
		curCB->setReadOnly(true);
		//curCB->setArea(URect(UDim(0.5,20),UDim(0,70+40*hOffset),UDim(1,-20),UDim(0,200+50*hOffset)));
		curCB->setPosition(UVector2(cbX,yPos));
		curCB->setSize(UVector2(width,height));
		curCB->setText(option.currentValue.c_str());

		float cbPixelHeight = 0;
		//adding values
		for(Ogre::StringVector::iterator it3 = option.possibleValues.begin();it3 != option.possibleValues.end();it3++)
		{
			Ogre::String val = *it3;
			CEGUI::ListboxTextItem *item = new ListboxTextItem(val.c_str());
			item->setSelectionBrushImage("LandsLook","TextSelectionBrush");
			curCB->addItem(item);
			Size itemSize = item->getPixelSize();
			cbPixelHeight += itemSize.d_height;
		}
		
		curCB->setHeight(checkboxheight);//CEGUI::UDim(0,cbPixelHeight));
		sizeComboList(curCB);
		mOptions[option.name] = curCB;
	}
}

/*****************OptionsWindow*******************/
OptionsWindow::OptionsWindow()	
{
	//key exception list!
	keyExceptionList[0] = keyNone;
	keyExceptionList[1] = keyMainmenu;
	keyExceptionList[2] = keyDebug3rdp;
	keyExceptionList[3] = keyDebugshowlines;

	loadLayout("OptionsWindow.layout");

	//getting elements
	tabCtrl = static_cast<CEGUI::TabControl*>(getWindow("TabControl"));
	
	okButton = static_cast<CEGUI::PushButton*>(getWindow("OkButton"));
	cancelButton = static_cast<CEGUI::PushButton*>(getWindow("CancelButton"));
	applyButton = static_cast<CEGUI::PushButton*>(getWindow("AcceptButton"));

	rendererSelectBox = static_cast<CEGUI::Combobox*>(getWindow("RendererSelect"));

	videoOptionContainer = static_cast<CEGUI::ScrollablePane*>(getWindow("VideoOptionContainer"));
	keyboardScrollPane = static_cast<CEGUI::ScrollablePane*>(getWindow("KeyboardScroll"));

	//elements: audio
	gainAll = static_cast<CEGUI::Slider*>(getWindow("options/gainAll"));
	gainAll->setMaxValue(1);
	gainAll->setClickStep(0.01);

	gainMusic = static_cast<CEGUI::Slider*>(getWindow("options/gainMusic"));
	gainMusic->setMaxValue(1);
	gainMusic->setClickStep(0.01);

	gainSfx = static_cast<CEGUI::Slider*>(getWindow("options/gainSFX"));
	gainSfx->setMaxValue(1);
	gainSfx->setClickStep(0.01);

	gainSfxPlayer = static_cast<CEGUI::Slider*>(getWindow("options/gainSFXPlayer"));
	gainSfxPlayer->setMaxValue(1);
	gainSfxPlayer->setClickStep(0.01);

	gainSpeech = static_cast<CEGUI::Slider*>(getWindow("options/gainSpeech"));
	gainSpeech->setMaxValue(1);
	gainSpeech->setClickStep(0.01);	
	

	//setupScrollbar(gainAll);
	//setupScrollbar(gainMusic);
	//setupScrollbar(gainSfx);
	//setupScrollbar(gainSfxPlayer);
	//setupScrollbar(gainSpeech);

	sndDistanceModel = static_cast<CEGUI::Combobox*>(getWindow("options/distance_model"));
	sndMaxDistance = static_cast<CEGUI::Spinner*>(getWindow("options/maxdist"));
	sndRefDistance = static_cast<CEGUI::Spinner*>(getWindow("options/refdist"));
	sndRoloffFactor = static_cast<CEGUI::Spinner*>(getWindow("options/rolloff"));

	//elements: game
	sensibilityX = static_cast<CEGUI::Slider*>(getWindow("options/sens_x"));
	sensibilityX->setMaxValue(1);
	sensibilityX->setClickStep(0.01);	

	sensibilityY = static_cast<CEGUI::Slider*>(getWindow("options/sens_y"));
	sensibilityY->setMaxValue(1);
	sensibilityY->setClickStep(0.01);	

	sensibilityX_keyboard = static_cast<CEGUI::Slider*>(getWindow("options/kb_sens_x"));
	sensibilityX_keyboard->setMaxValue(1);
	sensibilityX_keyboard->setClickStep(0.01);	

	sensibilityY_keyboard = static_cast<CEGUI::Slider*>(getWindow("options/kb_sens_y"));
	sensibilityY_keyboard->setMaxValue(1);
	sensibilityY_keyboard->setClickStep(0.01);	

	physicsPrecision = static_cast<CEGUI::Spinner*>(getWindow("options/physicval"));
	physicsPrecision->setStepSize(1);
	physicsPrecision->setMinimumValue(30);
	physicsPrecision->setMaximumValue(300);
	//physicsPrecision->setMaxValue(270);//because it's 30-300
	//physicsPrecision->setClickStep(1);	

	/*physicsPrecisionDisplay = getWindow("options/physicval_display");
	physicsPrecision->subscribeEvent(CEGUI::Slider::EventValueChanged,CEGUI::Event::Subscriber(&OptionsWindow::physicBarChanged, this));*/

	mouseLooking = static_cast<CEGUI::Checkbox*>(getWindow("options/mouselooking"));

	//elements: keyboard
	keyboardInfoText = getWindow("keyboardInfoText");	
	restoreDefaultKeysBtn = static_cast<CEGUI::PushButton*>(getWindow("RestoreDefaultKeys"));

	/*
	CEGUI::Combobox *distMod = (CEGUI::Combobox*)wmgr->getWindow("options/distance_model");
*/
	sndDistanceModel->setReadOnly(true);

	CEGUI::ListboxTextItem *itemINVERSE_DISTANCE = new CEGUI::ListboxTextItem("INVERSE_DISTANCE");
	itemINVERSE_DISTANCE->setSelectionBrushImage("LandsLook","TextSelectionBrush");
	sndDistanceModel->addItem(itemINVERSE_DISTANCE);

	CEGUI::ListboxTextItem *itemINVERSE_DISTANCE_CLAMPED = new CEGUI::ListboxTextItem("INVERSE_DISTANCE_CLAMPED");
	itemINVERSE_DISTANCE_CLAMPED->setSelectionBrushImage("LandsLook","TextSelectionBrush");
	sndDistanceModel->addItem(itemINVERSE_DISTANCE_CLAMPED);

	CEGUI::ListboxTextItem *itemLINEAR_DISTANCE = new CEGUI::ListboxTextItem("LINEAR_DISTANCE");
	itemLINEAR_DISTANCE->setSelectionBrushImage("LandsLook","TextSelectionBrush");
	sndDistanceModel->addItem(itemLINEAR_DISTANCE);

	CEGUI::ListboxTextItem *itemLINEAR_DISTANCE_CLAMPED = new CEGUI::ListboxTextItem("LINEAR_DISTANCE_CLAMPED");
	itemLINEAR_DISTANCE_CLAMPED->setSelectionBrushImage("LandsLook","TextSelectionBrush");
	sndDistanceModel->addItem(itemLINEAR_DISTANCE_CLAMPED);

	CEGUI::ListboxTextItem *itemEXPONENT_DISTANCE = new CEGUI::ListboxTextItem("EXPONENT_DISTANCE");
	itemEXPONENT_DISTANCE->setSelectionBrushImage("LandsLook","TextSelectionBrush");
	sndDistanceModel->addItem(itemEXPONENT_DISTANCE);

	CEGUI::ListboxTextItem *itemEXPONENT_DISTANCE_CLAMPED = new CEGUI::ListboxTextItem("EXPONENT_DISTANCE_CLAMPED");
	itemEXPONENT_DISTANCE_CLAMPED->setSelectionBrushImage("LandsLook","TextSelectionBrush");
	sndDistanceModel->addItem(itemEXPONENT_DISTANCE_CLAMPED);

	//subscribing
	rendererSelectBox->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted,CEGUI::Event::Subscriber(&OptionsWindow::rendererChanged, this));

	//rendererDropdown->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted,Event::Subscriber(&OptionsWindow::rendererChanged, this));

	okButton->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&OptionsWindow::okClick, this));
	cancelButton->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&OptionsWindow::cancelClick, this));
	applyButton->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&OptionsWindow::acceptClick, this));
	restoreDefaultKeysBtn->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&OptionsWindow::keysRestoreClick, this));	
}

bool OptionsWindow::isInExceptionList(int keyname)
{
	for(unsigned int i=0;i<_num_exceptions;i++)
	{
		if(keyExceptionList[i] == keyname)
			return true;
	}
	return false;
}


void OptionsWindow::updateShownSettings()
{
	updateVideoTab();
	updateAudioTab();
	updateKeyboardTab();
	updateGameTab();
	tabCtrl->setSelectedTab(0);
}


//void OptionsWindow::fillKeyExceptionList()
//{
//
//}


Ogre::String OptionsWindow::keyToString(UniKey key)
{
	if(key.isEmpty())
		return "- unassigned -";

	Ogre::String result = "";

	if(key.hasModifier())
	{
		switch(key.getModifier())
		{
			case OIS::Keyboard::Shift:
                result += "Shift+";
				break;
            case OIS::Keyboard::Alt:
                result += "Alt+";
				break;
            default:
                result += "Ctrl+";
		}
	}
	if(key.isKeyboard)
	{	
		switch(key.keyboardID)
		{
			//case KC_UNASSIGNED: return "- unassigned -";
			case KC_ESCAPE:		return result+"Escape";
			case KC_1:			return result+"1";
			case KC_2:			return result+"2";
			case KC_3:			return result+"3";
			case KC_4:			return result+"4";
			case KC_5:			return result+"5";
			case KC_6:			return result+"6";
			case KC_7:			return result+"7";
			case KC_8:			return result+"8";
			case KC_9:			return result+"9";
			case KC_0:			return result+"0";
			case KC_MINUS:		return result+"-";    // - on main keyboard
			case KC_EQUALS:		return result+"=";
			case KC_BACK:		return result+"Backspace";    // backspace
			case KC_TAB:		return result+"Tab";
			case KC_Q:			return result+"Q";
			case KC_W:			return result+"W";
			case KC_E:			return result+"E";
			case KC_R:			return result+"R";
			case KC_T:			return result+"T";
			case KC_Y:			return result+"Y";
			case KC_U:			return result+"U";
			case KC_I:			return result+"I";
			case KC_O:			return result+"O";
			case KC_P:			return result+"P";
			case KC_LBRACKET:	return result+"(";
			case KC_RBRACKET:	return result+")";
			case KC_RETURN:		return result+"Ender";    // Enter on main keyboard
			case KC_LCONTROL:	return result+"Left Ctrl";
			case KC_A:			return result+"A";
			case KC_S:			return result+"S";
			case KC_D:			return result+"D";
			case KC_F:			return result+"F";
			case KC_G:			return result+"G";
			case KC_H:			return result+"H";
			case KC_J:			return result+"J";
			case KC_K:			return result+"K";
			case KC_L:			return result+"L";
			case KC_SEMICOLON:	return result+";";
			case KC_APOSTROPHE: return result+"'";
			case KC_GRAVE:		return result+"`";    // accent
			case KC_LSHIFT:		return result+"Left Shift";
			case KC_BACKSLASH:	return result+"\\";
			case KC_Z:			return result+"Z";
			case KC_X:			return result+"X";
			case KC_C:			return result+"C";
			case KC_V:			return result+"V";
			case KC_B:			return result+"B";
			case KC_N:			return result+"N";
			case KC_M:			return result+"M";
			case KC_COMMA:		return result+",";
			case KC_PERIOD:		return result+".";    // . on main keyboard
			case KC_SLASH:		return result+"/";    // / on main keyboard
			case KC_RSHIFT:		return result+"Right Shift";
			case KC_MULTIPLY:	return result+"*";    // * on numeric keypad
			case KC_LMENU:		return result+"Left Alt";    // left Alt
			case KC_SPACE:		return result+"Spacebar";
			case KC_CAPITAL:	return result+"CAPITAL";
			case KC_F1:			return result+"F1";
			case KC_F2:			return result+"F2";
			case KC_F3:			return result+"F3";
			case KC_F4:			return result+"F4";
			case KC_F5:			return result+"F5";
			case KC_F6:			return result+"F6";
			case KC_F7:			return result+"F7";
			case KC_F8:			return result+"F8";
			case KC_F9:			return result+"F9";
			case KC_F10:		return result+"F10";
			case KC_NUMLOCK:	return result+"Num Lock";
			case KC_SCROLL:		return result+"Scroll Lock";    // Scroll Lock
			case KC_NUMPAD7:	return result+"7 (Numpad)";
			case KC_NUMPAD8:	return result+"8 (Numpad)";
			case KC_NUMPAD9:	return result+"9 (Numpad)";
			case KC_SUBTRACT:	return result+"- (Numpad)";    // - on numeric keypad
			case KC_NUMPAD4:	return result+"4 (Numpad)";
			case KC_NUMPAD5:	return result+"5 (Numpad)";
			case KC_NUMPAD6:	return result+"6 (Numpad)";
			case KC_ADD:		return result+"+ (Numpad)";    // + on numeric keypad
			case KC_NUMPAD1:	return result+"1 (Numpad)";
			case KC_NUMPAD2:	return result+"2 (Numpad)";
			case KC_NUMPAD3:	return result+"3 (Numpad)";
			case KC_NUMPAD0:	return result+"0 (Numpad)";
			case KC_DECIMAL:	return result+". (Numpad)";    // . on numeric keypad
			case KC_OEM_102:	return result+"Oem 102";    // < > | on UK/Germany keyboards
			case KC_F11:		return result+"F11";
			case KC_F12:		return result+"F12";
			case KC_F13:		return result+"F13";    //                     (NEC PC98)
			case KC_F14:		return result+"F14";    //                     (NEC PC98)
			case KC_F15:		return result+"F15";    //                     (NEC PC98)
			case KC_KANA:		return result+"Kana";    // (Japanese keyboard)
			case KC_ABNT_C1:	return result+"ABNT_C1";    // / ? on Portugese (Brazilian) keyboards
			case KC_CONVERT:	return result+"Convert";    // (Japanese keyboard)
			case KC_NOCONVERT:	return result+"Noconvert";    // (Japanese keyboard)
			case KC_YEN:		return result+"Yen";    // (Japanese keyboard)
			case KC_ABNT_C2:	return result+"ABNT_C2";    // Numpad . on Portugese (Brazilian) keyboards
			case KC_NUMPADEQUALS: return result+"= (Numeric Keypad)";    // = on numeric keypad (NEC PC98)
			case KC_PREVTRACK:	return result+"Prev Track";    // Previous Track (KC_CIRCUMFLEX on Japanese keyboard)
			case KC_AT:			return result+"AT";    //                     (NEC PC98)
			case KC_COLON:		return result+"Colon";    //                     (NEC PC98)
			case KC_UNDERLINE:	return result+"Underline";    //                     (NEC PC98)
			case KC_KANJI:		return result+"Kanji";    // (Japanese keyboard)
			case KC_STOP:		return result+"Stop";    //                     (NEC PC98)
			case KC_AX:			return result+"AX";    //                     (Japan AX)
			case KC_UNLABELED:	return result+"Unlabeled";    //                        (J3100)
			case KC_NEXTTRACK:	return result+"Next Track";    // Next Track
			case KC_NUMPADENTER: return result+"Enter (Numpad)";    // Enter on numeric keypad
			case KC_RCONTROL:	return result+"Right Ctrl";
			case KC_MUTE:		return result+"Mute";    // Mute
			case KC_CALCULATOR: return result+"Calculator";    // Calculator
			case KC_PLAYPAUSE:	return result+"Play/Pause";    // Play / Pause
			case KC_MEDIASTOP:	return result+"Media Stop";    // Media Stop
			case KC_VOLUMEDOWN: return result+"Volume -";    // Volume -
			case KC_VOLUMEUP:	return result+"Volume +";    // Volume +
			case KC_WEBHOME:	return result+"Web Home";    // Web home
			case KC_NUMPADCOMMA: return result+", (Numeric Keypad)";    // , on numeric keypad (NEC PC98)
			case KC_DIVIDE:		return result+"/ (Numpad)";    // / on numeric keypad
			case KC_SYSRQ:		return result+"SysRq";
			case KC_RMENU:		return result+"Right Alt";    // right Alt
			case KC_PAUSE:		return result+"Pause";    // Pause
			case KC_HOME:		return result+"Home";    // Home on arrow keypad
			case KC_UP:			return result+"Up";    // UpArrow on arrow keypad
			case KC_PGUP:		return result+"Page Up";    // PgUp on arrow keypad
			case KC_LEFT:		return result+"Left";    // LeftArrow on arrow keypad
			case KC_RIGHT:		return result+"Right";    // RightArrow on arrow keypad
			case KC_END:		return result+"End";    // End on arrow keypad
			case KC_DOWN:		return result+"Down";    // DownArrow on arrow keypad
			case KC_PGDOWN:		return result+"Page Down";    // PgDn on arrow keypad
			case KC_INSERT:		return result+"Insert";    // Insert on arrow keypad
			case KC_DELETE:		return result+"Delete";    // Delete on arrow keypad
			case KC_LWIN:		return result+"Left Windows";    // Left Windows key
			case KC_RWIN:		return result+"Right Windows";    // Right Windows key
			case KC_APPS:		return result+"Apps";    // AppMenu key
			case KC_POWER:		return result+"Power";    // System Power
			case KC_SLEEP:		return result+"Sleep";    // System Sleep
			case KC_WAKE:		return result+"Wake";    // System Wake
			case KC_WEBSEARCH:	return result+"Web Search";    // Web Search
			case KC_WEBFAVORITES: return result+"Web Favorites";    // Web Favorites
			case KC_WEBREFRESH: return result+"Web Refresh";    // Web Refresh
			case KC_WEBSTOP:	return result+"Web Stop";    // Web Stop
			case KC_WEBFORWARD: return result+"Web Forward";    // Web Forward
			case KC_WEBBACK:	return result+"Web Back";    // Web Back
			case KC_MYCOMPUTER: return result+"My Computer";    // My Computer
			case KC_MAIL:		return result+"Mail";    // Mail
			case KC_MEDIASELECT: return result+"Media Select";     // Media Select
		}
	}
	else
	{
		switch(key.mouseID)
		{
		case MB_Left:
			return result+"Mouse Left";
		case MB_Right:
			return result+"Mouse Right";
		case MB_Middle:
			return result+"Mouse Middle";
		case MB_Button3:
			return result+"Mouse 3";
		case MB_Button4:
			return result+"Mouse 4";
		case MB_Button5:
			return result+"Mouse 5";
		case MB_Button6:
			return result+"Mouse 6";
		case MB_Button7:
			return result+"Mouse 7";
		}
	}
	return "- illegal key -";
}

void OptionsWindow::updateGameTab()
{
	GameApp *app = GameApp::getSingletonPtr();
	sensibilityX->setCurrentValue(app->settings.sensibility_x_mouse);
	sensibilityY->setCurrentValue(app->settings.sensibility_x_mouse);

	sensibilityX_keyboard->setCurrentValue(app->settings.sensibility_x_keyboard);
	sensibilityY_keyboard->setCurrentValue(app->settings.sensibility_y_keyboard);

	physicsPrecision->setCurrentValue(app->settings.physics_fps);
	
	mouseLooking->setSelected(app->settings.mouselooking);
}

//updates the keyboard tab with a given key list
void OptionsWindow::updateKeyboardTabWith(KeyPair keys[GameSettings::_num_keys])
{
	using namespace CEGUI;
	
	size_t cCount = keyboardScrollPane->getContentPane()->getChildCount();
	int hOffset = 0;
	//size_t delIndex = 0;
	if(cCount > 0)
	{
		for(size_t i=0;i<cCount;i++)
		{
			destroyWindow(keyboardScrollPane->getContentPane()->getChildAtIdx(0));
		}
	}	
	//some UDims
	UDim mostLeftX = UDim(0,20);//x coord for the leftmost element
	UDim mainKeyX = UDim(0.5,20);
	UDim altKeyX = UDim(0.75,0);
	UDim curY = UDim(0,0);//y coord for everything. stuff is added on each iteration
	UDim incY = UDim(0,40);
	UDim labelWidth = UDim(0.5,-20);//width of the label
	UDim mainKeyWidth = UDim(0.20,0);//"width" of mainKey
	UDim height = UDim(0,25);
	//<Property Name="UnifiedAreaRect" Value="{{0.508478,0},{0,0},{0.75,0},{0,25}}" />

	keyWindowData.clear();
	unsigned int curWindowId = 0;
	GameApp *app = GameApp::getSingletonPtr();
	KeyWindowData curWindowData;
	for(int i=0;i<GameSettings::_num_keys;i++)
	{
		//always copy the key
		KeyPair curKey = keys[i];
		definedKeys[i] = curKey;
		if(!isInExceptionList(i))
		{
			Ogre::String name = keyNameToString(i);
		
			
			/*UniKey curKey = app->settings.keys[i].main;
			UniKey altKey = app->settings.keys[i].alt;*/
			CEGUI::String window_name = "key_"+TypeConverter::cegui_str(i);

			

			CEGUI::Window *curText = createWindow("StaticText",window_name+"_label");
			keyboardScrollPane->addChildWindow(curText);
			
			//curText->setArea(URect(mostLeftX,curY,labelWidth,height+curY));
			curText->setPosition(CEGUI::UVector2(mostLeftX,curY));
			curText->setSize(CEGUI::UVector2(labelWidth,height));

			curText->setText(TypeConverter::cegui_str(name));
			


			//creating main container
			CEGUI::Window *curCB = createWindow("StaticText",window_name+"_main");
			keyboardScrollPane->addChildWindow(curCB);
			curCB->setPosition(CEGUI::UVector2(mainKeyX,curY));
			curCB->setSize(CEGUI::UVector2(mainKeyWidth,height));
			curCB->setText(keyToString(curKey.main).c_str());
			curCB->setProperty("HorzFormatting","HorzCentred");
			curCB->subscribeEvent(CEGUI::Window::EventMouseClick,CEGUI::Event::Subscriber(&OptionsWindow::keyDisplayClicked, this));
			
			curCB->setID(curWindowId);			
			curWindowData.isMain = true;
			curWindowData.keyIndex = i;
			curWindowData.window = curCB;
			keyWindowData.push_back(curWindowData);
			curWindowId++;
			
			

			curCB = createWindow("StaticText",window_name+"_alt");
			keyboardScrollPane->addChildWindow(curCB);			
			curCB->setPosition(CEGUI::UVector2(altKeyX,curY));
			curCB->setSize(CEGUI::UVector2(mainKeyWidth,height));
			curCB->setText(keyToString(curKey.alt).c_str());
			curCB->setProperty("HorzFormatting","HorzCentred");			
			curCB->subscribeEvent(CEGUI::Window::EventMouseClick,CEGUI::Event::Subscriber(&OptionsWindow::keyDisplayClicked, this));
			
			curCB->setID(curWindowId);			
			curWindowData.isMain = false;
			curWindowData.keyIndex = i;
			curWindowData.window = curCB;
			keyWindowData.push_back(curWindowData);
			curWindowId++;

			//hOffset++;
			curY += incY;
		}
		
	}
	//
}

void OptionsWindow::updateKeyboardTab()
{
	updateKeyboardTabWith(GameApp::getSingletonPtr()->settings.keys);
}

Ogre::String OptionsWindow::keyNameToString(unsigned int kn)
{
	switch(kn)
	{
		case keyNone:			return "None";
		case keyMoveForward:	return "Move Forwards";
		case keyMoveBack:		return "Move Backwards";
		case keyMoveRight:		return "Move Right";
		case keyMoveLeft:		return "Move Left";
		case keyJump:			return "Jump";
		case keyAttack:			return "Attack";
		case keySpellcast:		return "Cast Spell";
		case keyRun:			return "Run";
		case keyAction:			return "Action";
		case keyInventory:		return "Inventory";
		case keyInvTakeOne:		return "Take One";
		case keyInvTakeAll:		return "Take All";
		case keyInvUse:			return "Use";
		case keySpellbook:		return "Open Spellbook";
		case keyScreenshot:		return "Make Screenshot";
		case keyMainmenu:		return "Main Menu";
		case keyLookUp:			return "Look Up";
		case keyLookDown:		return "Look Down";
		case keyLookLeft:		return "Look Left";
		case keyLookRight:		return "Look Right";
		case keyToggleMouselooking: return "Toggle Mouselooking";		
	
		case keyDebug3rdp: return "Debug 3rdperson";
		case keyDebugshowlines: return "Debug Show Lines";
		
		case keyDragItem:		return "Drag Item";
	};	
	return "Key "+ogre_str(kn);
}

void OptionsWindow::updateAudioTab()
{
	GameApp *app = GameApp::getSingletonPtr();

	

	gainAll->setCurrentValue(app->settings.gain_all);
	gainMusic->setCurrentValue(app->settings.gain_music);
	gainSfx->setCurrentValue(app->settings.gain_sfx);
	gainSfxPlayer->setCurrentValue(app->settings.gain_player_sfx);
	gainSpeech->setCurrentValue(app->settings.gain_speech);

	
	sizeComboList(sndDistanceModel);
	sndDistanceModel->setText(app->distanceModelToString(app->settings.distance_model));

	

	sndMaxDistance->setCurrentValue(app->settings.max_distance);
	sndRefDistance->setCurrentValue(app->settings.ref_distance);
	sndRoloffFactor->setCurrentValue(app->settings.rolloff_factor);
}

void OptionsWindow::updateVideoTab()
{
	using namespace CEGUI;	

	GameApp *app = GameApp::getSingletonPtr();
	Ogre::Root *mRoot = Ogre::Root::getSingletonPtr();
	
	Ogre::RenderSystem *cRS = app->getSelectedRenderer();//mRoot->getRenderSystem();	


	rendererSelectBox->setReadOnly(true);
	rendererSelectBox->setText(cRS->getName().c_str());

	rsList = mRoot->getAvailableRenderers();

	
	for(unsigned int i = 0;i<rsList.size();i++)//RenderSystemList::iterator itr = rsList.begin();itr!=rsList.end();itr++)
	{
		Ogre::RenderSystem *rs = rsList[i];//*itr;
		Ogre::String name = rs->getName();
		
		CEGUI::ListboxTextItem *renderItem = new ListboxTextItem(name.c_str());
		renderItem->setSelectionBrushImage("LandsLook","TextSelectionBrush");
		renderItem->setID(i);
		rendererSelectBox->addItem(renderItem);
		
		RendererOptionsWindow* curROW = new RendererOptionsWindow(rs);
		curROW->addToWindow(videoOptionContainer);
		if(rs == cRS)
		{
			curROW->show();
		}
		else
		{
			curROW->hide();
		}
		rowList.push_back(curROW);
		//i++;
	}
	sizeComboList(rendererSelectBox);
}



OptionsWindow::~OptionsWindow()
{
	//clear everything i constructed
	for(RendererOptionsList::iterator itr = rowList.begin();itr!=rowList.end();itr++)
	{
		delete (*itr);
	}
	destroyLayout();
}

void OptionsWindow::close()
{
	GameApp::getSingletonPtr()->setMenu(GameApp::GUI_MAINMENU);
}

void OptionsWindow::apply()
{
	GameApp *app = GameApp::getSingletonPtr();
	//make the selected renderer "selected"
	Ogre::RenderSystem *selectedRenderer = Ogre::Root::getSingletonPtr()->getRenderSystemByName(rendererSelectBox->getText().c_str()); //>getSelectedItem()->getID();
	//apply renderer settings
	GameApp::getSingletonPtr()->setSelectedRenderer(selectedRenderer);
	for(RendererOptionsList::iterator itr = rowList.begin();itr!=rowList.end();itr++)
	{
		(*itr)->applyOptions();
	}
	
	//audio settings...
	app->settings.gain_all		= gainAll->getCurrentValue();
	app->settings.gain_music	= gainMusic->getCurrentValue();
	app->settings.gain_sfx		= gainSfx->getCurrentValue();
	app->settings.gain_player_sfx=gainSfxPlayer->getCurrentValue();
	app->settings.gain_speech	= gainSpeech->getCurrentValue();	
	
	app->settings.distance_model = app->parseDistanceModel(sndDistanceModel->getText().c_str());
	

	app->settings.max_distance = sndMaxDistance->getCurrentValue();
	app->settings.ref_distance = sndRefDistance->getCurrentValue();
	app->settings.rolloff_factor=sndRoloffFactor->getCurrentValue();


	//"game" settings
	app->settings.sensibility_x_mouse = sensibilityX->getCurrentValue();
	app->settings.sensibility_x_mouse = sensibilityY->getCurrentValue();

	app->settings.sensibility_x_keyboard = sensibilityX_keyboard->getCurrentValue();
	app->settings.sensibility_y_keyboard = sensibilityY_keyboard->getCurrentValue();

	app->settings.physics_fps = physicsPrecision->getCurrentValue();
	
	app->settings.mouselooking = mouseLooking->isSelected();



	//now apply the keys
	for(unsigned int i=0;i<GameSettings::_num_keys;i++)
	{
		//if(!isInExceptionList(i))
		app->settings.keys[i] = definedKeys[i];
	}
}

bool OptionsWindow::cancelClick(const CEGUI::EventArgs& e)
{
	close();
	return true;
}

bool OptionsWindow::okClick(const CEGUI::EventArgs& e)
{
	apply();
	close();
	return true;
}

bool OptionsWindow::acceptClick(const CEGUI::EventArgs& e)
{
	apply();
	return true;
}

bool OptionsWindow::rendererChanged(const CEGUI::EventArgs& e)
{

	const CEGUI::WindowEventArgs& we = 
		static_cast<const CEGUI::WindowEventArgs&>(e);

	CEGUI::Combobox *box = static_cast<CEGUI::Combobox*>(we.window);
	
	CEGUI::uint index = box->getSelectedItem()->getID();

	//now disable all 
	for(RendererOptionsList::iterator itr = rowList.begin();itr!=rowList.end();itr++)
	{
		(*itr)->hide();
	}
	//now enable the one needed
	rowList[index]->show();

	return true;
}

bool OptionsWindow::keyDisplayClicked(const CEGUI::EventArgs& e)
{
	const CEGUI::WindowEventArgs& we = 
		static_cast<const CEGUI::WindowEventArgs&>(e);
	editingKeyDisplay = we.window;
	we.window->setText("press key...");
	//editingKeyIndex = editingKeyDisplay->getID();
	isEditingKey = true;
	//window has the "keyname" as ID
	GameApp::getSingletonPtr()->mFrameListener->enableKeyDetectionMode();

	//show info
	keyboardInfoText->setText("Press Escape to cancel editing, Backspace to clear the key");

	return true;
}

bool OptionsWindow::findKey(UniKey key, int &resultName, bool &isMain)
{
	resultName = 0;
	isMain = true;
	bool result = false;
	if(key.isEmpty())
		return false;
	for(int i=1;i<GameSettings::_num_keys;i++)
	{
		if(definedKeys[i].main == key)
		{
			definedKeys[i].main = UniKey();
			resultName = i;
			isMain = true;
			setKey(resultName,isMain,UniKey());
			result = true;
		}
		if(definedKeys[i].alt == key)
		{
			definedKeys[i].alt = UniKey();
			resultName = i;
			isMain = false;
			setKey(resultName,isMain,UniKey());
			result = true;
		}
	}
	return result;//this way it will unset every key which has the same definition. it will only return the latest, though
}

void OptionsWindow::setKey(int keyName, bool isMain, UniKey newKey)
{
	if(isMain)
	{
		definedKeys[keyName].main = newKey;
	}
	else
	{
		definedKeys[keyName].alt = newKey;
	}
	
	//do we have the window?
	for(std::vector<KeyWindowData>::iterator itr = keyWindowData.begin();itr!=keyWindowData.end();itr++)
	{
		KeyWindowData cur = *itr;
		if(cur.keyIndex == keyName && isMain == cur.isMain && cur.window)
		{
			cur.window->setText(keyToString(newKey).c_str());
			break;
		}
	}
}

void OptionsWindow::setCurrentKey(UniKey key)
{
	if(!isEditingKey)
		return;	
	keyboardInfoText->setText("");//clear, if nothing else
	isEditingKey = false;
	//check if escape has been pressed. in this case, editing is cancelled
	if(key == OIS::KC_ESCAPE)
	{		
		//escape means cancel editing key
		KeyWindowData data = keyWindowData[editingKeyDisplay->getID()];
		if(data.isMain)
		{
			//definedKeys[data.keyIndex].main = key;
			editingKeyDisplay->setText(keyToString(definedKeys[data.keyIndex].main).c_str());
		}
		else
		{
			editingKeyDisplay->setText(keyToString(definedKeys[data.keyIndex].alt).c_str());
			//definedKeys[data.keyIndex].alt = key;
		}
		return;
	}
	if(key == OIS::KC_BACK)
	{
		//backspace means un-assign
		key = UniKey();
		//executing the next part as is
	}

	
	
	editingKeyDisplay->setText(keyToString(key).c_str());
	KeyWindowData data = keyWindowData[editingKeyDisplay->getID()];

	int prevKeyName;
	bool prevIsMain;
	if(findKey(key,prevKeyName,prevIsMain))
	{
		Ogre::String infoText = keyToString(key)+" was used for "+keyNameToString(prevKeyName);
		if(!prevIsMain)
			infoText += " (alternative)";
		keyboardInfoText->setText(cegui_str(infoText));
	}

	if(data.isMain)
	{
		definedKeys[data.keyIndex].main = key;
	}
	else
	{
		definedKeys[data.keyIndex].alt = key;
	}
	

}

bool OptionsWindow::keysRestoreClick(const CEGUI::EventArgs& e)
{
	restoreDefaultKeys();
	return true;
}

void OptionsWindow::restoreDefaultKeys()
{
	//get new keys
	GameSettings defaultSettings;
	updateKeyboardTabWith(defaultSettings.keys);
	
	
}