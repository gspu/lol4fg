/********************************************************************************
** Form generated from reading UI file 'Objectlist.ui'
**
** Created: Tue 7. Sep 16:44:54 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef OBJECTLIST_H
#define OBJECTLIST_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ObjectSelectDialog
{
public:
    QVBoxLayout *verticalLayout;
    QTreeWidget *objectList;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *addObject;

    void setupUi(QDialog *ObjectSelectDialog)
    {
        if (ObjectSelectDialog->objectName().isEmpty())
            ObjectSelectDialog->setObjectName(QString::fromUtf8("ObjectSelectDialog"));
        ObjectSelectDialog->resize(279, 375);
        verticalLayout = new QVBoxLayout(ObjectSelectDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        objectList = new QTreeWidget(ObjectSelectDialog);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem(objectList);
        new QTreeWidgetItem(__qtreewidgetitem);
        new QTreeWidgetItem(__qtreewidgetitem);
        new QTreeWidgetItem(__qtreewidgetitem);
        QTreeWidgetItem *__qtreewidgetitem1 = new QTreeWidgetItem(objectList);
        new QTreeWidgetItem(__qtreewidgetitem1);
        objectList->setObjectName(QString::fromUtf8("objectList"));
        objectList->setWordWrap(false);
        objectList->header()->setVisible(false);

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

    void retranslateUi(QDialog *ObjectSelectDialog)
    {
        ObjectSelectDialog->setWindowTitle(QApplication::translate("ObjectSelectDialog", "Objects", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem = objectList->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("ObjectSelectDialog", "1", 0, QApplication::UnicodeUTF8));

        const bool __sortingEnabled = objectList->isSortingEnabled();
        objectList->setSortingEnabled(false);
        QTreeWidgetItem *___qtreewidgetitem1 = objectList->topLevelItem(0);
        ___qtreewidgetitem1->setText(0, QApplication::translate("ObjectSelectDialog", "static", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem2 = ___qtreewidgetitem1->child(0);
        ___qtreewidgetitem2->setText(0, QApplication::translate("ObjectSelectDialog", "mesh1", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem3 = ___qtreewidgetitem1->child(1);
        ___qtreewidgetitem3->setText(0, QApplication::translate("ObjectSelectDialog", "mesh2", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem4 = ___qtreewidgetitem1->child(2);
        ___qtreewidgetitem4->setText(0, QApplication::translate("ObjectSelectDialog", "mesh3", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem5 = objectList->topLevelItem(1);
        ___qtreewidgetitem5->setText(0, QApplication::translate("ObjectSelectDialog", "item", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem6 = ___qtreewidgetitem5->child(0);
        ___qtreewidgetitem6->setText(0, QApplication::translate("ObjectSelectDialog", "blah", 0, QApplication::UnicodeUTF8));
        objectList->setSortingEnabled(__sortingEnabled);

        addObject->setText(QApplication::translate("ObjectSelectDialog", "Add >>", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ObjectSelectDialog: public Ui_ObjectSelectDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // OBJECTLIST_H
