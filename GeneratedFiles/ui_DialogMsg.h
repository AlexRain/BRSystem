/********************************************************************************
** Form generated from reading UI file 'DialogMsg.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGMSG_H
#define UI_DIALOGMSG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DialogMsg
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *contentWidget;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *layout_title;
    QLabel *label_title;
    QPushButton *btn_close;
    QLabel *label_content;
    QHBoxLayout *layout_btn;

    void setupUi(QDialog *DialogMsg)
    {
        if (DialogMsg->objectName().isEmpty())
            DialogMsg->setObjectName(QStringLiteral("DialogMsg"));
        DialogMsg->resize(360, 216);
        verticalLayout = new QVBoxLayout(DialogMsg);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(8, 8, 8, 8);
        contentWidget = new QWidget(DialogMsg);
        contentWidget->setObjectName(QStringLiteral("contentWidget"));
        verticalLayout_2 = new QVBoxLayout(contentWidget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(5, 5, 5, 5);
        layout_title = new QHBoxLayout();
        layout_title->setSpacing(6);
        layout_title->setObjectName(QStringLiteral("layout_title"));
        layout_title->setContentsMargins(-1, -1, -1, 0);
        label_title = new QLabel(contentWidget);
        label_title->setObjectName(QStringLiteral("label_title"));
        label_title->setAlignment(Qt::AlignCenter);

        layout_title->addWidget(label_title);

        btn_close = new QPushButton(contentWidget);
        btn_close->setObjectName(QStringLiteral("btn_close"));
        btn_close->setMinimumSize(QSize(20, 20));
        btn_close->setMaximumSize(QSize(20, 20));

        layout_title->addWidget(btn_close);


        verticalLayout_2->addLayout(layout_title);

        label_content = new QLabel(contentWidget);
        label_content->setObjectName(QStringLiteral("label_content"));
        label_content->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(label_content);

        layout_btn = new QHBoxLayout();
        layout_btn->setSpacing(15);
        layout_btn->setObjectName(QStringLiteral("layout_btn"));
        layout_btn->setContentsMargins(-1, -1, -1, 20);

        verticalLayout_2->addLayout(layout_btn);

        verticalLayout_2->setStretch(1, 3);
        verticalLayout_2->setStretch(2, 2);

        verticalLayout->addWidget(contentWidget);


        retranslateUi(DialogMsg);

        QMetaObject::connectSlotsByName(DialogMsg);
    } // setupUi

    void retranslateUi(QDialog *DialogMsg)
    {
        DialogMsg->setWindowTitle(QApplication::translate("DialogMsg", "DialogMsg", nullptr));
        label_title->setText(QApplication::translate("DialogMsg", "title", nullptr));
        btn_close->setText(QString());
        label_content->setText(QApplication::translate("DialogMsg", "content", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DialogMsg: public Ui_DialogMsg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGMSG_H
