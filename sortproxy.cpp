/******************************************************************************
 * NAME:       sortproxy.cpp
 *
 * PURPOSE:    Provides custom, intuitive sorting for numeric strings and
 *             numerics.
 *
 * DATE:       September 28, 2009
 *
 * REVISIONS:
 * ---------
 * [092809] shawno- initial coding.
*******************************************************************************/
#include <QVariant>

#include "sortproxy.h"
#include "Utility.h"

SortFilterProxyModel::SortFilterProxyModel(QObject *parent)
              : QSortFilterProxyModel(parent) {}
/******************************************************************************
 * FilterAcceptRow(): optional row-filtering override.
 ******************************************************************************/
bool SortFilterProxyModel::filterAcceptsRow(int sourceRow,
                           const QModelIndex &sourceParent) const {
  (void) sourceRow, (void) sourceParent;
  return true;
}
/******************************************************************************
 * LessThan(): primary comparator override.
 ******************************************************************************/
bool SortFilterProxyModel::lessThan(const QModelIndex &left,
                                    const QModelIndex &right) const {
  QVariant leftData  = sourceModel()->data(left),
        rightData = sourceModel()->data(right);

  QString leftString  = leftData.toString(),
        rightString = rightData.toString();

  int leftInt  = leftData.toInt(),
     rightInt = rightData.toInt();

  if (left.column() == 1  &&  right.column() == 1)
    return units2bytes(leftString) < units2bytes(rightString);

  if (left.column() == 2  &&  right.column() == 2)
     return leftInt < rightInt;

  if (left.column() == 4  &&  right.column() == 4)
     return qstring2qdatetime(leftString) < qstring2qdatetime(rightString);

  return QString::localeAwareCompare(leftString, rightString) < 0;
}
/******************************************************************************
 * qstring2qdatetime(): auxillary method converts a QString with form:
 *                      <mm.dd.yy hh:mm:ss> into a QDateTime.
 ******************************************************************************/
QDateTime SortFilterProxyModel::qstring2qdatetime (const QString & s) const {
  if (s.isNull() || s.isEmpty() || (s.length() != 17))
    return QDateTime();

  QStringList sl = s.split(' ');
  QStringList mdy = sl[0].split('.');  // ("mm", "dd", "yy")
  QStringList hms = sl[1].split(':');  // ("hh", "mm", "ss")

  QDate date(mdy[2].toInt(), mdy[0].toInt(), mdy[1].toInt());  // y,m,d
  QTime time(hms[0].toInt(), hms[1].toInt(), hms[2].toInt());  // h,m,s

  return QDateTime(date, time);
}
