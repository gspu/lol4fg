/********************************************************************************
** Form generated from reading UI file 'objectview.ui'
**
** Created: Sat 22. Jan 17:36:42 2011
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef OBJECTVIEW_H
#define OBJECTVIEW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QListWidget>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ObjectListDialog
{
public:
    QAction *actionGo_to_Object;
    QAction *actionSelect_Object;
    QVBoxLayout *verticalLayout;
    QListWidget *levelObjectList;

    void setupUi(QDialog *ObjectListDialog)
    {
        if (ObjectListDialog->objectName().isEmpty())
            ObjectListDialog->setObjectName(QString::fromUtf8("ObjectListDialog"));
        ObjectListDialog->resize(249, 278);
        actionGo_to_Object = new QAction(ObjectListDialog);
        actionGo_to_Object->setObjectName(QString::fromUtf8("actionGo_to_Object"));
        actionSelect_Object = new QAction(ObjectListDialog);
        actionSelect_Object->setObjectName(QString::fromUtf8("actionSelect_Object"));
        verticalLayout = new QVBoxLayout(ObjectListDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        levelObjectList = new QListWidget(ObjectListDialog);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icon/box.png"), QSize(), QIcon::Normal, QIcon::Off);
        QListWidgetItem *__qlistwidgetitem = new QListWidgetItem(levelObjectList);
        __qlistwidgetitem->setIcon(icon);
        levelObjectList->setObjectName(QString::fromUtf8("levelObjectList"));

        verticalLayout->addWidget(levelObjectList);


        retranslateUi(ObjectListDialog);

        QMetaObject::connectSlotsByName(ObjectListDialog);
    } // setupUi

    void retranslateUi(QDialog *ObjectListDialog)
    {
        ObjectListDialog->setWindowTitle(QApplication::translate("ObjectListDialog", "Objects in Level", 0, QApplication::UnicodeUTF8));
        actionGo_to_Object->setText(QApplication::translate("ObjectListDialog", "Go to Object", 0, QApplication::UnicodeUTF8));
        actionSelect_Object->setText(QApplication::translate("ObjectListDialog", "Select Object", 0, QApplication::UnicodeUTF8));

        const bool __sortingEnabled = levelObjectList->isSortingEnabled();
        levelObjectList->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem = levelObjectList->item(0);
        ___qlistwidgetitem->setText(QApplication::translate("ObjectListDialog", "test", 0, QApplication::UnicodeUTF8));
        levelObjectList->setSortingEnabled(__sortingEnabled);

    } // retranslateUi

};

namespace Ui {
    class ObjectListDialog: public Ui_ObjectListDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // OBJECTVIEW_H
