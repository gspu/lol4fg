#include "GenericSelectDialog.h"
#include "h/SelectDialog.h"
#include <QtGui/QListWidget>

GenericSelectDialog::GenericSelectDialog(QWidget *parent, Qt::WindowFlags f):
QDialog(parent,f)
{
	ui = new Ui::SelectDialog();
	ui->setupUi(this);
	
	//this->setAttribute(Qt::WA_DeleteOnClose);
}

void GenericSelectDialog::accept()
{
	QList<QListWidgetItem*> selection = ui->listWidget->selectedItems();
	if(selection.empty())
		return;
	QListWidgetItem* item = selection.first();
	acceptSelection(item);
	//close();
	QDialog::accept();
}
	//void GenericSelectDialog::setupUi()
	//{
	//	dialog = new QDialog();//this;

	//	if (dialog->objectName().isEmpty())
	//		dialog->setObjectName(QString::fromUtf8("Dialog"));
	//	dialog->resize(303, 314);
	//	verticalLayout = new QVBoxLayout(dialog);
	//	verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
	//	listWidget = new QListWidget(dialog);
	//	listWidget->setObjectName(QString::fromUtf8("listWidget"));
	//	//listWidget->set

	//	verticalLayout->addWidget(listWidget);

	//	horizontalLayout = new QHBoxLayout();
	//	horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
	//	horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

	//	horizontalLayout->addItem(horizontalSpacer);

	//	cancelButton = new QPushButton(dialog);
	//	cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

	//	horizontalLayout->addWidget(cancelButton);

	//	okayButton = new QPushButton(dialog);
	//	okayButton->setObjectName(QString::fromUtf8("okayButton"));

	//	horizontalLayout->addWidget(okayButton);


	//	verticalLayout->addLayout(horizontalLayout);


	//	retranslateUi();

	//	QMetaObject::connectSlotsByName(this);
	//	connectSignals();
	//	fillList();
	//	this->setWidget(dialog);
	//} // setupUi

	//void GenericSelectDialog::retranslateUi()
	//{
	//	this->setWindowTitle(QApplication::translate("Dialog", "Dialog", 0, QApplication::UnicodeUTF8));
	//	cancelButton->setText(QApplication::translate("Dialog", "Cancel", 0, QApplication::UnicodeUTF8));
	//	okayButton->setText(QApplication::translate("Dialog", "OK", 0, QApplication::UnicodeUTF8));
	//	Q_UNUSED(this);
	//} // retranslateUi

	void GenericSelectDialog::connectSignals()
	{
		/*connect(cancelButton, SIGNAL(clicked()), this, SLOT(hide()));
		connect(okayButton, SIGNAL(clicked()), this, SLOT(okButtonClick()));*/
		connect(ui->listWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(acceptSelection(QListWidgetItem*)));
	}

	/*void GenericSelectDialog::okButtonClick()
	{
		QList<QListWidgetItem*> selection = listWidget->selectedItems();
		QListWidgetItem* item = selection.first();
		acceptSelection(item);
	}*/
