#ifndef SEARCHRESULT_H
#define SEARCHRESULT_H

#include <QFileInfo>
#include <QSharedPointer>
#include <QString>
#include <QVector>

#include <iostream>  // Todo: remove later

using namespace std;

struct LineInfo
{
    LineInfo() {}
    LineInfo(unsigned lineNo, const QString& line)
    :
    lineNo(lineNo),
    content(line)
    {}

    int lineNo;
    QString content;

   bool operator ==(const LineInfo& line);
};

struct SearchResult
{
   SearchResult() {} // add this default ctor because of qRegisterMetaType()

    SearchResult(unsigned sessionId, const QFileInfo& fileInfo)
    :
    searchSessionId(sessionId),
    matchedFile(fileInfo)
   {}

   unsigned searchSessionId;
   QFileInfo matchedFile;
   QVector<LineInfo> matchedLines;

   bool operator ==(const SearchResult& result);
};

typedef QSharedPointer<SearchResult> Result;

bool operator ==(const Result& lhs, const Result& rhs);

//TODO:: remove later test purpose only
ostream& operator<<(ostream& os, const LineInfo& line);
ostream& operator<<(ostream& os, const SearchResult& result);

/* class SearchResult
{
public:
   // TODO:: the default max_hit value should come from setting module
   SearchResult(int seq_no, const QFileInfo& file_info, int max_hit = 25);
   void AddMatchedLine(const LineInfo& line);
   void AddMatchedLine(int line_no, const string& content);
   QFileInfo GetMatchedFile();
   bool HasMoreMatchLine();
   LineInfo FirstMatchLine();
   LineInfo NextMatchLine();
   LineInfo LastMatchLine();
   // this will change the return value for NextMatchLine() to the first match line
   // TODO:: maybe there is better way to do this?
   void ResetCurrentMatchLine();

private:
   int search_seq_no;
   QFileInfo matched_file;
   // TODO:: reserve the vector space based on
   // the user designated value in the UI
   // what should be the default value?
   vector<LineInfo> matched_lines;
   vector<LineInfo>::const_iterator current_iterator;
}; */

#endif // SEARCHRESULT_H
