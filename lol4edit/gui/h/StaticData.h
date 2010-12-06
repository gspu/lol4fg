/********************************************************************************
** Form generated from reading UI file 'StaticData.ui'
**
** Created: Tue 7. Sep 16:44:54 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef STATICDATA_H
#define STATICDATA_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QSpacerItem>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_StaticData
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout_4;
    QLineEdit *staticMeshName;
    QLabel *label_20;
    QSpacerItem *horizontalSpacer_3;
    QToolButton *staticMeshSelect;
    QLabel *label_21;
    QLineEdit *staticMatName;
    QToolButton *staticMatSelect;
    QCheckBox *staticIsStatic;
    QSpacerItem *horizontalSpacer_4;
    QCheckBox *staticHasShadows;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_5;
    QLabel *label_22;
    QLineEdit *planeNormal;
    QLabel *label_23;
    QLineEdit *planeConstant;
    QLabel *label_24;
    QLabel *label_25;
    QLabel *label_26;
    QLabel *label_27;
    QLineEdit *planeWidth;
    QLineEdit *planeHeight;
    QLineEdit *planeXSeg;
    QLineEdit *planeYSeg;
    QLineEdit *planeUTile;
    QLineEdit *planeVTile;
    QLineEdit *planeUpVector;
    QLineEdit *planeTexCoords;
    QLabel *label_28;
    QLabel *label_29;
    QLabel *label_30;
    QLabel *label_31;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *StaticData)
    {
        if (StaticData->objectName().isEmpty())
            StaticData->setObjectName(QString::fromUtf8("StaticData"));
        StaticData->resize(400, 301);
        verticalLayout = new QVBoxLayout(StaticData);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        gridLayout_4 = new QGridLayout();
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        staticMeshName = new QLineEdit(StaticData);
        staticMeshName->setObjectName(QString::fromUtf8("staticMeshName"));

        gridLayout_4->addWidget(staticMeshName, 0, 1, 1, 1);

        label_20 = new QLabel(StaticData);
        label_20->setObjectName(QString::fromUtf8("label_20"));

        gridLayout_4->addWidget(label_20, 0, 0, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_3, 0, 4, 1, 1);

        staticMeshSelect = new QToolButton(StaticData);
        staticMeshSelect->setObjectName(QString::fromUtf8("staticMeshSelect"));

        gridLayout_4->addWidget(staticMeshSelect, 0, 2, 1, 1);

        label_21 = new QLabel(StaticData);
        label_21->setObjectName(QString::fromUtf8("label_21"));

        gridLayout_4->addWidget(label_21, 1, 0, 1, 1);

        staticMatName = new QLineEdit(StaticData);
        staticMatName->setObjectName(QString::fromUtf8("staticMatName"));

        gridLayout_4->addWidget(staticMatName, 1, 1, 1, 1);

        staticMatSelect = new QToolButton(StaticData);
        staticMatSelect->setObjectName(QString::fromUtf8("staticMatSelect"));

        gridLayout_4->addWidget(staticMatSelect, 1, 2, 1, 1);

        staticIsStatic = new QCheckBox(StaticData);
        staticIsStatic->setObjectName(QString::fromUtf8("staticIsStatic"));

        gridLayout_4->addWidget(staticIsStatic, 0, 5, 1, 1);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_4, 1, 4, 1, 1);

        staticHasShadows = new QCheckBox(StaticData);
        staticHasShadows->setObjectName(QString::fromUtf8("staticHasShadows"));

        gridLayout_4->addWidget(staticHasShadows, 1, 5, 1, 1);


        verticalLayout->addLayout(gridLayout_4);

        groupBox = new QGroupBox(StaticData);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        gridLayout_5 = new QGridLayout(groupBox);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        label_22 = new QLabel(groupBox);
        label_22->setObjectName(QString::fromUtf8("label_22"));

        gridLayout_5->addWidget(label_22, 0, 0, 1, 1);

        planeNormal = new QLineEdit(groupBox);
        planeNormal->setObjectName(QString::fromUtf8("planeNormal"));

        gridLayout_5->addWidget(planeNormal, 0, 1, 1, 1);

        label_23 = new QLabel(groupBox);
        label_23->setObjectName(QString::fromUtf8("label_23"));

        gridLayout_5->addWidget(label_23, 0, 2, 1, 1);

        planeConstant = new QLineEdit(groupBox);
        planeConstant->setObjectName(QString::fromUtf8("planeConstant"));

        gridLayout_5->addWidget(planeConstant, 0, 3, 1, 1);

        label_24 = new QLabel(groupBox);
        label_24->setObjectName(QString::fromUtf8("label_24"));

        gridLayout_5->addWidget(label_24, 1, 0, 1, 1);

        label_25 = new QLabel(groupBox);
        label_25->setObjectName(QString::fromUtf8("label_25"));

        gridLayout_5->addWidget(label_25, 2, 0, 1, 1);

        label_26 = new QLabel(groupBox);
        label_26->setObjectName(QString::fromUtf8("label_26"));

        gridLayout_5->addWidget(label_26, 3, 0, 1, 1);

        label_27 = new QLabel(groupBox);
        label_27->setObjectName(QString::fromUtf8("label_27"));

        gridLayout_5->addWidget(label_27, 4, 0, 1, 1);

        planeWidth = new QLineEdit(groupBox);
        planeWidth->setObjectName(QString::fromUtf8("planeWidth"));

        gridLayout_5->addWidget(planeWidth, 1, 1, 1, 1);

        planeHeight = new QLineEdit(groupBox);
        planeHeight->setObjectName(QString::fromUtf8("planeHeight"));

        gridLayout_5->addWidget(planeHeight, 1, 3, 1, 1);

        planeXSeg = new QLineEdit(groupBox);
        planeXSeg->setObjectName(QString::fromUtf8("planeXSeg"));

        gridLayout_5->addWidget(planeXSeg, 2, 1, 1, 1);

        planeYSeg = new QLineEdit(groupBox);
        planeYSeg->setObjectName(QString::fromUtf8("planeYSeg"));

        gridLayout_5->addWidget(planeYSeg, 2, 3, 1, 1);

        planeUTile = new QLineEdit(groupBox);
        planeUTile->setObjectName(QString::fromUtf8("planeUTile"));

        gridLayout_5->addWidget(planeUTile, 3, 1, 1, 1);

        planeVTile = new QLineEdit(groupBox);
        planeVTile->setObjectName(QString::fromUtf8("planeVTile"));

        gridLayout_5->addWidget(planeVTile, 3, 3, 1, 1);

        planeUpVector = new QLineEdit(groupBox);
        planeUpVector->setObjectName(QString::fromUtf8("planeUpVector"));

        gridLayout_5->addWidget(planeUpVector, 4, 1, 1, 1);

        planeTexCoords = new QLineEdit(groupBox);
        planeTexCoords->setObjectName(QString::fromUtf8("planeTexCoords"));

        gridLayout_5->addWidget(planeTexCoords, 4, 3, 1, 1);

        label_28 = new QLabel(groupBox);
        label_28->setObjectName(QString::fromUtf8("label_28"));

        gridLayout_5->addWidget(label_28, 4, 2, 1, 1);

        label_29 = new QLabel(groupBox);
        label_29->setObjectName(QString::fromUtf8("label_29"));

        gridLayout_5->addWidget(label_29, 3, 2, 1, 1);

        label_30 = new QLabel(groupBox);
        label_30->setObjectName(QString::fromUtf8("label_30"));

        gridLayout_5->addWidget(label_30, 2, 2, 1, 1);

        label_31 = new QLabel(groupBox);
        label_31->setObjectName(QString::fromUtf8("label_31"));

        gridLayout_5->addWidget(label_31, 1, 2, 1, 1);


        verticalLayout->addWidget(groupBox);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(StaticData);

        QMetaObject::connectSlotsByName(StaticData);
    } // setupUi

    void retranslateUi(QWidget *StaticData)
    {
        StaticData->setWindowTitle(QApplication::translate("StaticData", "Form", 0, QApplication::UnicodeUTF8));
        label_20->setText(QApplication::translate("StaticData", "Mesh/Name", 0, QApplication::UnicodeUTF8));
        staticMeshSelect->setText(QApplication::translate("StaticData", "...", 0, QApplication::UnicodeUTF8));
        label_21->setText(QApplication::translate("StaticData", "Material", 0, QApplication::UnicodeUTF8));
        staticMatSelect->setText(QApplication::translate("StaticData", "...", 0, QApplication::UnicodeUTF8));
        staticIsStatic->setText(QApplication::translate("StaticData", "Static", 0, QApplication::UnicodeUTF8));
        staticHasShadows->setText(QApplication::translate("StaticData", "Shadows", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("StaticData", "Plane", 0, QApplication::UnicodeUTF8));
        label_22->setText(QApplication::translate("StaticData", "Normal", 0, QApplication::UnicodeUTF8));
        label_23->setText(QApplication::translate("StaticData", "Constant", 0, QApplication::UnicodeUTF8));
        label_24->setText(QApplication::translate("StaticData", "Width", 0, QApplication::UnicodeUTF8));
        label_25->setText(QApplication::translate("StaticData", "xSeg", 0, QApplication::UnicodeUTF8));
        label_26->setText(QApplication::translate("StaticData", "uTile", 0, QApplication::UnicodeUTF8));
        label_27->setText(QApplication::translate("StaticData", "upVector", 0, QApplication::UnicodeUTF8));
        label_28->setText(QApplication::translate("StaticData", "TexCoords", 0, QApplication::UnicodeUTF8));
        label_29->setText(QApplication::translate("StaticData", "vTile", 0, QApplication::UnicodeUTF8));
        label_30->setText(QApplication::translate("StaticData", "ySeg", 0, QApplication::UnicodeUTF8));
        label_31->setText(QApplication::translate("StaticData", "Height", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class StaticData: public Ui_StaticData {};
} // namespace Ui

QT_END_NAMESPACE

#endif // STATICDATA_H
