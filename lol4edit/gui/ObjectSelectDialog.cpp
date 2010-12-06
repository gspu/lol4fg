#include "ObjectSelectDialog.h"
#include "h/Objectlist.h"
#include "EditorApp.h"
#include "data.h"
#include "Level.h"
#include "functions.h"


ObjectSelectDialog::ObjectSelectDialog(QWidget *parent, Qt::WindowFlags f)
	:QDialog(parent,f)//, (f & (~Qt::WindowMinimizeButtonHint | ~Qt::WindowMaximizeButtonHint)))
{
/*	this->parent = parent;*/
	
	ui = new Ui::ObjectSelectDialog();
	ui->setupUi(this);

	//connecting stuff	
	connect(ui->objectList,SIGNAL(itemDoubleClicked (QTreeWidgetItem*,int)),this,SLOT(selectEntry(QTreeWidgetItem*,int)));
	connect(ui->addObject,SIGNAL(clicked()),this,SLOT(addButtonClicked()));
}

void ObjectSelectDialog::selectEntry(QTreeWidgetItem *item, int column)
{
	if(item->childCount())
		return;
	ObjectTreeItem *objectItem = dynamic_cast<ObjectTreeItem*>(item);
	if(!objectItem)
		return;
	Ogre::String text = item->text(0).toStdString();
	gamedata *data = objectItem->getGamedata();
	switch(objectItem->getItemType())
	{
	case eitDefault:
		//now there must be gamedata
		if(data)
		{
			EditorApp::getSingletonPtr()->getQtOgreGameLogic()->dropItem(data);
		}
		else
		{
			mLog("Error: no GameData specified for Object Window Entry '"+text+"'");
		}
		break;
	case eitEntrance:
		EditorApp::getSingletonPtr()->getQtOgreGameLogic()->dropItem(otEntrance,"entr");//entr is basename for the entrance
		break;
	case eitLight:
		EditorApp::getSingletonPtr()->getQtOgreGameLogic()->dropItem(otLight,"");//nothing
		break;
	case eitPlane:
		EditorApp::getSingletonPtr()->getQtOgreGameLogic()->dropItem(otStatic,"colorWhite",WT_PLANE);//second parameter is material id
		//EditorApp::getSingletonPtr()->getQtOgreGameLogic()->dropItem(otStatic,"plane");
		break;
	case eitBillboard:
		EditorApp::getSingletonPtr()->getQtOgreGameLogic()->dropItem(otStatic,MAT_PSDUMMY,WT_BILLBOARD);//second parameter is material id
		break;	
	case eitWaterPlane:
		EditorApp::getSingletonPtr()->getQtOgreGameLogic()->dropItem(otWater,"");//second param not necessary
		break;
	case eitStaticMesh:
		EditorApp::getSingletonPtr()->getQtOgreGameLogic()->dropItem(otStatic,text,WT_MESH);//second parameter is .scene filemane
		break;	
	case eitStaticDotScene:
		EditorApp::getSingletonPtr()->getQtOgreGameLogic()->dropItem(otStatic,text,WT_DOTSCENE);//second parameter is .scene filemane
		break;			
	case eitStaticPS:
		EditorApp::getSingletonPtr()->getQtOgreGameLogic()->dropItem(otStatic,text,WT_PARTICLESYSTEM);//second param is PS name
		break;
	}
	
	
//	EditorApp::getSingletonPtr()->getQtOgreGameLogic()->dropItem(otContainer,item->text().toStdString());
}

