#include <algorithm>
#include <iostream>  //! \todo TODO:: remove later

#include <QtAlgorithms>

#include "Subject.h"
#include "Observer.h"

using namespace std;

Subject::Subject()
{
}

void Subject::Register(Observer* observer)
{
    DoRegister(observer);
}

void Subject::DoRegister(Observer* observer)
{
    if (qFind(observers.begin(), observers.end(), observer) == observers.end())
    if (find(observers.begin(), observers.end(), observer) == observers.end())
    {
        observers.push_back(observer);
    }

   //TODO: throw exception
}

void Subject::UnRegister(Observer* observer)
{
    DoUnRegister(observer);
}

void Subject::DoUnRegister(Observer* observer)
{
    QList<Observer*>::iterator iter = qFind(observers.begin(), observers.end(), observer);

    if (iter != observers.end())
    {
        observers.erase(iter);
    }

    //TODO: throw exception
}

void Subject::Notify(const QSharedPointer<SearchResult>& result)
{
    DoNotify(result);
}

void Subject::DoNotify(const QSharedPointer<SearchResult>& result)
{
    QList<Observer*>::const_iterator iter = observers.begin();

    for (; iter != observers.end(); ++iter)
    {
        (*iter)->Update(result);
    }
}

QList<Observer*>& Subject::GetObservers()
{
    return observers;
}
