/********************************************************************************
** Form generated from reading UI file 'CUiTop.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CUITOP_H
#define UI_CUITOP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CUiTop
{
public:
    QHBoxLayout *horizontalLayout;
    QLabel *label_logo;
    QSpacerItem *horizontalSpacer;
    QPushButton *btn_changeSkin;

    void setupUi(QWidget *CUiTop)
    {
        if (CUiTop->objectName().isEmpty())
            CUiTop->setObjectName(QStringLiteral("CUiTop"));
        CUiTop->resize(729, 28);
        CUiTop->setMinimumSize(QSize(0, 28));
        CUiTop->setMaximumSize(QSize(16777215, 28));
        CUiTop->setStyleSheet(QStringLiteral(""));
        horizontalLayout = new QHBoxLayout(CUiTop);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(5, 5, 5, 0);
        label_logo = new QLabel(CUiTop);
        label_logo->setObjectName(QStringLiteral("label_logo"));
        label_logo->setMinimumSize(QSize(115, 0));
        label_logo->setMaximumSize(QSize(115, 16777215));

        horizontalLayout->addWidget(label_logo);

        horizontalSpacer = new QSpacerItem(550, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btn_changeSkin = new QPushButton(CUiTop);
        btn_changeSkin->setObjectName(QStringLiteral("btn_changeSkin"));
        btn_changeSkin->setMinimumSize(QSize(23, 23));
        btn_changeSkin->setMaximumSize(QSize(23, 23));

        horizontalLayout->addWidget(btn_changeSkin);


        retranslateUi(CUiTop);

        QMetaObject::connectSlotsByName(CUiTop);
    } // setupUi

    void retranslateUi(QWidget *CUiTop)
    {
        CUiTop->setWindowTitle(QApplication::translate("CUiTop", "CUiTop", nullptr));
        label_logo->setText(QString());
        btn_changeSkin->setText(QApplication::translate("CUiTop", "skin", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CUiTop: public Ui_CUiTop {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CUITOP_H
