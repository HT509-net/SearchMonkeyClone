/******************************************************************************/
/*! \file : mainwindow.cpp
 *
 * PURPOSE: Searchmonkey main window implementation of class MainWindow.
 *
 * DATE:    October 18, 2009
 *
 * REVISIONS:
 * ---------
 * [083109] shawno- pilot app with NO *.ui file (QtDesigner)
 * [101809] adamc- designed final GUI with QtDesigner; added minor toggle
 * functions to support aforementioned *.ui file
 * [102109] shawno- ported my code from pilot app of [083109] into Adam's UI
 * code.  Designated path is searched (default recursively), matching files are
 * grepped, matching files are displayed, at one directory per cycle.  We now
 * have an unthreaded working pilot app featuring final overall GUI layout.
 * [020610] shawno- moved textEdit/proxyView code to tabResults.cpp
 *
 NOTES
 -----
 * checkin regularly
 * regexp user must escape (\) these:  [\^$.|?*+()
 * keep it ultra simple unless the user switches to advanced mode
 \todo v2.0
 ----------
 2 make window icon bigger or add searchmonkey pic to main window
 i anything tagged "todo"
 u keyboard navigation (tabs, ALT-key), mnemonics, tool tips
 \todo v3.0
 ----------
 I use QVector for old arrays of pointers w/fixed sizes
 a merge Xiao's search engine and classes with existing prototype code
 b allow multiple folders per search?  multiple content?
 c need CLI interface
 d separate cases? (as in case-sensitive for find/grep?)
 e check out QWeb and QTimer
 f minimize to tray option?  (Qt System Tray Icon example)
 g setting(s) to allow user to disable popups
 h regex/glob builder wizards
 j auto-init member data
 k break ctor further into helper functions?
 l case sensitivity applies to both find AND grep (all or nothing)?
 m Qt Application example
 n setting to ignore warnings about not being able to open files
 o Settings feature to ignore std backup files (Windows vs Linux etc)
 p more Preset definitions needed (see ack)
 q lots more QSettings can be save upon close!
 r need progress dialog for mappedReduced()?
 s total rework of Directory Settings regarding on-disk storage
 t support regexp in filename -- would require another adv option
 v test guicontext using new lineedits
 w need a checkbox before content to avoid time-consuming edits
 x search within results option?
 y allow user to choose color for lineno and highlighted content
 z migrate from Advance Installer to Wix
 0 put syntax combobox in Settings
 1 put target presets in main window
 3 set gui defaults programmatically instead of using last compile's defaults
 4 invert at each stage or invert right before showFiles() (I think the latter; I think not)
 LINUX
 -----
 a sort by column: may have to implement manually
 5
 PRE-RELEASE (any version)
 -----------
 a Doxygen
 0 Agile test cases
 1 compact GUI layout
 2 test, test, test
 3 test case: ack -awl '^(Name|Comment|Keywords)\[.+\]' works; can we do it?
 4 remove excess includes, asserts, debugs (qDebug), commented code
 5 remove whitespace; LINT etc...
*******************************************************************************/
#include "debug.h"

#include <QtWidgets>

//#include <QTest>

#include <QtConcurrent/QtConcurrentMap>

#include "about/about.h"
#include "dir.h"
#include "global.h"
#include "highlighter.h"
#include "license/license.h"
#include "mainwindow.h"
#include "preset.h"
#include "tabresults.h"
#include "settings.h"
#include "sortproxy.h"
#include "ui_mainwindow.h"
#include "ui_settings.h"
#include "ui_tabresults.h"

#include "AdvancedSearchState.h"
#include "QuickSearchState.h"
#include "Singleton.h"
#include "Utility.h"

extern bool ABORT_COUNTING_HITS;

MainWindow::MainWindow(
  QWidget *parent
) :
  QMainWindow(parent),
  aui(new About),
  lui(new License),
  sui(new Settings),
  ui(new Ui::MainWindow),
  m_tui(NULL),
  m_mruTabWidget(NULL),
  m_abortCalculating(false),
  m_abortPopulating(false),
  m_ctorDone(false),
  m_flagCalculating(false),
  m_flagPopulating(false),
  m_tabAdded(false),
  runState(false),
  m_depth(0),
  m_showModel(NULL),
  m_mruDir(""),
  m_mruQuickDir(""),
  state(NULL)
{
  ui->setupUi(this);

  qDebug() << QDate::currentDate() << " " << QTime::currentTime()
          << " +++++++++++++++++++++++++++++++++++++++++++++++";
  qd(0) << "level 0 msg";
  qd(1) << "level 1 msg";
  qd(2) << "level 2 msg";
  qd(3) << "level 3 msg";
  qd(4) << "level 4 msg";
  qd(5) << "level 5 msg";
  qd(6) << "level 6 msg";
  qd(7) << "level 7 msg";
  qd(8) << "level 8 msg";
  qd(9) << "level 9 msg";

  setContextMenuPolicy(Qt::PreventContextMenu);  // allows model context menu

  for (int i = 0; i < MAX_SEARCH_TABS; tui[i++] = NULL) ; // What does this do?
  m_hitv.resize(HITVEC_REALLOC_SZ);

  ui->resultsTabWidget->removeTab(0);  // kill extra tab (from mainwindow.ui)

  createActions();
  ui->mainToolBar->addAction(layoutAct);  // order matters...
  ui->mainToolBar->addAction(logHistoryAct);
  ui->mainToolBar->addAction(resultsBrowserAct);
  ui->mainToolBar->addAction(stateAct);
  ui->mainToolBar->addAction(settingsAct);
  ui->mainToolBar->addAction(newTabAct);

#ifdef Q_OS_WIN32
  ui->symLinkCheckBox->setVisible(false);
#endif
  //  \todo: pre-release shutdown of all broken parts
  logHistoryAct->setVisible(false);
  resultsBrowserAct->setVisible(false);
  sui->ui()->cStyleCheckBox->setVisible(false);
  ui->lcdLabel->setVisible(false);
  ui->lcdNumber->setVisible(false);
  ui->progressBar->setVisible(false);
  ui->resultsTabWidget->setTabsClosable(false);
  ui->mainToolBar->setVisible(false);
  ui->resetAdvButton->setVisible(false);
  //ui->menuLayout->setVisible(false);
  //ui->invertCheckBox->setVisible(false);
  //ui->emptyDirsCheckBox->setVisible(false);
  //ui->binaryCheckBox->setChecked(true);
  //ui->binaryCheckBox->setEnabled(false);

  setLineEditSize();
  ui->toolBox->setCurrentIndex(0);

  state = QuickSearchStateSingleton::Instance();
  state->init(ui, sui);
  state = AdvancedSearchStateSingleton::Instance();
  state->init(ui, sui);
  state = QuickSearchStateSingleton::Instance();

  createResultsTab("");  // must precede initState()
  initState();
  initLineEditCompleter();

  // set up datetime edits
  ui->dateTimeEdit->setDisplayFormat("MM.dd.yy hh:mm:ss");
  ui->dateTimeEdit2->setDisplayFormat("MM.dd.yy hh:mm:ss");
  ui->dateTimeEdit->setCalendarPopup(true);
  ui->dateTimeEdit2->setCalendarPopup(true);

  // initialize menu bar status messages
  /*
  ui->menuFile->setStatusTip("Ready.");
  ui->menuBar->setStatusTip("Ready.");
  connect(ui->menuFile, SIGNAL(hovered(QAction*)), this,
                 SLOT(showStatus(QAction*)));
  connect(ui->menuBar, SIGNAL(hovered(QAction*)), this,
                SLOT(showStatus(QAction*)));
                 */
  ui->menuBar->blockSignals(true);
  ui->menuFile->blockSignals(true);
  ui->menuBar->disconnect();
  ui->menuFile->disconnect();
  //connect(ui->menuFile, SIGNAL(triggered(QAction*)), this,
  //             SLOT(showStatus(QAction*)));
  normalLabel = new QLabel;
  statusBar()->addWidget(normalLabel);
  normalLabel->setText("Ready.");
  statusLabel = new QLabel;
  statusBar()->addPermanentWidget(statusLabel);
  statusLabel->setText("Ready.");

  //ui->depthMaxSpinBox->setRange(0, MAX_DIRECTORY_DEPTH);
  //ui->depthMinSpinBox->setRange(0, MAX_DIRECTORY_DEPTH);
  //ui->depthMaxLabel->setBuddy(ui->depthMaxSpinBox);
  //ui->depthMinLabel->setBuddy(ui->depthMinSpinBox);
  ui->quickCancelPushButton->setEnabled(false);
  ui->quickSearchPushButton->setFocus();
  ui->quickSearchPushButton->setDefault(true);
  ui->sizeLineEdit->setEnabled(false);
  ui->sizeLineEdit2->setEnabled(false);
  ui->sizeUnitsComboBox->setEnabled(false);
  ui->sizeUnitsComboBox2->setEnabled(false);
  //ui->syntaxLabel->setBuddy(ui->syntaxComboBox);
  //ui->syntaxComboBox->setEditable(false);
  ui->resultsTabWidget->setMovable(true);
  ui->resultsTabWidget->setTabsClosable(true);

  sizeValidator = new QRegExpValidator(QRegExp("\\d{1,9}"), this);
  ui->sizeLineEdit->setValidator(sizeValidator);
  ui->sizeLineEdit->setMaxLength(9);
  ui->sizeLineEdit2->setValidator(sizeValidator);
  ui->sizeLineEdit2->setMaxLength(9);

  connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
  connect(ui->actionLicense, SIGNAL(triggered()), this, SLOT(license()));
  connect(&coordinator, SIGNAL(OneSearchDone(const Result&)), this,
        SLOT(ResultAvailable(const Result&)), Qt::DirectConnection);

  // '[*]' - file modified indicator
  setWindowTitle("[*]Searchmonkey - Power Searching Without the Pain");
  setWindowIcon(QIcon(":/pixmaps/searchmonkey-300x300.png"));
  sui->setWindowIcon(QIcon(":/pixmaps/searchmonkey-300x300.png"));
  setWindowFlags(Qt::WindowMinMaxButtonsHint | Qt::Window);
  setUnifiedTitleAndToolBarOnMac(true);

  QGraphicsScene scene;
  scene.addPixmap(QPixmap(":/pixmaps/searchmonkey-32x32.png"));
  //ui->graphicsView->setScene(&scene);
  //ui->graphicsView->show();

  /*
  // \todo test settings
  ui->quickDirectoryLineEdit->setText("../../../test");
  ui->quickFileLineEdit->setText("*");
  ui->quickContentLineEdit->setText("ack");
  */

  sui->ui()->tabCheckBox->setChecked(false);

  // \todo Tx *tx = new Tx();
  // \todo tx->pool->start(tx);

  QCoreApplication::setOrganizationName("Obarnet HT");
  QCoreApplication::setApplicationName("FouyaPòt");
  readSettings();

  m_ctorDone = true;
  //statusBar()->showMessage(tr("Ready."));
  qDebug() << "exiting constructor**********************************"
          "*****************************************************";
}

