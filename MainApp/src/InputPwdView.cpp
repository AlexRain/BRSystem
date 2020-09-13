#include "InputPwdView.h"
#include "DialogMsg.h"
#include <QTime>

InputPwdView::InputPwdView(QWidget* parent)
    : BaseDialog(parent)
{
    ui.setupUi(this->getContent());
    connect(ui.lineEdit_password, &QLineEdit::textChanged, this, [=](const QString& text) {
        ui.btn_ok->setEnabled(!text.isEmpty());
    });
    QMetaObject::connectSlotsByName(this);
}

InputPwdView::~InputPwdView()
{
}

void InputPwdView::GetPassword(QString& password)
{
    password = ui.lineEdit_password->text();
}

void InputPwdView::on_btn_autoCreate_clicked()
{
    QString newPassword;
    CreatePassword(newPassword);
    ui.lineEdit_password->setText(newPassword);
}

void InputPwdView::CreatePassword(QString& password)
{
    int pwdLength[3] = { 6, 7, 8 };
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    int length = pwdLength[qrand() % 3];
    enum charactorType {
        number = 0,
        capital,
        lowercase
    };

    auto getCharactor = [=]() {
        charactorType type = static_cast<charactorType>(qrand() % 3);
        char charactor = '\0';
        switch (type) {
        case number: {
            charactor = static_cast<char>((48 + (qrand() % 10)));
            break;
        }
        case capital: {
            charactor = static_cast<char>((65 + (qrand() % 26)));
            break;
        }
        case lowercase: {
            charactor = static_cast<char>((97 + (qrand() % 26)));
            break;
        }
        default:
            break;
        }
        return charactor;
    };

    password.clear();
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    for (int i = 0; i < length; ++i) {
        password.append(getCharactor());
    }
}

void InputPwdView::on_btn_ok_clicked()
{
    if (ui.lineEdit_password->text().isEmpty()) {
        DialogMsg::question(this, tr("tips"), tr("please enter password or click create automatically"), QMessageBox::Ok);
        return;
    }
    this->accept();
}
