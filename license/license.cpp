/******************************************************************************
 * NAME:      license.cpp
 *
 * PURPOSE:   Implementation of license widget.
 *
 * DATE:      May 11, 2010
 *
 * REVISIONS:
 * ---------
 * [012110] shawno- initial coding
*******************************************************************************/
#include "license.h"
#include "ui_license.h"

License::License(QWidget *parent) :
    QDialog(parent),
	 m_ui(new Ui::License)
{
    m_ui->setupUi(this);
}
License::~License()
{
    delete m_ui;
}
void License::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