//---------------------------------------------------------------------------------

MainWindow::~MainWindow()
{
  for (int i = 0; i < MAX_SEARCH_TABS; i++)
    if (tui[i] != NULL)
      delete tui[i];
  delete ui;
  delete sui;
  delete lui;
  delete aui;
}

/**************************************************************************//**
 * HitMapReady(): called when mappedReduced() finishes.  [slot]
 ******************************************************************************/

void MainWindow::hitMapReady  ()
{
  sem.down();

  qd(3) << "A hit map is ready...[" << m_hitv.size() << "]";

  // The signal calling this slot indicates a future has finished, which may
  // or may not have been canceled.  If canceled, destroy one and return.
  // Otherwise, mark one for processing, process it and destroy it.  For the
  // sake of robustness, return if m_hitv[] contains all 0s (which should
  // not happed since this slot was called).
  int target = -1;
  for (int i = 0; i < m_hitv.size(); ++i) {
    if (m_hitv[i] == 0)
      continue;
    if (m_hitv[i]->watcher.isFinished()) {
      if (m_hitv[i]->watcher.isCanceled()) {
        delete m_hitv[i];
        m_hitv[i] = 0;
        m_hitv.remove(i);
        return;
      }
      else {
        target = i;
        break;
      }
    }
  }
  if (target == -1)
    return;

  QMapIterator<QString, int> i(m_hitv[target]->futureHits);

  while (i.hasNext()) {
    i.next();
    qd(3) << i.value() << " " << i.key();
    m_hitMap[i.key()] = i.value();
  }
  delete m_hitv[target];
  m_hitv[target] = 0;
  m_hitv.remove(target);
}

/**************************************************************************//**
 * HitMapDisplay(): called after finder() to display Matches column.
 ******************************************************************************/

void MainWindow::hitMapDisplay  () {
  Entex ee("hitMapDisplay", 1);

  statusBar()->showMessage(tr("Still calculating 'Matches' column...this may take a while..."));

  m_flagCalculating = true;
  int m = 0;
  int n = 0;
  while (!m_abortCalculating  &&  sem.getCount() > 0) {  // abort if user hits Cancel
    if (m == INT_MAX)
      m = 0;

    if (++m % 1000000 == 0) {
      qd(1) << "calling process wait...";
      if (++n == 10) {
        statusBar()->showMessage(tr("Still calculating 'Matches' column...this may take a while..."));
        n = 0;
      }
    }

    qApp->processEvents(QEventLoop::WaitForMoreEvents,250);
  }
  m_flagCalculating = false;

  if (m_abortCalculating) {  // cancel all mappedReduced() futures, cleanup and return
    m_abortCalculating = false;
    qDebug() << "m_abortCalculating was true...";
    for (int i = 0; i < m_hitv.size(); ++i) {
      if (m_hitv[i] == 0)
        continue;
      m_hitv[i]->watcher.cancel();
    }
    m_hitMap.clear();
    // No need to reset the sem since all the canceled calls will call
    // hitMapReady() above which decrements the sem.
    //sem.reset();
    // Now that all futures are canceled, stop all running mappedReduced()
    // calls, at least up until the user starts the next search.
    ABORT_COUNTING_HITS = true;
    return;
  }

  qDebug() << "setting row count to 0...";
  m_showModel->setRowCount(0);

  m_tui->proxyViewH->setSortingEnabled(false);  // disable before! model populated
  m_tui->proxyViewV->setSortingEnabled(false);  // ...

  statusBar()->showMessage(tr("Populating 'Matches' column...One moment please..."));

  qDebug() << "prior to loop size: " << m_hitMap.size();
  m_flagPopulating = true;
  QMapIterator<QString, int> i(m_hitMap);
  i.toBack();
  while (i.hasPrevious()) {
    i.previous();

    qApp->processEvents();
    if (m_abortPopulating) {
      m_abortPopulating = false;
      break;
    }

    QFile file(i.key());
    QFileInfo fi(file);

    QDateTime dt = fi.lastModified();

    addFile(m_showModel, i.key(), bytes2units(file.size()),
          i.value(), fi.suffix(), dt.toString("MM.dd.yy hh:mm:ss"));
  }
  m_flagPopulating = false;

  m_tui->proxyViewH->setSortingEnabled(true);  // enable after! model populated
  m_tui->proxyViewV->setSortingEnabled(true);  // ...

  m_hitMap.clear();
  sem.reset();
}

/**************************************************************************//**
 * About(): About menu option.
 ******************************************************************************/

void MainWindow::about  () {
  aui->show();
}


/*
void MainWindow::newFile() {
  if (maybeSave()) {
    ui->lineDisplayH->clear();
    ui->lineDisplayV->clear();
    qDebug() << "invoked newFile()";
    setCurrentFile("");
  }
}
void MainWindow::open(){QString fileName=QFileDialog::getOpenFileName(this);}
bool MainWindow::save() {
  if (curFile.isEmpty()) {
    return true; // saveAs();
  } else {
    return true; // saveFile(curFile);
  }
}
bool MainWindow::saveAs() {
    QString fileName = QFileDialog::getSaveFileName(this);
    if (fileName.isEmpty())
      return false;
    return saveFile(fileName);
 }
 */


/**************************************************************************//**
 * AddFile(): add row to the matched row model.
 ******************************************************************************/

void MainWindow::addFile (
  QAbstractItemModel *model,
  const QString &filename,
  const QString &size,
  int hits,
  const QString &type,
  const QString &modified
)
{
  int row = model->rowCount();
  model->insertRow(row);
  model->setData(model->index(row, 0), filename);
  model->setData(model->index(row, 1), size);
  model->setData(model->index(row, 2), hits);
  model->setData(model->index(row, 3), type);
  model->setData(model->index(row, 4), modified);
}

/**************************************************************************//**
 * Browse(): Advanced browse (button) for target directory.  [slot]
 ******************************************************************************/

void MainWindow::browse (
  QLineEdit *_lineEdit
)
{
  QList<QUrl> urls;
    urls << QUrl::fromLocalFile(QStandardPaths::standardLocations(
                                         QStandardPaths::DesktopLocation).at(0))
          << QUrl::fromLocalFile(QStandardPaths::standardLocations(
                                         QStandardPaths::DocumentsLocation).at(0))
          << QUrl::fromLocalFile(QStandardPaths::standardLocations(
                                         QStandardPaths::HomeLocation).at(0))
      << QUrl::fromLocalFile(QDir::rootPath());

  QFileDialog dialog(this, tr("Select Directory"),
    !m_mruDir.isEmpty() ? m_mruDir
        : QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).at(0));
  dialog.setSidebarUrls(urls);
  dialog.setFileMode(QFileDialog::Directory);
  dialog.setViewMode(QFileDialog::Detail);

  QStringList fileNames;
  if (dialog.exec())
    fileNames = dialog.selectedFiles();

  QString directory;
  if (!fileNames.isEmpty())
    directory = fileNames[0];
  if (!directory.isEmpty())
    _lineEdit->setText(m_mruDir = directory);
}

/**************************************************************************//**
 * BrowseQuick(): Quick browse (button) for target directory.  [slot]
//QDir::rootPath());
//QDir::currentPath());
 ******************************************************************************/

void MainWindow::browseQuick (
  QLineEdit *_lineEdit
)
{
  qd(1) << "doc loc-" << QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).at(0);

  QString directory = QFileDialog::getExistingDirectory(this,
                                    tr("Select Directory"),
    !m_mruQuickDir.isEmpty() ? m_mruQuickDir
        : QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).at(0));

  if (!directory.isEmpty())
    _lineEdit->setText(m_mruQuickDir = directory);
}

/**************************************************************************//**
 * CaseSensitive(): case sensitivite based on gui
 ******************************************************************************/

bool MainWindow::caseSensitive ()
{
  if (ui->actionAdvanced->isChecked())
    return ui->caseCheckBox->isChecked();
  else
    return false;  // quick mode is case insensitive
}

/**************************************************************************//**
 * CaseSensitivity(): case sensitivity in terms of Qt flags.
 ******************************************************************************/

Qt::CaseSensitivity MainWindow::caseSensitivity ()
{
  if (ui->actionAdvanced->isChecked())
    return ui->caseCheckBox->isChecked() ? Qt::CaseSensitive
        : Qt::CaseInsensitive;
  else
    return Qt::CaseInsensitive;  // quick mode is case insensitive
}

