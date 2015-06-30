#ifndef ADVANCEDSEARCHSTATE_H
#define ADVANCEDSEARCHSTATE_H

#include "SearchState.h"
#include "Singleton.h"
#include "SearchParameter.h"
#include "Types.h"

class AdvancedSearchState : public SearchState
{
public:
  virtual ~AdvancedSearchState() {}
  virtual State state() const { return ADVANCED; }
  virtual void reset();

private:
   Qt::CaseSensitivity CaseSensitivity(Ui::MainWindow* window);
   void DoGatherSearchParameter(SearchParameter& parameter, Ui::MainWindow* window);
   virtual QString DoGetContentKeyword(Ui::MainWindow* window);
   //virtual QString DoGetFileNameKeywords(Ui::MainWindow* window);
   virtual QString DoGetSearchPath(Ui::MainWindow* window);
   bool FollowSymLink(Ui::MainWindow* window);
   TimeType GetTimeTypeFrom(const QString& option);
   bool InvertSearch(Ui::MainWindow* window);
   void SetSizeFilter(Ui::MainWindow* window, SearchParameter& parameter);
   void SetTimeFilter(Ui::MainWindow* window, SearchParameter& parameter);
    void SetTimeFilterForTimeType(const QString& option, const TimeType timeType, Ui::MainWindow* window, SearchParameter& parameter);
   bool SkipHiddenFiles(Ui::MainWindow* window);
   QRegExp::PatternSyntax Syntax(Ui::MainWindow* window);
    QString TimeLowerBoundString(const TimeType timeType);
    QString TimeRangeString(const TimeType timeType);
   QString TimeUpperBoundString(const TimeType timeType);
};

typedef Singleton<AdvancedSearchState> AdvancedSearchStateSingleton;

#endif
