/********************************************************************************
** Form generated from reading UI file 'Objectlist_Widget.ui'
**
** Created: Wed 26. Jan 20:40:28 2011
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef OBJECTLIST_WIDGET_H
#define OBJECTLIST_WIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ObjectSelectDialog
{
public:
    QVBoxLayout *verticalLayout;
    QTreeView *objectList;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *addObject;

    void setupUi(QWidget *ObjectSelectDialog)
    {
        if (ObjectSelectDialog->objectName().isEmpty())
            ObjectSelectDialog->setObjectName(QString::fromUtf8("ObjectSelectDialog"));
        ObjectSelectDialog->resize(246, 334);
        verticalLayout = new QVBoxLayout(ObjectSelectDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        objectList = new QTreeView(ObjectSelectDialog);
        objectList->setObjectName(QString::fromUtf8("objectList"));

        verticalLayout->addWidget(objectList);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        addObject = new QPushButton(ObjectSelectDialog);
        addObject->setObjectName(QString::fromUtf8("addObject"));

        horizontalLayout->addWidget(addObject);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(ObjectSelectDialog);

        QMetaObject::connectSlotsByName(ObjectSelectDialog);
    } // setupUi

    void retranslateUi(QWidget *ObjectSelectDialog)
    {
        ObjectSelectDialog->setWindowTitle(QApplication::translate("ObjectSelectDialog", "Form", 0, QApplication::UnicodeUTF8));
        addObject->setText(QApplication::translate("ObjectSelectDialog", "Add >>", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ObjectSelectDialog: public Ui_ObjectSelectDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // OBJECTLIST_WIDGET_H
