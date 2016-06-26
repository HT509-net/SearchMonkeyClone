/******************************************************************************
 * NAME:       Utility.h
 *
 * PURPOSE:    Header for Utility.cpp and miscellaneous Searchmonkey constants.
 *
 * DATE:       September 6, 2009
 *
 * REVISIONS:
 * ---------
 * [090609] shawno- bytes2units(), units2bytes()
 * [091609] shawno- listSubtract(), ms2units()
 * [112209] shawno- chop(), parseDelim(), strip()
 * [011010] shawno- positionAtLine()
 * [022710] Xiao- lib.h combined with Utility.h
*******************************************************************************/
#ifndef UTILITY_H
#define UTILITY_H

#include <algorithm>
#include <functional> // bind1st deprecated
#include <string>

#include <QComboBox>
#include <QDateTime>
#include <QFile>
#include <QListWidget>
#include <QMessageBox>
#include <QRegExp>
#include <QString>
#include <QStringList>

using std::string;

typedef unsigned long long ull;

const string SM_CR = string (1, (char) 13);
const string SM_LF = string (1, (char) 10);
const string SM_SP = string (1, (char) 32);

const qint16 MAX_DIRECTORY_DEPTH = 256;
const qint16 MAX_SEARCH_TABS = 128;

const Qt::GlobalColor COLOR_TEXT = Qt::black;
const Qt::GlobalColor COLOR2_HIGHLIGHT = Qt::red;
const QColor COLOR1_HIGHLIGHT = QColor(220,0,3);    // darker, less annoying red
const QColor COLOR_HYPERLINK = QColor(6,105,205);  // RGB hyperlink blue

const QString CONFIG_FILE = "./.smrc";

const quint64 KB = Q_UINT64_C(1024);               // 1024
const quint64 MB = Q_UINT64_C(1048576);            // 1024x1024
const quint64 GB = Q_UINT64_C(1073741824);         // 1024x1024x1024
const quint64 TB = Q_UINT64_C(1099511627776);      // 1024x1024x1024x1024
const quint64 PB = Q_UINT64_C(1125899906842624);   // 1024x1024x1024x1024x1024

bool BoundSpecified(int boundValue);
bool BoundSpecified(QDateTime boundValue);
bool IsValidDir(const QString& dir);
bool validRx(const QString &_s, QRegExp::PatternSyntax _syntax);
bool validRx(const QStringList &_sl, QRegExp::PatternSyntax _syntax);
char* strip(char* pz, const char* g, int* n = NULL);
bool boxConfirm(const QString &s,
           const QMessageBox::Icon = QMessageBox::Warning);
void boxError(const QString &s, const QMessageBox::Icon = QMessageBox::Warning);
void listWidget2List(QListWidget *lw, QStringList *sl);
void list2ListWidget(const QStringList &sl, QListWidget *lw);
void updateComboBox(QComboBox *comboBox);
void* parseDelim(const char* str, const char* delims,char*** flds,int* n_found);
qint64 positionAfterLine(QFile &, const QString &);
qint64 units2bytes(const QString &);
string* strip(string* ps, const string& g, int* n = NULL);
QString bytes2units(qint64);
QString chop(QString s, int n);
QString getLine(QFile & _file);
QStringList listSubtract(const QStringList &_list1, const QStringList &_list2);
QString ms2units(quint64);
QRegExp::PatternSyntax uint2syntax(uint _index);

/* this helper function is used for easier call a member function of Caller on
  a container of Callee object, this member function take 1 parameter which is
  the callee object ie, the scenario like this:

    QList<Observer*> observers = GetObservers();
    QList<Observer*>::const_iterator iter = observers.begin();
    for (; iter != observers.end(); ++iter)
    {
        searchable->Register(*iter);
    }

    with this help function in place, the code above can be written as:
    CallEach(observers, searchable, &Subject::Register);
    here the Subject is the type of Caller
*/
template <typename CalleeContainer, typename Caller, typename Op>
void CallEach(const CalleeContainer& container, Caller& caller, Op op)
{

    std::for_each(container.begin(), container.end(), std::bind1st(std::mem_fun(op), caller));
}
template <typename T>
bool BoundSpecified(T boundValue)
{
    return false;
}
template <typename T>
bool LowerBoundFit(T value, T lowerBound)
{
    if (BoundSpecified(lowerBound) == false)
        return true;

    if (value >= lowerBound)
        return true;

    return false;
}
template <typename T>
bool UpperBoundFit(T value, T upperBound)
{
    if (BoundSpecified(upperBound) == false)
        return true;

    if (value <= upperBound)
        return true;

    return false;
}
template <typename T>
bool IsValueInRange(T value, T lowerBound, T upperBound)
{
    return LowerBoundFit(value, lowerBound) && UpperBoundFit(value, upperBound);
}

#endif
