#include "ObjectDetailsDialog.h"
#include "GameObject.h"
#include "functions.h"
#include "StaticObject.h"
#include "DoorObject.h"
#include "EntranceObject.h"
#include "LightObject.h"
#include "GameChar.h"
#include "ContainerObject.h"
#include "WaterPlane.h"
#include <QtGui/QTreeWidget>
#include "MaterialSelectDialog.h"
#include "PSSelectDialog.h"

#include "h/CharacterData.h"
#include "h/DoorData.h"
#include "h/LightData.h"
#include "h/StaticData.h"
#include "h/WaterData.h"
#include "h/editinventory.h"


ObjectDetailsDialog::ObjectDetailsDialog(GameObject *obj,QWidget *parent, Qt::WindowFlags f):
	QDialog(parent,f),
	ui(new Ui::DetailsDialog()),
	mObj(obj),
	chData(NULL),
	drData(NULL),
	liData(NULL),
	waData(NULL),
	stData(NULL)
{
	ui->setupUi(this);
	initObjectData();
	
	this->setAttribute(Qt::WA_DeleteOnClose);
	connectSlots();
	
}

void ObjectDetailsDialog::initContainerGui()
{
	initInventoryEditor(ui->DataContainer);
	
}

void ObjectDetailsDialog::showMatSelect()
{
	MaterialSelectDialog *dialog = new MaterialSelectDialog(this);
	dialog->exec(stData->staticMatName);
}

void ObjectDetailsDialog::showPSSelect()
{
	PSSelectDialog *dialog = new PSSelectDialog(this);
	dialog->exec(stData->staticMeshName);
}

void ObjectDetailsDialog::showSoundSelect()
{
}

void ObjectDetailsDialog::initStaticGui()
{
	stData = new Ui::StaticData();
	stData->setupUi(ui->DataContainer);
	
	//ui->staticMatSelect
}

void ObjectDetailsDialog::initWaterGui()
{
	waData = new Ui::WaterData();
	waData->setupUi(ui->DataContainer);

}

void ObjectDetailsDialog::initLightGui()
{
	liData = new Ui::LightData();
	liData->setupUi(ui->DataContainer);
	
}
void ObjectDetailsDialog::initDoorGui()
{
	drData = new Ui::DoorData();
	drData->setupUi(ui->DataContainer);
	
}

void ObjectDetailsDialog::initCharGui()
{
	chData = new Ui::CharacterData();
	chData->setupUi(ui->DataContainer);
	//und inv
	initInventoryEditor(chData->inventoryPlaceholder);

	
}

void ObjectDetailsDialog::initInventoryEditor(QWidget *widget)
{
	ctData = new Ui::EditInventory();
	ctData->setupUi(widget);
}

void ObjectDetailsDialog::fillInventoryList(Inventory *inv)
{
	QTreeWidget *inventoryList = ctData->inventoryList;
	
	       
   
	/*inventoryList->header()->setResizeMode(0,QHeaderView::Stretch);
	inventoryList->header()->setResizeMode(1,QHeaderView::ResizeToContents);
	inventoryList->setRootIsDecorated(false);*/
	inventoryList->clear();

	for(Inventory::ItemVector::iterator itr = inv->itemVector.begin();itr != inv->itemVector.end();itr++)
	{
		Inventory::invDataItem cur = *itr;
		QTreeWidgetItem *item = new QTreeWidgetItem(QStringList());
		item->setText(0,cur.item->id.c_str());
		item->setText(1,QString::number(cur.count));
		
		item->setFlags(Qt::ItemIsEditable | item->flags() );
		inventoryList->addTopLevelItem(item);			
	}      
	
}


void ObjectDetailsDialog::updateInventory(Inventory *inv)
{
	inv->clear();
	QTreeWidget *inventoryList = ctData->inventoryList;
	
	QTreeWidgetItem *item = inventoryList->topLevelItem(0);
	while(item)
	{
		
		
		unsigned int count = item->text(1).toUInt();
		Ogre::String id = item->text(0).toStdString();
		inv->addItem(id,count);
		item = inventoryList->itemBelow(item);				
	}	
}

void ObjectDetailsDialog::fillSpellbookList(Spellbook *sb)
{
	
	chData->spellbookList->clear();

	
	for(Spellbook::SpellVector::iterator itr = sb->spellVector.begin();itr != sb->spellVector.end();itr++)
	{
		gamedata_spell *cur = *itr;
		
		QTreeWidgetItem *item = new QTreeWidgetItem(QStringList());
		item->setText(0,cur->id.c_str());
		
		item->setFlags(Qt::ItemIsEditable | item->flags() );
		chData->spellbookList->addTopLevelItem(item);			
	}  
}

