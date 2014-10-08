/********************************************************************************
** Form generated from reading UI file 'climdisplay.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLIMDISPLAY_H
#define UI_CLIMDISPLAY_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CLimDisplayClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *CLimDisplayClass)
    {
        if (CLimDisplayClass->objectName().isEmpty())
            CLimDisplayClass->setObjectName(QStringLiteral("CLimDisplayClass"));
        CLimDisplayClass->resize(600, 400);
        menuBar = new QMenuBar(CLimDisplayClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        CLimDisplayClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(CLimDisplayClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        CLimDisplayClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(CLimDisplayClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        CLimDisplayClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(CLimDisplayClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        CLimDisplayClass->setStatusBar(statusBar);

        retranslateUi(CLimDisplayClass);

        QMetaObject::connectSlotsByName(CLimDisplayClass);
    } // setupUi

    void retranslateUi(QMainWindow *CLimDisplayClass)
    {
        CLimDisplayClass->setWindowTitle(QApplication::translate("CLimDisplayClass", "CLimDisplay", 0));
    } // retranslateUi

};

namespace Ui {
    class CLimDisplayClass: public Ui_CLimDisplayClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLIMDISPLAY_H
