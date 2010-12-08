#include "ContainerObject.h"
#include "Level.h"
#include "functions.h"
#include "GameChar.h"

ContainerObject::ContainerObject(TiXmlElement *node,Level *lvl, Ogre::Vector3 positionOffset)
{
	
	Ogre::String id = "";
	if(node->Attribute("id"))
	{
		id = node->Attribute("id");
	}
	Ogre::Vector3 pos = Ogre::StringConverter::parseVector3(node->Attribute("pos"))+positionOffset;
	Ogre::Vector3 scale = Ogre::Vector3::UNIT_SCALE;
	Ogre::Quaternion ornt = Ogre::Quaternion::IDENTITY;
	int SGID = 0;
			
	
	if(node->Attribute("orient"))
	{
		ornt = Ogre::StringConverter::parseQuaternion(node->Attribute("orient"));
	}	
	if(node->Attribute("scale"))
	{
		scale = Ogre::StringConverter::parseVector3(node->Attribute("scale"));
	}
	if(node->Attribute("SGID"))
	{
		SGID = Ogre::StringConverter::parseInt(node->Attribute("SGID"));
	}
	
	
	StandardApplication *app = StandardApplication::getSingletonPtr();
	gamedata_container *data = app->getContainerDataPtr(id);
	if(!data)
		throw etInvalidId;
	init(data, lvl, pos, ornt, scale,NULL,SGID);
	
	if(mBody)
	{
		Ogre::Vector3 omega = Ogre::Vector3::ZERO;
		Ogre::Vector3 velocity = Ogre::Vector3::ZERO;
		if(node->Attribute("omega"))
		{
			omega = Ogre::StringConverter::parseVector3(node->Attribute("omega"));
		}
		if(node->Attribute("velocity"))
		{
			velocity = Ogre::StringConverter::parseVector3(node->Attribute("velocity"));
		}
		mBody->setVelocity(velocity);
		mBody->setOmega(omega);
	}
	

	if(node->Attribute("inlevelfile"))
	{
		isInLevelFile = Ogre::StringConverter::parseBool(node->Attribute("inlevelfile"));
	}
	if(node->FirstChildElement("inventory"))
	{
		app->parseInventory(node->FirstChildElement("inventory"),mInv);
		
	}
}

ContainerObject::ContainerObject(ContainerObject *src, Level *lvl)
{
	SGID = 0;
    Level *L;
	if(lvl)
	{
		L = lvl;
	}
	else
	{
		L = src->mLevel;
	}
    Ogre::Vector3 pos;
	Ogre::Quaternion ornt;
    src->getPositionOrientation(pos,ornt);
    Inventory *inv = NULL;
    if(src->mInv)
        inv = new Inventory(src->mInv);
    init(static_cast<gamedata_container*>(src->mData),L,pos,ornt,src->getScale(),inv);
	
	
	

	//inventar klonen
}
//
//ContainerObject::ContainerObject(Ogre::String id, Level *lvl, Ogre::Vector3 pos, Ogre::Quaternion ornt, Ogre::Vector3 _scale, Inventory* inv)
//{
//	gamedata_container* myData = StandardApplication::getSingletonPtr()->getContainerDataPtr(id);
//	if(myData->id == "")
//	{
//		throw etInvalidId;
//	}
//	init(myData,lvl,pos,ornt,_scale,inv);
//}
void ContainerObject::init(gamedata_container *data, Level *lvl, Ogre::Vector3 pos, Ogre::Quaternion ornt, Ogre::Vector3 _scale, Inventory *inv, int _SGID)
{
	SGID = _SGID;
	mNode = NULL;
	mArt = NULL;
	mLevel = lvl;
	isInLevelFile = false;
	type = otContainer;


	mData = static_cast<gamedata*>(data);

	ID = mData->id;
	name = data->name;

	mColData = data->cData;
	baseScale = data->wData.scale;

	mArt = new WorldArt(this,data->wData,NULL,_scale);

	


	createBody(data->cData);
	
	if(data->cData.colType != CT_TREECOLLISION && data->cData.colType != CT_EXTERNAL)
		//mBody->setCustomForceAndTorqueCallback( boost::bind( &GameObject::forceCallback, this, _1 ) );  // add the previous defined callback function as the body custom force and torque callback
		mBody->setCustomForceAndTorqueCallback( boost::bind( &GameObject::forceCallback, this, _1 ) );
	
	mBody->setPositionOrientation(pos,ornt);

	
	mLevel->registerObject(this);

	if(inv)
		mInv=inv;
	else
		mInv = new Inventory(data->defaultSize);

}

