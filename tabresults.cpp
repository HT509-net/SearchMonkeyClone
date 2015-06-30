/******************************************************************************
 * NAME:      tabresults.cpp
 *
 * PURPOSE:   Implementation of tabresults widget.
 *
 * DATE:      October 24, 2009
 *
 * REVISIONS:
 * ---------
 * [102409] adamc- initial coding
 * [021210] shawno- added tabbed searching capability (no context yet)
 todo
 ----
*******************************************************************************/
#include "debug.h"

#include <QtGui>

#include <QEventLoop>

#include "highlighter.h"
#include "mainwindow.h"
#include "sortproxy.h"
#include "tabresults.h"
#include "Types.h"
#include "Utility.h"

/******************************************************************************
 * TabResults(): create a new tab
 ******************************************************************************/
TabResults::TabResults(MainWindow *_mw, int _index, QWidget *_parent)
	:
	QWidget(_parent),
	mw(_mw),
	m_ui(new Ui::TabResults),
	m_gui(new GuiContext),
	m_state(_mw->getState()),
	m_idx(_index) {

	m_gui->setIndex(_index);

	m_state->init(_mw->getUi(), _mw->getSui());

	m_ui->setupUi(this);

	openAct = new QAction(tr("&Open..."), this);
	openAct->setShortcuts(QKeySequence::Open);
	openAct->setStatusTip(tr("Open existing file"));
	connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

	m_ui->resultsStackedWidget->setCurrentIndex(1);  // start in vertical layout

	// smo: pre-release shutdown of all broken parts
	m_ui->layoutPushButton->setVisible(false);

	proxyModel = new SortFilterProxyModel(this);
	proxyModel->setDynamicSortFilter(true);
	proxyModel->setSourceModel(createFilesModel(this));  // instantiates model

	smodel = new QItemSelectionModel(proxyModel);
	/*
	connect(smodel, SIGNAL(currentRowChanged(QModelIndex, QModelIndex)),
			  this, SLOT(displayLines(QModelIndex, QModelIndex)));
	connect(smodel, SIGNAL(currentChanged(QModelIndex, QModelIndex)),
			  this, SLOT(displayLines(QModelIndex, QModelIndex)));
	*/
	connect(smodel, SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
			  this, SLOT(displayLines(QItemSelection, QItemSelection)));

	initProxyView(m_ui->proxyViewH);          // horizontal
	m_ui->proxyViewH->setModel(proxyModel);
	m_ui->proxyViewH->setSelectionModel(smodel);
	m_ui->proxyViewH->setColumnWidth(0,250);  // path
	m_ui->proxyViewH->setColumnWidth(1,45);   // size
	m_ui->proxyViewH->setColumnWidth(2,30);   // matches
	m_ui->proxyViewH->setColumnWidth(3,20);   // type
	m_ui->proxyViewH->setColumnWidth(4,85);   // modified

	initProxyView(m_ui->proxyViewV);          // vertical
	m_ui->proxyViewV->setModel(proxyModel);
	m_ui->proxyViewV->setSelectionModel(smodel);
	m_ui->proxyViewV->setColumnWidth(0,500);  // path
	m_ui->proxyViewV->setColumnWidth(1,90);   // size
	m_ui->proxyViewV->setColumnWidth(2,60);   // matches
	m_ui->proxyViewV->setColumnWidth(3,40);   // type
	m_ui->proxyViewV->setColumnWidth(4,170);  // modified

	m_ui->lineDisplayH->setReadOnly(true);  // O.w. bug in setHtml() when
	m_ui->lineDisplayV->setReadOnly(true);  // switching stacked layout. (v3.0)

	connect(&m_coordinator, SIGNAL(OneSearchDone(const Result&)), this,
				SLOT(ResultAvailable(const Result&)), Qt::DirectConnection);

	/*if (!_path.isEmpty()) {
		model->setRowCount(0);
		m_coordinator.StartSearch(m_state->GatherSearchParameter(_mw->getUi(),
																					_mw->getSui()));
	}*/

	/* v3.0
	connect(m_ui->lineDisplayH->document(), SIGNAL(contentsChanged()), mw,
														 SLOT(documentWasModified()));
	connect(m_ui->lineDisplayV->document(), SIGNAL(contentsChanged()), mw,
														 SLOT(documentWasModified())); */
}
TabResults::~TabResults() {
    delete m_ui;
	 delete m_gui;
}
void TabResults::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
/******************************************************************************
 * CreateFilesModel(): Constructor helper.
 ******************************************************************************/
