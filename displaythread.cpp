#include "debug.h"

#include "displaythread.h"
#include "tabresults.h"

void DisplayThread::run() {

  this->tabResults->displayLines(index);
  //tabResults->hello();
}

