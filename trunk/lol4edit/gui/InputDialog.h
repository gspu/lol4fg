/********************************************************************************
** Form generated from reading ui file 'inputdialogxC4424.ui'
**
** Created: Mon 7. Dec 22:25:30 2009
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef INPUTDIALOGXC4424_H
#define INPUTDIALOGXC4424_H

#include <QtGui/QDialog>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include "Ogre.h"

namespace Ui
{
	class InputStringDialog;
}

class InputDialog: public QDialog
{
	//Q_OBJECT
public:
	Ui::InputStringDialog *ui;

	InputDialog(QWidget *parent = 0, Qt::WindowFlags f = 0);

	int exec(QString title, QString text);

	Ogre::String getInputValue();
//protected slots:
//	void acceptInput();//nicht accept, damit es sich nicht mit dem slot überschneidet
protected:
	

	
	
	//Ogre::String returnValue;

	//signals:
	//void returnValue(Ogre::String str);

};





#endif // INPUTDIALOGXC4424_H
