#ifndef QUICKSEARCHSTATE_H
#define QUICKSEARCHSTATE_H

#include "SearchParameter.h"
#include "SearchState.h"
#include "Singleton.h"
#include "Types.h"

class QuickSearchState : public SearchState
{
public:
	virtual ~QuickSearchState() {}
	virtual State state() const { return QUICK; }
	virtual void reset();

private:
    virtual void DoGatherSearchParameter(SearchParameter& param, Ui::MainWindow* window);
	 virtual QString DoGetContentKeyword(Ui::MainWindow* window);
	 //virtual QString DoGetFileNameKeywords(Ui::MainWindow* window);
	 virtual QString DoGetSearchPath(Ui::MainWindow* window);
};

typedef Singleton<QuickSearchState> QuickSearchStateSingleton;

#endif
