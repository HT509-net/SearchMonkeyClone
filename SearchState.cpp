#include "debug.h"

#include "dir.h"
#include "mainwindow.h"
#include "preset.h"
#include "SearchState.h"
#include "SessionIDGenerator.h"
#include "settings.h"
#include "ui_mainwindow.h"
#include "Utility.h"

// default implementation is do nothing
void SearchState::DoGatherSearchParameter(SearchParameter& parameter, Ui::MainWindow* window)
{ (void) parameter, (void) window; }
SearchParameter SearchState::GatherSearchParameter(Ui::MainWindow* window, Settings* sui)
{
   unsigned sessionId = SessionIDGeneratorSingleton::Instance()->NextID();
   qDebug() << "SearchState::GatherSearchParameter(): sessionId-" << sessionId;

   QString searchPath = GetSearchPath(window);
   QStringList contentKeyword = GetContentKeyword(window),
           fileNameKeywords = GetFileNameKeywords(window, sui);

   qDebug() << "fileNameKeywords-" << fileNameKeywords << " SearchPath-" << searchPath << " contentKeyword-" << contentKeyword;

   sui->getDir()->setDirFilter();
   QDir d(searchPath);
   if (sui->getDir()->filter().contains(d.dirName()))
   {
     sui->reset();
     // Todo-throw exception aborting search - cur dir is part of exclude list
     qDebug() << "Settings exclude current dir, should be aborting...";
   }

   QSharedPointer<PhaseOneParameter> oneParam(new PhaseOneParameter(sessionId,
                          fileNameKeywords, searchPath, sui));
   QSharedPointer<PhaseTwoParameter> twoParam(new PhaseTwoParameter(sui));

   if (!contentKeyword.isEmpty()  &&  contentKeyword[0] != "")
     twoParam = QSharedPointer<PhaseTwoParameter>(new PhaseTwoParameter(contentKeyword[0], sui));

   SearchParameter param(oneParam, twoParam);

   DoGatherSearchParameter(param, window);  // anything else to do?

   return param;
}
QString SearchState::GetContentKey(Ui::MainWindow* window) {
  return GetContentKeyword(window)[0];
}
QStringList SearchState::GetContentKeyword(Ui::MainWindow* window)
{
    QString contentKeyword = DoGetContentKeyword(window);

  if (!validRx(contentKeyword,
     QRegExp::RegExp/*uint2syntax(static_cast<uint>(window->syntaxComboBox->currentIndex()))*/))
  {
    boxError(QString(QObject::tr("Invalid Content")));
    return QStringList("sss");  // Todo: throw exception
  }
  setText(&contentKeyword);

  return QStringList(contentKeyword);
}
/******************************************************************************
 * GetFileNameKeywords(): establish filename search patterns.
 smo- polymorphise this function
 ******************************************************************************/
QStringList SearchState::GetFileNameKeywords(Ui::MainWindow* ui, Settings* sui)
{
  Entex ee("SearchState::GetFileNameKeywords", 2);
  QStringList sl;

  QString s = ui->actionAdvanced->isChecked()
         ? ui->fileLineEdit->text()
         : ui->quickFileLineEdit->text();

  if (s.isEmpty()) {
    if (sui->getPreset()->target().isEmpty())
      sl << "*";
  }
  else {
    int n = 0;
    char** patterns;
    qd(2) << "parsing-" << s.toStdString().c_str();
    parseDelim(s.toStdString().c_str(), ", ;", &patterns, &n);

    qd(2) << "n-" << n;
    for (int i = 0; i < n; ++i) {  // smo debug loop
      if (*patterns[i]) {
        qd(2) << "pushing-''" << patterns[i] << "''";
        sl << patterns[i];
      }
    }
  }
  qd(2) << "sl-" << sl;
  if (!validRx(sl, QRegExp::Wildcard)) {
    boxError(QString(QObject::tr("Invalid Filename(s)")));
    return QStringList();  // Todo: throw exception i.e. cancel search
  }
  if (sui->getPreset()->set(&sl) == -1) {  // Todo: another cancel exception
    sui->reset();
    qd(0) << "options cancelled...returning";
    return QStringList();  // opts cancelled out (nothing to do) or usage err
  }

   return sl;
}
/******************************************************************************
 * GetSearchPath(): get the path to search.
 ******************************************************************************/
QString SearchState::GetSearchPath(Ui::MainWindow* window)
{
  QString path = DoGetSearchPath(window);

  if (!IsValidDir(path))
    boxError(QString(QObject::tr("Invalid Folder")));  // Todo: throw exception when exception class is in place

  return path;
}
/******************************************************************************
 * SetText(): mofify content to grep based on configuration.
 ******************************************************************************/
void SearchState::setText  (QString *_content) {
  (void)_content;
  ; // smo- wrap search text in a regular expression based on user choices
}
