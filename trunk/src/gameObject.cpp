#include "GameObject.h"
#include "Level.h"
#include "functions.h"
#include "GameChar.h"
#include "DoorObject.h"
#include "LightObject.h"
#include "entranceObject.h"
#include "ContainerObject.h"
#include "waterplane.h"
#include "Source.h"
//
//
//GameObject::~GameObject()
//{
//	//nothing
//}
bool GameObject::getContactSoundPlaying()
{
	if(!contactSound)
		return false;
	return contactSound->isPlaying();
}
GameObject::GameObject()
{
    mWater = NULL;
    mNode = NULL;
    mBody = NULL;
	mLevel = NULL;
	mArt = NULL;
    swimming = false;
    //onWaterSurface = false;
    contactSound = NULL;
	onWaterSurfaceState = 0;
	//nothing
}
GameObject::~GameObject()
{
	if(mLevel)
		mLevel->unregisterObject(this);
    if(mWater)
        mWater->removeColliding(this);
	
	//delete subObjectList;
	//delete jointList;
	if(mBody)
	    delete mBody;
 //   if(mSnd)
	//{
	//	mSnd->stop();
	//	mLevel->destroySource(mSnd);
	//	//mNode->detachObject(mSnd);
	//	//OgreAL::SoundManager::getSingletonPtr()->destroySound(mSnd);
	//}	
	if(mArt)
    {
        //if(
        delete mArt;
    }
	if(mNode)
		destroyNode(mLevel->getSceneManager(),mNode);
}

void GameObject::playContactSound(Ogre::String filename)
{
    if(!contactSound)
        contactSound = mLevel->createSource();
    if(contactSound->isPlaying())
    {
        contactSound->stop();
    }
    SimpleSound::Buffer *buf = mLevel->getBuffer(filename);
    contactSound->setBuffer(buf);
    contactSound->play();
}

void GameObject::update(Ogre::Real time)
{
    if(!contactSound)
        return;

    contactSound->setPosition(getPosition());
    contactSound->setOrientation(getOrientation());
    if(!contactSound->isPlaying())
    {
        mLevel->destroySource(contactSound);
        contactSound = NULL;
    }
};



void GameObject::forceCallback( OgreNewt::Body* me )
{
	
    
    Ogre::Vector3 intertia;
    Ogre::Real mass;
    Ogre::Real gravity = mLevel->getGravity();
    me->getMassMatrix(mass,intertia);
    
    me->addForce(Ogre::Vector3(0,gravity*mass,0));
    //jetzt auftriebskraft
    //GameObject *obj = static_cast<GameObject *>(bod->getUserData());
   // mLog("forceCB, onWaterSurface="+str(isOnWaterSurface()));
    
    if(isInWater())
    {
		FluidData fd = mWater->getFluidData();
        //mDebugText("adding buoyancy");
		me->addBouyancyForce<GameObject>( fd.density, fd.linearViscosity, fd.angularViscosity, Ogre::Vector3(0.0f,gravity,0.0f), &GameObject::buoyancyCallback, this );
        
    }
    //onWaterSurface = false;
	if(onWaterSurfaceState)
		onWaterSurfaceState--;
    
}

bool GameObject::isOnWaterSurface()
{
    if(!mWater)
    {
        //onWaterSurface = false;
        return false;
    }
    //Ogre::AxisAlignedBox box = getCollisionAABB();//wenn der Abstand zwischen ebene und mir größer ist als meine
    //return onWaterSurface;
	return onWaterSurfaceState > 0;
    /*if(!onWaterSurface)
    {
        //onWaterSurface = false;
        return false;
    }
    
    return true;*/
}

bool GameObject::isInWater()
{
    if(!mWater)
            return false;
    
    if(!isOnWaterSurface())
    {            
        return ( mWater->getPlane().getDistance(getPosition()) < 0 );
    }
    else
        return true;
    //return (mWater != NULL || swimming);
}

