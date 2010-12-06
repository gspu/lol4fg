/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2006 Torus Knot Software Ltd
Also see acknowledgements in Readme.html

You may use this sample code for anything you like, it is not covered by the
LGPL like the rest of the engine.
-----------------------------------------------------------------------------
*/


#ifndef _WATER_MESH_H_
#define _WATER_MESH_H_

#include "OgrePlatform.h"
#include "Ogre.h"

//fwdec
class WaterPlane;

using namespace Ogre ;

class WaterMesh
{
    friend class WaterCircle;
private:
	MeshPtr mesh ;
	SubMesh *subMesh ; 
	float *vertexBuffers[3] ; // we need 3 vertex buffers
	int currentBuffNumber ;
	int complexity ;
	String meshName ;
	int numFaces ;
	int numVertices ;
	Vector3* vNormals ;

	HardwareVertexBufferSharedPtr posVertexBuffer ;
	HardwareVertexBufferSharedPtr normVertexBuffer ;
	HardwareVertexBufferSharedPtr texcoordsVertexBuffer ;
	HardwareIndexBufferSharedPtr indexBuffer ;

    //shared stuff of all circles:
    // some buffers shared by all circles
	HardwareVertexBufferSharedPtr circle_posnormVertexBuffer ;
	HardwareIndexBufferSharedPtr circle_indexBuffer ; // indices for 2 faces
	HardwareVertexBufferSharedPtr *circle_texcoordsVertexBuffers ;

	Real lastTimeStamp ;
	Real lastAnimationTimeStamp;
	Real lastFrameTime ;
	
	void calculateFakeNormals();
	void calculateNormals();

    //ehem. methoden von WaterCircle
    void prepareCircleBuffers();
    //
    void clearCircleBuffers();
    void prepareCircleMaterial();
public:
    //MOD: planeSize is a Vector3 now, with x and z being the plane's sizes
	WaterMesh(const String& meshName, Vector3 planeSize, int complexity) ;
    
    virtual ~WaterMesh ();

    inline String getMeshName()
    {
        return meshName;
    }
    inline MeshPtr getMesh()
    {
        return mesh;
    }

	/** "pushes" a mesh at position [x,y]. Note, that x,y are float, hence 
	*	4 vertices are actually pushed
	*	@note 
	*		This should be replaced by push with 'radius' parameter to simulate
	*  		big objects falling into water
	*/
	void push(Real x, Real y, Real depth, bool absolute=false) ;

	/** gets height at given x and y, takes average value of the closes nodes */
	Real getHeight(Real x, Real y);

	/** updates mesh */
	void updateMesh(Real timeSinceLastFrame) ;
	
	Real PARAM_C ; // ripple speed 
	Real PARAM_D ; // distance
	Real PARAM_U ; // viscosity
	Real PARAM_T ; // time
	bool useFakeNormals ;
} ;

#define CIRCLE_SIZE 500.0
#define CIRCLE_TIME 0.5f
class WaterCircle
{
private:
	String name ;
	SceneNode *node ;
	MeshPtr mesh ;
	SubMesh *subMesh ;
	Entity *entity ;
	Real tm ;
	//static bool first ;
	

	float *texBufData;
	void _prepareMesh();
    WaterPlane *mParentObject;
    
public:
	int lvl ;
	void updateTextureLevel();
	WaterCircle(WaterPlane *parent,Real x, Real y);
	~WaterCircle();
	void animate(Real timeSinceLastFrame);
	
    inline String ptrAsStr()
    {
        long address = reinterpret_cast<long>(this);
	    char buffer[33];
	    //int base = 16;
	    ltoa(address,buffer,16);
	    return Ogre::String(buffer);
    }
} ;
//bool WaterCircle::first = true ;
//HardwareVertexBufferSharedPtr WaterCircle::posnormVertexBuffer =
//	HardwareVertexBufferSharedPtr() ;
//HardwareIndexBufferSharedPtr WaterCircle::indexBuffer =
//	HardwareIndexBufferSharedPtr() ;
//HardwareVertexBufferSharedPtr* WaterCircle::texcoordsVertexBuffers = 0 ;
//

#endif
