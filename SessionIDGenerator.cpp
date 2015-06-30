#include "SessionIDGenerator.h"

SessionIDGenerator::SessionIDGenerator()
:
currentID(0)
{}

unsigned SessionIDGenerator::GetCurrentID() const
{
    return currentID;
}

unsigned SessionIDGenerator::NextID()
{
    ++currentID;

    return currentID;
}

void SessionIDGenerator::Reset()
{
    currentID = 0;
}
