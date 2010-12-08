#ifndef __mainmenu
#define __mainmenu

#include "GameWindow.h"
//#include "gamedata.h"

class MainMenu: public GameWindow
{
public:
	MainMenu();
	~MainMenu();

	//to enable/disable the resume button
	void setResumeEnabled(bool set);
private:
	CEGUI::Window *resumeBtn;
	CEGUI::Window *startBtn;
	CEGUI::Window *saveLoadBtn;
	CEGUI::Window *optionsBtn;
	CEGUI::Window *quitBtn;
protected:
	bool quitClick(const CEGUI::EventArgs& e);
	bool startGameClick(const CEGUI::EventArgs& e);
	bool resumeClick(const CEGUI::EventArgs& e);
	bool optionsClick(const CEGUI::EventArgs& e);
	bool saveLoadClick(const CEGUI::EventArgs& e);
};

#endif
