/********************************************************************************
** Form generated from reading UI file 'BubbleTipWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BUBBLETIPWIDGET_H
#define UI_BUBBLETIPWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BubbleTipWidget
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *contentWidget;

    void setupUi(QWidget *BubbleTipWidget)
    {
        if (BubbleTipWidget->objectName().isEmpty())
            BubbleTipWidget->setObjectName(QStringLiteral("BubbleTipWidget"));
        BubbleTipWidget->resize(400, 300);
        verticalLayout = new QVBoxLayout(BubbleTipWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(15, 15, 15, 15);
        contentWidget = new QWidget(BubbleTipWidget);
        contentWidget->setObjectName(QStringLiteral("contentWidget"));

        verticalLayout->addWidget(contentWidget);


        retranslateUi(BubbleTipWidget);

        QMetaObject::connectSlotsByName(BubbleTipWidget);
    } // setupUi

    void retranslateUi(QWidget *BubbleTipWidget)
    {
        BubbleTipWidget->setWindowTitle(QApplication::translate("BubbleTipWidget", "BubbleTipWidget", nullptr));
    } // retranslateUi

};

namespace Ui {
    class BubbleTipWidget: public Ui_BubbleTipWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BUBBLETIPWIDGET_H
