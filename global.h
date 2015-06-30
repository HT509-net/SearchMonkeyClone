/******************************************************************************
 * NAME:    global.h
 *
 * PURPOSE: Definitions for global.cpp.
 *
 * DATE:    December 6, 2009
 *
 * REVISIONS:
 * ---------
 * [120609] shawno- initial coding
*******************************************************************************/
#ifndef GLOBAL_H
#define GLOBAL_H

#include <QMap>
#include <QRegExp>
#include <QString>

typedef QMap<QString, int> WordCount;

class MapGlobals {

public:
  static MapGlobals *instance() {
    if (!m_instance)
      m_instance = new MapGlobals();
    return m_instance;
  }
  static MapGlobals *instance(Qt::CaseSensitivity kase, QString ptrn,
                     QRegExp::PatternSyntax sntx) {
    if (!m_instance)
      m_instance = new MapGlobals(kase, ptrn, sntx);
    return m_instance;
  }
  inline Qt::CaseSensitivity getCase() { return SMCASE; }
  inline QString getPtrn() { return SMPTRN; }
  inline QRegExp::PatternSyntax getSntx() { return SMSNTX; }

  inline void setCase(Qt::CaseSensitivity kase) { SMCASE = kase; }
  inline void setSntx(QRegExp::PatternSyntax sntx) { SMSNTX = sntx; }
  inline void setPtrn(QString ptrn) { SMPTRN = ptrn; }

protected:
  MapGlobals() {
    SMCASE = Qt::CaseSensitive;
    SMPTRN = QString("");
    SMSNTX = QRegExp::RegExp;
  }
  MapGlobals(Qt::CaseSensitivity kase, QString ptrn,
          QRegExp::PatternSyntax sntx) :
          SMCASE(kase),
          SMPTRN(ptrn),
          SMSNTX(sntx) { }

  virtual ~MapGlobals() { if (m_instance) delete m_instance; }

private:
  static MapGlobals *m_instance;

  Qt::CaseSensitivity    SMCASE;
  QString                SMPTRN;  // shadows MainWindow m_text
  QRegExp::PatternSyntax SMSNTX;  // shadows MainWindow m_syntax
};

WordCount countHits(QString _abspath);
void reduceHits(WordCount &result, const WordCount &w);

#endif // GLOBAL_H
