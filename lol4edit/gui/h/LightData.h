/********************************************************************************
** Form generated from reading UI file 'LightData.ui'
**
** Created: Tue 7. Sep 16:44:53 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef LIGHTDATA_H
#define LIGHTDATA_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LightData
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout_6;
    QLabel *label_35;
    QLineEdit *lightRange;
    QLabel *label_36;
    QLineEdit *lightConstant;
    QLabel *label_37;
    QLineEdit *lightLinear;
    QLineEdit *lightQuadric;
    QLabel *label_38;
    QLabel *label_33;
    QLabel *label_32;
    QLineEdit *lightDiffuseColor;
    QComboBox *lightType;
    QSpacerItem *verticalSpacer_3;
    QLineEdit *lightSpecularColor;
    QLabel *label_34;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_42;
    QLineEdit *lightPowerscale;
    QSpacerItem *horizontalSpacer_5;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_7;
    QLabel *label_39;
    QLabel *label_40;
    QLineEdit *lightInner;
    QLineEdit *lightOuter;
    QLabel *label_41;
    QLineEdit *lightFaloff;

    void setupUi(QWidget *LightData)
    {
        if (LightData->objectName().isEmpty())
            LightData->setObjectName(QString::fromUtf8("LightData"));
        LightData->resize(400, 300);
        verticalLayout = new QVBoxLayout(LightData);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        gridLayout_6 = new QGridLayout();
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        label_35 = new QLabel(LightData);
        label_35->setObjectName(QString::fromUtf8("label_35"));

        gridLayout_6->addWidget(label_35, 4, 0, 1, 1);

        lightRange = new QLineEdit(LightData);
        lightRange->setObjectName(QString::fromUtf8("lightRange"));

        gridLayout_6->addWidget(lightRange, 4, 1, 1, 1);

        label_36 = new QLabel(LightData);
        label_36->setObjectName(QString::fromUtf8("label_36"));

        gridLayout_6->addWidget(label_36, 4, 2, 1, 1);

        lightConstant = new QLineEdit(LightData);
        lightConstant->setObjectName(QString::fromUtf8("lightConstant"));

        gridLayout_6->addWidget(lightConstant, 4, 3, 1, 1);

        label_37 = new QLabel(LightData);
        label_37->setObjectName(QString::fromUtf8("label_37"));

        gridLayout_6->addWidget(label_37, 5, 0, 1, 1);

        lightLinear = new QLineEdit(LightData);
        lightLinear->setObjectName(QString::fromUtf8("lightLinear"));

        gridLayout_6->addWidget(lightLinear, 5, 1, 1, 1);

        lightQuadric = new QLineEdit(LightData);
        lightQuadric->setObjectName(QString::fromUtf8("lightQuadric"));

        gridLayout_6->addWidget(lightQuadric, 5, 3, 1, 1);

        label_38 = new QLabel(LightData);
        label_38->setObjectName(QString::fromUtf8("label_38"));

        gridLayout_6->addWidget(label_38, 5, 2, 1, 1);

        label_33 = new QLabel(LightData);
        label_33->setObjectName(QString::fromUtf8("label_33"));

        gridLayout_6->addWidget(label_33, 1, 0, 1, 1);

        label_32 = new QLabel(LightData);
        label_32->setObjectName(QString::fromUtf8("label_32"));

        gridLayout_6->addWidget(label_32, 0, 0, 1, 1);

        lightDiffuseColor = new QLineEdit(LightData);
        lightDiffuseColor->setObjectName(QString::fromUtf8("lightDiffuseColor"));

        gridLayout_6->addWidget(lightDiffuseColor, 1, 1, 1, 1);

        lightType = new QComboBox(LightData);
        lightType->setObjectName(QString::fromUtf8("lightType"));

        gridLayout_6->addWidget(lightType, 0, 1, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_6->addItem(verticalSpacer_3, 3, 1, 1, 1);

        lightSpecularColor = new QLineEdit(LightData);
        lightSpecularColor->setObjectName(QString::fromUtf8("lightSpecularColor"));

        gridLayout_6->addWidget(lightSpecularColor, 1, 3, 1, 1);

        label_34 = new QLabel(LightData);
        label_34->setObjectName(QString::fromUtf8("label_34"));

        gridLayout_6->addWidget(label_34, 1, 2, 1, 1);


        verticalLayout->addLayout(gridLayout_6);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_42 = new QLabel(LightData);
        label_42->setObjectName(QString::fromUtf8("label_42"));

        horizontalLayout_6->addWidget(label_42);

        lightPowerscale = new QLineEdit(LightData);
        lightPowerscale->setObjectName(QString::fromUtf8("lightPowerscale"));

        horizontalLayout_6->addWidget(lightPowerscale);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_5);


        verticalLayout->addLayout(horizontalLayout_6);

        groupBox_2 = new QGroupBox(LightData);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        gridLayout_7 = new QGridLayout(groupBox_2);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        label_39 = new QLabel(groupBox_2);
        label_39->setObjectName(QString::fromUtf8("label_39"));

        gridLayout_7->addWidget(label_39, 0, 0, 1, 1);

        label_40 = new QLabel(groupBox_2);
        label_40->setObjectName(QString::fromUtf8("label_40"));

        gridLayout_7->addWidget(label_40, 1, 0, 1, 1);

        lightInner = new QLineEdit(groupBox_2);
        lightInner->setObjectName(QString::fromUtf8("lightInner"));

        gridLayout_7->addWidget(lightInner, 0, 1, 1, 1);

        lightOuter = new QLineEdit(groupBox_2);
        lightOuter->setObjectName(QString::fromUtf8("lightOuter"));

        gridLayout_7->addWidget(lightOuter, 1, 1, 1, 1);

        label_41 = new QLabel(groupBox_2);
        label_41->setObjectName(QString::fromUtf8("label_41"));

        gridLayout_7->addWidget(label_41, 0, 2, 1, 1);

        lightFaloff = new QLineEdit(groupBox_2);
        lightFaloff->setObjectName(QString::fromUtf8("lightFaloff"));

        gridLayout_7->addWidget(lightFaloff, 0, 3, 1, 1);


        verticalLayout->addWidget(groupBox_2);


        retranslateUi(LightData);

        QMetaObject::connectSlotsByName(LightData);
    } // setupUi

    void retranslateUi(QWidget *LightData)
    {
        LightData->setWindowTitle(QApplication::translate("LightData", "Form", 0, QApplication::UnicodeUTF8));
        label_35->setText(QApplication::translate("LightData", "Range", 0, QApplication::UnicodeUTF8));
        label_36->setText(QApplication::translate("LightData", "Constant", 0, QApplication::UnicodeUTF8));
        label_37->setText(QApplication::translate("LightData", "Linear", 0, QApplication::UnicodeUTF8));
        label_38->setText(QApplication::translate("LightData", "Quadric", 0, QApplication::UnicodeUTF8));
        label_33->setText(QApplication::translate("LightData", "Diffuse Color", 0, QApplication::UnicodeUTF8));
        label_32->setText(QApplication::translate("LightData", "Type", 0, QApplication::UnicodeUTF8));
        lightType->clear();
        lightType->insertItems(0, QStringList()
         << QApplication::translate("LightData", "Point", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("LightData", "Spotlight", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("LightData", "Directional", 0, QApplication::UnicodeUTF8)
        );
        label_34->setText(QApplication::translate("LightData", "Specular Color", 0, QApplication::UnicodeUTF8));
        label_42->setText(QApplication::translate("LightData", "Powerscale", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("LightData", "Spotlight Range", 0, QApplication::UnicodeUTF8));
        label_39->setText(QApplication::translate("LightData", "Inner", 0, QApplication::UnicodeUTF8));
        label_40->setText(QApplication::translate("LightData", "Outer", 0, QApplication::UnicodeUTF8));
        label_41->setText(QApplication::translate("LightData", "Falloff", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class LightData: public Ui_LightData {};
} // namespace Ui

QT_END_NAMESPACE

#endif // LIGHTDATA_H
