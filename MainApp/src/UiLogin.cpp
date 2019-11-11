#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QGraphicsDropShadowEffect>
#include "define.h"
#include "UserSession.h"
#include "UiLogin.h"
#include "CDbHelper.h"
#include "DialogMsg.h"
#include "BubbleTipWidget.h"
#include <QPropertyAnimation>
#include <QEventLoop>

UiLogin::UiLogin(QWidget *parent)
	: QDialog(parent), m_pEditName(nullptr), m_pEditPwd(nullptr), m_bCanMove(false)
{
	this->setWindowOpacity(0.0);
	this->setWindowIcon(QIcon("images/app.ico"));
	this->setWindowTitle(TOCH("物品借还系统"));
	this->setWindowFlags(Qt::FramelessWindowHint);
	this->setAttribute(Qt::WA_TranslucentBackground);
	this->setMouseTracking(true);

	QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
	shadow->setOffset(0, 0);
	shadow->setColor(Qt::black);
	shadow->setBlurRadius(10);
	this->setGraphicsEffect(shadow);

	QHBoxLayout *layout_main = new QHBoxLayout(this);
	QWidget *widgetContent = new QWidget(this);
	widgetContent->setObjectName("contentWidget");
	layout_main->addWidget(widgetContent);

	QLabel *pName = new QLabel(TOCH("用户名(&U)："), this);
	m_pEditName = new QComboBox(this);
	m_pEditName->setEditable(true);
	m_pEditName->setMinimumHeight(35);
	pName->setBuddy(m_pEditName);

	QLabel *pPwd = new QLabel(TOCH("密码(&P)："), this);
	m_pEditPwd = new QLineEdit(this);
	connect(m_pEditName, SIGNAL(currentIndexChanged(int)), m_pEditPwd, SLOT(setFocus()));
	m_pEditPwd->setMinimumHeight(35);
	m_pEditPwd->setEchoMode(QLineEdit::Password);
	pPwd->setBuddy(m_pEditPwd);

	QGridLayout *layout_input = new QGridLayout;
	layout_input->addWidget(pName, 0, 0, 1, 1);
	layout_input->addWidget(m_pEditName, 0, 1, 1, 1);
	layout_input->addWidget(pPwd, 1, 0, 1, 1);
	layout_input->addWidget(m_pEditPwd, 1, 1, 1, 1);
	layout_input->setVerticalSpacing(25);
	layout_input->setContentsMargins(15, 15, 15, 15);

	QGridLayout *layout = new QGridLayout(widgetContent);

	QPushButton *btn_close = UiHelper::creatPushButton(this, [=]() {
		this->reject();
	}, 25, 25, "", "btn_close");

	QHBoxLayout *layout_close = new QHBoxLayout;
	layout_close->setAlignment(Qt::AlignTop);
	layout_close->addStretch();
	layout_close->addWidget(btn_close);

	layout->addLayout(layout_close,  0, 0, 1, 1);

	QLabel *logo = new QLabel(this);
	QPixmap pixLogo("images/logo.png");
	logo->setMinimumWidth(115);
	logo->setPixmap(UiHelper::justPixmapByWidth(115,pixLogo));

	QHBoxLayout *layout_logo = new QHBoxLayout;
	layout_logo->addStretch();
	layout_logo->addWidget(logo);
	layout_logo->addStretch();

	layout->addLayout(layout_logo, 1, 0, 3, 1);
	layout->addLayout(layout_input, 4, 0, 3, 1);

	auto verify = std::bind(&UiLogin::verify, this);
	QPushButton *btn_ok = UiHelper::creatPushButton(this, verify, 0, 0,TOCH("登录"));
	btn_ok->setShortcut(QKeySequence(Qt::Key_Return));

	QHBoxLayout *layout_ok = new QHBoxLayout;
	layout_ok->addStretch();
	layout_ok->addWidget(btn_ok);
	layout_ok->addStretch();

	layout->addLayout(layout_ok,     7,0,3,1);
	this->resize(430,330);

	CDbHelper::opendatabase("db.s3db");
	this->initUser();
}

UiLogin::~UiLogin()
{
}

