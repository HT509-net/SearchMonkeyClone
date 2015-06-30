#ifndef SEARCHER_H
#define SEARCHER_H

#include "Subject.h"
#include "Observer.h"

class Searcher : public Subject, public Observer
{
public:
    void Search();
    void StopSearch();
    virtual ~Searcher() {}

 private:
    virtual void DoSearch() = 0;
    virtual void DoStopSearch() = 0;
};

#endif
