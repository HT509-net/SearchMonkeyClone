/*****************************************************************************/
/*! \file :    mainwindow.h
 *
 * PURPOSE:    Main program header file.
 *
 * DATE:       September 28, 2009
 *
 * REVISIONS:
 * ---------
 * [092809] shawno- initial coding.
*******************************************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtConcurrentRun>
#include <QtGui/QMainWindow>

#include <QDir>
#include <QFlags>
#include <QFuture>
#include <QFutureWatcher>
#include <QRunnable>
#include <QThread>
#include <QThreadPool>
#include <QVector>

#include "global.h"
#include "SearchResult.h"
#include "SearchCoordinator.h"
#include "ui_mainwindow.h"
#include "Utility.h"

QT_BEGIN_NAMESPACE
class QAbstractButton;
class QAbstractItemModel;
class QCloseEvent;
class QComboBox;
class QCompleter;
class QDir;
class QDirModel;
class QGroupBox;
class QLabel;
class QRegExpValidator;
class QStandardItemModel;
class QTextEdit;
class QTreeView;
QT_END_NAMESPACE

class About;
class Hits;
class License;
class Preset;
class SearchCoordinator;
class SearchState;
class Settings;
class SortFilterProxyModel;
class TabResults;

namespace Ui {
	class MainWindow;
	class TabResults;
}

typedef QFuture<WordCount> fwc;  ///<  QMap<QString, int>
typedef QFutureWatcher<void> fw;

const qint16 HITVEC_REALLOC_SZ = 64;  ///< realloc size for m_hitv (hit vector)

class Sem {
public:
	Sem() { m_count = 0; }
	inline int getCount() const { return m_count; }
	inline void down() { --m_count; }
	inline void reset() { m_count = 0; }
	inline void up() { ++m_count; }

private:
	int m_count;
};

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();
	void addFile(QAbstractItemModel *model, const QString &filename,
					 const QString &size, int hits, const QString &type,
					 const QString &modified);
	void browse(QLineEdit *);
	void browseQuick(QLineEdit *);
	inline bool caseSensitive();
	Qt::CaseSensitivity caseSensitivity();
	void clear();
	QStringList complement(const QStringList & files, const QDir & dir);
	void createActions();
	void createResultsTab(const QString &);
	void debugResultsTab(const QString &) const;
	///<bool event(QEvent *e);
	QStringList filepath2abspath(const QStringList &_files, const QDir &_dir);
	QStringList filterDateTime(const QStringList &files, const QDir &currentDir);
	QStringList filterSize(const QStringList &_files, const QDir &_currentDir);
	void find() throw();
	void _find() throw();
	void findEmptyDirs(const QString &path);
	void finder(const QString &path);
	QFlags<QDir::Filter> flags();
	int getHitIdx();
	QStringList grep(const QStringList &files, const QDir &currentDir);
	void hitMapDisplay();
	void initLineEditCompleter();
	void initState();
	inline bool modeEmptyDirs() {	return false; /*ui->actionAdvanced->isChecked() && ui->emptyDirsCheckBox->isChecked();*/ }
	inline bool modeInvert();
	inline bool modeDateTime();
	inline bool modeSize();
	inline bool modeSkipHidden();
	inline bool modeSymLink();
	void readSettings();
	void refresh();
	void resetActiveTab();
	void resetAdvanced();
	void resolveSymLinks(QStringList &_files, const QDir &_currentDir);
	void setDepthMax(int depth);
	void setDepthMin(int depth);
	void setLineEditSize();
	void setSourceModel(QAbstractItemModel *model);
	void setSyntax(int index);
	QString setText();
	void showEmptyDirs(QStringList dirs, const QDir &currentDir);
	void showFiles(const QStringList &files, const QDir &currentDir);
	void stateDateTime(QString option);
	void stateEmptyDirsOnly(int state);
	void stateFind(bool state);
	void stateSize(QString _option);
	bool suiIgnoreFileErrs();
	bool suiTabbedSearching();
	void writeSettings();

	inline QString getContent() const { return m_content; }
	inline QAction *getLayoutAct() const { return layoutAct; }
	inline SearchState *getState() const { return state; }
	inline QRegExp::PatternSyntax getSyntax() const { return m_syntax; }
	inline Settings *getSui() const { return sui; }
	inline Ui::MainWindow *getUi() const { return ui; }

	QLabel *normalLabel;              ///< normal duration status message (for grep)
