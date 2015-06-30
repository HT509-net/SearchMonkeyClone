#include "ContentSearcher.h"
#include "FileNameSearcher.h"
#include "FileSizeSearcher.h"
#include "FileTimeSearcher.h"
#include "Searcher.h"
#include "SearcherFactory.h"
#include "../SearchParameter.h"

Searcher* SearcherFactory::BuildSearcher(const SearchParameter& param)
{
    PhaseOneParameter oneParam = *param.phaseOneParam;

	 Searcher* searcher = new FileNameSearcher(oneParam); // need to build anyhow

    if (oneParam.HasSizeConstraint())
    {
        searcher = new FileSizeSearcher(searcher, oneParam);
    }

    if (oneParam.HasTimeConstraints())
    {
        searcher = new FileTimeSearcher(searcher, oneParam);
    }

	 // for performance issue, we should always construct ContentSearcher at last so this content searcher will performed at a minimum time
    if (param.PhaseTwoSearchNeeded())
    {
        searcher = new ContentSearcher(searcher, *param.phaseTwoParam);
    }

    return searcher;
}
