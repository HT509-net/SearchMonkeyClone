#include "debug.h"

#include "SearchParameter.h"
#include "Utility.h"

PhaseOneParameter::PhaseOneParameter(unsigned sessionId,
												 const QStringList& keywords,
												 const QString& startPath,
												 Settings *sui)
    :
    searchSessionId(sessionId),
	 keywords(keywords),
    startPath(startPath),
	 syntax(QRegExp::RegExp),
    caseSensitivity(Qt::CaseInsensitive),
	 recursive(true),
    followSymLink(false),
	 skipHidden(false),
    invertSearch(false),
	 recursiveDepth(MAX_DIRECTORY_DEPTH),
    sizeLowerBound(0),
	 sizeUpperBound(0),
	 m_sui(sui)
{}
PhaseTwoParameter::PhaseTwoParameter(const QString& keyword, Settings *sui)
	:
	keyword(keyword),
	caseSensitivity(Qt::CaseSensitive),
	syntax(QRegExp::RegExp),
	m_sui(sui)
{}
PhaseTwoParameter::PhaseTwoParameter(Settings *sui)
	:
	m_sui(sui)
{}
bool PhaseOneParameter::FollowSymLink() const
{
	 return followSymLink;
}
QDateTime PhaseOneParameter::GetBeginTime() const
{
	 return beginTime;
}
Qt::CaseSensitivity PhaseOneParameter::GetCaseSensitivity() const
{
	 return caseSensitivity;
}
Qt::CaseSensitivity PhaseTwoParameter::GetCaseSensitivity() const
{
	 return caseSensitivity;
}
QDateTime PhaseOneParameter::GetEndTime() const
{
	 return endTime;
}
QStringList PhaseOneParameter::GetKeywords() const
{
	 return keywords;
}
QStringList PhaseTwoParameter::GetKeyword() const
{
	 return keyword;
}
TimeType PhaseOneParameter::GetMatchedTimeType() const
{
	 return matchedTimeType;
}
unsigned PhaseOneParameter::GetRecursiveDepth() const
{
    return recursiveDepth;
}
unsigned PhaseOneParameter::GetSessionId() const
{
	 return searchSessionId;
}
unsigned PhaseOneParameter::GetSizeLowerBound() const
{
    return sizeLowerBound;
}
unsigned PhaseOneParameter::GetSizeUpperBound() const
{
    return sizeUpperBound;
}
QString PhaseOneParameter::GetStartPath() const
{
	 return startPath;
}
QRegExp::PatternSyntax PhaseOneParameter::GetSyntax() const
{
	 return syntax;
}
QRegExp::PatternSyntax PhaseTwoParameter::GetSyntax() const
{
	 return syntax;
}
bool PhaseOneParameter::HasSizeConstraint() const
{
    return (sizeLowerBound != 0 || sizeUpperBound != 0);
}
bool PhaseOneParameter::HasTimeConstraints() const
{
    return (beginTime.isNull() == false || endTime.isNull() == false);
}
bool PhaseOneParameter::InvertSearch() const
{
	return invertSearch;
}
bool PhaseOneParameter::IsRecursive() const
{
	 return recursive;
}
bool SearchParameter::PhaseTwoSearchNeeded() const
{
	 return phaseTwoParam != NULL;
}
void PhaseOneParameter::SetBeginTime(const QDateTime& time)
{
	 beginTime = time;
}
void PhaseOneParameter::SetCaseSensitivity(const Qt::CaseSensitivity& cs)
{
    caseSensitivity = cs;
}
void PhaseTwoParameter::SetCaseSensitivity(const Qt::CaseSensitivity& cs)
{
	 caseSensitivity = cs;
}
void PhaseOneParameter::SetEndTime(const QDateTime& time)
{
	 endTime = time;
}
void PhaseOneParameter::SetFollowSymLink(bool follow)
{
	 followSymLink = follow;
}
void PhaseOneParameter::SetInvertSearch(bool invert)
{
	 invertSearch = invert;
}
void PhaseOneParameter::SetMatchedTimeType(TimeType timeType)
{
	 matchedTimeType = timeType;
}
void PhaseOneParameter::SetRecursive(bool recursive)
{
    this->recursive = recursive;
}
void PhaseOneParameter::SetRecursiveDepth(unsigned depth)
{
    recursiveDepth = depth;
}
void PhaseOneParameter::SetSizeLowerBound(unsigned size)
{
    sizeLowerBound = size;
}
void PhaseOneParameter::SetSizeUpperBound(unsigned size)
{
    sizeUpperBound = size;
}
void PhaseOneParameter::SetSkipHidden(bool skip)
{
	 skipHidden = skip;
}
void PhaseOneParameter::SetSyntax(const QRegExp::PatternSyntax& syntax)
{
	 this->syntax = syntax;
}
void PhaseTwoParameter::SetSyntax(const QRegExp::PatternSyntax& syntax)
{
    this->syntax = syntax;
}
bool PhaseOneParameter::SkipHidden() const
{
	 return skipHidden;
}
