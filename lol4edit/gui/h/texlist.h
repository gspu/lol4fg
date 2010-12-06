/********************************************************************************
** Form generated from reading UI file 'texlist.ui'
**
** Created: Tue 7. Sep 16:44:54 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef TEXLIST_H
#define TEXLIST_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EditTextureListDialog
{
public:
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QListWidget *availTexList;
    QLabel *texturePreview;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer_2;
    QToolButton *TexAddButton;
    QToolButton *TexRemoveWorld;
    QSpacerItem *verticalSpacer;
    QVBoxLayout *verticalLayout_3;
    QTabWidget *tabWidget;
    QWidget *tab;
    QHBoxLayout *horizontalLayout_4;
    QListWidget *usedTexList;
    QWidget *tab_2;
    QHBoxLayout *horizontalLayout_5;
    QListWidget *usedTexList_2;
    QHBoxLayout *horizontalLayout;
    QPushButton *texUpButton;
    QPushButton *texDownButton;
    QCheckBox *useHolesCheckbox;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *cancelButton;
    QPushButton *okButton;

    void setupUi(QDialog *EditTextureListDialog)
    {
        if (EditTextureListDialog->objectName().isEmpty())
            EditTextureListDialog->setObjectName(QString::fromUtf8("EditTextureListDialog"));
        EditTextureListDialog->resize(556, 300);
        horizontalLayout_3 = new QHBoxLayout(EditTextureListDialog);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        availTexList = new QListWidget(EditTextureListDialog);
        availTexList->setObjectName(QString::fromUtf8("availTexList"));

        verticalLayout_2->addWidget(availTexList);

        texturePreview = new QLabel(EditTextureListDialog);
        texturePreview->setObjectName(QString::fromUtf8("texturePreview"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(texturePreview->sizePolicy().hasHeightForWidth());
        texturePreview->setSizePolicy(sizePolicy);
        texturePreview->setMaximumSize(QSize(250, 100));
        texturePreview->setFrameShape(QFrame::NoFrame);
        texturePreview->setTextFormat(Qt::PlainText);
        texturePreview->setPixmap(QPixmap(QString::fromUtf8("Textures/big_kiesel_white.01.png")));
        texturePreview->setScaledContents(false);
        texturePreview->setWordWrap(false);

        verticalLayout_2->addWidget(texturePreview);


        horizontalLayout_3->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        TexAddButton = new QToolButton(EditTextureListDialog);
        TexAddButton->setObjectName(QString::fromUtf8("TexAddButton"));

        verticalLayout->addWidget(TexAddButton);

        TexRemoveWorld = new QToolButton(EditTextureListDialog);
        TexRemoveWorld->setObjectName(QString::fromUtf8("TexRemoveWorld"));

        verticalLayout->addWidget(TexRemoveWorld);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout_3->addLayout(verticalLayout);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        tabWidget = new QTabWidget(EditTextureListDialog);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setTabPosition(QTabWidget::North);
        tabWidget->setUsesScrollButtons(true);
        tabWidget->setDocumentMode(false);
        tabWidget->setTabsClosable(false);
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        horizontalLayout_4 = new QHBoxLayout(tab);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        usedTexList = new QListWidget(tab);
        usedTexList->setObjectName(QString::fromUtf8("usedTexList"));

        horizontalLayout_4->addWidget(usedTexList);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        horizontalLayout_5 = new QHBoxLayout(tab_2);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        usedTexList_2 = new QListWidget(tab_2);
        usedTexList_2->setObjectName(QString::fromUtf8("usedTexList_2"));

        horizontalLayout_5->addWidget(usedTexList_2);

        tabWidget->addTab(tab_2, QString());

        verticalLayout_3->addWidget(tabWidget);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        texUpButton = new QPushButton(EditTextureListDialog);
        texUpButton->setObjectName(QString::fromUtf8("texUpButton"));

        horizontalLayout->addWidget(texUpButton);

        texDownButton = new QPushButton(EditTextureListDialog);
        texDownButton->setObjectName(QString::fromUtf8("texDownButton"));

        horizontalLayout->addWidget(texDownButton);


        verticalLayout_3->addLayout(horizontalLayout);

        useHolesCheckbox = new QCheckBox(EditTextureListDialog);
        useHolesCheckbox->setObjectName(QString::fromUtf8("useHolesCheckbox"));

        verticalLayout_3->addWidget(useHolesCheckbox);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        cancelButton = new QPushButton(EditTextureListDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        horizontalLayout_2->addWidget(cancelButton);

        okButton = new QPushButton(EditTextureListDialog);
        okButton->setObjectName(QString::fromUtf8("okButton"));

        horizontalLayout_2->addWidget(okButton);


        verticalLayout_3->addLayout(horizontalLayout_2);


        horizontalLayout_3->addLayout(verticalLayout_3);


        retranslateUi(EditTextureListDialog);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(EditTextureListDialog);
    } // setupUi

    void retranslateUi(QDialog *EditTextureListDialog)
    {
        EditTextureListDialog->setWindowTitle(QApplication::translate("EditTextureListDialog", "Edit Texture List", 0, QApplication::UnicodeUTF8));
        texturePreview->setText(QString());
        TexAddButton->setText(QApplication::translate("EditTextureListDialog", ">", 0, QApplication::UnicodeUTF8));
        TexRemoveWorld->setText(QApplication::translate("EditTextureListDialog", "<", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("EditTextureListDialog", "Current Page", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("EditTextureListDialog", "Level default", 0, QApplication::UnicodeUTF8));
        texUpButton->setText(QApplication::translate("EditTextureListDialog", "Up", 0, QApplication::UnicodeUTF8));
        texDownButton->setText(QApplication::translate("EditTextureListDialog", "Down", 0, QApplication::UnicodeUTF8));
        useHolesCheckbox->setText(QApplication::translate("EditTextureListDialog", "Use Holes", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("EditTextureListDialog", "Cancel", 0, QApplication::UnicodeUTF8));
        okButton->setText(QApplication::translate("EditTextureListDialog", "Ok", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class EditTextureListDialog: public Ui_EditTextureListDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // TEXLIST_H
