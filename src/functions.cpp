#include "functions.h"
#ifndef __editor
#include "CEGUI.h"
#else
#include <QImage>
#endif
#include "defines.h"
#include "Ogre.h"
#include <TypeConverter.h>

using namespace TypeConverter;
using namespace Ogre;

#ifndef __editor
CEGUI::MouseButton convertOISMouseButtonToCegui(OIS::MouseButtonID buttonID)
{
    switch (buttonID)
    {
	case OIS::MB_Left:		return CEGUI::LeftButton;
	case OIS::MB_Right:		return CEGUI::RightButton;
	case OIS::MB_Middle:	return CEGUI::MiddleButton;
	case OIS::MB_Button3:	return CEGUI::X1Button;
	case OIS::MB_Button4:	return CEGUI::X2Button;
	default:				return CEGUI::LeftButton;
    }
}

OIS::MouseButtonID convertCeguiMouseButtonToOis(CEGUI::MouseButton buttonID)
{
    switch (buttonID)
    {
	case CEGUI::LeftButton:		return OIS::MB_Left;
	case CEGUI::RightButton:	return OIS::MB_Right;
	case CEGUI::MiddleButton:	return OIS::MB_Middle;
	case CEGUI::X1Button:		return OIS::MB_Button3;
	case CEGUI::X2Button:		return OIS::MB_Button4;
	default:					return OIS::MB_Left;
    }
}
bool isMouseOverGui()//float const& x, float const& y){
{
   using namespace CEGUI;
   Window* mouseOverWin = System::getSingleton().getWindowContainingMouse();
   if (mouseOverWin == NULL) return false;
   CEGUI::String type = mouseOverWin->getType();
   if(type == ((utf8*)"DefaultWindow")) {
      return false;
   } else {
      return true;
   }
}
#endif

ObjType stringToObjectType(Ogre::String ogre_str)
{
	Ogre::StringUtil::toLowerCase(ogre_str);
	if(ogre_str == "static")
		return otStatic;
	else if(ogre_str == "special")
		return otDynamic;
	else if(ogre_str == "item")
		return otItem;
	else if(ogre_str == "char")
		return otChar;
	else if(ogre_str == "door")
		return otDoor;
	else if(ogre_str == "light")
		return otLight;
	else if(ogre_str == "entrance")
		return otEntrance;
	else if(ogre_str == "missile")
		return otMissile;
	else if(ogre_str == "container")
		return otContainer;
	else if(ogre_str == "group")
		return otGroup;
	else if(ogre_str == "deleted")
		return otDeleted;
	else if(ogre_str == "water")
		return otWater;
	return otInvalid;	
}

Ogre::String objectTypeToString(ObjType type)
{
	switch(type)
	{
	case otStatic:
		return "static";		
	case otDynamic:
		return "special";		
	case otItem:
		return "item";		
	case otChar:
		return "char";		
	case otDoor:
		return "door";		
	case otLight:
		return "light";		
	case otEntrance:
		return "entrance";		
	case otMissile:
		return "missile";		
	case otContainer:
		return "container";		
	case otGroup:
		return "group";		
	case otExplosion:
		return "explosion";		
	case otDeleted:
		return "deleted";
	case otWater:
		return "water";
	}
	
	return "";	
}


void destroyNode(Ogre::SceneManager *mSceneMgr,Ogre::SceneNode *mNode)
{
	if(!mNode || !mSceneMgr)
		return;
	mNode->removeAndDestroyAllChildren();
	while(mNode->numAttachedObjects())
	{

		MovableObject *m = mNode->detachObject((unsigned short)0);
	//	Ogre::LogManager::getSingletonPtr()->logMessage("destroyNode: "+m->getName());
		mSceneMgr->destroyMovableObject(m);

	}

	mSceneMgr->destroySceneNode(mNode);
	mNode = NULL;
}

