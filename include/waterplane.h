#ifndef __waterplane
#define __waterplane

#include "GameObject.h"
#include "ogre.h"
#include "OgreNewt.h"

//#include "WaterPlaneMesh.h"



class WaterPlane: public GameObject
{
public:
    WaterPlane(WaterPlane *src,Level *targetLvl)
    {
        init(targetLvl,src->getPosition(),src->getOrientation(),src->getScale(),src->mFluidData);
    }
    WaterPlane(Level *lvl,Ogre::Vector3 position,Ogre::Quaternion orientation,Ogre::Vector3 scale = Ogre::Vector3::ZERO,Ogre::String material = "WaterMaterial")
    {
		FluidData newdata(material);
		
        init(lvl,position,orientation,scale,newdata);
    }
	WaterPlane(Level *lvl, Ogre::Vector3 position, Ogre::Quaternion orient,FluidData data, Ogre::Vector3 scale = Ogre::Vector3::ZERO)
	{
		init(lvl,position,orient,scale,data);
	}
    WaterPlane(TiXmlElement *node, Level *lvl, Ogre::Vector3 positionOffset = Ogre::Vector3::ZERO);
    ~WaterPlane();
    TiXmlElement* getXmlElem(Ogre::Vector3 startPosition = Ogre::Vector3::ZERO,bool savegame=false);
    GameObject* clone(Level *targetLvl = NULL)
    {
        if(targetLvl)
            return new WaterPlane(this,targetLvl);
        else
            return new WaterPlane(this,mLevel);
    }
	Ogre::String getDisplayName()
	{
		return "(wa)"+getMaterial();
	}
    inline Ogre::Plane getPlane()
    {
        return mPlane;
    }
    Ogre::Vector3 getScale()
    {
        return mNode->getScale()/baseScale;
    }
    //in diesem Fall aktualisiert es mPlane
    void updateBody();
	inline Ogre::String getMaterial()
	{
		return mFluidData.material;
	}
	FluidData getFluidData()
	{
		return mFluidData;
	}
	void setFluidData(FluidData newData);

    void addColliding(GameObject *obj);
    void removeColliding(GameObject *obj);
    void update(Ogre::Real time);


    void onCollision(GameObject *with,ContCallback *callback);
private:
//    WaterCircles mCircles;
	FluidData mFluidData;
    Ogre::Plane mPlane;
 
    //Ogre::Entity *mEnt;
    void init(Level *lvl,Ogre::Vector3 position,Ogre::Quaternion orientation,Ogre::Vector3 scale,FluidData data);
    //Ogre::String material;
    //alle objekte, die mit diesem Wasser kollidieren. in erster linie für den destruktor im editor wichtig
    ObjectList collidingWithMe;
    bool destructorCalled;

};

#endif