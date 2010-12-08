#include "PSSelectDialog.h"
#include "h/SelectDialog.h"
#include <QtGui/QLineEdit>
#include "Ogre.h"

int PSSelectDialog::exec(QLineEdit *target)
{
	this->target = target;
	return QDialog::exec();
}

void PSSelectDialog::acceptSelection(QListWidgetItem *item)
{
	if(!item)
		return;
	target->setText(item->text());
}

void PSSelectDialog::fillList()
{
	ui->listWidget->clear();	
	Ogre::ParticleSystemManager::ParticleSystemTemplateIterator psitr = Ogre::ParticleSystemManager::getSingletonPtr()->getTemplateIterator();
	while(psitr.hasMoreElements())
	{
		
		QString name = psitr.peekNextValue()->getName().c_str();
		ui->listWidget->addItem(name);		
		psitr.moveNext();	
	}	
	
}