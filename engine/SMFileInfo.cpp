#include "debug.h"

#include "../SearchResult.h"
#include "SMFileInfo.h"

SMFileInfo::SMFileInfo(unsigned sessionId, const QFileInfo& info, bool invertSearch)
:  //Todo: need to take in invertSearch flag
searchSessionId(sessionId),
fileInfo(info),
invertSearch(invertSearch)
{}

SMFileInfo::~SMFileInfo()
{
}

void SMFileInfo::DoSearch(const QStringList& keywords, const Qt::CaseSensitivity& caseSensitive, QRegExp::PatternSyntax syntax)
{
  Entex ee("SMFileInfo::DoSearch", 4);

  qd(3) << ee.fn() << "filename-" << fileInfo.fileName() << " keys-" << keywords << " cs-" << caseSensitive << " syntax-" << syntax;

  bool matched = false;
  int size = keywords.size();
  for (int i = 0; i < size  &&  !matched; ++i) {
    QRegExp matcher(keywords[i], caseSensitive, QRegExp::Wildcard);
    if (matcher.indexIn(fileInfo.fileName()) != -1)
      matched = true;
  }
  qd(4) << "matched-" << matched << " invertSearch-" << invertSearch;

  if ((matched && !invertSearch) || (!matched && invertSearch)) {
    Result result = Result(new SearchResult(searchSessionId, fileInfo));
    Notify(result);
  }
}
