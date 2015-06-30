/******************************************************************************
 * NAME:      tabresults.h
 *
 * PURPOSE:   Class definition for tabresults widget.
 *
 * DATE:      October 24, 2009
 *
 * REVISIONS:
 * ---------
 * [102409] adamc- initial coding
 * [021210] shawno- added tabbed searching capability (no context yet)
*******************************************************************************/
#ifndef TABRESULTS_H
#define TABRESULTS_H

#include <QWidget>

#include "guicontext.h"
#include "SearchCoordinator.h"
#include "ui_tabresults.h"

QT_BEGIN_NAMESPACE
class QAbstractItemModel;
class QModelIndex;
class QStandardItemModel;
class QTreeView;
QT_END_NAMESPACE

class MainWindow;
class SearchState;
class SortFilterProxyModel;

namespace Ui {
	class TabResults;
}

class TabResults : public QWidget {
	Q_OBJECT
public:
	TabResults(MainWindow *mw, int index = -1, QWidget *parent = 0);
	virtual ~TabResults();
	QAbstractItemModel *createFilesModel(QObject *parent);
	void find();
	inline QStandardItemModel *getModelPtr() const { return model; }
	inline GuiContext *gui() const { return m_gui; }
	void init();
	void initProxyView(QTreeView* proxyView);
	inline bool isSearching() const { return m_coordinator.IsSearching(); }
	inline Layout layout() const { return (m_ui->resultsStackedWidget->currentIndex() == HORIZONTAL) ? HORIZONTAL : VERTICAL; }
	void layoutPushButtonSetLabel();
	void open(const QModelIndex &_index);
	inline Ui::TabResults *tui() const { return m_ui; }
	void hello();

protected:
	void changeEvent(QEvent *e);

private:
	QAction *openAct;                  // right click to open a file
	QItemSelectionModel *smodel;       // gets sigs off the tree view
	QStandardItemModel *model;         // data struct for this tab
	SortFilterProxyModel *proxyModel;  // allows custom sorting
	MainWindow *mw;                    // ptr to the main window app
	Ui::TabResults *m_ui;              // ptr to this gui widget
	GuiContext *m_gui;                 // ptr to gui context object
	SearchCoordinator m_coordinator;   // search engine coordinator
	SearchState* m_state;              // tracks the current gui state
	int m_idx;                         // index into main window array

public slots:
	void displayLines(const QModelIndex &index, const QModelIndex &);
	void displayLines(const QModelIndex &index);
	void displayLines(const QItemSelection &, const QItemSelection &);
	void open();
	void ResultAvailable(const Result& result);

private slots:
	void on_layoutPushButton_clicked();
	//void on_proxyViewH_clicked(QModelIndex index);
	//void on_proxyViewV_clicked(QModelIndex index);
};

#endif // TABRESULTS_H
