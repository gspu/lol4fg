#include "Listener.h"


//singleton
template<> SimpleSound::Listener* Ogre::Singleton<SimpleSound::Listener>::ms_Singleton = 0;