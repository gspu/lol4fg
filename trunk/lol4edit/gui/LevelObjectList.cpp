#include "LevelObjectList.h"
#include "h/objectview.h"
#include <Level.h>
#include <GameObject.h>
#include "TypeConverter.h"
#include <EditorApp.h>
#include <QtGui/QMenu>

LevelObjectList::LevelObjectList(QWidget *parent, Qt::WindowFlags f)
		:QDialog(parent,f)//, (f & (~Qt::WindowMinimizeButtonHint | ~Qt::WindowMaximizeButtonHint)))
{
	ui = new Ui::ObjectListDialog();
	ui->setupUi(this);

	QObject::connect(ui->levelObjectList,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(itemDoubleClicked(QListWidgetItem*)));
}



void ObjectListItem::setGameObject(GameObject *obj)
{
	QIcon icon;
	QString iconFile;
	QString toolTip;
	switch(obj->getType())
	{
	case otInvalid:
		iconFile = QString::fromUtf8(":/icon/invalid.png");
		toolTip = "Invald Item";
		break;
	case otDynamic:
		iconFile = QString::fromUtf8(":/icon/cog.png");
		toolTip = "Dynamic Object";
		break;
	case otItem:
		iconFile = QString::fromUtf8(":/icon/sword.png");
		toolTip = "Item";
		break;
	case otChar:
		iconFile = QString::fromUtf8(":/icon/user_silhouette.png");
		toolTip = "Character";
		break;
	case otDoor:
		iconFile = QString::fromUtf8(":/icon/door.png");
		toolTip = "Door";
		break;
	case otLight:
		iconFile = QString::fromUtf8(":/icon/lamp.png");
		toolTip = "Light";
		break;
	case otEntrance:
		iconFile = QString::fromUtf8(":/icon/door_in.png");
		toolTip = "Entrance";
		break;
	case otStatic:
		iconFile = QString::fromUtf8(":/icon/box.png");
		toolTip = "Static Object";
		break;
	case otMissile:
		iconFile = QString::fromUtf8(":/icon/rocket-fly.png");
		toolTip = "Missile";
		break;
	case otContainer:
		iconFile = QString::fromUtf8(":/icon/paperbag.png");
		toolTip = "Container";
		break;
	/*case otGroup:
		iconFile = QString::fromUtf8(":/icon/door.png");
		break;*/
	case otExplosion:
		iconFile = QString::fromUtf8(":/icon/bomb.png");
		toolTip = "Explosion";
		break;
	case otDeleted:
		iconFile = QString::fromUtf8(":/icon/deleted.png");
		toolTip = "Deleted";
		break;
	case otWater:
		iconFile = QString::fromUtf8(":/icon/water.png");
		toolTip = "Water Plane";
		break;
	case otBeam:
		iconFile = QString::fromUtf8(":/icon/beam.png");
		toolTip = "Beam";
		break;
	}
	icon.addFile(iconFile, QSize(), QIcon::Normal, QIcon::Off);
	setToolTip(toolTip);
    setIcon(icon);
	mObject = obj;
}



//LEVELOBJECTLIST//

void LevelObjectList::initPopupMenu()
{
	/*mMenu = new QMenu();
        menuFile->setObjectName(QString::fromUtf8("menuFile"));*/
}

void LevelObjectList::setLevel(Level *lvl)
{
	mLevel = lvl;
	updateList();
}

void LevelObjectList::itemDoubleClicked ( QListWidgetItem * item )
{
	ObjectListItem *myItem = dynamic_cast<ObjectListItem*>(item);
	if(myItem)
	{		
		EditorApp::getSingletonPtr()->showObjectFromTop(myItem->getGameObject());
	}
}

void LevelObjectList::updateList(GameObject *changed,bool added )
{
	ui->levelObjectList->clear();
	for(ObjectList::iterator itr = mLevel->mObjects.begin();itr!= mLevel->mObjects.end();itr++)
	{
		GameObject *obj = *itr;

		new ObjectListItem(obj,TypeConverter::qt_str(obj->getDisplayName()),ui->levelObjectList);
	}
}