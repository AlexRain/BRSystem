/********************************************************************************
** Form generated from reading UI file 'OperationLog.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OPERATIONLOG_H
#define UI_OPERATIONLOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_OperationLog
{
public:
    QGridLayout *gridLayout;
    QTableView *tableView;
    QLabel *label_count;

    void setupUi(QWidget *OperationLog)
    {
        if (OperationLog->objectName().isEmpty())
            OperationLog->setObjectName(QStringLiteral("OperationLog"));
        OperationLog->resize(698, 435);
        gridLayout = new QGridLayout(OperationLog);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(-1, 44, -1, -1);
        tableView = new QTableView(OperationLog);
        tableView->setObjectName(QStringLiteral("tableView"));

        gridLayout->addWidget(tableView, 0, 0, 1, 1);

        label_count = new QLabel(OperationLog);
        label_count->setObjectName(QStringLiteral("label_count"));

        gridLayout->addWidget(label_count, 1, 0, 1, 1);


        retranslateUi(OperationLog);

        QMetaObject::connectSlotsByName(OperationLog);
    } // setupUi

    void retranslateUi(QWidget *OperationLog)
    {
        OperationLog->setWindowTitle(QApplication::translate("OperationLog", "OperationLog", nullptr));
        label_count->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class OperationLog: public Ui_OperationLog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OPERATIONLOG_H
