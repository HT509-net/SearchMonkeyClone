#include "Searchable.h"

void Searchable::DoStopSearch()
{
   // provide this default implementation which does nothing for
   // some classes(like SMFileInfo) in the hierarchy
}
void Searchable::Search(const QStringList& keywords,
                const Qt::CaseSensitivity& caseSensitive,
                QRegExp::PatternSyntax syntax)
{
   DoSearch(keywords, caseSensitive, syntax);
}
void Searchable::StopSearch()
{
    DoStopSearch();
}
