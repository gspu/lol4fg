/********************************************************************************
** Form generated from reading ui file 'editorp52604.ui'
**
** Created: Mon 15. Jun 02:11:16 2009
**      by: Qt User Interface Compiler version 4.5.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef _qtMainWindow
#define _qtMainWindow

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QActionGroup>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMdiArea>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>
#include "FwDec.h"
#include "Ogre.h"



//wah
namespace QtOgre{
    class QtEditorApp;

}

namespace Ui
{
	class MainWindow;		
	
}

class ObjectSelectDialog;
class LevelSelectDialog;
class InputDialog;
class ConfirmDialog;
class MaterialSelectDialog;
class TerrainEditDialog;
class PageStatusWindow;
class LevelObjectList;



class EditorMainWindow: public QMainWindow
{
	Q_OBJECT
public:
	//move, rotate, scale
	QActionGroup *manipulationMode;

	//edit terrain or objects
	QActionGroup *editingMode;

    QMdiArea *mdiArea;
	QtOgre::QtEditorApp *qtapp;
	EditorApp *app;
	Ui::MainWindow *ui;


    EditorMainWindow(QtOgre::QtEditorApp *_app, QWidget *parent = 0, Qt::WindowFlags flags = 0);

	//this adds several stuff to the ui, like the mdiArea
    void setupUi();
	void setupActions();

	
    /*void retranslateUi();*/

    void setOgreWidget(QWidget* widget);
    void setLogWidget(QWidget* widget);
    void resyncSpecialWidgets();

	

    QMdiSubWindow *ogreSubWindow;
    QMdiSubWindow *logSubWindow;
	//mhm... ich gruppier die erstmal so
	QMdiSubWindow *objectSelectSubWindow;
	ObjectSelectDialog *objectSelectDialog;

	QMdiSubWindow *terrainEditSubWindow;
	TerrainEditDialog *terrainEditDialog;

	PageStatusWindow *pageStatusWindow;
	QMdiSubWindow *pageStatusSubWindow;

	QMdiSubWindow *objectListSubWindow;
	LevelObjectList *objectListDialog;

	LevelSelectDialog *openLevelDialog;

	//erstellt und initialisiert die anderen fenster
	void initGuiElements();
	//aktualisiert alles mögliche, nachdem gamedata geladen wurde
	void updateAfterLoading();

	void showObjectDetails(GameObject *obj);

	void setStatusText(Ogre::String text);


	/*
	Aligns the 3 windows like this:
	 _ ___
	|1|   |
	|-| 3 |
	|2|   |
	 ¯ ¯¯¯
	 probably it would be better if the app would just save the sizes..
	*/
	void alignMdiWindows(QMdiSubWindow *leftTop,QMdiSubWindow *leftBottom,QMdiSubWindow *rightBig);

	//QString EditorMainWindow::getObjListName(GameObject *cur);
	
/*	Ui::InputDialog* getInputDialog()
	{
		return inputDialog;
	}*/

	//shortcut zum inputdialog. fordert den User zur Texteingabe auf.
	//gibt bei accept true zurück und schreibt den String ins result
	//gibt bei reject false zurück und schreibt einen Leerstring ins result
	bool prompt(Ogre::String &result,Ogre::String title,Ogre::String text);

	//shortcut zum confirmdialog. Fordert den User auf, etwas zu bestätigen
	//gibt bei accept true zurück, bei reject false
	bool confirm(Ogre::String title,Ogre::String text);

	//hides oldWnd and displays newWnd in it's place
	void replaceSubWindow(QMdiSubWindow *oldWnd, QMdiSubWindow* newWnd);
	
public slots:
	void performDebugTest(bool dummy);
	//ACTION SLOTS
	//neu
	void newLevel(bool checked = false);
	//level öffnen
	void showOpenLevel(bool checked = false);
	//speichern
	void saveLevel(bool checked = false);
	//speichern unter
	void showSaveAs(bool checked = false);
	//gridsnap
	void setGridSnap(bool checked = false);
	//angle snap
	void setAngleSnap(bool checked = false);
	//scale snap
	void setScaleSnap(bool checked = false);
	//move mode
	void setMoveMode(bool checked = false);
	//rotate mode
	void setRotateMode(bool checked = false);
	//scale mode
	void setScaleMode(bool checked = false);
	//physics
	void setPhysicsEnabled(bool checked = false);

	//terrain editing
	void setTerrainEditing(bool checked = false);
	//objects editing
	void setObjectsEditing(bool checked = false);
	
	//CALLBACK-ARTIGE SLOTS
	//slot für das confirm bei "neues lvl". irgendwann
	//slot für den "speichern unter" inputdialog
	void saveAs(Ogre::String name,bool confirmOverwrite = false);
