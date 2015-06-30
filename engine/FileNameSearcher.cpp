#include "debug.h"

#include "FileNameSearcher.h"
#include "Searchable.h"
#include "../SearchParameter.h"
#include "SMDir.h"

FileNameSearcher::FileNameSearcher(const PhaseOneParameter& param)
:
param(param)
{}
Searchable* FileNameSearcher::BuildSearchable()  // Todo:: wrap Searchable* in QSharedPointer
{
    Searchable* searchable = new SMDir(param);
    qDebug() << "FileNameSearcher::BuildSearchable() ";
    return searchable;
}
void FileNameSearcher::DoSearch()
{
  searchable = BuildSearchable();
  searchable->Register(this);

  qDebug() << "FileNameSearcher::DoSearch(): calling searchable->Search()";
  searchable->Search(param.GetKeywords(), param.GetCaseSensitivity(), param.GetSyntax());

  delete searchable;  // Todo::should we delete searchable here?
}
void FileNameSearcher::DoStopSearch()
{
  if (searchable != NULL)  // searchable has not done job so we can issue stop
    searchable->StopSearch();
}
void FileNameSearcher::DoUpdate(const Result& result)
{
   qd(4) << "FileNameSearcher::DoUpdate()";
    Notify(result);
}
