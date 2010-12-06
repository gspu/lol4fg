#ifndef __collisionmemoryserialiser
#define __collisionmemoryserialiser

#include "Ogre.h"
#include "OgreNewt.h"
using namespace Ogre;
using namespace OgreNewt;

class CollisionMemorySerialiser
{
public:
    CollisionMemorySerialiser(){}
    ~CollisionMemorySerialiser(){}
   
    void exportTreeCollision(const CollisionPrimitives::TreeCollision* collision, DataStreamPtr& stream);

    
    void importTreeCollision(Ogre::DataStreamPtr& stream, CollisionPrimitives::TreeCollision* pDest);

    /*!
    Callback function for Newton. It should never be called directly, but will be called by Newton to save the TreeCollision to a stream.
    (Newton calls this function several times for each serialization, once for each chunk of its file format apparently)
    */
    static void _CDECL _newtonSerializeCallback(void* serializeHandle, const void* buffer, size_t size);

    /*!
    Callback function for Newton. It should never be called directly, but will be called by Newton to load the TreeCollision from a stream.
    (Newton calls this function several times for each deserialization, once for each chunk of its file format apparently)
    */
    static void _CDECL _newtonDeserializeCallback(void* deserializeHandle, void* buffer, size_t size);
};

#endif