void GameObject::collisionWithWater(WaterPlane *water)
{
    if(mWater != water)
    {
        if(mWater)
            mWater->removeColliding(this);
        if(water)
            water->addColliding(this);
    }
    mWater = water;
	//mLog("collisionWithWater, onWaterSurface="+str(isOnWaterSurface()));
    if( mWater && !isOnWaterSurface() /*&& !notwater->isInWater()*/ && !getContactSoundPlaying() )
    {
        //mLog("playing snd");
        //sound spielen
        //Ogre::Vector3 cpos,cnorm;
        //getContactPositionAndNormal(cpos,cnorm);
		playContactSound(mWater->getFluidData().sound);
        //water->getLevel()->playSound(cpos,"waterSPLASH.WAV");
    }
    //mLog("watercollision "+ID);
    
    
    swimming = true;
    //onWaterSurface = true;
	onWaterSurfaceState = 2;
    
}
bool GameObject::buoyancyCallback( int colID, OgreNewt::Body* me, const Ogre::Quaternion& orient, const Ogre::Vector3& pos, Ogre::Plane& plane )
{    
	//mLog("buoyancyCB, onWaterSurface="+str(isOnWaterSurface()));
    if(mWater)
    {
        plane = mWater->getPlane();
        //mLevel->debugShowPlane(plane);
    }
	//onWaterSurface = false;
    return true;
}

//
//void GameObject::update(Ogre::Real time)
//{
//	//do something, like ai->update(time), or mAnimSTate->addtime...
//}
//
void GameObject::setPosition(Ogre::Vector3 pos)
{
	if(mBody)
	{
		Ogre::Quaternion ornt;
		Ogre::Vector3 oldPos;
		mBody->getPositionOrientation(oldPos,ornt);
		mBody->setPositionOrientation(pos,ornt);
	}
	else if(mNode)
	{
		mNode->setPosition(pos);
	}
}
//
void GameObject::setOrientation(Ogre::Quaternion ornt)
{
	if(mBody)
	{
		Ogre::Quaternion oldOrnt;
		Ogre::Vector3 pos;
		mBody->getPositionOrientation(pos,oldOrnt);
		mBody->setPositionOrientation(pos,ornt);
	}
	else if(mNode)
	{
		mNode->setOrientation(ornt);
	}

}

void GameObject::setPositionOrientation(Ogre::Vector3 pos,Ogre::Quaternion ornt)
{
	if(mBody)
	{
		mBody->setPositionOrientation(pos,ornt);
	}
	else if(mNode)
	{
		mNode->setOrientation(ornt);
		mNode->setPosition(pos);
	}

}


void GameObject::rotate(const Ogre::Vector3& axis, const Ogre::Radian& angle, Ogre::Node::TransformSpace relativeTo)
{
	Ogre::Quaternion q;
	q.FromAngleAxis(angle,axis);
	if(mBody)
	{
		
		rotate(q, relativeTo);
	}
	else if(mNode)
	{
		mNode->rotate(q, relativeTo);
	}
}



void GameObject::rotate(const Ogre::Quaternion& q, Ogre::Node::TransformSpace relativeTo)
{
	if(mBody)
	{
		Ogre::Quaternion oldOrnt;
		Ogre::Vector3 pos;
		Ogre::Quaternion newOrnt;
		mBody->getPositionOrientation(pos,oldOrnt);
		switch(relativeTo)
        {
		case Ogre::Node::TS_WORLD:
        case Ogre::Node::TS_PARENT:
            // Rotations are normally relative to local axes, transform up
            newOrnt = q * oldOrnt;
            break;
   //     case Ogre::Node::TS_WORLD:
   //         // Rotations are normally relative to local axes, transform up
			////das SOLLTE eigentlich das gleiche bewirken wie TS_LOCAL, da gameObjects doch eigentlich 
			////keine kinder sein sollten... erst recht wenns hier um bodies geht! 
   //         newOrnt = oldOrnt * oldOrnt.Inverse()
   //             * q * oldOrnt;
   //         break;
        case Ogre::Node::TS_LOCAL:
            // Note the order of the mult, i.e. q comes after
            newOrnt = oldOrnt * q;
            break;
        }
		
		mBody->setPositionOrientation(pos,newOrnt);
	}
	else if(mNode)
	{
		//mNode->setOrientation(ornt);
		mNode->rotate(q,relativeTo);
	}

}

//
Ogre::Vector3 GameObject::getPosition()
{
	if(mBody)
	{
		Ogre::Quaternion ornt;
		Ogre::Vector3 pos;
		mBody->getPositionOrientation(pos,ornt);
		return pos;
	}
	else if(mNode)
	{
		return mNode->getPosition();
	}
	else
	{
		return Ogre::Vector3::ZERO;
	}
}

