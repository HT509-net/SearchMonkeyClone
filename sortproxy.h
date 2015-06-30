/******************************************************************************
 * NAME:       sortproxy.h
 *
 * PURPOSE:    Definitions for sort proxy.
 *
 * DATE:       September 28, 2009
 *
 * REVISIONS:
 * ---------
 * [092809] shawno- initial coding.
*******************************************************************************/
#ifndef SORTPROXY_H
#define SORTPROXY_H

#include <QDateTime>
#include <QModelIndex>
#include <QSortFilterProxyModel>

class SortFilterProxyModel : public QSortFilterProxyModel {
    Q_OBJECT

public:
    SortFilterProxyModel(QObject *parent = 0);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
	 QDateTime qstring2qdatetime(const QString & s) const;
};

#endif // SORTPROXY_H