void ObjectSelectDialog::addButtonClicked()
{
	QList<QTreeWidgetItem*> sel = ui->objectList->selectedItems();
	if(sel.isEmpty())
		return;
	QTreeWidgetItem* item = sel.first();
	selectEntry(item,0);
}
void ObjectSelectDialog::fillLists()
{
	StandardApplication *app = StandardApplication::getSingletonPtr();
	ui->objectList->clear();

	QTreeWidgetItem *staticItem = new QTreeWidgetItem(ui->objectList);
	staticItem->setText(0,"static");
	//adding 3 subbranches
	QTreeWidgetItem *meshItem = new QTreeWidgetItem(staticItem);
	meshItem->setText(0,"mesh");
	QTreeWidgetItem *dsItem = new QTreeWidgetItem(staticItem);
	dsItem->setText(0,"dotScene");
	QTreeWidgetItem *particleItem = new QTreeWidgetItem(staticItem);
	particleItem->setText(0,"particle");

	QTreeWidgetItem *dynamicItem = new QTreeWidgetItem(ui->objectList);
	dynamicItem->setText(0,"dynamic");

	QTreeWidgetItem *itemItem = new QTreeWidgetItem(ui->objectList);
	itemItem->setText(0,"item");	

	QTreeWidgetItem *spellItem = new QTreeWidgetItem(ui->objectList);
	spellItem->setText(0,"spell");

	QTreeWidgetItem *charItem = new QTreeWidgetItem(ui->objectList);
	charItem->setText(0,"character");	


	QTreeWidgetItem *doorItem = new QTreeWidgetItem(ui->objectList);
	doorItem->setText(0,"door");
	

	QTreeWidgetItem *containerItem = new QTreeWidgetItem(ui->objectList);
	containerItem->setText(0,"container");
	


	QTreeWidgetItem *elseItem = new QTreeWidgetItem(ui->objectList);
	elseItem->setText(0,"other");	

	new ObjectTreeItem(elseItem,"Entrance",NULL,eitEntrance);
	new ObjectTreeItem(elseItem,"Light",NULL,eitLight);
	new ObjectTreeItem(elseItem,"Plane",NULL,eitPlane);
	new ObjectTreeItem(elseItem,"Billboard",NULL,eitBillboard);
	new ObjectTreeItem(elseItem,"Waterplane",NULL,eitWaterPlane);	
	
	for(StandardApplication::GameDataMap::iterator itr = app->gameDataMap.begin();
		itr != app->gameDataMap.end();itr++)
	{
		QString str = itr->first.c_str();
		
		gamedata* data = itr->second;

		switch(data->type)
		{
		case gtDynamic: 
			new ObjectTreeItem(dynamicItem,str,data);
			break;
		case gtContainer: 
			new ObjectTreeItem(containerItem,str,data);
			break;
		case gtItem: 
			new ObjectTreeItem(itemItem,str,data);
			break;
		case gtSpell:
			new ObjectTreeItem(spellItem,str,data);
			break;
		case gtCharacter:
			new ObjectTreeItem(charItem,str,data);
			break;
		case gtDoor: 
			new ObjectTreeItem(doorItem,str,data);
			break;		
		}
		//now only static stuff is missing	
	}	

	//list of Particle Systems

	//#ifdef _DEBUG
	//	int debug_limiter = 0;
	//#endif	
	Ogre::ParticleSystemManager::ParticleSystemTemplateIterator psitr = Ogre::ParticleSystemManager::getSingletonPtr()->getTemplateIterator();
	while(psitr.hasMoreElements())
	{
		
		QString name = psitr.peekNextValue()->getName().c_str();
		new ObjectTreeItem(particleItem,name,NULL,eitStaticPS);		
	
		psitr.moveNext();
	//#ifdef _DEBUG
	//		debug_limiter++;
	//		if(debug_limiter > 20)
	//			break;
	//#endif
	}	
	//particle systems END

	//loading list of meshes. WICHTIG
	Ogre::StringVectorPtr meshvector = Ogre::ResourceGroupManager::getSingletonPtr()->findResourceNames(Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,"*.mesh");
	
	for (Ogre::StringVector::iterator i = meshvector->begin(); i != meshvector->end(); ++i)
	{
		Ogre::String str = *i;
		new ObjectTreeItem(meshItem,str.c_str(),NULL,eitStaticMesh);
	}		
	//END list of meshes

	//loading list of dotscenes
	Ogre::StringVectorPtr dsvector = Ogre::ResourceGroupManager::getSingletonPtr()->findResourceNames(Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,"*.scene");		
	for (Ogre::StringVector::iterator i = dsvector->begin(); i != dsvector->end(); ++i)
	{
		Ogre::String str = *i;
		new ObjectTreeItem(dsItem,str.c_str(),NULL,eitStaticDotScene);
	}		
	//END list of dotscenes

}

void ObjectSelectDialog::notifyObjectNotPlaceable(gamedata *data)
{
}