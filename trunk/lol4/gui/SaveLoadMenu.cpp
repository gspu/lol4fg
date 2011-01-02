#include "SaveLoadMenu.h"
#include <GameApp.h>
#include <StandardApplication.h>
#include <functions.h>

bool NameTimeCompareFunction(const SaveLoadMenu::NameTime& first, const SaveLoadMenu::NameTime& second)
{
	//returns true if the first argument goes before the second argument
	//largest dates go first, therefore
	return first.time > second.time;
}


SaveLoadMenu::SaveLoadMenu()
{
	loadLayout("SaveLoadMenu.layout");

	saveGameList = static_cast<CEGUI::Listbox*>(getWindow("saveloadwnd/savegamelist"));
	nameBox = static_cast<CEGUI::Editbox*>(getWindow("saveloadwnd/namebox"));
	saveBtn = static_cast<CEGUI::PushButton*>(getWindow("saveloadwnd/save"));
	loadBtn = static_cast<CEGUI::PushButton*>(getWindow("saveloadwnd/load"));
	deleteBtn = static_cast<CEGUI::PushButton*>(getWindow("saveloadwnd/delete"));
	cancelBtn = static_cast<CEGUI::PushButton*>(getWindow("saveloadwnd/cancel"));

	
	saveBtn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&SaveLoadMenu::saveClick, this));
	loadBtn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&SaveLoadMenu::loadClick, this));
	deleteBtn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&SaveLoadMenu::deleteClick, this));
	cancelBtn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&SaveLoadMenu::cancelClick, this));
	saveGameList->subscribeEvent(CEGUI::Window::EventMouseClick, CEGUI::Event::Subscriber(&SaveLoadMenu::listClick, this));
	saveGameList->subscribeEvent(CEGUI::Window::EventMouseDoubleClick, CEGUI::Event::Subscriber(&SaveLoadMenu::listDblClick, this));
	
}

SaveLoadMenu::~SaveLoadMenu()
{
	destroyLayout();
}

bool SaveLoadMenu::loadClick(const CEGUI::EventArgs& e)
{
	//CEGUI::Editbox *editbox = static_cast<CEGUI::Editbox*>(wmgr->getWindow("saveloadwnd/namebox"));
	Ogre::String name = nameBox->getText().c_str();
	if(name == "")
		return true;
	
	GameApp::getSingletonPtr()->loadGame(name);
	return true;
}

bool SaveLoadMenu::saveClick(const CEGUI::EventArgs& e)
{
	GameApp *app = GameApp::getSingletonPtr();
	if(!app->getCurrentLevel())
		return true;
	
	
	Ogre::String name = nameBox->getText().c_str();
	if(name == "")
		return true;
	app->saveGame(name);
    
	update();
	return true;
}

bool SaveLoadMenu::deleteClick(const CEGUI::EventArgs& e)
{
	GameApp *app = GameApp::getSingletonPtr();
	CEGUI::ListboxItem* cur = saveGameList->getFirstSelectedItem();
	if(!cur)
		return true;
	ZipSaveFile toDel(cur->getText().c_str(),app->saveGamePath,SAVEGAME_EXTENSION);
	
	toDel.eraseArchive();
	update();
	return true;
}

bool SaveLoadMenu::cancelClick(const CEGUI::EventArgs& e)
{
	GameApp::getSingletonPtr()->setMenu(GameApp::GUI_MAINMENU);
	return true;
}

bool SaveLoadMenu::listClick(const CEGUI::EventArgs& e)
{	
	CEGUI::ListboxItem *cur = saveGameList->getFirstSelectedItem();
	if(!cur)
		return true;
	nameBox->setText(cur->getText());
	return true;
}

bool SaveLoadMenu::listDblClick(const CEGUI::EventArgs& e)
{
	return true;//wtf? was wollte ich hier?!
}

void SaveLoadMenu::update()
{
	GameApp *app = GameApp::getSingletonPtr();
	Ogre::ResourceGroupManager *resMgr = Ogre::ResourceGroupManager::getSingletonPtr();
	//loading list of saves
	StringVectorPtr savevector = resMgr->findResourceNames("Savegame",Ogre::String("*.")+SAVEGAME_EXTENSION);
	//liste sortieren
	NameTimeVector timevector;
	for (Ogre::StringVector::iterator i = savevector->begin(); i != savevector->end(); ++i)
	{
		NameTime cur;
		cur.name = *i;
		cur.time = resMgr->resourceModifiedTime("Savegame",cur.name);//getFileDate( app->saveGamePath+"/"+cur.name );
		timevector.push_back(cur);
	}
	//sortNameTimeVector(timevector);
	std::sort(timevector.begin(),timevector.end(),NameTimeCompareFunction);

	size_t cnt = saveGameList->getItemCount();	
	for(size_t i = 0;i < cnt;i++)
	{
		CEGUI::ListboxItem *listboxitem = saveGameList->getListboxItemFromIndex(0);
		//setDebugText(String("\nremoved:")+listboxitem->getText().c_str(),true);
		saveGameList->removeItem(listboxitem);
	}
	//saveList->subscribeEvent(CEGUI::Window::EventMouseButtonDown,CEGUI::Event::Subscriber(&EditorApp::meshListDown,this));
	for (NameTimeVector::iterator i = timevector.begin(); i != timevector.end(); ++i)
	{
		//Ogre::String wtf = *i;
		//getFileDate( saveGamePath+"/"+wtf );
		Ogre::String fname, fext;
		Ogre::StringUtil::splitBaseFilename((*i).name,fname,fext);
		CEGUI::ListboxTextItem *listboxitem = new CEGUI::ListboxTextItem(fname.c_str());
		listboxitem->setSelectionBrushImage(STYLE_GAME,"TextSelectionBrush");
		//listboxitem->setTextColours(clBlack);
		saveGameList->addItem(listboxitem);
	}
	
}


void SaveLoadMenu::sortNameTimeVector(NameTimeVector &vec)
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