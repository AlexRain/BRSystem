/********************************************************************************
** Form generated from reading UI file 'CEditInfoDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CEDITINFODIALOG_H
#define UI_CEDITINFODIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CEditInfoDialog
{
public:
    QGridLayout *gridLayout_2;
    QGroupBox *groupBox_status;
    QHBoxLayout *horizontalLayout_2;
    QRadioButton *radioBtn_notReturned;
    QRadioButton *radioBtn_returned;
    QRadioButton *radioBtn_lost;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *btn_delete;
    QSpacerItem *horizontalSpacer;
    QPushButton *btn_history;
    QPushButton *btn_save;
    QPushButton *btn_cancel;
    QGroupBox *groupBox_main;
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBox_info;
    QGridLayout *gridLayout;
    QLineEdit *lineEdit_reason;
    QLabel *label_5;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *lineEdit_productId;
    QLineEdit *lineEdit_browerName;
    QLineEdit *lineEdit_productName;
    QLabel *label_3;
    QGroupBox *groupBox_mark;
    QVBoxLayout *verticalLayout;
    QPlainTextEdit *textEdit_mark;
    QLabel *label_4;

    void setupUi(QWidget *CEditInfoDialog)
    {
        if (CEditInfoDialog->objectName().isEmpty())
            CEditInfoDialog->setObjectName(QStringLiteral("CEditInfoDialog"));
        CEditInfoDialog->resize(698, 435);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(CEditInfoDialog->sizePolicy().hasHeightForWidth());
        CEditInfoDialog->setSizePolicy(sizePolicy);
        gridLayout_2 = new QGridLayout(CEditInfoDialog);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(-1, 44, -1, -1);
        groupBox_status = new QGroupBox(CEditInfoDialog);
        groupBox_status->setObjectName(QStringLiteral("groupBox_status"));
        groupBox_status->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        horizontalLayout_2 = new QHBoxLayout(groupBox_status);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        radioBtn_notReturned = new QRadioButton(groupBox_status);
        radioBtn_notReturned->setObjectName(QStringLiteral("radioBtn_notReturned"));

        horizontalLayout_2->addWidget(radioBtn_notReturned);

        radioBtn_returned = new QRadioButton(groupBox_status);
        radioBtn_returned->setObjectName(QStringLiteral("radioBtn_returned"));

        horizontalLayout_2->addWidget(radioBtn_returned);

        radioBtn_lost = new QRadioButton(groupBox_status);
        radioBtn_lost->setObjectName(QStringLiteral("radioBtn_lost"));

        horizontalLayout_2->addWidget(radioBtn_lost);


        gridLayout_2->addWidget(groupBox_status, 1, 0, 1, 2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        btn_delete = new QPushButton(CEditInfoDialog);
        btn_delete->setObjectName(QStringLiteral("btn_delete"));

        horizontalLayout_3->addWidget(btn_delete);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        btn_history = new QPushButton(CEditInfoDialog);
        btn_history->setObjectName(QStringLiteral("btn_history"));

        horizontalLayout_3->addWidget(btn_history);

        btn_save = new QPushButton(CEditInfoDialog);
        btn_save->setObjectName(QStringLiteral("btn_save"));

        horizontalLayout_3->addWidget(btn_save);

        btn_cancel = new QPushButton(CEditInfoDialog);
        btn_cancel->setObjectName(QStringLiteral("btn_cancel"));

        horizontalLayout_3->addWidget(btn_cancel);


        gridLayout_2->addLayout(horizontalLayout_3, 3, 1, 1, 1);

        groupBox_main = new QGroupBox(CEditInfoDialog);
        groupBox_main->setObjectName(QStringLiteral("groupBox_main"));
        horizontalLayout = new QHBoxLayout(groupBox_main);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        groupBox_info = new QGroupBox(groupBox_main);
        groupBox_info->setObjectName(QStringLiteral("groupBox_info"));
        groupBox_info->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        gridLayout = new QGridLayout(groupBox_info);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        lineEdit_reason = new QLineEdit(groupBox_info);
        lineEdit_reason->setObjectName(QStringLiteral("lineEdit_reason"));
        lineEdit_reason->setContextMenuPolicy(Qt::NoContextMenu);

        gridLayout->addWidget(lineEdit_reason, 3, 1, 1, 1);

        label_5 = new QLabel(groupBox_info);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setAlignment(Qt::AlignJustify|Qt::AlignVCenter);

        gridLayout->addWidget(label_5, 3, 0, 1, 1);

        label = new QLabel(groupBox_info);
        label->setObjectName(QStringLiteral("label"));
        label->setAlignment(Qt::AlignJustify|Qt::AlignVCenter);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_2 = new QLabel(groupBox_info);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setAlignment(Qt::AlignJustify|Qt::AlignVCenter);

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        lineEdit_productId = new QLineEdit(groupBox_info);
        lineEdit_productId->setObjectName(QStringLiteral("lineEdit_productId"));
        lineEdit_productId->setContextMenuPolicy(Qt::NoContextMenu);

        gridLayout->addWidget(lineEdit_productId, 0, 1, 1, 1);

        lineEdit_browerName = new QLineEdit(groupBox_info);
        lineEdit_browerName->setObjectName(QStringLiteral("lineEdit_browerName"));
        lineEdit_browerName->setContextMenuPolicy(Qt::NoContextMenu);

        gridLayout->addWidget(lineEdit_browerName, 2, 1, 1, 1);

        lineEdit_productName = new QLineEdit(groupBox_info);
        lineEdit_productName->setObjectName(QStringLiteral("lineEdit_productName"));
        lineEdit_productName->setContextMenuPolicy(Qt::NoContextMenu);

        gridLayout->addWidget(lineEdit_productName, 1, 1, 1, 1);

        label_3 = new QLabel(groupBox_info);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setAlignment(Qt::AlignJustify|Qt::AlignVCenter);

        gridLayout->addWidget(label_3, 2, 0, 1, 1);


        horizontalLayout->addWidget(groupBox_info);

        groupBox_mark = new QGroupBox(groupBox_main);
        groupBox_mark->setObjectName(QStringLiteral("groupBox_mark"));
        verticalLayout = new QVBoxLayout(groupBox_mark);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        textEdit_mark = new QPlainTextEdit(groupBox_mark);
        textEdit_mark->setObjectName(QStringLiteral("textEdit_mark"));
        textEdit_mark->setContextMenuPolicy(Qt::NoContextMenu);

        verticalLayout->addWidget(textEdit_mark);


        horizontalLayout->addWidget(groupBox_mark);


        gridLayout_2->addWidget(groupBox_main, 0, 0, 1, 2);

        label_4 = new QLabel(CEditInfoDialog);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout_2->addWidget(label_4, 2, 1, 1, 1);

        QWidget::setTabOrder(lineEdit_productId, lineEdit_productName);
        QWidget::setTabOrder(lineEdit_productName, lineEdit_browerName);
        QWidget::setTabOrder(lineEdit_browerName, lineEdit_reason);
        QWidget::setTabOrder(lineEdit_reason, textEdit_mark);
        QWidget::setTabOrder(textEdit_mark, radioBtn_notReturned);
        QWidget::setTabOrder(radioBtn_notReturned, radioBtn_returned);
        QWidget::setTabOrder(radioBtn_returned, radioBtn_lost);
        QWidget::setTabOrder(radioBtn_lost, btn_delete);
        QWidget::setTabOrder(btn_delete, btn_history);
        QWidget::setTabOrder(btn_history, btn_save);
        QWidget::setTabOrder(btn_save, btn_cancel);

        retranslateUi(CEditInfoDialog);

        QMetaObject::connectSlotsByName(CEditInfoDialog);
    } // setupUi

    void retranslateUi(QWidget *CEditInfoDialog)
    {
        CEditInfoDialog->setWindowTitle(QApplication::translate("CEditInfoDialog", "Form", nullptr));
        groupBox_status->setTitle(QApplication::translate("CEditInfoDialog", "\347\211\251\345\223\201\347\212\266\346\200\201", nullptr));
        radioBtn_notReturned->setText(QApplication::translate("CEditInfoDialog", "\346\234\252\345\275\222\350\277\230", nullptr));
        radioBtn_returned->setText(QApplication::translate("CEditInfoDialog", "\345\267\262\345\275\222\350\277\230", nullptr));
        radioBtn_lost->setText(QApplication::translate("CEditInfoDialog", "\344\270\242\345\244\261", nullptr));
        btn_delete->setText(QApplication::translate("CEditInfoDialog", "\345\210\240\351\231\244", nullptr));
        btn_history->setText(QApplication::translate("CEditInfoDialog", "\345\200\237\347\224\250\350\256\260\345\275\225", nullptr));
        btn_save->setText(QApplication::translate("CEditInfoDialog", "\344\277\235\345\255\230", nullptr));
        btn_cancel->setText(QApplication::translate("CEditInfoDialog", "\345\217\226\346\266\210", nullptr));
        groupBox_main->setTitle(QApplication::translate("CEditInfoDialog", "\345\200\237\347\224\250\344\277\241\346\201\257", nullptr));
        groupBox_info->setTitle(QString());
        label_5->setText(QApplication::translate("CEditInfoDialog", "<html><head/><body><p>\345\200\237\347\224\250\345\216\237\347\224\261\357\274\232<span style='color:red'>*</span></p></body></html>", nullptr));
        label->setText(QApplication::translate("CEditInfoDialog", "<html><head/><body><p>\347\211\251\345\223\201\347\274\226\345\217\267\357\274\232<span style='color:red'>*</span></p></body></html>", nullptr));
        label_2->setText(QApplication::translate("CEditInfoDialog", "<html><head/><body><p>\347\211\251\345\223\201\345\220\215\347\247\260\357\274\232<span style='color:red'>*</span></p></body></html>", nullptr));
        label_3->setText(QApplication::translate("CEditInfoDialog", "<html><head/><body><p>\345\200\237\347\224\250\344\272\272\357\274\232<span style='color:red'>*</span></p></body></html>", nullptr));
        groupBox_mark->setTitle(QApplication::translate("CEditInfoDialog", "\345\244\207\346\263\250", nullptr));
        label_4->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class CEditInfoDialog: public Ui_CEditInfoDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CEDITINFODIALOG_H