private:
	
	
	void showTerrainEditingWindows(bool show);
	void showObjectEditingWindows(bool show);
	//void createDialogs();
	
/*      void closeEvent(QCloseEvent *evt);*/
    //wichtige mdi-fenster
    

};


#endif // EDITORP52604_H

//
//
//#include <QtCore/QVariant>
//#include <QtGui/QAction>
//#include <QtGui/QApplication>
//#include <QtGui/QButtonGroup>
//#include <QtGui/QHBoxLayout>
//#include <QtGui/QHeaderView>
//#include <QtGui/QMainWindow>
//#include <QtGui/QMdiArea>
//#include <QtGui/QMenu>
//#include <QtGui/QMenuBar>
//#include <QtGui/QStatusBar>
//#include <QtGui/QToolBar>
//#include <QtGui/QWidget>
//
//QT_BEGIN_NAMESPACE
//
//class Ui_MainWindow
//{
//public:
//    QAction *actionNew_Level;
//    QAction *actionOpen;
//    QAction *actionSave;
//    QAction *actionSave_As;
//    QAction *actionExit;
//    QAction *actionEdit_Terrain;
//    QAction *actionEdit_Objects;
//    QAction *actionSave_grouped_Meshes;
//    QAction *actionSave_grouped_Objects;
//    QAction *actionSave_Collision;
//    QAction *actionEditor_Settings;
//    QAction *actionLevel_Settings;
//    QAction *actionSnap_on_Grid;
//    QAction *actionSnap_on_Angle;
//    QAction *actionSnap_on_Scale;
//    QAction *actionPhysics;
//    QAction *actionFog;
//    QAction *actionMove_Mode;
//    QAction *actionRotate_Mode;
//    QAction *actionScale_Mode;
//    QWidget *centralwidget;
//    QHBoxLayout *horizontalLayout;
//    QMdiArea *mdiArea;
//    QMenuBar *menubar;
//    QMenu *menuFile;
//    QMenu *menuEdit;
//    QMenu *menuOptions;
//    QStatusBar *statusbar;
//    QToolBar *toolBar;
//
//    void setupUi(QMainWindow *this)
//    {
//        if (this->objectName().isEmpty())
//            this->setObjectName(QString::fromUtf8("this"));
//        this->resize(644, 643);
//        actionNew_Level = new QAction(this);
//        actionNew_Level->setObjectName(QString::fromUtf8("actionNew_Level"));
//        actionOpen = new QAction(this);
//        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
//        actionSave = new QAction(this);
//        actionSave->setObjectName(QString::fromUtf8("actionSave"));
//        actionSave_As = new QAction(this);
//        actionSave_As->setObjectName(QString::fromUtf8("actionSave_As"));
//        actionExit = new QAction(this);
//        actionExit->setObjectName(QString::fromUtf8("actionExit"));
//        actionEdit_Terrain = new QAction(this);
//        actionEdit_Terrain->setObjectName(QString::fromUtf8("actionEdit_Terrain"));
//        actionEdit_Terrain->setCheckable(true);
//        actionEdit_Objects = new QAction(this);
//        actionEdit_Objects->setObjectName(QString::fromUtf8("actionEdit_Objects"));
//        actionEdit_Objects->setCheckable(true);
//        actionSave_grouped_Meshes = new QAction(this);
//        actionSave_grouped_Meshes->setObjectName(QString::fromUtf8("actionSave_grouped_Meshes"));
//        actionSave_grouped_Objects = new QAction(this);
//        actionSave_grouped_Objects->setObjectName(QString::fromUtf8("actionSave_grouped_Objects"));
//        actionSave_Collision = new QAction(this);
//        actionSave_Collision->setObjectName(QString::fromUtf8("actionSave_Collision"));
//        actionEditor_Settings = new QAction(this);
//        actionEditor_Settings->setObjectName(QString::fromUtf8("actionEditor_Settings"));
//        actionLevel_Settings = new QAction(this);
//        actionLevel_Settings->setObjectName(QString::fromUtf8("actionLevel_Settings"));
//        actionSnap_on_Grid = new QAction(this);
//        actionSnap_on_Grid->setObjectName(QString::fromUtf8("actionSnap_on_Grid"));
//        actionSnap_on_Grid->setCheckable(true);
//        actionSnap_on_Angle = new QAction(this);
//        actionSnap_on_Angle->setObjectName(QString::fromUtf8("actionSnap_on_Angle"));
//        actionSnap_on_Angle->setCheckable(true);
//        actionSnap_on_Scale = new QAction(this);
//        actionSnap_on_Scale->setObjectName(QString::fromUtf8("actionSnap_on_Scale"));
//        actionSnap_on_Scale->setCheckable(true);
//        actionPhysics = new QAction(this);
//        actionPhysics->setObjectName(QString::fromUtf8("actionPhysics"));
//        actionFog = new QAction(this);
//        actionFog->setObjectName(QString::fromUtf8("actionFog"));
//        actionMove_Mode = new QAction(this);
//        actionMove_Mode->setObjectName(QString::fromUtf8("actionMove_Mode"));
//        QIcon icon;
//        icon.addPixmap(QPixmap(QString::fromUtf8(":/editor/move.png")), QIcon::Normal, QIcon::Off);
//        icon.addPixmap(QPixmap(QString::fromUtf8(":/editor/move.png")), QIcon::Disabled, QIcon::Off);
//        actionMove_Mode->setIcon(icon);
//        actionRotate_Mode = new QAction(this);
//        actionRotate_Mode->setObjectName(QString::fromUtf8("actionRotate_Mode"));
//        QIcon icon1;
//        icon1.addPixmap(QPixmap(QString::fromUtf8(":/editor/rotate.png")), QIcon::Normal, QIcon::Off);
//        actionRotate_Mode->setIcon(icon1);
//        actionScale_Mode = new QAction(this);
//        actionScale_Mode->setObjectName(QString::fromUtf8("actionScale_Mode"));
//        QIcon icon2;
//        icon2.addPixmap(QPixmap(QString::fromUtf8(":/editor/scale.png")), QIcon::Normal, QIcon::Off);
//        actionScale_Mode->setIcon(icon2);
//        /*centralwidget = new QWidget(this);
//        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));*/
//        mdiArea = new QMdiArea();
//        mdiArea->setObjectName(QString::fromUtf8("mdiArea"));
//        this->setCentralWidget(mdiArea);
//        //horizontalLayout = new QHBoxLayout(mdiArea);
///*      horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
//        
//
//        horizontalLayout->addWidget(mdiArea);
//*/
//        
//        menubar = new QMenuBar(this);
//        menubar->setObjectName(QString::fromUtf8("menubar"));
//        menubar->setGeometry(QRect(0, 0, 644, 19));
//        menuFile = new QMenu(menubar);
//        menuFile->setObjectName(QString::fromUtf8("menuFile"));
//        menuEdit = new QMenu(menubar);
//        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
//        menuOptions = new QMenu(menubar);
//        menuOptions->setObjectName(QString::fromUtf8("menuOptions"));
//        this->setMenuBar(menubar);
//        statusbar = new QStatusBar(this);
//        statusbar->setObjectName(QString::fromUtf8("statusbar"));
//        this->setStatusBar(statusbar);
//        toolBar = new QToolBar(this);
//        toolBar->setObjectName(QString::fromUtf8("toolBar"));
//        this->addToolBar(Qt::TopToolBarArea, toolBar);
//
//        menubar->addAction(menuFile->menuAction());
//        menubar->addAction(menuEdit->menuAction());
//        menubar->addAction(menuOptions->menuAction());
//        menuFile->addAction(actionNew_Level);
//        menuFile->addAction(actionOpen);
//        menuFile->addSeparator();
//        menuFile->addAction(actionSave);
//        menuFile->addAction(actionSave_As);
//        menuFile->addSeparator();
//        menuFile->addAction(actionExit);
//        menuEdit->addAction(actionEdit_Terrain);
//        menuEdit->addAction(actionEdit_Objects);
//        menuEdit->addSeparator();
//        menuEdit->addAction(actionSave_grouped_Meshes);
//        menuEdit->addAction(actionSave_grouped_Objects);
//        menuEdit->addAction(actionSave_Collision);
//        menuOptions->addAction(actionEditor_Settings);
//        menuOptions->addAction(actionLevel_Settings);
//        menuOptions->addSeparator();
//        menuOptions->addAction(actionSnap_on_Grid);
//        menuOptions->addAction(actionSnap_on_Angle);
//        menuOptions->addAction(actionSnap_on_Scale);
//        menuOptions->addAction(actionPhysics);
//        menuOptions->addAction(actionFog);
//        toolBar->addAction(actionMove_Mode);
//        toolBar->addAction(actionRotate_Mode);
//        toolBar->addAction(actionScale_Mode);
//        toolBar->addSeparator();
//
//        retranslateUi(this);
//
//        QMetaObject::connectSlotsByName(this);
//    } // setupUi
//
//    void retranslateUi(QMainWindow *this)
//    {
//        this->setWindowTitle(QApplication::translate("this", "this", 0, QApplication::UnicodeUTF8));
//        actionNew_Level->setText(QApplication::translate("this", "New Level", 0, QApplication::UnicodeUTF8));
//        actionNew_Level->setShortcut(QApplication::translate("this", "Ctrl+N", 0, QApplication::UnicodeUTF8));
//        actionOpen->setText(QApplication::translate("this", "Open", 0, QApplication::UnicodeUTF8));
//        actionOpen->setShortcut(QApplication::translate("this", "Ctrl+O", 0, QApplication::UnicodeUTF8));
//        actionSave->setText(QApplication::translate("this", "Save", 0, QApplication::UnicodeUTF8));
//        actionSave->setShortcut(QApplication::translate("this", "Ctrl+S", 0, QApplication::UnicodeUTF8));
//        actionSave_As->setText(QApplication::translate("this", "Save As", 0, QApplication::UnicodeUTF8));
//        actionExit->setText(QApplication::translate("this", "Exit", 0, QApplication::UnicodeUTF8));
//        actionEdit_Terrain->setText(QApplication::translate("this", "Edit Terrain", 0, QApplication::UnicodeUTF8));
//        actionEdit_Objects->setText(QApplication::translate("this", "Edit Objects", 0, QApplication::UnicodeUTF8));
//        actionSave_grouped_Meshes->setText(QApplication::translate("this", "Save grouped Meshes", 0, QApplication::UnicodeUTF8));
//        actionSave_grouped_Objects->setText(QApplication::translate("this", "Save grouped Objects", 0, QApplication::UnicodeUTF8));
//        actionSave_Collision->setText(QApplication::translate("this", "Save Collision", 0, QApplication::UnicodeUTF8));
//        actionEditor_Settings->setText(QApplication::translate("this", "Editor Settings", 0, QApplication::UnicodeUTF8));
//        actionLevel_Settings->setText(QApplication::translate("this", "Level Settings", 0, QApplication::UnicodeUTF8));
//        actionSnap_on_Grid->setText(QApplication::translate("this", "Snap on Grid", 0, QApplication::UnicodeUTF8));
//        actionSnap_on_Angle->setText(QApplication::translate("this", "Snap on Angle", 0, QApplication::UnicodeUTF8));
//        actionSnap_on_Scale->setText(QApplication::translate("this", "Snap on Scale", 0, QApplication::UnicodeUTF8));
//        actionPhysics->setText(QApplication::translate("this", "Physics", 0, QApplication::UnicodeUTF8));
//        actionFog->setText(QApplication::translate("this", "Fog", 0, QApplication::UnicodeUTF8));
//        actionMove_Mode->setText(QApplication::translate("this", "Set Move Mode", 0, QApplication::UnicodeUTF8));
//        actionMove_Mode->setShortcut(QApplication::translate("this", "M", 0, QApplication::UnicodeUTF8));
//        actionRotate_Mode->setText(QApplication::translate("this", "Rotate Mode", 0, QApplication::UnicodeUTF8));
//        actionRotate_Mode->setShortcut(QApplication::translate("this", "R", 0, QApplication::UnicodeUTF8));
//        actionScale_Mode->setText(QApplication::translate("this", "Scale Mode", 0, QApplication::UnicodeUTF8));
//        actionScale_Mode->setShortcut(QApplication::translate("this", "S", 0, QApplication::UnicodeUTF8));
//        menuFile->setTitle(QApplication::translate("this", "File", 0, QApplication::UnicodeUTF8));
//        menuEdit->setTitle(QApplication::translate("this", "Edit", 0, QApplication::UnicodeUTF8));
//        menuOptions->setTitle(QApplication::translate("this", "Options", 0, QApplication::UnicodeUTF8));
//        toolBar->setWindowTitle(QApplication::translate("this", "toolBar", 0, QApplication::UnicodeUTF8));
//    } // retranslateUi
//
//};
//
//namespace Ui {
//    class this: public Ui_MainWindow {};
//} // namespace Ui
//
//QT_END_NAMESPACE
