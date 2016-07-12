/********************************************************************************
** Form generated from reading UI file 'app.ui'
**
** Created: Sun 10. Jul 09:54:57 2016
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_APP_H
#define UI_APP_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QTextEdit>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <qcustomplot/qcustomplot.h>

QT_BEGIN_NAMESPACE

class Ui_App
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QTextEdit *tParser;
    QCustomPlot *customPlot;
    QTextEdit *tOutput;
    QHBoxLayout *horizontalLayout;
    QLineEdit *eLogFile;
    QPushButton *bReload;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *App)
    {
        if (App->objectName().isEmpty())
            App->setObjectName(QString::fromUtf8("App"));
        App->resize(1071, 700);
        centralWidget = new QWidget(App);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tParser = new QTextEdit(centralWidget);
        tParser->setObjectName(QString::fromUtf8("tParser"));

        verticalLayout->addWidget(tParser);

        customPlot = new QCustomPlot(centralWidget);
        customPlot->setObjectName(QString::fromUtf8("customPlot"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(customPlot->sizePolicy().hasHeightForWidth());
        customPlot->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(customPlot);

        tOutput = new QTextEdit(centralWidget);
        tOutput->setObjectName(QString::fromUtf8("tOutput"));

        verticalLayout->addWidget(tOutput);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        eLogFile = new QLineEdit(centralWidget);
        eLogFile->setObjectName(QString::fromUtf8("eLogFile"));

        horizontalLayout->addWidget(eLogFile);

        bReload = new QPushButton(centralWidget);
        bReload->setObjectName(QString::fromUtf8("bReload"));

        horizontalLayout->addWidget(bReload);


        verticalLayout->addLayout(horizontalLayout);

        App->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(App);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1071, 21));
        App->setMenuBar(menuBar);
        mainToolBar = new QToolBar(App);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        App->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(App);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        App->setStatusBar(statusBar);

        retranslateUi(App);

        QMetaObject::connectSlotsByName(App);
    } // setupUi

    void retranslateUi(QMainWindow *App)
    {
        App->setWindowTitle(QApplication::translate("App", "App", 0, QApplication::UnicodeUTF8));
        bReload->setText(QApplication::translate("App", "Reload", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class App: public Ui_App {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_APP_H
