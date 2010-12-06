/********************************************************************************
** Form generated from reading UI file 'objectselect.ui'
**
** Created: Tue 7. Sep 16:44:54 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef OBJECTSELECT_H
#define OBJECTSELECT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QListWidget>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ObjectSelectDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QTabWidget *mainTabCtrl;
    QWidget *tab;
    QVBoxLayout *verticalLayout;
    QListWidget *staticObjectList;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_3;
    QListWidget *dynamicObjectList;
    QWidget *Seite;
    QHBoxLayout *horizontalLayout;
    QListWidget *dotsceneObjectList;
    QWidget *Seite_2;
    QHBoxLayout *horizontalLayout_2;
    QListWidget *itemObjectList;
    QWidget *Seite_3;
    QHBoxLayout *horizontalLayout_3;
    QListWidget *charObjectList;
    QWidget *Seite_4;
    QHBoxLayout *horizontalLayout_4;
    QListWidget *doorObjectList;
    QWidget *Seite_5;
    QHBoxLayout *horizontalLayout_5;
    QListWidget *contObjectList;
    QWidget *Seite_6;
    QHBoxLayout *horizontalLayout_6;
    QListWidget *elseObjectList;

    void setupUi(QDialog *ObjectSelectDialog)
    {
        if (ObjectSelectDialog->objectName().isEmpty())
            ObjectSelectDialog->setObjectName(QString::fromUtf8("ObjectSelectDialog"));
        ObjectSelectDialog->resize(277, 361);
        verticalLayout_2 = new QVBoxLayout(ObjectSelectDialog);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        mainTabCtrl = new QTabWidget(ObjectSelectDialog);
        mainTabCtrl->setObjectName(QString::fromUtf8("mainTabCtrl"));
        mainTabCtrl->setElideMode(Qt::ElideMiddle);
        mainTabCtrl->setUsesScrollButtons(true);
        mainTabCtrl->setDocumentMode(false);
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        verticalLayout = new QVBoxLayout(tab);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        staticObjectList = new QListWidget(tab);
        staticObjectList->setObjectName(QString::fromUtf8("staticObjectList"));

        verticalLayout->addWidget(staticObjectList);

        mainTabCtrl->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        verticalLayout_3 = new QVBoxLayout(tab_2);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        dynamicObjectList = new QListWidget(tab_2);
        dynamicObjectList->setObjectName(QString::fromUtf8("dynamicObjectList"));

        verticalLayout_3->addWidget(dynamicObjectList);

        mainTabCtrl->addTab(tab_2, QString());
        Seite = new QWidget();
        Seite->setObjectName(QString::fromUtf8("Seite"));
        horizontalLayout = new QHBoxLayout(Seite);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        dotsceneObjectList = new QListWidget(Seite);
        dotsceneObjectList->setObjectName(QString::fromUtf8("dotsceneObjectList"));

        horizontalLayout->addWidget(dotsceneObjectList);

        mainTabCtrl->addTab(Seite, QString());
        Seite_2 = new QWidget();
        Seite_2->setObjectName(QString::fromUtf8("Seite_2"));
        horizontalLayout_2 = new QHBoxLayout(Seite_2);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        itemObjectList = new QListWidget(Seite_2);
        itemObjectList->setObjectName(QString::fromUtf8("itemObjectList"));

        horizontalLayout_2->addWidget(itemObjectList);

        mainTabCtrl->addTab(Seite_2, QString());
        Seite_3 = new QWidget();
        Seite_3->setObjectName(QString::fromUtf8("Seite_3"));
        horizontalLayout_3 = new QHBoxLayout(Seite_3);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        charObjectList = new QListWidget(Seite_3);
        charObjectList->setObjectName(QString::fromUtf8("charObjectList"));

        horizontalLayout_3->addWidget(charObjectList);

        mainTabCtrl->addTab(Seite_3, QString());
        Seite_4 = new QWidget();
        Seite_4->setObjectName(QString::fromUtf8("Seite_4"));
        horizontalLayout_4 = new QHBoxLayout(Seite_4);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        doorObjectList = new QListWidget(Seite_4);
        doorObjectList->setObjectName(QString::fromUtf8("doorObjectList"));

        horizontalLayout_4->addWidget(doorObjectList);

        mainTabCtrl->addTab(Seite_4, QString());
        Seite_5 = new QWidget();
        Seite_5->setObjectName(QString::fromUtf8("Seite_5"));
        horizontalLayout_5 = new QHBoxLayout(Seite_5);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        contObjectList = new QListWidget(Seite_5);
        contObjectList->setObjectName(QString::fromUtf8("contObjectList"));

        horizontalLayout_5->addWidget(contObjectList);

        mainTabCtrl->addTab(Seite_5, QString());
        Seite_6 = new QWidget();
        Seite_6->setObjectName(QString::fromUtf8("Seite_6"));
        horizontalLayout_6 = new QHBoxLayout(Seite_6);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        elseObjectList = new QListWidget(Seite_6);
        elseObjectList->setObjectName(QString::fromUtf8("elseObjectList"));

        horizontalLayout_6->addWidget(elseObjectList);

        mainTabCtrl->addTab(Seite_6, QString());

        verticalLayout_2->addWidget(mainTabCtrl);


        retranslateUi(ObjectSelectDialog);

        mainTabCtrl->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(ObjectSelectDialog);
    } // setupUi

    void retranslateUi(QDialog *ObjectSelectDialog)
    {
        ObjectSelectDialog->setWindowTitle(QApplication::translate("ObjectSelectDialog", "Objects", 0, QApplication::UnicodeUTF8));
        mainTabCtrl->setTabText(mainTabCtrl->indexOf(tab), QApplication::translate("ObjectSelectDialog", "Static", 0, QApplication::UnicodeUTF8));
        mainTabCtrl->setTabText(mainTabCtrl->indexOf(tab_2), QApplication::translate("ObjectSelectDialog", "Dynamic", 0, QApplication::UnicodeUTF8));
        mainTabCtrl->setTabText(mainTabCtrl->indexOf(Seite), QApplication::translate("ObjectSelectDialog", "DotScene", 0, QApplication::UnicodeUTF8));
        mainTabCtrl->setTabText(mainTabCtrl->indexOf(Seite_2), QApplication::translate("ObjectSelectDialog", "Items", 0, QApplication::UnicodeUTF8));
        mainTabCtrl->setTabText(mainTabCtrl->indexOf(Seite_3), QApplication::translate("ObjectSelectDialog", "Chars", 0, QApplication::UnicodeUTF8));
        mainTabCtrl->setTabText(mainTabCtrl->indexOf(Seite_4), QApplication::translate("ObjectSelectDialog", "Doors", 0, QApplication::UnicodeUTF8));
        mainTabCtrl->setTabText(mainTabCtrl->indexOf(Seite_5), QApplication::translate("ObjectSelectDialog", "Containers", 0, QApplication::UnicodeUTF8));
        mainTabCtrl->setTabText(mainTabCtrl->indexOf(Seite_6), QApplication::translate("ObjectSelectDialog", "Else", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ObjectSelectDialog: public Ui_ObjectSelectDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // OBJECTSELECT_H
