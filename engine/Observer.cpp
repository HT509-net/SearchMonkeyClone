#include "Observer.h"

Observer::Observer()
{
}

void Observer::Update(const QSharedPointer<SearchResult>& result)
{
    DoUpdate(result);
}