bool GameObject::getPositionOrientation(Ogre::Vector3 &pos, Ogre::Quaternion &orient)
{
	pos = Ogre::Vector3::ZERO;
	orient = Ogre::Quaternion::IDENTITY;
	if(mBody)
	{		
		mBody->getPositionOrientation(pos,orient);
		return true;
	}
	else if(mNode)
	{
		pos = mNode->getPosition();
		orient = mNode->getOrientation();
		return true;
	}
	else
	{
		return false;
	}
}


Ogre::Quaternion GameObject::getOrientation()
{
	if(mBody)
	{
		Ogre::Quaternion ornt;
		Ogre::Vector3 pos;
		mBody->getPositionOrientation(pos,ornt);
		return ornt;
	}
	else if(mNode)
	{
		return mNode->getOrientation();
	}
	else
	{
		return Ogre::Quaternion::IDENTITY;
	}
}

OgreNewt::Body *GameObject::getBody()
{
	return mBody;
}
Ogre::SceneNode *GameObject::getSceneNode()
{
	return mNode;
}
//pointer to the level this obj is currently in
Level *GameObject::getLevel()
{
	return mLevel;
}
//void GameObject::destroyNode()
//{
//	mNode->removeAndDestroyAllChildren();
//	while(mNode->numAttachedObjects())
//	{
//		
//		MovableObject *m = mNode->detachObject((unsigned short)0);
//		mLevel->getSceneManager()->destroyMovableObject(m);
//		
//	}
//	
//	mLevel->getSceneManager()->destroySceneNode(mNode->getName());
//	mNode = NULL;
//}



//
//OgreNewt::Body *GameObject::makeBody(short colType, Ogre::Real mass, Ogre::Vector3 CollisionParams, Ogre::SceneNode *nod)
//{
//	OgreNewt::Collision *col = 0;
//	OgreNewt::World *mWorld = mLevel->getONWorld();
//	Ogre::Vector3 inertia;
//	Ogre::Real temp;
//	if(mass > 0)
//	{
//
//		switch(colType)
//		{
//		case CT_BOX:
//			col = new OgreNewt::CollisionPrimitives::Box(mWorld,CollisionParams);
//			inertia = OgreNewt::MomentOfInertia::CalcBoxSolid(mass,CollisionParams);
//			break;
//		case CT_CAPSULE:
//			col = new OgreNewt::CollisionPrimitives::Capsule(mWorld,CollisionParams.x,CollisionParams.y);
//			inertia = OgreNewt::MomentOfInertia::CalcCylinderSolid(mass,CollisionParams.x,CollisionParams.y);
//			break;
//		case CT_CHAMFERCYLINDER:
//			col = new OgreNewt::CollisionPrimitives::ChamferCylinder(mWorld,CollisionParams.x,CollisionParams.y);
//			inertia = OgreNewt::MomentOfInertia::CalcCylinderSolid(mass,CollisionParams.x,CollisionParams.y);
//			break;
//		case CT_CONE:
//			col = new OgreNewt::CollisionPrimitives::Cone(mWorld,CollisionParams.x,CollisionParams.y);
//			inertia = OgreNewt::MomentOfInertia::CalcCylinderSolid(mass,CollisionParams.x,CollisionParams.y);
//			break;
//		case CT_CONVEXHULL:
//			col = new OgreNewt::CollisionPrimitives::ConvexHull(mWorld,nod);
//			temp = nod->getAttachedObject(0)->getBoundingRadius();
//			//!!Das ist ne zwischenlösung!!!
//			inertia = OgreNewt::MomentOfInertia::CalcEllipsoidSolid(mass,Ogre::Vector3(temp,temp,temp));
//			break;
//		case CT_CYLINDER:
//			col = new OgreNewt::CollisionPrimitives::Cylinder(mWorld,CollisionParams.x,CollisionParams.y);
//			inertia = OgreNewt::MomentOfInertia::CalcCylinderSolid(mass,CollisionParams.x,CollisionParams.y);
//
//			break;
//		case CT_ELLIPSOID:
//			col = new OgreNewt::CollisionPrimitives::Ellipsoid(mWorld,CollisionParams);
//			inertia = OgreNewt::MomentOfInertia::CalcEllipsoidSolid(mass,CollisionParams);
//
//			break;
//		case CT_PYRAMID:
//			col = new OgreNewt::CollisionPrimitives::Pyramid(mWorld,CollisionParams);
//			inertia = OgreNewt::MomentOfInertia::CalcCylinderSolid(mass,CollisionParams.x,CollisionParams.y);
//
//			break;
//		case CT_TREECOLLISION:
//			col = new OgreNewt::CollisionPrimitives::TreeCollision(mWorld,nod,false);
//			break;
//
//		/*default:
//			goOn = false;*/
//
//		};
//		if(!col)
//		{
//			//log("col == 0!!!");
//		}
//	}//-------------ENDE von if(cur_obj.mass > 0)
//	else
//	{
//		//also wenn es null ist -_-
//		col = new OgreNewt::CollisionPrimitives::TreeCollision(mWorld,nod,false);
////		log("kk, treecollision");
//		//me n00b, lol...
//	}
//	if(col)
//	{
//		OgreNewt::Body *bod = new OgreNewt::Body(mWorld,col);
////		log("body created");
//		if(colType != CT_TREECOLLISION)
//		{
//			//log("mass > 0");
//			bod->setMassMatrix(mass,inertia);
//			//bod->setType(BT_MOVABLE);
//			bod->setStandardForceCallback();
//
//		}
//		else
//		{
//			//log("mass = 0");
//			//bod->setType(BT_STATIC);
//		}
//		bod->attachToNode(nod);
//		delete col;
//		return bod;
//	}
//	else
//	{
//		return false;
//	}
//}

