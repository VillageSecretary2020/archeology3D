/********************************************************************************
** Form generated from reading UI file 'threeDdetectSystem.ui'
**
** Created by: Qt User Interface Compiler version 5.9.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_THREEDDETECTSYSTEM_H
#define UI_THREEDDETECTSYSTEM_H

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

class Ui_threeDdetectSystemClass
{
public:
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *threeDdetectSystemClass)
    {
        if (threeDdetectSystemClass->objectName().isEmpty())
            threeDdetectSystemClass->setObjectName(QStringLiteral("threeDdetectSystemClass"));
        threeDdetectSystemClass->resize(600, 400);
        centralWidget = new QWidget(threeDdetectSystemClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        threeDdetectSystemClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(threeDdetectSystemClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 23));
        threeDdetectSystemClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(threeDdetectSystemClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        threeDdetectSystemClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(threeDdetectSystemClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        threeDdetectSystemClass->setStatusBar(statusBar);

        retranslateUi(threeDdetectSystemClass);

        QMetaObject::connectSlotsByName(threeDdetectSystemClass);
    } // setupUi

    void retranslateUi(QMainWindow *threeDdetectSystemClass)
    {
        threeDdetectSystemClass->setWindowTitle(QApplication::translate("threeDdetectSystemClass", "threeDdetectSystem", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class threeDdetectSystemClass: public Ui_threeDdetectSystemClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_THREEDDETECTSYSTEM_H
