/******************************************************************************
 * NAME:      about.h
 *
 * PURPOSE:   Class definition for about widget.
 *
 * DATE:      January 21, 2010
 *
 * REVISIONS:
 * ---------
 * [012110] shawno- initial coding
*******************************************************************************/
#ifndef ABOUT_H
#define ABOUT_H

#include <QtGlobal>
#include <QtGui/QDialog>

namespace Ui {
	class About;
}

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

class About : public QDialog {
	 Q_OBJECT
	 Q_DISABLE_COPY(About)
public:
	 explicit About(QWidget *parent = 0);
	 virtual ~About();
	 Ui::About *ui();

protected:
	 virtual void changeEvent(QEvent *e);

private:
	 Ui::About *m_ui;
};

#endif // ABOUT_H
