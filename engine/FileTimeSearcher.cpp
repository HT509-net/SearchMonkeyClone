#include "debug.h"

#include "FileTimeSearcher.h"
#include "Searcher.h"
#include "../SearchParameter.h"
#include "../Utility.h"

FileTimeSearcher::FileTimeSearcher(Searcher* searcher, const PhaseOneParameter& param)
:
SearcherDecorator(searcher)
{
    beginTime = param.GetBeginTime();
    endTime = param.GetEndTime();
    matchedTimeType = param.GetMatchedTimeType();

    qDebug() << "FileTimeSearcher::ctor";
    qDebug() << "beginTime:" << beginTime;
    qDebug() << "endTime:" << endTime;
}

void FileTimeSearcher::DoUpdate(const Result& result)
{
    QDateTime matchedFileTime = GetTimeFrom(result, matchedTimeType);

    qDebug() << "FileTimeSearcher::DoUpdate()";
    qDebug() << "matchedFileTime: " << matchedFileTime;

    // no size bound limit
    if (beginTime.isNull() && endTime.isNull())
    {
  // throw exception
  // should not happen
    }

    if (IsValueInRange(matchedFileTime, beginTime, endTime))
    {
  // ok, meet requirement, pass it along
  qDebug() << "match meet time requirement";
  Notify(result);
    }
}

QDateTime FileTimeSearcher::GetTimeFrom(const Result& result, TimeType type)
{
    switch (type)
    {
  case AccessedTime:
     return result->matchedFile.lastRead();
  case ModifiedTime:
     return result->matchedFile.lastModified();
  case CreatedTime:
     return result->matchedFile.created();
  default:
    Q_ASSERT(0);
    return result->matchedFile.lastModified();
   }
}