//
//OgreNewt::Collision *makeCollision(Ogre::Vector3& inertia,OgreNewt::World *mWorld,short colType, Ogre::Real mass, Ogre::Vector3 collParams, Ogre::SceneNode *nod, Ogre::Vector3 scale)
//{
//	OgreNewt::Collision *col = 0;
//
//	//Ogre::Vector3 inertia;
//	Ogre::Vector3 CollisionParams = collParams * scale;
//	Ogre::Real temp;
//	mLog("coltype="+ogre_str(colType));
//	/*if(mass > 0)
//	{*/
//
//
//
//	switch(colType)
//	{
//	case CT_BOX:
//		col = new OgreNewt::CollisionPrimitives::Box(mWorld,CollisionParams);
//		inertia = OgreNewt::MomentOfInertia::CalcBoxSolid(mass,CollisionParams);
//		break;
//	case CT_CAPSULE:
//		col = new OgreNewt::CollisionPrimitives::Capsule(mWorld,CollisionParams.x,CollisionParams.y);
//		inertia = OgreNewt::MomentOfInertia::CalcCylinderSolid(mass,CollisionParams.x,CollisionParams.y);
//		break;
//	case CT_CHAMFERCYLINDER:
//		col = new OgreNewt::CollisionPrimitives::ChamferCylinder(mWorld,CollisionParams.x,CollisionParams.y);
//		inertia = OgreNewt::MomentOfInertia::CalcCylinderSolid(mass,CollisionParams.x,CollisionParams.y);
//		break;
//	case CT_CONE:
//		col = new OgreNewt::CollisionPrimitives::Cone(mWorld,CollisionParams.x,CollisionParams.y);
//		inertia = OgreNewt::MomentOfInertia::CalcCylinderSolid(mass,CollisionParams.x,CollisionParams.y);
//		break;
//	case CT_CONVEXHULL:
//		col = new OgreNewt::CollisionPrimitives::ConvexHull(mWorld,nod);
//		temp = nod->getAttachedObject(0)->getBoundingRadius();
//		//!!Das ist ne zwischenlösung!!!
//		inertia = OgreNewt::MomentOfInertia::CalcEllipsoidSolid(mass,Ogre::Vector3(temp,temp,temp));
//		/*inertia = OgreNewt::MomentOfInertia::CalcEllipsoidSolid(mass,CollisionParams);*/
//		break;
//	case CT_CYLINDER:
//		col = new OgreNewt::CollisionPrimitives::Cylinder(mWorld,CollisionParams.x,CollisionParams.y);
//		inertia = OgreNewt::MomentOfInertia::CalcCylinderSolid(mass,CollisionParams.x,CollisionParams.y);
//
//		break;
//	case CT_ELLIPSOID:
//		col = new OgreNewt::CollisionPrimitives::Ellipsoid(mWorld,CollisionParams);
//		inertia = OgreNewt::MomentOfInertia::CalcEllipsoidSolid(mass,CollisionParams);
//
//		break;
//	case CT_PYRAMID:
//		col = new OgreNewt::CollisionPrimitives::Pyramid(mWorld,CollisionParams);
//		inertia = OgreNewt::MomentOfInertia::CalcCylinderSolid(mass,CollisionParams.x,CollisionParams.y);
//
//		break;
//	case CT_TREECOLLISION:
//		mLog("treecollision");
//		col = new OgreNewt::CollisionPrimitives::TreeCollision(mWorld,nod,false);
//		break;
//	case CT_TREECOLLISIONSCENE:
//		{
//			OgreNewt::CollisionPrimitives::TreeCollisionSceneParser *testcol = new OgreNewt::CollisionPrimitives::TreeCollisionSceneParser(mWorld);
//			testcol->parseScene(nod);
//			col = testcol;
//		}
//		break;
//
//	default:
//		col = new OgreNewt::CollisionPrimitives::TreeCollision(mWorld,nod,false);
//		break;
//
//	};
//	mLog("makeCollision fertig");
//	return col;
//
//}
//
//OgreNewt::Body *makeBody(OgreNewt::World *mWorld,short colType, Ogre::Real mass, Ogre::Vector3 CollisionParams, Ogre::SceneNode *nod, Ogre::Vector3 scale)
//{
////	testLoopNode(nod);
//	mLog("makeBody beginnt");
//
//	Ogre::Vector3 inertia;
//	if(colType == CT_TREECOLLISION || colType == CT_CONVEXHULL)
//	{
//	}
//	OgreNewt::Collision *col = makeCollision(inertia,mWorld, colType, mass, CollisionParams, nod, scale);
//
//
//	//Ogre::Vector3 inertia;
//	//Ogre::Real temp;
//	///*if(mass > 0)
//	//{*/
//
//	//switch(colType)
//	//{
//	//case CT_BOX:
//	//	col = new OgreNewt::CollisionPrimitives::Box(mWorld,CollisionParams);
//	//	inertia = OgreNewt::MomentOfInertia::CalcBoxSolid(mass,CollisionParams);
//	//	break;
//	//case CT_CAPSULE:
//	//	col = new OgreNewt::CollisionPrimitives::Capsule(mWorld,CollisionParams.x,CollisionParams.y);
//	//	inertia = OgreNewt::MomentOfInertia::CalcCylinderSolid(mass,CollisionParams.x,CollisionParams.y);
//	//	break;
//	//case CT_CHAMFERCYLINDER:
//	//	col = new OgreNewt::CollisionPrimitives::ChamferCylinder(mWorld,CollisionParams.x,CollisionParams.y);
//	//	inertia = OgreNewt::MomentOfInertia::CalcCylinderSolid(mass,CollisionParams.x,CollisionParams.y);
//	//	break;
//	//case CT_CONE:
//	//	col = new OgreNewt::CollisionPrimitives::Cone(mWorld,CollisionParams.x,CollisionParams.y);
//	//	inertia = OgreNewt::MomentOfInertia::CalcCylinderSolid(mass,CollisionParams.x,CollisionParams.y);
//	//	break;
//	//case CT_CONVEXHULL:
//	//	col = new OgreNewt::CollisionPrimitives::ConvexHull(mWorld,nod);
//	//	temp = nod->getAttachedObject(0)->getBoundingRadius();
//	//	//!!Das ist ne zwischenlösung!!!
//	//	inertia = OgreNewt::MomentOfInertia::CalcEllipsoidSolid(mass,Ogre::Vector3(temp,temp,temp));
//	//	break;
//	//case CT_CYLINDER:
//	//	col = new OgreNewt::CollisionPrimitives::Cylinder(mWorld,CollisionParams.x,CollisionParams.y);
//	//	inertia = OgreNewt::MomentOfInertia::CalcCylinderSolid(mass,CollisionParams.x,CollisionParams.y);
//
//	//	break;
//	//case CT_ELLIPSOID:
//	//	col = new OgreNewt::CollisionPrimitives::Ellipsoid(mWorld,CollisionParams);
//	//	inertia = OgreNewt::MomentOfInertia::CalcEllipsoidSolid(mass,CollisionParams);
//
//	//	break;
//	//case CT_PYRAMID:
//	//	col = new OgreNewt::CollisionPrimitives::Pyramid(mWorld,CollisionParams);
//	//	inertia = OgreNewt::MomentOfInertia::CalcCylinderSolid(mass,CollisionParams.x,CollisionParams.y);
//
//	//	break;
//	//case CT_TREECOLLISION:
//	//	col = new OgreNewt::CollisionPrimitives::TreeCollision(mWorld,nod,false);
//	//	break;
//
//	///*default:
//	//	goOn = false;*/
//
//	//};
//	if(!col)
//	{
//		mLog("col == 0!!!");
//	}
//	//}//-------------ENDE von if(cur_obj.mass > 0)
//	//else
//	//{
//	//	//also wenn es null ist -_-
//	//	col = new OgreNewt::CollisionPrimitives::TreeCollision(mWorld,nod,false);
//	//	mLog("kk, treecollision");
//	//	//me n00b, lol...
//	//}
//	if(col)
//	{
//
//		OgreNewt::Body *bod = new OgreNewt::Body(mWorld,col);
////		log("body created");
//		if(colType != CT_TREECOLLISION)
//		{
//			//log("mass > 0");
//			bod->setMassMatrix(mass,inertia);
//			//bod->setType(BT_MOVABLE);
//			/*bod->setStandardForceCallback();*/
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
//		return NULL;
//	}
//}

Ogre::SceneNode *getMostParentNode(Ogre::SceneManager *mSceneMgr,Ogre::SceneNode *child)
{
	Ogre::SceneNode *mostParent = child;
	Ogre::SceneNode *nextParent = child;
	while(nextParent != mSceneMgr->getRootSceneNode())
	{
		mostParent = nextParent;
		nextParent = mostParent->getParentSceneNode();
	}
	return mostParent;
}

