/******************************************************************************
 * NAME:      Utility.cpp
 *
 * PURPOSE:   Searchmonkey reusable and miscellaneous functions.
 *
 * DATE:      September 6, 2009
 *
 * REVISIONS:
 * ---------
 * [090609] shawno- bytes2units(), units2bytes()
 * [091609] shawno- listSubtract(), ms2units()
 * [112209] shawno- chop(), parseDelim(), strip()
 * [011010] shawno- positionAtLine()
 * [022710] Xiaoming- lib.cpp combined with Utility.cpp
*******************************************************************************/
#include "debug.h"

#include <QDir>
#include <QFileInfo>
#include <QList>

#include "Utility.h"

bool BoundSpecified(int boundValue)
{
   return boundValue != 0;
}
bool BoundSpecified(QDateTime boundValue)
{
   return boundValue.isNull() == false;
}
/******************************************************************************
 * BoxConfirm(): pop up a confirmation message box.  (Default - warning icon.)
 ******************************************************************************/
bool boxConfirm  (const QString &_s, const QMessageBox::Icon _icon) {
  QMessageBox msgBox;
  msgBox.setText(_s);
  msgBox.setIcon(_icon);
  msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
  msgBox.setDefaultButton(QMessageBox::Cancel);
  msgBox.setWindowIcon(QIcon(":/pixmaps/searchmonkey-300x300.png"));
  switch(msgBox.exec()) {
    case QMessageBox::Ok:
      return true;
    case QMessageBox::Cancel:
      return false;
    default:
      Q_ASSERT(0);
      return true;
  }
}
/******************************************************************************
 * BoxError(): pop up an error message box.  (Default - warning icon.)
 ******************************************************************************/
void boxError  (const QString &_s, const QMessageBox::Icon _icon) {
  QMessageBox msgBox;
  msgBox.setText(_s);
  msgBox.setIcon(_icon);
  msgBox.setWindowIcon(QIcon(":/pixmaps/searchmonkey-300x300.png"));
  msgBox.exec();
}
/******************************************************************************
 * bytes2units(): Convert bytes to B, KB, MB, GB, ...
 *
 * parms   : _size - bytes
 *
 * returns : '0 B' if _size < 0; otherwise, nice string like '724 KB'
 ******************************************************************************/
QString bytes2units  (qint64 _size) {

  quint64 size = _size < 0 ? Q_UINT64_C(0) : static_cast<ull>(_size);
  (void)_size;

  char s[16];
  for (int i = 0; i < 16; s[i++]='\0') ;

  if (size < KB)
    sprintf (s, "%d  B ", static_cast<int>(size));
  else if (size < MB) {
    sprintf (s, "%1.1lf KB ", static_cast<double>(size) / KB);
    //
    // enforce N KB not N.0 KB
    //
    string _s(s);
    string::size_type loc = _s.find(".0");
    if (loc != string::npos) {
      _s.erase(loc, 2);
      sprintf (s, "%s", _s.c_str());
    }
  }
  else if (size < GB)
    sprintf (s, "%1.1lf MB ", static_cast<double>(size) / MB);
  else if (size < TB)
    sprintf (s, "%1.1lf GB ", static_cast<double>(size) / GB);
  else if (size < PB)
    sprintf (s, "%1.1lf TB ", static_cast<double>(size) / TB);
  else
    sprintf (s, "%1.1lf PB ", static_cast<double>(size) / PB);

  return QString(s);
}
/******************************************************************************
 * Chop()  : wrapper with return val for: void QString::chop().
 *
 * parms   : s - string to be chopped
 *           n - chars to chop off end of s
 *
 * returns : chopped QString
 ******************************************************************************/
QString chop  (QString s, int n) {
  s.chop(n);
  return s;
}
/******************************************************************************
 * getLine() : get line from searchmonkey config file.
 *
 * parms     : _file - config file
 *
 * returns   : line as QString
 ******************************************************************************/
