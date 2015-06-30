#include "debug.h"

#include <iostream>

#include <QModelIndex>

#include "displaythread.h"
#include "engine/SearchThread.h"
#include "SearchCoordinator.h"

using namespace std;

SearchCoordinator::SearchCoordinator()
	: searching(false)
{
	pool.setMaxThreadCount(3);
}
void SearchCoordinator::DoUpdate(const Result& result)
{
	cout << "SearchCoordinator::DoUpdate()" << endl;
	cout << "****************" << endl;
	cout << *result;
	cout << "****************" << endl;

	emit OneSearchDone(result);
}
void SearchCoordinator::SearchDone(unsigned sessionId)
{
	 qDebug() << "before remove in SearchDone(), threads.size(): " << threads.size();
	 threads.remove(sessionId);
	 qDebug() << "after remove in SearchDone(), threads.size(): " << threads.size();

	 emit SearchFinished(sessionId);
}
void SearchCoordinator::StartDisplay(TabResults *tabResults, QModelIndex index)
{
	 Entex ee("SearchCoordinator::StartDisplay", 1);

	 DisplayThread *thread = new DisplayThread(tabResults, index);

	 qDebug() << ee.fn() << "display max thread cnt: " << pool.maxThreadCount() << " active thread before pool.start(): " << pool.activeThreadCount();
	 pool.start(thread);
	 qDebug() << "display active thread after pool.start(): " << pool.activeThreadCount();
}
void SearchCoordinator::StartSearch(const SearchParameter& param)
{
	 Entex ee("SearchCoordinator::StartSearch", 1);

	 searching = true;

    SearchThread* thread = new SearchThread(param);
    thread->Register(this);

    //QObject::connect(thread, SIGNAL(SearchFinished(unsigned)), this, SIGNAL(SearchFinished(unsigned)), Qt::DirectConnection);
    QObject::connect(thread, SIGNAL(SearchFinished(unsigned)), this, SLOT(SearchDone(unsigned)), Qt::DirectConnection);

    threads.insert(param.phaseOneParam->GetSessionId(), thread);

	 qDebug() << ee.fn() << "max thread cnt: " << pool.maxThreadCount() << " active thread before pool.start(): " << pool.activeThreadCount();
    pool.start(thread);
	 qDebug() << "active thread after pool.start(): " << pool.activeThreadCount();
}
void SearchCoordinator::StopSearch(unsigned sessionId)
{
    QMap<unsigned, SearchThread*>::const_iterator iter = threads.find(sessionId);

    // we need to make sure the SearchThread has not been deleted by thread pool
    if (iter != threads.end() && iter.value() != NULL)
    {
       SearchThread* thread = iter.value();
       thread->StopSearch();
    }
	 searching = false;
}
