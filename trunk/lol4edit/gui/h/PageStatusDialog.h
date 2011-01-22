/********************************************************************************
** Form generated from reading UI file 'PageStatusDialog.ui'
**
** Created: Sat 22. Jan 17:36:42 2011
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef PAGESTATUSDIALOG_H
#define PAGESTATUSDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_PageStatusDialog
{
public:
    QVBoxLayout *verticalLayout;
    QTreeWidget *pageStatusList;
    QCheckBox *autoLoadPages;
    QGridLayout *gridLayout;
    QLabel *label;
    QSpinBox *loadPageX;
    QLabel *label_2;
    QSpinBox *loadPageY;
    QSpacerItem *horizontalSpacer;
    QPushButton *loadPageButton;
    QPushButton *pushButton;

    void setupUi(QDialog *PageStatusDialog)
    {
        if (PageStatusDialog->objectName().isEmpty())
            PageStatusDialog->setObjectName(QString::fromUtf8("PageStatusDialog"));
        PageStatusDialog->resize(323, 336);
        verticalLayout = new QVBoxLayout(PageStatusDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        pageStatusList = new QTreeWidget(PageStatusDialog);
        new QTreeWidgetItem(pageStatusList);
        new QTreeWidgetItem(pageStatusList);
        pageStatusList->setObjectName(QString::fromUtf8("pageStatusList"));
        pageStatusList->setRootIsDecorated(false);
        pageStatusList->setSortingEnabled(true);
        pageStatusList->setExpandsOnDoubleClick(false);
        pageStatusList->header()->setVisible(true);
        pageStatusList->header()->setDefaultSectionSize(60);
        pageStatusList->header()->setHighlightSections(false);
        pageStatusList->header()->setProperty("showSortIndicator", QVariant(true));

        verticalLayout->addWidget(pageStatusList);

        autoLoadPages = new QCheckBox(PageStatusDialog);
        autoLoadPages->setObjectName(QString::fromUtf8("autoLoadPages"));
        autoLoadPages->setChecked(true);

        verticalLayout->addWidget(autoLoadPages);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(PageStatusDialog);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        loadPageX = new QSpinBox(PageStatusDialog);
        loadPageX->setObjectName(QString::fromUtf8("loadPageX"));
        loadPageX->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        loadPageX->setMinimum(-32768);
        loadPageX->setMaximum(32767);

        gridLayout->addWidget(loadPageX, 0, 1, 1, 1);

        label_2 = new QLabel(PageStatusDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 0, 2, 1, 1);

        loadPageY = new QSpinBox(PageStatusDialog);
        loadPageY->setObjectName(QString::fromUtf8("loadPageY"));
        loadPageY->setMinimum(-32768);
        loadPageY->setValue(0);

        gridLayout->addWidget(loadPageY, 0, 3, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 4, 1, 1);

        loadPageButton = new QPushButton(PageStatusDialog);
        loadPageButton->setObjectName(QString::fromUtf8("loadPageButton"));
        loadPageButton->setAutoDefault(false);

        gridLayout->addWidget(loadPageButton, 0, 5, 1, 1);

        pushButton = new QPushButton(PageStatusDialog);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setAutoDefault(false);

        gridLayout->addWidget(pushButton, 1, 5, 1, 1);


        verticalLayout->addLayout(gridLayout);


        retranslateUi(PageStatusDialog);
        QObject::connect(autoLoadPages, SIGNAL(toggled(bool)), loadPageX, SLOT(setEnabled(bool)));
        QObject::connect(autoLoadPages, SIGNAL(toggled(bool)), loadPageY, SLOT(setEnabled(bool)));
        QObject::connect(autoLoadPages, SIGNAL(toggled(bool)), loadPageButton, SLOT(setEnabled(bool)));
        QObject::connect(autoLoadPages, SIGNAL(toggled(bool)), pushButton, SLOT(setEnabled(bool)));

        QMetaObject::connectSlotsByName(PageStatusDialog);
    } // setupUi

    void retranslateUi(QDialog *PageStatusDialog)
    {
        PageStatusDialog->setWindowTitle(QApplication::translate("PageStatusDialog", "Page Status", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem = pageStatusList->headerItem();
        ___qtreewidgetitem->setText(3, QApplication::translate("PageStatusDialog", "saved", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(2, QApplication::translate("PageStatusDialog", "status", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(1, QApplication::translate("PageStatusDialog", "y", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(0, QApplication::translate("PageStatusDialog", "x", 0, QApplication::UnicodeUTF8));

        const bool __sortingEnabled = pageStatusList->isSortingEnabled();
        pageStatusList->setSortingEnabled(false);
        QTreeWidgetItem *___qtreewidgetitem1 = pageStatusList->topLevelItem(0);
        ___qtreewidgetitem1->setText(3, QApplication::translate("PageStatusDialog", "yes", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem1->setText(2, QApplication::translate("PageStatusDialog", "loading", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem1->setText(1, QApplication::translate("PageStatusDialog", "3", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem1->setText(0, QApplication::translate("PageStatusDialog", "0", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem2 = pageStatusList->topLevelItem(1);
        ___qtreewidgetitem2->setText(3, QApplication::translate("PageStatusDialog", "no", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem2->setText(2, QApplication::translate("PageStatusDialog", "loaded", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem2->setText(1, QApplication::translate("PageStatusDialog", "2", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem2->setText(0, QApplication::translate("PageStatusDialog", "-1", 0, QApplication::UnicodeUTF8));
        pageStatusList->setSortingEnabled(__sortingEnabled);

        autoLoadPages->setText(QApplication::translate("PageStatusDialog", "Auto Load/Unload", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("PageStatusDialog", "X:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("PageStatusDialog", "Y:", 0, QApplication::UnicodeUTF8));
        loadPageButton->setText(QApplication::translate("PageStatusDialog", "Load", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("PageStatusDialog", "Unload Selected", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class PageStatusDialog: public Ui_PageStatusDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // PAGESTATUSDIALOG_H
