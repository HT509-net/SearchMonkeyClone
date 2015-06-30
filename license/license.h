/******************************************************************************
 * NAME:      license.h
 *
 * PURPOSE:   Class definition for license widget.
 *
 * DATE:      May 11, 2010
 *
 * REVISIONS:
 * ---------
 * [012110] shawno- initial coding
*******************************************************************************/
#ifndef LICENSE_H
#define LICENSE_H

#include <QtGui/QDialog>

namespace Ui {
	 class License;
}

class License : public QDialog {
    Q_OBJECT
public:
	 License(QWidget *parent = 0);
	 ~License();

protected:
    void changeEvent(QEvent *e);

private:
	 Ui::License *m_ui;
};

#endif // LICENSE_H
