/******************************************************************************
 * NAME:       highlighter.h
 *
 * PURPOSE:    Provides highlighting for matched expressions in QTextEdit
 *             object displaying matched lines.
 *
 * DATE:       September 28, 2009
 *
 * REVISIONS:
 * ---------
 * [092809] shawno- initial coding.
 * [040310] shawno- added changeFormat()
*******************************************************************************/
#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QBrush>
#include <QRegExp>
#include <QSyntaxHighlighter>
#include <QTextCharFormat>

QT_BEGIN_NAMESPACE
class QTextDocument;
QT_END_NAMESPACE

class Highlighter : public QSyntaxHighlighter {
	 Q_OBJECT

public:
	 Highlighter(QTextDocument *parent = 0, const QString &_text = "",
					 QRegExp::PatternSyntax _syntax = QRegExp::RegExp,
					 Qt::CaseSensitivity _cs = Qt::CaseSensitive);
	 void changeFormat(QBrush foreground, int fontWeight, int length, int start = 0);
	 inline void init() {}
protected:
	 void highlightBlock(const QString &text);

private:
	 QRegExp m_rx;
	 QTextCharFormat m_format;
};

#endif // HIGHLIGHTER_H