void ObjectDetailsDialog::updateSpellbook(Spellbook *sb)
{
	sb->clear();


	
	QTreeWidgetItem *item = chData->spellbookList->topLevelItem(0);
	while(item)
	{
	
		
		
		Ogre::String id = item->text(0).toStdString();
		sb->addSpell(id);
		item = chData->spellbookList->itemBelow(item);				
	}	
}

void ObjectDetailsDialog::connectSlots()
{
	QObject::connect(ui->applyBtn, SIGNAL(clicked()), this, SLOT(apply()));
	QObject::connect(ui->okBtn, SIGNAL(clicked()), this, SLOT(accept()));
	QObject::connect(ui->cancelBtn, SIGNAL(clicked()), this, SLOT(close()));
	//connect my close to the mdisubwindow's close...
	
	
}

void ObjectDetailsDialog::initObjectData()
{
	this->setWindowTitle(mObj->getDisplayName().c_str());
	/*
	0 = door
	1 = char
	2 = cont
	3 = stat
	4 = light
	5 = water
	*/
	Ogre::Vector3 mPos, scale;
	Ogre::Quaternion q;
	mObj->getPositionOrientation(mPos,q);
	scale = mObj->getScale();
	
	Ogre::Degree angle;
	Ogre::Vector3 axis;
	q.ToAngleAxis(angle,axis);

	ui->posEdit->setText(str(mPos).c_str());
	ui->orientEdit->setText(str(q).c_str());
	ui->scaleEdit->setText(str(scale).c_str());

	ui->orientAxisEdit->setText(str(axis).c_str());
	ui->orientAngleEdit->setText(str(angle).c_str());

	QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

	switch(mObj->getType())
	{
	case otDoor:
	case otEntrance:
		{								
			initDoorGui();
			
			//ACTUAL DATA
			if(mObj->getType() == otDoor)
			{
				DoorData stuff = static_cast<DoorObject*>(mObj)->getDoorData();
				drData->doorNameEdit->setEnabled(false);
				drData->targetNameEdit->setText(stuff.targetEntrance.c_str());
				drData->targetLevelEdit->setText(stuff.targetLevel.c_str());	
			}
			else
			{
				drData->doorNameEdit->setText(static_cast<entranceObject*>(mObj)->getName().c_str());
				drData->targetNameEdit->setEnabled(false);
				drData->targetLevelEdit->setEnabled(false);
			}
			
		}
		break;			


	case otChar:
		{
			GameChar *ch = static_cast<GameChar*>(mObj);
			initCharGui();
			fillInventoryList(ch->getInventory());
			fillSpellbookList(ch->getSpellbook());

			

			chData->charMelee->setText(ch->getMeleeId().c_str());
			chData->charRanged->setText(ch->getRangedId().c_str());
			chData->charArmor->setText(ch->getArmorId().c_str());
			chData->charShield->setText(ch->getShieldId().c_str());
			chData->charElse1->setText(ch->getElse1Id().c_str());
			chData->charElse2->setText(ch->getElse2Id().c_str());
			chData->charElse3->setText(ch->getElse3Id().c_str());
			chData->charElse4->setText(ch->getElse4Id().c_str());
			chData->charBag->setText(ch->getBagId().c_str());

		}
		break;
	case otContainer:
		{
			ContainerObject *ct = static_cast<ContainerObject*>(mObj);
			initContainerGui();
			fillInventoryList(ct->mInv);			
		}
		break;
	case otLight:
		{
			initLightGui();
			
			LightObject *light = static_cast<LightObject*>(mObj);

			Ogre::Light *l = light->getLight();

			switch(l->getType())
			{
				//0 = point, 1 = spot, 2 = dir
			case Ogre::Light::LT_DIRECTIONAL:
				liData->lightType->setCurrentIndex(2);
				break;
			case Ogre::Light::LT_POINT:
				liData->lightType->setCurrentIndex(0);
				break;
			case Ogre::Light::LT_SPOTLIGHT:
				liData->lightType->setCurrentIndex(1);
				break;
			}
			
			//COlours
			liData->lightDiffuseColor->setText(str(l->getDiffuseColour()).c_str());
			liData->lightSpecularColor->setText(str(l->getSpecularColour()).c_str());
			//Attenuation
			liData->lightRange->setText(str(l->getAttenuationRange()).c_str());
			liData->lightConstant->setText(str(l->getAttenuationConstant()).c_str());
			liData->lightLinear->setText(str(l->getAttenuationLinear()).c_str());
			liData->lightQuadric->setText(str(l->getAttenuationQuadric()).c_str());
			//Spotlight
		
			liData->lightInner->setText(str(l->getSpotlightInnerAngle()).c_str());
			liData->lightOuter->setText(str(l->getSpotlightOuterAngle()).c_str());
			liData->lightFaloff->setText(str(l->getSpotlightFalloff()).c_str());	
	
			liData->lightPowerscale->setText(str(l->getPowerScale()).c_str());	
			

		}
		break;
	case otWater:			
		{		
			initWaterGui();

			WaterPlane *water = static_cast<WaterPlane*>(mObj);

			//WorldArtData waData = obj->getWorldArt()->getData();
			FluidData fd = water->getFluidData();
			
			

			//waterPlaneNormal->setText(str(fd.plane.normal).c_str());
//				waterPlaneConstant->setText(str(fd.plane.constant).c_str());
			waData->waterPlaneWidth->setText(str(fd.plane.width).c_str());
			waData->waterPlaneHeight->setText(str(fd.plane.height).c_str());
			waData->waterPlaneXSeg->setText(str(fd.plane.xSeg).c_str());
			waData->waterPlaneYSeg->setText(str(fd.plane.ySeg).c_str());
			waData->waterPlaneUpVector->setText(str(fd.plane.up).c_str());
			waData->waterPlaneVTile->setText(str(fd.plane.vTile).c_str());
			waData->waterPlaneUTile->setText(str(fd.plane.uTile).c_str());
			waData->waterPlaneTexCoords->setText(str(fd.plane.numTexCoordSets).c_str());

			waData->waterAngVisc->setText(str(fd.angularViscosity).c_str());
			waData->waterDensity->setText(str(fd.density).c_str());
			waData->waterLinVisc->setText(str(fd.linearViscosity).c_str());
			waData->waterSound->setText(fd.sound.c_str());



			//fd.plane
			waData->waterMaterial->setText(fd.material.c_str());

		


		}
		break;
	case otStatic:
		{			
			initStaticGui();

			WorldArtData cur = mObj->getWorldArt()->getData();


			stData->staticMeshName->setEnabled(false);
			stData->staticMatName->setEnabled(false);

			stData->staticMeshSelect->setEnabled(false);
			stData->staticMatName->setEnabled(false);

			stData->planeNormal->setEnabled(false);
			stData->planeConstant->setEnabled(false);
			stData->planeWidth->setEnabled(false);
			stData->planeHeight->setEnabled(false);
			stData->planeXSeg->setEnabled(false);
			stData->planeYSeg->setEnabled(false);
			stData->planeUpVector->setEnabled(false);
			stData->planeVTile->setEnabled(false);
			stData->planeUTile->setEnabled(false);
			stData->planeTexCoords->setEnabled(false);

		
			switch(cur.type)
			{
			case WT_MESH:
				stData->staticMeshName->setEnabled(true);
				stData->staticMatName->setEnabled(true);
				stData->staticMeshName->setText(cur.filename.c_str());
				stData->staticMatName->setText(cur.material.c_str());
				stData->staticMatSelect->setEnabled(true);
				//connect mat button
				connect(stData->staticMatSelect,SIGNAL(clicked()),this,SLOT(showMatSelect()));
				break;
			case WT_BILLBOARD:
				stData->staticMatName->setEnabled(true);
				stData->staticMatName->setText(cur.material.c_str());
				stData->staticMatSelect->setEnabled(true);
				//connect mat button
				connect(stData->staticMatSelect,SIGNAL(clicked()),this,SLOT(showMatSelect()));
				break;
			case WT_DOTSCENE:
				stData->staticMeshName->setEnabled(true);
				stData->staticMeshName->setText(cur.filename.c_str());
				break;
			case WT_PARTICLESYSTEM:
				stData->staticMeshName->setEnabled(true);
				stData->staticMeshName->setText(cur.material.c_str());
				stData->staticMeshSelect->setEnabled(true);
				//connect mesh button to PS selector
				connect(stData->staticMeshSelect,SIGNAL(clicked()),this,SLOT(showPSSelect()));
				break;
			case WT_PLANE:
				stData->staticMatName->setEnabled(true);
				stData->staticMatName->setText(cur.material.c_str());

				stData->staticMatSelect->setEnabled(true);

				stData->planeNormal->setEnabled(true);
				stData->planeConstant->setEnabled(true);
				stData->planeWidth->setEnabled(true);
				stData->planeHeight->setEnabled(true);
				stData->planeXSeg->setEnabled(true);
				stData->planeYSeg->setEnabled(true);
				stData->planeUpVector->setEnabled(true);
				stData->planeVTile->setEnabled(true);
				stData->planeUTile->setEnabled(true);
				stData->planeTexCoords->setEnabled(true);
				//connect mat button
				connect(stData->staticMatSelect,SIGNAL(clicked()),this,SLOT(showMatSelect()));

				stData->planeNormal->setText(str(cur.pData.normal).c_str());
				stData->planeConstant->setText(str(cur.pData.constant).c_str());
				stData->planeWidth->setText(str(cur.pData.width).c_str());
				stData->planeHeight->setText(str(cur.pData.height).c_str());
				stData->planeXSeg->setText(str(cur.pData.xSeg).c_str());
				stData->planeYSeg->setText(str(cur.pData.ySeg).c_str());
				stData->planeUpVector->setText(str(cur.pData.up).c_str());
				stData->planeVTile->setText(str(cur.pData.vTile).c_str());
				stData->planeUTile->setText(str(cur.pData.uTile).c_str());
				stData->planeTexCoords->setText(str(cur.pData.numTexCoordSets).c_str());

			}
			
			
			bool col = static_cast<StaticObject*>(mObj)->getCollision();
			stData->staticHasShadows->setChecked(cur.shadow);//->setText(cur.material.c_str());
			stData->staticIsStatic->setChecked(col);
		}
		break;
	default:
		{

		//stackedWidget->hide();
		}

	}
	
}




