/******************************************************************************
 * NAME:       dir.cpp
 *
 * PURPOSE:    Provides methods and data for directories to be excluded. Exists
 *             as a data attribute of class Settings.
 *
 * DATE:       Jauary 2, 2010
 *
 * REVISIONS:
 * ---------
 * [010210] shawno- initial coding.
 smo
 ---
 a mem to test reactivation of apply button
*******************************************************************************/
#include "debug.h"

#include <QFile>
#include <QInputDialog>
#include <QStringList>
#include <QListWidgetItem>

#include "dir.h"
#include "ui_settings.h"
#include "Utility.h"

/******************************************************************************
 * Dir(): construct - store Settings ui pointer and initialize exclude list.
 ******************************************************************************/
Dir::Dir  (Ui::Settings *sui) {
	m_searchAll = false;
	m_sui = sui;
	reset();
	init();
	initListWidget();
}
/******************************************************************************
 * Add(): add a directory name to the exclude list.
 ******************************************************************************/
void Dir::add  (Settings *settings) {
	bool ok;
	QString s =
	QInputDialog::getText(settings, QObject::tr("Enter a new directory name"),
										 QObject::tr("Directory name:"),QLineEdit::Normal,
										 QString(), &ok);
	//
	// Add new item as specified to exclude list as long as it is in neither the
	// exclude list nor the include list.
	//
	if (ok && !s.isEmpty()
			 && !listContains(m_sui->excludeListWidget, s)
			 && !listContains(m_sui->includeListWidget, s)) {
		QListWidgetItem *newItem = new QListWidgetItem;
		newItem->setText(s);
		m_sui->excludeListWidget->insertItem(0, newItem);
	}
}
/******************************************************************************
 * Cancel(): cancel button clicked.
 ******************************************************************************/
void Dir::cancel  () {
	m_sui->searchAllDirsCheckBox->setChecked(m_searchAll);
	m_sui->includeListWidget->clear();
	m_sui->excludeListWidget->clear();
	list2ListWidget(m_saveLeft, m_sui->includeListWidget);
	list2ListWidget(m_saveRight, m_sui->excludeListWidget);
}
/******************************************************************************
 * FilterDir(): Determine if a dir is in the exclude list.
 ******************************************************************************/
bool Dir::filterDir  (const QString &dir) {
	return m_right.contains(dir);
}
/******************************************************************************
 * FilterDirs(): filter directory list based on excludes user wants to skip.
 ******************************************************************************/
QStringList Dir::filterDirs  (const QStringList &_dirs) {
	return listSubtract(_dirs, m_right);
}
/******************************************************************************
 * Init(): init preset string constants from header file or "./.smrc" file.
 ******************************************************************************/
void Dir::init  () {
	QFile file(CONFIG_FILE);

	if (file.open(QIODevice::ReadOnly)) {

		qDebug() << "opened config file for reading...";
		positionAfterLine(file, "[Directories]");

		//
		// Process included directory names.
		//
		QString line = getLine(file);
		line.remove("INCLUDE=");
		line = line.trimmed();
		m_left = line.split(QRegExp("\\s+"));
		if (!m_left.isEmpty()  &&  m_left[0] == "")
			m_left.clear();  // it split on all whitespace - ""
		qDebug() << "init(left)-" << m_left;

		//
		// Process excluded directory names.
		//
		line = getLine(file);
		line.remove("EXCLUDE=");
		line = line.trimmed();
		m_right = line.split(QRegExp("\\s+"));

		if (!m_right.isEmpty() && m_right[0] == "")
			m_right.clear();  // it split on all whitespace - ""

		qDebug() << "init(right)-" << m_right;

		//
		// Process search all flag.
		//
		line = getLine(file);
		line.remove("SEARCHALL=");
		line = line.trimmed();
		m_searchAll = (line == "0") ? false : true;

		qDebug() << "init(searchAll)-" << m_searchAll;

		file.close();

	}  // don't squawk if file does not exist - user's responsibility

}  // init()
/******************************************************************************
 * InitListWidget(): put any configured values into list widget.
 ******************************************************************************/
void Dir::initListWidget  () {
	list2ListWidget(m_right, m_sui->excludeListWidget);
	list2ListWidget(m_left, m_sui->includeListWidget);
	m_sui->searchAllDirsCheckBox->setChecked(m_searchAll);
}
/******************************************************************************
 * List2File(): slam list into config file as a "record".
 ******************************************************************************/