QString getLine(QFile & _file) {
  char buf[512];
  memset (buf, '\0', sizeof(buf));
  QString line;
  while (!_file.atEnd()) {
    _file.readLine(buf, sizeof(buf));
    line = QString(buf);
    line = line.trimmed();
    if (line.isEmpty()  ||  line[0] == '#')
      continue;
    else
      break;
  }
  return line;
}
/******************************************************************************
 * IsValidDir(): Validate a Qstring as an existing dir for this application.
 ******************************************************************************/
bool IsValidDir(const QString& dir)
{
   QFileInfo fi(dir);
   return fi.isDir();
}
/******************************************************************************
 * List2ListWidget(): populate list widget from string list.
 ******************************************************************************/
void list2ListWidget  (const QStringList &sl, QListWidget *lw) {
  if (sl.isEmpty() || !lw)
    return;

  lw->clear();
  lw->addItems(sl);
}
/******************************************************************************
 * listSubtract(): Subtract one list from another.  (list1 - list2)
 *
 * parms   : list1 - minuend
 *           list2 - subtrahend
 *
 * returns : difference list
 ******************************************************************************/
QStringList listSubtract  (const QStringList & list1,const QStringList & list2){

  if (list2.empty())
    return QStringList(list1);

  if (list1.empty()  ||  list1 == list2)  // smo sort these?
    return QStringList();

  QStringList difference;
  register int size = list1.size();
  for (register int i = 0; i < size; ++i) {
    if (list2.indexOf(list1[i]) == -1)
      difference << list1[i];
  }
  return difference;
}
/******************************************************************************
 * ListWidget2List(): populate string list from list widget.
 ******************************************************************************/
void listWidget2List  (QListWidget *lw, QStringList *sl) {
  if (!lw || !sl)
    return;

  sl->clear();

  int count = lw->count();
  for (int i = 0; i < count; ++i) {
    QListWidgetItem * item = lw->item(i);
    QString data = (item->data(Qt::DisplayRole)).toString();
    *sl << data;
  }
}
/******************************************************************************
 * ms2units(): Convert milliseconds to common macro units w/ integer rounding.
 *             Form: 'n <units> [n <units>]'
 *
 * parms   : _ms - milliseconds (non-negative)
 *
 * returns : handy string like '1 min 2 sec', '24 sec', or '123 ms'
 ******************************************************************************/
