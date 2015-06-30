/******************************************************************************
 * NAME:       dir.h
 *
 * PURPOSE:    Defines class Dir (a data attribute of class Settings).
 *
 * DATE:       January 2, 2010
 *
 * REVISIONS:
 * ---------
 * [010210] shawno- initial coding.
*******************************************************************************/
#ifndef DIR_H
#define DIR_H

#include "settings.h"
#include "ui_settings.h"

// awk: autom4te.cache, blib, _build, .bzr, .cdv, cover_db, CVS, _darcs, ~.dep,
//      ~.dot, .git, .hg, ~.nib, .pc, ~.plst, RCS, SCCS, _sgbak and .svn
const QStringList EXCLUDES_DFLT = (QStringList()
<< "autom4te.cache" << "blib" << "_build" << ".bzr" << ".cdv" << "cover_db"
<< "CVS" << "_darcs" << ".dep" << ".dot" << ".git" << ".hg" << ".nib" << ".pc"
<< ".plst" << "RCS" << "SCCS" << "_sgbak" << ".svn" << "bin_sparc");
//<< "Local Settings");

QT_BEGIN_NAMESPACE
class QFile;
QT_END_NAMESPACE

class Dir {

public:
  Dir (Ui::Settings *sui);
  virtual ~Dir() { }
  void add(Settings *settings);
  void cancel();
  inline int dirFilterSz() const { return m_right.size(); }
  inline QStringList filter() const { return m_right; }
  bool filterDir(const QString &_dir);
  QStringList filterDirs(const QStringList &_dirs);
  void init();
  void initListWidget();
  void list2File(QFile &_file, const QStringList & _list,
                      const QString &_tag);
  bool listContains(const QListWidget *lw, const QString &s);
  void moveAllItems(QListWidget *src, QListWidget *dst);
  void moveSelectedItem(QListWidget *src, QListWidget *dst);
  void okApply();
  void reset();
  void saveInCaseCancel();
  void setDirFilter();
  void setSearchAll(bool b) { m_searchAll = b; }
  Ui::Settings *sui();

private:
  bool m_searchAll;
  QStringList m_left,
          m_right,   // search filter (excludes)
          m_saveLeft,
          m_saveRight;

  Ui::Settings *m_sui;

private slots:
   void on_generalButtonBox_clicked(QAbstractButton* button);
};

#endif // DIR_H