/**************************************************************************//**
 * Clear(): clear gui when Clear button clicked.  [slot]
 ******************************************************************************/

void MainWindow::clear ()
{

  state->reset();
  this->update();
}

/**************************************************************************//**
 * CloseEvent(): last chance to get in pre-shutdown tasks.
 ******************************************************************************/
void MainWindow::closeEvent (
  QCloseEvent *event
)
{
  if (true) {
    writeSettings();
    event->accept();
  }
  else
    event->ignore();

  emit stop(FLAG);
}

/**************************************************************************//**
 * Complement(): find complement of the set of files in the given directory.
 * \todo-test this function (inverted search)
 ******************************************************************************/

QStringList MainWindow::complement (
  const QStringList &_files,
  const QDir &_dir
)
{
  QStringList files = _dir.entryList(QStringList("*"), QDir::Files | flags());

  if (modeSymLink())
  {;} // resolveSymLinks(files, _dir);

  return listSubtract(files, _files);
}

/**************************************************************************//**
 * CreateResultsTab(): create a new, empty results tab.  [slot]
 ******************************************************************************/

void MainWindow::createResultsTab ()
{
  createResultsTab("");
}

/**************************************************************************//**
 * CreateResultsTab(): create a new results tab.
 ******************************************************************************/

void MainWindow::createResultsTab (
  const QString &_path
)
{
  Entex ee("CreateResultsTab", 0);

  int idx = 0;
  while (idx < MAX_SEARCH_TABS) {
    if (tui[idx] == NULL)
      break;
    idx++;
  }
  if (idx >= MAX_SEARCH_TABS) {
    boxError("Too many tabs.");
    return;
  }
  // idx now points at the first available ptr
  tui[idx] = new TabResults(this, idx);
  QString sidx = QString("%1").arg(idx);
  qd(0) << ee.fn() << "adding tab...";
  ui->resultsTabWidget->setTabsClosable(false);
  int index = ui->resultsTabWidget->addTab(tui[idx], QIcon(":/pixmaps/searchmonkey-300x300.png"), "Results"/*sidx*/);
  ui->resultsTabWidget->setTabsClosable(false);

  if (m_mruTabWidget != NULL)
    m_mruTabWidget->gui()->ripContext(ui, sui->ui());  // rip prev context

  tui[idx]->tui()->layoutPushButton->setAutoDefault(true);
  //
  // This is the active tab.  This is important since many operations are per-
  // formed on this tab before and after the find operation.  Further, in the
  // case of non-tabbed searching or if tabbed searching is unchecked, then
  // this pointer will point to the active tab over many find() operations.
  // Moreover, if this tab is deleted on the tabCloseRequested signal, then it
  // will have to be reset.
  //
  {
    m_mruTabWidget = tui[idx];
    //
    // Assign the active tab results ui ptr to the main window shortcut ptr.
    //
    m_tui = m_mruTabWidget->tui();
  }
  {
    m_tabAdded = true;  // indicates, with below sig, that context was ripped
    ui->resultsTabWidget->setCurrentIndex(index);  // sends a signal
  }

  // set the tooltip
  int size = m_fileNames.size();
  QString filenames;
  if (size > 0)
    filenames = m_fileNames[0];
  for (int i = 1; i < size; filenames += (", " + m_fileNames[i++])) ;
  QString tip = QString("%1|%2|%3").arg(m_content).arg(filenames).arg(_path);
  ui->resultsTabWidget->setTabToolTip(index, tip);
}

/**************************************************************************//**
 * DebugResultsTab(): display tabs, active tab and current tab.
 ******************************************************************************/

void MainWindow::debugResultsTab (
  const QString &_f
) const
{
  int i = 0;
  QString s;
  for (; i < MAX_SEARCH_TABS; i++) {
    if (tui[i] != NULL) {
      qd(3) << "idx-" << i << " " << tui[i]->gui();
      if (tui[i] == ui->resultsTabWidget->currentWidget())
        s += "-";
      if (tui[i] == m_mruTabWidget)
        s += "+";
      s += QString("%1").arg(i);
    }
  }
  qd(1) << _f << " TABS: " << s;
}

/**************************************************************************//**
 * DocumentWasModified(): indicate text display document modified with [*].
 ******************************************************************************/

void MainWindow::documentWasModified()
{
  setWindowModified(m_tui->lineDisplayH->document()->isModified() ||
                    m_tui->lineDisplayV->document()->isModified());
}

/**************************************************************************//**
 * Filepath2abspath(): convert list of filepaths to absolute paths.
 ******************************************************************************/

QStringList MainWindow::filepath2abspath  (
  const QStringList &_files,
  const QDir &_dir
)
{
  QStringList files;
  register int size = _files.size();
  for (register int i = 0; i < size; ++i)
    files << _dir.absoluteFilePath(_files[i]);

  return files;
}

/**************************************************************************//**
 * FilterDateTime(): filter file list based on date/time constraints and
 *                   provide support for inverted search.
 ******************************************************************************/

QStringList MainWindow::filterDateTime (
  const QStringList &_files,
  const QDir &_currentDir
)
{
  QDateTime dt;
  QDateTime dt1 = ui->dateTimeEdit->dateTime();
  QDateTime dt2 = ui->dateTimeEdit2->dateTime();
  QString option = ui->dateTimeComboBox->currentText();
  QStringList subset;

  bool accessed = option.indexOf("Accessed") != -1,
  created  = option.indexOf("Created")  != -1,
  after    = option.indexOf("After")    != -1,
  before   = option.indexOf("Before")   != -1;

  register int size = _files.size();
  for (register int i = 0; i < size; ++i) {

    QFile file(_currentDir.absoluteFilePath(_files[i]));
    QFileInfo fi(file);

    dt = accessed ? fi.lastRead() : created ? fi.created() :fi.lastModified();

    if (after) {
      if (dt >= dt1)
        subset << _files[i];
    }
    else if (before) {
      if (dt <= dt1)
        subset << _files[i];
    }
    else {
      if (dt >= dt1  &&  dt <= dt2)  // between
        subset << _files[i];
    }
  }
  return true /* !modeInvert() */ ? subset : listSubtract(_files, subset);
}
/**************************************************************************//**
 * FilterSize(): filter file list based on size constraints and provide support
 *               for inverted search.
 ******************************************************************************/

QStringList MainWindow::filterSize  (
  const QStringList &_files,
  const QDir &_currentDir
)
{
  QStringList subset;
  QString option = ui->sizeComboBox->currentText();
  bool between = option.indexOf("Between") != -1,
  larger  = option.indexOf("Larger")  != -1,
  smaller = option.indexOf("Smaller") != -1;
  qint64 sz;
  qint64 sz1 = units2bytes(QString("%1 %2").arg(ui->sizeLineEdit->text()).arg(
      chop(ui->sizeUnitsComboBox->currentText(), 4)));   // chop 'ytes'
  qint64 sz2 = between ? units2bytes(QString("%1 %2").arg(
                                         ui->sizeLineEdit2->text()).arg(chop(
                                         ui->sizeUnitsComboBox2->currentText(), 4))) : -1;

  register int size = _files.size();
  for (register int i = 0; i < size; ++i) {
    QFile file(_currentDir.absoluteFilePath(_files[i]));
    QFileInfo fi(file);

    sz = file.size();

    if (larger) {
      if (sz >= sz1)
        subset << _files[i];
    }
    else if (smaller) {
      if (sz <= sz1)
        subset << _files[i];
    }
    else {
      if (sz >= sz1  &&  sz <= sz2)  // between
        subset << _files[i];
    }
  }
  return true /* !modeInvert() */ ? subset
                        : listSubtract(_files, subset);
}

/**************************************************************************//**
 * _Find(): Find (button) matching files.  Initiate search engine.  [slot]
 ******************************************************************************/

void MainWindow::_find () throw()
{
  if (suiTabbedSearching())
    if (m_mruTabWidget->getModelPtr()->rowCount() > 0  ||  m_mruTabWidget->isSearching())
      createResultsTab(state->GetSearchPath(ui));

  m_mruTabWidget->init();

  m_tui->proxyViewH->setColumnHidden(2, state->GetContentKey(ui) == "");
  m_tui->proxyViewV->setColumnHidden(2, state->GetContentKey(ui) == "");

  //
  // Prepare find state, find, setup post-find state.
  //
  QTime time;
  time.start();

  stateFind(true);
  try
  {
    !modeEmptyDirs() ? m_mruTabWidget->find()
                : findEmptyDirs(state->GetSearchPath(ui));
  }
  catch (Stop s) {}
  stateFind(false);

  sui->reset();

  m_tui->proxyViewH->setSortingEnabled(true);  // enable after! model populated
  m_tui->proxyViewV->setSortingEnabled(true);  // ...

  const char *m = (m_numFiles == 1) ? "%1 file found" : "%1 files found";
  statusBar()->showMessage(tr(m).arg(m_numFiles) +
                   tr(" in %1").arg(ms2units(time.elapsed())));
  debugResultsTab("_find()");

}  // _find()

/**************************************************************************//**
 * Find(): Find (button) matching files.  Initiate recursion.  [slot]
 ******************************************************************************/