QString ms2units  (quint64 _ms) {

  const quint64 sec   = Q_UINT64_C(1000);          // ms in sec
  const quint64 min   = Q_UINT64_C(60000);         // ms in min
  const quint64 hour  = Q_UINT64_C(3600000);       // ms in hour
  const quint64 day   = Q_UINT64_C(86400000);      // ms in day
  const quint64 month = Q_UINT64_C(2592000000);    // ms in month
  const quint64 year  = Q_UINT64_C(31104000000);   // ms in year

  char s[32];
  for (int i = 0; i < 32; s[i++]='\0') ;

  if (_ms < sec)
    sprintf (s, "%llu ms",              (ull)_ms);
  else if (_ms < min)
    sprintf (s, "%llu sec",             (ull)(_ms / sec));
  else if (_ms < hour)
    sprintf (s, "%llu min %llu sec",    (ull)(_ms / min),
                            (ull)((_ms % min)   / sec));
  else if (_ms < day)
    sprintf (s, "%llu hour %llu min",   (ull)(_ms / hour),
                            (ull)((_ms % hour)  / min));
  else if (_ms < month)
    sprintf (s, "%llu day %llu hour",   (ull)(_ms / day),
                            (ull)((_ms % day)   / hour));
  else if (_ms < year)
    sprintf (s, "%llu month %llu day",  (ull)(_ms / month),
                            (ull)((_ms % month) / day));
  else
    sprintf (s, "%llu year %llu month", (ull)(_ms / year),
                            (ull)((_ms % year)  / month));
  return QString(s);
}
/*******************************************************************************
* FUNCTION:  ParseDelim()
*
* PURPOSE:  Separates a string into fields by setting delimiters to null.
*
* INPUT:     str     - ptr to string to be parsed
*         delims  - ptr to string of possible delims
*
* OUTPUT:  flds    - buffer to hold parsed fields
*         n_found - fields parsed (optional)
*
* COMMENTS:  Allocates memory to hold both the pointers to the parsed fields, as
*         well as the input string itself.  This buffer is returned, and it is
*         the calling application's responsibilty to free the memory.  The
*         size of memory equals:
*
*        n_found * sizeof (char *) + strlen (str) + 1 - xmem
*
* RETURNS:  flds - buffer holding parsed fields
*******************************************************************************/
void* parseDelim  (const char* str, const char* delims, char*** flds,
             int* n_found) {

  int  fields  = 1,      // fields parsed
      xmem     = 0;      // accumulate extra mem from stripped chars

  /*------------------------------------------------------------------------*
    Inititialize pfix, optional number found parm; validate pointers; copy
    subject string into marching pointer; and save the start address.
   *------------------------------------------------------------------------*/

  n_found ? *n_found = 0 : 0;

  if (!(delims && *delims) || !flds || !(str && *str))
    return (0);

  char *  s,              // marching ptr
      *  start;            // sentinel ptr

  if (!(start = s = strdup (str))) {
    qDebug() << "strdup(" << str << ") failed";
    return (0);
  }

  /*------------------------------------------------------------------------*
    Remove trailing CR, LF and SP.
   *------------------------------------------------------------------------*/

  int  tmp;

  strip (s, string (SM_CR + SM_LF + SM_SP).c_str(), &tmp);

  xmem += tmp;

  /*------------------------------------------------------------------------*
    Parse.  The position pointer, s, points to the next segment to process.
   *------------------------------------------------------------------------*/

  bool  done = false;

  while (!done) {

    if ((s = strpbrk (s, delims))) {

      /*----------------------------------------------------------------*
          Strip trailing blanks from each field.
       *----------------------------------------------------------------*/

      char * p = s;

      if (p > start  &&  *(p - 1) == ' ') {

        while (p - 1 >= start  &&  *(p - 1) == ' ')
          p--;

        if (s - p) {

          xmem += s - p;

          s = (char *) memmove (p, s, strlen (s) + 1);
        }
      }

      *s++ = '\0';    // define field

      fields++;
    }

    else done = true;

  }  // while

  /*------------------------------------------------------------------------*
    Allocate the exact amount of memory needed.  The input string now has
    embedded nulls to delineate the fields.  Copy this string into the lower
    part of the allocated buffer.  Align the respective pointers upstream in
    the same buffer.  Return the entire buffer.
   *------------------------------------------------------------------------*/

  int  space;

  tmp = strlen (str) + 1 - xmem;

  if ((*flds = (char **) malloc (space = fields * sizeof (char *) + tmp))) {

    memset (*flds, '\0', space);

    memcpy (s = (char *) *flds + fields * sizeof (char *), start, tmp);

    for (int i = 0; i < fields; i++) {

      (*flds)[i] = s;

      s = strchr (s, '\0') + 1;
    }

    n_found ? *n_found = fields : 0;
  }
  else {

    *flds = (char **) 0;
  }

  free (start);

  return ((void *) *flds);

}  // ParseDelim()
/*******************************************************************************
* FUNCTION:     positionAtLine()
*
* PURPOSE:      Position file ptr to end of line equal to given string.
*
* INPUT:        _file - file
*               _s    - string
*
* RETURNS:      file ptr position
******************************************************************************/
qint64 positionAfterLine(QFile &_file, const QString &_string) {

    char buf[512];
    bool found = false;

    while (!_file.atEnd()) {
      memset(buf, '\0', sizeof(buf));
      _file.readLine(buf, sizeof(buf));
      QString s(buf);
      s = s.trimmed();
      // qDebug() << "s-" << s;
      if (s[0] == '#')
        continue;
      if ((found = s == _string))
        break;
    }
    if (!found)
      _file.write(("\n" + _string + "\n").toStdString().c_str());

    return _file.pos();
}
/*******************************************************************************
* FUNCTION:     strip()
*
* PURPOSE:      Strip trailing garbage from a string.
*
* INPUT:        ps - string
*               g  - garbage
*               n  - length change (optional)
*
* OUTPUT:       ps - stripped
*
* RETURNS:      stripped string address
******************************************************************************/
string* strip  (string* ps, const string& g, int* n) {

  if (!ps || g.empty())
    return (ps);

  // while the last element in the string is garbage, erase it

  int     n_ = 0;
  size_t  i;

  while ((i = (*ps).size() - 1) > 0  &&

        g.find_first_of (string (1, (*ps)[i])) != string::npos) {

    (*ps).erase (i);

    ++n_;
  }

  n ? *n = n_ : n_;

  return (ps);

}
char * strip  (char * pz, const char * g, int * n) {

  if (!(pz || *pz) || !(g || *g))
    return (pz);

      /*--------------------------------------------------------------------*
       (1) pass constructed string ptr and garbage ref to string version
       (2) dereference returned string ptr
       (3) call resultant string's member c_str() conversion function
       (4) copy returned buffer into original buffer (always exceeds
          or equals size of stripped buffer)
      *--------------------------------------------------------------------*/

  string s_(pz);

  return (strcpy (pz, (*strip (&s_, string (g), n)).c_str()));

}  // strip()
/******************************************************************************
 * uint2syntax(): convert uint to QRegExp::PatternSyntax without relying on the
 *                actual values of the enumerated type.  We are relying on the
 *                order of elements in the combobox.
 *
 * parms   : _index - position in syntaxComboBox
 *
 * returns : QRegExp::PatternSyntax
 ******************************************************************************/
