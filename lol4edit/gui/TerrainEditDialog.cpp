#include "TerrainEditDialog.h"
#include "h/TerrainEditDialog.h"
#include <Ogre.h>

TerrainEditDialog::TerrainEditDialog(QWidget *parent, Qt::WindowFlags f)
	:QDialog(parent,f)//, (f & (~Qt::WindowMinimizeButtonHint | ~Qt::WindowMaximizeButtonHint)))
{
/*	this->parent = parent;*/
	
	ui = new Ui::TerrainEditDialog();
	ui->setupUi(this);

	fillTextureList();
}

void TerrainEditDialog::fillTextureList()
{
	ui->texListWidget->clear();
	Ogre::ResourceGroupManager *resMgr = Ogre::ResourceGroupManager::getSingletonPtr();
	Ogre::FileInfoListPtr files = resMgr->listResourceFileInfo("TerrainTextures");
	
	
	
	for(Ogre::FileInfoList::iterator it = files->begin();it != files->end();it++)
	{
		
		Ogre::String name = it->filename;		
		/*Ogre::Image img;
		img.load(name,"TerrainTextures");*/
		Ogre::DataStreamPtr data = resMgr->openResource(name,"TerrainTextures");
		
		Ogre::uchar *dataBuffer = OGRE_ALLOC_T(Ogre::uchar,data->size(),Ogre::MEMCATEGORY_RESOURCE);
		data->read(dataBuffer,data->size());
		
		
		QPixmap pixmap;//img.getWidth(),img.getHeight());
		pixmap.loadFromData(dataBuffer,data->size());

		QIcon icon(pixmap);

		QListWidgetItem *item = new QListWidgetItem(ui->texListWidget);
		item->setIcon(icon);
	}
}

void TerrainEditDialog::fillBrushList()
{
}