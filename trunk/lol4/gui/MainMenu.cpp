#include "MainMenu.h"
#include <CEGUI.h>
#include <GameApp.h>

MainMenu::MainMenu()
{
	loadLayout("MainMenu.layout");

	resumeBtn = getWindow("resume");
	startBtn = getWindow("startgame");
	saveLoadBtn = getWindow("saveload");
	optionsBtn = getWindow("options");
	quitBtn = getWindow("quit");

	resumeBtn->disable();

	//connecting
	resumeBtn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenu::resumeClick, this));
	startBtn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenu::startGameClick, this));
	saveLoadBtn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenu::saveLoadClick, this));
	optionsBtn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenu::optionsClick, this));
	quitBtn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenu::quitClick, this));
		
}
MainMenu::~MainMenu()
{
	destroyLayout();
}

void MainMenu::setResumeEnabled(bool set)
{
	resumeBtn->setEnabled(set);
}

bool MainMenu::quitClick(const CEGUI::EventArgs& e)
{
	GameApp::getSingletonPtr()->quitGame();
	return true;
}
bool MainMenu::startGameClick(const CEGUI::EventArgs& e)
{
	resumeBtn->enable();
	GameApp::getSingletonPtr()->startGame();
	return true;
}
bool MainMenu::resumeClick(const CEGUI::EventArgs& e)
{
	GameApp::getSingletonPtr()->resumeGame();
	return true;
}
bool MainMenu::optionsClick(const CEGUI::EventArgs& e)
{
	GameApp::getSingletonPtr()->showOptions();
	return true;
}
bool MainMenu::saveLoadClick(const CEGUI::EventArgs& e)
{
	GameApp::getSingletonPtr()->showSaveLoad();
	return true;
}