#include "InputDialog.h"
#include "h/InputDialog.h"

InputDialog::InputDialog(QWidget *parent, Qt::WindowFlags f)
		:QDialog(parent,f)//, (f & (~Qt::WindowMinimizeButtonHint | ~Qt::WindowMaximizeButtonHint)))
{
	ui = new Ui::InputStringDialog();
	ui->setupUi(this);
}

int InputDialog::exec(QString title, QString text)
{
	setWindowTitle(title);
	ui->displayTextLabel->setText(text);
	ui->inputStringEdit->setText("");
	ui->inputStringEdit->setFocus();
	return QDialog::exec();
}
Ogre::String InputDialog::getInputValue()
{
	return ui->inputStringEdit->text().toStdString();
}