void MainWindow::find  () throw()
{
  autoSettingsReset asr(sui);  // dtor calls sui->reset()

  bool modeAdvanced = ui->actionAdvanced->isChecked();
  qDebug() << "modeAdvanced-" << modeAdvanced;
  //
  // Establish search path.
  //
  QString path = modeAdvanced ? ui->directoryLineEdit->text()
                     : ui->quickDirectoryLineEdit->text();
  if (path.isEmpty())
    path = QDir::homePath();

  if (!IsValidDir(path)) {
    boxError(QString(tr("Invalid Folder")));
    return;
  }
  //
  // Establish filename search patterns (ported).
  //
  m_fileNames.clear();

  QString s = modeAdvanced ? ui->fileLineEdit->text()
                   : ui->quickFileLineEdit->text();
  if (s.isEmpty()) {
    if (sui->getPreset()->target().isEmpty())
      m_fileNames << "*";
  }
  else {
    int n = 0;
    char** patterns;
    qDebug() << "parsing-" << s.toStdString().c_str();
    parseDelim(s.toStdString().c_str(), ", ;", &patterns, &n);

    qDebug() << "n-" << n;
    for (int i = 0; i < n; ++i) {
      if (*patterns[i]) {
        qDebug() << "pushing-''" << patterns[i] << "''";
        m_fileNames << patterns[i];
      }
    }
  }
  if (!validRx(m_fileNames, QRegExp::Wildcard)) {
    boxError(QString(tr("Invalid Filename(s)")));
    return;
  }
  //
  // Establish content to grep.
  //
  m_content = setText();  // may augment user regex

  if (!validRx(m_content, m_syntax)) {
    boxError(QString(tr("Invalid Content")));
    return;
  }
  //
  // Perform additional checks after validations.
  //
  switch (sui->getPreset()->set(&m_fileNames)) {
    case -2:
      statusBar()->showMessage(tr("Options cancelled out...nothing to do"));
      return;
    case -1:
      statusBar()->showMessage(tr("Settings usage error"));
      return;
    case 0: ;
  }
  qDebug() << "m_fileNames-" << m_fileNames;

  if (m_depthMin > m_depthMax) {
    statusBar()->showMessage(tr("Minimum depth exceeds maximum depth...nothing to do"));
    return;
  }
  //
  // Establish Directories Settings.
  //
  sui->getDir()->setDirFilter();
  QDir d(path);
  if (sui->getDir()->filter().contains(d.dirName())) {
    statusBar()->showMessage(tr("Current directory is part of exclude list...nothing to do"));
    return;
  }
  //
  // Establish results tab.
  //
  if (suiTabbedSearching())
    if (m_mruTabWidget->getModelPtr()->rowCount() > 0)
      createResultsTab(path);

  m_mruTabWidget->init();  // sets row count to zero
  statusBar()->clearMessage();
  normalLabel->setText("");
  statusLabel->setText("Searching...");

  m_tui->proxyViewH->setColumnHidden(2, m_content == "" || modeEmptyDirs());
  m_tui->proxyViewV->setColumnHidden(2, m_content == "" || modeEmptyDirs());

  m_tui->proxyViewH->setSortingEnabled(false);  // disable before! model populated
  m_tui->proxyViewV->setSortingEnabled(false);  // ...

  m_tui->lineDisplayH->setVisible(!m_content.isEmpty());
  m_tui->lineDisplayV->setVisible(!m_content.isEmpty());
  //
  // Prepare find state, find, setup post-find state.
  //
  QTime time;
  time.start();

  stateFind(true);
  ABORT_COUNTING_HITS = false;  // make sure mappedReduced() will not abort
  try
  {
    !modeEmptyDirs() ? finder(path) : findEmptyDirs(path);
  }
  catch (Stop s) {}
  stateFind(false);

  m_tui->proxyViewH->setSortingEnabled(true);  // enable after! model populated
  m_tui->proxyViewV->setSortingEnabled(true);  // ...

  const char *m = (m_numFiles == 1) ? "%1 file processed":"%1 files processed";
  s = tr(m).arg(m_numFiles) + tr(" in %1").arg(ms2units(time.elapsed()));
  statusBar()->showMessage(s);
  statusLabel->setText(s);
  debugResultsTab("find()");

}  // find()

/**************************************************************************//**
 * SetText(): set the content to grep based on configuration.
 ******************************************************************************/

QString MainWindow::setText () 
{
  QString text = ui->actionAdvanced->isChecked()
           ? ui->contentLineEdit->text()
           : ui->quickContentLineEdit->text();

  // \todo- wrap search text in a regular expression based on user choices

   return text;
}

/**************************************************************************//**
 * FindEmptyDirs(): Find only empty (or nonempty) directories.
 * \todo-thinks special winxp dirs with cookies, index.dat and ini are empty!?!
 ******************************************************************************/

void MainWindow::findEmptyDirs (
  const QString &_path
)
{
   Entex ee("findEmptyDirs", 3);

   if (!runState)  // check for exit state
    return;

  static int depth  = 0;
  bool depthChanged = 0;
  QStringList emptyDirs,
          nonEmptyDirs;

  QDir currentDir(_path);
  int cnt = currentDir.count() - 2;

  QString s = QString("  [%1 files]").arg(cnt);
  QString s2 = (cnt > 256) ? s : QString();
  statusBar()->showMessage(_path + s2);
  qApp->processEvents();

  QStringList dirs = currentDir.entryList(QStringList(), QDir::Dirs | flags());

  if (dirs.size()  &&  depth >= m_depthMin) {

    register int size = dirs.size();
    for (register int i = 0; i < size; ++i) {
      QDir dir(currentDir.absoluteFilePath(dirs[i]));

      if (dir.count() <= 2)        // counts . and .. (\todo test root dir)
        emptyDirs << dirs[i];
      else if (modeInvert())
        nonEmptyDirs << dirs[i];  // counts nonempty dirs
    }
    showEmptyDirs(modeInvert() ? nonEmptyDirs : emptyDirs, currentDir);
  }

  dirs = currentDir.entryList(QStringList(), QDir::AllDirs | flags());

  dirs = listSubtract(dirs, emptyDirs);  // performance tweak

  if ((depthChanged = static_cast<bool>(dirs.size()))) {
    if (++depth > m_depthMax) {
      --depth;
      return;
    }
  }
  if (ui->subdirCheckBox->isChecked()) {
    register int size = dirs.size();
    for (register int i = 0; i < size; ++i)
      findEmptyDirs(currentDir.absoluteFilePath(dirs[i]));  // recurse
  }
  if (depthChanged)
    --depth;

}  // findEmptyDirs()

/**************************************************************************//**
 * Finder(): Find, grep and display one directory at a time recursively.
 * \todo - separate flag for inverting a datetime restricted search?
 * \todo - if symlink off, they are neither listed nor followed; if on, links
 *     to files are listed and grepped but links to dirs are followed i.e.
 *     these we cannot grep.  But we also lose grepping links to files
 *     because, when on, these shall be replaced by targets.
 * \todo - better test inversion to make sure no double inversion occurs
 ******************************************************************************/
void MainWindow::finder (
  const QString &_path
) 
{
  if (!runState)  // check for exit state
    return;

  QDir currentDir(_path);

  qd(7) << "current dir-" << currentDir.absolutePath();

  int cnt = currentDir.count() - 2;  // not . or ..

  QString s = QString("  [%1 files]").arg(cnt);
  QString s2 = (cnt > 256) ? s : QString();
  statusBar()->showMessage(_path + s2);

  qApp->processEvents();

  // get list of files in this dir with wildcard glob
  QStringList files = currentDir.entryList(m_fileNames, QDir::Files | flags(), QDir::Reversed);
  qd(7) << "files-" << files;

  if (modeSymLink())
    {;} // resolveSymLinks(files, currentDir);  // conversion (no inversion)

  if (modeInvert())
    files = complement(files, currentDir);  // invert based on filename

  if (modeDateTime())
    files = filterDateTime(files, currentDir);  // supports search inversion

  if (modeSize())
    files = filterSize(files, currentDir);  // supports search inversion

  if (sui->getPreset()->skipFilterSz())
    files = sui->getPreset()->filterSkip(files, currentDir);

  if (files.size()  &&  m_depth >= m_depthMin) {
    if (!m_content.isEmpty())
      files = grep(files, currentDir);  // filter file list based on regexp

    showFiles(files, currentDir);  // display matched files in this dir
  }

  // get list of dirs in this dir to process recursively
  QStringList dirs = currentDir.entryList(QStringList(),QDir::AllDirs|flags(), QDir::Reversed);
  qd(7) << "dirs-" << dirs;
   if (sui->getDir()->dirFilterSz())
     dirs = sui->getDir()->filterDirs(dirs);

  bool depthChanged = static_cast<bool>(dirs.size());

  if (depthChanged)
    ++m_depth;

  if (m_depth > m_depthMax) {
    --m_depth;
    return;
  }

  if (ui->subdirCheckBox->isChecked()) {
    register int size = dirs.size();
    for (register int i = 0; i < size; ++i)
      finder(currentDir.absoluteFilePath(dirs[i]));  // recurse
  }

  if (depthChanged)
    --m_depth;

}  // finder()

/**************************************************************************//**
 * Flags(): set flags for getting file/dir entry list.
 ******************************************************************************/

QFlags<QDir::Filter> MainWindow::flags  () 
{
  QFlags<QDir::Filter> flags = QDir::NoDotAndDotDot | QDir::System;

  if (caseSensitive())
    flags |= QDir::CaseSensitive;

  if (!modeSymLink())
    flags |= QDir::NoSymLinks;

  if (!modeSkipHidden())
    flags |= QDir::Hidden;

  return flags;
}

/**************************************************************************//**
 * GetHitIdx(): get next avail ptr from hit map pointer pool.
 *
 * returns: index of next available pointer (may realloc)
 ******************************************************************************/