void ObjectDetailsDialog::apply()
{
	

	Ogre::String posStr = ui->posEdit->text().toStdString();
	Ogre::String orientStr = ui->orientEdit->text().toStdString();
	Ogre::String scaleStr = ui->scaleEdit->text().toStdString();

	Ogre::Vector3 pos = Ogre::StringConverter::parseVector3(posStr);
	Ogre::Quaternion ornt = Ogre::StringConverter::parseQuaternion(orientStr);
	Ogre::Vector3 scale = Ogre::StringConverter::parseVector3(scaleStr);

	mObj->setPositionOrientation(pos, ornt);
	mObj->setScale(scale);


	/*
	orientAxisEdit->setText(str(axis).c_str());
	orientAngleEdit->setText(str(angle).c_str());
	*/


	switch(mObj->getType())
	{
	case otDoor:
		{
			DoorData stuff;// = static_cast<DoorObject*>(mObj)->getDoorData();
			stuff.targetEntrance = drData->targetNameEdit->text().toStdString();
			stuff.targetLevel = drData->targetLevelEdit->text().toStdString();
			static_cast<DoorObject*>(mObj)->setDoorData(stuff);
		}
		break;
	case otEntrance:
		static_cast<entranceObject*>(mObj)->setName(drData->doorNameEdit->text().toStdString());				
		break;
	case otChar:
		{
			GameChar *ch = static_cast<GameChar*>(mObj);		

			
			//todo überprüfung einbauen
			ch->setMelee(chData->charMelee->text().toStdString());
			ch->setRanged(chData->charRanged->text().toStdString());
			ch->setArmor(chData->charArmor->text().toStdString());
			ch->setShield(chData->charShield->text().toStdString());
			ch->setElse1(chData->charElse1->text().toStdString());
			ch->setElse2(chData->charElse2->text().toStdString());
			ch->setElse3(chData->charElse3->text().toStdString());
			ch->setElse4(chData->charElse4->text().toStdString());
			ch->setBag(chData->charBag->text().toStdString());

			updateInventory(ch->getInventory());
			updateSpellbook(ch->getSpellbook());

		}
		break;
		case otContainer:
			{
				ContainerObject *ct = static_cast<ContainerObject*>(mObj);		
				updateInventory(ct->mInv);
				//stackedWidget->setCurrentIndex(2);
				//hier bloß ein inv machen. soll das gleiche sein wie bei char
				//evtl this->setupInventory(gruppenwidget)?
			}
			break;
		case otLight:
			{				
				LightObject *light = static_cast<LightObject*>(mObj);

				Ogre::Light *l = light->getLight();
				
				
	
				switch(liData->lightType->currentIndex())
				{
					//0 = point, 1 = spot, 2 = dir
				case 2:
					light->setLightType(Ogre::Light::LT_DIRECTIONAL);
					break;
				case 1:
					light->setLightType(Ogre::Light::LT_SPOTLIGHT);
					break;
				default:
				//case 0:
					light->setLightType(Ogre::Light::LT_POINT);
				//	break;
				
				}
				
				//Colours
				l->setDiffuseColour(Ogre::StringConverter::parseColourValue(liData->lightDiffuseColor->text().toStdString()));
				l->setSpecularColour(Ogre::StringConverter::parseColourValue(liData->lightSpecularColor->text().toStdString()));
				//Attenuation
				Ogre::Real range		= liData->lightRange->text().toFloat(); 
				Ogre::Real constant	= liData->lightConstant->text().toFloat();
				Ogre::Real linear		= liData->lightLinear->text().toFloat();
				Ogre::Real quadratic	= liData->lightQuadric->text().toFloat();
				l->setAttenuation(range,constant,linear,quadratic);//Real range, Real constant, Real linear, Real quadratic
				
				//Spotlight

				//Angles
				l->setSpotlightInnerAngle(Ogre::StringConverter::parseAngle(liData->lightInner->text().toStdString()));
				l->setSpotlightOuterAngle(Ogre::StringConverter::parseAngle(liData->lightOuter->text().toStdString()));
				
				l->setSpotlightFalloff(liData->lightFaloff->text().toFloat());	

				l->setPowerScale(liData->lightPowerscale->text().toFloat());	
				
				light->showMesh();
			}
			break;
		case otWater:			
			{		
				

				WaterPlane *water = static_cast<WaterPlane*>(mObj);
				

				//WorldArtData waData = obj->getWorldArt()->getData();
				FluidData fd;// = water->getFluidData();		
				


				fd.plane.width				= waData->waterPlaneWidth->text().toFloat();
				fd.plane.height				= waData->waterPlaneHeight->text().toFloat();
				fd.plane.xSeg				= waData->waterPlaneXSeg->text().toInt();
				fd.plane.ySeg				= waData->waterPlaneYSeg->text().toInt();
				fd.plane.up					= Ogre::StringConverter::parseVector3(waData->waterPlaneUpVector->text().toStdString());
				fd.plane.vTile				= waData->waterPlaneVTile->text().toFloat();
				fd.plane.uTile				= waData->waterPlaneUTile->text().toFloat();
				fd.plane.numTexCoordSets	= waData->waterPlaneTexCoords->text().toInt();

				fd.angularViscosity = waData->waterAngVisc->text().toFloat();
				fd.density			= waData->waterDensity->text().toFloat();
				fd.linearViscosity	= waData->waterLinVisc->text().toFloat();
				fd.sound			= waData->waterSound->text().toStdString();
				fd.material			= waData->waterMaterial->text().toStdString();

				water->setFluidData(fd);
			}
			break;
		case otStatic:
			{			
				
				StaticObject* obj = static_cast<StaticObject*>(mObj);
				WorldArtData cur = mObj->getWorldArt()->getData();
				//not sure if I should allow to change the type. if yes, then the state of the 
				//combo box should be checked in the following switch

			
				switch(cur.type)
				{
				case WT_MESH:
					//meshname, matname
					cur.filename = stData->staticMeshName->text().toStdString();
					cur.material = stData->staticMatName->text().toStdString();					
					break;
				case WT_BILLBOARD:
					//matname only
					cur.material = stData->staticMatName->text().toStdString();						
					break;
				case WT_DOTSCENE:
					//filename only
					cur.filename = stData->staticMeshName->text().toStdString();
					break;
				case WT_PARTICLESYSTEM:
					//cur.material is used, but it is in the staticMeshName field
					cur.material = stData->staticMeshName->text().toStdString();						
					break;
				case WT_PLANE:
					//material and all the plane data
					cur.material = stData->staticMatName->text().toStdString();

					


					cur.pData.normal	= Ogre::StringConverter::parseVector3(stData->planeNormal->text().toStdString());
					cur.pData.constant	= stData->planeConstant->text().toFloat();
					cur.pData.width		= stData->planeWidth->text().toFloat();
					cur.pData.height	= stData->planeHeight->text().toFloat();
					cur.pData.xSeg		= stData->planeXSeg->text().toInt();
					cur.pData.ySeg		= stData->planeYSeg->text().toInt();
					cur.pData.up		= Ogre::StringConverter::parseVector3(stData->planeUpVector->text().toStdString());
					cur.pData.vTile		= stData->planeVTile->text().toFloat();
					cur.pData.uTile		= stData->planeUTile->text().toFloat();
					cur.pData.numTexCoordSets = stData->planeTexCoords->text().toInt();

				}

				obj->setWorldArtData(cur);
				
				//bool col = obj->getCollision();
				//cur.shadow = stData->staticHasShadows->isChecked();
				

				
				//staticIsStatic: not yet used
			}
			break;
	default:
		{
		//stackedWidget->hide();
		}

	}
}
