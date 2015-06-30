/******************************************************************************
 * NAME:      settings.h
 *
 * PURPOSE:   Class definition for settings widget.
 *
 * DATE:      December 7, 2009
 *
 * REVISIONS:
 * ---------
 * [120709] adamc- initial coding
*******************************************************************************/
#ifndef SETTINGS_H
#define SETTINGS_H

#include <limits.h>

#include <QtGlobal>
#include <QtGui/QDialog>

const quint32 GEN_TAB    = 1U << 0;  // General tab of Settings
const quint32 GEN_IGNORE = 1U << 1;  // ...
const quint32 GEN_CSTYLE = 1U << 2;  // ...
const quint32 GEN_MASK = UINT_MAX;

const qint64 PRESET_RECLEN =  80;  // size of preset "records"
const qint64 DIRNAM_RECLEN = 240;  // size of preset "records"

namespace Ui {
    class Settings;
}

QT_BEGIN_NAMESPACE
class QAbstractButton;
QT_END_NAMESPACE

class Dir;
class Preset;

class Settings : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(Settings)
public:
    explicit Settings(QWidget *parent = 0);
    virtual ~Settings();
	 void createConfigFile();
	 inline Dir *getDir() { return m_dir; }
	 inline Preset *getPreset() { return m_preset; }
	 void reset();
	 void saveInCaseCancel();
	 inline void setPreset(Preset *_preset) { m_preset = _preset; }
	 Ui::Settings *ui();

protected:
    virtual void changeEvent(QEvent *e);

private slots:
	void on_addPushButton_clicked();
	void on_buttonBox_clicked(QAbstractButton* button);
	void on_defaultPushButton_clicked();
	void on_deletePushButton_clicked();
	void on_directoriesButtonBox_clicked(QAbstractButton* button);
	void on_generalButtonBox_clicked(QAbstractButton* button);
	void on_leftAllPushButton_clicked();
	void on_leftPushButton_clicked();
	void on_moreLineEdit_textEdited(QString);
	void on_rightAllPushButton_clicked();
	void on_rightPushButton_clicked();
	void on_searchAllDirsCheckBox_toggled(bool checked);
	void on_setTypePushButton_clicked();

private:
	 quint32 m_general;
	 Dir *m_dir;
	 Preset *m_preset;
    Ui::Settings *m_ui;
};

// handy helper class to reset Settings when a function leaves scope
class autoSettingsReset {
public:
	autoSettingsReset(Settings *settings) { m_settings = settings; }
	~autoSettingsReset() { m_settings->reset(); }
private:
	Settings *m_settings;
};

#endif // SETTINGS_H