//Ogre::Entity *getClosestEntity(Ogre::SceneManager *mSceneMgr,Ogre::Ray mRay,Ogre::Real maxDist,bool fullRaycast,Ogre::Entity *ignore)
//{
//	RaySceneQuery *	mRaySceneQuery = mSceneMgr->createRayQuery(mRay);
//	mRaySceneQuery->setSortByDistance(true);
//
//
////   Ogre::Vector3 rayOrigin = mouseRay.getOrigin();
////   Ogre::Vector3 rayDirection = mouseRay.getDirection();
//
//   // Execute query
//	RaySceneQueryResult &result = mRaySceneQuery->execute();
//
//
//
//	Ogre::Real closest_distance = -1.0f;
//	Ogre::Vector3 closest_result;
//	Ogre::Entity *closest_entity = NULL;
//
//	if(result.begin() != result.end())
//	{
//
//		for(RaySceneQueryResult::iterator itr = result.begin( );
//		itr != result.end();itr++)
//		{
//			//Ogre::String name = itr->movable->getName();
//			if(maxDist != 0)
//			{
//				if(itr->distance > maxDist)
//					break;
//			}
//
//			if(itr->movable)
//			{
//
//				// app->setDebugText(itr->movable->getMovableType()+" : "+name,true);
//				if(itr->movable->getMovableType() == "Ogre::Entity")
//				{
//
//
//					//polygon check BEGIN-------------------------------------
//					// get the entity to check
//					Ogre::Entity *Ent = static_cast<Ogre::Entity*>(itr->movable);
//					if(!fullRaycast)
//					{
//						//dh es reicht das zurückzugeben, was wir als erstes finden
//						if(ignore != NULL)
//						{
//							if(Ent != ignore)
//							{
//								closest_entity = Ent;
//								break;
//							}
//						}
//						else
//						{
//							closest_entity = Ent;
//							break;
//						}
//
//
//					}//if(!fullRaycast)
//
//					// mesh data to retrieve
//					size_t vertex_count;
//					size_t index_count;
//					Ogre::Vector3 *vertices;
//					unsigned long *indices;
//
//					// get the mesh information
//					Ogre::MeshPtr temp = Ent->getMesh();
//					Ogre::Mesh *myMesh = temp.getPointer();
//
//					getMeshInformation(myMesh, vertex_count, vertices, index_count, indices,
//					Ent->getParentNode()->_getDerivedPosition(),
//					Ent->getParentNode()->_getDerivedOrientation(),
//					Ent->getParentNode()->getScale());
//
//					// test for hitting individual triangles on the mesh
//					//bool new_closest_found = false;
//					for (int i = 0; i < static_cast<int>(index_count); i += 3)
//					{
//						// check for a hit against this triangle
//						std::pair<bool, Ogre::Real> hit = Ogre::Math::intersects(mRay, vertices[indices[i]],
//						vertices[indices[i+1]], vertices[indices[i+2]], true, false);
//
//						// if it was a hit check if its the closest
//						if (hit.first)
//						{
//							if ((closest_distance < 0.0f) ||
//							(hit.second < closest_distance))
//							{
//								// this is the closest so far, save it off
//								closest_distance = hit.second;
//							//	notrly_closest_dist = itr->distance;
//								closest_entity = Ent;
//								//new_closest_found = true;
//							}
//						}
//					}
//
//					// free the verticies and indicies memory
//					delete[] vertices;
//					delete[] indices;
//
//					// if we found a new closest raycast for this object, update the
//					// closest_result before moving on to the next object.
//					//if (new_closest_found)
//					//{
//					//	//closest_result = mouseRay.getPoint(closest_distance);
//					//	closest_entity = Ent;
//					//}
//					//polygon check END--------------------------
//
//
//				}//von if(entity
//
//			}
//			else
//			{
//				//no movable
//			}
//			//app->setDebugText("="+Ogre::StringConverter::toString(itr->distance)+"\n",true);
//
//
//
//
//
//
//		}//for(RaySceneQueryResult::iterator itr = result.begin( );...
//
//	}//if(result.begin != result.end())
//
//	mRaySceneQuery->clearResults();
//	mSceneMgr->destroyQuery(mRaySceneQuery);
//
//	return closest_entity;
//}
//

Ogre::Vector3 getWorldScale(Ogre::SceneManager *mgr,Ogre::SceneNode *nod)
{
	if(nod->getParentSceneNode() == mgr->getRootSceneNode() || !nod->getInheritScale())
	{
		return nod->getScale();
	}
	else
	{
		return nod->getScale()*getWorldScale(mgr,nod->getParentSceneNode());
	}

}

