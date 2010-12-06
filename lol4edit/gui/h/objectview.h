/********************************************************************************
** Form generated from reading UI file 'objectview.ui'
**
** Created: Tue 7. Sep 16:44:54 2010
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
    QVBoxLayout *verticalLayout;
    QListWidget *levelObjectList;

    void setupUi(QDialog *ObjectListDialog)
    {
        if (ObjectListDialog->objectName().isEmpty())
            ObjectListDialog->setObjectName(QString::fromUtf8("ObjectListDialog"));
        ObjectListDialog->resize(249, 278);
        verticalLayout = new QVBoxLayout(ObjectListDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        levelObjectList = new QListWidget(ObjectListDialog);
        levelObjectList->setObjectName(QString::fromUtf8("levelObjectList"));

        verticalLayout->addWidget(levelObjectList);


        retranslateUi(ObjectListDialog);

        QMetaObject::connectSlotsByName(ObjectListDialog);
    } // setupUi

    void retranslateUi(QDialog *ObjectListDialog)
    {
        ObjectListDialog->setWindowTitle(QApplication::translate("ObjectListDialog", "Objects in Level", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ObjectListDialog: public Ui_ObjectListDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // OBJECTVIEW_H
