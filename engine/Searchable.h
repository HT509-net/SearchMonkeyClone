#ifndef SEARCHABLE_H
#define SEARCHABLE_H

#include <QRegExp>
#include <QStringList>

#include "Subject.h"

class Searchable : public Subject
{
public:
	virtual ~Searchable() {}
	void Search(const QStringList& keywords,
               const Qt::CaseSensitivity& caseSensitive,
					QRegExp::PatternSyntax syntax = QRegExp::RegExp);
   void StopSearch();

private:
	 virtual void DoSearch(const QStringList& keywords,
                          const Qt::CaseSensitivity& caseSensitive,
								  QRegExp::PatternSyntax syntax = QRegExp::RegExp) = 0;
    virtual void DoStopSearch();
};

#endif
