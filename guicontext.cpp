/******************************************************************************
 * NAME:      tabresults.cpp
 *
 * PURPOSE:   Implementation of GUI Context class i.e. rips all GUI settings.
 *
 * DATE:      February 12, 2010
 *
 * REVISIONS:
 * ---------
 * [021210] shawno- initial coding
*******************************************************************************/
#include "debug.h"

#include "guicontext.h"
#include "mainwindow.h"
#include "Utility.h"

GuiContext::~GuiContext  () {
  if (!m_lft.empty())
    m_lft.clear();
  if (!m_rght.empty())
    m_rght.clear();
}
/******************************************************************************
 * BurnContext(): burn all the gui settings for the current tab.
 ******************************************************************************/
void GuiContext::burnContext  (Ui::MainWindow *ui, Ui::Settings *sui) {

  qd(5) << "**********burning object-" << this << " (" << getIndex() << ")";

  qd(5) << "setting checked with m_state-" << m_state;
  ui->actionQuick->setChecked(m_state == QUICK);
  ui->actionAdvanced->setChecked(m_state == ADVANCED);

  qd(5) << "burning m_folder-" << m_folder << " m_files-" << m_files << " m_content-" << m_content;
  if (m_state == ADVANCED) {
    ui->directoryLineEdit->setText(m_folder);
    ui->fileLineEdit->setText(m_files);
    ui->contentLineEdit->setText(m_content);
  }
  else {
    qd(5) << "burning for quick state...";
    ui->quickDirectoryLineEdit->setText(m_folder);
    ui->quickFileLineEdit->setText(m_files);
    ui->quickContentLineEdit->setText(m_content);
  }
  qd(5) << "burn complete...";

  ui->actionLogHistory->setChecked(m_visLogHistory);
  ui->actionResultsBrowser->setChecked(m_visResultsBrowser);
  ui->actionSearchDialog->setChecked(m_visSearchDialog);

  qd(5) << "burning m_layout(" << m_layout << ")" << ((m_layout == HORIZONTAL) ? "horizontal" : "vertical");
  if (m_layout == HORIZONTAL)
    ui->actionHorizontal->setChecked(true);
  else
    ui->actionVertical->setChecked(true);

  //ui->replaceLineEdit->setText(m_replace);
  //ui->replaceComboBox->setCurrentIndex(m_replaceIdx);
  //ui->syntaxComboBox->setCurrentIndex(m_syntaxIdx);
  ui->sizeComboBox->setCurrentIndex(m_sizeIdx);

  ui->caseCheckBox->setChecked(m_kase == SENSITIVE);

  ui->subdirCheckBox->setChecked(m_subdir);
  //ui->binaryCheckBox->setChecked(m_binary);
  //ui->emptyDirsCheckBox->setChecked(m_emptyDirs);
  ui->symLinkCheckBox->setChecked(m_symLink);
  ui->skipHiddenCheckBox->setChecked(m_skipHidden);
  //ui->invertCheckBox->setChecked(m_invert);

  //ui->depthMaxSpinBox->setValue(m_depthMax);
  //ui->depthMinSpinBox->setValue(m_depthMin);

  if (m_sizeEnabled) {
    ui->sizeLineEdit->setEnabled(true);
    ui->sizeLineEdit->setText(m_size);
  }
  if (m_size2Enabled) {
    ui->sizeLineEdit2->setEnabled(true);
    ui->sizeLineEdit2->setText(m_size2);
  }
  if (m_sizeUnitsEnabled) {
    ui->sizeUnitsComboBox->setEnabled(true);
    ui->sizeUnitsComboBox->setCurrentIndex(m_sizeUnitsIdx);
  }
  if (m_sizeUnits2Enabled) {
    ui->sizeUnitsComboBox2->setEnabled(true);
    ui->sizeUnitsComboBox2->setCurrentIndex(m_sizeUnits2Idx);
  }
  ui->dateTimeComboBox->setCurrentIndex(m_dateTimeIdx);
  if (m_dateTimeEnabled) {
    ui->dateTimeEdit->setEnabled(true);
    ui->dateTimeEdit->setDateTime(m_dateTime);
  }
  if (m_dateTime2Enabled) {
    ui->dateTimeEdit2->setEnabled(true);
    ui->dateTimeEdit2->setDateTime(m_dateTime2);
  }
  sui->searchAllDirsCheckBox->setChecked(m_searchAll);

  sui->alsoRadioButton->setChecked(m_targetRadio == ALSO);
  sui->onlyRadioButton->setChecked(m_targetRadio == ONLY);
  sui->skipRadioButton->setChecked(m_targetRadio == SKIP);

  sui->tabCheckBox->setChecked(m_general & GEN_TAB);
  sui->ignoreCheckBox->setChecked(m_general & GEN_IGNORE);
  sui->cStyleCheckBox->setChecked(m_general & GEN_CSTYLE);

  sui->moreLineEdit->setText(m_more);
  sui->targetComboBox->setCurrentIndex(m_targetIdx);

  list2ListWidget(m_lft, sui->includeListWidget);
  list2ListWidget(m_rght, sui->excludeListWidget);
}
/******************************************************************************
 * RipContext(): rip all the gui settings for the previous tab.
 ******************************************************************************/
