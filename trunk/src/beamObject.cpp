#include "beamObject.h"
#include "GameChar.h"
#include "functions.h"
#include "Level.h"
#include "ExplosionObject.h"

void BeamObject::init(ObjectPtr _caster, gamedata_damagedealer *_source, Level *lvl,int SaveGameID)
{
	
	isInLevelFile = false;
	SGID = SaveGameID;
	lifetime = 0;
	mNode = NULL;
	mArt = NULL;
	mLevel = lvl;
//	mSnd = NULL;
	type = otBeam;
	mBody = NULL;
	caster = ObjectPtr(_caster);
	source = _source;
	
	if(!source->Beam)
	{
		mLog("Could not construct Beam, because item/spell "+source->id+"has no BeamData");
		throw EX_CONSTR_FAILED;
	}
    
	//mWidth = source->Beam->width
	//mData = data;
	Ogre::SceneManager *mSceneMgr = mLevel->getSceneManager();
	
   
    //bbChain->set
    
    
    /*Ogre::String mesh = mData.mesh;
	Ogre::String material = mData.material;*/
	//baseScale = Ogre::Vector3(0.5);
	//hasCollided = false;
	ID = "beam";
	//mNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();


	mArt = NULL;
	//mArt = new WorldArt(this,temp,NULL);
	mNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();//mArt->getSceneNode();
    //bbchain BEGIN

    bbChain = mSceneMgr->createBillboardChain("BeamBillBoardChain"+ptrToString(this));
    bbChain->setMaterialName(source->Beam->material);
    bbChain->setNumberOfChains(1);
    bbChain->setDynamic(true);
    int maxElems = Ogre::Math::ICeil(source->Beam->maxRange/source->Beam->texCoord)*2;//ist mir nun wayne obs zu viel ist
    bbChain->setMaxChainElements(maxElems);
    
    bbChain->addChainElement(0,Ogre::BillboardChain::Element(Ogre::Vector3::ZERO,source->Beam->width,0,Ogre::ColourValue(0,0,0,0)));
    /*

    bbChain->addChainElement(0,Ogre::BillboardChain::Element(Ogre::Vector3(0,0,-5),2,1,Ogre::ColourValue::White));
    bbChain->addChainElement(0,Ogre::BillboardChain::Element(Ogre::Vector3(0,0,-10),2,2,Ogre::ColourValue::White));
    bbChain->addChainElement(0,Ogre::BillboardChain::Element(Ogre::Vector3(0,0,-15),2,3,Ogre::ColourValue::White));
    */
    mNode->attachObject(bbChain);
    //bbCHain END
	////manualObj BEGIN
	//manObj = mSceneMgr->createManualObject("ManObj"+ptrToString(this));
	//manObj->begin(source->Beam->material);//,RenderOperation::OT_LINE_LIST);
	//manObj->setDynamic(true);
	////   | -z
	//// |   |
	////-4---3-
	//// | \ |
	////-1---2-
	//// |   |
	////   |_  x
	//manObj->position(-0.5,0,1);//4
	//manObj->textureCoord(0,1);	

	//manObj->position(0.5,0,0);//2
	//manObj->textureCoord(1,0);

	//manObj->position(-0.5,0,0);//1
	//manObj->textureCoord(0,0);

	//
	//manObj->end();
	//mNode->attachObject(manObj);
	////manualObj END
	Ogre::Vector3 pos = caster->getPosition();
	setPosition(pos);

//
	//jetzt sound
	mSnd = NULL;
	if(source->Beam->sound != "")
	{

		mSnd = mLevel->createSource(NULL,stSfxLevel,true);
		if(mSnd)
		{
			mSnd->setBuffer(mLevel->getBuffer(source->Beam->sound ));
			mSnd->setPosition(pos);
			mSnd->play();
			//mSnd->setLoop(true);
		}


	}
	mEffect = NULL;
	if(source->Explosion)
	{
        mEffect = new ExplosionObject(caster.obj,NULL,source,mLevel,pos,true,true);
	}
	mLevel->registerObject(this);
	lastLength = 0;
    currentNumSections = 0;
	//setLength(3);
}

void BeamObject::update(Ogre::Real time)
{
	//return;
	Ogre::Vector3 playerPos;
	Ogre::Quaternion playerOrnt;
	caster->getPositionOrientation(playerPos,playerOrnt);
	if(mSnd)
		mSnd->setPosition(playerPos);
	Ogre::Vector3 target;
	Ogre::Real dist;
	Ogre::Quaternion ornt;
    static_cast<GameChar*>(caster.obj)->getTargetData(ornt,target,dist);
    //ornt = ornt.Inverse();
    ornt = static_cast<GameChar*>(caster.obj)->getFiringOrientation();
	//Ogre::Vector3 mPos = (target+playerPos)/2;//target/2-playerPos/2+playePos=target/2+playerPos/2
	setPositionOrientation(playerPos,ornt);
	setLength(dist);
	if(mEffect)
	{
		mEffect->setPositionOrientation(target,playerOrnt);
	}
	lifetime += time;
	if (lifetime >= source->Beam->lifespan )
		mLevel->deleteObject(this);
}

