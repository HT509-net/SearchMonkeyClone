/******************************************************************************
 * NAME:    main.cpp
 *
 * PURPOSE: Searchmonkey program entry point.
 *
 * DATE:    October 18, 2009
 *
 * REVISIONS:
 * ---------
 * [101809] adamc- initial coding.
 * [120609] shawno- preprocessor directives
 * [030110] shawno- debug stuff
*******************************************************************************/
#include <iostream>
#include <QApplication>
#include "mainwindow.h"

#ifndef QT_NO_CONCURRENT

bool ABORT_COUNTING_HITS,
    LEVEL[10];  // debug levels

int main  (int argc, char *argv[]) {
  ABORT_COUNTING_HITS = false;
  LEVEL[0] = 1;
  LEVEL[1] = 1;
  LEVEL[2] = 1;
  LEVEL[3] = 1;
  LEVEL[4] = 0;
  LEVEL[5] = 0;
  LEVEL[6] = 1;
  LEVEL[7] = 0;
  LEVEL[8] = 0;
  LEVEL[9] = 0;
  QApplication a(argc, argv);
  MainWindow w;
  w.show();
  return a.exec();
}

#else

int main() {
  qDebug()  << "Qt Concurrent is not yet supported on this platform.";
  std::cerr << "Qt Concurrent is not yet supported on this platform.";
}

#endif