//Ogre::String GameObject::getUName()
//{
//	return uName;
//}
//Ogre::String GameObject::getID()
//{
//	return ID;
//}
//Ogre::String GameObject::getName()
//{
//	return "";
////}
//void GameObject::createWorldArt(WorldArtData d,Ogre::SceneNode *nod)
//{
//
//	//mArt = new WorldArt(this,nod);
//	switch(d.type)
//	{
//	case WT_BILLBOARD:
//		mArt = new WorldArt(this,d.material,d.scale.x,d.scale.y,d.bbColor,d.positionOffset,nod);
//		break;
//	case WT_DOTSCENE:
//		mArt = new WorldArt(this,d.filename,d.scale,d.positionOffset,d.orientationOffset,nod);
//		break;
//	case WT_MESH:
//		mArt = new WorldArt(this,d.filename,d.material,d.scale,d.positionOffset,d.orientationOffset,nod);
//
//	}
//}

OgreNewt::CollisionPtr GameObject::createCollision(CollisionData d,Ogre::Vector3 &inertia)
{
	

	OgreNewt::Collision *col = NULL;//this is for the actual pointer
	OgreNewt::World *mWorld = mLevel->getWorld();

	//TEST
	Ogre::Vector3 centerOfMass;

	bool plane = false;
	if(mArt && (mArt->getType() == WT_PLANE && d.colType == CT_TREECOLLISION))
	{
		//pöhser hack
		plane = true;
		d.colType = CT_BOX;
		PlaneData pl = mArt->getData().pData;
		d.collistionParams.x = pl.width;
		d.collistionParams.y = pl.height;
		d.collistionParams.z = 0.1;
		d.positionOffset = Ogre::Vector3(0,0,0.05);
		d.orientationOffset = Ogre::Vector3::UNIT_Z.getRotationTo(pl.normal);
		
	}

	Ogre::Vector3 scaledParams = getScale()*d.collistionParams;
	Ogre::Vector3 scaledOffset = getScale()*d.positionOffset;
	if(plane)
	{
		//skalierung der Höhe zurücksetzen
		scaledParams.z = 0.1;
		scaledOffset.z = 0.05;
	}
	
	switch(d.colType)
	{
	case CT_BOX:
		col = new OgreNewt::CollisionPrimitives::Box(mWorld,scaledParams,0,d.orientationOffset,scaledOffset);
		((OgreNewt::CollisionPrimitives::Box*)col)->calculateInertialMatrix(inertia,centerOfMass);
		//inertia = OgreNewt::MomentOfInertia::CalcBoxSolid(d.mass,scaledParams);
		break;
	case CT_CAPSULE:
		col = new OgreNewt::CollisionPrimitives::Capsule(mWorld,scaledParams.x,scaledParams.y,0,d.orientationOffset,scaledOffset);
		((OgreNewt::CollisionPrimitives::Capsule*)col)->calculateInertialMatrix(inertia,centerOfMass);
		//inertia = OgreNewt::MomentOfInertia::CalcCylinderSolid(d.mass,scaledParams.x,scaledParams.y);
		break;
	case CT_CHAMFERCYLINDER:
		col = new OgreNewt::CollisionPrimitives::ChamferCylinder(mWorld,scaledParams.x,scaledParams.y,0,d.orientationOffset,scaledOffset);
		((OgreNewt::CollisionPrimitives::ChamferCylinder*)col)->calculateInertialMatrix(inertia,centerOfMass);
		//inertia = OgreNewt::MomentOfInertia::CalcCylinderSolid(d.mass,scaledParams.x,scaledParams.y);
		break;
	case CT_CONE:
		col = new OgreNewt::CollisionPrimitives::Cone(mWorld,scaledParams.x,scaledParams.y,0,d.orientationOffset,scaledOffset);
		((OgreNewt::CollisionPrimitives::Cone*)col)->calculateInertialMatrix(inertia,centerOfMass);
		//inertia = OgreNewt::MomentOfInertia::CalcCylinderSolid(d.mass,scaledParams.x,scaledParams.y);
		break;
	case CT_CONVEXHULL:
		{			
			/*		WT_BILLBOARD,
		WT_DOTSCENE,
		WT_PLANE,
		WT_PARTICLESYSTEM,
		WT_BILLBOARDCHAIN,
		WT_RIBBONTRAIL*/
			if(mArt->getType() == WT_MESH || mArt->getType() == WT_PLANE)
			{
				Ogre::MovableObject *obj = mArt->getSceneNode()->getAttachedObject(0);
				if(obj->getMovableType() == "Entity")
				{
					Ogre::Entity *ent = (Ogre::Entity*)obj;
					col = new OgreNewt::CollisionPrimitives::ConvexHull(mWorld,ent,0);
					((OgreNewt::CollisionPrimitives::ConvexHull*)col)->calculateInertialMatrix(inertia,centerOfMass);
				}
				
			}
			else
			{
				mLog("Error while creating Collision for Object "+ID+": ConvexHull supported for meshes only right now.");
			}
			//col = new OgreNewt::CollisionPrimitives::ConvexHull(mWorld);//,mNode);
			////Ogre::Real temp = mNode->getAttachedObject(0)->getBoundingRadius();
			////((OgreNewt::CollisionPrimitives::ConvexHull*)col)->
			//((OgreNewt::CollisionPrimitives::ConvexHull*)col)->calculateInertialMatrix(inertia,centerOfMass);
			////!!Das ist ne zwischenlösung!!!
			//inertia = OgreNewt::MomentOfInertia::CalcEllipsoidSolid(d.mass,Ogre::Vector3(temp));
			///*inertia = OgreNewt::MomentOfInertia::CalcEllipsoidSolid(mass,CollisionParams);*/
		}
		break;
	case CT_CYLINDER:
		col = new OgreNewt::CollisionPrimitives::Cylinder(mWorld,scaledParams.x,scaledParams.y,0,d.orientationOffset,scaledOffset);
		//inertia = OgreNewt::MomentOfInertia::CalcCylinderSolid(d.mass,scaledParams.x,scaledParams.y);
		((OgreNewt::CollisionPrimitives::Cylinder*)col)->calculateInertialMatrix(inertia,centerOfMass);
		break;
	case CT_ELLIPSOID:
		col = new OgreNewt::CollisionPrimitives::Ellipsoid(mWorld,scaledParams,0,d.orientationOffset,scaledOffset);
		//inertia = OgreNewt::MomentOfInertia::CalcEllipsoidSolid(d.mass,scaledParams);
		((OgreNewt::CollisionPrimitives::Ellipsoid*)col)->calculateInertialMatrix(inertia,centerOfMass);

		break;
	case CT_PYRAMID:
		col = new OgreNewt::CollisionPrimitives::Pyramid(mWorld,scaledParams,0,d.orientationOffset,scaledOffset);
		//inertia = OgreNewt::MomentOfInertia::CalcCylinderSolid(d.mass,scaledParams.x,scaledParams.y);
		((OgreNewt::CollisionPrimitives::Pyramid*)col)->calculateInertialMatrix(inertia,centerOfMass);

		break;	
		
	/*case CT_TREECOLLISIONSCENE:
		col = col = mArt->getTreeCollision(true);
		break;*/
	case CT_COMPOUND:
		mLog("Error while creating Collision for Object "+ID+": Compound Collisions are not implemented yet.");
		//assert(false);
		break;
	case CT_EXTERNAL:
		{
			mLog("Error while creating Collision for Object "+ID+": External (Serialized) Collisions are not implemented yet.");
			//Ogre::ResourceGroupManager *rgm = Ogre::ResourceGroupManager::getSingletonPtr();
			//OgreNewt::TreeCollisionSerializer* serializer=new OgreNewt::TreeCollisionSerializer();
			//if(rgm->resourceExists("Collision",d.collisionFile))
			//{
			//	//ok, col kann geladen werden
			//	Ogre::DataStreamPtr ptr = rgm->openResource(d.collisionFile,"Collision");
			//	col=new OgreNewt::CollisionPrimitives::TreeCollision(mWorld);
			//	//FILE* file=fopen("SomeFileName.collision","rb");
			//	//Ogre::DataStreamPtr ptr(new FileHandleDataStream(file));
			//	ptr.setNull();
			//	serializer->importTreeCollision(ptr,(OgreNewt::CollisionPrimitives::TreeCollision*)col);				
			//}
//			mass = 0;
		}
		break;
	case CT_TREECOLLISION:
	default:
//		mass = 0;
		return mArt->getTreeCollision();
		break;
	};
	return OgreNewt::CollisionPtr(col);
}

