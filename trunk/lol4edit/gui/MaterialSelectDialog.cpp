#include "MaterialSelectDialog.h"
#include "h/SelectDialog.h"
#include <QtGui/QLineEdit>
#include "Ogre.h"

int MaterialSelectDialog::exec(QLineEdit *target)
{
	this->target = target;
	return QDialog::exec();
}

void MaterialSelectDialog::acceptSelection(QListWidgetItem *item)
{
	if(!item)
		return;
	target->setText(item->text());
}

void MaterialSelectDialog::fillList()
{
	ui->listWidget->clear();
	//filling list of materials
//#ifdef _DEBUG
//	int debug_limiter = 0;
//#endif
	
	Ogre::ResourceManager::ResourceMapIterator itr = Ogre::MaterialManager::getSingletonPtr()->getResourceIterator();
	while(itr.hasMoreElements())
	{
		QString name = itr.peekNextValue()->getName().c_str();
		
		ui->listWidget->addItem(name);	
		itr.moveNext();
//#ifdef _DEBUG
//		debug_limiter++;
//		if(debug_limiter > 20)
//			break;
//#endif
       

	}  
	//list of materials END



	
	
	
}