Ogre::RaySceneQueryResultEntry getClosest(Ogre::SceneManager *mSceneMgr,Ogre::Ray mRay,Ogre::Real maxDist,Ogre::Real minDist,bool fullRaycast, Ogre::String movableType, Ogre::uint32 queryFlags)
{
	RaySceneQuery *	mRaySceneQuery = mSceneMgr->createRayQuery(mRay);
	mRaySceneQuery->setSortByDistance(true);
	mRaySceneQuery->setQueryMask(queryFlags);


//   Ogre::Vector3 rayOrigin = mouseRay.getOrigin();
//   Ogre::Vector3 rayDirection = mouseRay.getDirection();

   // Execute query
	RaySceneQueryResult &result = mRaySceneQuery->execute();






	Ogre::Real closest_distance = -1.0f;
	Ogre::Vector3 closest_result;
	//Ogre::MovableObject *closest_mov = NULL;
	Ogre::RaySceneQueryResultEntry closest_res;
	closest_res.distance = 0;
	closest_res.movable = NULL;
	closest_res.worldFragment = NULL;
	if(result.begin() != result.end())
	{

		for(RaySceneQueryResult::iterator itr = result.begin( );
		itr != result.end();itr++)
		{
			//Ogre::String name = itr->movable->getName();
			if(itr->distance < minDist)
			{
				continue;
			}
			if(itr->distance > maxDist && maxDist != 0)
			{
				break;
				//sie sind nach dist sortiert, sobald eins zu weit ist, werden alle zu weit sein
				//continue;
			}
			/*if(maxDist != 0)
			{
				if(itr->distance > maxDist)
					break;
			}*/


			if(itr->movable)
			{
				Ogre::String curMovType = itr->movable->getMovableType();
				if((movableType != "all" && movableType != curMovType) || curMovType == "Ogre::Camera")
				{
					//du bist falsch, der nächste...
					continue;

				}
				if(!fullRaycast)
				{
						//dh es reicht das zurückzugeben, was wir als erstes finden
					closest_res = *itr;
					//closest_mov = itr->movable;
					break;
				}//!fullRaycast

				// app->setDebugText(itr->movable->getMovableType()+" : "+name,true);
				if(itr->movable->getMovableType() == "Ogre::Entity")
				{


					//polygon check BEGIN-------------------------------------
					// get the entity to check
					Ogre::Entity *Ent = static_cast<Ogre::Entity*>(itr->movable);


					// mesh data to retrieve
					size_t vertex_count;
					size_t index_count;
					Ogre::Vector3 *vertices;
					unsigned long *indices;

					// get the mesh information
					Ogre::MeshPtr temp = Ent->getMesh();
					Ogre::Mesh *myMesh = temp.getPointer();

					getMeshInformation(myMesh, vertex_count, vertices, index_count, indices,
					Ent->getParentNode()->_getDerivedPosition(),
					Ent->getParentNode()->_getDerivedOrientation(),
					getWorldScale(mSceneMgr,static_cast<Ogre::SceneNode*>(Ent->getParentNode()))/*Ent->getParentNode()->getScale()*/);

					Ogre::Vector3 test1 = Ent->getParentNode()->_getDerivedPosition();
					Ogre::Quaternion test2 = Ent->getParentNode()->_getDerivedOrientation();
					Ogre::Vector3 test3 = Ent->getParentNode()->getScale();
					Ogre::Vector3 test4 = getWorldScale(mSceneMgr,static_cast<Ogre::SceneNode*>(Ent->getParentNode()));

					// test for hitting individual triangles on the mesh
					//bool new_closest_found = false;
					for (int i = 0; i < static_cast<int>(index_count); i += 3)
					{
						// check for a hit against this triangle
						std::pair<bool, Ogre::Real> hit = Ogre::Math::intersects(mRay, vertices[indices[i]],
						vertices[indices[i+1]], vertices[indices[i+2]], true, false);

						// if it was a hit check if its the closest
						if (hit.first)
						{
							if ((closest_distance < 0.0f) ||
							(hit.second < closest_distance))
							{
								// this is the closest so far, save it off
								closest_distance = hit.second;

								//closest_mov = itr->movable;

								closest_res = *itr;




								//new_closest_found = true;
							}
						}
					}

					// free the verticies and indicies memory
					delete[] vertices;
					delete[] indices;

					// if we found a new closest raycast for this object, update the
					// closest_result before moving on to the next object.
					//if (new_closest_found)
					//{
					//	//closest_result = mouseRay.getPoint(closest_distance);
					//	closest_entity = Ent;
					//}
					//polygon check END--------------------------


				}//von if(entity
				else
				{
					//sonstiges movable
					closest_res = *itr;

				}

			}
			else
			{
				//no movable
			}
			//app->setDebugText("="+Ogre::StringConverter::toString(itr->distance)+"\n",true);






		}//for(RaySceneQueryResult::iterator itr = result.begin( );...

	}//if(result.begin != result.end())

	mRaySceneQuery->clearResults();
	mSceneMgr->destroyQuery(mRaySceneQuery);

	return closest_res;
}



void getMeshInformation( const Ogre::Mesh* const mesh, size_t &vertex_count,
                                 Ogre::Vector3* &vertices,
                                 size_t &index_count, unsigned long* &indices,
                                 const Ogre::Vector3 &position,
                                 const Ogre::Quaternion &orient,
                                 const Ogre::Vector3 &scale)
{
 bool added_shared = false;
 size_t current_offset = 0;
 size_t shared_offset = 0;
 size_t next_offset = 0;
 size_t index_offset = 0;


  vertex_count = index_count = 0;

    // Calculate how many vertices and indices we're going to need
    for ( unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
    {
      Ogre::SubMesh* submesh = mesh->getSubMesh( i );

        // We only need to add the shared vertices once
      if(submesh->useSharedVertices)
      {
          if( !added_shared )
          {
            vertex_count += mesh->sharedVertexData->vertexCount;
            added_shared = true;
          }
      }
      else
      {
          vertex_count += submesh->vertexData->vertexCount;
      }

      // Add the indices
      index_count += submesh->indexData->indexCount;
  }


  // Allocate space for the vertices and indices
  vertices = new Ogre::Vector3[vertex_count];
  indices = new unsigned long[index_count];

  added_shared = false;

  // Run through the submeshes again, adding the data into the arrays
  for ( unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
  {
    Ogre::SubMesh* submesh = mesh->getSubMesh(i);

    Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;

    if((!submesh->useSharedVertices)||(submesh->useSharedVertices && !added_shared))
    {
      if(submesh->useSharedVertices)
      {
        added_shared = true;
        shared_offset = current_offset;
      }

      const Ogre::VertexElement* posElem =
        vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);

      Ogre::HardwareVertexBufferSharedPtr vbuf =
        vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());

      unsigned char* vertex =
        static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

// There is _no_ baseVertexPointerToElement() which takes an Ogre::Real or a double
//  as second argument. So make it float, to avoid trouble when Ogre::Real will
//  be comiled/typedefed as double:
//      Ogre::Real* pReal;
      float* pReal;

      for( size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
      {
        posElem->baseVertexPointerToElement(vertex, &pReal);

        Ogre::Vector3 pt(pReal[0], pReal[1], pReal[2]);

        vertices[current_offset + j] = (orient * (pt * scale)) + position;
      }

      vbuf->unlock();
      next_offset += vertex_data->vertexCount;
    }


    Ogre::IndexData* index_data = submesh->indexData;
    size_t numTris = index_data->indexCount / 3;
    Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;

    bool use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);

    unsigned long*  pLong = static_cast<unsigned long*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
    unsigned short* pShort = reinterpret_cast<unsigned short*>(pLong);


    size_t offset = (submesh->useSharedVertices)? shared_offset : current_offset;

    if ( use32bitindexes )
    {
      for ( size_t k = 0; k < numTris*3; ++k)
      {
        indices[index_offset++] = pLong[k] + static_cast<unsigned long>(offset);
      }
    }
    else
    {
      for ( size_t k = 0; k < numTris*3; ++k)
      {
        indices[index_offset++] = static_cast<unsigned long>(pShort[k]) +
                                  static_cast<unsigned long>(offset);
      }
    }

    ibuf->unlock();
    current_offset = next_offset;
  }
}



short StringToColType(Ogre::String mystring)
{
	Ogre::String s; 
	s = mystring;
	Ogre::StringUtil::toLowerCase(s);

	if(s == Ogre::String("box"))
	{
		return CT_BOX;
	}
	else if(s == Ogre::String("capsule"))
	{
		return CT_CAPSULE;
	}
	else if(s == Ogre::String("chamfercylinder"))
	{
		return CT_CHAMFERCYLINDER;
	}
	else if(s == Ogre::String("cone"))
	{
		return CT_CONE;
	}
	else if(s == Ogre::String("convexhull"))
	{
		return CT_CONVEXHULL;
	}
	else if(s == Ogre::String("cylinder"))
	{
		return CT_CYLINDER;
	}
	else if(s == Ogre::String("ellipsoid"))
	{
		return CT_ELLIPSOID;
	}
	else if(s == Ogre::String("pyramid"))
	{
		return CT_PYRAMID;
	}
	else if(s == Ogre::String("static") || s == Ogre::String("treecollision"))
	{
		return CT_TREECOLLISION;
	}
	else
	{
		return CT_NOCOLLISION;
	}

}