int MainWindow::getHitIdx ()
{
  int idx = 0;
  while (idx < m_hitv.size()) {
    if (m_hitv[idx] == 0)
      break;
    idx++;
  }
  if (idx >= m_hitv.size()) {
    m_hitv.resize(m_hitv.size() + HITVEC_REALLOC_SZ);
    qDebug() << "resize() occurred from " << m_hitv.size() - 8 << " to " << m_hitv.size();
    return getHitIdx();  // realloc invalidates all indecies so start over
  }
  // idx now points at the first available ptr
  return idx;
}

/**************************************************************************//**
 * Grep(): Filter via regexp - grep one directory's worth of files.
 *
 * parms : files      - list of filenames in current directory
 *         currentDir - current directory find is processing
 * \todo-hangs in winxp on a *.pst file when MS Outlook is running -- fix this
 * \todo-should be able to use qtconcurrent::mappedReduced() here
 * \todo-support inverted grep?
 ******************************************************************************/

QStringList MainWindow::grep (
  const QStringList &files,
  const QDir &_currentDir
)
{

  QProgressDialog progressDialog(this);
  progressDialog.setCancelButtonText(tr("&Cancel"));
  progressDialog.setRange(0, files.size());
  progressDialog.setWindowTitle(tr("Grepping for content"));

  QStringList foundFiles;

  register int size = files.size();
  for (register int i = 0; i < size; ++i) {
    qApp->processEvents();

    progressDialog.setValue(i);
    progressDialog.setLabelText(tr("Searching file %1 of %2 in '%3' : (%4)")
              .arg(i)
              .arg(size)
              .arg(_currentDir.dirName())
              .arg(_currentDir.absoluteFilePath(files[i])));
    progressDialog.setWindowOpacity(0.90);

    if (!runState)
      progressDialog.cancel();
    if (progressDialog.wasCanceled())
      break;

    QFile file(_currentDir.absoluteFilePath(files[i]));

    QRegExp rx(m_content);
    rx.setPatternSyntax(m_syntax);
    rx.setCaseSensitivity(caseSensitivity());
    const QRegExp crx(rx);  // performance tweak

    if (file.open(QIODevice::ReadOnly)) {
      QString line;
      QTextStream in(&file);
      while (!in.atEnd()) {
        qApp->processEvents();  // \todo expensive?

        if (!runState)  // \todo expensive?
          progressDialog.cancel();
        if (progressDialog.wasCanceled())
          break;

        line = in.readLine();
        if (crx.indexIn(line, 0) != -1) {
          foundFiles << files[i];
          break;
        }
      }
    }
    else if (!suiIgnoreFileErrs())
      boxError(QString(tr("Could not open '%1'."))
            .arg(_currentDir.absoluteFilePath(files[i])));
  }  // for

  return (/*modeInvert()*/ false ? listSubtract (files, foundFiles)
                       : foundFiles);
}  // grep()
/**************************************************************************//**
 * InitLineEditCompleter(): setup filesystem completer with all line edits.
 ******************************************************************************/
void MainWindow::initLineEditCompleter  () {
  completer = new QCompleter(this);
  completer->setCaseSensitivity(Qt::CaseSensitive);
  completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);

  dirModel = new QDirModel(completer);
  dirModel->setNameFilters(QStringList());
  dirModel->setFilter(QDir::AllDirs|QDir::NoDotAndDotDot);
  dirModel->setSorting(QDir::Name|QDir::LocaleAware);
  dirModel->setResolveSymlinks(false);

  QModelIndex mi = dirModel->index(0,0);
  qDebug() << "rows-" << dirModel->rowCount() << " cols-"
      << dirModel->columnCount() << " (0,0) = " << mi.data();

  completer->setModel(dirModel);

  ui->quickDirectoryLineEdit->setCompleter(completer);
  ui->directoryLineEdit->setCompleter(completer);
}
/**************************************************************************//**
 * InitState(): assist constructor and Reset button with original config.
 ******************************************************************************/
void MainWindow::initState  () {
  m_depthMax = MAX_DIRECTORY_DEPTH;  // defaults ...
  m_depthMin = m_numFiles = 0;
  m_numHits = 0L;
  m_syntax = QRegExp::RegExp;
  m_content = "";

  ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
  ui->dateTimeEdit2->setDateTime(QDateTime::currentDateTime());

  sui->ui()->alsoRadioButton->setChecked(true);   // initialize Settings dialog
  sui->ui()->typenameLineEdit->setMaxLength(16);  // ...

  ui->logHistoryDockWidget->setVisible(false);
  on_actionLogHistory_toggled(false);             // configure taskbars
  ui->resultsBrowserDockWidget->setVisible(false);
  on_actionResultsBrowser_toggled(false);         // ...

  ui->actionVertical->setChecked(true);
  ui->actionQuick->setChecked(true);
  ui->actionAdvanced->setChecked(false);

  on_actionQuick_toggled(true);  // copies adv comboboxes to quick

  //ui->replaceComboBox->setCurrentIndex(0);
  //ui->replaceLineEdit->clear();

  resetAdvanced();

  ui->dateTimeComboBox->setCurrentIndex(0);
  ui->dateTimeEdit->setEnabled(false);
  ui->dateTimeEdit2->setEnabled(false);
  ui->sizeComboBox->setCurrentIndex(0);
  ui->sizeLineEdit->clear();
  ui->sizeLineEdit->setEnabled(false);
  ui->sizeUnitsComboBox->setCurrentIndex(2);
  ui->sizeUnitsComboBox->setEnabled(false);
  ui->sizeLineEdit2->clear();
  ui->sizeLineEdit2->setEnabled(false);
  ui->sizeUnitsComboBox2->setCurrentIndex(2);
  ui->sizeUnitsComboBox2->setEnabled(false);
  //ui->syntaxComboBox->setCurrentIndex(0);
  //ui->depthMaxSpinBox->setValue(MAX_DIRECTORY_DEPTH);
  //ui->depthMinSpinBox->setValue(0);

  ui->searchPushButton->setEnabled(true);
  ui->quickSearchPushButton->setEnabled(true);

  ui->cancelPushButton->setEnabled(false);
  ui->quickCancelPushButton->setEnabled(false);
  ui->clearPushButton->setEnabled(true);
  ui->quickClearPushButton->setEnabled(true);

  state = QuickSearchStateSingleton::Instance();

  ui->quickSearchPushButton->setFocus();
  ui->quickSearchPushButton->setDefault(true);
  ui->quickClearPushButton->setAutoDefault(true);

  m_mruTabWidget->init();  // clears the model (and the text edits)

}  // initState()
/**************************************************************************//**
 * KeyPressEvent(): reimplemented keypress event handler.
 ******************************************************************************/
void MainWindow::keyPressEvent  (QKeyEvent *event) {
  qd(2) << "got a key...";

  switch (event->key()) {

  case Qt::Key_Space:  // tree view must be grabbing this for some reason
  case Qt::Key_Enter:
  case Qt::Key_Return:
    if (m_mruTabWidget->tui()->proxyViewH->hasFocus() ||
       m_mruTabWidget->tui()->proxyViewV->hasFocus())
      switch (m_mruTabWidget->layout()) {
      case HORIZONTAL:
        m_mruTabWidget->displayLines(m_mruTabWidget->tui()->proxyViewH->currentIndex());
        return;
      case VERTICAL:
        m_mruTabWidget->displayLines(m_mruTabWidget->tui()->proxyViewV->currentIndex());
        return;
      }

    if (event->key() == Qt::Key_Space)
      return;

    switch (state->state()) {
    case QUICK:
      if (ui->quickDirectoryLineEdit->hasFocus() ||
         ui->quickFileLineEdit->hasFocus() ||
         ui->quickContentLineEdit->hasFocus()) {
        find();
        return;
      }
      break;
    case ADVANCED:
      if (ui->directoryLineEdit->hasFocus() ||
         ui->fileLineEdit->hasFocus() ||
         ui->contentLineEdit->hasFocus() ||
         ui->sizeLineEdit->hasFocus() ||
         ui->sizeLineEdit2->hasFocus() ||
         ui->dateTimeEdit->hasFocus() ||
         ui->dateTimeEdit2->hasFocus()) {
        find();
        return;
      }
    }

    break;

  case Qt::Key_Escape:
    if (runState && boxConfirm(tr("Really Cancel?")))
      stop(FLAG/*THROW*/);  // throws
    break;

  case Qt::Key_Down:
  case Qt::Key_Up:
    ; // These keys are consumed by the tree view, that is, when the tree
      // view has focus and the up or down arrow is used, this handler is
      // not called and these cases are not considered.  What to do?
    break;

  default:
    break;

  }  // switch()

  QMainWindow::keyPressEvent(event);

}  // keyPressEvent()
/**************************************************************************//**
 * License(): License menu option.
 ******************************************************************************/
void MainWindow::license  () {
  lui->show();
}
/**************************************************************************//**
 * ModeDateTime(): mode is find files with date/time constraints.
 ******************************************************************************/
bool MainWindow::modeDateTime  () {
  return ui->actionAdvanced->isChecked() &&
       ui->dateTimeComboBox->currentText() != "Any date and time";
}
/**************************************************************************//**
 * ModeSkipHidden(): mode is find/grep skipping hidden files.
 ******************************************************************************/
bool MainWindow::modeSkipHidden  () {
  return ui->actionAdvanced->isChecked()&& ui->skipHiddenCheckBox->isChecked();
}
/**************************************************************************//**
 * ModeInvert(): mode is find/grep for non-matching filenames/lines.
 ******************************************************************************/
bool MainWindow::modeInvert  () {
  //return ui->actionAdvanced->isChecked() && ui->invertCheckBox->isChecked();
  return false;
}
/**************************************************************************//**
 * ModeSize(): mode is find/grep for non-matching filenames/lines.
 ******************************************************************************/
