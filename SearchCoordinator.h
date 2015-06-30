#ifndef SEARCHCOORDINATOR_H
#define SEARCHCOORDINATOR_H

#include <QMap>
#include <QModelIndex>
#include <QThreadPool>

#include "engine/Observer.h"
#include "engine/Searcher.h"
#include "SearchParameter.h"
#include "SearchResult.h"

class SearchThread;
class TabResults;

class SearchCoordinator : public QObject, public Observer
{
Q_OBJECT
public:
   SearchCoordinator();
  inline bool IsSearching() const { return searching; }
   void StartSearch(const SearchParameter& param);
  void StartDisplay(TabResults *tabResults, QModelIndex index);
   void StopSearch(unsigned sessionId);

public slots:
   // added this slot to remove the thread from map(but not delete, thread
   // pool will take care of that) since it won't be able to stop anymore
    void SearchDone(unsigned sessionId);

signals:
   void OneSearchDone(const Result& result);
  void SearchFinished(unsigned sessionId);

private:
   virtual void DoUpdate(const Result& result);

  bool searching;
  QThreadPool pool;
   QMap<unsigned, SearchThread*> threads; 
};

#endif // SEARCHCOORDINATOR_H
