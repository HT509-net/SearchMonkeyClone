/******************************************************************************
 * NAME:      settings.cpp
 *
 * PURPOSE:   Implementation of settings widget.
 *
 * DATE:      December 7, 2009
 *
 * REVISIONS:
 * ---------
 * [120709] adamc- initial coding
 * [121509] shawno- added m_ui allowing access to ui pointer
 * [122509] shawno- addes slots for Presets
 todo
 ----
 a auto-inits in ctor
 b create config file on first run
*******************************************************************************/
#include "debug.h"

#include <QListWidget>
#include <QStatusBar>

#include "dir.h"
#include "preset.h"
#include "settings.h"
#include "ui_settings.h"
#include "Utility.h"

Settings::Settings(QWidget *parent) :
	QDialog(parent),
	m_ui(new Ui::Settings) {

	m_ui->setupUi(this);
	{
	//if (!QFile::exists(CONFIG_FILE))
		//createConfigFile();

	m_dir = new Dir(m_ui);
	}
	m_preset = new Preset(m_ui);

	m_ui->excludeListWidget->setSortingEnabled(true);
	m_ui->includeListWidget->setSortingEnabled(true);

	m_general = 0U;
}
Settings::~Settings() {
	delete m_ui;
}
void Settings::changeEvent(QEvent *e) {
	switch (e->type()) {
	case QEvent::LanguageChange:
		m_ui->retranslateUi(this);
		break;
	default:
		break;
    }
}
void Settings::createConfigFile() {
	QFile file(CONFIG_FILE);

	if (file.open(QIODevice::WriteOnly)) {
		QTextStream out(&file);

		out << "# comments on separate lines" << endl;
		out << "#" << endl;
		out << "# [Directories]" << endl;
		out << "# INCLUDE=includeDirName1 includeDirName2 ..." << endl;
		out << "# EXCLUDE=excludeDirName1 excludeDirName2 ..." << endl;
		out << "# SEARCHALL=0 or 1" << endl;
		out << "#" << endl;
		out << "# [Types]" << endl;
		out << "# type1 ext1 ext2 ext3 ..." << endl;
		out << "# type2 ext4 ext5 ext6 ..." << endl;
		out << "#..." << endl;
		out << "#" << endl;
		out << "# No slashes allowed in directory names." << endl;
		out << "# No periods allowed in extensions." << endl;
		out << "#" << endl;
		out << "# First 2 \"records\" must be space-padded out to 240 chars, "
				 "3rd to 80 chars." << endl;
		out << "[Directories]" << endl;
		out << "INCLUDE=";
		int pad = DIRNAM_RECLEN - QString("INCLUDE").size();
		for (int i = 0; i < pad; ++i)
			out << " ";
		out << endl;


		QString exclude = "EXCLUDE=";
		int size = EXCLUDES_DFLT.size();
		for (int i = 0; i < size; ++i) {
			exclude += EXCLUDES_DFLT[i];
			exclude += (i == size - 1) ? "" : " ";
		}
		out << exclude;
		pad = DIRNAM_RECLEN - QString(exclude).size();
		for (int i = 0; i < pad; ++i)
			out << " ";
		out << endl;

		out << "SEARCHALL=0";
		pad = 80 - QString("SEARCHALL=0").size() - 1;
		for (int i = 0; i < pad; ++i)
			out << " ";
		out << endl;
		out << "# These \"records\" must be space-padded out to 80 chars." <<endl;
		out << "[Types]" << endl;
	}
	else {
		boxError(QString(tr("Could not open configuration file '%1'."))
							 .arg(CONFIG_FILE));
		return;
	}

	file.close();
}
void Settings::on_generalButtonBox_clicked(QAbstractButton* button) {
	if (button->text() == "OK") {
		this->hide();
	}
	else if (button->text() == "Cancel") {
		m_ui->tabCheckBox->setChecked(m_general & GEN_TAB);
		m_ui->ignoreCheckBox->setChecked(m_general & GEN_IGNORE);
		m_ui->cStyleCheckBox->setChecked(m_general & GEN_CSTYLE);
		this->hide();
	}
	else
		Q_ASSERT(0);

	m_general = 0U;
}
void Settings::reset() {
	m_dir->reset();
	m_preset->reset();
}
void Settings::saveInCaseCancel() {
	if (m_ui->tabCheckBox->isChecked())
		m_general |= GEN_TAB;
	if (m_ui->ignoreCheckBox->isChecked())
		m_general |= GEN_IGNORE;
	if (m_ui->cStyleCheckBox->isChecked())
		m_general |= GEN_CSTYLE;
}
Ui::Settings *Settings::ui  () {
	return m_ui;
}
//
// Class Dir slots.
//
void Settings::on_addPushButton_clicked() {
	m_dir->add(this);
}
void Settings::on_defaultPushButton_clicked() {
	m_ui->searchAllDirsCheckBox->setChecked(false);
	m_dir->reset();
	m_ui->excludeListWidget->clear();
	m_ui->includeListWidget->clear();
	list2ListWidget(EXCLUDES_DFLT, m_ui->excludeListWidget);
}
void Settings::on_deletePushButton_clicked() {

	QListWidget *e = m_ui->excludeListWidget;  // shortcut

	if (e->currentItem() && !e->currentItem()->isSelected())
		return;  // may only be selection rectangle

	QListWidgetItem *item = e->takeItem(e->row(e->currentItem()));

	delete item;
}
void Settings::on_directoriesButtonBox_clicked(QAbstractButton* button) {
	if (button->text() == "OK") {
		m_dir->okApply();
		this->hide();
	}
	else if (button->text() == "Apply") {
		m_ui->directoriesButtonBox->button(QDialogButtonBox::Apply)
										  ->setEnabled(false);
		m_dir->okApply();
	}
	else if (button->text() == "Cancel") {
		m_dir->cancel();
		this->hide();
	}
	else
		Q_ASSERT(0);
}
void Settings::on_leftAllPushButton_clicked() {
	m_dir->moveAllItems(m_ui->excludeListWidget, m_ui->includeListWidget);
}
void Settings::on_leftPushButton_clicked() {
	m_dir->moveSelectedItem(m_ui->excludeListWidget, m_ui->includeListWidget);
}
void Settings::on_rightAllPushButton_clicked() {
	m_dir->moveAllItems(m_ui->includeListWidget, m_ui->excludeListWidget);
}
void Settings::on_rightPushButton_clicked() {
	m_dir->moveSelectedItem(m_ui->includeListWidget, m_ui->excludeListWidget);
}
void Settings::on_searchAllDirsCheckBox_toggled(bool toggle) {
	m_dir->setSearchAll(toggle);
	m_ui->excludeListWidget->setDisabled(toggle);
	m_ui->includeListWidget->setDisabled(toggle);
	m_ui->addPushButton->setDisabled(toggle);
	m_ui->deletePushButton->setDisabled(toggle);
	m_ui->leftPushButton->setDisabled(toggle);
	m_ui->rightPushButton->setDisabled(toggle);
	m_ui->leftAllPushButton->setDisabled(toggle);
	m_ui->rightAllPushButton->setDisabled(toggle);
}
//
// Class Preset slots.
//
void Settings::on_buttonBox_clicked(QAbstractButton* button) {
	if (button->text() == "OK") {
		m_preset->okApply();
	}
	else if (button->text() == "Apply") {
		m_ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(false);
		m_preset->okApply();
	}
	else if (button->text() == "Cancel") {
		m_preset->cancel();
	}
	else
		Q_ASSERT(0);
}
void Settings::on_setTypePushButton_clicked() {
	m_ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(true);
	m_preset->setType();
}
void Settings::on_moreLineEdit_textEdited(QString)  {
	QStatusBar sb(this);
	sb.setVisible(true);
	sb.raise();
	sb.showMessage("Usage: [-also type] [-only type] [-skip type] [...]");

	this->setStatusTip("Usage: [-also type] [-only type] [-skip type] [...]");
}
