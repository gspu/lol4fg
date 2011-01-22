/********************************************************************************
** Form generated from reading UI file 'inputdialog.ui'
**
** Created: Sat 22. Jan 17:36:42 2011
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef INPUTDIALOG_H
#define INPUTDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_InputStringDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *displayTextLabel;
    QLineEdit *inputStringEdit;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *okButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *InputStringDialog)
    {
        if (InputStringDialog->objectName().isEmpty())
            InputStringDialog->setObjectName(QString::fromUtf8("InputStringDialog"));
        InputStringDialog->resize(347, 122);
        verticalLayout = new QVBoxLayout(InputStringDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        displayTextLabel = new QLabel(InputStringDialog);
        displayTextLabel->setObjectName(QString::fromUtf8("displayTextLabel"));

        verticalLayout->addWidget(displayTextLabel);

        inputStringEdit = new QLineEdit(InputStringDialog);
        inputStringEdit->setObjectName(QString::fromUtf8("inputStringEdit"));

        verticalLayout->addWidget(inputStringEdit);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        okButton = new QPushButton(InputStringDialog);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        okButton->setDefault(true);

        horizontalLayout->addWidget(okButton);

        cancelButton = new QPushButton(InputStringDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        horizontalLayout->addWidget(cancelButton);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(InputStringDialog);
        QObject::connect(okButton, SIGNAL(clicked()), InputStringDialog, SLOT(accept()));
        QObject::connect(cancelButton, SIGNAL(clicked()), InputStringDialog, SLOT(reject()));
        QObject::connect(inputStringEdit, SIGNAL(returnPressed()), InputStringDialog, SLOT(accept()));

        QMetaObject::connectSlotsByName(InputStringDialog);
    } // setupUi

    void retranslateUi(QDialog *InputStringDialog)
    {
        InputStringDialog->setWindowTitle(QApplication::translate("InputStringDialog", "Input XXX", 0, QApplication::UnicodeUTF8));
        displayTextLabel->setText(QApplication::translate("InputStringDialog", "TextLabel", 0, QApplication::UnicodeUTF8));
        okButton->setText(QApplication::translate("InputStringDialog", "OK", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("InputStringDialog", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class InputStringDialog: public Ui_InputStringDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // INPUTDIALOG_H
