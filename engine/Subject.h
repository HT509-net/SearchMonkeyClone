#ifndef SUBJECT_H
#define SUBJECT_H

#include <QList>
#include <QSharedPointer>

class Observer;
struct SearchResult;

class Subject
{
public:
    Subject();
    virtual ~Subject() {}
    void Register(Observer* observer);
    void UnRegister(Observer* observer);
    void Notify(const QSharedPointer<SearchResult>& result);

protected:
    QList<Observer*>& GetObservers();

private:
    virtual void DoRegister(Observer* observer);
    virtual void DoUnRegister(Observer* observer);
    virtual void DoNotify(const QSharedPointer<SearchResult>& result);
    QList<Observer*> observers;

};

#endif // SUBJECT_H
