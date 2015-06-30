#include "debug.h"

#include <QTextStream>

#include "../SearchResult.h"
#include "SMFile.h"

SMFile::SMFile(unsigned sessionId, const QFileInfo& info)
:
searchSessionId(sessionId),
fileInfo(info),
stopIssued(false)
{}
SMFile::~SMFile()
{}
void SMFile::DoSearch(const QStringList& keyword, const Qt::CaseSensitivity& caseSensitive, QRegExp::PatternSyntax syntax)
{
  Entex ee("SMFile::DoSearch", 3);

  if (keyword.isEmpty())
    return;

  QFile file(fileInfo.filePath());
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    qDebug() << "file open failed";  //Todo:: throw exception
    return;
  }
  QTextStream in(&file);
  int lineNo = 1;

  // Todo:: maybe we should have some sort of SearchResult builder to build
  // SearchResult which has the knowledge of SearchSessionId and takes in a
  // QFileInfo as parameter
  Result result(new SearchResult(searchSessionId, fileInfo));

  while (!in.atEnd())
  {
    if (stopIssued == true)
      break;

    QString line = in.readLine();

    if (IsLineMatched(line, keyword[0], caseSensitive, syntax))
    {
      qd(4) << "found matched line";
      result->matchedLines.push_back(LineInfo(lineNo, line));
    }

    ++lineNo;
  }
  if (result->matchedLines.empty() == false)
    Notify(result);
}
void SMFile::DoStopSearch()
{
   stopIssued = true;
}
bool SMFile::IsLineMatched(const QString& line, const QString& keyword, const Qt::CaseSensitivity caseSensitive, QRegExp::PatternSyntax syntax )
{
  QRegExp matcher(keyword, caseSensitive, syntax);
  return matcher.indexIn(line, 0) != -1;
}
