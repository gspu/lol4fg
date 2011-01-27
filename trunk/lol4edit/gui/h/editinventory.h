/********************************************************************************
** Form generated from reading UI file 'editinventory.ui'
**
** Created: Wed 26. Jan 20:40:28 2011
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef EDITINVENTORY_H
#define EDITINVENTORY_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EditInventory
{
public:
    QHBoxLayout *horizontalLayout;
    QTreeWidget *inventoryList;
    QVBoxLayout *verticalLayout_8;
    QSpinBox *itemCountSpinbox;
    QPushButton *newItemButton;
    QPushButton *deleteItemButton;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *EditInventory)
    {
        if (EditInventory->objectName().isEmpty())
            EditInventory->setObjectName(QString::fromUtf8("EditInventory"));
        EditInventory->resize(400, 300);
        horizontalLayout = new QHBoxLayout(EditInventory);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        inventoryList = new QTreeWidget(EditInventory);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setTextAlignment(0, Qt::AlignLeft|Qt::AlignVCenter);
        inventoryList->setHeaderItem(__qtreewidgetitem);
        new QTreeWidgetItem(inventoryList);
        new QTreeWidgetItem(inventoryList);
        inventoryList->setObjectName(QString::fromUtf8("inventoryList"));
        inventoryList->setAcceptDrops(true);
        inventoryList->setRootIsDecorated(false);
        inventoryList->setUniformRowHeights(false);
        inventoryList->setItemsExpandable(false);
        inventoryList->setAllColumnsShowFocus(true);
        inventoryList->setHeaderHidden(false);
        inventoryList->setExpandsOnDoubleClick(false);
        inventoryList->header()->setCascadingSectionResizes(false);
        inventoryList->header()->setDefaultSectionSize(96);
        inventoryList->header()->setStretchLastSection(true);

        horizontalLayout->addWidget(inventoryList);

        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        itemCountSpinbox = new QSpinBox(EditInventory);
        itemCountSpinbox->setObjectName(QString::fromUtf8("itemCountSpinbox"));

        verticalLayout_8->addWidget(itemCountSpinbox);

        newItemButton = new QPushButton(EditInventory);
        newItemButton->setObjectName(QString::fromUtf8("newItemButton"));

        verticalLayout_8->addWidget(newItemButton);

        deleteItemButton = new QPushButton(EditInventory);
        deleteItemButton->setObjectName(QString::fromUtf8("deleteItemButton"));

        verticalLayout_8->addWidget(deleteItemButton);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_8->addItem(verticalSpacer);


        horizontalLayout->addLayout(verticalLayout_8);


        retranslateUi(EditInventory);

        QMetaObject::connectSlotsByName(EditInventory);
    } // setupUi

    void retranslateUi(QWidget *EditInventory)
    {
        EditInventory->setWindowTitle(QApplication::translate("EditInventory", "Form", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem = inventoryList->headerItem();
        ___qtreewidgetitem->setText(1, QApplication::translate("EditInventory", "Count", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(0, QApplication::translate("EditInventory", "Item", 0, QApplication::UnicodeUTF8));

        const bool __sortingEnabled = inventoryList->isSortingEnabled();
        inventoryList->setSortingEnabled(false);
        QTreeWidgetItem *___qtreewidgetitem1 = inventoryList->topLevelItem(0);
        ___qtreewidgetitem1->setText(1, QApplication::translate("EditInventory", "1", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem1->setText(0, QApplication::translate("EditInventory", "testitem", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem2 = inventoryList->topLevelItem(1);
        ___qtreewidgetitem2->setText(1, QApplication::translate("EditInventory", "3", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem2->setText(0, QApplication::translate("EditInventory", "asd", 0, QApplication::UnicodeUTF8));
        inventoryList->setSortingEnabled(__sortingEnabled);

        newItemButton->setText(QApplication::translate("EditInventory", "New", 0, QApplication::UnicodeUTF8));
        deleteItemButton->setText(QApplication::translate("EditInventory", "Delete", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class EditInventory: public Ui_EditInventory {};
} // namespace Ui

QT_END_NAMESPACE

#endif // EDITINVENTORY_H
