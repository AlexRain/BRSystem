/********************************************************************************
** Form generated from reading UI file 'UiLogin.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UILOGIN_H
#define UI_UILOGIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>

QT_BEGIN_NAMESPACE

class Ui_UiLogin
{
public:

    void setupUi(QDialog *UiLogin)
    {
        if (UiLogin->objectName().isEmpty())
            UiLogin->setObjectName(QStringLiteral("UiLogin"));
        UiLogin->resize(431, 285);

        retranslateUi(UiLogin);

        QMetaObject::connectSlotsByName(UiLogin);
    } // setupUi

    void retranslateUi(QDialog *UiLogin)
    {
        UiLogin->setWindowTitle(QApplication::translate("UiLogin", "UiLogin", nullptr));
    } // retranslateUi

};

namespace Ui {
    class UiLogin: public Ui_UiLogin {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UILOGIN_H
