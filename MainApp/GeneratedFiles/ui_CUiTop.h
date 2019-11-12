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
#include "CLabel.h"

QT_BEGIN_NAMESPACE

class Ui_CUiTop
{
public:
    QHBoxLayout *horizontalLayout;
    CLabel *label_logo;
    QSpacerItem *horizontalSpacer;
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
            CUiTop->setObjectName(QStringLiteral("CUiTop"));
        CUiTop->resize(691, 38);
        CUiTop->setMinimumSize(QSize(0, 38));
        CUiTop->setMaximumSize(QSize(16777215, 38));
        CUiTop->setStyleSheet(QStringLiteral(""));
        horizontalLayout = new QHBoxLayout(CUiTop);
        horizontalLayout->setSpacing(23);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(15, 5, 5, 0);
        label_logo = new CLabel(CUiTop);
        label_logo->setObjectName(QStringLiteral("label_logo"));
        label_logo->setMinimumSize(QSize(115, 0));
        label_logo->setMaximumSize(QSize(115, 16777215));
        label_logo->setCursor(QCursor(Qt::PointingHandCursor));

        horizontalLayout->addWidget(label_logo);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(10);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(-1, -1, 0, -1);
        label_head = new QLabel(CUiTop);
        label_head->setObjectName(QStringLiteral("label_head"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_head->sizePolicy().hasHeightForWidth());
        label_head->setSizePolicy(sizePolicy);
        label_head->setMinimumSize(QSize(30, 30));
        label_head->setMaximumSize(QSize(30, 30));

        horizontalLayout_4->addWidget(label_head);

        label_userName = new CLabel(CUiTop);
        label_userName->setObjectName(QStringLiteral("label_userName"));
        label_userName->setCursor(QCursor(Qt::PointingHandCursor));

        horizontalLayout_4->addWidget(label_userName);


        horizontalLayout->addLayout(horizontalLayout_4);

        btn_changeSkin = new QPushButton(CUiTop);
        btn_changeSkin->setObjectName(QStringLiteral("btn_changeSkin"));
        sizePolicy.setHeightForWidth(btn_changeSkin->sizePolicy().hasHeightForWidth());
        btn_changeSkin->setSizePolicy(sizePolicy);
        btn_changeSkin->setMinimumSize(QSize(23, 23));
        btn_changeSkin->setMaximumSize(QSize(23, 23));
        btn_changeSkin->setCursor(QCursor(Qt::PointingHandCursor));

        horizontalLayout->addWidget(btn_changeSkin);

        btn_setting = new QPushButton(CUiTop);
        btn_setting->setObjectName(QStringLiteral("btn_setting"));
        sizePolicy.setHeightForWidth(btn_setting->sizePolicy().hasHeightForWidth());
        btn_setting->setSizePolicy(sizePolicy);
        btn_setting->setMinimumSize(QSize(18, 18));
        btn_setting->setMaximumSize(QSize(18, 18));
        btn_setting->setCursor(QCursor(Qt::PointingHandCursor));

        horizontalLayout->addWidget(btn_setting);

        divider = new QLabel(CUiTop);
        divider->setObjectName(QStringLiteral("divider"));
        divider->setMinimumSize(QSize(1, 23));
        divider->setMaximumSize(QSize(1, 23));

        horizontalLayout->addWidget(divider);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(-1, -1, 0, -1);
        btn_min = new QPushButton(CUiTop);
        btn_min->setObjectName(QStringLiteral("btn_min"));
        sizePolicy.setHeightForWidth(btn_min->sizePolicy().hasHeightForWidth());
        btn_min->setSizePolicy(sizePolicy);
        btn_min->setMinimumSize(QSize(20, 20));
        btn_min->setMaximumSize(QSize(20, 20));
        btn_min->setCursor(QCursor(Qt::PointingHandCursor));

        horizontalLayout_2->addWidget(btn_min);

        btn_max = new QPushButton(CUiTop);
        btn_max->setObjectName(QStringLiteral("btn_max"));
        sizePolicy.setHeightForWidth(btn_max->sizePolicy().hasHeightForWidth());
        btn_max->setSizePolicy(sizePolicy);
        btn_max->setMinimumSize(QSize(20, 20));
        btn_max->setMaximumSize(QSize(20, 20));
        btn_max->setCursor(QCursor(Qt::PointingHandCursor));

        horizontalLayout_2->addWidget(btn_max);

        btn_close = new QPushButton(CUiTop);
        btn_close->setObjectName(QStringLiteral("btn_close"));
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