bool createPlaneMesh(Ogre::String name)
{


	//ok, ein fall für den meshmanager
	if(Ogre::MeshManager::getSingleton().resourceExists(name))
	{
		//dann ist alles ok
		//mLog("gibts schon");
		return true;
	}
	else
	{
		/*//////app->log("gibts nicht");*/
		Ogre::Plane pl(Ogre::Vector3(0,0,-1),0);



		Ogre::String stringvals[4];
		stringvals[0] = "";
		stringvals[1] = "";
		stringvals[2] = "";
		stringvals[3] = "";
		int svIndex = 0;
		//0 = xseg, 1 = yseg, 2 = width, 3 = height


		/*Ogre::String xseg_s = "";
		Ogre::String yseg_s = "";

		Ogre::String width_s = "";
		Ogre::String height_s = "";*/

		Ogre::String temp_string= "";
		for(unsigned int i=7;i<name.length();i++)
		{
			if(name[i] != '/')
			{
				temp_string += name[i];
			}
			else
			{
				stringvals[svIndex] = temp_string;
				svIndex++;
				//xseg_s = temp_string;
				temp_string="";
			}

		}
		stringvals[svIndex] = temp_string;
		//yseg_s = temp_string;

		/*mLog("zerlegt: xSeg="+stringvals[0]+", ySeg="+
			stringvals[1]+", width="+stringvals[2]+
			" height="+stringvals[3]);*/
		int xSeg  = Ogre::StringConverter::parseInt(stringvals[0]);
		int ySeg  = Ogre::StringConverter::parseInt(stringvals[1]);

		int width = 1;
		if(stringvals[2] != "")
		{
			width = Ogre::StringConverter::parseInt(stringvals[2]);
		}
		int height= 1;
		if(stringvals[3] != "")
		{
			height = Ogre::StringConverter::parseInt(stringvals[3]);
		}

		if(width == 0) width = 1;
		if(height== 0) height= 1;

		/*mLog("konvertiert: xSeg="+ogre_str(xSeg)+", ySeg="+
			ogre_str(ySeg)+", width="+ogre_str(width)+
			" height="+ogre_str(height));*/

		if(xSeg > 0 && ySeg > 0)
		{
			MeshPtr planeMeshPtr = Ogre::MeshManager::getSingleton().createPlane(name,
			   Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, pl,
			   width,height,xSeg,ySeg,
			   true,1,5,5,Ogre::Vector3(1,0,0));
			planeMeshPtr.getPointer()->buildTangentVectors();
			return true;
		}
		else
		{
			return false;
		}
	}

	return false;
}

Ogre::Real getFullYaw(Ogre::Quaternion q)
{
	Ogre::Vector3 rotated = q * Ogre::Vector3::UNIT_Z;
	rotated.y = 0;
	rotated.normalise();
	/*
	also: berechne den winkel zw. der x-achse und den vektoren mit
	sin(w1) = y/länge und cos(w2) = x/länge
	für w1 >= 0 ist winkel = w2
	für w1 <= 0 ist winkel = 360-w2
	//statt w2 reicht die Y(bzw hier: Z) komponente
	*/
	Ogre::Real w1 = Ogre::Math::ACos(rotated.x).valueDegrees();
	if(rotated.z >= 0)
	{
		w1 = 360-w1;
	}
	return w1;

}

int round(Ogre::Real x)
{
	Ogre::Real f = x - Ogre::Math::Floor(x);
	if(f >= 0.5)
		return Ogre::Math::ICeil(x);
	else
		return Ogre::Math::IFloor(x);
}

Ogre::Real getFullPitch(Ogre::Quaternion q)
{
	Ogre::Vector3 rotated = q * Ogre::Vector3::UNIT_Y;
	rotated.x = 0;
	rotated.normalise();
	/*
	also: berechne den winkel zw. der x-achse und den vektoren mit
	sin(w1) = y/länge und cos(w2) = x/länge
	für w1 >= 0 ist winkel = w2
	für w1 <= 0 ist winkel = 360-w2
	//statt w2 reicht die Y(bzw hier: Z) komponente
	*/
	Ogre::Real w1 = Ogre::Math::ACos(rotated.z).valueDegrees();
	if(rotated.y >= 0)
	{
		w1 = 360-w1;
	}
	return w1;

}


Ogre::Real getFullRoll(Ogre::Quaternion q)
{
	Ogre::Vector3 rotated = q * Ogre::Vector3::UNIT_X;
	rotated.z = 0;
	rotated.normalise();
	/*
	also: berechne den winkel zw. der x-achse und den vektoren mit
	sin(w1) = y/länge und cos(w2) = x/länge
	für w1 >= 0 ist winkel = w2
	für w1 <= 0 ist winkel = 360-w2
	//statt w2 reicht die Y(bzw hier: Z) komponente
	*/
	Ogre::Real w1 = Ogre::Math::ACos(rotated.y).valueDegrees();
	if(rotated.x >= 0)
	{
		w1 = 360-w1;
	}
	return w1;

}

