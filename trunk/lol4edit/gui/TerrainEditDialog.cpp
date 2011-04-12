#include "TerrainEditDialog.h"
#include "h/TerrainEditDialog.h"
#include <Ogre.h>
#include <OgreFreeImageCodec.h>
#include <functions.h>
#include <TypeConverter.h>
#include <EditorApp.h>
#include <Level.h>
#include <OgreTerrain.h>
#include "DataHoldingListWidgetItem.h"
#include "LevelTerrainGroup.h"

TerrainEditDialog::TerrainEditDialog(QWidget *parent, Qt::WindowFlags f)
	:QDialog(parent,f)//, (f & (~Qt::WindowMinimizeButtonHint | ~Qt::WindowMaximizeButtonHint)))
{
/*	this->parent = parent;*/
	
	ui = new Ui::TerrainEditDialog();
	ui->setupUi(this);

	fillBrushList();
	brushSelectionChanged();
	fillTextureList();
	//connecting
	QObject::connect(ui->brushList, SIGNAL(itemSelectionChanged()), this, SLOT(brushSelectionChanged()));
	QObject::connect(ui->selectMode, SIGNAL(currentIndexChanged(int)), this, SLOT(editModeChanged(int)));
}

void TerrainEditDialog::fillBrushList()
{
	ui->brushList->clear();
	Ogre::ResourceGroupManager *resMgr = Ogre::ResourceGroupManager::getSingletonPtr();
	Ogre::FileInfoListPtr files = resMgr->listResourceFileInfo("TerrainBrushes");
	//Ogre::StringVector test = Ogre::Codec::getExtensions();
	
	bool firstItem = true;
	for(Ogre::FileInfoList::iterator it = files->begin();it != files->end();it++)
	{
		Ogre::String name = it->filename;	
		QPixmap pixmap;//img.getWidth(),img.getHeight());
		if(createQtPixmapFromResource(pixmap,name,"TerrainBrushes"))
		{
			//worked
			//but check stuff first
			if(pixmap.width() != 64 || pixmap.width() != pixmap.height())
			{
				mLog("Could not load brush "+name+". Only square 64x64 images are supported as brushes");
			}
			else
			{
				QIcon icon(pixmap);
				QListWidgetItem *item = new QListWidgetItem(ui->brushList);
				item->setText(name.c_str());
				item->setIcon(icon);
				if(firstItem)
				{
					ui->brushList->setItemSelected(item,true);
					firstItem = false;
				}
			}
		}
		else
		{
			//not so much
			mLog("Could not load brush "+name+". Is this really a valid image?");
		}
	}
	
}

int TerrainEditDialog::getCurrentTexture()
{
	QListWidgetItem *item = getSelectedItem(ui->texListWidget);
	if(item)
	{
		DataHoldingListWidgetItem *realItem = static_cast<DataHoldingListWidgetItem*>(item);
		return realItem->getCustomType();
	}
	return -1;
}

void TerrainEditDialog::updateTextureList(Level *lvl)
{
	/*
	ui->texListWidget->clear();
	if(!lvl->hasTerrain())
		return;
	//Ogre::StringVector test = Ogre::Codec::getExtensions();
	
	Ogre::Terrain::ImportData& defaultimp = lvl->mTerrainGroup->getDefaultImportSettings();

	for(size_t i = 0;i<defaultimp.layerList.size();i++)//LayerInstanceList::iterator itr = defaultimp.layerList.begin();itr != defaultimp.layerList.end();itr++)
	{		
		//i do the loop this way to have an index
		Ogre::String curName = defaultimp.layerList[i].textureNames[0];
		DataHoldingListWidgetItem *item = new DataHoldingListWidgetItem(ui->texListWidget);
		item->setText(TypeConverter::qt_str(curName));
		item->setCustomType(i);

		QPixmap pixmap;//img.getWidth(),img.getHeight());
		if(createQtPixmapFromResource(pixmap,curName,"TerrainTextures"))
		{
			//worked
			QIcon icon(pixmap);
			//QListWidgetItem *item = new QListWidgetItem(ui->texListWidget);
			item->setText(TypeConverter::qt_str(curName));
			item->setIcon(icon);
		}
		else
		{
			QIcon icon;
			icon.addFile(QString::fromUtf8(":/icon/NoPreview.png"));
			item->setIcon(icon);
		}
	}

	*/
}


