/********************************************************************************
** Form generated from reading UI file 'confirmdialog.ui'
**
** Created: Sat 22. Jan 17:36:42 2011
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef CONFIRMDIALOG_H
#define CONFIRMDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ConfirmDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *okButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *ConfirmDialog)
    {
        if (ConfirmDialog->objectName().isEmpty())
            ConfirmDialog->setObjectName(QString::fromUtf8("ConfirmDialog"));
        ConfirmDialog->resize(400, 109);
        verticalLayout = new QVBoxLayout(ConfirmDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(ConfirmDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setWordWrap(true);

        verticalLayout->addWidget(label);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        okButton = new QPushButton(ConfirmDialog);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        okButton->setDefault(true);

        horizontalLayout->addWidget(okButton);

        cancelButton = new QPushButton(ConfirmDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        horizontalLayout->addWidget(cancelButton);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(ConfirmDialog);
        QObject::connect(cancelButton, SIGNAL(clicked()), ConfirmDialog, SLOT(reject()));
        QObject::connect(okButton, SIGNAL(clicked()), ConfirmDialog, SLOT(accept()));

        QMetaObject::connectSlotsByName(ConfirmDialog);
    } // setupUi

    void retranslateUi(QDialog *ConfirmDialog)
    {
        ConfirmDialog->setWindowTitle(QApplication::translate("ConfirmDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("ConfirmDialog", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">muhkuh</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        okButton->setText(QApplication::translate("ConfirmDialog", "OK", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("ConfirmDialog", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ConfirmDialog: public Ui_ConfirmDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // CONFIRMDIALOG_H
