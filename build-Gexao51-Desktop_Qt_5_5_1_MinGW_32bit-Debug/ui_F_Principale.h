/********************************************************************************
** Form generated from reading UI file 'F_Principale.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_F_PRINCIPALE_H
#define UI_F_PRINCIPALE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_F_Principale
{
public:
    QWidget *centralwidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *F_Principale)
    {
        if (F_Principale->objectName().isEmpty())
            F_Principale->setObjectName(QStringLiteral("F_Principale"));
        F_Principale->resize(800, 364);
        centralwidget = new QWidget(F_Principale);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        F_Principale->setCentralWidget(centralwidget);
        menubar = new QMenuBar(F_Principale);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 21));
        F_Principale->setMenuBar(menubar);
        statusbar = new QStatusBar(F_Principale);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        F_Principale->setStatusBar(statusbar);

        retranslateUi(F_Principale);

        QMetaObject::connectSlotsByName(F_Principale);
    } // setupUi

    void retranslateUi(QMainWindow *F_Principale)
    {
        F_Principale->setWindowTitle(QApplication::translate("F_Principale", "MainWindow", 0));
    } // retranslateUi

};

namespace Ui {
    class F_Principale: public Ui_F_Principale {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_F_PRINCIPALE_H
