/********************************************************************************
** Form generated from reading UI file 'settings.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGS_H
#define UI_SETTINGS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Settings
{
public:
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *tab;
    QCheckBox *tabCheckBox;
    QCheckBox *ignoreCheckBox;
    QCheckBox *cStyleCheckBox;
    QDialogButtonBox *generalButtonBox;
    QWidget *tab_2;
    QComboBox *targetComboBox;
    QLabel *targetLabel;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QRadioButton *skipRadioButton;
    QRadioButton *onlyRadioButton;
    QRadioButton *alsoRadioButton;
    QLabel *moreEgLabel;
    QLabel *moreLabel;
    QLineEdit *moreLineEdit;
    QFrame *line_2;
    QLabel *typenameLabel;
    QLineEdit *typenameLineEdit;
    QLabel *extensionsLabel;
    QLineEdit *extensionsLineEdit;
    QLabel *extensionsEgLabel;
    QPushButton *setTypePushButton;
    QDialogButtonBox *buttonBox;
    QWidget *tab_4;
    QListWidget *includeListWidget;
    QLabel *includeLabel;
    QLabel *excludeLabel;
    QListWidget *excludeListWidget;
    QCheckBox *searchAllDirsCheckBox;
    QPushButton *defaultPushButton;
    QPushButton *rightPushButton;
    QPushButton *leftAllPushButton;
    QPushButton *rightAllPushButton;
    QPushButton *leftPushButton;
    QDialogButtonBox *directoriesButtonBox;
    QPushButton *deletePushButton;
    QPushButton *addPushButton;
    QButtonGroup *buttonGroup;

    void setupUi(QDialog *Settings)
    {
        if (Settings->objectName().isEmpty())
            Settings->setObjectName(QStringLiteral("Settings"));
        Settings->resize(534, 415);
        Settings->setSizeGripEnabled(true);
        Settings->setModal(true);
        verticalLayout = new QVBoxLayout(Settings);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        tabWidget = new QTabWidget(Settings);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        tabCheckBox = new QCheckBox(tab);
        tabCheckBox->setObjectName(QStringLiteral("tabCheckBox"));
        tabCheckBox->setGeometry(QRect(70, 60, 251, 18));
        ignoreCheckBox = new QCheckBox(tab);
        ignoreCheckBox->setObjectName(QStringLiteral("ignoreCheckBox"));
        ignoreCheckBox->setGeometry(QRect(70, 90, 141, 18));
        cStyleCheckBox = new QCheckBox(tab);
        cStyleCheckBox->setObjectName(QStringLiteral("cStyleCheckBox"));
        cStyleCheckBox->setGeometry(QRect(70, 120, 231, 18));
        generalButtonBox = new QDialogButtonBox(tab);
        generalButtonBox->setObjectName(QStringLiteral("generalButtonBox"));
        generalButtonBox->setGeometry(QRect(330, 330, 156, 23));
        generalButtonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        targetComboBox = new QComboBox(tab_2);
        targetComboBox->setObjectName(QStringLiteral("targetComboBox"));
        targetComboBox->setGeometry(QRect(64, 13, 421, 20));
        targetComboBox->setEditable(false);
        targetLabel = new QLabel(tab_2);
        targetLabel->setObjectName(QStringLiteral("targetLabel"));
        targetLabel->setGeometry(QRect(23, 13, 31, 16));
        targetLabel->setMaximumSize(QSize(232, 16777215));
        layoutWidget = new QWidget(tab_2);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(60, 40, 421, 20));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        skipRadioButton = new QRadioButton(tab_2);
        buttonGroup = new QButtonGroup(Settings);
        buttonGroup->setObjectName(QStringLiteral("buttonGroup"));
        buttonGroup->addButton(skipRadioButton);
        skipRadioButton->setObjectName(QStringLiteral("skipRadioButton"));
        skipRadioButton->setGeometry(QRect(350, 40, 136, 18));
        onlyRadioButton = new QRadioButton(tab_2);
        buttonGroup->addButton(onlyRadioButton);
        onlyRadioButton->setObjectName(QStringLiteral("onlyRadioButton"));
        onlyRadioButton->setGeometry(QRect(209, 40, 135, 18));
        alsoRadioButton = new QRadioButton(tab_2);
        buttonGroup->addButton(alsoRadioButton);
        alsoRadioButton->setObjectName(QStringLiteral("alsoRadioButton"));
        alsoRadioButton->setGeometry(QRect(67, 40, 136, 18));
        moreEgLabel = new QLabel(tab_2);
        moreEgLabel->setObjectName(QStringLiteral("moreEgLabel"));
        moreEgLabel->setGeometry(QRect(60, 80, 191, 20));
        moreLabel = new QLabel(tab_2);
        moreLabel->setObjectName(QStringLiteral("moreLabel"));
        moreLabel->setGeometry(QRect(20, 100, 46, 14));
        moreLineEdit = new QLineEdit(tab_2);
        moreLineEdit->setObjectName(QStringLiteral("moreLineEdit"));
        moreLineEdit->setGeometry(QRect(60, 100, 421, 20));
        line_2 = new QFrame(tab_2);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setGeometry(QRect(-3, 150, 521, 20));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);
        typenameLabel = new QLabel(tab_2);
        typenameLabel->setObjectName(QStringLiteral("typenameLabel"));
        typenameLabel->setGeometry(QRect(20, 180, 51, 16));
        typenameLineEdit = new QLineEdit(tab_2);
        typenameLineEdit->setObjectName(QStringLiteral("typenameLineEdit"));
        typenameLineEdit->setGeometry(QRect(80, 180, 113, 20));
        extensionsLabel = new QLabel(tab_2);
        extensionsLabel->setObjectName(QStringLiteral("extensionsLabel"));
        extensionsLabel->setGeometry(QRect(20, 240, 61, 16));
        extensionsLineEdit = new QLineEdit(tab_2);
        extensionsLineEdit->setObjectName(QStringLiteral("extensionsLineEdit"));
        extensionsLineEdit->setGeometry(QRect(80, 240, 401, 20));
        extensionsEgLabel = new QLabel(tab_2);
        extensionsEgLabel->setObjectName(QStringLiteral("extensionsEgLabel"));
        extensionsEgLabel->setGeometry(QRect(80, 220, 81, 16));
        setTypePushButton = new QPushButton(tab_2);
        setTypePushButton->setObjectName(QStringLiteral("setTypePushButton"));
        setTypePushButton->setGeometry(QRect(80, 290, 75, 23));
        buttonBox = new QDialogButtonBox(tab_2);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(265, 340, 241, 25));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Apply|QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        tabWidget->addTab(tab_2, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QStringLiteral("tab_4"));
        includeListWidget = new QListWidget(tab_4);
        includeListWidget->setObjectName(QStringLiteral("includeListWidget"));
        includeListWidget->setGeometry(QRect(20, 30, 161, 301));
        includeListWidget->setSortingEnabled(true);
        includeLabel = new QLabel(tab_4);
        includeLabel->setObjectName(QStringLiteral("includeLabel"));
        includeLabel->setGeometry(QRect(80, 10, 46, 14));
        excludeLabel = new QLabel(tab_4);
        excludeLabel->setObjectName(QStringLiteral("excludeLabel"));
        excludeLabel->setGeometry(QRect(290, 10, 46, 14));
        excludeListWidget = new QListWidget(tab_4);
        excludeListWidget->setObjectName(QStringLiteral("excludeListWidget"));
        excludeListWidget->setGeometry(QRect(230, 30, 161, 301));
        excludeListWidget->setSortingEnabled(true);
        searchAllDirsCheckBox = new QCheckBox(tab_4);
        searchAllDirsCheckBox->setObjectName(QStringLiteral("searchAllDirsCheckBox"));
        searchAllDirsCheckBox->setGeometry(QRect(20, 340, 121, 18));
        defaultPushButton = new QPushButton(tab_4);
        defaultPushButton->setObjectName(QStringLiteral("defaultPushButton"));
        defaultPushButton->setGeometry(QRect(420, 240, 61, 23));
        rightPushButton = new QPushButton(tab_4);
        rightPushButton->setObjectName(QStringLiteral("rightPushButton"));
        rightPushButton->setGeometry(QRect(190, 130, 31, 23));
        leftAllPushButton = new QPushButton(tab_4);
        leftAllPushButton->setObjectName(QStringLiteral("leftAllPushButton"));
        leftAllPushButton->setGeometry(QRect(190, 300, 31, 23));
        rightAllPushButton = new QPushButton(tab_4);
        rightAllPushButton->setObjectName(QStringLiteral("rightAllPushButton"));
        rightAllPushButton->setGeometry(QRect(190, 270, 31, 23));
        leftPushButton = new QPushButton(tab_4);
        leftPushButton->setObjectName(QStringLiteral("leftPushButton"));
        leftPushButton->setGeometry(QRect(190, 160, 31, 23));
        directoriesButtonBox = new QDialogButtonBox(tab_4);
        directoriesButtonBox->setObjectName(QStringLiteral("directoriesButtonBox"));
        directoriesButtonBox->setGeometry(QRect(310, 340, 191, 25));
        directoriesButtonBox->setOrientation(Qt::Horizontal);
        directoriesButtonBox->setStandardButtons(QDialogButtonBox::Apply|QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        deletePushButton = new QPushButton(tab_4);
        deletePushButton->setObjectName(QStringLiteral("deletePushButton"));
        deletePushButton->setGeometry(QRect(420, 60, 61, 23));
        addPushButton = new QPushButton(tab_4);
        addPushButton->setObjectName(QStringLiteral("addPushButton"));
        addPushButton->setGeometry(QRect(420, 30, 61, 23));
        tabWidget->addTab(tab_4, QString());

        verticalLayout->addWidget(tabWidget);


        retranslateUi(Settings);
        QObject::connect(buttonBox, SIGNAL(accepted()), Settings, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), Settings, SLOT(reject()));

        tabWidget->setCurrentIndex(1);
        targetComboBox->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(Settings);
    } // setupUi

    void retranslateUi(QDialog *Settings)
    {
        Settings->setWindowTitle(QApplication::translate("Settings", "Settings", 0));
        tabCheckBox->setText(QApplication::translate("Settings", "Start new tab for search", 0));
        ignoreCheckBox->setText(QApplication::translate("Settings", "Ignore file open errors", 0));
        cStyleCheckBox->setText(QApplication::translate("Settings", "Only search C-style function names", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("Settings", "General", 0));
        targetLabel->setText(QApplication::translate("Settings", "Target", 0));
        skipRadioButton->setText(QApplication::translate("Settings", "Skip", 0));
        onlyRadioButton->setText(QApplication::translate("Settings", "Only", 0));
        alsoRadioButton->setText(QApplication::translate("Settings", "Also", 0));
        moreEgLabel->setText(QApplication::translate("Settings", "eg. -also type1 -also type2 -skip type3", 0));
        moreLabel->setText(QApplication::translate("Settings", "More", 0));
        typenameLabel->setText(QApplication::translate("Settings", "Typename", 0));
        extensionsLabel->setText(QApplication::translate("Settings", "Extensions", 0));
        extensionsEgLabel->setText(QApplication::translate("Settings", "eg. bat com exe", 0));
        setTypePushButton->setText(QApplication::translate("Settings", "Set type", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("Settings", "Presets", 0));
        includeLabel->setText(QApplication::translate("Settings", "Include", 0));
        excludeLabel->setText(QApplication::translate("Settings", "Exclude", 0));
        searchAllDirsCheckBox->setText(QApplication::translate("Settings", "Search all directories", 0));
        defaultPushButton->setText(QApplication::translate("Settings", "Default", 0));
        rightPushButton->setText(QApplication::translate("Settings", "->", 0));
        leftAllPushButton->setText(QApplication::translate("Settings", "<<", 0));
        rightAllPushButton->setText(QApplication::translate("Settings", ">>", 0));
        leftPushButton->setText(QApplication::translate("Settings", "<-", 0));
        deletePushButton->setText(QApplication::translate("Settings", "Delete", 0));
        addPushButton->setText(QApplication::translate("Settings", "Add", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("Settings", "Directories", 0));
    } // retranslateUi

};

namespace Ui {
    class Settings: public Ui_Settings {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGS_H
