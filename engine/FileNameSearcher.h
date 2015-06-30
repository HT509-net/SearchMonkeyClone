#ifndef FILENAMESEARCHER_H
#define FILENAMESEARCHER_H

#include "Searcher.h"
#include "../SearchParameter.h"
#include "../SearchResult.h"

class Searchable;
class PhaseOneParameter;

class FileNameSearcher : public Searcher
{
public:
    FileNameSearcher(const PhaseOneParameter& param);

protected:
    Searchable* BuildSearchable();

private:
    virtual void DoSearch();
    virtual void DoStopSearch();
	 virtual void DoUpdate(const Result& result);

	 PhaseOneParameter param;
    Searchable* searchable;
};

#endif
