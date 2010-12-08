/********************************************************************************
** Form generated from reading UI file 'details.ui'
**
** Created: Mon 20. Sep 09:34:49 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef DETAILS_H
#define DETAILS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DetailsDialog
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox_5;
    QGridLayout *gridLayout_9;
    QLabel *label;
    QLineEdit *posEdit;
    QLabel *label_2;
    QLineEdit *orientEdit;
    QHBoxLayout *horizontalLayout;
    QLabel *label_5;
    QLineEdit *orientAxisEdit;
    QLabel *label_6;
    QLineEdit *orientAngleEdit;
    QLineEdit *scaleEdit;
    QLabel *label_4;
    QWidget *DataContainer;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *applyBtn;
    QPushButton *cancelBtn;
    QPushButton *okBtn;

    void setupUi(QDialog *DetailsDialog)
    {
        if (DetailsDialog->objectName().isEmpty())
            DetailsDialog->setObjectName(QString::fromUtf8("DetailsDialog"));
        DetailsDialog->resize(423, 450);
        verticalLayout = new QVBoxLayout(DetailsDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox_5 = new QGroupBox(DetailsDialog);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        gridLayout_9 = new QGridLayout(groupBox_5);
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        label = new QLabel(groupBox_5);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_9->addWidget(label, 0, 0, 1, 1);

        posEdit = new QLineEdit(groupBox_5);
        posEdit->setObjectName(QString::fromUtf8("posEdit"));

        gridLayout_9->addWidget(posEdit, 0, 1, 1, 1);

        label_2 = new QLabel(groupBox_5);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_9->addWidget(label_2, 1, 0, 1, 1);

        orientEdit = new QLineEdit(groupBox_5);
        orientEdit->setObjectName(QString::fromUtf8("orientEdit"));

        gridLayout_9->addWidget(orientEdit, 1, 1, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_5 = new QLabel(groupBox_5);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout->addWidget(label_5);

        orientAxisEdit = new QLineEdit(groupBox_5);
        orientAxisEdit->setObjectName(QString::fromUtf8("orientAxisEdit"));

        horizontalLayout->addWidget(orientAxisEdit);

        label_6 = new QLabel(groupBox_5);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        horizontalLayout->addWidget(label_6);

        orientAngleEdit = new QLineEdit(groupBox_5);
        orientAngleEdit->setObjectName(QString::fromUtf8("orientAngleEdit"));

        horizontalLayout->addWidget(orientAngleEdit);


        gridLayout_9->addLayout(horizontalLayout, 2, 1, 1, 1);

        scaleEdit = new QLineEdit(groupBox_5);
        scaleEdit->setObjectName(QString::fromUtf8("scaleEdit"));

        gridLayout_9->addWidget(scaleEdit, 3, 1, 1, 1);

        label_4 = new QLabel(groupBox_5);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_9->addWidget(label_4, 3, 0, 1, 1);


        verticalLayout->addWidget(groupBox_5);

        DataContainer = new QWidget(DetailsDialog);
        DataContainer->setObjectName(QString::fromUtf8("DataContainer"));

        verticalLayout->addWidget(DataContainer);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_2);

        applyBtn = new QPushButton(DetailsDialog);
        applyBtn->setObjectName(QString::fromUtf8("applyBtn"));

        horizontalLayout_4->addWidget(applyBtn);

        cancelBtn = new QPushButton(DetailsDialog);
        cancelBtn->setObjectName(QString::fromUtf8("cancelBtn"));

        horizontalLayout_4->addWidget(cancelBtn);

        okBtn = new QPushButton(DetailsDialog);
        okBtn->setObjectName(QString::fromUtf8("okBtn"));

        horizontalLayout_4->addWidget(okBtn);


        verticalLayout->addLayout(horizontalLayout_4);


        retranslateUi(DetailsDialog);

        QMetaObject::connectSlotsByName(DetailsDialog);
    } // setupUi

    void retranslateUi(QDialog *DetailsDialog)
    {
        DetailsDialog->setWindowTitle(QApplication::translate("DetailsDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        groupBox_5->setTitle(QApplication::translate("DetailsDialog", "Transformation", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("DetailsDialog", "Position", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("DetailsDialog", "Orientation", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("DetailsDialog", "or: Axis", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("DetailsDialog", "Angle", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("DetailsDialog", "Scale", 0, QApplication::UnicodeUTF8));
        applyBtn->setText(QApplication::translate("DetailsDialog", "Apply", 0, QApplication::UnicodeUTF8));
        cancelBtn->setText(QApplication::translate("DetailsDialog", "Cancel", 0, QApplication::UnicodeUTF8));
        okBtn->setText(QApplication::translate("DetailsDialog", "OK", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DetailsDialog: public Ui_DetailsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // DETAILS_H
