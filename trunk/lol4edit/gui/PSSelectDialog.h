#ifndef __praPSSelectDialig
#define __praPSSelectDialig

#include "GenericSelectDialog.h"

//this is supposed to be used like "dialog->exec(lineEdit)"


class PSSelectDialog: public GenericSelectDialog
{
public:
	PSSelectDialog(QWidget *parent = 0, Qt::WindowFlags f = 0):
		GenericSelectDialog(parent,f)
	{
		fillList();
	}

	int exec(QLineEdit *target);

	void fillList();

	virtual void acceptSelection(QListWidgetItem *item);
	//virtual void okButtonClick();//es schaut was ausgew�hlt ist und ruft dann die funktion oben auf
	/*inline QString getResult()
	{
		return result;
	}*/

private:
	QLineEdit *target;
	//QString result;

};

#endif