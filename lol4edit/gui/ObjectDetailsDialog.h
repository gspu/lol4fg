/********************************************************************************
** Form generated from reading ui file 'detailsXd5160.ui'
**
** Created: Tue 17. Nov 00:00:09 2009
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef DETAILSXD5160_H
#define DETAILSXD5160_H

//#include <QtCore/QVariant>
//#include <QtGui/QAction>
//#include <QtGui/QApplication>
//#include <QtGui/QButtonGroup>
//#include <QtGui/QCheckBox>
//#include <QtGui/QComboBox>
//#include <QtGui/QDialog>
//#include <QtGui/QFormLayout>
//#include <QtGui/QGridLayout>
//#include <QtGui/QGroupBox>
//#include <QtGui/QHBoxLayout>
//#include <QtGui/QHeaderView>
//#include <QtGui/QLabel>
//#include <QtGui/QLineEdit>
//#include <QtGui/QPushButton>
//#include <QtGui/QSpacerItem>
//#include <QtGui/QStackedWidget>
//#include <QtGui/QToolButton>
//#include <QtGui/QVBoxLayout>
//#include <QtGui/QWidget>
#include <QtGui/QTreeWidget>
#include "h/details.h"
#include <QtGui/QMdiSubWindow>
#include "FwDec.h"

//forward dec stuff
namespace Ui{
	class CharacterData;
	class DoorData;
	class LightData;
	class WaterData;
	class StaticData;
	class EditInventory;
}


class ObjectDetailsDialog: public QDialog
{
	Q_OBJECT
public:
	Ui::DetailsDialog *ui;

	

	ObjectDetailsDialog(GameObject *obj,QWidget *parent = 0, Qt::WindowFlags f = 0);



public slots:
	void accept()
	{
		apply();
		close();
	}
	void apply();

	void close()
	{
		this->parentWidget()->close();
	}

	void showMatSelect();
	void showPSSelect();
	//void showMeshSelect();
	void showSoundSelect();
	
	
protected:
	GameObject *mObj;

	//widgets containing additional "data" for the different item types	
	Ui::CharacterData	*chData;
	Ui::DoorData		*drData;
	Ui::LightData		*liData;
	Ui::WaterData		*waData;
	Ui::StaticData		*stData;
	Ui::EditInventory	*ctData;


	void initObjectData();
	void connectSlots();

	

	//erzeugt guielemente für tür
	void initDoorGui();

	void initCharGui();

	void initLightGui();

	void initWaterGui();

	void initStaticGui();

	void initContainerGui();

	//initializes inventory edit gui to the target widget
	//sets ctData, this is also set when we edit container ATM
	void initInventoryEditor(QWidget *widget);

	void fillInventoryList(Inventory *inv);

	void updateInventory(Inventory *inv);

	void fillSpellbookList(Spellbook *sb);

	void updateSpellbook(Spellbook *sb);

};



#endif // DETAILSXD5160_H
