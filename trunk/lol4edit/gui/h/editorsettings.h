/********************************************************************************
** Form generated from reading UI file 'editorsettings.ui'
**
** Created: Sat 22. Jan 17:36:42 2011
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef EDITORSETTINGS_H
#define EDITORSETTINGS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QFormLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_EditorSettings
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBox;
    QFormLayout *formLayout_2;
    QLabel *label;
    QDoubleSpinBox *gridsnapSpinBox;
    QLabel *label_2;
    QDoubleSpinBox *anglesnapSpinBox;
    QLabel *label_3;
    QDoubleSpinBox *scalesnapSpinBox;
    QGroupBox *groupBox_3;
    QFormLayout *formLayout_3;
    QLabel *label_7;
    QDoubleSpinBox *tAxesScalespinBox;
    QLabel *label_8;
    QLineEdit *cameraStartPosedit;
    QLabel *label_9;
    QLineEdit *WindowTitleEdit;
    QGroupBox *groupBox_2;
    QFormLayout *formLayout;
    QLabel *label_4;
    QDoubleSpinBox *doubleSpinBox_4;
    QLabel *label_5;
    QLabel *label_6;
    QDoubleSpinBox *doubleSpinBox_5;
    QDoubleSpinBox *doubleSpinBox_6;
    QCheckBox *loadTerranCol;
    QCheckBox *showFog;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *EditorSettings)
    {
        if (EditorSettings->objectName().isEmpty())
            EditorSettings->setObjectName(QString::fromUtf8("EditorSettings"));
        EditorSettings->resize(478, 212);
        verticalLayout = new QVBoxLayout(EditorSettings);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        groupBox = new QGroupBox(EditorSettings);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        formLayout_2 = new QFormLayout(groupBox);
        formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, label);

        gridsnapSpinBox = new QDoubleSpinBox(groupBox);
        gridsnapSpinBox->setObjectName(QString::fromUtf8("gridsnapSpinBox"));

        formLayout_2->setWidget(0, QFormLayout::FieldRole, gridsnapSpinBox);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, label_2);

        anglesnapSpinBox = new QDoubleSpinBox(groupBox);
        anglesnapSpinBox->setObjectName(QString::fromUtf8("anglesnapSpinBox"));

        formLayout_2->setWidget(1, QFormLayout::FieldRole, anglesnapSpinBox);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        formLayout_2->setWidget(2, QFormLayout::LabelRole, label_3);

        scalesnapSpinBox = new QDoubleSpinBox(groupBox);
        scalesnapSpinBox->setObjectName(QString::fromUtf8("scalesnapSpinBox"));

        formLayout_2->setWidget(2, QFormLayout::FieldRole, scalesnapSpinBox);


        horizontalLayout->addWidget(groupBox);

        groupBox_3 = new QGroupBox(EditorSettings);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        formLayout_3 = new QFormLayout(groupBox_3);
        formLayout_3->setObjectName(QString::fromUtf8("formLayout_3"));
        formLayout_3->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        label_7 = new QLabel(groupBox_3);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        formLayout_3->setWidget(0, QFormLayout::LabelRole, label_7);

        tAxesScalespinBox = new QDoubleSpinBox(groupBox_3);
        tAxesScalespinBox->setObjectName(QString::fromUtf8("tAxesScalespinBox"));

        formLayout_3->setWidget(0, QFormLayout::FieldRole, tAxesScalespinBox);

        label_8 = new QLabel(groupBox_3);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        formLayout_3->setWidget(1, QFormLayout::LabelRole, label_8);

        cameraStartPosedit = new QLineEdit(groupBox_3);
        cameraStartPosedit->setObjectName(QString::fromUtf8("cameraStartPosedit"));

        formLayout_3->setWidget(1, QFormLayout::FieldRole, cameraStartPosedit);

        label_9 = new QLabel(groupBox_3);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        formLayout_3->setWidget(2, QFormLayout::LabelRole, label_9);

        WindowTitleEdit = new QLineEdit(groupBox_3);
        WindowTitleEdit->setObjectName(QString::fromUtf8("WindowTitleEdit"));

        formLayout_3->setWidget(2, QFormLayout::FieldRole, WindowTitleEdit);


        horizontalLayout->addWidget(groupBox_3);

        groupBox_2 = new QGroupBox(EditorSettings);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        formLayout = new QFormLayout(groupBox_2);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label_4);

        doubleSpinBox_4 = new QDoubleSpinBox(groupBox_2);
        doubleSpinBox_4->setObjectName(QString::fromUtf8("doubleSpinBox_4"));

        formLayout->setWidget(0, QFormLayout::FieldRole, doubleSpinBox_4);

        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_5);

        label_6 = new QLabel(groupBox_2);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_6);

        doubleSpinBox_5 = new QDoubleSpinBox(groupBox_2);
        doubleSpinBox_5->setObjectName(QString::fromUtf8("doubleSpinBox_5"));

        formLayout->setWidget(1, QFormLayout::FieldRole, doubleSpinBox_5);

        doubleSpinBox_6 = new QDoubleSpinBox(groupBox_2);
        doubleSpinBox_6->setObjectName(QString::fromUtf8("doubleSpinBox_6"));

        formLayout->setWidget(2, QFormLayout::FieldRole, doubleSpinBox_6);


        horizontalLayout->addWidget(groupBox_2);


        verticalLayout->addLayout(horizontalLayout);

        loadTerranCol = new QCheckBox(EditorSettings);
        loadTerranCol->setObjectName(QString::fromUtf8("loadTerranCol"));

        verticalLayout->addWidget(loadTerranCol);

        showFog = new QCheckBox(EditorSettings);
        showFog->setObjectName(QString::fromUtf8("showFog"));

        verticalLayout->addWidget(showFog);

        buttonBox = new QDialogButtonBox(EditorSettings);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(EditorSettings);
        QObject::connect(buttonBox, SIGNAL(accepted()), EditorSettings, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), EditorSettings, SLOT(reject()));

        QMetaObject::connectSlotsByName(EditorSettings);
    } // setupUi

    void retranslateUi(QDialog *EditorSettings)
    {
        EditorSettings->setWindowTitle(QApplication::translate("EditorSettings", "Dialog", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("EditorSettings", "Grid Settings", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("EditorSettings", "Grid Snap", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("EditorSettings", "Angle Snap", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("EditorSettings", "Scale Snap", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("EditorSettings", "Misc", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("EditorSettings", "Transform Axes Scale", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("EditorSettings", "Camera Start Pos", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("EditorSettings", "Wndow Titel", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("EditorSettings", "Sensitivity", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("EditorSettings", "Move", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("EditorSettings", "Rotate", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("EditorSettings", "Scroll", 0, QApplication::UnicodeUTF8));
        loadTerranCol->setText(QApplication::translate("EditorSettings", "Load Terrain Collision", 0, QApplication::UnicodeUTF8));
        showFog->setText(QApplication::translate("EditorSettings", "Show Fog", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class EditorSettings: public Ui_EditorSettings {};
} // namespace Ui

QT_END_NAMESPACE

#endif // EDITORSETTINGS_H