OgreNewt::Joint * GameObject::createJoint(OgreNewt::Body *parent,OgreNewt::Body *child,joint_data data)
{
	Ogre::Vector3 scale = getScale();
	Ogre::Vector3 pPos;
	Ogre::Quaternion pOrnt;
	parent->getPositionOrientation(pPos,pOrnt);
	OgreNewt::World *mWorld = mLevel->getWorld();
	OgreNewt::Joint *j = NULL;
	data.pos = (pOrnt*data.pos)*scale;
	data.pin0= pOrnt*data.pin0;
	data.pin1 = pOrnt*data.pin1;
	switch(data.type)
	{
	case JT_BALL_AND_SOCKET:
		j = new OgreNewt::BallAndSocket(child,parent,data.pos+pPos);
		break;
	case JT_HINGLE:
		j = new OgreNewt::Hinge(child,parent,data.pos+pPos,data.pin0);
		break;
	case JT_SLIDER:
		j = new OgreNewt::Slider(child,parent,data.pos+pPos,data.pin0);
		break;
	/*case JT_UNIVERSAL:

		j = new OgreNewt::Universal(child,parent,data.pos+pPos,data.pin0,data.pin1);
		break;*/
	}
	return j;
}

void GameObject::createBody(CollisionData d,bool mustHaveBody)
{
	//mLog("erstelle body");
	mBody = NULL;
	
	if(mArt && mArt->getType() != WT_DOTSCENE && mArt->getType() != WT_MESH && mArt->getType() != WT_PLANE && d.colType == CT_TREECOLLISION) 
	{
		if(mustHaveBody)
		{
			d.colType = CT_BOX;
			d.collistionParams = Ogre::Vector3::UNIT_SCALE;
		}
		else
		{
			
			
			return;
		}
	}

	

	if(d.colType == CT_NOCOLLISION)
	{
        if(mustHaveBody && (mArt->getType() == WT_DOTSCENE || mArt->getType() == WT_MESH || mArt->getType() == WT_PLANE))
		{
			d.colType = CT_TREECOLLISION;
		}
		else
			return;
	}
	OgreNewt::World *mWorld = mLevel->getWorld();
	OgreNewt::CollisionPtr col;
	Ogre::Vector3 inertia;
	Ogre::Real mass = d.mass;
	col = createCollision(d,inertia);
	if(!col)
	{
		if(mustHaveBody)
			mLog("No body could be created for Object "+ID);
		return;
	}
	mBody = new OgreNewt::Body(mWorld,col,type);
	if(mass != 0)
	{
		mBody->setMassMatrix(mass,inertia);
		if(d.upVector != Ogre::Vector3::ZERO)
		{
			OgreNewt::UpVector* uv2 = new OgreNewt::UpVector(mBody,d.upVector); // create an upvector. This forces the body to rotate just in the X and Z axes.
	
		}
	}
	mBody->setUserData(Ogre::Any((GameObject*)this));
	mBody->attachNode(mNode);
	collisionAABB = col->getAABB();
	//delete col;
}