private:
	TabResults *tui[MAX_SEARCH_TABS]; ///< the results tabs in the main window
	QVector<Hits *> m_hitv;

	About *aui;                       ///< ptr to the About object
	License *lui;                     ///< ptr to the License object
	Settings *sui;                    ///< ptr to the Settings object
	Ui::MainWindow *ui;               ///< ptr to the main window ui
	Ui::TabResults *m_tui;            ///< shortcut ptr to active results tab ui

	TabResults *m_mruTabWidget;       ///< most recently used tab widget

	bool m_abortCalculating;          //!< \var stop calculating hits (matches)
	bool m_abortPopulating;           ///< stop populating Matches column
	bool m_ctorDone;                  ///< true if constructor done
	bool m_flagCalculating;           ///< calculating
	bool m_flagPopulating;            ///< popuating
	bool m_tabAdded;                  ///< a tab was just added
	bool runState;                    ///< TRUE if running a search
	int m_depth,                      ///< current depth counter
		 m_depthMax,                   ///< search depth max
		 m_depthMin;                   ///< "          " min
	qint32 m_numFiles;                ///< total files found
	qint64 m_numHits;                 ///< total matches within file
	QCompleter *completer;            ///< filesystem completer for line edits
	QDirModel *dirModel;              ///< basis of filesystem completer
	QLabel *statusLabel;              ///< status bar message
	QRegExp::PatternSyntax m_syntax;  ///< content match (grep) syntax
	QStandardItemModel *m_showModel;  ///< current widget when search began
	QString m_content;                ///< content to search
	QString m_mruDir;                 ///< most recently used dir
	QString m_mruQuickDir;            ///< most recently used quick dir
	QStringList m_fileNames;          ///< filename wildcard patterns

	SortFilterProxyModel *proxyModel; ///< allows sorting tree view by size

	QRegExpValidator *sizeValidator;

	QAction *exitAct;
	QAction *layoutAct;
	QAction *logHistoryAct;
	QAction *newAct;
	QAction *newTabAct;
	QAction *openAct;
	QAction *stateAct;
	QAction *resultsBrowserAct;
	QAction *settingsAct;

	SearchCoordinator coordinator;
	SearchState* state;

	Sem sem;
	WordCount m_hitMap;

public slots:
	///<void open(const QModelIndex &index);
	///<void open();
	void showStatus(QAction *);
	void createResultsTab();
	void hitMapReady();
	void on_actionHorizontal_toggled(bool);
	void on_actionVertical_toggled(bool);
	void ResultAvailable(const Result& result);

private slots:
	void on_actionAdvanced_toggled(bool );
	void on_actionLogHistory_toggled(bool);
	void on_actionQuick_toggled(bool);
	void on_actionRestore_triggered();
	void on_actionResultsBrowser_toggled(bool);
	void on_actionSearch_Dialog_toggled(bool );
	void on_actionSettings_triggered();
	void on_browseToolButton_clicked();
	void on_cancelPushButton_clicked();
	void on_clearPushButton_clicked();
	void on_contentLineEdit_textChanged(QString);
	void on_dateTimeComboBox_currentIndexChanged(QString);
	void on_depthMaxSpinBox_valueChanged(int max);
	void on_depthMinSpinBox_valueChanged(int min);
	void on_directoryLineEdit_textChanged(QString);
	void on_dockWidget_visibilityChanged(bool visible);
	void on_emptyDirsCheckBox_stateChanged(int state);
	void on_fileLineEdit_textChanged(QString);
	void on_lineDisplayH_textChanged();
	void on_lineDisplayV_textChanged();
	void on_logHistoryDockWidget_visibilityChanged(bool visible);
	void on_quickBrowseToolButton_clicked();
	void on_quickCancelPushButton_clicked();
	void on_quickClearPushButton_clicked();
	void on_quickContentLineEdit_textChanged(QString);
	void on_quickFileLineEdit_textChanged(QString);
	void on_quickDirectoryLineEdit_textChanged(QString);
	void on_quickSearchPushButton_clicked();
	void on_resetAdvButton_clicked();
	void on_resultsBrowserDockWidget_visibilityChanged(bool visible);
	void on_resultsTabWidget_currentChanged(int index);
	void on_resultsTabWidget_tabCloseRequested(int index);
	void on_searchPushButton_clicked();
	void on_sizeComboBox_currentIndexChanged(QString option);
	void on_sizeLineEdit_textEdited(QString);
	void on_sizeLineEdit2_textEdited(QString);
	void on_syntaxComboBox_currentIndexChanged(int index);
	void on_tbarActionLayout_triggered();
	void on_tbarActionLogHistory_triggered();
	void on_tbarActionResultsBrowser_triggered();
	void on_tbarActionSettings_triggered();
	void on_tbarActionState_triggered();

	void about();
	void documentWasModified();
	void license();
	//void newFile();
	//bool save();
	//bool saveAs();
	void stop(bool);

protected:
	void closeEvent(QCloseEvent *event);
	void keyPressEvent(QKeyEvent *event);
};

static const bool FLAG = true,    ///< cancel find with a flag
						THROW = false;  ///< cancel find with exception
class Stop {};

class Hits {
public:
	fwc futureHits;
	fw  watcher;
};

class Tx : public QRunnable {  ///< smo- experimental thread class
	public:
		Tx() {
			pool = new QThreadPool();
			pool->setMaxThreadCount(QThread::idealThreadCount());
		}
		void run() { qDebug() << "Hello world."; }
		QThreadPool *pool;
};

#endif ///< MAINWINDOW_H
