#include "LevelSelectDialog.h"
#include "StandardApplication.h"
#include "EditorApp.h"
#include "h/SelectDialog.h"

void LevelSelectDialog::fillList()
{
	ui->listWidget->clear();
	Ogre::FileInfoListPtr levelfiles = Ogre::ResourceGroupManager::getSingleton().listResourceFileInfo("Level");
	
	
	for(Ogre::FileInfoList::iterator it = levelfiles->begin();it != levelfiles->end();it++)
	{
		Ogre::String obj_id = it->filename;			
		
		if(obj_id != "" && (Ogre::StringUtil::endsWith(obj_id.c_str(),LEVELEXT) || Ogre::StringUtil::endsWith(obj_id.c_str(),ZLEVELEXT) ))
		{
			QString itemname = obj_id.c_str();
			ui->listWidget->addItem(itemname);				
		}
	}
}

void LevelSelectDialog::acceptSelection(QListWidgetItem *item)
{
	EditorApp *app = (EditorApp*)StandardApplication::getSingletonPtr();
	Ogre::String lvlname = item->text().toStdString();
	//hide();
	app->loadLevel(lvlname);
	//QList<QListWidgetItem*> selection = listWidget->selectedItems();
	
}


