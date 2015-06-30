/******************************************************************************
 * NAME:    global.cpp
 *
 * PURPOSE: Searchmonkey global variables and functions for the call to
 *          mappedReduced (including QFutureWatcher callback) in mainwindow.cpp.
 *
 * DATE:    December 6, 2009
 *
 * REVISIONS:
 * ---------
 * [120609] shawno- initial coding
 * [031610] shawno- added typedefs and globals WATCHER and HITS
*******************************************************************************/
#include <QApplication>
#include <QFile>
#include <QFuture>
#include <QFutureWatcher>

#include "global.h"
#include "Singleton.h"

MapGlobals *MapGlobals::m_instance = 0;

extern bool ABORT_COUNTING_HITS;

/******************************************************************************
 * CountHits(): Count lines in file that contain at least one pattern (m_text).
 ******************************************************************************/
WordCount countHits  (QString _abspath) {
	QFile file(_abspath);

	MapGlobals * z = Singleton<class MapGlobals>::Instance();
	QRegExp rx(z->getPtrn());
	rx.setPatternSyntax(z->getSntx());
	rx.setCaseSensitivity(z->getCase());
	const QRegExp crx(rx);  // performance tweak

	WordCount wc;
	wc[_abspath] = 0;

	if (file.open(QIODevice::ReadOnly)) {
		QString line;
		QTextStream in(&file);
		while (!in.atEnd()) {
			qApp->processEvents();

			if (ABORT_COUNTING_HITS)
				break;

			line = in.readLine();

			if (crx.indexIn(line, 0) != -1)
				wc[_abspath] += 1;
		}
	}
	else
		fprintf(stderr, "could not open %s", _abspath.toStdString().c_str());

	return wc;
}
/******************************************************************************
 * ReduceHits(): reduce hit maps to one map to return to caller.
 ******************************************************************************/
void reduceHits  (WordCount &result, const WordCount &w) {
	QMapIterator<QString, int> i(w);
	while (i.hasNext()) {
		i.next();
		result[i.key()] += i.value();
	}
}
