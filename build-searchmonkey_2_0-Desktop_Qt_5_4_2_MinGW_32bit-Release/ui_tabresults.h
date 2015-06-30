/********************************************************************************
** Form generated from reading UI file 'tabresults.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TABRESULTS_H
#define UI_TABRESULTS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TabResults
{
public:
    QGridLayout *gridLayout;
    QStackedWidget *resultsStackedWidget;
    QWidget *page_0;
    QGridLayout *gridLayout_3;
    QGridLayout *gridLayout_2;
    QTreeView *proxyViewH;
    QTextEdit *lineDisplayH;
    QWidget *page_1;
    QHBoxLayout *horizontalLayout_2;
    QGridLayout *gridLayout_5;
    QTreeView *proxyViewV;
    QTextEdit *lineDisplayV;
    QPushButton *layoutPushButton;

    void setupUi(QWidget *TabResults)
    {
        if (TabResults->objectName().isEmpty())
            TabResults->setObjectName(QStringLiteral("TabResults"));
        TabResults->resize(1128, 498);
        gridLayout = new QGridLayout(TabResults);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        resultsStackedWidget = new QStackedWidget(TabResults);
        resultsStackedWidget->setObjectName(QStringLiteral("resultsStackedWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(resultsStackedWidget->sizePolicy().hasHeightForWidth());
        resultsStackedWidget->setSizePolicy(sizePolicy);
        page_0 = new QWidget();
        page_0->setObjectName(QStringLiteral("page_0"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(99);
        sizePolicy1.setVerticalStretch(99);
        sizePolicy1.setHeightForWidth(page_0->sizePolicy().hasHeightForWidth());
        page_0->setSizePolicy(sizePolicy1);
        page_0->setMaximumSize(QSize(16777215, 16777215));
        gridLayout_3 = new QGridLayout(page_0);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        proxyViewH = new QTreeView(page_0);
        proxyViewH->setObjectName(QStringLiteral("proxyViewH"));
        sizePolicy1.setHeightForWidth(proxyViewH->sizePolicy().hasHeightForWidth());
        proxyViewH->setSizePolicy(sizePolicy1);
        proxyViewH->setMinimumSize(QSize(0, 0));
        proxyViewH->setMaximumSize(QSize(16777215, 16777215));

        gridLayout_2->addWidget(proxyViewH, 0, 0, 1, 1);

        lineDisplayH = new QTextEdit(page_0);
        lineDisplayH->setObjectName(QStringLiteral("lineDisplayH"));
        sizePolicy1.setHeightForWidth(lineDisplayH->sizePolicy().hasHeightForWidth());
        lineDisplayH->setSizePolicy(sizePolicy1);
        lineDisplayH->setMinimumSize(QSize(0, 0));
        lineDisplayH->setMaximumSize(QSize(16777215, 16777215));

        gridLayout_2->addWidget(lineDisplayH, 0, 1, 1, 1);

        gridLayout_2->setColumnStretch(0, 99);
        gridLayout_2->setColumnStretch(1, 99);

        gridLayout_3->addLayout(gridLayout_2, 0, 0, 1, 1);

        resultsStackedWidget->addWidget(page_0);
        page_1 = new QWidget();
        page_1->setObjectName(QStringLiteral("page_1"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(99);
        sizePolicy2.setVerticalStretch(99);
        sizePolicy2.setHeightForWidth(page_1->sizePolicy().hasHeightForWidth());
        page_1->setSizePolicy(sizePolicy2);
        horizontalLayout_2 = new QHBoxLayout(page_1);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        gridLayout_5 = new QGridLayout();
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        proxyViewV = new QTreeView(page_1);
        proxyViewV->setObjectName(QStringLiteral("proxyViewV"));

        gridLayout_5->addWidget(proxyViewV, 0, 0, 1, 1);

        lineDisplayV = new QTextEdit(page_1);
        lineDisplayV->setObjectName(QStringLiteral("lineDisplayV"));

        gridLayout_5->addWidget(lineDisplayV, 1, 0, 1, 1);


        horizontalLayout_2->addLayout(gridLayout_5);

        resultsStackedWidget->addWidget(page_1);

        gridLayout->addWidget(resultsStackedWidget, 0, 1, 2, 1);

        layoutPushButton = new QPushButton(TabResults);
        layoutPushButton->setObjectName(QStringLiteral("layoutPushButton"));

        gridLayout->addWidget(layoutPushButton, 1, 0, 1, 1);


        retranslateUi(TabResults);

        resultsStackedWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(TabResults);
    } // setupUi

    void retranslateUi(QWidget *TabResults)
    {
        TabResults->setWindowTitle(QApplication::translate("TabResults", "Form", 0));
        layoutPushButton->setText(QApplication::translate("TabResults", "&Horizontal", 0));
    } // retranslateUi

};

namespace Ui {
    class TabResults: public Ui_TabResults {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TABRESULTS_H
