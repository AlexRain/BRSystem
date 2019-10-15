#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include "define.h"
#include "UiLogin.h"

UiLogin::UiLogin(QWidget *parent)
	: QDialog(parent), _pEditName(nullptr), _pEditPwd(nullptr)
{
	ui.setupUi(this);

	this->setWindowIcon(QIcon("images/app.ico"));
	this->setWindowTitle(TOCH("物品借还系统"));
	this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);

	QGridLayout *layout = new QGridLayout(this);
	QLabel *pName = new QLabel(TOCH("用户名(&U)："),this);
	_pEditName = new QLineEdit(this);
	pName->setBuddy(_pEditName);

	QLabel *pPwd = new QLabel(TOCH("密码(&P)："),this);
	_pEditPwd = new QLineEdit(this);
	_pEditPwd->setEchoMode(QLineEdit::Password);
	pPwd->setBuddy(_pEditPwd);

	layout->addWidget(pName, 0, 0, 1, 1);
	layout->addWidget(_pEditName, 0, 1, 1, 6);
	layout->addWidget(pPwd, 1, 0, 1, 1);
	layout->addWidget(_pEditPwd, 1, 1, 1, 6);

	auto verify = std::bind(&UiLogin::verify, this);
	QPushButton *btn_ok = UiHelper::creatPushButton(this, verify, 0, 0,TOCH("登录"));
	btn_ok->setShortcut(QKeySequence(Qt::Key_Enter));
	layout->addWidget(btn_ok,2,2,1,3);

}

UiLogin::~UiLogin()
{
}

void UiLogin::verify()
{
	bool ok = true;
	if ("123456" != _pEditPwd->text() || _pEditPwd->text().isEmpty()) ok = false;
	if ("admin" != _pEditName->text() || _pEditName->text().isEmpty()) ok = false;
	if (ok) this->accept();
		
}
