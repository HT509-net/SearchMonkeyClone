#ifndef FILETIMESEARCHER_H
#define FILETIMESEARCHER_H

#include <QDateTime>

#include "SearcherDecorator.h"
#include "../SearchResult.h"
#include "../Types.h"

class Searcher;
class PhaseOneParameter;

class FileTimeSearcher : public SearcherDecorator
{
public:
    FileTimeSearcher(Searcher* searcher, const PhaseOneParameter& param);

private:
    virtual void DoUpdate(const Result& result);
    QDateTime GetTimeFrom(const Result& result, TimeType type);
    QDateTime beginTime;
    QDateTime endTime;
    TimeType matchedTimeType;
};

#endif