void BeamObject::setLength(Ogre::Real length)
{
	
	if(source->Beam->maxRange < length)
        length = source->Beam->maxRange;
    if(Ogre::Math::Abs(lastLength-length) < 0.01)
		return;
    lastLength = length;
    
    Ogre::Real mSize = source->Beam->width;
    Ogre::Real texSize = source->Beam->texCoord;
    //anzahl der Segmente, ohne dem (immer vorhandene) Nullten oder dem Letzten
    int nrOfSections = Ogre::Math::IFloor(length/texSize);


    //if(nrOfSections < currentNumSections)
    //{
        //verkleinern
        //ich hau nun erstmal alles weg
        bbChain->clearChain(0);
        //nun mach ich alles von vorn
        currentNumSections = 0;
        //das nullte adden
        bbChain->addChainElement(0,Ogre::BillboardChain::Element(Ogre::Vector3::ZERO,source->Beam->width,0,Ogre::ColourValue(0,0,0,0)));
   
        //for(int i=0;i<currentNumSections-nrOfSections;i++)
        //{
        //    bbChain->removeChainElement(0);
        //}
        //for(int i=0;i<=nrOfSections+2;i++)
        //{
        //    Ogre::BillboardChain::Element elem = bbChain->getChainElement(0,i);
        //    elem.texCoord = texSize * i;
        //    elem.position = Ogre::Vector3(0,0,-1) * elem.texCoord;
        //    bbChain->updateChainElement(0,i,elem);
        //    //bbChain->removeChainElement(0);
        //}

        //currentNumSections = nrOfSections;
    //}

    if(nrOfSections > currentNumSections)
    {
        for(int i=currentNumSections+1;i<=nrOfSections;i++)
        {
            Ogre::Real curTex = texSize * i;
            Ogre::Vector3 curPos = Ogre::Vector3(0,0,-1) * curTex;
            bbChain->addChainElement(0,Ogre::BillboardChain::Element(curPos,mSize,curTex,Ogre::ColourValue::White));
        }
        //nun das letzte
        Ogre::Real curTex = texSize * length;
        Ogre::Vector3 curPos = Ogre::Vector3(0,0,-1) * curTex;
        bbChain->addChainElement(0,Ogre::BillboardChain::Element(curPos,mSize,curTex,Ogre::ColourValue(0,0,0,0)));
        currentNumSections = nrOfSections;
    }
    
    if(currentNumSections != 0)
    {
        //updaten?
        Ogre::BillboardChain::Element elem = bbChain->getChainElement(0,0);
        elem.texCoord = texSize * length;
        elem.position = Ogre::Vector3(0,0,-1) * elem.texCoord;
        //elem.colour = Ogre::ColourValue(0,0,0,0);//Ogre::ColourValue::White;
        bbChain->updateChainElement(0,0,elem);
    }
    //farbupdate!
    //temp
    //Ogre::Real x = 2;
    //for(int i=0;i<currentNumSections+2;i++)
    //{
    //    Ogre::Real alpha = 1;
    //    Ogre::BillboardChain::Element elem = bbChain->getChainElement(0,i);
    //    if(i>=0 && i<=x)
    //    {
    //        alpha = float(i)/x;
    //    }
    //    /*else if(i >= currentNumSections+2-x && i <= currentNumSections+2)
    //    {
    //        alpha = (currentNumSections+2-i)/x;
    //    }*/
    //    
    //    elem.colour = Ogre::ColourValue(1,1,1,alpha);
    //    bbChain->updateChainElement(0,i,elem);
    //}
	//int nrOfSections = Ogre::Math::ICeil(length/texSize);//3;
 //   
 //   /*if(nrOfSections > bbChain->getMaxChainElements())
 //       nrOfSections = bbChain->getMaxChainElements();*/
 //   mDebugText(str(nrOfSections));
 //   //currentLength = bbChain->getNumChainElements(0);
 //   if(currentNumSections == nrOfSections)
 //   {
 //       //letztes updaten
 //       //bbChain->getch
 //      /* Ogre::BillboardChain::Element elem = bbChain->getChainElement(0,currentNumSections+2);
 //       
 //       elem.position = Ogre::Vector3(0,0,-texSize)*length;
 //       elem.texCoord = texSize*length;
 //       bbChain->updateChainElement(0,currentNumSections+2,elem);*/
 //   }
 //   else if(currentNumSections > nrOfSections)
 //   {
 //       
 //    //   for(int i=0;i<currentNumSections;i++)
	//    //{
 //    //       bbChain->removeChainElement(0);
 //    //   }
 //    //   for(int i=1;i<=nrOfSections;i++)
	//    //{
 //    //       Ogre::Vector3 pos = Ogre::Vector3(0,0,-texSize)*i;
 //    //       Ogre::Real texcoord;
 //    //       //if(i<nrOfSections-1)
 //    //           texcoord = texSize*i;
 //    //       //else
 //    //       //    texcoord = length;
 //    //       bbChain->addChainElement(0,Ogre::BillboardChain::Element(pos,mSize,texcoord,Ogre::ColourValue::White));
 //    //   }
 //       //und nun anpassen
 //       //Ogre::Real offset = (currentNumSections - nrOfSections)*mSize;
 //       //int newLength = nrOfSections;
 //       //for(int i=0;i<=newLength;i++)
 //       //{
 //       //    Ogre::BillboardChain::Element elem= bbChain->getChainElement(0,i);
 //       //    elem.position = Ogre::Vector3(0,0,-texSize)*i;//.z += offset;//+, weil der Wert negativ ist
 //       //    //if(i < newLength-1)
 //       //        elem.texCoord = texSize*i;
 //       //    /*else
 //       //        elem.texCoord = length;*/
 //       //    bbChain->updateChainElement(0,i,elem);
 //       //}
 //   }
 //   else
 //   {   
 //       //wachsen   
 //       for(int i=currentNumSections;i<=nrOfSections;i++)
	//    {
 //           Ogre::Vector3 pos;
 //           Ogre::Real texcoord;
 //           if(i<nrOfSections)
 //           {
 //               texcoord = texSize*i;
 //               pos = Ogre::Vector3(0,0,-texSize)*i;
 //           }
 //           else
 //           {                
 //               texcoord = texSize*length;
 //               pos = Ogre::Vector3(0,0,-texSize)*length;
 //           }
 //           bbChain->addChainElement(0,Ogre::BillboardChain::Element(pos,mSize,texcoord,Ogre::ColourValue::White));
 //       }
 //   }
 //   currentNumSections = nrOfSections;

    //HIER BEGINNT DAS MIT MANUALOBJ
	//int realNr = nrOfSections + 1;
	//Ogre::Real lastSegmentLength = length - Ogre::Math::IFloor(length);
	//if(lastSegmentLength < 0.01)
	//{
	//	lastSegmentLength = 0;
	//	realNr--;
	//}
	//manObj->beginUpdate(0);
	//manObj->estimateVertexCount(realNr*6);
	//
	//for(int i=nrOfSections;i>=0;i--)
	//{
	//	//   | -z
	//	// |   |
	//	//-4---3-
	//	// | \ |
	//	//-1---2-
	//	// |   |
	//	//   |_  x
	//	Ogre::Real delta = 0;
	//	if(i == nrOfSections)
	//		delta = -lastSegmentLength;
	//	manObj->position(-mWidth/2,0,(-i+1)*mHeight);//4
	//	manObj->textureCoord(i+1,0);	

	//	manObj->position(mWidth/2,0,(-i+delta)*mHeight);//2
	//	manObj->textureCoord(i+delta,1);

	//	manObj->position(-mWidth/2,0,(-i+delta)*mHeight);//1
	//	manObj->textureCoord(i+delta,0);

	//	
	//	manObj->position(mWidth/2,0,(-i+delta)*mHeight);//2
	//	manObj->textureCoord(i+delta,1);

	//	manObj->position(-mWidth/2,0,(-i+1)*mHeight);//4
	//	manObj->textureCoord(i+1,0);

	//	manObj->position(mWidth/2,0,(-i+1)*mHeight);//3
	//	manObj->textureCoord(i+1,1);
	//}

	//
	////}
	//manObj->end();
    //mDebugText(str(bbChain->getNumChainElements(0)));
}

void BeamObject::setScale(Ogre::Vector3 scale)
{
	GameObject::setScale(scale);

}

BeamObject::~BeamObject()
{
//	mLevel->getSceneManager()->destroyManualObject(manObj);
	mLevel->unregisterObject(this);
	
	/*if(mArt)
		delete mArt;*/

	if(mSnd)
	{
		mSnd->stop();
		mLevel->destroySource(mSnd);
		//mNode->detachObject(mSnd);
		//OgreAL::SoundManager::getSingletonPtr()->destroySound(mSnd);
	}
    //mLog("BeamObj destructor: mEffect="+ptrToString(mEffect));
    
	if(mEffect && !mLevel->getDestructorCalled())
        //weil das Level auch selbst den effect löscht, 
        //da der effect ein eigenständiges GameObject ist
		delete mEffect;
    if(bbChain)
        mLevel->getSceneManager()->destroyBillboardChain(bbChain);
	
	destroyNode(mLevel->getSceneManager(),mNode);
	
}