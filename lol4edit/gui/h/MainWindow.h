/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created: Tue 7. Sep 16:44:54 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionNew_Level;
    QAction *actionOpen;
    QAction *actionSave;
    QAction *actionSave_As;
    QAction *actionExit;
    QAction *actionEdit_Terrain;
    QAction *actionEdit_Objects;
    QAction *actionSave_grouped_Meshes;
    QAction *actionSave_grouped_Objects;
    QAction *actionSave_Collision;
    QAction *actionEditor_Settings;
    QAction *actionLevel_Settings;
    QAction *actionSnap_on_Grid;
    QAction *actionSnap_on_Angle;
    QAction *actionSnap_on_Scale;
    QAction *actionPhysics;
    QAction *actionFog;
    QAction *actionMove_Mode;
    QAction *actionRotate_Mode;
    QAction *actionScale_Mode;
    QWidget *mdiArea;
    QHBoxLayout *horizontalLayout;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuOptions;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(644, 643);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/editor/hammergrauklein.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        actionNew_Level = new QAction(MainWindow);
        actionNew_Level->setObjectName(QString::fromUtf8("actionNew_Level"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/editor/New.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionNew_Level->setIcon(icon1);
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/editor/Open.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen->setIcon(icon2);
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/editor/Save.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave->setIcon(icon3);
        actionSave_As = new QAction(MainWindow);
        actionSave_As->setObjectName(QString::fromUtf8("actionSave_As"));
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionEdit_Terrain = new QAction(MainWindow);
        actionEdit_Terrain->setObjectName(QString::fromUtf8("actionEdit_Terrain"));
        actionEdit_Terrain->setCheckable(true);
        actionEdit_Objects = new QAction(MainWindow);
        actionEdit_Objects->setObjectName(QString::fromUtf8("actionEdit_Objects"));
        actionEdit_Objects->setCheckable(true);
        actionSave_grouped_Meshes = new QAction(MainWindow);
        actionSave_grouped_Meshes->setObjectName(QString::fromUtf8("actionSave_grouped_Meshes"));
        actionSave_grouped_Objects = new QAction(MainWindow);
        actionSave_grouped_Objects->setObjectName(QString::fromUtf8("actionSave_grouped_Objects"));
        actionSave_Collision = new QAction(MainWindow);
        actionSave_Collision->setObjectName(QString::fromUtf8("actionSave_Collision"));
        actionEditor_Settings = new QAction(MainWindow);
        actionEditor_Settings->setObjectName(QString::fromUtf8("actionEditor_Settings"));
        actionLevel_Settings = new QAction(MainWindow);
        actionLevel_Settings->setObjectName(QString::fromUtf8("actionLevel_Settings"));
        actionSnap_on_Grid = new QAction(MainWindow);
        actionSnap_on_Grid->setObjectName(QString::fromUtf8("actionSnap_on_Grid"));
        actionSnap_on_Grid->setCheckable(true);
        actionSnap_on_Grid->setChecked(true);
        actionSnap_on_Angle = new QAction(MainWindow);
        actionSnap_on_Angle->setObjectName(QString::fromUtf8("actionSnap_on_Angle"));
        actionSnap_on_Angle->setCheckable(true);
        actionSnap_on_Angle->setChecked(true);
        actionSnap_on_Scale = new QAction(MainWindow);
        actionSnap_on_Scale->setObjectName(QString::fromUtf8("actionSnap_on_Scale"));
        actionSnap_on_Scale->setCheckable(true);
        actionSnap_on_Scale->setChecked(true);
        actionPhysics = new QAction(MainWindow);
        actionPhysics->setObjectName(QString::fromUtf8("actionPhysics"));
        actionPhysics->setCheckable(true);
        actionFog = new QAction(MainWindow);
        actionFog->setObjectName(QString::fromUtf8("actionFog"));
        actionMove_Mode = new QAction(MainWindow);
        actionMove_Mode->setObjectName(QString::fromUtf8("actionMove_Mode"));
        actionMove_Mode->setCheckable(true);
        actionMove_Mode->setChecked(true);
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/editor/arrow_move.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionMove_Mode->setIcon(icon4);
        actionRotate_Mode = new QAction(MainWindow);
        actionRotate_Mode->setObjectName(QString::fromUtf8("actionRotate_Mode"));
        actionRotate_Mode->setCheckable(true);
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/editor/arrow_rotate.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRotate_Mode->setIcon(icon5);
        actionScale_Mode = new QAction(MainWindow);
        actionScale_Mode->setObjectName(QString::fromUtf8("actionScale_Mode"));
        actionScale_Mode->setCheckable(true);
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/editor/arrow_scale.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionScale_Mode->setIcon(icon6);
        mdiArea = new QWidget(MainWindow);
        mdiArea->setObjectName(QString::fromUtf8("mdiArea"));
        horizontalLayout = new QHBoxLayout(mdiArea);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        MainWindow->setCentralWidget(mdiArea);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 644, 18));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuEdit = new QMenu(menubar);
        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
        menuOptions = new QMenu(menubar);
        menuOptions->setObjectName(QString::fromUtf8("menuOptions"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        toolBar->setMovable(false);
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuEdit->menuAction());
        menubar->addAction(menuOptions->menuAction());
        menuFile->addAction(actionNew_Level);
        menuFile->addAction(actionOpen);
        menuFile->addSeparator();
        menuFile->addAction(actionSave);
        menuFile->addAction(actionSave_As);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuEdit->addAction(actionEdit_Terrain);
        menuEdit->addAction(actionEdit_Objects);
        menuEdit->addSeparator();
        menuEdit->addAction(actionSave_grouped_Meshes);
        menuEdit->addAction(actionSave_grouped_Objects);
        menuEdit->addAction(actionSave_Collision);
        menuOptions->addAction(actionEditor_Settings);
        menuOptions->addAction(actionLevel_Settings);
        menuOptions->addSeparator();
        menuOptions->addAction(actionSnap_on_Grid);
        menuOptions->addAction(actionSnap_on_Angle);
        menuOptions->addAction(actionSnap_on_Scale);
        menuOptions->addAction(actionPhysics);
        menuOptions->addAction(actionFog);
        toolBar->addAction(actionNew_Level);
        toolBar->addAction(actionOpen);
        toolBar->addAction(actionSave);
        toolBar->addSeparator();
        toolBar->addAction(actionMove_Mode);
        toolBar->addAction(actionRotate_Mode);
        toolBar->addAction(actionScale_Mode);
        toolBar->addSeparator();

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Editor", 0, QApplication::UnicodeUTF8));
        actionNew_Level->setText(QApplication::translate("MainWindow", "New Level", 0, QApplication::UnicodeUTF8));
        actionNew_Level->setShortcut(QApplication::translate("MainWindow", "Ctrl+N", 0, QApplication::UnicodeUTF8));
        actionOpen->setText(QApplication::translate("MainWindow", "Open", 0, QApplication::UnicodeUTF8));
        actionOpen->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", 0, QApplication::UnicodeUTF8));
        actionSave->setText(QApplication::translate("MainWindow", "Save", 0, QApplication::UnicodeUTF8));
        actionSave->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", 0, QApplication::UnicodeUTF8));
        actionSave_As->setText(QApplication::translate("MainWindow", "Save As", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("MainWindow", "Exit", 0, QApplication::UnicodeUTF8));
        actionEdit_Terrain->setText(QApplication::translate("MainWindow", "Edit Terrain", 0, QApplication::UnicodeUTF8));
        actionEdit_Objects->setText(QApplication::translate("MainWindow", "Edit Objects", 0, QApplication::UnicodeUTF8));
        actionSave_grouped_Meshes->setText(QApplication::translate("MainWindow", "Save grouped Meshes", 0, QApplication::UnicodeUTF8));
        actionSave_grouped_Objects->setText(QApplication::translate("MainWindow", "Save grouped Objects", 0, QApplication::UnicodeUTF8));
        actionSave_Collision->setText(QApplication::translate("MainWindow", "Save Collision", 0, QApplication::UnicodeUTF8));
        actionEditor_Settings->setText(QApplication::translate("MainWindow", "Editor Settings", 0, QApplication::UnicodeUTF8));
        actionLevel_Settings->setText(QApplication::translate("MainWindow", "Level Settings", 0, QApplication::UnicodeUTF8));
        actionSnap_on_Grid->setText(QApplication::translate("MainWindow", "Snap on Grid", 0, QApplication::UnicodeUTF8));
        actionSnap_on_Angle->setText(QApplication::translate("MainWindow", "Snap on Angle", 0, QApplication::UnicodeUTF8));
        actionSnap_on_Scale->setText(QApplication::translate("MainWindow", "Snap on Scale", 0, QApplication::UnicodeUTF8));
        actionPhysics->setText(QApplication::translate("MainWindow", "Physics", 0, QApplication::UnicodeUTF8));
        actionFog->setText(QApplication::translate("MainWindow", "Fog", 0, QApplication::UnicodeUTF8));
        actionMove_Mode->setText(QApplication::translate("MainWindow", "Set Move Mode", 0, QApplication::UnicodeUTF8));
        actionMove_Mode->setShortcut(QApplication::translate("MainWindow", "M", 0, QApplication::UnicodeUTF8));
        actionRotate_Mode->setText(QApplication::translate("MainWindow", "Rotate Mode", 0, QApplication::UnicodeUTF8));
        actionRotate_Mode->setShortcut(QApplication::translate("MainWindow", "R", 0, QApplication::UnicodeUTF8));
        actionScale_Mode->setText(QApplication::translate("MainWindow", "Scale Mode", 0, QApplication::UnicodeUTF8));
        actionScale_Mode->setShortcut(QApplication::translate("MainWindow", "S", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8));
        menuEdit->setTitle(QApplication::translate("MainWindow", "Edit", 0, QApplication::UnicodeUTF8));
        menuOptions->setTitle(QApplication::translate("MainWindow", "Options", 0, QApplication::UnicodeUTF8));
        toolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // MAINWINDOW_H
