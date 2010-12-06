#ifndef __saveloadmenu
#define __saveloadmenu

#include "GameWindow.h"

class SaveLoadMenu: public GameWindow
{
public:
	SaveLoadMenu();
	~SaveLoadMenu();

	//der teil ist für spielstände
	struct NameTime
	{
		Ogre::String name;
		time_t time;
	};
	typedef std::vector<NameTime> NameTimeVector;

	void update();
private:
	CEGUI::Listbox *saveGameList;
	CEGUI::Editbox *nameBox;
	CEGUI::PushButton *saveBtn;
	CEGUI::PushButton *loadBtn;
	CEGUI::PushButton *deleteBtn;
	CEGUI::PushButton *cancelBtn;

	void sortNameTimeVector(NameTimeVector &vec);
protected:
	bool loadClick(const CEGUI::EventArgs& e);
	bool saveClick(const CEGUI::EventArgs& e);
	bool deleteClick(const CEGUI::EventArgs& e);
	bool cancelClick(const CEGUI::EventArgs& e);

	bool listClick(const CEGUI::EventArgs& e);
	bool listDblClick(const CEGUI::EventArgs& e);	
};

#endif
