/******************************************************************************
 * NAME:      guicontext.h
 *
 * PURPOSE:   Class definition for GUI Context class.
 *
 * DATE:      February 12, 2010
 *
 * REVISIONS:
 * ---------
 * [021210] shawno- initial coding
 smo
 ---
 a button states -- search running?
 b layout state
 c quick/adv
 d visible dock widgets?
 e settings!!!
*******************************************************************************/
#ifndef GUICONTEXT_H
#define GUICONTEXT_H

#include <QDateTime>
#include <QStringList>

#include "SearchParameter.h"
#include "SearchState.h"
#include "settings.h"
#include "Types.h"
#include "ui_settings.h"

class GuiContext {

	friend class MainWindow;

public:
	GuiContext() { m_general = 0U; }
	virtual ~GuiContext();
	void burnContext(Ui::MainWindow *ui, Ui::Settings *sui);
	inline int getIndex() { return m_index; }
	void ripContext (Ui::MainWindow *ui, Ui::Settings *sui);
	void setIndex(int index) { m_index = index; }

private:
	bool m_binary;
	bool m_dateTimeEnabled;
	bool m_dateTime2Enabled;
	bool m_emptyDirs;
	bool m_invert;
	bool m_searchAll;
	bool m_sizeEnabled;
	bool m_size2Enabled;
	bool m_sizeUnitsEnabled;
	bool m_sizeUnits2Enabled;
	bool m_skipHidden;
	bool m_subdir;
	bool m_symLink;
	bool m_visLogHistory;
	bool m_visResultsBrowser;
	bool m_visSearchDialog;
	int m_dateTimeIdx;
	int m_depthMax;
	int m_depthMin;
	int m_replaceIdx;
	int m_sizeIdx;
	int m_sizeUnitsIdx;
	int m_sizeUnits2Idx;
	int m_syntaxIdx;
	int m_targetIdx;
	Case m_kase;                // enum
	Layout m_layout;            // enum
	State m_state;              // enum
	TargetRadio m_targetRadio;  // enum
	quint32 m_general;
	QDateTime m_dateTime;
	QDateTime m_dateTime2;
	QString m_content;
	QString m_files;
	QString m_folder;
	QString m_more;
	QString m_replace;
	QString m_size;
	QString m_size2;
	QStringList m_lft;
	QStringList m_rght;

	int m_index;  // debug - index into tab results array
};

#endif // GUICONTEXT_H
