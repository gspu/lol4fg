/********************************************************************************
** Form generated from reading UI file 'WaterData.ui'
**
** Created: Wed 26. Jan 20:40:28 2011
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef WATERDATA_H
#define WATERDATA_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QSpacerItem>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WaterData
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_55;
    QLineEdit *waterMaterial;
    QToolButton *waterMaterialSelect;
    QSpacerItem *horizontalSpacer_6;
    QGroupBox *groupBox_4;
    QGridLayout *gridLayout_10;
    QLabel *label_51;
    QLineEdit *waterDensity;
    QLabel *label_53;
    QLineEdit *waterSound;
    QLabel *label_52;
    QLabel *label_54;
    QLineEdit *waterLinVisc;
    QLineEdit *waterAngVisc;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_8;
    QLineEdit *waterPlaneWidth;
    QLabel *label_43;
    QLabel *label_44;
    QLineEdit *waterPlaneHeight;
    QLabel *label_45;
    QLabel *label_46;
    QLabel *label_47;
    QLineEdit *waterPlaneXSeg;
    QLineEdit *waterPlaneUTile;
    QLineEdit *waterPlaneUpVector;
    QLabel *label_48;
    QLabel *label_49;
    QLabel *label_50;
    QLineEdit *waterPlaneYSeg;
    QLineEdit *waterPlaneVTile;
    QLineEdit *waterPlaneTexCoords;

    void setupUi(QWidget *WaterData)
    {
        if (WaterData->objectName().isEmpty())
            WaterData->setObjectName(QString::fromUtf8("WaterData"));
        WaterData->resize(400, 300);
        verticalLayout = new QVBoxLayout(WaterData);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_55 = new QLabel(WaterData);
        label_55->setObjectName(QString::fromUtf8("label_55"));

        horizontalLayout_7->addWidget(label_55);

        waterMaterial = new QLineEdit(WaterData);
        waterMaterial->setObjectName(QString::fromUtf8("waterMaterial"));

        horizontalLayout_7->addWidget(waterMaterial);

        waterMaterialSelect = new QToolButton(WaterData);
        waterMaterialSelect->setObjectName(QString::fromUtf8("waterMaterialSelect"));

        horizontalLayout_7->addWidget(waterMaterialSelect);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_6);


        verticalLayout->addLayout(horizontalLayout_7);

        groupBox_4 = new QGroupBox(WaterData);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        gridLayout_10 = new QGridLayout(groupBox_4);
        gridLayout_10->setObjectName(QString::fromUtf8("gridLayout_10"));
        label_51 = new QLabel(groupBox_4);
        label_51->setObjectName(QString::fromUtf8("label_51"));

        gridLayout_10->addWidget(label_51, 0, 0, 1, 1);

        waterDensity = new QLineEdit(groupBox_4);
        waterDensity->setObjectName(QString::fromUtf8("waterDensity"));

        gridLayout_10->addWidget(waterDensity, 0, 1, 1, 1);

        label_53 = new QLabel(groupBox_4);
        label_53->setObjectName(QString::fromUtf8("label_53"));

        gridLayout_10->addWidget(label_53, 0, 2, 1, 1);

        waterSound = new QLineEdit(groupBox_4);
        waterSound->setObjectName(QString::fromUtf8("waterSound"));

        gridLayout_10->addWidget(waterSound, 0, 3, 1, 1);

        label_52 = new QLabel(groupBox_4);
        label_52->setObjectName(QString::fromUtf8("label_52"));

        gridLayout_10->addWidget(label_52, 1, 0, 1, 1);

        label_54 = new QLabel(groupBox_4);
        label_54->setObjectName(QString::fromUtf8("label_54"));

        gridLayout_10->addWidget(label_54, 1, 2, 1, 1);

        waterLinVisc = new QLineEdit(groupBox_4);
        waterLinVisc->setObjectName(QString::fromUtf8("waterLinVisc"));

        gridLayout_10->addWidget(waterLinVisc, 1, 1, 1, 1);

        waterAngVisc = new QLineEdit(groupBox_4);
        waterAngVisc->setObjectName(QString::fromUtf8("waterAngVisc"));

        gridLayout_10->addWidget(waterAngVisc, 1, 3, 1, 1);


        verticalLayout->addWidget(groupBox_4);

        groupBox_3 = new QGroupBox(WaterData);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        gridLayout_8 = new QGridLayout(groupBox_3);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        waterPlaneWidth = new QLineEdit(groupBox_3);
        waterPlaneWidth->setObjectName(QString::fromUtf8("waterPlaneWidth"));

        gridLayout_8->addWidget(waterPlaneWidth, 0, 1, 1, 1);

        label_43 = new QLabel(groupBox_3);
        label_43->setObjectName(QString::fromUtf8("label_43"));

        gridLayout_8->addWidget(label_43, 0, 0, 1, 1);

        label_44 = new QLabel(groupBox_3);
        label_44->setObjectName(QString::fromUtf8("label_44"));

        gridLayout_8->addWidget(label_44, 0, 2, 1, 1);

        waterPlaneHeight = new QLineEdit(groupBox_3);
        waterPlaneHeight->setObjectName(QString::fromUtf8("waterPlaneHeight"));

        gridLayout_8->addWidget(waterPlaneHeight, 0, 3, 1, 1);

        label_45 = new QLabel(groupBox_3);
        label_45->setObjectName(QString::fromUtf8("label_45"));

        gridLayout_8->addWidget(label_45, 1, 0, 1, 1);

        label_46 = new QLabel(groupBox_3);
        label_46->setObjectName(QString::fromUtf8("label_46"));

        gridLayout_8->addWidget(label_46, 2, 0, 1, 1);

        label_47 = new QLabel(groupBox_3);
        label_47->setObjectName(QString::fromUtf8("label_47"));

        gridLayout_8->addWidget(label_47, 3, 0, 1, 1);

        waterPlaneXSeg = new QLineEdit(groupBox_3);
        waterPlaneXSeg->setObjectName(QString::fromUtf8("waterPlaneXSeg"));

        gridLayout_8->addWidget(waterPlaneXSeg, 1, 1, 1, 1);

        waterPlaneUTile = new QLineEdit(groupBox_3);
        waterPlaneUTile->setObjectName(QString::fromUtf8("waterPlaneUTile"));

        gridLayout_8->addWidget(waterPlaneUTile, 2, 1, 1, 1);

        waterPlaneUpVector = new QLineEdit(groupBox_3);
        waterPlaneUpVector->setObjectName(QString::fromUtf8("waterPlaneUpVector"));

        gridLayout_8->addWidget(waterPlaneUpVector, 3, 1, 1, 1);

        label_48 = new QLabel(groupBox_3);
        label_48->setObjectName(QString::fromUtf8("label_48"));

        gridLayout_8->addWidget(label_48, 1, 2, 1, 1);

        label_49 = new QLabel(groupBox_3);
        label_49->setObjectName(QString::fromUtf8("label_49"));

        gridLayout_8->addWidget(label_49, 2, 2, 1, 1);

        label_50 = new QLabel(groupBox_3);
        label_50->setObjectName(QString::fromUtf8("label_50"));

        gridLayout_8->addWidget(label_50, 3, 2, 1, 1);

        waterPlaneYSeg = new QLineEdit(groupBox_3);
        waterPlaneYSeg->setObjectName(QString::fromUtf8("waterPlaneYSeg"));

        gridLayout_8->addWidget(waterPlaneYSeg, 1, 3, 1, 1);

        waterPlaneVTile = new QLineEdit(groupBox_3);
        waterPlaneVTile->setObjectName(QString::fromUtf8("waterPlaneVTile"));

        gridLayout_8->addWidget(waterPlaneVTile, 2, 3, 1, 1);

        waterPlaneTexCoords = new QLineEdit(groupBox_3);
        waterPlaneTexCoords->setObjectName(QString::fromUtf8("waterPlaneTexCoords"));

        gridLayout_8->addWidget(waterPlaneTexCoords, 3, 3, 1, 1);


        verticalLayout->addWidget(groupBox_3);


        retranslateUi(WaterData);

        QMetaObject::connectSlotsByName(WaterData);
    } // setupUi

    void retranslateUi(QWidget *WaterData)
    {
        WaterData->setWindowTitle(QApplication::translate("WaterData", "Form", 0, QApplication::UnicodeUTF8));
        label_55->setText(QApplication::translate("WaterData", "Material", 0, QApplication::UnicodeUTF8));
        waterMaterialSelect->setText(QApplication::translate("WaterData", "...", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QApplication::translate("WaterData", "Fluid", 0, QApplication::UnicodeUTF8));
        label_51->setText(QApplication::translate("WaterData", "Density", 0, QApplication::UnicodeUTF8));
        label_53->setText(QApplication::translate("WaterData", "Sound", 0, QApplication::UnicodeUTF8));
        label_52->setText(QApplication::translate("WaterData", "Lin. Viscosity", 0, QApplication::UnicodeUTF8));
        label_54->setText(QApplication::translate("WaterData", "Ang. Viscosity", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("WaterData", "Plane", 0, QApplication::UnicodeUTF8));
        label_43->setText(QApplication::translate("WaterData", "Width", 0, QApplication::UnicodeUTF8));
        label_44->setText(QApplication::translate("WaterData", "Height", 0, QApplication::UnicodeUTF8));
        label_45->setText(QApplication::translate("WaterData", "xSeg", 0, QApplication::UnicodeUTF8));
        label_46->setText(QApplication::translate("WaterData", "uTile", 0, QApplication::UnicodeUTF8));
        label_47->setText(QApplication::translate("WaterData", "upVector", 0, QApplication::UnicodeUTF8));
        label_48->setText(QApplication::translate("WaterData", "ySeg", 0, QApplication::UnicodeUTF8));
        label_49->setText(QApplication::translate("WaterData", "vTile", 0, QApplication::UnicodeUTF8));
        label_50->setText(QApplication::translate("WaterData", "TexCoords", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class WaterData: public Ui_WaterData {};
} // namespace Ui

QT_END_NAMESPACE

#endif // WATERDATA_H
