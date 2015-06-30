#include "SearcherFactory.h"
#include "SearchThread.h"

SearchThread::SearchThread(const SearchParameter& param)
:
searchSessionId(param.phaseOneParam->GetSessionId()),
searcher(NULL)
{
   searcher = SearcherFactory::BuildSearcher(param);
   searcher->Register(this);
}
void SearchThread::StopSearch()
{
    if (searcher != NULL)
        searcher->StopSearch();
}
void SearchThread::run()
{
    if (searcher != NULL)
    {
      searcher->Search();
      emit SearchFinished(searchSessionId);
      delete searcher;
    }
}
void SearchThread::DoUpdate(const Result& result)
{
   Notify(result);  // just pass along to the observers of SearchThread
}
