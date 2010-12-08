#include "ConfirmDialog.h"
#include "h/ConfirmDialog.h"

ConfirmDialog::ConfirmDialog(QWidget *parent, Qt::WindowFlags f)
	:QDialog(parent,f)
{
	ui = new Ui::ConfirmDialog();
	ui->setupUi(this);
}

int ConfirmDialog::exec(QString title,QString text)
{
	setWindowTitle(title);
	ui->label->setText(text);
	return QDialog::exec();
}