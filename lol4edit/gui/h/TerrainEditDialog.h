/********************************************************************************
** Form generated from reading UI file 'TerrainEditDialog.ui'
**
** Created: Sat 22. Jan 17:36:42 2011
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef TERRAINEDITDIALOG_H
#define TERRAINEDITDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TerrainEditDialog
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

    void setupUi(QDialog *TerrainEditDialog)
    {
        if (TerrainEditDialog->objectName().isEmpty())
            TerrainEditDialog->setObjectName(QString::fromUtf8("TerrainEditDialog"));
        TerrainEditDialog->resize(299, 407);
        TerrainEditDialog->setSizeGripEnabled(false);
        verticalLayout_2 = new QVBoxLayout(TerrainEditDialog);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        terrainTabWidget = new QTabWidget(TerrainEditDialog);
        terrainTabWidget->setObjectName(QString::fromUtf8("terrainTabWidget"));
        brushTab = new QWidget();
        brushTab->setObjectName(QString::fromUtf8("brushTab"));
        horizontalLayout_3 = new QHBoxLayout(brushTab);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        brushList = new QListWidget(brushTab);
        QIcon icon;
        icon.addFile(QString::fromUtf8("brush.png"), QSize(), QIcon::Normal, QIcon::Off);
        QListWidgetItem *__qlistwidgetitem = new QListWidgetItem(brushList);
        __qlistwidgetitem->setIcon(icon);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8("brush2.png"), QSize(), QIcon::Normal, QIcon::Off);
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
        brushScaleSpin->setMinimum(1);
        brushScaleSpin->setMaximum(16);
        brushScaleSpin->setValue(8);

        verticalLayout->addWidget(brushScaleSpin);

        brushScaleSlider = new QSlider(brushTab);
        brushScaleSlider->setObjectName(QString::fromUtf8("brushScaleSlider"));
        brushScaleSlider->setMinimum(1);
        brushScaleSlider->setMaximum(16);
        brushScaleSlider->setValue(8);
        brushScaleSlider->setSliderPosition(8);
        brushScaleSlider->setOrientation(Qt::Horizontal);

        verticalLayout->addWidget(brushScaleSlider);

        label_2 = new QLabel(brushTab);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(label_2);

        intensSpin = new QSpinBox(brushTab);
        intensSpin->setObjectName(QString::fromUtf8("intensSpin"));
        intensSpin->setMinimum(1);
        intensSpin->setMaximum(101);
        intensSpin->setValue(50);

        verticalLayout->addWidget(intensSpin);

        intensSlider = new QSlider(brushTab);
        intensSlider->setObjectName(QString::fromUtf8("intensSlider"));
        intensSlider->setMinimum(1);
        intensSlider->setMaximum(100);
        intensSlider->setValue(50);
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
        icon2.addFile(QString::fromUtf8("Textures/splatting0.png"), QSize(), QIcon::Normal, QIcon::Off);
        QListWidgetItem *__qlistwidgetitem2 = new QListWidgetItem(texListWidget);
        __qlistwidgetitem2->setIcon(icon2);
        QIcon icon3;
        icon3.addFile(QString::fromUtf8("Textures/splatting1.png"), QSize(), QIcon::Normal, QIcon::Off);
        QListWidgetItem *__qlistwidgetitem3 = new QListWidgetItem(texListWidget);
        __qlistwidgetitem3->setIcon(icon3);
        QIcon icon4;
        icon4.addFile(QString::fromUtf8("Textures/lava.png"), QSize(), QIcon::Normal, QIcon::Off);
        QListWidgetItem *__qlistwidgetitem4 = new QListWidgetItem(texListWidget);
        __qlistwidgetitem4->setIcon(icon4);
        QIcon icon5;
        icon5.addFile(QString::fromUtf8("Textures/big_kiesel_white.01.png"), QSize(), QIcon::Normal, QIcon::Off);
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
        selectMode = new QComboBox(TerrainEditDialog);
        selectMode->setObjectName(QString::fromUtf8("selectMode"));

        horizontalLayout->addWidget(selectMode);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout_2->addLayout(horizontalLayout);


        retranslateUi(TerrainEditDialog);
        QObject::connect(brushScaleSpin, SIGNAL(valueChanged(int)), brushScaleSlider, SLOT(setValue(int)));
        QObject::connect(brushScaleSlider, SIGNAL(valueChanged(int)), brushScaleSpin, SLOT(setValue(int)));
        QObject::connect(intensSpin, SIGNAL(valueChanged(int)), intensSlider, SLOT(setValue(int)));
        QObject::connect(intensSlider, SIGNAL(valueChanged(int)), intensSpin, SLOT(setValue(int)));

        terrainTabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(TerrainEditDialog);
    } // setupUi

    void retranslateUi(QDialog *TerrainEditDialog)
    {
        TerrainEditDialog->setWindowTitle(QApplication::translate("TerrainEditDialog", "Terrain", 0, QApplication::UnicodeUTF8));

        const bool __sortingEnabled = brushList->isSortingEnabled();
        brushList->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem = brushList->item(0);
        ___qlistwidgetitem->setText(QApplication::translate("TerrainEditDialog", "test", 0, QApplication::UnicodeUTF8));
        QListWidgetItem *___qlistwidgetitem1 = brushList->item(1);
        ___qlistwidgetitem1->setText(QApplication::translate("TerrainEditDialog", "muh", 0, QApplication::UnicodeUTF8));
        brushList->setSortingEnabled(__sortingEnabled);

        label->setText(QApplication::translate("TerrainEditDialog", "Brush Scale", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("TerrainEditDialog", "Intensity", 0, QApplication::UnicodeUTF8));
        terrainTabWidget->setTabText(terrainTabWidget->indexOf(brushTab), QApplication::translate("TerrainEditDialog", "Brushes", 0, QApplication::UnicodeUTF8));

        const bool __sortingEnabled1 = texListWidget->isSortingEnabled();
        texListWidget->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem2 = texListWidget->item(0);
        ___qlistwidgetitem2->setText(QApplication::translate("TerrainEditDialog", "splatting0.png", 0, QApplication::UnicodeUTF8));
        QListWidgetItem *___qlistwidgetitem3 = texListWidget->item(1);
        ___qlistwidgetitem3->setText(QApplication::translate("TerrainEditDialog", "splatting1.png", 0, QApplication::UnicodeUTF8));
        QListWidgetItem *___qlistwidgetitem4 = texListWidget->item(2);
        ___qlistwidgetitem4->setText(QApplication::translate("TerrainEditDialog", "lava.png", 0, QApplication::UnicodeUTF8));
        QListWidgetItem *___qlistwidgetitem5 = texListWidget->item(3);
        ___qlistwidgetitem5->setText(QApplication::translate("TerrainEditDialog", "big_kiesel_white.01.png", 0, QApplication::UnicodeUTF8));
        texListWidget->setSortingEnabled(__sortingEnabled1);

        EditTexlist->setText(QApplication::translate("TerrainEditDialog", "Textures...", 0, QApplication::UnicodeUTF8));
        terrainTabWidget->setTabText(terrainTabWidget->indexOf(texTab), QApplication::translate("TerrainEditDialog", "Textures", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("TerrainEditDialog", "MinExtends", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("TerrainEditDialog", "MaxExtends", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("TerrainEditDialog", "Light Dir", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("TerrainEditDialog", "Light Color", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("TerrainEditDialog", "Ambient Color", 0, QApplication::UnicodeUTF8));
        terrainTabWidget->setTabText(terrainTabWidget->indexOf(Seite), QApplication::translate("TerrainEditDialog", "Settings", 0, QApplication::UnicodeUTF8));
        selectMode->clear();
        selectMode->insertItems(0, QStringList()
         << QApplication::translate("TerrainEditDialog", "Deform", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("TerrainEditDialog", "Flatten", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("TerrainEditDialog", "Soften", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("TerrainEditDialog", "Paint textures", 0, QApplication::UnicodeUTF8)
        );
    } // retranslateUi

};

namespace Ui {
    class TerrainEditDialog: public Ui_TerrainEditDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // TERRAINEDITDIALOG_H