QAbstractItemModel *TabResults::createFilesModel  (QObject *_parent) {
	 model = new QStandardItemModel(0, 5, _parent);

	 model->setHeaderData(0, Qt::Horizontal, QObject::tr("File Name"));
	 model->setHeaderData(1, Qt::Horizontal, QObject::tr("Size"));
	 model->setHeaderData(2, Qt::Horizontal, QObject::tr("Matches"));
	 model->setHeaderData(3, Qt::Horizontal, QObject::tr("Type"));
	 model->setHeaderData(4, Qt::Horizontal, QObject::tr("Modified"));

	 return (model);
}
/******************************************************************************
 * DisplayLines(): Overload caught off the selection model.  [slot]
 ******************************************************************************/
void TabResults::displayLines  (const QItemSelection & _is,
										  const QItemSelection &) {
	if (_is.indexes().isEmpty())
		return;
	displayLines(_is.indexes()[0]);
}
/******************************************************************************
 * DisplayLines(): Overload caught off the selection model.  [slot]
 ******************************************************************************/
void TabResults::displayLines  (const QModelIndex &_index,
										  const QModelIndex &) {
	displayLines(_index);
}
/******************************************************************************
 * DisplayLines(): Single-click a listed file to display matched lines.  [slot]
	smo-consider only displaying until visible area filled, then somehow sus-
	pending until user scrolls down.  (Googlesque)
 ******************************************************************************/