void TerrainEditDialog::fillTextureList()
{
	ui->texListWidget->clear();
	//Ogre::ResourceGroupManager *resMgr = Ogre::ResourceGroupManager::getSingletonPtr();
	//Ogre::FileInfoListPtr files = resMgr->listResourceFileInfo("TerrainTextures");
	////Ogre::StringVector test = Ogre::Codec::getExtensions();
	//
	//
	//for(Ogre::FileInfoList::iterator it = files->begin();it != files->end();it++)
	//{
	//	bool couldLoad = false;
	//	Ogre::String name = it->filename;	
	//	QPixmap pixmap;//img.getWidth(),img.getHeight());
	//	if(createQtPixmapFromResource(pixmap,name,"TerrainTextures"))
	//	{
	//		//worked
	//		QIcon icon(pixmap);
	//		QListWidgetItem *item = new QListWidgetItem(ui->texListWidget);
	//		item->setText(name.c_str());
	//		item->setIcon(icon);
	//	}
	//	else
	//	{
	//		//not so much
	//		mLog("Could not load texture "+name+". Is this really a valid texture?");
	//	}
	//	
	//	///*Ogre::Image img;
	//	//img.load(name,"TerrainTextures");*/
	//	//Ogre::DataStreamPtr data = resMgr->openResource(name,"TerrainTextures");
	//	//
	//	//Ogre::uchar *dataBuffer = OGRE_ALLOC_T(Ogre::uchar,data->size(),Ogre::MEMCATEGORY_RESOURCE);
	//	//data->read(dataBuffer,data->size());
	//	//
	//	//
	//	//
	//	//if(!pixmap.loadFromData(dataBuffer,data->size()))
	//	//{
	//	//	//OGRE_FREE(dataBuffer,Ogre::MEMCATEGORY_RESOURCE);
	//	//	////attempt something else. more complicated for example?
	//	//	//Ogre::Image img;
	//	//	//img.load(name,"TerrainTextures");
	//	//	//
	//	//	//img.save("test.png");

	//	//	//data = img.encode(".png");//try to encode it in png. this might work
	//	//	////now do the same magic to allocate stuff and load
	//	//	//Ogre::uchar *dataBuffer = OGRE_ALLOC_T(Ogre::uchar,data->size(),Ogre::MEMCATEGORY_RESOURCE);
	//	//	//data->read(dataBuffer,data->size());
	//	//	//if(pixmap.loadFromData(dataBuffer,data->size()))
	//	//	//	couldLoad = true;
	//	//	//OGRE_FREE(dataBuffer,Ogre::MEMCATEGORY_RESOURCE);
	//	//}
	//	//else
	//	//{
	//	//	couldLoad = true;
	//	//}
	//	//OGRE_FREE(dataBuffer,Ogre::MEMCATEGORY_RESOURCE);
	//	/*if(couldLoad)
	//	{
	//		QIcon icon(pixmap);
	//		QListWidgetItem *item = new QListWidgetItem(ui->texListWidget);
	//		item->setText(name.c_str());
	//		item->setIcon(icon);
	//	}
	//	else
	//	{	
	//		mLog("Could not load texture "+name+". Is this really a valid texture?");
	//	}*/
	//}
}

Ogre::Image TerrainEditDialog::getSelectedBrush()
{
	QListWidgetItem *item = getSelectedItem(ui->brushList);
	if(item)
	{
		//check if stuff is still actual
		if(selectedScale != ui->brushScaleSpin->value() ||
			selectedItem != item)
		{
			brushSelectionChanged();
		}
	}
	return selectedBrush;
	/*Ogre::Image img;
	using namespace TypeConverter;
	QList<QListWidgetItem*> selected = ui->brushList->selectedItems();
	if(selected.count())
	{
		QListWidgetItem *item = selected[0];
		
		img.load(ogre_str(item->text()),"TerrainBrushes");
	}
	return img;*/
}

int TerrainEditDialog::getBrushSize()
{
	return ui->brushScaleSpin->value()*100;
}

//returns selected intensity
int TerrainEditDialog::getIntensity()
{
	//Ogre::Real factor = (float)ui->brushScaleSpin->value()/(float)100.0f;
	return ui->intensSpin->value();
}

float TerrainEditDialog::getIntensityFactor()
{
	return (float)ui->intensSpin->value()/(float)50.0f;
}


QListWidgetItem *TerrainEditDialog::getSelectedItem(QListWidget *list)
{
	QList<QListWidgetItem*> selected = list->selectedItems();
	if(selected.count() > 0)
	{
		return selected[0];		
	}
	return NULL;
}

void TerrainEditDialog::brushSelectionChanged()
{
	using namespace TypeConverter;
	//QList<QListWidgetItem*> selected = ui->brushList->selectedItems();
	QListWidgetItem *item = getSelectedItem(ui->brushList);
	if(item)
	{
		selectedItem = item;		
		
		selectedBrush.load(ogre_str(item->text()),"TerrainBrushes");
		//maybe scale it?
		int test = ui->brushScaleSpin->value();
		//values are between 1 and 100
		//first, I need to bring them to 1 to 50
		//then, 25 should be "1"
		Ogre::Real factor = getScaleFactor();//(float)ui->brushScaleSpin->value()/(float)100.0f;
		if(factor != 1)
		{			
			if(selectedBrush.getWidth()*factor < 1)
				selectedBrush.resize(1,1);
			else
				selectedBrush.resize(selectedBrush.getWidth()*factor,selectedBrush.getHeight()*factor);
		}
	}
	selectedScale = ui->brushScaleSpin->value();
	
}

float TerrainEditDialog::getScaleFactor()
{
	return  (float)ui->brushScaleSpin->value()/(float)100.0f;
}

void TerrainEditDialog::editModeChanged(int index)
{
	EditorApp *app = EditorApp::getSingletonPtr();
	/*   enum TerrainEditMode
    {
        teDeform,
        tePaint,
        teSoften,
        teFlatten,
		teHoles
    };*/
	switch(index)
	{
	case 0://deform
		app->setTerrainEditMode(EditorApp::teDeform);
		break;
	case 1://flatten
		app->setTerrainEditMode(EditorApp::teFlatten);
		break;
	case 2://soften
		app->setTerrainEditMode(EditorApp::teSoften);
		break;
	case 3://paint
		app->setTerrainEditMode(EditorApp::tePaint);
		break;
	//case 4://holes
	//	app->setTerrainEditMode(EditorApp::teHoles);
	//	break;
	};
}