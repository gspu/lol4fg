/********************************************************************************
** Form generated from reading ui file 'confirmnP5996.ui'
**
** Created: Wed 16. Dec 22:54:42 2009
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef CONFIRMNP5996_H
#define CONFIRMNP5996_H

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

namespace Ui
{
	class ConfirmDialog;
}


class ConfirmDialog: public QDialog
{
public:
	

	ConfirmDialog(QWidget *parent = 0, Qt::WindowFlags f = 0);
	

	int exec(QString title,QString text);

	Ui::ConfirmDialog *ui;
protected:

	

};



#endif // CONFIRMNP5996_H
