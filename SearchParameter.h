#ifndef SEARCHPARAMETER_H
#define SEARCHPARAMETER_H

#include <QDateTime>
#include <QRegExp>
#include <QSharedPointer>
#include <QString>
#include <QStringList>

#include "Types.h"

class Settings;

class PhaseOneParameter
{
public:
   PhaseOneParameter(unsigned sessionId, const QStringList& keywords,
               const QString& startPath, Settings *sui);
   Qt::CaseSensitivity GetCaseSensitivity() const;
   bool FollowSymLink() const;
   QDateTime GetBeginTime() const;
   QDateTime GetEndTime() const;
   QStringList GetKeywords() const;
   TimeType GetMatchedTimeType() const;
   unsigned GetRecursiveDepth() const;
   unsigned GetSessionId() const;
   unsigned GetSizeLowerBound() const;
   unsigned GetSizeUpperBound() const;
   QString GetStartPath() const;
   QRegExp::PatternSyntax GetSyntax() const;
   bool HasSizeConstraint() const;
    bool HasTimeConstraints() const;
   bool InvertSearch() const;
   bool IsRecursive() const;
    bool SkipHidden() const;
   inline Settings *sui() const { return m_sui; }

    void SetCaseSensitivity(const Qt::CaseSensitivity& cs);
   void SetBeginTime(const QDateTime& time);
   void SetEndTime(const QDateTime& time);
   void SetFollowSymLink(bool follow);
   void SetInvertSearch(bool invert);
   void SetMatchedTimeType(TimeType timeType);
   void SetRecursive(bool recursive);
    void SetRecursiveDepth(unsigned depth);
    void SetSizeLowerBound(unsigned size);
   void SetSizeUpperBound(unsigned size);
   void SetSkipHidden(bool skip);
   void SetSyntax(const QRegExp::PatternSyntax& syntax);

private:
    unsigned searchSessionId;
   QStringList keywords;
    QString startPath;
    QRegExp::PatternSyntax syntax;
    Qt::CaseSensitivity caseSensitivity;
    bool recursive;
    bool followSymLink;
    bool skipHidden;
    bool invertSearch;
    unsigned recursiveDepth;
    unsigned sizeLowerBound;
    unsigned sizeUpperBound;
    QDateTime beginTime;
    QDateTime endTime;
    TimeType matchedTimeType;
   Settings *m_sui;
    // not needed anymore
   // UI don't differentiate normal and regexp case it just pass the string
   // along with flag we can construct regexp later ourself
    //QString fileNameRegExp;
    //QString contentRegExp;
};
class PhaseTwoParameter
{
public:
   PhaseTwoParameter(const QString& keyword, Settings *sui);
   PhaseTwoParameter(Settings *sui);
   Qt::CaseSensitivity GetCaseSensitivity() const;
   QStringList GetKeyword() const;
    QRegExp::PatternSyntax GetSyntax() const;

    void SetCaseSensitivity(const Qt::CaseSensitivity& cs);
    void SetSyntax(const QRegExp::PatternSyntax& syntax);
   inline Settings *sui() { return m_sui; }

private:
   QStringList keyword;
    Qt::CaseSensitivity caseSensitivity;
    QRegExp::PatternSyntax syntax;
   Settings *m_sui;
};
class SearchParameter
{
public:
   SearchParameter(QSharedPointer<PhaseOneParameter> one =
              QSharedPointer<PhaseOneParameter>(NULL),
              QSharedPointer<PhaseTwoParameter> two =
              QSharedPointer<PhaseTwoParameter>(NULL))
   :
    phaseOneParam(one),
   phaseTwoParam(two)
    {}

    bool PhaseTwoSearchNeeded() const;

    QSharedPointer<PhaseOneParameter> phaseOneParam;
    QSharedPointer<PhaseTwoParameter> phaseTwoParam;

private:
  Settings *m_sui;
};

#endif // SEARCHPARAMETER_H
