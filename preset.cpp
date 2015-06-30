/******************************************************************************
 * NAME:       preset.cpp
 *
 * PURPOSE:    Provides methods and data for search type presets.  Exists as
 *             a data attribute of class Settings.
 *
 * DATE:       December 12, 2009
 *
 * REVISIONS:
 * ---------
 * [121209] shawno- initial coding.
 todo
 ----
 a allow a type for files with no extension; since require a regex cannot use
	QDir::entryList() which only handles globs; so we'd have to pull up all
	filenames and then filter by the regex for every desired extension PLUS
	the regex for files with no extension; could be expensive.  Anyway:
		^[^.\n]+\.?$ matches no-extension files excluding hidden files, and:
		^\.?[^.\n]+\.?$ matches same including hidden files.
 b add a button along with 'Set type' called 'Add exts' to append exts to
	an existing type?
 c make compact function to remove blank (overwritten) lines from config file?
*******************************************************************************/
#include "debug.h"

#include <string>

#include "preset.h"
#include "Utility.h"

using namespace std;
using std::string;

/******************************************************************************
 * Preset(): construct - store Settings ui pointer and initialize preset list.
 ******************************************************************************/
Preset::Preset  (Ui::Settings *sui) {
	m_sui = sui;
	init();
	initComboBox();
}
/******************************************************************************
 * Cancel(): cancel button clicked.
 ******************************************************************************/
void Preset::cancel  () {
	m_sui->targetComboBox->setCurrentIndex(0);  // blank
	m_sui->moreLineEdit->clear();

	// do not write any new type definitions to disk
}
/******************************************************************************
 * FilterSkip(): filter file list based on presets user wants to skip.
 ******************************************************************************/
QStringList Preset::filterSkip  (const QStringList &_files,
											const QDir &_currentDir) {
	QStringList files;
	int size = _files.size();
	for (int i = 0; i < size; ++i) {
		QFile file(_currentDir.absoluteFilePath(_files[i]));
		QFileInfo fi(file);
		if (!m_skipFilter.contains("*." + fi.suffix())) {
			files << _files[i];
		}
	}
	return files;
}
/******************************************************************************
 * GetKey(): grab the key from beginning of preset string: key (ext1 ext2 ...)
 ******************************************************************************/
QString Preset::getKey  (const QString & s) {
	int index = s.indexOf(" ");
	QString ret = s.left(index);
	qDebug() << "getKey() returning-" << ret;
	return ret;
}
/******************************************************************************
 * Init(): init preset string constants from header file and "./.smrc" file.
 ******************************************************************************/
void Preset::init  () {

	m_presets["Ccpp"] = typeCcpp;
	m_presets["Perl"] = typePerl;
	m_presets["Text"] = typeText;
	m_presets["Winx"] = typeWinx;
	//
	// Load from config file which takes precedence over above hardcodes.
	//
	QFile file(CONFIG_FILE);
	if (file.open(QIODevice::ReadOnly)) {
		QTextStream in(&file);
		QString line;

		while (!in.atEnd()  &&  !(line == "[Types]"))
			line = in.readLine().trimmed();

		while (!in.atEnd()) {
			line = in.readLine().trimmed();

			if (line.isEmpty()  ||  line[0] == '#')
				continue;

			QStringList parsed = line.split(QRegExp("\\s+")),
							value;

			QString key = parsed[0];

			int size = parsed.size();
			for (int i = 1; i < size; ++i)
					value << parsed[i];

			// qDebug() << "VALUE-" << value;

			if (!value.isEmpty())  // file may contain empty "Type" entries
				m_presets[key] = value;
		}

		file.close();

	}  // don't squawk if file does not exist - user's responsibility
}
/******************************************************************************
 * InitComboBox(): put presets - key (ext1 ext2 ...) - into preset combobox.
 ******************************************************************************/
