/******************************************************************************
 * NAME:     highlighter.cpp
 *
 * PURPOSE:  Provides highlighting for matched expressions in QTextEdit
 *           object displaying matched lines.
 *
 * DATE:     September 28, 2009
 *
 * REVISIONS:
 * ---------
 * [092809] shawno- initial coding.
 * [040310] shawno- added changeFormat() and change to highlightBlock()
*******************************************************************************/
#include "highlighter.h"
#include "Utility.h"

Highlighter::Highlighter(QTextDocument *parent, const QString &_text,
    QRegExp::PatternSyntax _syntax, Qt::CaseSensitivity _cs)
   : QSyntaxHighlighter(parent) {

   m_rx.setPattern(_text);
   m_rx.setPatternSyntax(_syntax);
   m_rx.setCaseSensitivity(_cs);

     QBrush brush(COLOR1_HIGHLIGHT);
   m_format.setForeground(brush);
   m_format.setFontWeight(QFont::Bold);
}
void Highlighter::changeFormat(QBrush _foreground, int _fontWeight, int _length,
                                  int _start) {
   m_format.setForeground(_foreground);
   m_format.setFontWeight(_fontWeight);
   setFormat(_start, _length, m_format);
}
void Highlighter::highlightBlock(const QString &_text) {
  int hack = _text.indexOf(">");  // line number form is: n>

  int index = _text.indexOf(m_rx);
  while (index >= 0) {
    int length = m_rx.matchedLength();
    if (index > hack)  // do not highlight the line number
      setFormat(index, length, m_format);
    index = _text.indexOf(m_rx, index + length);
  }
}