void UiLogin::initUser()
{
	CDbHelper dbHelper;
	bool succ = dbHelper.open();
	if (!succ){
		DialogMsg::question(this, TOCH("发生错误"), dbHelper.lastErrorText(), QMessageBox::Ok);
		exit(0);
	}

	if (dbHelper.isTableExist(DIC_BORROW_RETURN))
	{
		QList<ModelData> vModel;
		int rows = dbHelper.Queuey(vModel, "SELECT * FROM DIC_USER ORDER BY createTime asc");
		for (int i = 0,nLen = vModel.size(); i < nLen; ++i)
		{
			const ModelData &model = vModel[i];
			UserData userData;
			userData.userId = model["userId"];
			userData.userName = model["userName"];
			userData.isAdmin = 1 == model["isAdmin"].toInt();
			userData.departmentId = model["departmentId"];
			userData.departmentName = model["departmentName"];
			userData.password = model["password"];
			m_pEditName->addItem(model["userName"], QVariant::fromValue(userData));
		}

		m_pEditName->setCurrentIndex(0);
		m_pEditPwd->setFocus();
	}
	else {
		DialogMsg::question(this,TOCH("发生错误"),TOCH("数据错误，请联系管理员。"),QMessageBox::Ok);
		exit(0);
	}
}

void UiLogin::verify()
{
	bool ok = false;
	do 
	{
		QString strTip = "";
		if (m_pEditName->currentText().isEmpty()) {
			strTip = TOCH("请选择用户");
			QPoint pos = m_pEditName->mapToGlobal(QPoint(m_pEditName->width() / 2, 15));
			BubbleTipWidget::showTextTipsWithShadowColor(strTip, pos, BubbleTipWidget::Top, QColor(170, 0, 0), this);
			ok = false;
			break;
		}
		QString id = m_pEditName->currentData().value<UserData>().userId;
		if (id.isEmpty() || -1 == m_pEditName->findText(m_pEditName->currentText()))
		{
			strTip = TOCH("请在下拉菜单中选择用户");
			QPoint pos = m_pEditName->mapToGlobal(QPoint(m_pEditName->width() / 2, 15));
			BubbleTipWidget::showTextTipsWithShadowColor(strTip, pos, BubbleTipWidget::Top, QColor(170, 0, 0), this);
			ok = false;
			break;
		}
		QString password = m_pEditName->currentData().value<UserData>().password;
		if (password != m_pEditPwd->text() || m_pEditPwd->text().isEmpty()) {
			strTip = TOCH("密码错误");
			QPoint pos = m_pEditPwd->mapToGlobal(QPoint(m_pEditPwd->width() / 2, 15));
			BubbleTipWidget::showTextTipsWithShadowColor(strTip, pos, BubbleTipWidget::Top, QColor(170, 0, 0), this);
			ok = false;
			break;
		}
		ok = true;
	} while (0);
	
	if (ok) {
		UserSession::getInstance().setUserData(qvariant_cast<UserData>(m_pEditName->currentData()));
		this->accept();
	}
}

int UiLogin::fadeIn()
{
	this->show();

	/*开启事件循环，同步等待动画完成再启动*/
	QEventLoop eventLoop;
	QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity", this);
	connect(animation, &QPropertyAnimation::finished, &eventLoop,&QEventLoop::quit);
	animation->setDuration(200);
	animation->setStartValue(0.0);
	animation->setEndValue(1.0);
	animation->start(QAbstractAnimation::DeleteWhenStopped);
	eventLoop.exec();

	return this->exec();
}

void UiLogin::mouseMoveEvent(QMouseEvent *event)
{
	QDialog::mouseMoveEvent(event);
	if (event->buttons().testFlag(Qt::LeftButton) && m_bCanMove){
		QPoint posDes = event->globalPos() - m_dragPoint;
		this->move(posDes);
	}
}

void UiLogin::mousePressEvent(QMouseEvent *event)
{
	QDialog::mousePressEvent(event);
	if (QRect(0, 0, this->width(), 44).contains(event->pos())){
		m_bCanMove = true;
		m_dragPoint = event->globalPos() - frameGeometry().topLeft();
	}
	else
		m_bCanMove = false;
}

void UiLogin::mouseReleaseEvent(QMouseEvent *event)
{
	QDialog::mouseReleaseEvent(event);
	m_bCanMove = false;
}

void UiLogin::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
		this->verify();
	}
}
