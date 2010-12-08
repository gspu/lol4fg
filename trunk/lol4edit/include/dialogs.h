/********************************************************************************
** Form generated from reading ui file 'objectselectgq2952.ui'
**
** Created: Sun 19. Jul 18:27:45 2009
**      by: Qt User Interface Compiler version 4.5.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef OBJECTSELECTGQ2952_H
#define OBJECTSELECTGQ2952_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QListView>
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
    QListView *staticObjectList;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_3;
    QListView *dynamicObjectList;
    QWidget *Seite;
    QHBoxLayout *horizontalLayout;
    QListView *dotsceneObjectList;
    QWidget *Seite_2;
    QHBoxLayout *horizontalLayout_2;
    QListView *itemObjectList;
    QWidget *Seite_3;
    QHBoxLayout *horizontalLayout_3;
    QListView *charObjectList;
    QWidget *Seite_4;
    QHBoxLayout *horizontalLayout_4;
    QListView *doorObjectList;
    QWidget *Seite_5;
    QHBoxLayout *horizontalLayout_5;
    QListView *contObjectList;
    QWidget *Seite_6;
    QHBoxLayout *horizontalLayout_6;
    QListView *elseObjectList;

    void setupUi(QDialog *ObjectSelectDialog)
    {
        if (ObjectSelectDialog->objectName().isEmpty())
            ObjectSelectDialog->setObjectName(QString::fromUtf8("ObjectSelectDialog"));
        ObjectSelectDialog->resize(302, 361);
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
        staticObjectList = new QListView(tab);
        staticObjectList->setObjectName(QString::fromUtf8("staticObjectList"));

        verticalLayout->addWidget(staticObjectList);

        mainTabCtrl->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        verticalLayout_3 = new QVBoxLayout(tab_2);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        dynamicObjectList = new QListView(tab_2);
        dynamicObjectList->setObjectName(QString::fromUtf8("dynamicObjectList"));

        verticalLayout_3->addWidget(dynamicObjectList);

        mainTabCtrl->addTab(tab_2, QString());
        Seite = new QWidget();
        Seite->setObjectName(QString::fromUtf8("Seite"));
        horizontalLayout = new QHBoxLayout(Seite);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        dotsceneObjectList = new QListView(Seite);
        dotsceneObjectList->setObjectName(QString::fromUtf8("dotsceneObjectList"));

        horizontalLayout->addWidget(dotsceneObjectList);

        mainTabCtrl->addTab(Seite, QString());
        Seite_2 = new QWidget();
        Seite_2->setObjectName(QString::fromUtf8("Seite_2"));
        horizontalLayout_2 = new QHBoxLayout(Seite_2);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        itemObjectList = new QListView(Seite_2);
        itemObjectList->setObjectName(QString::fromUtf8("itemObjectList"));

        horizontalLayout_2->addWidget(itemObjectList);

        mainTabCtrl->addTab(Seite_2, QString());
        Seite_3 = new QWidget();
        Seite_3->setObjectName(QString::fromUtf8("Seite_3"));
        horizontalLayout_3 = new QHBoxLayout(Seite_3);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        charObjectList = new QListView(Seite_3);
        charObjectList->setObjectName(QString::fromUtf8("charObjectList"));

        horizontalLayout_3->addWidget(charObjectList);

        mainTabCtrl->addTab(Seite_3, QString());
        Seite_4 = new QWidget();
        Seite_4->setObjectName(QString::fromUtf8("Seite_4"));
        horizontalLayout_4 = new QHBoxLayout(Seite_4);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        doorObjectList = new QListView(Seite_4);
        doorObjectList->setObjectName(QString::fromUtf8("doorObjectList"));

        horizontalLayout_4->addWidget(doorObjectList);

        mainTabCtrl->addTab(Seite_4, QString());
        Seite_5 = new QWidget();
        Seite_5->setObjectName(QString::fromUtf8("Seite_5"));
        horizontalLayout_5 = new QHBoxLayout(Seite_5);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        contObjectList = new QListView(Seite_5);
        contObjectList->setObjectName(QString::fromUtf8("contObjectList"));

        horizontalLayout_5->addWidget(contObjectList);

        mainTabCtrl->addTab(Seite_5, QString());
        Seite_6 = new QWidget();
        Seite_6->setObjectName(QString::fromUtf8("Seite_6"));
        horizontalLayout_6 = new QHBoxLayout(Seite_6);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        elseObjectList = new QListView(Seite_6);
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
        Q_UNUSED(ObjectSelectDialog);
    } // retranslateUi

};

namespace Ui {
    class ObjectSelectDialog: public Ui_ObjectSelectDialog {};
} // namespace Ui


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
        Q_UNUSED(ObjectListDialog);
    } // retranslateUi

};

namespace Ui {
    class ObjectListDialog: public Ui_ObjectListDialog {};
} // namespace Ui

class Ui_TerrainDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QTabWidget *terrainTabWidget;
    QWidget *brushTab;
    QHBoxLayout *horizontalLayout_3;
    QListWidget *brushList;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QSpinBox *brushScaleSpin;
    QSlider *brushScaleSlider;
    QLabel *label_2;
    QSpinBox *intensSpin;
    QSlider *intensSlider;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer;
    QWidget *texTab;
    QVBoxLayout *verticalLayout_4;
    QListWidget *texListWidget;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *EditTexlist;
    QWidget *Seite;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_4;
    QLineEdit *minExtendsEdit;
    QLabel *label_3;
    QLineEdit *maxExtendsEdit;
    QLabel *label_7;
    QLineEdit *lightDirEdit;
    QLabel *label_6;
    QLineEdit *lightColEdit;
    QLabel *label_5;
    QLineEdit *ambientColEdit;
    QHBoxLayout *horizontalLayout;
    QComboBox *selectMode;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->resize(259, 345);
        Dialog->setSizeGripEnabled(false);
        verticalLayout_2 = new QVBoxLayout(Dialog);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        terrainTabWidget = new QTabWidget(Dialog);
        terrainTabWidget->setObjectName(QString::fromUtf8("terrainTabWidget"));
        brushTab = new QWidget();
        brushTab->setObjectName(QString::fromUtf8("brushTab"));
        horizontalLayout_3 = new QHBoxLayout(brushTab);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        brushList = new QListWidget(brushTab);
        QIcon icon;
        icon.addPixmap(QPixmap(QString::fromUtf8("brush.png")), QIcon::Normal, QIcon::Off);
        QListWidgetItem *__qlistwidgetitem = new QListWidgetItem(brushList);
        __qlistwidgetitem->setIcon(icon);
        QIcon icon1;
        icon1.addPixmap(QPixmap(QString::fromUtf8("brush2.png")), QIcon::Normal, QIcon::Off);
        QListWidgetItem *__qlistwidgetitem1 = new QListWidgetItem(brushList);
        __qlistwidgetitem1->setIcon(icon1);
        brushList->setObjectName(QString::fromUtf8("brushList"));
        brushList->setIconSize(QSize(64, 64));

        horizontalLayout_3->addWidget(brushList);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(brushTab);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(label);

        brushScaleSpin = new QSpinBox(brushTab);
        brushScaleSpin->setObjectName(QString::fromUtf8("brushScaleSpin"));

        verticalLayout->addWidget(brushScaleSpin);

        brushScaleSlider = new QSlider(brushTab);
        brushScaleSlider->setObjectName(QString::fromUtf8("brushScaleSlider"));
        brushScaleSlider->setOrientation(Qt::Horizontal);

        verticalLayout->addWidget(brushScaleSlider);

        label_2 = new QLabel(brushTab);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(label_2);

        intensSpin = new QSpinBox(brushTab);
        intensSpin->setObjectName(QString::fromUtf8("intensSpin"));

        verticalLayout->addWidget(intensSpin);

        intensSlider = new QSlider(brushTab);
        intensSlider->setObjectName(QString::fromUtf8("intensSlider"));
        intensSlider->setOrientation(Qt::Horizontal);

        verticalLayout->addWidget(intensSlider);

        horizontalSpacer = new QSpacerItem(87, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        verticalLayout->addItem(horizontalSpacer);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout_3->addLayout(verticalLayout);

        terrainTabWidget->addTab(brushTab, QString());
        texTab = new QWidget();
        texTab->setObjectName(QString::fromUtf8("texTab"));
        verticalLayout_4 = new QVBoxLayout(texTab);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        texListWidget = new QListWidget(texTab);
        QIcon icon2;
        icon2.addPixmap(QPixmap(QString::fromUtf8("Textures/splatting0.png")), QIcon::Normal, QIcon::Off);
        QListWidgetItem *__qlistwidgetitem2 = new QListWidgetItem(texListWidget);
        __qlistwidgetitem2->setIcon(icon2);
        QIcon icon3;
        icon3.addPixmap(QPixmap(QString::fromUtf8("Textures/splatting1.png")), QIcon::Normal, QIcon::Off);
        QListWidgetItem *__qlistwidgetitem3 = new QListWidgetItem(texListWidget);
        __qlistwidgetitem3->setIcon(icon3);
        QIcon icon4;
        icon4.addPixmap(QPixmap(QString::fromUtf8("Textures/lava.png")), QIcon::Normal, QIcon::Off);
        QListWidgetItem *__qlistwidgetitem4 = new QListWidgetItem(texListWidget);
        __qlistwidgetitem4->setIcon(icon4);
        QIcon icon5;
        icon5.addPixmap(QPixmap(QString::fromUtf8("Textures/big_kiesel_white.01.png")), QIcon::Normal, QIcon::Off);
        QListWidgetItem *__qlistwidgetitem5 = new QListWidgetItem(texListWidget);
        __qlistwidgetitem5->setIcon(icon5);
        texListWidget->setObjectName(QString::fromUtf8("texListWidget"));
        texListWidget->setIconSize(QSize(64, 64));

        verticalLayout_4->addWidget(texListWidget);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);

        EditTexlist = new QPushButton(texTab);
        EditTexlist->setObjectName(QString::fromUtf8("EditTexlist"));

        horizontalLayout_2->addWidget(EditTexlist);


        verticalLayout_4->addLayout(horizontalLayout_2);

        terrainTabWidget->addTab(texTab, QString());
        Seite = new QWidget();
        Seite->setObjectName(QString::fromUtf8("Seite"));
        verticalLayout_3 = new QVBoxLayout(Seite);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        label_4 = new QLabel(Seite);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy1);

        verticalLayout_3->addWidget(label_4);

        minExtendsEdit = new QLineEdit(Seite);
        minExtendsEdit->setObjectName(QString::fromUtf8("minExtendsEdit"));

        verticalLayout_3->addWidget(minExtendsEdit);

        label_3 = new QLabel(Seite);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        sizePolicy1.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy1);

        verticalLayout_3->addWidget(label_3);

        maxExtendsEdit = new QLineEdit(Seite);
        maxExtendsEdit->setObjectName(QString::fromUtf8("maxExtendsEdit"));

        verticalLayout_3->addWidget(maxExtendsEdit);

        label_7 = new QLabel(Seite);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        sizePolicy1.setHeightForWidth(label_7->sizePolicy().hasHeightForWidth());
        label_7->setSizePolicy(sizePolicy1);

        verticalLayout_3->addWidget(label_7);

        lightDirEdit = new QLineEdit(Seite);
        lightDirEdit->setObjectName(QString::fromUtf8("lightDirEdit"));

        verticalLayout_3->addWidget(lightDirEdit);

        label_6 = new QLabel(Seite);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        sizePolicy1.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
        label_6->setSizePolicy(sizePolicy1);

        verticalLayout_3->addWidget(label_6);

        lightColEdit = new QLineEdit(Seite);
        lightColEdit->setObjectName(QString::fromUtf8("lightColEdit"));

        verticalLayout_3->addWidget(lightColEdit);

        label_5 = new QLabel(Seite);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        sizePolicy1.setHeightForWidth(label_5->sizePolicy().hasHeightForWidth());
        label_5->setSizePolicy(sizePolicy1);

        verticalLayout_3->addWidget(label_5);

        ambientColEdit = new QLineEdit(Seite);
        ambientColEdit->setObjectName(QString::fromUtf8("ambientColEdit"));

        verticalLayout_3->addWidget(ambientColEdit);

        terrainTabWidget->addTab(Seite, QString());

        verticalLayout_2->addWidget(terrainTabWidget);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        selectMode = new QComboBox(Dialog);
        selectMode->setObjectName(QString::fromUtf8("selectMode"));

        horizontalLayout->addWidget(selectMode);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout_2->addLayout(horizontalLayout);


        retranslateUi(Dialog);
        QObject::connect(brushScaleSpin, SIGNAL(valueChanged(int)), brushScaleSlider, SLOT(setValue(int)));
        QObject::connect(brushScaleSlider, SIGNAL(valueChanged(int)), brushScaleSpin, SLOT(setValue(int)));
        QObject::connect(intensSpin, SIGNAL(valueChanged(int)), intensSlider, SLOT(setValue(int)));
        QObject::connect(intensSlider, SIGNAL(valueChanged(int)), intensSpin, SLOT(setValue(int)));

        terrainTabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "Terrain", 0, QApplication::UnicodeUTF8));

        const bool __sortingEnabled = brushList->isSortingEnabled();
        brushList->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem = brushList->item(0);
        ___qlistwidgetitem->setText(QApplication::translate("Dialog", "test", 0, QApplication::UnicodeUTF8));
        QListWidgetItem *___qlistwidgetitem1 = brushList->item(1);
        ___qlistwidgetitem1->setText(QApplication::translate("Dialog", "muh", 0, QApplication::UnicodeUTF8));
        brushList->setSortingEnabled(__sortingEnabled);

        label->setText(QApplication::translate("Dialog", "Brush Scale", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("Dialog", "Intensity", 0, QApplication::UnicodeUTF8));
        terrainTabWidget->setTabText(terrainTabWidget->indexOf(brushTab), QApplication::translate("Dialog", "Brushes", 0, QApplication::UnicodeUTF8));

        const bool __sortingEnabled1 = texListWidget->isSortingEnabled();
        texListWidget->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem2 = texListWidget->item(0);
        ___qlistwidgetitem2->setText(QApplication::translate("Dialog", "splatting0.png", 0, QApplication::UnicodeUTF8));
        QListWidgetItem *___qlistwidgetitem3 = texListWidget->item(1);
        ___qlistwidgetitem3->setText(QApplication::translate("Dialog", "splatting1.png", 0, QApplication::UnicodeUTF8));
        QListWidgetItem *___qlistwidgetitem4 = texListWidget->item(2);
        ___qlistwidgetitem4->setText(QApplication::translate("Dialog", "lava.png", 0, QApplication::UnicodeUTF8));
        QListWidgetItem *___qlistwidgetitem5 = texListWidget->item(3);
        ___qlistwidgetitem5->setText(QApplication::translate("Dialog", "big_kiesel_white.01.png", 0, QApplication::UnicodeUTF8));
        texListWidget->setSortingEnabled(__sortingEnabled1);

        EditTexlist->setText(QApplication::translate("Dialog", "Textures...", 0, QApplication::UnicodeUTF8));
        terrainTabWidget->setTabText(terrainTabWidget->indexOf(texTab), QApplication::translate("Dialog", "Textures", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("Dialog", "MinExtends", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("Dialog", "MaxExtends", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("Dialog", "Ogre::Light Dir", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("Dialog", "Ogre::Light Color", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("Dialog", "Ambient Color", 0, QApplication::UnicodeUTF8));
        terrainTabWidget->setTabText(terrainTabWidget->indexOf(Seite), QApplication::translate("Dialog", "Settings", 0, QApplication::UnicodeUTF8));
        selectMode->clear();
        selectMode->insertItems(0, QStringList()
         << QApplication::translate("Dialog", "Deform", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Dialog", "Paint textures", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Dialog", "Flatten", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Dialog", "Soften", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Dialog", "Make Holes", 0, QApplication::UnicodeUTF8)
        );
        Q_UNUSED(Dialog);
    } // retranslateUi

};

namespace Ui {
    class TerrainDialog: public Ui_TerrainDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // OBJECTSELECTGQ2952_H