/******************************************************************************
 * NAME:       preset.h
 *
 * PURPOSE:    Defines class Preset (a data attribute of class Settings).
 *
 * DATE:       December 12, 2009
 *
 * REVISIONS:
 * ---------
 * [121209] shawno- initial coding.
*******************************************************************************/
#ifndef PRESET_H
#define PRESET_H

#include <QDir>
#include <QMap>
#include <QStringList>

#include "settings.h"
#include "ui_settings.h"

typedef QMap<QString, QStringList> Presets;
typedef QMap<QString, int> Indexes;

const QStringList typeCcpp = (QStringList()
<< "c" << "cpp" << "h");
const QStringList typePerl = (QStringList()
<< "pl" << "pm" << "pod" << "t");
const QStringList typeText = (QStringList()  // smo perl -T
<< "asc" << "log" << "txt" << "zw" << "ini");
const QStringList typeWinx = (QStringList()
<< "bat" << "com" << "exe");

class Preset {
public:
	Preset(Ui::Settings *sui);
	virtual ~Preset() {}
	void cancel();
	QStringList filterSkip(const QStringList &_files, const QDir &_currentDir);
	QString getKey(const QString & text);
	void init();
	void initComboBox();
	void okApply();
	int processMore(const QStringList &_list);
	void reset();
	int set(QStringList *patterns);
	void setType();
	inline int skipFilterSz() const { return m_skipFilter.size(); }
	QStringList starDotted(const QStringList & _sl);
	inline QString target() const { return m_sui->targetComboBox->currentText();}
	Ui::Settings *sui();

private:
	Indexes m_indexes;
	Presets m_newsets,
			  m_presets;
	QStringList m_also,
					m_only,
					m_skip,
					m_skipFilter;
	Ui::Settings *m_sui;
};

#endif // PRESET_H
