#ifndef SEARCHERDECORATOR_H
#define SEARCHERDECORATOR_H

#include "Searcher.h"
#include "../SearchResult.h"

class SearcherDecorator : public Searcher
{
public:
    SearcherDecorator(Searcher* searcher);
    virtual ~SearcherDecorator();

protected:
    Searcher* searcher;

private:
    virtual void DoSearch();
    virtual void DoStopSearch();
   virtual void DoUpdate(const Result& result);
};

#endif