void Preset::initComboBox  () {

	QMap<QString, QStringList>::const_iterator i = m_presets.constBegin();

	int index = 0;

	m_sui->targetComboBox->insertItem(index++, "");

	while (i != m_presets.constEnd()) {
		QString s = i.key() + " (";
		int size = i.value().size();

		for (int j = 0; j < size; j++)
			s += i.value()[j] + (j == size - 1 ? "" : " ");

		s += ")";

		m_sui->targetComboBox->insertItem(index, s);

		m_indexes[i.key()] = index;  // track type index positions

		++index;
		++i;
	}
}
/******************************************************************************
 * OkApply(): OK or Apply button clicked - write to the config file.
 ******************************************************************************/
 void Preset::okApply  () {

	QFile file(CONFIG_FILE);
	if (file.open(QIODevice::ReadWrite)) {

		qint64 top = positionAfterLine(file, "[Types]");

		QMap<QString, QStringList>::const_iterator i = m_newsets.constBegin();

		// while new user-defined types exist that have not been written to disk
		while (i != m_newsets.constEnd()) {

			// position file ptr to either start of type key or eof
			char buf[512];
			char keybuf[PRESET_RECLEN];
			while (!file.atEnd()) {
				memset(keybuf, '\0', sizeof(keybuf));

				file.peek(keybuf, sizeof(keybuf));
				QString s(keybuf);
				s = s.trimmed();

				QStringList sl = s.split(QRegExp("\\s+"));
				QString t = sl[0];

				qDebug() << "i.key()-" << i.key() << endl << "t-" << t;

				if (t == i.key())
					break;

				file.readLine(buf, sizeof(buf));
			}

			// ready to write at this file ptr position
			qint64 b = 0;
			int size = i.value().size();

			if (size) {
				b+=file.write(i.key().toStdString().c_str());
				b+=file.write(" ");
			}
			for (int j = 0; j < size; j++) {
				QString s = i.value()[j];
				b+=file.write(s.toStdString().c_str());
				b+=file.write((j == size - 1) ? "" : " ");
			}
			while (b < PRESET_RECLEN - 1)
				b+=file.write(" ");

			file.write("\n");

			file.seek(top);

			++i;
		}
		m_newsets.clear();
	}
	else
		boxError(QString(QObject::tr("Could not open configuration file '%1'."))
							 .arg(CONFIG_FILE));
	file.close();

}  // okApply()
/******************************************************************************
 * ProcessMore(): process arguments in the More combobox:
 *                [-also type] [-only type] [-skip type] [...]
 ******************************************************************************/
 int Preset::processMore  (const QStringList &_list) {

	if (_list.size() % 2)
		return -1;

	int size = _list.size();
	for (int i = 0; i < size; i += 2) {
		QString s = _list[i];  // the switch

		if (!m_presets.contains(_list[i + 1]))
			continue;  // already searching for this type

		QStringList sl = starDotted(m_presets[_list[i + 1]]);

		if (s == "-also")
			m_also += sl;
		else if (s == "-only")
			m_only += sl;
		else if (s == "-skip")
			m_skip += sl;
		else
			return -1;
	}
	return 0;
 }
/******************************************************************************
 * Reset(): reset Preset automatic variables.
 ******************************************************************************/
void Preset::reset  () {
	m_skipFilter.clear();
	m_also.clear();
	m_only.clear();
	m_skip.clear();
}
/******************************************************************************
 * Set(): based on all extensions indicated by the user, load up a string list
 *        corresponding to each type of usage - also, only, skip.
 *
 * Returns: -1 on usage, -2 if options cancel out, 0 otherwise.
 ******************************************************************************/
int Preset::set  (QStringList *patterns) {

	//
	// Process Filenames combobox.
	//
	m_also = *patterns;

	//
	// Process Target combobox.
	//
	QString key,
			  text = m_sui->targetComboBox->currentText();

	if (!text.isEmpty()) {

		key = getKey(text);

		QStringList value = starDotted(m_presets[key]);

		if (m_sui->alsoRadioButton->isChecked())
			m_also += value;
		else if (m_sui->onlyRadioButton->isChecked())
			m_only = value;
		else if (m_sui->skipRadioButton->isChecked())
			m_skip = value;
		else
			Q_ASSERT(0);
	}
	//
	// Process More combobox
	//
	QString s = m_sui->moreLineEdit->text();
	s = s.trimmed();
	QStringList list = s.split(QRegExp("\\s+"));
	if (!s.isEmpty()) {
		if (processMore(list) == -1) {
			boxError("Usage: [-also type] [-only type] [-skip type] "
						"[...]");

			return(-1);
		}
	}
	qDebug() << "also-" << m_also;
	qDebug() << "only-" << m_only;
	qDebug() << "skip-" << m_skip;

	//
	// Now that we have a string list for also, only and skip, use the following
	// logic to reduce these lists to a single list of patterns.
	//
	list.clear();
	list = m_only.size() ? m_only : m_also;  // only trumps also

	if (list.contains("*")) {  // search all files with a skip filter
		patterns->clear();
		*patterns << "*";
		m_skipFilter = m_skip;
		qDebug() << "set skip filter-" << m_skipFilter;
		return 0;
	}

	if (m_skip.size())
		list = listSubtract(list, m_skip);

	if (list.empty())  // user options cancelled out, nothing to do
		return -2;

	*patterns = list;

	 if (!validRx(*patterns, QRegExp::Wildcard)) {
		  boxError(QString(QObject::tr("Invalid Filename(s)")));
		  return -1;
	 }

	return 0;

}  // set()
/******************************************************************************
 * SetType(): set (add or override) a new/existing type.
 ******************************************************************************/
void Preset::setType  () {
	QString key = m_sui->typenameLineEdit->text();

	QString s = m_sui->extensionsLineEdit->text();
	QStringList value;
	s = s.trimmed();
	if (!s.isEmpty())
		value = s.split(QRegExp("\\s+"));

	m_newsets[key] = value;  // use to update config file

	if (value.isEmpty())
		m_presets.remove(key);
	else
		m_presets[key] = value;

	m_sui->targetComboBox->clear();
	initComboBox();

	// make this new type visible in combobox so user can see it
	m_sui->targetComboBox->setCurrentIndex(value.isEmpty() ? 0 : m_indexes[key]);

	m_sui->typenameLineEdit->clear();
	m_sui->extensionsLineEdit->clear();
}
/******************************************************************************
 * StarDotted(): prepend the "*." to selected preset patterns.
 ******************************************************************************/
QStringList Preset::starDotted  (const QStringList & _sl) {
	QStringList sl;
	for (int i = 0; i < _sl.size(); ++i)
		sl << ("*." + _sl[i]);
	return sl;
}
/******************************************************************************
 * Ui(): return stored pointer to the Settings ui.
 ******************************************************************************/
Ui::Settings *Preset::sui  () {
	return m_sui;
}