QRegExp::PatternSyntax uint2syntax  (uint _index) {
  switch (_index) {
    case 0 : return QRegExp::RegExp;
    case 1 : return QRegExp::Wildcard;
    case 2 : return QRegExp::FixedString;
    case 3 : return QRegExp::RegExp2;
    default:
      Q_ASSERT(0);
      return QRegExp::RegExp;
  }
}
/******************************************************************************
 * units2bytes(): Convert B, KB, MB, GB, TB, PB to bytes.
 *
 * parms   : _units : string - 'N <units>' eg. 123 KB
 *
 * returns : -1 on error, otherwise bytes
 ******************************************************************************/
qint64 units2bytes  (const QString &_units) {

  if (_units.isEmpty() || _units.isNull())
    return (-1);

  float mag;
  char buf[_units.size()];  // more than big enough

  sscanf(_units.toStdString().c_str(), "%f %s", &mag, buf);
  QString s("hack");
  qd(4) << s;
  QString z = QString(QObject::tr("units2bytes(): mag- %1 units-'%2'")).arg(mag).arg(buf);
  qd(4) << z;
  //qd(4) << "units2bytes(): mag-" << mag << " units-'" << buf << "'";

  (void)_units;

  if (mag < 0)
    return (-1);

  QString units(buf);

  if (units == "B")
    return static_cast<qint64>(mag);
  if (units == "KB")
    return static_cast<qint64>(mag * KB);
  if (units == "MB")
    return static_cast<qint64>(mag * MB);
  if (units == "GB")
    return static_cast<qint64>(mag * GB);
  if (units == "TB")
    return static_cast<qint64>(mag * TB);
  if (units == "PB")
    return static_cast<qint64>(mag * PB);

  return -1;
}
/******************************************************************************
 * UpdateComboBox(): update a combobox's history.
 ******************************************************************************/
void updateComboBox  (QComboBox *comboBox) {
  if (comboBox->findText(comboBox->currentText()) == -1)
    comboBox->addItem(comboBox->currentText());
}
/******************************************************************************
 * ValidRx(): Validate a Qstring as a valid regular expression.
 ******************************************************************************/
bool validRx(const QString &s, QRegExp::PatternSyntax syntax)
{
   QRegExp rx(s);
   rx.setPatternSyntax(syntax);
   return rx.isValid();
}
/******************************************************************************
 * ValidRx(): Validate a QstringList as a valid regular expression.
 ******************************************************************************/
bool validRx(const QStringList &_sl, QRegExp::PatternSyntax _syntax)
{
    QList<QString>::const_iterator i = _sl.begin();
    while (i != _sl.end()) {
        if (!validRx(*(i++), _syntax))
            return false;
    }

    return true;
}