ContainerObject::~ContainerObject()
{
	//mLevel->unregisterObject(this);
	//delete mBody;
	//
	//if(mArt)
	//	delete mArt;
	//destroyNode(mLevel->getSceneManager(),mNode);
}

TiXmlElement* ContainerObject::getXmlElem(Ogre::Vector3 startPosition,bool savegame)
{
	TiXmlElement* xmlElem = new TiXmlElement(objectTypeToString(this->type).c_str());
	Ogre::Vector3 pos, scale;
	Ogre::Quaternion ornt;

		
	scale = getScale();
	getPositionOrientation(pos,ornt);
		
	pos -= startPosition;
	xmlElem->SetAttribute("pos",Ogre::StringConverter::toString(pos).c_str());
	if(ornt != Ogre::Quaternion::IDENTITY)
	{
		xmlElem->SetAttribute("orient",Ogre::StringConverter::toString(ornt).c_str());
	}
	if(SGID)
	{
		xmlElem->SetAttribute("SGID",Ogre::StringConverter::toString(SGID).c_str());
	}
	if(savegame)
	{
		
		if(mBody && !mBody->isFreezed())
		{
			Ogre::Vector3 omega = mBody->getOmega();
			Ogre::Vector3 velocity = mBody->getVelocity();
			if(omega != Ogre::Vector3::ZERO)
			{
				xmlElem->SetAttribute("omega",Ogre::StringConverter::toString(omega).c_str());
			}
			if(velocity != Ogre::Vector3::ZERO)
			{
				xmlElem->SetAttribute("velocity",Ogre::StringConverter::toString(velocity).c_str());
			}
		}
	}
	
		
	xmlElem->SetAttribute("id",ID.c_str());
	if(scale != Ogre::Vector3::UNIT_SCALE)
	{
		xmlElem->SetAttribute("scale",Ogre::StringConverter::toString(scale).c_str());
	}

	ContainerObject *c = static_cast<ContainerObject*>(this);
	if(c->mInv)
	{
		TiXmlElement *invnode = new TiXmlElement( "inventory" );
		StandardApplication::getSingletonPtr()->inventoryToXml(invnode,c->mInv);
		xmlElem->LinkEndChild(invnode);
	}
	xmlElem->SetAttribute("inlevelfile",Ogre::StringConverter::toString(isInLevelFile).c_str());
	return xmlElem;
}

void ContainerObject::onActivate(GameChar *user,bool cursorMode)
{
#ifndef __editor
	GameApp *app = static_cast<GameApp*>(StandardApplication::getSingletonPtr());
	

	if(!user)
	{
        return;          
    }
	if(static_cast<gamedata_container*>(mData)->carryable)
	{
        if(!user->getBag())
        {
	        if(!cursorMode)
	        {
		        //nur wenn der char kein inventar hat, morrowind-style nehmen
                user->setBag(static_cast<gamedata_container*>(mData)->inv_item,mInv);
        		
	        }
	        else
	        {
		        //wenn man cursormode draufklickt. ERSTMAL so, dass man's nur auf´nehmen kann, wenn man kein andres hat
                app->setHoldingContainer(static_cast<gamedata_container*>(mData)->inv_item,mInv);

	        }
	        mLevel->deleteObject(this);
            return; //das return verhindert das showOtherInventory am ende
        }
        else
        {
            /*if(!mInv || mInv->getTotalCount() == 0)
            {
                //leeres inv, dh wir geben dem Spieler das Item
                if(cursorMode)
	            {
		            Inventory::invDataItem cur;
		            cur.count = 1;
		            cur.item = static_cast<gamedata_container*>(mData)->inv_item;
		            static_cast<GameApp*>(StandardApplication::getSingletonPtr())->setHoldingItem(cur);
		            //app->setholdingitem(cur);
		            mLevel->deleteObject(this);
                    return;//wieder abbrechen
		            //delete obj;
	            }
	            else
	            {
		            if(user->getInventory()->hasSpace())
		            {
                        user->getInventory()->addItem(static_cast<gamedata_container*>(mData)->inv_item->id);
                        mLevel->deleteObject(this);
                        return;//wieder abbrechen
		            }
	            }
                
            }*/
            //sonst eben durchfallen und das Ding öffnen lassen
        }
	}
	//else
	//{
	//wenn wir hier noch leben, einfach öffnen

	app->showOtherInventory(mInv,this);


	//}

#endif
}