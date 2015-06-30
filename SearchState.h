#ifndef SEARCHSTATE_H
#define SEARCHSTATE_H

#include <QStringList>

#include "SearchParameter.h"
#include "ui_mainwindow.h"

class MainWindow;
class Settings;

class SearchState
{
  friend class MainWindow;
  friend class TabResults;

public:
   virtual ~SearchState() {}
   SearchParameter GatherSearchParameter(Ui::MainWindow* window, Settings* sui);
   void init(Ui::MainWindow* window, Settings* sui)
   {
     m_ui = window, m_sui = sui;
   }
   void setText(QString *_content);
   virtual State state() const = 0;
   virtual void reset() = 0;

protected:
   QString GetContentKey(Ui::MainWindow* window);
   QStringList GetContentKeyword(Ui::MainWindow* window);
   QStringList GetFileNameKeywords(Ui::MainWindow* window, Settings* sui);
   QString GetSearchPath(Ui::MainWindow* window);

   Ui::MainWindow* m_ui;
   Settings* m_sui;

private:
    virtual void DoGatherSearchParameter(SearchParameter& param, Ui::MainWindow* window);
   virtual QString DoGetContentKeyword(Ui::MainWindow* window) = 0;
   //virtual QString DoGetFileNameKeywords(Ui::MainWindow* window) = 0;
   virtual QString DoGetSearchPath(Ui::MainWindow* window) = 0;
};

#endif
