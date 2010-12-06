#ifndef __praSmallClasses
#define __praSmallClasses
/* für "kleine klassen", die nicht viel tun. sollte nach möglichkeit von keinen headern includet werden! */

#include "GameObject.h"
#include "Level.h"



//ne "pointerklasse", um probleme beim speichern/laden zu umgehen:
class ObjectPtr
{
public:
    
    //Konstruktor, wenn das obj exisiert. wozu auch immer    
    inline ObjectPtr(GameObject *obj)
    { 
        //lvl = obj->getLevel(); //irrelevant, wenn wir eh schon das obj haben
        sgid = obj->getSGID();
        this->obj = obj; 
    }
    //Konstruktor, wenn nur SGID bekannt, gleichzeitig defaultconstructor
    inline ObjectPtr(Level *level = NULL, int SGID = 0) 
    { 
        lvl = level; 
        sgid = SGID;
        obj = NULL;
    }
    
    inline GameObject* operator->() //const 
    { 
        if(!obj)
            find();
        return obj; 
    }
    inline void operator= (GameObject* n)
    {
        obj = n;
    }
    inline bool operator== (GameObject* cmp)
    {
        return (cmp == obj);
    }
    inline bool operator== (ObjectPtr ptr)
    {
        return (ptr.obj == obj);
    }
    inline bool isNull()
    {
        if(sgid == 0 && obj == NULL)
            return true;
        return false;
    }
    void setNull()
    {
        sgid = 0;
        obj = NULL;
    }

    GameObject *obj;
private:
    int sgid;
    
    Level *lvl;

    inline void find()
    {
        assert(sgid != 0); 
        obj = lvl->getObjectBySGID(sgid);
        assert(obj); 
    }
};


#endif