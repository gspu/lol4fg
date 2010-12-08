/********************************************************************************
** Form generated from reading ui file 'selectdialogHp4372.ui'
**
** Created: Mon 2. Nov 21:00:16 2009
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef SELECTDIALOGHP4372_H
#define SELECTDIALOGHP4372_H

#include <QtGui/QDialog>
#include <QtGui/QListWidget>

namespace Ui
{
	class SelectDialog;
}

class GenericSelectDialog: public QDialog
{
	Q_OBJECT
public:
	/*QVBoxLayout *verticalLayout;
	QListWidget *listWidget;
	QHBoxLayout *horizontalLayout;
	QSpacerItem *horizontalSpacer;
	QPushButton *cancelButton;
	QPushButton *okayButton;

	QDialog *dialog;*/

	GenericSelectDialog(QWidget *parent = 0, Qt::WindowFlags f = 0);

	virtual ~GenericSelectDialog(){}

	Ui::SelectDialog *ui;
	/*void setupUi();

	void retranslateUi();*/

	//funktion um die liste zu füllen. wird automatisch im setupUi aufgerufen. muss extra implementiert werden
	virtual void fillList(){};
	//funktion um die signale zu verbinden (also ok und cancel usw). wird automatisch im setupUi aufgerufen
	//macht per default, dass cancel das fenster schließt
	virtual void connectSignals();
public slots:
	//doch, ich muss das so machen. sonst muss jede abgeleitete auch ein qobj sein
	virtual void acceptSelection(QListWidgetItem *item) = 0;//{};//der bleibt aber void
	//virtual void okButtonClick();//{};//es schaut was ausgewählt ist und ruft dann die funktion oben auf

	virtual void accept();
	virtual void reject()
	{
		QDialog::reject();
		//close();
	}
	//void close()
	//{
	//	QDialog::close();
	//	//this->parentWidget()->close();
	//}



	//virtual void itemDoubleClicked(){};
};



#endif // SELECTDIALOGHP4372_H
