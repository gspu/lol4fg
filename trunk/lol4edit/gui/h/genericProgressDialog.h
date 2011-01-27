/********************************************************************************
** Form generated from reading UI file 'genericProgressDialog.ui'
**
** Created: Wed 26. Jan 20:40:28 2011
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef GENERICPROGRESSDIALOG_H
#define GENERICPROGRESSDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QProgressBar>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ProgressShowDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QProgressBar *progressBar;

    void setupUi(QDialog *ProgressShowDialog)
    {
        if (ProgressShowDialog->objectName().isEmpty())
            ProgressShowDialog->setObjectName(QString::fromUtf8("ProgressShowDialog"));
        ProgressShowDialog->resize(386, 100);
        verticalLayout = new QVBoxLayout(ProgressShowDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(ProgressShowDialog);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        progressBar = new QProgressBar(ProgressShowDialog);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setValue(24);

        verticalLayout->addWidget(progressBar);


        retranslateUi(ProgressShowDialog);

        QMetaObject::connectSlotsByName(ProgressShowDialog);
    } // setupUi

    void retranslateUi(QDialog *ProgressShowDialog)
    {
        ProgressShowDialog->setWindowTitle(QApplication::translate("ProgressShowDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("ProgressShowDialog", "TextLabel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ProgressShowDialog: public Ui_ProgressShowDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // GENERICPROGRESSDIALOG_H