void GuiContext::ripContext  (Ui::MainWindow *ui, Ui::Settings *sui) {

  qd(5) << "**********ripping object-" << this << " (" << getIndex() << ")";

  m_state = ui->actionQuick->isChecked() ? QUICK : ADVANCED;  // rip first

  qd(5) << "ripped m_state-" << m_state;

  m_files = (m_state == QUICK) ? ui->quickFileLineEdit->text()
                      : ui->fileLineEdit->text();
  qd(5) << "quickFile-" << ui->quickFileLineEdit->text() << " advFile-" << ui->fileLineEdit->text();
  qd(5) << "m_files-" << m_files;
  m_folder = (m_state == QUICK) ? ui->quickDirectoryLineEdit->text()
                      : ui->directoryLineEdit->text();
  m_content = (m_state == QUICK) ? ui->quickContentLineEdit->text()
                       : ui->contentLineEdit->text();
  qd(5) << "ripped: m_files-" << m_files << " m_folder-" << m_folder << " m_content-" << m_content;

  m_visLogHistory = ui->actionLogHistory->isChecked();
  m_visResultsBrowser = ui->actionResultsBrowser->isChecked();
  m_visSearchDialog = ui->actionSearchDialog->isChecked();

  m_layout = ui->actionHorizontal->isChecked() ? HORIZONTAL : VERTICAL;
  qd(5) << "ripped m_layout(" << m_layout << ")" << ((m_layout == HORIZONTAL) ? "horizontal" : "vertical");

  //m_replace = ui->replaceLineEdit->text();
  //m_replaceIdx = ui->replaceComboBox->currentIndex();
  //m_syntaxIdx = ui->syntaxComboBox->currentIndex();
  m_sizeIdx = ui->sizeComboBox->currentIndex();

  m_kase = ui->caseCheckBox->isChecked() ? SENSITIVE : INSENSITIVE;
  m_subdir = ui->subdirCheckBox->isChecked();
  //m_binary = ui->binaryCheckBox->isChecked();
  //m_invert = ui->invertCheckBox->isChecked();
  m_skipHidden = ui->skipHiddenCheckBox->isChecked();
  m_symLink = ui->symLinkCheckBox->isChecked();
  //m_emptyDirs = ui->emptyDirsCheckBox->isChecked();

  //m_depthMax = ui->depthMaxSpinBox->value();
  //m_depthMin = ui->depthMinSpinBox->value();

  if ((m_sizeEnabled = ui->sizeLineEdit->isEnabled()))
    m_size = ui->sizeLineEdit->text();
  if ((m_size2Enabled = ui->sizeLineEdit2->isEnabled()))
    m_size2 = ui->sizeLineEdit2->text();
  if ((m_sizeUnitsEnabled = ui->sizeUnitsComboBox->isEnabled()))
    m_sizeUnitsIdx = ui->sizeUnitsComboBox->currentIndex();
  if ((m_sizeUnits2Enabled = ui->sizeUnitsComboBox2->isEnabled()))
    m_sizeUnits2Idx = ui->sizeUnitsComboBox2->currentIndex();

  m_dateTimeIdx = ui->dateTimeComboBox->currentIndex();
  if ((m_dateTimeEnabled = ui->dateTimeEdit->isEnabled()))
    m_dateTime = ui->dateTimeEdit->dateTime();
  if ((m_dateTime2Enabled = ui->dateTimeEdit2->isEnabled()))
    m_dateTime2 = ui->dateTimeEdit2->dateTime();

  m_searchAll = sui->searchAllDirsCheckBox->isChecked();

  m_targetRadio = sui->alsoRadioButton->isChecked() ? ALSO
            : sui->onlyRadioButton->isChecked() ? ONLY : SKIP;

  m_general = 0U;
  if (sui->tabCheckBox->isChecked())
    m_general |= GEN_TAB;
  if (sui->ignoreCheckBox->isChecked())
    m_general |= GEN_IGNORE;
  if (sui->cStyleCheckBox->isChecked())
    m_general |= GEN_CSTYLE;

  m_more = sui->moreLineEdit->text();
  m_targetIdx = sui->targetComboBox->currentIndex();

  listWidget2List(sui->includeListWidget, &m_lft);
  listWidget2List(sui->excludeListWidget, &m_rght);
}
