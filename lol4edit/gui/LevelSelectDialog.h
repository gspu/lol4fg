#ifndef __praLevelSelectDialig
#define __praLevelSelectDialig

#include "GenericSelectDialog.h"



class LevelSelectDialog: public GenericSelectDialog
{
public:
	LevelSelectDialog(QWidget *parent = 0, Qt::WindowFlags f = 0):
		GenericSelectDialog(parent,f)
	{
		fillList();
	}

	virtual void fillList();

	virtual void acceptSelection(QListWidgetItem *item);//der bleibt aber void
	//virtual void okButtonClick();//es schaut was ausgewählt ist und ruft dann die funktion oben auf

};

#endif