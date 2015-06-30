#include "debug.h"

#include "AdvancedSearchState.h"
#include "SessionIDGenerator.h"
#include "ui_mainwindow.h"
#include "Utility.h"

Qt::CaseSensitivity AdvancedSearchState::CaseSensitivity(Ui::MainWindow* window)
{
	 return (window->caseCheckBox->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive);
}
void AdvancedSearchState::DoGatherSearchParameter(SearchParameter& parameter,
                                                  Ui::MainWindow* window)
{   // Todo: set other flags in param based on the user input later
	 parameter.phaseOneParam->SetCaseSensitivity(CaseSensitivity(window));
	 parameter.phaseOneParam->SetFollowSymLink(FollowSymLink(window));
	 parameter.phaseOneParam->SetInvertSearch(InvertSearch(window));
	 parameter.phaseOneParam->SetSkipHidden(SkipHiddenFiles(window));
	 parameter.phaseOneParam->SetSyntax(Syntax(window));
    SetTimeFilter(window, parameter);
    SetSizeFilter(window, parameter);
}
QString AdvancedSearchState::DoGetContentKeyword(Ui::MainWindow* window)
{
	 qDebug() << "in AdvancedSearchState::DoGetContentKeyword()";
	 return window->contentLineEdit->text();
}
/*QString AdvancedSearchState::DoGetFileNameKeyword(Ui::MainWindow* window)
{
    qDebug() << "in AdvancedSearchState::DoGetFileNameKeyword()";
    return window->fileComboBox->currentText();
}*/
QString AdvancedSearchState::DoGetSearchPath(Ui::MainWindow* window)
{
	 qDebug() << "in AdvancedSearchState::DoGetSearchPath()";
	 return window->directoryLineEdit->text();
}
bool AdvancedSearchState::FollowSymLink(Ui::MainWindow* window)
{
	 return window->symLinkCheckBox->isChecked();
}
bool AdvancedSearchState::InvertSearch(Ui::MainWindow* window)
{
	 (void) window;
	 return false; /*window->invertCheckBox->isChecked();*/
}
TimeType AdvancedSearchState::GetTimeTypeFrom(const QString& option)
{
	if (option.indexOf("Modified") != -1)
		return ModifiedTime;
	else if (option.indexOf("Accessed") != -1)
		return AccessedTime;
	else if (option.indexOf("Created") != -1)
		return CreatedTime;
	else {
		Q_ASSERT(0);
		return ModifiedTime;
	}
}
void AdvancedSearchState::reset()
{
	if (m_ui->directoryLineEdit->text().isEmpty() &&
		 m_ui->fileLineEdit->text().isEmpty() &&
		 m_ui->contentLineEdit->text().isEmpty())
		return;

	if (!boxConfirm("Really clear all 3 search boxes?"))
		return;

	m_ui->directoryLineEdit->clear();
	m_ui->fileLineEdit->clear();
	m_ui->contentLineEdit->clear();
}
void AdvancedSearchState::SetSizeFilter(Ui::MainWindow* window, SearchParameter& parameter)
{
	QString option = window->sizeComboBox->currentText();
	bool sizeIgnored = option.indexOf("Ignore") != -1;

	qint64 lowerSize = 0,
			 upperSize = 0;

	if (sizeIgnored == false)
	{
		bool between = option.indexOf("Between") != -1;
		bool larger  = option.indexOf("Larger")  != -1;
		bool smaller = option.indexOf("Smaller") != -1;

		if (larger)
			lowerSize = units2bytes(QString("%1 %2").arg(window->sizeLineEdit->text()).arg(
							chop(window->sizeUnitsComboBox->currentText(), 4))); // chop 'ytes'
		if (smaller)
			upperSize = units2bytes(QString("%1 %2").arg(window->sizeLineEdit->text()).arg(
							chop(window->sizeUnitsComboBox->currentText(), 4))); // chop 'ytes'
		if (between)
		{
			lowerSize = units2bytes(QString("%1 %2").arg(window->sizeLineEdit->text()).arg(
							chop(window->sizeUnitsComboBox->currentText(), 4))); // chop 'ytes'

			upperSize = units2bytes(QString("%1 %2").arg(window->sizeLineEdit2->text()).arg(
							chop(window->sizeUnitsComboBox2->currentText(), 4)));
		}
	}
	parameter.phaseOneParam->SetSizeLowerBound(lowerSize);
	parameter.phaseOneParam->SetSizeUpperBound(upperSize);
}
void AdvancedSearchState::SetTimeFilter(Ui::MainWindow* window, SearchParameter& parameter)
{
	QString option = window->dateTimeComboBox->currentText();

	if (option == "Ignored")
		return;

	TimeType timeType = GetTimeTypeFrom(option);

	SetTimeFilterForTimeType(option, timeType, window, parameter);
}
void AdvancedSearchState::SetTimeFilterForTimeType(const QString& option, const TimeType timeType, Ui::MainWindow* window, SearchParameter& parameter)
{
	if (option.indexOf(TimeLowerBoundString(timeType)) != -1)
	{
		QDateTime beginTime = window->dateTimeEdit->dateTime();
		parameter.phaseOneParam->SetBeginTime(beginTime);
		parameter.phaseOneParam->SetMatchedTimeType(timeType);
	}
	else if (option.indexOf(TimeUpperBoundString(timeType)) != -1)
	{
		QDateTime endTime = window->dateTimeEdit->dateTime();
		parameter.phaseOneParam->SetEndTime(endTime);
		parameter.phaseOneParam->SetMatchedTimeType(timeType);
	}
	else if (option.indexOf(TimeRangeString(timeType)) != -1)
	{
		QDateTime beginTime = window->dateTimeEdit->dateTime();
		QDateTime endTime = window->dateTimeEdit2->dateTime();
		parameter.phaseOneParam->SetBeginTime(beginTime);
		parameter.phaseOneParam->SetEndTime(endTime);
		parameter.phaseOneParam->SetMatchedTimeType(timeType);
	}
}
bool AdvancedSearchState::SkipHiddenFiles(Ui::MainWindow* window)
{
	 return window->skipHiddenCheckBox->isChecked();
}
QRegExp::PatternSyntax AdvancedSearchState::Syntax(Ui::MainWindow* window)
{
	(void) window;
	return QRegExp::RegExp; // uint2syntax(static_cast<uint>(window->syntaxComboBox->currentIndex()));
}
QString AdvancedSearchState::TimeLowerBoundString(const TimeType timeType)
{
	switch (timeType)
	{
	case AccessedTime:
		return "Accessed After";
	case ModifiedTime:
	   return "Modified After";
	case CreatedTime:
	   return "Created After";
	default:
	   return "";
	}
}
QString AdvancedSearchState::TimeRangeString(const TimeType timeType)
{
	switch (timeType)
	{
	case AccessedTime:
	   return "Accessed Between";
	case ModifiedTime:
	   return "Modified Between";
	case CreatedTime:
	   return "Created Between";
	default:
	   return "";
	}
}
QString AdvancedSearchState::TimeUpperBoundString(const TimeType timeType)
{
	switch (timeType)
	{
	case AccessedTime:
		return "Accessed Before";
	case ModifiedTime:
		return "Modified Before";
	case CreatedTime:
		return "Created Before";
	default:
		return "";
	}
}
