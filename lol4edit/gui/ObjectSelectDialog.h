/*******************************************************************************
** Form generated from reading ui file 'objectselectHp5120.ui'
**
** Created: Mon 26. Oct 23:24:29 2009
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef OBJECTSELECTHP5120_H
#define OBJECTSELECTHP5120_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QListWidget>
//#include <QtGui/QListViewItem>
#include <QtGui/QTabWidget>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <QtGui/QMdiSubWindow>


namespace Ui {
    class ObjectSelectDialog;//fwdec
} 
class gamedata;


//little helper class 
//this is a shortcut to some stuff.
//ObjectTreeItem should only be child of QTreeWidgetItem, and, since 
//their main purpose is to display a text and to hold gamedata, the constructor provides this
//also, only column 0 is used
class ObjectTreeItem: public QTreeWidgetItem
{
public:
	ObjectTreeItem(QTreeWidgetItem *parent, QString text, gamedata *data,int type = 0):
		QTreeWidgetItem(parent)
	{
		setText(0,text);
		itemType = type;
		this->data = data;
	}
	gamedata *getGamedata()
	{
		return data;
	}
	void setGamedata(gamedata *set)
	{
		data = set;
	}
	int getItemType()
	{
		return itemType;
	}
private:
	gamedata *data;
	int itemType;//no need for gamedata_extra, i just set it here...
};



class ObjectSelectDialog: public QDialog
{
	Q_OBJECT
public:
	//default constructor
	ObjectSelectDialog(QWidget *parent = 0, Qt::WindowFlags f = 0);

	//types for the entries in the "else" and also for other stuff...
	enum ItemType
	{
		eitDefault			= 0,//for regular stuff which contains gamedata
		eitEntrance			= 1,
		eitLight			= 2,
		eitPlane			= 3,
		eitBillboard		= 4,
		//eitParticleSystem	= 1005,
		eitWaterPlane		= 5,
		eitStaticMesh		= 6,//means, the text of the entry should be considered the mesh name
		eitStaticDotScene	= 7,//same with .scene
		eitStaticPS			= 8 //same with particle systems

	};

	Ui::ObjectSelectDialog *ui;



	void fillLists();

	//QMdiArea *parent;//!!
	//QMdiSubWindow *asSubWindow;//
public slots:

	void selectEntry(QTreeWidgetItem *item, int column);
	void addButtonClicked();
	
protected:
	void notifyObjectNotPlaceable(gamedata *data);

	

};


 


#endif // OBJECTSELECTHP5120_H
