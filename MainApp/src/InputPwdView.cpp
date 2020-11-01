#include "InputPwdView.h"
#include "DialogMsg.h"
#include <QTime>


QString getRandomString(int length)
{
	qsrand(QDateTime::currentMSecsSinceEpoch());//为随机值设定一个seed

	const char chrs[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	int chrs_size = sizeof(chrs);

	char* ch = new char[length + 1];
	memset(ch, 0, length + 1);
	int randomx = 0;
	for (int i = 0; i < length; ++i)
	{
		randomx = rand() % (chrs_size - 1);
		ch[i] = chrs[randomx];
	}

	QString ret(ch);
	delete[] ch;
	return ret;
}

InputPwdView::InputPwdView(QWidget* parent)
    : BaseDialog(parent)
{
    ui.setupUi(this->getContent());
    this->SetTitle(tr("modify password"));
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
	qsrand(time(NULL));
	int n = qrand() % 9;

    int pwdLength[3] = { 8, 9, 10 };
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
	password = password + QString::number(n);
}

void InputPwdView::on_btn_ok_clicked()
{
    if (ui.lineEdit_password->text().isEmpty()) {
        DialogMsg::question(this, tr("tips"), tr("please enter password or click create automatically"), QMessageBox::Ok);
        return;
    }
    this->accept();
}
