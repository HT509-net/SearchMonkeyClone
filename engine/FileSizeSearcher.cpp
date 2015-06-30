#include "debug.h"

#include "FileSizeSearcher.h"
#include "Searcher.h"
#include "../SearchParameter.h"
#include "../Utility.h"

FileSizeSearcher::FileSizeSearcher(Searcher* searcher, const PhaseOneParameter& param)
:
SearcherDecorator(searcher)
{
    sizeLowerBound = param.GetSizeLowerBound();
    sizeUpperBound = param.GetSizeUpperBound();
	 qDebug() << "FileSizeSearcher::ctor(): sizeLowerBound-" << sizeLowerBound << " sizeUpperBound-" << sizeUpperBound;
}
void FileSizeSearcher::DoUpdate(const Result& result)
{
    const int matchedFileSize = result->matchedFile.size();

	 qDebug() << "FileSizeSearcher::DoUpdate(): matchedFileSize-" << matchedFileSize;

    if ((sizeLowerBound == 0) && (sizeUpperBound == 0))
		  return;  //Todo: throw exception - should not happen

    if (IsValueInRange(matchedFileSize, sizeLowerBound, sizeUpperBound) == true)
    {
        qDebug() << "match meet size requirement";
		  Notify(result);  // ok, meet requirement, pass it along
	 }
}
