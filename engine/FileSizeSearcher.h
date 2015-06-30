#ifndef FILESIZESEARCHER_H
#define FILESIZESEARCHER_H

#include "SearcherDecorator.h"
#include "../SearchResult.h"

class Searcher;
class PhaseOneParameter;

class FileSizeSearcher : public SearcherDecorator
{
public:
    FileSizeSearcher(Searcher* searcher, const PhaseOneParameter& param);

private:
    virtual void DoUpdate(const Result& result);
    int sizeLowerBound;
    int sizeUpperBound;
};

#endif
