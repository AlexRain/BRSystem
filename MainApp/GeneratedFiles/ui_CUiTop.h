/********************************************************************************
** Form generated from reading UI file 'CUiTop.ui'
**
** Created by: Qt User Interface Compiler version 5.12.6
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
#include "CLabel.h"

QT_BEGIN_NAMESPACE

class Ui_CUiTop
{
public:
    QHBoxLayout *horizontalLayout;
    CLabel *label_logo;
    QPushButton *btn_qq_manage;
    QPushButton *btn_phone_manage;
    QSpacerItem *horizontalSpacer;
    QPushButton *btn_login_register;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_head;
    CLabel *label_userName;
    QPushButton *btn_changeSkin;
    QPushButton *btn_setting;
    QLabel *divider;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *btn_min;
    QPushButton *btn_max;
    QPushButton *btn_close;

    void setupUi(QWidget *CUiTop)
    {
        if (CUiTop->objectName().isEmpty())
            CUiTop->setObjectName(QString::fromUtf8("CUiTop"));
        CUiTop->resize(759, 38);
        CUiTop->setStyleSheet(QString::fromUtf8(""));
        horizontalLayout = new QHBoxLayout(CUiTop);
        horizontalLayout->setSpacing(23);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(15, 5, 5, 0);
        label_logo = new CLabel(CUiTop);
        label_logo->setObjectName(QString::fromUtf8("label_logo"));
        label_logo->setCursor(QCursor(Qt::PointingHandCursor));

        horizontalLayout->addWidget(label_logo);

        btn_qq_manage = new QPushButton(CUiTop);
        btn_qq_manage->setObjectName(QString::fromUtf8("btn_qq_manage"));

        horizontalLayout->addWidget(btn_qq_manage);

        btn_phone_manage = new QPushButton(CUiTop);
        btn_phone_manage->setObjectName(QString::fromUtf8("btn_phone_manage"));

        horizontalLayout->addWidget(btn_phone_manage);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btn_login_register = new QPushButton(CUiTop);
        btn_login_register->setObjectName(QString::fromUtf8("btn_login_register"));

        horizontalLayout->addWidget(btn_login_register);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(10);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(-1, -1, 0, -1);
        label_head = new QLabel(CUiTop);
        label_head->setObjectName(QString::fromUtf8("label_head"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_head->sizePolicy().hasHeightForWidth());
        label_head->setSizePolicy(sizePolicy);
        label_head->setMinimumSize(QSize(30, 30));
        label_head->setMaximumSize(QSize(30, 30));

        horizontalLayout_4->addWidget(label_head);

        label_userName = new CLabel(CUiTop);
        label_userName->setObjectName(QString::fromUtf8("label_userName"));
        label_userName->setCursor(QCursor(Qt::PointingHandCursor));

        horizontalLayout_4->addWidget(label_userName);


        horizontalLayout->addLayout(horizontalLayout_4);

        btn_changeSkin = new QPushButton(CUiTop);
        btn_changeSkin->setObjectName(QString::fromUtf8("btn_changeSkin"));
        sizePolicy.setHeightForWidth(btn_changeSkin->sizePolicy().hasHeightForWidth());
        btn_changeSkin->setSizePolicy(sizePolicy);
        btn_changeSkin->setMinimumSize(QSize(23, 23));
        btn_changeSkin->setMaximumSize(QSize(23, 23));
        btn_changeSkin->setCursor(QCursor(Qt::PointingHandCursor));

        horizontalLayout->addWidget(btn_changeSkin);

        btn_setting = new QPushButton(CUiTop);
        btn_setting->setObjectName(QString::fromUtf8("btn_setting"));
        sizePolicy.setHeightForWidth(btn_setting->sizePolicy().hasHeightForWidth());
        btn_setting->setSizePolicy(sizePolicy);
        btn_setting->setMinimumSize(QSize(18, 18));
        btn_setting->setMaximumSize(QSize(18, 18));
        btn_setting->setCursor(QCursor(Qt::PointingHandCursor));

        horizontalLayout->addWidget(btn_setting);

        divider = new QLabel(CUiTop);
        divider->setObjectName(QString::fromUtf8("divider"));
        divider->setMinimumSize(QSize(1, 23));
        divider->setMaximumSize(QSize(1, 23));

        horizontalLayout->addWidget(divider);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(-1, -1, 0, -1);
        btn_min = new QPushButton(CUiTop);
        btn_min->setObjectName(QString::fromUtf8("btn_min"));
        sizePolicy.setHeightForWidth(btn_min->sizePolicy().hasHeightForWidth());
        btn_min->setSizePolicy(sizePolicy);
        btn_min->setMinimumSize(QSize(20, 20));
        btn_min->setMaximumSize(QSize(20, 20));
        btn_min->setCursor(QCursor(Qt::PointingHandCursor));

        horizontalLayout_2->addWidget(btn_min);

        btn_max = new QPushButton(CUiTop);
        btn_max->setObjectName(QString::fromUtf8("btn_max"));
        sizePolicy.setHeightForWidth(btn_max->sizePolicy().hasHeightForWidth());
        btn_max->setSizePolicy(sizePolicy);
        btn_max->setMinimumSize(QSize(20, 20));
        btn_max->setMaximumSize(QSize(20, 20));
        btn_max->setCursor(QCursor(Qt::PointingHandCursor));

        horizontalLayout_2->addWidget(btn_max);

        btn_close = new QPushButton(CUiTop);
        btn_close->setObjectName(QString::fromUtf8("btn_close"));
        sizePolicy.setHeightForWidth(btn_close->sizePolicy().hasHeightForWidth());
        btn_close->setSizePolicy(sizePolicy);
        btn_close->setMinimumSize(QSize(20, 20));
        btn_close->setMaximumSize(QSize(20, 20));
        btn_close->setCursor(QCursor(Qt::PointingHandCursor));

        horizontalLayout_2->addWidget(btn_close);


        horizontalLayout->addLayout(horizontalLayout_2);


        retranslateUi(CUiTop);

        QMetaObject::connectSlotsByName(CUiTop);
    } // setupUi

    void retranslateUi(QWidget *CUiTop)
    {
        CUiTop->setWindowTitle(QApplication::translate("CUiTop", "CUiTop", nullptr));
        label_logo->setText(QString());
        btn_qq_manage->setText(QApplication::translate("CUiTop", "qq manage", nullptr));
        btn_phone_manage->setText(QApplication::translate("CUiTop", "phone manage", nullptr));
        btn_login_register->setText(QApplication::translate("CUiTop", "register acount", nullptr));
        label_head->setText(QString());
        label_userName->setText(QString());
        btn_changeSkin->setText(QString());
        btn_setting->setText(QString());
        divider->setText(QString());
        btn_min->setText(QString());
        btn_max->setText(QString());
        btn_close->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class CUiTop: public Ui_CUiTop {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CUITOP_H