//
//
//
//Ogre::String ogre_str(Ogre::Real val, unsigned short precision, unsigned short width, char fill,
//    std::ios::fmtflags flags )
//{
//	return Ogre::StringConverter::toString(val,precision,width,fill,flags);
//}
//
///** Converts a Ogre::Radian to a Ogre::String. */
//Ogre::String ogre_str(Ogre::Radian val, unsigned short precision, unsigned short width, char fill,
//    std::ios::fmtflags flags )
//{
//    return Ogre::StringConverter::toString(val.valueAngleUnits(), precision, width, fill, flags);
//}
///** Converts a Ogre::Degree to a Ogre::String. */
//Ogre::String ogre_str(Ogre::Degree val, unsigned short precision,
//    unsigned short width, char fill,
//    std::ios::fmtflags flags )
//{
//    return Ogre::StringConverter::toString(val.valueAngleUnits(), precision, width, fill, flags);
//}
///** Converts an int to a Ogre::String. */
//Ogre::String ogre_str(int val, unsigned short width,char fill,std::ios::fmtflags flags)
//{
//	return Ogre::StringConverter::toString(val,width,fill,flags);
//}
//
///** Converts a size_t to a Ogre::String. */
//Ogre::String ogre_str(size_t val, unsigned short width, char fill, std::ios::fmtflags flags)
//{
//	return Ogre::StringConverter::toString(val,width,fill,flags);
//}
//
///** Converts an unsigned long to a Ogre::String. */
//Ogre::String ogre_str(unsigned long val, unsigned short width, char fill,
//    std::ios::fmtflags flags)
//{
//	return Ogre::StringConverter::toString(val,width,fill,flags);
//}
//
///** Converts a long to a Ogre::String. */
//Ogre::String ogre_str(long val, unsigned short width, char fill, std::ios::fmtflags flags)
//{
//	return Ogre::StringConverter::toString(val,width,fill,flags);
//}
//
///** Converts a boolean to a Ogre::String.
//@param yesNo If set to true, result is 'yes' or 'no' instead of 'true' or 'false'
//*/
//Ogre::String ogre_str(bool val, bool yesNo)
//{
//	return Ogre::StringConverter::toString(val,yesNo);
//}
///** Converts a Ogre::Vector2 to a Ogre::String.
//@remarks
//    Format is "x y" (i.e. 2x Ogre::Real values, space delimited)
//*/
//Ogre::String ogre_str(const Ogre::Vector2& val)
//{
//	return Ogre::StringConverter::toString(val);
//}
///** Converts a Ogre::Vector3 to a Ogre::String.
//@remarks
//    Format is "x y z" (i.e. 3x Ogre::Real values, space delimited)
//*/
//
//Ogre::String ogre_str(const Ogre::Vector3& val)
//{
//	return Ogre::StringConverter::toString(val);
//}
//
//Ogre::String ogre_str(const Ogre::Vector3& val,bool newLines)
//{
//	if(newLines)
//	{
//		return ogre_str(val.x)+"\n"+ogre_str(val.y)+"\n"+ogre_str(val.z);
//	}
//	else
//	{
//		return Ogre::StringConverter::toString(val);
//	}
//}
///** Converts a Ogre::Vector4 to a Ogre::String.
//@remarks
//    Format is "x y z w" (i.e. 4x Ogre::Real values, space delimited)
//*/
//Ogre::String ogre_str(const Ogre::Vector4& val)
//{
//	return Ogre::StringConverter::toString(val);
//}
///** Converts a Ogre::Matrix3 to a Ogre::String.
//@remarks
//    Format is "00 01 02 10 11 12 20 21 22" where '01' means row 0 column 1 etc.
//*/
//Ogre::String ogre_str(const Ogre::Matrix3& val)
//{
//	return Ogre::StringConverter::toString(val);
//}
///** Converts a Ogre::Matrix4 to a Ogre::String.
//@remarks
//    Format is "00 01 02 03 10 11 12 13 20 21 22 23 30 31 32 33" where
//    '01' means row 0 column 1 etc.
//*/
//Ogre::String ogre_str(const Ogre::Matrix4& val)
//{
//	return Ogre::StringConverter::toString(val);
//}
///** Converts a Ogre::Quaternion to a Ogre::String.
//@remarks
//    Format is "w x y z" (i.e. 4x Ogre::Real values, space delimited)
//*/
//Ogre::String ogre_str(const Ogre::Quaternion& val)
//{
//	return Ogre::StringConverter::toString(val);
//}
///** Converts a Ogre::ColourValue to a Ogre::String.
//@remarks
//    Format is "r g b a" (i.e. 4x Ogre::Real values, space delimited).
//*/
//Ogre::String ogre_str(const Ogre::ColourValue& val)
//{
//	return Ogre::StringConverter::toString(val);
//}
///** Converts a Ogre::StringVector to a string.
//@remarks
//    Strings must not contain spaces since space is used as a delimeter in
//    the output.
//*/
//Ogre::String ogre_str(const Ogre::StringVector& val)
//{
//	return Ogre::StringConverter::toString(val);
//}

