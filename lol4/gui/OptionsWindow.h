#ifndef __OptionsWindow
#define __OptionsWindow

#include <GameWindow.h>
#include "CEGUI.h"
#include "FwDec.h"
#include "GameSettings.h"

//class for the part(s) of the options window where
//the options for a renderer are listed. 
//creates it's own CEGUI::Window, for switching purposes
class RendererOptionsWindow: public GameWindow
{
public:
	typedef std::map<Ogre::String,CEGUI::Window*> RenderOptionMap;

	//constructor. takes a RenderSystem and retrieves the options itself
	RendererOptionsWindow(Ogre::RenderSystem* renderSystem);

	~RendererOptionsWindow();

	//reads the options as set in the gui and applys them to the render system
	void applyOptions();
private:
	//Ogre::ConfigOptionMap mOptions;
	Ogre::RenderSystem *mRenderSystem;
	//map for containing Option Name -> CEGUI::Window selector relation. does not contain labels
	RenderOptionMap mOptions;

	//creates a combobox or similar from an option and adds it
	void addOptionSelector(Ogre::ConfigOption option, CEGUI::UDim yPos, bool useCheckboxes = true);
protected:
};

//class for the "options"-window
class OptionsWindow: public GameWindow
{
public:
	typedef std::vector<RendererOptionsWindow*> RendererOptionsList;
	struct KeyWindowData
	{
		//key for the definedKeys array, also keyName
		unsigned int keyIndex;
		//whenever this is the main or the alt key
		bool isMain;
		//also, pointer to the corresponding window
		CEGUI::Window *window;
	};

	OptionsWindow();
	~OptionsWindow();

	//sets the fields to the current settings
	void updateShownSettings();

	void close();
	void apply();

	//sets the currently editing key
	void setCurrentKey(UniKey key);
private:
	CEGUI::TabControl *tabCtrl;

	CEGUI::PushButton *okButton;
	CEGUI::PushButton *cancelButton;
	CEGUI::PushButton *applyButton;

	

	CEGUI::Combobox *rendererSelectBox;

	//CEGUI::ScrollablePane *videoScrollPane;
	CEGUI::Window *videoOptionContainer;
	CEGUI::ScrollablePane *keyboardScrollPane;

	//elements: audio
	CEGUI::Slider *gainAll;
	CEGUI::Slider *gainMusic;
	CEGUI::Slider *gainSfx;
	CEGUI::Slider *gainSfxPlayer;
	CEGUI::Slider *gainSpeech;

	CEGUI::Combobox *sndDistanceModel;
	CEGUI::Spinner *sndMaxDistance;
	CEGUI::Spinner *sndRefDistance;
	CEGUI::Spinner *sndRoloffFactor;

	//elements: game
	CEGUI::Slider *sensibilityX;
	CEGUI::Slider *sensibilityY;
	CEGUI::Slider *sensibilityX_keyboard;
	CEGUI::Slider *sensibilityY_keyboard;
	CEGUI::Spinner *physicsPrecision;
	//CEGUI::Window *physicsPrecisionDisplay;
	CEGUI::Checkbox *mouseLooking;

	//elements: keys
	CEGUI::PushButton *restoreDefaultKeysBtn;
	CEGUI::Window *keyboardInfoText;//keyboardInfoText

	void updateVideoTab();
	void updateKeyboardTab();
	void updateAudioTab();
	void updateGameTab();

	//updates the keyboard tab with a given key list
	void updateKeyboardTabWith(KeyPair keys[GameSettings::_num_keys]);

	void restoreDefaultKeys();

	bool isInExceptionList(int keyname);
	

	//void setupScrollbar(CEGUI::Scrollbar *bar);

	Ogre::String keyNameToString(unsigned int kn);

	//converts a key to something human-readable
	Ogre::String keyToString(UniKey key);

	//checks if a key is alerady defined. if found, it also un-sets the previous definition
	//param key: the key to search
	//param resultName: to return the KeyName which has been found or keyNone if none
	//param isMain: if true, this is the main key, if false, the alternative
	//returns: true if found something
	bool findKey(UniKey key, int &resultName, bool &isMain);

	//updates a key display window
	//void updateKeyDisplay(UniKey key, int keyName, bool isMain);

	//sets a specified key and updates the display window
	void setKey(int keyName, bool isMain, UniKey newKey);

	//list of renderers. indices are the same as with RendererOptionList
	Ogre::RenderSystemList rsList;
	RendererOptionsList rowList;

	//list of keys as defined here
	KeyPair definedKeys[GameSettings::_num_keys];
	//if we are editing a key right now
	bool isEditingKey;
	//unsigned int editingKeyIndex;
	//the window of the currently editing key
	CEGUI::Window *editingKeyDisplay;
	//this is a vector with an id -> (keyId, isMain) relation
	//the windows for the key display hold an ID which is the key for this array
	std::vector<KeyWindowData> keyWindowData;

	static const int _num_exceptions = 4;
	//array with the keyNames of keys which should not be defined
	int keyExceptionList[_num_exceptions];//initialized in the constructor


protected:
	bool cancelClick(const CEGUI::EventArgs& e);
	bool okClick(const CEGUI::EventArgs& e);
	bool acceptClick(const CEGUI::EventArgs& e);

	bool rendererChanged(const CEGUI::EventArgs& e);

	bool keyDisplayClicked(const CEGUI::EventArgs& e);

	bool keysRestoreClick(const CEGUI::EventArgs& e);

};

#endif