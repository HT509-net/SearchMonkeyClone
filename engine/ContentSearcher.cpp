#include "debug.h"

#include "ContentSearcher.h"
#include "Searchable.h"
#include "Searcher.h"
#include "../SearchParameter.h"
#include "SMFile.h"
#include "Utility.h"

ContentSearcher::ContentSearcher(Searcher* searcher, const PhaseTwoParameter& param)
:
SearcherDecorator(searcher),
contentKeyword(param.GetKeyword()),
caseSensitive(param.GetCaseSensitivity()),
syntax(param.GetSyntax())
{}
void ContentSearcher::DoUpdate(const Result& result)
{
	 //qDebug() << "ContentSearcher::DoUpdate()";
    Searchable* searchable = new SMFile(result->searchSessionId, result->matchedFile);

	 // here we make the ContentSearcher's observers to observer searchable
	 // since searchable will call Notify() which has the eventual result
    CallEach(GetObservers(), searchable, &Subject::Register);
    searchable->Search(contentKeyword, caseSensitive, syntax);
}
