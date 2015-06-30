#include "SearcherDecorator.h"

SearcherDecorator::SearcherDecorator(Searcher* searcher)
:
searcher(searcher)
{
  // need to get notified for prev result so we can do own search here
   searcher->Register(this);
}
SearcherDecorator::~SearcherDecorator()
{
   searcher->UnRegister(this);
   delete searcher;
}
void SearcherDecorator::DoUpdate(const Result& result)
{
    Notify(result);
}
void SearcherDecorator::DoSearch()
{
   searcher->Search();
}
void SearcherDecorator::DoStopSearch()
{
   searcher->StopSearch();
}