//void testLoopNode(Ogre::SceneNode *nod,Ogre::Vector3 scale, Ogre::Vector3 pos)
//{
//	mLog("beginne loopnode");
//	if(nod->numChildren())
//	{
//		Ogre::Node::ChildNodeIterator itr = nod->getChildIterator();
//		while(itr.hasMoreElements())
//		{
//			Ogre::Node *nd = itr.getNext();
//			//nd->_getDerivedPosition();
//			mLog("curNode = "+nd->getName());
//			testLoopNode((Ogre::SceneNode*)nd,nod->getScale(),nod->getPosition()-pos);
//		}
//	}
//	if(nod->numAttachedObjects())
//	{
//
//		Ogre::SceneNode::ObjectIterator itr = nod->getAttachedObjectIterator();
//		while(itr.hasMoreElements())
//		{
//			MovableObject *mov = itr.getNext();
//			mLog("curMov = "+mov->getName());
//		}
//	}
//
//
//}
//-------------------------------------------------------------------------------------------------------
Ogre::String getUniqueNodeName(Ogre::SceneManager *mgr, Ogre::String base)
{
	int i = 0;
	Ogre::String name = base;
	while(mgr->hasSceneNode(name))
	{
		name = base+ogre_str(++i);
	}
	return name;
}
//-------------------------------------------------------------------------------------------------------
Ogre::String getUniqueEntityName(Ogre::SceneManager *mgr, Ogre::String base)
{
	int i = 0;
	Ogre::String name = base;
	while(mgr->hasEntity(name))
	{
		name = base+ogre_str(++i);
	}
	return name;
}
//-------------------------------------------------------------------------------------------------------
Ogre::String getUniquePSName(Ogre::SceneManager *mgr, Ogre::String base)
{
	int i = 0;
	Ogre::String name = base;
	while(mgr->hasParticleSystem(name))
	{
		name = base+ogre_str(++i);
	}
	return name;
}
//-------------------------------------------------------------------------------------------------------
Ogre::String getUniqueLightName(Ogre::SceneManager *mgr, Ogre::String base)
{
	int i = 0;
	Ogre::String name = base;
	while(mgr->hasLight(name))
	{
		name = base+ogre_str(++i);
	}
	return name;
}
//-------------------------------------------------------------------------------------------------------
Ogre::String getUniqueBBSetName(Ogre::SceneManager *mgr, Ogre::String base)
{
	int i = 0;
	Ogre::String name = base;
	while(mgr->hasBillboardSet(name))
	{
		name = base+ogre_str(++i);
	}
	return name;
}
//-------------------------------------------------------------------------------------------------------
Ogre::SceneNode *cloneSceneNode(Ogre::SceneManager *mgr, Ogre::SceneNode* orig, Ogre::SceneNode *_parent, Ogre::Any clAny)
{

	//erstmal erstellen
	Ogre::SceneNode *parent = _parent;
	Ogre::SceneNode *clone = NULL;
	if(!parent)
	{
		parent = mgr->getRootSceneNode();
	}
	clone = parent->createChildSceneNode();
	clone->setPosition(orig->getPosition());
	clone->setOrientation(orig->getOrientation());
	clone->setScale(orig->getScale());
	//cloning children
	Ogre::SceneNode::ChildNodeIterator nodei = orig->getChildIterator();
	while (nodei.hasMoreElements())
	{
	   Ogre::SceneNode* childNode =     static_cast<Ogre::SceneNode*>(nodei.getNext());
	   cloneSceneNode(mgr,childNode,clone,clAny);
	}

	//cloning attached
	Ogre::SceneNode::ObjectIterator iter = orig->getAttachedObjectIterator();
	while (iter.hasMoreElements())
	{
		Ogre::MovableObject* mobj = iter.getNext();
		Ogre::String mType = mobj->getMovableType();

		if(mType == "Ogre::Entity")
		{

			Ogre::Entity *ent_o = static_cast<Ogre::Entity*>(mobj);
			Ogre::String name = getUniqueEntityName(mgr);
			Ogre::Mesh *mp = ent_o->getMesh().getPointer();
			Ogre::String mesh = mp->getName();  //->getPointer()->getName();



			//Ogre::String mat  = ent_o->getSubEntity(0)->getMaterialName();
			Ogre::Entity *ent_c = mgr->createEntity(name,mesh);
			unsigned int se = ent_o->getNumSubEntities();
			for(unsigned int i = 0;i<se;i++)
			{
			    ent_c->getSubEntity(i)->setMaterialName(
                    ent_o->getSubEntity(i)->getMaterialName());

			}
			//ent_c->setMaterialName(mat);
			ent_c->setCastShadows(ent_o->getCastShadows());
			clone->attachObject(ent_c);
			if(!clAny.isEmpty())
			{
				ent_c->setUserAny(clAny);
			}
		}
		else if(mType == "Ogre::Light")
		{
			Ogre::Light *light_o = static_cast<Ogre::Light*>(mobj);
			Ogre::String name = getUniqueLightName(mgr);

			//Ogre::String mat  = ent_o->getSubEntity(0)->getMaterialName();
			Ogre::Light *light_c = mgr->createLight(name);

			clone->attachObject(light_c);
			light_c->setType(light_o->getType());

            light_c->setVisible(light_o->getVisible());
            light_c->setCastShadows(light_o->getCastShadows());
            light_c->setPosition(light_o->getPosition());
            light_c->setDirection(light_o->getDirection());

            light_c->setDiffuseColour(light_o->getDiffuseColour());
            light_c->setSpecularColour(light_o->getSpecularColour());

			Ogre::Real aRange  = light_o->getAttenuationRange();
			Ogre::Real aConst  = light_o->getAttenuationConstant();
			Ogre::Real aLinear = light_o->getAttenuationLinear();
			Ogre::Real aQuad   = light_o->getAttenuationQuadric();

			light_c->setAttenuation(aRange,aConst,aLinear,aQuad);

			light_c->setPowerScale(light_o->getPowerScale());

			if(light_c->getType() == Ogre::Light::LT_SPOTLIGHT)
			{
			    light_c->setSpotlightInnerAngle(light_o->getSpotlightInnerAngle());
			    light_c->setSpotlightOuterAngle(light_o->getSpotlightOuterAngle());
			    light_c->setSpotlightFalloff(light_o->getSpotlightFalloff());
			}



			if(!clAny.isEmpty())
			{
				light_c->setUserAny(clAny);
			}

		}
		else
		{
			mLog("cloneSceneNode: WTF?! "+mType);
		}


	}
	return clone;

}

Ogre::AxisAlignedBox getSceneNodeBoundingBox(Ogre::SceneNode *node,bool ignoreParticleEffects)
{
	//Ogre::Vector3 nPos = ;
	//Ogre::AxisAlignedBox res(nPos,nPos);
	Ogre::Vector3 mMin = node->_getDerivedPosition();
	Ogre::Vector3 mMax = mMin;
	
	Ogre::SceneNode::ChildNodeIterator nodei = node->getChildIterator();
	while (nodei.hasMoreElements())
	{
	   Ogre::SceneNode* childNode = static_cast<Ogre::SceneNode*>(nodei.getNext());
	   Ogre::AxisAlignedBox cBox = getSceneNodeBoundingBox(childNode);
	   Ogre::Vector3 cMin = cBox.getMinimum();
	   Ogre::Vector3 cMax = cBox.getMaximum();

	   if(mMin.x > cMin.x)
		   mMin.x = cMin.x;
	   if(mMin.y > cMin.y)
		   mMin.y = cMin.y;
	   if(mMin.z > cMin.z)
		   mMin.z = cMin.z;

	   if(mMax.x < cMax.x)
		   mMax.x = cMax.x;
	   if(mMax.y < cMax.y)
		   mMax.y = cMax.y;
	   if(mMax.z < cMax.z)
		   mMax.z = cMax.z;
	}

	Ogre::SceneNode::ObjectIterator iter = node->getAttachedObjectIterator();
	while (iter.hasMoreElements())
	{
		Ogre::MovableObject* mobj = iter.getNext();
		Ogre::String type = mobj->getMovableType();
		/*if(ignoreParticleEffects && type =="ParticleSystem")
			continue;*/
		if(type != "Ogre::Entity" && type != "Billboard")
			continue;
		//Ogre::String mType = mobj->getMovableType();
		Ogre::AxisAlignedBox cBox = mobj->getWorldBoundingBox();
		if(!cBox.isFinite())
			continue;
		/*if(cBox.getMaximum().y > 100)
		{
			mLog("hier");
		}*/

		Ogre::Vector3 cMin = cBox.getMinimum();
		Ogre::Vector3 cMax = cBox.getMaximum();

		if(mMin.x > cMin.x)
		   mMin.x = cMin.x;
		if(mMin.y > cMin.y)
		   mMin.y = cMin.y;
		if(mMin.z > cMin.z)
		   mMin.z = cMin.z;

		if(mMax.x < cMax.x)
		   mMax.x = cMax.x;
		if(mMax.y < cMax.y)
		   mMax.y = cMax.y;
		if(mMax.z < cMax.z)
		   mMax.z = cMax.z;
	}
	return Ogre::AxisAlignedBox(mMin,mMax);
	//return res;
}