void TabResults::displayLines  (const QModelIndex &_index) {

	Entex ee("TabResults::displayLines", 6);

	if (!smodel->hasSelection() || !_index.isValid()) {
		qDebug() << "returning on no selection or invalid index...";
		return;
	}

	m_state = mw->getState();
	QString content = m_state->GetContentKey(mw->getUi());
	qDebug() << "content-" << content;

	if (content.isEmpty())
		return;

	QString filepath;
	if (_index.column() == 0)
		filepath = _index.data().toString();
	else {  // ok to double-click on any column in the row
		QModelIndex index = _index.sibling(_index.row(), 0);
		filepath = index.data().toString();
	}
	QFile file(filepath);

	QTextEdit *lineDisplay = (layout() == HORIZONTAL) ? m_ui->lineDisplayH
																	  : m_ui->lineDisplayV;
	lineDisplay->clear();
	// Make sure line edit visible (user entered content after search done).
	lineDisplay->setVisible(true);

	Highlighter *highlighter = new Highlighter(lineDisplay->document(),
					 content, mw->getSyntax(), mw->caseSensitivity());
	highlighter->init();

	// Display the filepath but change the format of the highlighter in case
	// the filepath contains any content that would otherwise stand out.
	lineDisplay->setTextColor(COLOR_HYPERLINK);
	lineDisplay->setFontWeight(QFont::Bold);

	highlighter->changeFormat(QBrush(COLOR_HYPERLINK), QFont::Bold, filepath.length());
	lineDisplay->insertPlainText(filepath + "\n");

	lineDisplay->setFontWeight(QFont::Normal);
	lineDisplay->setTextColor(COLOR_TEXT);

	// The progress dialog trips this signal so shut if off for this method.
	disconnect(smodel, SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
					 this, SLOT(displayLines(QItemSelection, QItemSelection)));

	QProgressDialog progressDialog(this);
	progressDialog.setCancelButtonText(tr("&Cancel"));
	int rangeMax = file.size() / 1024;
	progressDialog.setRange(0, rangeMax);
	progressDialog.setWindowTitle(tr("Find a match"));
	progressDialog.setWindowOpacity(0.90);
	progressDialog.setMinimumDuration(2000);

	QRegExp rx(content);
	rx.setPatternSyntax(mw->getSyntax());
	rx.setCaseSensitivity(mw->caseSensitivity());
	const QRegExp crx(rx);  // performance tweak

	int hits = 0;
	if (file.open(QIODevice::ReadOnly)) {
		QString line;
		QTextStream in(&file);
		int lineno = 0,
			 progress = 0;
		while (!in.atEnd()) {
			++lineno;

			qApp->processEvents();

			line = in.readLine();

			progressDialog.setValue((progress += line.size())/1024);
			progressDialog.setLabelText(tr("Searching kilobyte %1 of %2...")
													 .arg(progress/1024).arg(rangeMax));
			if (progressDialog.wasCanceled())
				break;

			if (crx.indexIn(line) != -1) {
				++hits;
				lineDisplay->insertPlainText("\n");

				lineDisplay->setTextColor(COLOR_HYPERLINK);
				lineDisplay->setFontWeight(QFont::Bold);

				QString s = QString("%1>").arg(lineno);
				highlighter->changeFormat(QBrush(COLOR_HYPERLINK), QFont::Bold,
												  s.length());
				lineDisplay->insertPlainText(s);

				lineDisplay->setFontWeight(QFont::Normal);
				lineDisplay->setTextColor(COLOR_TEXT);

				// Despite our best efforts this will highlight the line number
				// making it stand out so changes were made in highlighter.cpp.
				highlighter->changeFormat(QBrush(COLOR_HIGHLIGHT), QFont::Bold, 0); //line.length(), s.length());
				lineDisplay->insertPlainText(line);
			}
		}
		QTextCursor cursor = lineDisplay->textCursor();
		cursor.movePosition(QTextCursor::Start);
		lineDisplay->setTextCursor(cursor);
	}
	else if (!mw->suiIgnoreFileErrs())
		boxError(QString(tr("Could not open- '%1'")).arg(filepath));

	// The progress dialog scrambles the current index and selection so reset
	// them on the target index.
	smodel->clearSelection();
	smodel->setCurrentIndex(_index, QItemSelectionModel::Rows |
											  QItemSelectionModel::Current);
	smodel->select(_index, QItemSelectionModel::Rows |
								  QItemSelectionModel::Select);

	connect(smodel, SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
				 this, SLOT(displayLines(QItemSelection, QItemSelection)));

	mw->normalLabel->setText(tr("%1 matched lines").arg(hits));
	mw->statusBar()->showMessage(tr("%1 matched lines").arg(hits));

}  // displayLines()
/******************************************************************************
 * Find(): initiate a search with search engine for this tab.
 ******************************************************************************/
void TabResults::find  () {
	Entex ee("TabResults::find", 0);
	qDebug() << ee.fn() << "searching on tab: " << m_idx;
	m_coordinator.StartSearch(m_state->GatherSearchParameter(mw->getUi(),
																				mw->getSui()));
	//QEventLoop loop(this);
	//loop.exec();

	qApp->processEvents();
}
/******************************************************************************
 * Init(): initialize a tab prior to searching with it.
 ******************************************************************************/
void TabResults::init  () {
	m_ui->lineDisplayH->clear();
	m_ui->lineDisplayV->clear();
	model->setRowCount(0);
}
/******************************************************************************
 * InitProxyView(): Initialize both horizontal and vertical proxy views.
 ******************************************************************************/
