#ifndef DEBUG_H
#define DEBUG_H

// #define QT_NO_DEBUG         // buildout qDebug() - needs a clean build
#define QT_NO_DEBUG_OUTPUT  // ""

#include <QDebug>
#include <QString>

extern bool LEVEL[9];

#define qd(x) if (LEVEL[x]) qDebug()

class Entex
{
  public:
    Entex(const QString &_fname, int _level)
    :
    fname(_fname),
    level(_level)
    {
        if (LEVEL[level])
            qDebug() << "entering " << fname << "()...";
    }
    virtual ~Entex()
    {
        if (LEVEL[level])
            qDebug() << "...exiting " << fname << "()";
    }
    inline QString fn() const { return fname + "(): "; }

	QString fname;
	int level;
};

#endif // DEBUG_H