time_t getFileDate( Ogre::String fileName )
{
	struct stat mStat;
	stat(fileName.c_str(),&mStat);
	return mStat.st_mtime;
	/*test.
	WIN32_FILE_ATTRIBUTE_DATA attr;
	GetFileAttributesEx(fileName.c_str(),GetFileExInfoStandard,static_cast<void*>(&attr));
	SYSTEMTIME fileTime;
	FileTimeToSystemTime(attr.ftLastWriteTime,&fileTime);
	fileTime.*/

	//attr.ftLastWriteTime.
	

}

bool objectIsStatic(Ogre::String strType)
{
	if(strType == "static")
		return true;
	if(strType == "invalid")
		return true;
	if(strType == "light")
		return true;
	if(strType == "entrance")
		return true;
	if(strType == "group")
		return true;
	if(strType == "door")
		return true;
    if(strType == "water")
		return true;
	return false;			
}

bool FileExists( Ogre::String fileName )
{
    FILE* fp = NULL;

    //will not work if you do not have read permissions

    //to the file, but if you don't have read, it

    //may as well not exist to begin with.

	fp = fopen( fileName.c_str(), "rb" );
    if( fp != NULL )
    {
        fclose( fp );
        return true;
    }

    return false;
}


//testfunktion
void updateSceneManagersAfterMaterialsChange()
{
	Ogre::Root *m_Root = Ogre::Root::getSingletonPtr();
   if(m_Root && (Ogre::Pass::getDirtyHashList().size()!=0 || Ogre::Pass::getPassGraveyard().size()!=0))
   {
      Ogre::SceneManagerEnumerator::SceneManagerIterator scenesIter = m_Root->getSceneManagerIterator();
      
      while(scenesIter.hasMoreElements())
      {
          Ogre::SceneManager* pScene = scenesIter.getNext();
          if(pScene)
          {
            Ogre::RenderQueue* pQueue = pScene->getRenderQueue();
            if(pQueue)
            {
               Ogre::RenderQueue::QueueGroupIterator groupIter = pQueue->_getQueueGroupIterator();
               while(groupIter.hasMoreElements())
               {
                  Ogre::RenderQueueGroup* pGroup = groupIter.getNext();
                  if(pGroup)
                     pGroup->clear(false);
               }//end_while(groupIter.hasMoreElements())
            }//end_if(pScene)
          }//end_if(pScene)
      }//end_while(scenesIter.hasMoreElements())      
      
      // Now trigger the pending pass updates
        Ogre::Pass::processPendingPassUpdates();

   }//end_if(m_Root..
}
#ifdef __editor
bool createQtPixmapFromResource(QPixmap &resultPixmap, Ogre::String resName, Ogre::String resGroup,bool onlySquare, size_t resultSize)
{
	//resultPixmap = QPixmap;
	//let's do it
	bool couldLoad = false;

	if(Ogre::StringUtil::endsWith(resName,"dds"))
	{
		return false;//now I hate dds
	}

	Ogre::ResourceGroupManager *resMgr = Ogre::ResourceGroupManager::getSingletonPtr();
	
	Ogre::DataStreamPtr data = resMgr->openResource(resName,resGroup);//opening the resource
	
	//writing it into an uchar buffer
	Ogre::uchar *dataBuffer = OGRE_ALLOC_T(Ogre::uchar,data->size(),Ogre::MEMCATEGORY_RESOURCE);
	data->read(dataBuffer,data->size());

	
	
	//try to load it
	if(!resultPixmap.loadFromData(dataBuffer,data->size()))
	{		

		//when this strange codec problem has been fixed, put this back in


		//OGRE_FREE(dataBuffer,Ogre::MEMCATEGORY_RESOURCE);
		//attempt something else. more complicated for example?
		Ogre::Image img;
		img.load(resName,resGroup);
		//try to pre-validate the image a little
		/*if(img.getWidth() == img.getHeight())
		{*/
		
		//}
		//okay, so resizing using Ogre won't work. I guess I have no choice then...
		size_t imgWidth = img.getWidth();
		size_t imgHeight= img.getHeight();
		QImage tempImage(imgWidth,imgHeight,QImage::Format_ARGB32);

		//I'ä, Cthulhu Fhtagn!
		for(size_t y = 0;y<imgHeight;y++)
		{
			QRgb *scanLine = reinterpret_cast<QRgb*>(tempImage.scanLine(y));
			for(size_t x = 0;x<imgWidth;x++)
			{
				Ogre::ColourValue curOgreColor = img.getColourAt(x,y,0);
				QColor curQtColor;
				curQtColor.setRgbF(curOgreColor.r,curOgreColor.g,curOgreColor.b,curOgreColor.a);
				scanLine[x] = curQtColor.rgba();
			}
		}
		tempImage.save("asdtest.png");

		tempImage = tempImage.scaled(resultSize,resultSize,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
		tempImage.save("asdtest2.png");
		
		resultPixmap = QPixmap::fromImage(tempImage);
		resultPixmap.save("asdtest3.png");
		couldLoad = true;
		
		
		//so, now I will use dark magic

		////the preview images are only 64x64, so scale it to make the following dark magic a little easier
		//PixelBox srcBox = img.getPixelBox();
		//PixelBox targetBox(64,64,1,Ogre::PF_R8G8B8A8);
		//
		//Image::scale(srcBox,targetBox);
		//
		//targetBox.setConsecutive();
		//size_t test = targetBox.getConsecutiveSize();
		//size_t test2=64*64*4;
		//and now...
		//resultPixmap.loadFromData(targetBox.data,64*64*4);
		//img.resize(64,64);

			

		//QImage tempImage(64,64,QImage::Format_ARGB32);

		
		/*resultPixmap.fromImage(tempImage);
		*/

		
		//img.getDepth
		
		

		//data = img.encode(".png");//try to encode it in png. this might work
		////now do the same magic to allocate stuff and load
		//Ogre::uchar *dataBuffer = OGRE_ALLOC_T(Ogre::uchar,data->size(),Ogre::MEMCATEGORY_RESOURCE);
		//data->read(dataBuffer,data->size());
		//if(pixmap.loadFromData(dataBuffer,data->size()))
		//	couldLoad = true;
		//OGRE_FREE(dataBuffer,Ogre::MEMCATEGORY_RESOURCE);
	}
	else
	{
		couldLoad = true;
	}
	OGRE_FREE(dataBuffer,Ogre::MEMCATEGORY_RESOURCE);
	return couldLoad;
}
#endif