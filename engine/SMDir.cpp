#include "debug.h"

#include <QDir>
#include <QFileInfoList>
#include <QSharedPointer>

#include "dir.h"
#include "../SearchParameter.h"
#include "settings.h"
#include "SMDir.h"
#include "SMFileInfo.h"
#include "Utility.h"

SMDir::SMDir(const PhaseOneParameter& param)
:
searchSessionId(param.GetSessionId()),
path(param.GetStartPath()),
recursive(param.IsRecursive()),
recursiveDepth(param.GetRecursiveDepth()),
sui(param.sui()),
skipHidden(param.SkipHidden()),
followSymLink(param.FollowSymLink()),
invertSearch(param.InvertSearch()),
stopIssued(false)
{}
SMDir::SMDir(unsigned sessionId, const QString& path, bool recursive,
         unsigned recursiveDepth, Settings *sui, bool skipHidden, bool followSymLink,
         bool invertSearch)
:
searchSessionId(sessionId),
path(path),
recursive(recursive),
recursiveDepth(recursiveDepth),
sui(sui),
skipHidden(skipHidden),
followSymLink(followSymLink),
invertSearch(invertSearch),
stopIssued(false)
{}
SMDir::~SMDir()
{}
QSharedPointer<Searchable> SMDir::BuildSearchable(const QFileInfo& info)
{
  QSharedPointer<Searchable> nil = QSharedPointer<Searchable>(NULL);

  if (info.exists() == false)
    return nil;
  if (info.isHidden() && skipHidden)
    return nil;

  if (info.isSymLink())
  {
    if (followSymLink)
    {
      QFileInfo target(info.symLinkTarget());
      // recursive call here so we don't even have to test if it exists or not
      // it's already been done in the beginning of this function
      BuildSearchable(target);
    }
    else
      return nil;
  }
  // now we need to test if it's regular file or directory reference by info
  if (info.isFile())
    return QSharedPointer<Searchable>(new SMFileInfo(searchSessionId, info, invertSearch));
  else
  {
    if (recursive && recursiveDepth > 0)
    {
      if (sui->getDir()->dirFilterSz())
        if (sui->getDir()->filterDir(info.dir().dirName()) == false)
          return nil;

      return QSharedPointer<Searchable>(new SMDir(searchSessionId,
           info.filePath(), recursive, recursiveDepth - 1, sui, skipHidden,
           followSymLink, invertSearch));
    }
    else
      return nil;
  }
}
void SMDir::DoStopSearch()
{
    stopIssued = true;
}
void SMDir::DoSearch(const QStringList& keywords, const Qt::CaseSensitivity& caseSensitive, QRegExp::PatternSyntax syntax)
{
  Entex ee("SMDir::DoSearch", 4);
  qd(4) << ee.fn() << "searching path-" << path;

  QDir dir(path);
  QFileInfoList entries;

  if (recursive == false)
    entries = dir.entryInfoList(QDir::Files | QDir::Hidden);
  else
    entries = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);

  qd(3) << ee.fn() << "entries size: " << entries.size();

  QFileInfoList::const_iterator iter = entries.begin();

  for (; iter != entries.end(); ++iter)
  {
    if (stopIssued == true)  // every loop we check if stop search is issued if is, then stop doing job
      break;

    QSharedPointer<Searchable> searchable = BuildSearchable(*iter);

    if (searchable != NULL)  // if it's NULL, we simply think it can be skipped
    {
      // use a temporary pointer object here to pass the compilation
      // otherwise this won't compile:
      // CallEach(observers, searchable.data(), &Subject::Register);
      Searchable* tmp = searchable.data();
      CallEach(GetObservers(), tmp, &Subject::Register);
      searchable->Search(keywords, caseSensitive, syntax);
    }
  }  // end of  for (; iter != entries.end(); ++iter)
}
