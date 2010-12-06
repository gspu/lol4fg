/********************************************************************************
** Form generated from reading UI file 'CharacterData.ui'
**
** Created: Tue 7. Sep 16:44:53 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef CHARACTERDATA_H
#define CHARACTERDATA_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CharacterData
{
public:
    QHBoxLayout *horizontalLayout;
    QTabWidget *tabWidget;
    QWidget *tabEquipment;
    QGridLayout *gridLayout;
    QLabel *label_9;
    QLineEdit *charMelee;
    QLabel *label_10;
    QLineEdit *charRanged;
    QLabel *label_11;
    QLineEdit *charArmor;
    QLabel *label_12;
    QLineEdit *charShield;
    QLabel *label_14;
    QLineEdit *charElse1;
    QLabel *label_17;
    QLineEdit *charElse2;
    QLabel *label_15;
    QLineEdit *charElse3;
    QLabel *label_18;
    QLineEdit *charElse4;
    QLabel *label_16;
    QLineEdit *charBag;
    QLabel *label_19;
    QLineEdit *charSpell;
    QSpacerItem *verticalSpacer;
    QWidget *tabInventory;
    QHBoxLayout *horizontalLayout_3;
    QWidget *inventoryPlaceholder;
    QWidget *tabSpellbook;
    QHBoxLayout *horizontalLayout_2;
    QTreeWidget *spellbookList;
    QVBoxLayout *verticalLayout_9;
    QPushButton *addSpellButton;
    QPushButton *deleteSpellButton;
    QSpacerItem *verticalSpacer_2;

    void setupUi(QWidget *CharacterData)
    {
        if (CharacterData->objectName().isEmpty())
            CharacterData->setObjectName(QString::fromUtf8("CharacterData"));
        CharacterData->resize(457, 298);
        horizontalLayout = new QHBoxLayout(CharacterData);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        tabWidget = new QTabWidget(CharacterData);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabEquipment = new QWidget();
        tabEquipment->setObjectName(QString::fromUtf8("tabEquipment"));
        gridLayout = new QGridLayout(tabEquipment);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_9 = new QLabel(tabEquipment);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout->addWidget(label_9, 0, 0, 1, 1);

        charMelee = new QLineEdit(tabEquipment);
        charMelee->setObjectName(QString::fromUtf8("charMelee"));

        gridLayout->addWidget(charMelee, 0, 1, 1, 1);

        label_10 = new QLabel(tabEquipment);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout->addWidget(label_10, 0, 2, 1, 1);

        charRanged = new QLineEdit(tabEquipment);
        charRanged->setObjectName(QString::fromUtf8("charRanged"));

        gridLayout->addWidget(charRanged, 0, 3, 1, 1);

        label_11 = new QLabel(tabEquipment);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout->addWidget(label_11, 1, 0, 1, 1);

        charArmor = new QLineEdit(tabEquipment);
        charArmor->setObjectName(QString::fromUtf8("charArmor"));

        gridLayout->addWidget(charArmor, 1, 1, 1, 1);

        label_12 = new QLabel(tabEquipment);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        gridLayout->addWidget(label_12, 1, 2, 1, 1);

        charShield = new QLineEdit(tabEquipment);
        charShield->setObjectName(QString::fromUtf8("charShield"));

        gridLayout->addWidget(charShield, 1, 3, 1, 1);

        label_14 = new QLabel(tabEquipment);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        gridLayout->addWidget(label_14, 2, 0, 1, 1);

        charElse1 = new QLineEdit(tabEquipment);
        charElse1->setObjectName(QString::fromUtf8("charElse1"));

        gridLayout->addWidget(charElse1, 2, 1, 1, 1);

        label_17 = new QLabel(tabEquipment);
        label_17->setObjectName(QString::fromUtf8("label_17"));

        gridLayout->addWidget(label_17, 2, 2, 1, 1);

        charElse2 = new QLineEdit(tabEquipment);
        charElse2->setObjectName(QString::fromUtf8("charElse2"));

        gridLayout->addWidget(charElse2, 2, 3, 1, 1);

        label_15 = new QLabel(tabEquipment);
        label_15->setObjectName(QString::fromUtf8("label_15"));

        gridLayout->addWidget(label_15, 3, 0, 1, 1);

        charElse3 = new QLineEdit(tabEquipment);
        charElse3->setObjectName(QString::fromUtf8("charElse3"));

        gridLayout->addWidget(charElse3, 3, 1, 1, 1);

        label_18 = new QLabel(tabEquipment);
        label_18->setObjectName(QString::fromUtf8("label_18"));

        gridLayout->addWidget(label_18, 3, 2, 1, 1);

        charElse4 = new QLineEdit(tabEquipment);
        charElse4->setObjectName(QString::fromUtf8("charElse4"));

        gridLayout->addWidget(charElse4, 3, 3, 1, 1);

        label_16 = new QLabel(tabEquipment);
        label_16->setObjectName(QString::fromUtf8("label_16"));

        gridLayout->addWidget(label_16, 4, 0, 1, 1);

        charBag = new QLineEdit(tabEquipment);
        charBag->setObjectName(QString::fromUtf8("charBag"));

        gridLayout->addWidget(charBag, 4, 1, 1, 1);

        label_19 = new QLabel(tabEquipment);
        label_19->setObjectName(QString::fromUtf8("label_19"));

        gridLayout->addWidget(label_19, 4, 2, 1, 1);

        charSpell = new QLineEdit(tabEquipment);
        charSpell->setObjectName(QString::fromUtf8("charSpell"));

        gridLayout->addWidget(charSpell, 4, 3, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 5, 0, 1, 1);

        tabWidget->addTab(tabEquipment, QString());
        tabInventory = new QWidget();
        tabInventory->setObjectName(QString::fromUtf8("tabInventory"));
        horizontalLayout_3 = new QHBoxLayout(tabInventory);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        inventoryPlaceholder = new QWidget(tabInventory);
        inventoryPlaceholder->setObjectName(QString::fromUtf8("inventoryPlaceholder"));

        horizontalLayout_3->addWidget(inventoryPlaceholder);

        tabWidget->addTab(tabInventory, QString());
        tabSpellbook = new QWidget();
        tabSpellbook->setObjectName(QString::fromUtf8("tabSpellbook"));
        horizontalLayout_2 = new QHBoxLayout(tabSpellbook);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        spellbookList = new QTreeWidget(tabSpellbook);
        new QTreeWidgetItem(spellbookList);
        new QTreeWidgetItem(spellbookList);
        spellbookList->setObjectName(QString::fromUtf8("spellbookList"));
        spellbookList->setAcceptDrops(true);
        spellbookList->setRootIsDecorated(false);
        spellbookList->setUniformRowHeights(false);
        spellbookList->header()->setVisible(false);

        horizontalLayout_2->addWidget(spellbookList);

        verticalLayout_9 = new QVBoxLayout();
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        addSpellButton = new QPushButton(tabSpellbook);
        addSpellButton->setObjectName(QString::fromUtf8("addSpellButton"));

        verticalLayout_9->addWidget(addSpellButton);

        deleteSpellButton = new QPushButton(tabSpellbook);
        deleteSpellButton->setObjectName(QString::fromUtf8("deleteSpellButton"));

        verticalLayout_9->addWidget(deleteSpellButton);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_9->addItem(verticalSpacer_2);


        horizontalLayout_2->addLayout(verticalLayout_9);

        tabWidget->addTab(tabSpellbook, QString());

        horizontalLayout->addWidget(tabWidget);


        retranslateUi(CharacterData);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(CharacterData);
    } // setupUi

    void retranslateUi(QWidget *CharacterData)
    {
        CharacterData->setWindowTitle(QApplication::translate("CharacterData", "Form", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("CharacterData", "Melee", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("CharacterData", "Ranged", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("CharacterData", "Armor", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("CharacterData", "Shield", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("CharacterData", "Else1", 0, QApplication::UnicodeUTF8));
        label_17->setText(QApplication::translate("CharacterData", "Else2", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("CharacterData", "Else3", 0, QApplication::UnicodeUTF8));
        label_18->setText(QApplication::translate("CharacterData", "Else4", 0, QApplication::UnicodeUTF8));
        label_16->setText(QApplication::translate("CharacterData", "Bag", 0, QApplication::UnicodeUTF8));
        label_19->setText(QApplication::translate("CharacterData", "Spell", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tabEquipment), QApplication::translate("CharacterData", "Equipment", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tabInventory), QApplication::translate("CharacterData", "Inventory", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem = spellbookList->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("CharacterData", "Spell", 0, QApplication::UnicodeUTF8));

        const bool __sortingEnabled = spellbookList->isSortingEnabled();
        spellbookList->setSortingEnabled(false);
        QTreeWidgetItem *___qtreewidgetitem1 = spellbookList->topLevelItem(0);
        ___qtreewidgetitem1->setText(0, QApplication::translate("CharacterData", "sdfsf", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem2 = spellbookList->topLevelItem(1);
        ___qtreewidgetitem2->setText(0, QApplication::translate("CharacterData", "sdff", 0, QApplication::UnicodeUTF8));
        spellbookList->setSortingEnabled(__sortingEnabled);

        addSpellButton->setText(QApplication::translate("CharacterData", "New", 0, QApplication::UnicodeUTF8));
        deleteSpellButton->setText(QApplication::translate("CharacterData", "Delete", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tabSpellbook), QApplication::translate("CharacterData", "Spellbook", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CharacterData: public Ui_CharacterData {};
} // namespace Ui

QT_END_NAMESPACE

#endif // CHARACTERDATA_H
