#ifndef SEARCHTHREAD_H
#define SEARCHTHREAD_H

#include <QRunnable>

#include "Subject.h"
#include "Searcher.h"
#include "../SearchResult.h"
#include "../SearchParameter.h"

class SearchThread : public QObject, public QRunnable, public Subject, public Observer
{
Q_OBJECT
public:
   SearchThread(const SearchParameter& param);
   void run();
   void StopSearch();

signals:
   void SearchFinished(unsigned sessionId);

private:
   virtual void DoUpdate(const Result& result);

   unsigned searchSessionId;
   Searcher* searcher;
};

#endif // SEARCHTHREAD_H
