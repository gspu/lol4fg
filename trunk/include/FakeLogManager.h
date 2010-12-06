#ifndef __praFakeLog
#define __praFakeLog

#include "log.h"

/*BAH!!!!!!!!
Ich will kein so bescheuertes logfenster!*/

class FakeLog: public QtOgre::Log{
    Q_OBJECT
public:
    FakeLog(const QString& name):Log(name)
    {       
        
    }//name und parent interessieren mich nicht
    ~FakeLog(){}
    void logMessage(const QString& message, QtOgre::LogLevel logLevel);
private:

};

#endif