void Dir::list2File  (QFile &_file, const QStringList & _list,
												const QString &_tag) {
	qint64 b = 0;
	b+=_file.write(_tag.toStdString().c_str());
	int size = _list.size();
	for (int i = 0; i < size; ++i) {
		b+=_file.write(_list[i].toStdString().c_str());
		if (i != size - 1)
			b+=_file.write(" ");
	}
	while (b < DIRNAM_RECLEN - 1)  // pad the "record"
		b+=_file.write(" ");
	_file.write("\n");
}
/******************************************************************************
 * ListContains(): does list contain specified item specified by given string?
 ******************************************************************************/
bool Dir::listContains(const QListWidget *_lw, const QString &_s) {
	int cnt = _lw->count();
	if (cnt < 1)
		return false;

	for (int i = 0; i < cnt; ++i) {
		QListWidgetItem * item = _lw->item(i);
		QString data = (item->data(Qt::DisplayRole)).toString();
		if (data == _s)
			return true;
	}
	return false;
}
/******************************************************************************
 * MoveSelectedItem(): move selected item from one list widget to another.
 ******************************************************************************/
void Dir::moveSelectedItem  (QListWidget *src, QListWidget *dst) {
	if (src->count() < 1)
		return;

	if (src->currentItem() && !src->currentItem()->isSelected())
		return;  // may only be selection rectangle

	QListWidgetItem *item = src->takeItem(src->row(src->currentItem()));

	dst->insertItem(0, item);

	m_sui->directoriesButtonBox->button(QDialogButtonBox::Apply)
										->setEnabled(true);
}
/******************************************************************************
 * MoveAllItems(): move all items from one list widget to another.
 ******************************************************************************/
void Dir::moveAllItems  (QListWidget *src, QListWidget *dst) {
	int count = src->count();
	for (int i = 0; i < count; ++i)
		dst->insertItem(0, src->takeItem(src->row(src->item(0))));
	m_sui->directoriesButtonBox->button(QDialogButtonBox::Apply)
										->setEnabled(true);
}
/******************************************************************************
 * OkApply(): OK or Apply button clicked.
 ******************************************************************************/
 void Dir::okApply  () {

	m_searchAll = m_sui->searchAllDirsCheckBox->isChecked();

	QFile file(CONFIG_FILE);

	if (!file.open(QIODevice::ReadWrite)) {
		boxError(QString(QObject::tr("Could not open configuration file '%1'."))
							 .arg(CONFIG_FILE));
		return;
	}

	listWidget2List(m_sui->excludeListWidget, &m_right);
	listWidget2List(m_sui->includeListWidget, &m_left);

	positionAfterLine(file, "[Directories]");

	list2File(file, m_left, "INCLUDE=");
	list2File(file, m_right, "EXCLUDE=");

	int b = 0;
	b+=file.write("SEARCHALL=");
	b+=file.write(m_searchAll ? "1" : "0");
	for (int i = 0; i < 80 - b - 1; ++i)
		file.write(" ");
	file.write("\n");

	file.close();
}
/******************************************************************************
 * Reset(): reset Dir automatic variables.
 ******************************************************************************/
void Dir::reset  () {
	m_left.clear();
	m_right.clear();
	m_saveLeft.clear();
	m_saveRight.clear();
}
/******************************************************************************
 * SaveInCaseCancel(): save initial state in case user cancels.
 ******************************************************************************/
void Dir::saveInCaseCancel  () {
	m_searchAll = m_sui->searchAllDirsCheckBox->isChecked();
	listWidget2List(m_sui->excludeListWidget, &m_saveRight);
	listWidget2List(m_sui->includeListWidget, &m_saveLeft);
}
/******************************************************************************
 * SetDirFilter(): Set directory filter list based on user-defined excludes.
 ******************************************************************************/
void Dir::setDirFilter  () {
	if (m_searchAll) {
		m_right.clear();
		return;
	}
	listWidget2List(m_sui->excludeListWidget, &m_right);
	qDebug() << "setDirFilter()-" << m_right;
}
/******************************************************************************
 * Sui(): return stored pointer to the Settings ui.
 ******************************************************************************/
Ui::Settings *Dir::sui  () {
	return m_sui;
}
