/********************************************************************************
** Form generated from reading UI file 'DoorData.ui'
**
** Created: Sat 22. Jan 17:36:42 2011
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef DOORDATA_H
#define DOORDATA_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DoorData
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout_2;
    QLabel *label_3;
    QLineEdit *doorNameEdit;
    QLabel *label_7;
    QLineEdit *targetLevelEdit;
    QLabel *label_8;
    QLineEdit *targetNameEdit;
    QCheckBox *targetIsDoorCB;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *DoorData)
    {
        if (DoorData->objectName().isEmpty())
            DoorData->setObjectName(QString::fromUtf8("DoorData"));
        DoorData->resize(400, 134);
        verticalLayout = new QVBoxLayout(DoorData);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_3 = new QLabel(DoorData);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy);

        gridLayout_2->addWidget(label_3, 0, 0, 1, 1);

        doorNameEdit = new QLineEdit(DoorData);
        doorNameEdit->setObjectName(QString::fromUtf8("doorNameEdit"));

        gridLayout_2->addWidget(doorNameEdit, 0, 1, 1, 1);

        label_7 = new QLabel(DoorData);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        sizePolicy.setHeightForWidth(label_7->sizePolicy().hasHeightForWidth());
        label_7->setSizePolicy(sizePolicy);

        gridLayout_2->addWidget(label_7, 1, 0, 1, 1);

        targetLevelEdit = new QLineEdit(DoorData);
        targetLevelEdit->setObjectName(QString::fromUtf8("targetLevelEdit"));

        gridLayout_2->addWidget(targetLevelEdit, 1, 1, 1, 1);

        label_8 = new QLabel(DoorData);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        sizePolicy.setHeightForWidth(label_8->sizePolicy().hasHeightForWidth());
        label_8->setSizePolicy(sizePolicy);

        gridLayout_2->addWidget(label_8, 2, 0, 1, 1);

        targetNameEdit = new QLineEdit(DoorData);
        targetNameEdit->setObjectName(QString::fromUtf8("targetNameEdit"));

        gridLayout_2->addWidget(targetNameEdit, 2, 1, 1, 1);


        verticalLayout->addLayout(gridLayout_2);

        targetIsDoorCB = new QCheckBox(DoorData);
        targetIsDoorCB->setObjectName(QString::fromUtf8("targetIsDoorCB"));

        verticalLayout->addWidget(targetIsDoorCB);

        verticalSpacer = new QSpacerItem(20, 174, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(DoorData);

        QMetaObject::connectSlotsByName(DoorData);
    } // setupUi

    void retranslateUi(QWidget *DoorData)
    {
        DoorData->setWindowTitle(QApplication::translate("DoorData", "Form", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("DoorData", "Name", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("DoorData", "Target Level", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("DoorData", "Target Name", 0, QApplication::UnicodeUTF8));
        targetIsDoorCB->setText(QApplication::translate("DoorData", "Target is a door", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DoorData: public Ui_DoorData {};
} // namespace Ui

QT_END_NAMESPACE

#endif // DOORDATA_H