void TabResults::initProxyView  (QTreeView *proxyView) {
	proxyView->setRootIsDecorated(false);
	proxyView->setAlternatingRowColors(true);
	proxyView->setSelectionMode(QAbstractItemView::SingleSelection);
	proxyView->setSelectionBehavior(QAbstractItemView::SelectRows);
	proxyView->setUniformRowHeights(true);
	proxyView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	proxyView->setContextMenuPolicy(Qt::ActionsContextMenu);
	//_proxyView->addAction(cancelAct);
	//_proxyView->addAction(deleteAct);
	proxyView->addAction(openAct);

	// enable to allow double-clicking to open file listed in the view
	//connect(_proxyView, SIGNAL(activated(const QModelIndex &)), this,
	//                    SLOT(openFileOfItem(const QModelIndex &)));

	//connect(proxyView, SIGNAL(clicked(const QModelIndex &)), this,
	//						 SLOT(displayLines(const QModelIndex &)), Qt::DirectConnection);
}
/******************************************************************************
 * Open(): Use view context menu to open file.  [slot]
 ******************************************************************************/
void TabResults::open  () {
	open(layout() == HORIZONTAL ? m_ui->proxyViewH->currentIndex()
										 : m_ui->proxyViewV->currentIndex());
}
/******************************************************************************
 * Open(): Double-click a listed file to open it.
 smo consider +QWebWidget to embed live pages in app (Adam has examples in svn)
 ******************************************************************************/
void TabResults::open  (const QModelIndex &_index) {
	// makes it ok to double-click on any column in the row
	QModelIndex index = (_index.column() == 0) ? _index
															 : _index.sibling(_index.row(), 0);
	(void)_index;

	QString path = index.data().toString();
	if (IsValidDir(path))
		QFileDialog::getOpenFileName(this, tr("Browse Directory"), path,
											  tr("All Files (*)"));
	else
		QDesktopServices::openUrl(path);

	mw->update();
	mw->repaint();
}
/******************************************************************************
 * LayoutPushButtonSetLabel(): set the label on this push button.
 ******************************************************************************/
void TabResults::layoutPushButtonSetLabel  () {
		m_ui->layoutPushButton->setText(mw->getUi()->actionHorizontal->isChecked()
																 ? "&Vertical" : "&Horizontal");
}
/******************************************************************************
 * ResultAvailable(): receive result data signal from search engine [slot].
 ******************************************************************************/
void TabResults::ResultAvailable  (const Result& result)
{
	Entex ee("TabResults::ResultAvailable", 4);
	qd(4) << ee.fn() << "adding line to view...";

	mw->addFile(
		model,
		result->matchedFile.absoluteFilePath(),
		bytes2units(result->matchedFile.size()),
		result->matchedLines.size(),
		result->matchedFile.suffix(),
		result->matchedFile.lastModified().toString("MM.dd.yy hh:mm:ss")
	);
	qApp->processEvents();
}
/******************************************************************************
 * Function group [slots]: headers generated by QtDesigner.
 ******************************************************************************/
void TabResults::on_layoutPushButton_clicked  () {
	if (m_ui->layoutPushButton->text() == "&Horizontal") {
		m_ui->layoutPushButton->setText("&Vertical");
		mw->getLayoutAct()->setText("&Vertical");
		mw->getUi()->actionHorizontal->setChecked(true);
	}
	else {
		m_ui->layoutPushButton->setText("&Horizontal");
		mw->getLayoutAct()->setText("&Horizontal");
		mw->getUi()->actionVertical->setChecked(true);
	}
}
/*
void TabResults::on_proxyViewH_clicked(QModelIndex index) {
	if (!mw->modeEmptyDirs()) {
		displayLines(index);
	}
}
void TabResults::on_proxyViewV_clicked(QModelIndex index)
{
	//m_coordinator.StartDisplay(this, index);
	if (!mw->modeEmptyDirs()) {
		displayLines(index);
	}
}
*/
void TabResults::hello() {
	qd(6) << "HELLO WORLD!";
}
