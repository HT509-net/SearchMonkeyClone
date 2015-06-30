#include "SearchResult.h"

/*
SearchResult::SearchResult(int seq_no, const QFileInfo& file_info, int max_hit)
   :
   search_seq_no(seq_no),
   matched_file(file_info)
{
   matched_lines.reserve(max_hit);
   //TODO:: what happens if match_lines is empty? at this momement, it's
   // definitely empty
   //TODO:: what should be the initial value for current_iterator
   // Will just the begin() better?
   // but then what to do with HasMoreMatchLine()?
   current_iterator = matched_lines.begin() - 1;
}

void SearchResult::AddMatchedLine(const LineInfo& line)
{
   matched_lines.push_back(line);
}

void SearchResult::AddMatchedLine(int line_no, const string& content)
{
   LineInfo line(line_no, content);
   matched_lines.push_back(line);
}

QFileInfo SearchResult::GetMatchedFile()
{
   return matched_file;
}

bool SearchResult::HasMoreMatchLine()
{
   return (current_iterator + 1 != matched_lines.end());
}

LineInfo SearchResult::FirstMatchLine()
{
   if (matched_lines.empty() != false)
   {
      return matched_lines.front();
   } 
   // TODO:: throw exception?
}

LineInfo SearchResult::NextMatchLine()
{
   ++current_iterator;

   if (current_iterator != matched_lines.end())
   {
      return *current_iterator;
   }

   // throw exception??
}

LineInfo SearchResult::LastMatchLine()
{
   if (matched_lines.empty() != false)
   {
      return matched_lines.back();
   } 
   // TODO:: throw exception?
}

void SearchResult::ResetCurrentMatchLine()
{
   current_iterator = matched_lines.begin();
}
*/

bool LineInfo::operator ==(const LineInfo& line)
{
    if ((lineNo == line.lineNo)
       && (content == line.content))
    {
        return true;
    }

    return false;
}

bool SearchResult::operator ==(const SearchResult& result)
{
    if ((matchedFile == result.matchedFile)
     && (matchedLines == result.matchedLines))
    {
        return true;
    }

    return false;
}

ostream& operator<<(ostream& os, const LineInfo& line)
{
   os << "Line " << line.lineNo << ":" << endl;
   os << line.content.toStdString() << endl;

   return os;
}

ostream& operator<<(ostream& os, const SearchResult& result)
{
    os << "matched file: " << result.matchedFile.fileName().toStdString() << endl;
    os << "matched lines: ";

    QVector<LineInfo>::const_iterator iter = result.matchedLines.begin();

    for (; iter != result.matchedLines.end(); ++iter)
    {
        os << *iter;
    }

    os << endl;

    return os;
}

bool operator ==(const Result& lhs, const Result& rhs)
{
    return *lhs == *rhs;
}