TiXmlElement* GameObject::getXmlElem(Ogre::Vector3 startPosition,bool savegame)
{
//	//ObjType type = obj->getType();
//	Ogre::String strtype = objectTypeToString(type);
//	if(strtype == "")
//		return NULL;
//	TiXmlElement* xmlElem = new TiXmlElement(strtype.c_str());
//	Ogre::Vector3 pos, scale;
//	Ogre::Quaternion ornt;
////	Ogre::String id = obj->getID(); = ID
//		
//	scale = getScale();
//	getPositionOrientation(pos,ornt);
//		
//	pos -= startPosition;
//	xmlElem->SetAttribute("pos",Ogre::StringConverter::toString(pos).c_str());
//	if(ornt != Ogre::Quaternion::IDENTITY)
//	{
//		xmlElem->SetAttribute("orient",Ogre::StringConverter::toString(ornt).c_str());
//	}
//	if(SGID)
//	{
//		xmlElem->SetAttribute("SGID",Ogre::StringConverter::toString(SGID).c_str());
//	}
//	
//		//scale = nod->getScale();
//	
//	switch(type)
//	{
//	
//	case otItem:
//		{
//			//gamedata_item it = app->getItemData(obj->getID());
//			if(scale != Ogre::Vector3::UNIT_SCALE)
//			{
//				xmlElem->SetAttribute("scale",Ogre::StringConverter::toString(scale).c_str());
//			}
//			xmlElem->SetAttribute("id",ID.c_str());
//
//		}			
//		break;
//	case otDynamic:
//		{
//			//gamedata_dynamic ob = app->getDynamicData(obj->getID());
//			if(scale != Ogre::Vector3::UNIT_SCALE)
//			{
//				xmlElem->SetAttribute("scale",Ogre::StringConverter::toString(scale).c_str());
//			}
//			xmlElem->SetAttribute("id",ID.c_str());
//
//		}			
//		break;
//	case otChar:
//		{
//			//gamedata_char ch = app->getCharData(obj->getID());
//			/*if(ch.scale != scale)
//			{
//				xmlElem->SetAttribute("scale",Ogre::StringConverter::toString(scale).c_str());
//			}*/
//			xmlElem->SetAttribute("id",ID.c_str());
//			GameChar *c = (GameChar*)(this);
//			TiXmlElement *eqnode = new TiXmlElement( "equipment" );
//			eqnode->SetAttribute("melee",c->getMeleeId().c_str());
//			eqnode->SetAttribute("armor",c->getArmorId().c_str());
//			eqnode->SetAttribute("ranged",c->getRangedId().c_str());
//			eqnode->SetAttribute("shield",c->getShieldId().c_str());
//			eqnode->SetAttribute("else1",c->getElse1Id().c_str());
//			eqnode->SetAttribute("else2",c->getElse2Id().c_str());
//			eqnode->SetAttribute("else3",c->getElse3Id().c_str());
//			eqnode->SetAttribute("else4",c->getElse4Id().c_str());
//			eqnode->SetAttribute("bag",c->getBagId().c_str());
//			xmlElem->LinkEndChild(eqnode);
//			if(c->getInventory())
//			{
//				TiXmlElement *invnode = new TiXmlElement( "inventory" );
//				StandardApplication::getSingletonPtr()->inventoryToXml(invnode,c->getInventory());
//				xmlElem->LinkEndChild(invnode);
//			}
//
//			
//
//		}			
//		break;
//	case otDoor:
//		{
//			//gamedata_door ac = app->getDoorData(obj->getID());
//			xmlElem->SetAttribute("id",ID.c_str());
//			if(scale != Ogre::Vector3::UNIT_SCALE)
//			{
//				xmlElem->SetAttribute("scale",Ogre::StringConverter::toString(scale).c_str());
//			}
//			DoorData ad = static_cast<DoorObject*>(this)->getDoorData();
//			xmlElem->SetAttribute("targetLevel",ad.targetLevel.c_str());
//			xmlElem->SetAttribute("targetEntrance",ad.targetEntrance.c_str());
//
//		}			
//		break;
//	case otLight:
//		{
//			LightObject *lo = (LightObject*)this;
//			Ogre::Light *light = lo->getLight();
//			Ogre::String strtype = "point";
//			
//			switch(light->getType())
//			{					
//			case Ogre::Light::LT_POINT:
//				strtype = "point";
//				break;
//			case Ogre::Light::LT_DIRECTIONAL:
//				strtype = "directional";
//				break;
//			case Ogre::Light::LT_SPOTLIGHT:
//				strtype = "spotlight";
//				break;
//			}
//			xmlElem->SetValue("light");
//			xmlElem->SetAttribute("type",strtype.c_str());
//			
//			
//			Ogre::ColourValue diffuse, specular;
//			diffuse = light->getDiffuseColour();
//			specular = light->getSpecularColour();
//			xmlElem->SetAttribute("diffuse",Ogre::StringConverter::toString(diffuse).c_str());
//			xmlElem->SetAttribute("specular",Ogre::StringConverter::toString(specular).c_str());
//			
//			Ogre::Real aRange  = light->getAttenuationRange();
//			Ogre::Real aConst  = light->getAttenuationConstant();
//			Ogre::Real aLinear = light->getAttenuationLinear();
//			Ogre::Real aQuad   = light->getAttenuationQuadric();
//
//			xmlElem->SetAttribute("aRange",Ogre::StringConverter::toString(aRange).c_str());
//			xmlElem->SetAttribute("aConst",Ogre::StringConverter::toString(aConst).c_str());
//			xmlElem->SetAttribute("aLinear",Ogre::StringConverter::toString(aLinear).c_str());
//			xmlElem->SetAttribute("aQuad",Ogre::StringConverter::toString(aQuad).c_str());
//
//			if(light->getType() == Ogre::Light::LT_SPOTLIGHT)
//			{
//				Ogre::Radian sInner = light->getSpotlightInnerAngle();
//				Ogre::Radian sOuter = light->getSpotlightOuterAngle();
//				Ogre::Real falloff  = light->getSpotlightFalloff();
//
//				xmlElem->SetAttribute("sInner",Ogre::StringConverter::toString(sInner).c_str());
//				xmlElem->SetAttribute("sOuter",Ogre::StringConverter::toString(sOuter).c_str());
//				xmlElem->SetAttribute("falloff",Ogre::StringConverter::toString(falloff).c_str());
//
//
//			}
//
//		
//			
//		}
//		break;
//#ifdef __editor
//	case otEntrance:
//		{
//			mLog("speichere entrance");
//			entranceObject *eo = (entranceObject*)this;
//			Ogre::String name = eo->getName();
//			xmlElem->SetValue("entrance");
//			xmlElem->SetAttribute("name",name.c_str());
//		}
//		break;
//#endif
//	case otContainer:
//		{
//			//xmlElem->SetValue("dotScene");
//			xmlElem->SetAttribute("id",ID.c_str());
//			if(scale != Ogre::Vector3::UNIT_SCALE)
//			{
//				xmlElem->SetAttribute("scale",Ogre::StringConverter::toString(scale).c_str());
//			}
//
//			ContainerObject *c = static_cast<ContainerObject*>(this);
//			if(c->mInv)
//			{
//				TiXmlElement *invnode = new TiXmlElement( "inventory" );
//				StandardApplication::getSingletonPtr()->inventoryToXml(invnode,c->mInv);
//				xmlElem->LinkEndChild(invnode);
//			}
//		}
//		break;
//	case otStatic:
//		{
////				!le compile preventer
//		
//			//xmlElem->SetValue("static");
//			
//			StandardApplication::getSingletonPtr()->worldArtToXml(mArt->getData(),xmlElem);//->SetValue(type.c_str());
//			if(scale != Ogre::Vector3::UNIT_SCALE)
//			{
//				xmlElem->SetAttribute("scale",Ogre::StringConverter::toString(scale).c_str());
//			}
//			xmlElem->SetValue("static");
//			
//
//		}
//		break;
//	case otMissile:
//		{
//		}
//		break;
//	case otExplosion:
//		{
//		}
//		break;
//	default:
//		return NULL;
//		
//	}
//	return xmlElem;
	return NULL;
}

void GameObject::updateBody()
{
	if(mBody)
	{
		Ogre::Vector3 inertia;
		Ogre::Real mass;
		mBody->getMassMatrix(mass,inertia);
		OgreNewt::CollisionPtr col = createCollision(mColData,inertia);
		mBody->setCollision(col);
		mBody->setMassMatrix(mass,inertia);
	}
}

Ogre::String GameObject::getDebugInfo()
{		
	return "Object "+ptrAsStr()+", type="+objectTypeToString(getType())+", ID="+ID;
}