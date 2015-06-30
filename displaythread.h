#ifndef DISPLAYTHREAD_H
#define DISPLAYTHREAD_H

#include <QModelIndex>
#include <QObject>
#include <QRunnable>

class TabResults;

class DisplayThread : public QObject, public QRunnable
{
Q_OBJECT

public:
	DisplayThread(TabResults *_tr, QModelIndex _index)
	:
	tabResults(_tr),
	index(_index)
	{}
	void run();

signals:

private:
	TabResults *tabResults;
	QModelIndex index;
};

#endif // DISPLAYTHREAD_H
