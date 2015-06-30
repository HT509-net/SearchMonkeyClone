#ifndef SMDIR_H
#define SMDIR_H

#include <QFileInfo>
#include <QRegExp>
#include <QSharedPointer>
#include <QString>

#include "Searchable.h"

class PhaseOneParameter;

class SMDir : public Searchable
{
public:
    SMDir(const PhaseOneParameter& param);
   SMDir(unsigned sessionId, const QString& path, bool recursive,
       unsigned recursiveDepth, Settings *sui, bool skipHidden = true,
       bool followSymLink = false, bool invertSearch = false);
    ~SMDir();

private:
   virtual void DoSearch(const QStringList& keywords,
                  const Qt::CaseSensitivity& caseSensitive,
                  QRegExp::PatternSyntax syntax = QRegExp::FixedString);
    virtual void DoStopSearch();

    QSharedPointer<Searchable> BuildSearchable(const QFileInfo& info);
    unsigned searchSessionId;
    QString keyword;
    QString path;
    bool recursive;
    unsigned recursiveDepth;
   Settings *sui;
   bool skipHidden;
    bool followSymLink;
    bool invertSearch;

    bool stopIssued;

   // we can't use this list anymore since we need to change the way
   // constructing those searchables we should only construct the needed
   // searchable(the one needs do the search) when a search is called upon it
   // to reduce mem usage. otherwise mem usage is too big (>100m)
   //QList<Searchable*> searchables;
};

#endif
