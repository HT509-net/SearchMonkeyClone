#ifndef SMFILEINFO_H
#define SMFILEINFO_H

#include <QFileInfo>

#include "Searchable.h"

// SMFileInfo models the class that only name searchable so the SearchResult
// returned by this Search() will have empty matched_lines
class SMFileInfo : public Searchable
{
public:
    SMFileInfo(unsigned sessionId, const QFileInfo& info, bool invertSearch);
    ~SMFileInfo();

private:
	 // we make SMFileInfo to do nothing when StopSearch() called since its
	 // Search() operation takes no time to complete so it doesn't really make
	 // lots of sense to stop it.
	 virtual void DoSearch(const QStringList& keywords,
								  const Qt::CaseSensitivity& caseSensitive,
										  QRegExp::PatternSyntax syntax =QRegExp::RegExp);
    unsigned searchSessionId;
    QFileInfo fileInfo;
    bool invertSearch;
};

#endif