bool MainWindow::modeSize  () {
  return ui->actionAdvanced->isChecked()  &&
       ui->sizeComboBox->currentText() != "Any size";
}
/**************************************************************************//**
 * ModeSymLink(): mode is find/grep following symbolic links.
 ******************************************************************************/
bool MainWindow::modeSymLink  () {
  return ui->actionAdvanced->isChecked() && ui->symLinkCheckBox->isChecked();
}
/**************************************************************************//**
 * ReadSettings(): read some settings.
 ******************************************************************************/
void MainWindow::readSettings  () {
    QSettings settings("Project Searchmonkey", "Searchmonkey");

    settings.beginGroup("MainWindow");
    resize(settings.value("size", QSize(400, 400)).toSize());
    move(settings.value("pos", QPoint(200, 200)).toPoint());
    m_mruDir = settings.value("mruDir", "").toString();
    m_mruQuickDir = settings.value("mruQuickDir", "").toString();
    qDebug() << "m_mruQuickDir-" << m_mruQuickDir;
    settings.endGroup();
 }
/**************************************************************************//**
 * Refresh(): Refresh main window.  Red text for invalid entries.  [slot]
 ******************************************************************************/
void MainWindow::refresh  () {
  qApp->processEvents();

  QPalette palette;
  if (ui->actionAdvanced->isChecked()) {

    palette = ui->directoryLineEdit->palette();
    palette.setColor(QPalette::Text,
                IsValidDir(ui->directoryLineEdit->text())?
                                                                     COLOR_TEXT : COLOR1_HIGHLIGHT);
    ui->directoryLineEdit->setPalette(palette);

    palette = ui->fileLineEdit->palette();
    palette.setColor(QPalette::Text,
                validRx(ui->fileLineEdit->text(), QRegExp::Wildcard)?
                                                                     COLOR_TEXT : COLOR1_HIGHLIGHT);
    ui->fileLineEdit->setPalette(palette);

    palette = ui->contentLineEdit->palette();
    palette.setColor(QPalette::Text,
                validRx(ui->contentLineEdit->text(), m_syntax)?
                                                                     COLOR_TEXT : COLOR1_HIGHLIGHT);
    ui->contentLineEdit->setPalette(palette);
  }
  else {
    palette = ui->quickDirectoryLineEdit->palette();
    palette.setColor(QPalette::Text,
                IsValidDir(ui->quickDirectoryLineEdit->text())?
                                                                     COLOR_TEXT : COLOR1_HIGHLIGHT);
    ui->quickDirectoryLineEdit->setPalette(palette);

    palette = ui->quickFileLineEdit->palette();
    palette.setColor(QPalette::Text,
                validRx(ui->quickFileLineEdit->text(), QRegExp::Wildcard)?
                                                                     COLOR_TEXT : COLOR1_HIGHLIGHT);
    ui->quickFileLineEdit->setPalette(palette);

    palette = ui->quickContentLineEdit->palette();
    palette.setColor(QPalette::Text,
                validRx(ui->quickContentLineEdit->text(), m_syntax)?
                                                                     COLOR_TEXT : COLOR1_HIGHLIGHT);
    ui->quickContentLineEdit->setPalette(palette);
  }

  if (ui->actionAdvanced->isChecked()) {
    int pos = 0;
    QString s = ui->sizeLineEdit->text();
    palette = ui->sizeLineEdit->palette();
    palette.setColor(QPalette::Text, sizeValidator->validate(s, pos) ==
                QValidator::Acceptable ? COLOR_TEXT : Qt::green);
    ui->sizeLineEdit->setPalette(palette);

    s = ui->sizeLineEdit2->text();
    palette = ui->sizeLineEdit2->palette();
    palette.setColor(QPalette::Text, sizeValidator->validate(s, pos) ==
                QValidator::Acceptable ? COLOR_TEXT : Qt::green);
    ui->sizeLineEdit2->setPalette(palette);
  }

}  // refresh()
/**************************************************************************//**
 * ResetActiveTab(): reset the active tab to the highest indexed tab.
 ******************************************************************************/
void MainWindow::resetActiveTab  () {
  int max    = 0,
     target = 0;

  for (int i = 0; i < MAX_SEARCH_TABS; i++) {
    if (tui[i] == NULL)
      continue;
    int idx = ui->resultsTabWidget->indexOf(tui[i]);
    if (idx >= max) {
      max = idx;
      target = i;
    }
  }
  {
    // Target is the index of the tab with the highest ui index position.
    m_mruTabWidget = tui[target];
    //
    // Assign the active tab results ui ptr to the main window shortcut ptr.
    //
    m_tui = m_mruTabWidget->tui();
  }
}
/**************************************************************************//**
 * ResetAdvanced(): Reset advanced checkboxes.  [slot]
 ******************************************************************************/
void MainWindow::resetAdvanced  () {
  ui->subdirCheckBox->setChecked(true);
  //ui->binaryCheckBox->setChecked(false);
  //ui->invertCheckBox->setChecked(false);
  //ui->emptyDirsCheckBox->setChecked(false);
  ui->caseCheckBox->setChecked(false);
  ui->symLinkCheckBox->setChecked(false);
  ui->skipHiddenCheckBox->setChecked(false);
}
/**************************************************************************//**
 * ResolveSymLinks(): convert symlinks in the list to be the actual targets,
 *                    unless broken - then leave actual link in list.
 ******************************************************************************/
void MainWindow::resolveSymLinks  (QStringList &_files,const QDir &_currentDir){
  register int size = _files.size();
  for (register int i = 0; i < size; ++i) {

    QString abspath = _currentDir.absoluteFilePath(_files[i]),
          target  = QFile::symLinkTarget(abspath);

    if (!target.isEmpty() && QFile::exists(target))
      _files[i] = target;
  }
}
/**************************************************************************//**
 * SetDepthMax(): Set max search depth from signal.  [slot]
 ******************************************************************************/
void MainWindow::setDepthMax(int _depth) {
  m_depthMax = _depth;
  qDebug() << "set max depth-" << m_depthMax;
}
/**************************************************************************//**
 * SetDepthMin(): Set min search depth from signal.  [slot]
 ******************************************************************************/
void MainWindow::setDepthMin(int _depth) {
  m_depthMin = _depth;
  qDebug() << "set min depth-" << m_depthMin;
}
/**************************************************************************//**
 * SetLineEditSize(): Force line edits to expand with search widget.  [slot]
 ******************************************************************************/
void MainWindow::setLineEditSize  () {
  QSizePolicy sp(QSizePolicy::Expanding, QSizePolicy::Fixed, QSizePolicy::LineEdit);
  sp.setHorizontalStretch(99);

  ui->directoryLineEdit->setSizePolicy(sp);
  ui->fileLineEdit->setSizePolicy(sp);
  ui->contentLineEdit->setSizePolicy(sp);
  ui->quickDirectoryLineEdit->setSizePolicy(sp);
  ui->quickFileLineEdit->setSizePolicy(sp);
  ui->quickContentLineEdit->setSizePolicy(sp);

  QSizePolicy sp2(QSizePolicy::Expanding, QSizePolicy::Fixed);
  ui->toolBox->setSizePolicy(sp);
  /*
  ui->dockWidgetContents_3->setSizePolicy(sp2);
  ui->stackedWidget->setSizePolicy(sp2);
  ui->page_5->setSizePolicy(sp2);
  ui->formGroupBox->setSizePolicy(sp);
  */
}
/**************************************************************************//**
 * SetSyntax(): Set grep syntax.  [slot]
 ******************************************************************************/
void MainWindow::setSyntax  (int _index) {
  qd(3) << "setting syntax on index-" << _index;
  m_syntax = uint2syntax(static_cast<uint>(_index));
  qd(3) << "syntax set-" << m_syntax;
}
/**************************************************************************//**
 * ShowEmptyDirs(): Display empty dirs in the passed list.
 ******************************************************************************/
void MainWindow::showEmptyDirs  (QStringList _dirs, const QDir &_currentDir) {

  if (modeDateTime())
    _dirs = filterDateTime(_dirs, _currentDir);  // supports search inversion

  register int size = _dirs.size();
  for (register int i = 0; i < size; ++i) {

    QString abspath = _currentDir.absoluteFilePath(_dirs[i]);

    QDir dir(abspath);
    QFile file(abspath);
    QFileInfo fi(file);
    QDateTime dt = fi.lastModified();

    addFile(m_showModel, abspath,  bytes2units(file.size()), 0, "dir",
          dt.toString("MM.dd.yy hh:mm:ss"));
  }
  m_numFiles += size;
}
/**************************************************************************//**
 * ShowFiles(): Display a directory worth of files after handing off future hit
 *              map (for the "Matches" column) to mappedReduced().
 ******************************************************************************/
