/********************************************************************************
** Form generated from reading UI file 'UiUsageRecord.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UIUSAGERECORD_H
#define UI_UIUSAGERECORD_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "CSearchLineEdit.h"

QT_BEGIN_NAMESPACE

class Ui_UiUsageRecord
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    CSearchLineEdit *lineEdit;
    QDateEdit *dateEdit;
    QLabel *label;
    QDateEdit *dateEdit_2;
    QSpacerItem *horizontalSpacer;
    QTableView *tableView;

    void setupUi(QWidget *UiUsageRecord)
    {
        if (UiUsageRecord->objectName().isEmpty())
            UiUsageRecord->setObjectName(QStringLiteral("UiUsageRecord"));
        UiUsageRecord->resize(570, 360);
        verticalLayout = new QVBoxLayout(UiUsageRecord);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(-1, 44, -1, -1);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        lineEdit = new CSearchLineEdit(UiUsageRecord);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lineEdit->sizePolicy().hasHeightForWidth());
        lineEdit->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(lineEdit);

        dateEdit = new QDateEdit(UiUsageRecord);
        dateEdit->setObjectName(QStringLiteral("dateEdit"));

        horizontalLayout->addWidget(dateEdit);

        label = new QLabel(UiUsageRecord);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        dateEdit_2 = new QDateEdit(UiUsageRecord);
        dateEdit_2->setObjectName(QStringLiteral("dateEdit_2"));

        horizontalLayout->addWidget(dateEdit_2);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        tableView = new QTableView(UiUsageRecord);
        tableView->setObjectName(QStringLiteral("tableView"));

        verticalLayout->addWidget(tableView);


        retranslateUi(UiUsageRecord);

        QMetaObject::connectSlotsByName(UiUsageRecord);
    } // setupUi

    void retranslateUi(QWidget *UiUsageRecord)
    {
        UiUsageRecord->setWindowTitle(QApplication::translate("UiUsageRecord", "UiUsageRecord", nullptr));
        label->setText(QApplication::translate("UiUsageRecord", "-", nullptr));
    } // retranslateUi

};

namespace Ui {
    class UiUsageRecord: public Ui_UiUsageRecord {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UIUSAGERECORD_H
