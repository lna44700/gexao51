/********************************************************************************
** Form generated from reading UI file 'F_Sonde.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_F_SONDE_H
#define UI_F_SONDE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_F_Sonde
{
public:

    void setupUi(QWidget *F_Sonde)
    {
        if (F_Sonde->objectName().isEmpty())
            F_Sonde->setObjectName(QStringLiteral("F_Sonde"));
        F_Sonde->resize(400, 300);

        retranslateUi(F_Sonde);

        QMetaObject::connectSlotsByName(F_Sonde);
    } // setupUi

    void retranslateUi(QWidget *F_Sonde)
    {
        F_Sonde->setWindowTitle(QApplication::translate("F_Sonde", "Form", 0));
    } // retranslateUi

};

namespace Ui {
    class F_Sonde: public Ui_F_Sonde {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_F_SONDE_H
