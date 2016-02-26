/********************************************************************************
** Form generated from reading UI file 'F_ChoisirSonde.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_F_CHOISIRSONDE_H
#define UI_F_CHOISIRSONDE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_F_ChoisirSonde
{
public:

    void setupUi(QWidget *F_ChoisirSonde)
    {
        if (F_ChoisirSonde->objectName().isEmpty())
            F_ChoisirSonde->setObjectName(QStringLiteral("F_ChoisirSonde"));
        F_ChoisirSonde->resize(400, 300);

        retranslateUi(F_ChoisirSonde);

        QMetaObject::connectSlotsByName(F_ChoisirSonde);
    } // setupUi

    void retranslateUi(QWidget *F_ChoisirSonde)
    {
        F_ChoisirSonde->setWindowTitle(QApplication::translate("F_ChoisirSonde", "Form", 0));
    } // retranslateUi

};

namespace Ui {
    class F_ChoisirSonde: public Ui_F_ChoisirSonde {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_F_CHOISIRSONDE_H