void MainWindow::showFiles  (const QStringList &_files,const QDir &_currentDir){

  if (!m_content.isEmpty()) {

    QStringList abspaths =  filepath2abspath(_files, _currentDir);

    MapGlobals * z = Singleton<class MapGlobals>::Instance();
    z->setCase(caseSensitivity());
    z->setPtrn(m_content);
    z->setSntx(m_syntax);
    //
    // mappedReduced() does not support passing multiple parameters to map
    // function, and places restrictions on passing member functions.
    //
    int i = getHitIdx();
    m_hitv[i] = new Hits;
    connect(&m_hitv[i]->watcher, SIGNAL(finished()), this, SLOT(hitMapReady()));
    m_hitv[i]->futureHits = QtConcurrent::mappedReduced(abspaths, countHits, reduceHits);
    m_hitv[i]->watcher.setFuture(m_hitv[i]->futureHits);
    sem.up();
    qd(3) << "sem count: " << sem.getCount();
  }

  register int size = _files.size();
  for (register int i = 0; i < size; ++i) {

    // does nothing if files[n] already absolute eg. symlink target
    QString abspath = _currentDir.absoluteFilePath(_files[i]);

    QFile file(abspath);
    QFileInfo fi(file);

    QDateTime dt = fi.lastModified();

    addFile(m_showModel, abspath, bytes2units(file.size()),
         !m_content.isEmpty() ? 1 : 0, fi.suffix(),
          dt.toString("MM.dd.yy hh:mm:ss"));

    //ui->lcdNumber->display(static_cast<double>(m_numHits+=hitMap[abspath]));
  }
  m_numFiles += size;

}  // showFiles()
/**************************************************************************//**
 * showStatus(): show init status message in the main status bar.
 ******************************************************************************/
void MainWindow::showStatus  (QAction *) {
  statusBar()->showMessage(tr("Ready."));
}
/**************************************************************************//**
 * StateDateTime(): Set GUI state based on dateTimeComboBox's state.  [slot]
 ******************************************************************************/
void MainWindow::stateDateTime  (QString _option) {
  if (_option == "Ignored") {
    ui->dateTimeEdit->setEnabled(false);
    ui->dateTimeEdit2->setEnabled(false);
  }
  else if (_option.indexOf("Between") != -1) {
    ui->dateTimeEdit->setEnabled(true);
    ui->dateTimeEdit2->setEnabled(true);
  }
  else {  // before/after
    ui->dateTimeEdit->setEnabled(true);
    ui->dateTimeEdit2->setEnabled(false);
  }
}
/**************************************************************************//**
 * StateEmptyDirsOnly(): Set GUI state based on this checkbox's state.  [slot]
 ******************************************************************************/
void MainWindow::stateEmptyDirsOnly  (int _state) {

  bool state = (_state == Qt::Checked) ? false : true;

  ui->fileLineEdit->setEnabled(state);
  //ui->syntaxComboBox->setEnabled(state);
  ui->contentLineEdit->setEnabled(state);
}
/**************************************************************************//**
 * StateFind(): Set GUI state based on find state: true-before; false-after
 ******************************************************************************/
void MainWindow::stateFind  (bool _state) {
  runState = _state;

  if (!_state && !m_content.isEmpty() && !modeEmptyDirs())
    hitMapDisplay();  // may wait until hit map ready to display

  ui->searchPushButton->setEnabled(!_state);
  ui->quickSearchPushButton->setEnabled(!_state);
  ui->clearPushButton->setEnabled(!_state);
  ui->quickClearPushButton->setEnabled(!_state);

  ui->browseToolButton->setEnabled(!_state);
  ui->quickBrowseToolButton->setEnabled(!_state);

  ui->toolBox->setEnabled(!_state);
  //ui->menuBar->setEnabled(!_state);

  ui->cancelPushButton->setEnabled(_state);
  ui->quickCancelPushButton->setEnabled(_state);

  if (_state) {
    m_showModel = m_mruTabWidget->getModelPtr();
    qDebug() << "set show model: " << m_showModel;
    m_depth = 0;
    m_numFiles = 0;
    m_numHits = 0L;
    if (state->state() == ADVANCED) {
      ui->cancelPushButton->setDefault(true);
      ui->cancelPushButton->setFocus();
    }
    else {
      ui->quickCancelPushButton->setDefault(true);
      ui->quickCancelPushButton->setFocus();
    }
  }
  else {
    if (state->state() == ADVANCED) {
      ui->searchPushButton->setDefault(true);
      ui->searchPushButton->setFocus();
    }
    else {
      ui->quickSearchPushButton->setDefault(true);
      ui->quickSearchPushButton->setFocus();
    }
    qDebug() << "nullifying show model: " << m_showModel;
    m_showModel = NULL;
  }
}
/**************************************************************************//**
 * StateSize(): Set GUI state based on sizeComboBox's state.  [slot]
 ******************************************************************************/
void MainWindow::stateSize  (QString _option) {
  if (_option.indexOf("Ignore") != -1) {
    ui->sizeLineEdit->setEnabled(false);
    ui->sizeLineEdit2->setEnabled(false);
    ui->sizeUnitsComboBox->setEnabled(false);
    ui->sizeUnitsComboBox2->setEnabled(false);
  }
  else if (_option.indexOf("Between") != -1) {
    ui->sizeLineEdit->setEnabled(true);
    ui->sizeLineEdit2->setEnabled(true);
    ui->sizeUnitsComboBox->setEnabled(true);
    ui->sizeUnitsComboBox2->setEnabled(true);
  }
  else {  // larger/smaller
    ui->sizeLineEdit->setEnabled(true);
    ui->sizeLineEdit2->setEnabled(false);
    ui->sizeUnitsComboBox->setEnabled(true);
    ui->sizeUnitsComboBox2->setEnabled(false);
  }
}
/**************************************************************************//**
 * Stop(): Stop (button) to halt find operation.  [slot]
 ******************************************************************************/
void MainWindow::stop  (bool _run) {
  if (_run == FLAG) {
    qd(2) << "setting runState to false...";
    runState = false;
  }
  else {  // THROW
    Stop s;
    throw s;
  }
}
/**************************************************************************//**
 * suiIgnoreFileErrs(): return flag to ignore filesystem errors.
 ******************************************************************************/
bool MainWindow::suiIgnoreFileErrs  () {
  return sui->ui()->ignoreCheckBox->isChecked();
}
/**************************************************************************//**
 * suiTabbedSearching(): return flag for tabbed searching.
 ******************************************************************************/
bool MainWindow::suiTabbedSearching() {
  return sui->ui()->tabCheckBox->isChecked();
}
/**************************************************************************//**
 * WriteSettings(): write some settings.
 ******************************************************************************/
void MainWindow::writeSettings  () {
    QSettings settings("Project Searchmonkey", "Searchmonkey");

    settings.beginGroup("MainWindow");
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.setValue("mruDir", m_mruDir);
    settings.setValue("mruQuickDir", m_mruQuickDir);
    settings.endGroup();
 }
/**************************************************************************//**
 * Function group [slots]: headers generated by QtDesigner.
 ******************************************************************************/
