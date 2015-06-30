#ifndef SESSIONIDGENERATOR_H
#define SESSIONIDGENERATOR_H

#include "Singleton.h"

class SessionIDGenerator
{
public:
    SessionIDGenerator();
    unsigned NextID();
    unsigned GetCurrentID() const;
    void Reset();

private:
    unsigned currentID;
};

typedef Singleton<SessionIDGenerator> SessionIDGeneratorSingleton;

#endif
