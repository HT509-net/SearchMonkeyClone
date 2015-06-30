#ifndef CONTENTSEARCHER_H
#define CONTENTSEARCHER_H

#include <QRegExp>
#include <QStringList>

#include "SearcherDecorator.h"

class Searcher;
class PhaseTwoParameter;

class ContentSearcher : public SearcherDecorator
{
public:
    ContentSearcher(Searcher* searcher, const PhaseTwoParameter& param);

private:
   virtual void DoUpdate(const Result& result);

   QStringList contentKeyword;
    Qt::CaseSensitivity caseSensitive;
    QRegExp::PatternSyntax syntax;
};

#endif
