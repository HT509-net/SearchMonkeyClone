#include "debug.h"

#include <QString>

#include "QuickSearchState.h"
#include "ui_mainwindow.h"
#include "Utility.h"

void QuickSearchState::DoGatherSearchParameter(
  SearchParameter& param,
  Ui::MainWindow* window
)
{ (void) param, (void) window;
}

//---------------------------------------------------------------------------------

QString QuickSearchState::DoGetContentKeyword(Ui::MainWindow* window)
{
  Entex ee("QuickSearchState::DoGetContentKeyword", 2);
  return window->quickContentLineEdit->text();
}

//---------------------------------------------------------------------------------

/*QString QuickSearchState::DoGetFileNameKeywords(Ui::MainWindow* window)
{
    return window->quickFileComboBox->currentText();
}*/

//---------------------------------------------------------------------------------

QString QuickSearchState::DoGetSearchPath(
  Ui::MainWindow* window
)
{
   return window->quickDirectoryLineEdit->text();
}

//---------------------------------------------------------------------------------

void QuickSearchState::reset()
{
  if (m_ui->quickDirectoryLineEdit->text().isEmpty() &&
      m_ui->quickFileLineEdit->text().isEmpty() &&
      m_ui->quickContentLineEdit->text().isEmpty())
    return;

  if (!boxConfirm("Really clear all 3 search boxes?"))
    return;

  m_ui->quickDirectoryLineEdit->clear();
  m_ui->quickFileLineEdit->clear();
  m_ui->quickContentLineEdit->clear();
}

