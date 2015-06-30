#ifndef SMFILE_H
#define SMFILE_H

#include <QFileInfo>
#include <QRegExp>
#include <QString>

#include "Searchable.h"

// SMFile represents the class can do the content search inside file
class SMFile : public Searchable
{
public:
	 // Todo:: does invertSearch option apply to phase two search?
    SMFile(unsigned sessionId, const QFileInfo& info);
    ~SMFile();

protected:
    bool IsLineMatched(const QString& line, const QString& keyword,
							  const Qt::CaseSensitivity caseSensitive,
									  QRegExp::PatternSyntax syntax);
	 unsigned searchSessionId;
    QFileInfo fileInfo;
	 bool stopIssued;

private:
	 virtual void DoSearch(const QStringList& keywords,
								  const Qt::CaseSensitivity& caseSensitive,
										  QRegExp::PatternSyntax syntax =QRegExp::RegExp);
    virtual void DoStopSearch();
};

/*for future
class SMPdf : public SMFile
{};*/

#endif
