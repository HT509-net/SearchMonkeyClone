#ifndef OBSERVER_H
#define OBSERVER_H

#include <QSharedPointer>

class SearchResult;
class Observer
{
public:
   Observer();
   virtual ~Observer() {}
   void Update(const QSharedPointer<SearchResult>& result);

private:
   virtual void DoUpdate(const QSharedPointer<SearchResult>& result) = 0;
};

#endif // OBSERVER_H