void MainWindow::on_actionAdvanced_toggled(bool _toggle) {
  if (_toggle) {
    qDebug() << "got sig on action advanced toggled(" << _toggle << ")";
    state = AdvancedSearchStateSingleton::Instance();

    ui->searchPushButton->setFocus();
    ui->searchPushButton->setDefault(true);
    ui->clearPushButton->setAutoDefault(true);

    ui->quickSearchPushButton->setDefault(false);
    ui->quickClearPushButton->setAutoDefault(false);

    ui->stackedWidget->setCurrentIndex(1);
    ui->actionQuick->setChecked(false);
    stateAct->setText("&Quick");
    if (m_ctorDone) {  // ignore sigs emitted during construction
      ui->directoryLineEdit->setText(
                      ui->quickDirectoryLineEdit->text());
      ui->fileLineEdit->setText(ui->quickFileLineEdit->text());
      ui->contentLineEdit->setText(ui->quickContentLineEdit->text());
    }
  }
}
void MainWindow::on_actionHorizontal_toggled(bool _toggle) {
  if (_toggle) {
    ui->actionVertical->setChecked(false);  // emits to vert toggle slot
    m_tui->resultsStackedWidget->setCurrentIndex(0);
    if (!m_tui->lineDisplayV->toPlainText().isEmpty()) {
      Highlighter *highlighter =new Highlighter(m_tui->lineDisplayH->document(),
                             m_content, m_syntax, caseSensitivity());
      highlighter->init();
      m_tui->lineDisplayH->setHtml(m_tui->lineDisplayV->toHtml());
    }
    layoutAct->setText("&Vertical");
    m_mruTabWidget->layoutPushButtonSetLabel();
  }
}
void MainWindow::on_actionLogHistory_toggled(bool toggle) {
  if (ui->logHistoryDockWidget->isVisible() != toggle) {
    ui->logHistoryDockWidget->setVisible(toggle);
    logHistoryAct->setVisible(!toggle);
  }
}
void MainWindow::on_actionQuick_toggled(bool _toggle) {
  if (_toggle) {
    state = QuickSearchStateSingleton::Instance();

    ui->quickSearchPushButton->setFocus();
    ui->quickSearchPushButton->setDefault(true);
    ui->quickClearPushButton->setAutoDefault(true);

    ui->searchPushButton->setDefault(false);
    ui->clearPushButton->setAutoDefault(false);

    ui->stackedWidget->setCurrentIndex(0);
    ui->actionAdvanced->setChecked(false);
    stateAct->setText("&Advanced");
    if (m_ctorDone) {  // ignore sigs emitted during construction
      ui->quickDirectoryLineEdit->setText(
                         ui->directoryLineEdit->text());
      ui->quickFileLineEdit->setText(ui->fileLineEdit->text());
      ui->quickContentLineEdit->setText(ui->contentLineEdit->text());
    }
  }
}
void MainWindow::on_actionRestore_triggered() {
  if (boxConfirm(tr("Restore configuration to factory defaults?"))) {
    QuickSearchStateSingleton::Instance()->reset();
    AdvancedSearchStateSingleton::Instance()->reset();
    initState();
  }
}
void MainWindow::on_actionResultsBrowser_toggled(bool toggle) {
  if (ui->resultsBrowserDockWidget->isVisible() != toggle) {
    ui->resultsBrowserDockWidget->setVisible(toggle);
    resultsBrowserAct->setVisible(!toggle);
  }
}
void MainWindow::on_actionSearch_Dialog_toggled(bool toggle) {
  ui->dockWidget->setVisible(toggle);
}
void MainWindow::on_actionSettings_triggered() {
  sui->show();
  sui->getDir()->saveInCaseCancel();  // directories tab
  sui->saveInCaseCancel();            // general tab
}
void MainWindow::on_actionVertical_toggled(bool _toggle) {
  if (_toggle) {
    ui->actionHorizontal->setChecked(false);  // emits to horiz toggle slot
    qd(3) << "setting stack widget to 1...";
    m_tui->resultsStackedWidget->setCurrentIndex(1);
    if (!m_tui->lineDisplayH->toPlainText().isEmpty()) {
      Highlighter *highlighter =new Highlighter(m_tui->lineDisplayV->document(),
                               m_content, m_syntax, caseSensitivity());
      highlighter->init();
      m_tui->lineDisplayV->setHtml(m_tui->lineDisplayH->toHtml());
      //delete highlighter; \todo auto_ptr?
    }
    layoutAct->setText("&Horizontal");
    m_mruTabWidget->layoutPushButtonSetLabel();
  }
}
void MainWindow::on_browseToolButton_clicked() {
  browse(ui->directoryLineEdit);
}
void MainWindow::on_cancelPushButton_clicked() {
  if (!boxConfirm("Really Cancel?"))
    return;
  if (runState)
    stop(FLAG/*THROW*/);
  else {
    if (m_flagCalculating)
      m_abortCalculating = true;
    else if (m_flagPopulating)
      m_abortPopulating = true;
  }
}
void MainWindow::on_clearPushButton_clicked() {
  clear();
}
void MainWindow::on_contentLineEdit_textChanged(QString) {
  refresh();
}
void MainWindow::on_dateTimeComboBox_currentIndexChanged(QString option) {
  stateDateTime(option);
}
void MainWindow::on_depthMaxSpinBox_valueChanged(int max) {
  setDepthMax(max);
}
void MainWindow::on_depthMinSpinBox_valueChanged(int min) {
  setDepthMin(min);
}
void MainWindow::on_directoryLineEdit_textChanged(QString s) {
  int sz = s.size();
  if (sz > 0  &&  s[sz - 1] == QChar('\\')) {
    qd(3) << "sorting QDirModel for Completer...";
    dirModel->sort(0);  // allows completer to do binary search
  }
  refresh();
}
void MainWindow::on_dockWidget_visibilityChanged(bool visible) {
  // ignore sig if user minimizes main win; otherwise, dock widget stays hidden
  qApp->processEvents();
  if (this->isMinimized())
    return;
  ui->actionSearchDialog->setChecked(visible);
}
void MainWindow::on_emptyDirsCheckBox_stateChanged(int state) {
  stateEmptyDirsOnly(state);
}
void MainWindow::on_fileLineEdit_textChanged(QString) {
  refresh();
}
void MainWindow::on_lineDisplayH_textChanged() {
  documentWasModified();
}
void MainWindow::on_lineDisplayV_textChanged() {
  documentWasModified();
}
void MainWindow::on_logHistoryDockWidget_visibilityChanged(bool visible) {
  if (ui->actionLogHistory->isChecked() != visible) {
    ui->actionLogHistory->setChecked(visible);
    logHistoryAct->setVisible(!visible);
  }
}
void MainWindow::on_quickBrowseToolButton_clicked() {
  browseQuick(ui->quickDirectoryLineEdit);
}
void MainWindow::on_quickCancelPushButton_clicked() {
  on_cancelPushButton_clicked();  // both do same thing
}
void MainWindow::on_quickClearPushButton_clicked() {
  clear();
}
void MainWindow::on_quickContentLineEdit_textChanged(QString) {
  refresh();
}
void MainWindow::on_quickDirectoryLineEdit_textChanged(QString s) {
  int sz = s.size();
  if (sz > 0  &&  s[sz - 1] == QChar('\\')) {
    qd(3) << "sorting QDirModel for Completer...";
    dirModel->sort(0);  // allows completer to do binary search
  }
  refresh();
}
void MainWindow::on_quickFileLineEdit_textChanged(QString) {
  refresh();
}
void MainWindow::on_quickSearchPushButton_clicked() {
  qDebug() << "initiating a search *********************************"
          "******************************************************";
  //m_mruTabWidget->getModelPtr()->setRowCount(0);
  //coordinator.StartSearch(state->GatherSearchParameter(this->ui,this->sui));
  find();
  //_find();
}
void MainWindow::on_resetAdvButton_clicked() {
  resetAdvanced();
}
void MainWindow::on_resultsBrowserDockWidget_visibilityChanged(bool visible) {
  if (ui->actionResultsBrowser->isChecked() != visible) {
    ui->actionResultsBrowser->setChecked(visible);
    resultsBrowserAct->setVisible(!visible);
  }
}
void MainWindow::on_searchPushButton_clicked() {
  on_quickSearchPushButton_clicked();
}
void MainWindow::on_sizeComboBox_currentIndexChanged(QString option) {
  stateSize(option);
}
void MainWindow::on_sizeLineEdit_textEdited(QString) {
  refresh();
}
void MainWindow::on_sizeLineEdit2_textEdited(QString) {
  refresh();
}
void MainWindow::on_syntaxComboBox_currentIndexChanged(int index) {
  setSyntax(index);
}
void MainWindow::on_tbarActionLayout_triggered() {
  if (layoutAct->text() == "&Horizontal")
    ui->actionHorizontal->setChecked(true);
  else
    ui->actionVertical->setChecked(true);
}
void MainWindow::on_tbarActionLogHistory_triggered() {
  on_actionLogHistory_toggled(true);
  logHistoryAct->setVisible(false);
}
void MainWindow::on_tbarActionResultsBrowser_triggered() {
  on_actionResultsBrowser_toggled(true);
  resultsBrowserAct->setVisible(false);
}
void MainWindow::on_tbarActionSettings_triggered() {
  on_actionSettings_triggered();
}
void MainWindow::on_tbarActionState_triggered() {
  if (stateAct->text() == "&Quick")
    ui->actionQuick->setChecked(true);     // emits sig toggled()
  else
    ui->actionAdvanced->setChecked(true);  // emits sig toggled()
}
/* \todo bool MainWindow::event(QEvent *e) {
  if(e->type() == QEvent::WindowStateChange)
    if (isMinimized()) {
      qApp->processEvents();
      QTimer::singleShot(250, this, SLOT(hide()));
      e->ignore();
    }
  return QMainWindow::event(e);
} */
void MainWindow::on_resultsTabWidget_currentChanged(int index) {
  if (ui->resultsTabWidget->count() < 2)  // don't bother
    return;
  /*
   * For future coding be aware that this function gets called whenever a tab
   * is removed, even if current tab did NOT change.
   */

  qd(2) << "got sig current changed; m_tabAdded-" << m_tabAdded;

  if (m_tabAdded)  // already ripped context adding tab
    m_tabAdded = false;
  else {
    m_mruTabWidget->gui()->ripContext(ui, sui->ui());  // rip prev context
    qd(2) << "ripped-" << m_mruTabWidget;
  }

  //
  // If tabbed searching is off, we want the active tab to be the same as the
  // current tab, so that results are displayed in the visible (or only) tab.
  //
  // This is precisely the current tab widget.
  quintptr ptr = reinterpret_cast<quintptr>(ui->resultsTabWidget->widget(index));

  // Also need the index so we know the array element to assign as active tab.
  int i = 0;
  for (; i < MAX_SEARCH_TABS; i++)
    if (reinterpret_cast<quintptr>(tui[i]) == ptr)
      break;

  if (i == MAX_SEARCH_TABS)  // very unlikely since we created tab earlier
    Q_ASSERT(0);
  {
    m_mruTabWidget = tui[i];  // assign the current tab to active tab variable
    //
    // Assign the active tab results ui ptr to the main window shortcut ptr.
    //
    m_tui = m_mruTabWidget->tui();
  }
  if (m_mruTabWidget->getModelPtr()->rowCount() > 0) {   // ignore new tabs
    qDebug() << "burn target idx-" << i;
    m_mruTabWidget->gui()->burnContext(ui, sui->ui());  // gui context
    qDebug() << "burned-" << m_mruTabWidget;
  }

  debugResultsTab("ortwcc()");
}
void MainWindow::on_resultsTabWidget_tabCloseRequested(int index) {

  if (ui->resultsTabWidget->count() < 2)  // always keep one tab
    return;

  // This is precisely the widget to delete.
  quintptr ptr = reinterpret_cast<quintptr>(ui->resultsTabWidget->widget(index));

  ui->resultsTabWidget->removeTab(index);

  // We also need the index so we can set the ptr to NULL.
  int i = 0;
  for (; i < MAX_SEARCH_TABS; i++)
    if (reinterpret_cast<quintptr>(tui[i]) == ptr)
      break;

  if (i == MAX_SEARCH_TABS)  // very unlikely since we created tab earlier
    Q_ASSERT(0);

  bool reset = (tui[i] == m_mruTabWidget);  // if deleting active tab

  delete tui[i];
  tui[i] = NULL;

  if (reset)
    resetActiveTab(); // reset active tab (in case of non-tabbed searching)

  debugResultsTab("ortwtcr()");
}
void MainWindow::ResultAvailable  (const Result& result) {
  qd(3) << "ResultAvailable(): calling addFile()...";
  addFile(m_showModel,
        result->matchedFile.absoluteFilePath(),
        bytes2units(result->matchedFile.size()),
        result->matchedLines.size(),
        result->matchedFile.suffix(),
        result->matchedFile.lastModified().toString("MM.dd.yy hh:mm:ss"));
}
