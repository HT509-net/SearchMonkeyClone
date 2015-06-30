#ifndef SEARCHERFACTORY_H
#define SEARCHERFACTORY_H

class Searcher;
class SearchParameter;

class SearcherFactory
{
public:
    static Searcher* BuildSearcher(const SearchParameter& param);
};

#endif
