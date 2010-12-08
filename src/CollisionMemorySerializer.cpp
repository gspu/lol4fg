#include "CollisionMemorySerializer.h"




void CollisionMemorySerialiser::exportTreeCollision(const CollisionPrimitives::TreeCollision* collision, DataStreamPtr& stream)
{        
    NewtonTreeCollisionSerialize(collision->m_col,&TreeCollisionSerializer::_newtonSerializeCallback,&stream);
}


void CollisionMemorySerialiser::importTreeCollision(Ogre::DataStreamPtr& stream, CollisionPrimitives::TreeCollision* pDest)
{
    NewtonCollision* col=NewtonCreateTreeCollisionFromSerialization(pDest->getWorld()->getNewtonWorld(), NULL, &TreeCollisionSerializer::_newtonDeserializeCallback, &stream);
    pDest->m_col=col;
}


void CollisionMemorySerialiser::_newtonSerializeCallback(void* serializeHandle, const void* buffer, size_t size)
{
    TreeCollisionSerializer* me=(static_cast<TreeCollisionSerializer*>(serializeHandle));
    me->writeData(buffer,1,size);
}


void CollisionMemorySerialiser::_newtonDeserializeCallback(void* deserializeHandle, void* buffer, size_t size)
{
    Ogre::DataStreamPtr ptr=*(static_cast<Ogre::DataStreamPtr*>(deserializeHandle));
    ptr->read(buffer,size);
}


