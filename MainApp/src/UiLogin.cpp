#include "UiLogin.h"
#include "BubbleTipWidget.h"
#include "CDbHelper.h"
#include "DialogMsg.h"
#include "UserSession.h"
#include "WebHandler.h"
#include "define.h"
#include <QEventLoop>
#include <QFormLayout>
#include <QGraphicsDropShadowEffect>
#include <QLabel>
#include <QLineEdit>
#include <QPropertyAnimation>

static const char* USER_NAME = "USER_NAME";

UiLogin::UiLogin(QWidget* parent)
    : QDialog(parent)
    , m_pEditName(nullptr)
    , m_pEditPwd(nullptr)
    , m_bCanMove(false)
{
    this->setWindowOpacity(0.0);
    this->setWindowIcon(QIcon("images/app.ico"));
    this->setWindowTitle(tr("feng he network"));
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setMouseTracking(true);

    qRegisterMetaType<ResponData>("ResponData");
    WebHandler::bindDataCallback(this, SLOT(onRequestCallback(const ResponData&)));
    //connect(WebHandler::instance(), &WebHandler::requestCallback, this, &UiLogin::onRequestCallback, Qt::QueuedConnection);

    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(0, 0);
    shadow->setColor(Qt::black);
    shadow->setBlurRadius(10);
    this->setGraphicsEffect(shadow);

    QHBoxLayout* layout_main = new QHBoxLayout(this);
    QWidget* widgetContent = new QWidget(this);
    widgetContent->setObjectName("contentWidget");
    layout_main->addWidget(widgetContent);

    QLabel* pName = new QLabel(tr("user name"), this); //用户名(&U)：
    m_pEditName = new QLineEdit(this);
    m_pEditName->setMinimumHeight(35);
    pName->setBuddy(m_pEditName);

    QLabel* pPwd = new QLabel(tr("login password"), this); //密码(&P)：
    m_pEditPwd = new QLineEdit(this);
    m_pEditPwd->setMinimumHeight(35);
    m_pEditPwd->setEchoMode(QLineEdit::Password);
    pPwd->setBuddy(m_pEditPwd);

    QFormLayout* layout_input = new QFormLayout;
    {
        QHBoxLayout* userNameLayout = new QHBoxLayout;
        userNameLayout->setSpacing(5);
        userNameLayout->setContentsMargins(0, 0, 0, 0);
        userNameLayout->addWidget(m_pEditName);
        QPushButton* registerBtn = new QPushButton(tr("register"), this);
        registerBtn->setObjectName("registerBtn");
        connect(registerBtn, &QPushButton::clicked, [=]() {});
        userNameLayout->addWidget(registerBtn);
        layout_input->addRow(pName, userNameLayout);
    }

    {
        QHBoxLayout* passwordLayout = new QHBoxLayout;
        passwordLayout->setSpacing(5);
        passwordLayout->setContentsMargins(0, 0, 0, 0);
        passwordLayout->addWidget(m_pEditPwd);
        QPushButton* forgetPwd = new QPushButton(tr("forget password"), this);
        forgetPwd->setObjectName("forgetPassword");
        connect(forgetPwd, &QPushButton::clicked, [=]() {});
        passwordLayout->addWidget(forgetPwd);
        layout_input->addRow(pPwd, passwordLayout);
    }

    layout_input->setVerticalSpacing(15);
    layout_input->setContentsMargins(15, 15, 15, 15);

    QGridLayout* layout = new QGridLayout(widgetContent);

    QPushButton* btn_close = UiHelper::creatPushButton(
        this, [=]() {
            this->reject();
        },
        25, 25, "", "btn_close");

    QHBoxLayout* layout_close = new QHBoxLayout;
    layout_close->setAlignment(Qt::AlignTop);
    layout_close->addStretch();
    layout_close->addWidget(btn_close);

    layout->addLayout(layout_close, 0, 0, 1, 1);

    QLabel* logo = new QLabel(this);
    QPixmap pixLogo("images/logo.png");
    logo->setMinimumWidth(115);
    logo->setPixmap(UiHelper::justPixmapByWidth(115, pixLogo));

    QHBoxLayout* layout_logo = new QHBoxLayout;
    layout_logo->addStretch();
    layout_logo->addWidget(logo);
    layout_logo->addStretch();

    layout->addLayout(layout_logo, 1, 0, 3, 1);
    layout->addLayout(layout_input, 4, 0, 3, 1);

    auto verify = std::bind(&UiLogin::verify, this);
    buttonLogin = UiHelper::creatPushButton(this, verify, 0, 0, tr("login")); //登录
    buttonLogin->setShortcut(QKeySequence(Qt::Key_Return));

    QHBoxLayout* layout_ok = new QHBoxLayout;
    layout_ok->addStretch();
    layout_ok->addWidget(buttonLogin);
    layout_ok->addStretch();

    layout->addLayout(layout_ok, 7, 0, 3, 1);
    this->resize(430, 330);

    CDbHelper::opendatabase("db.s3db");
    this->initUser();
}

UiLogin::~UiLogin()
{
}

void UiLogin::initUser()
{
    QSettings userSetting("user.ini", QSettings::IniFormat);
    QString userName = userSetting.value(USER_NAME, "").toString();
    m_pEditName->setText(userName);
}

void UiLogin::verify()
{
    do {
        QString strTip = "";
        QString userName = m_pEditName->text();
        if (userName.isEmpty()) {
            strTip = tr("please input user name");
            QPoint pos = m_pEditName->mapToGlobal(QPoint(m_pEditName->width() / 2, 15));
            BubbleTipWidget::showTextTipsWithShadowColor(strTip, pos, BubbleTipWidget::Top, QColor(170, 0, 0), this);
            break;
        }
        QString password = m_pEditPwd->text();
        if (password.isEmpty()) {
            strTip = tr("please input password");
            QPoint pos = m_pEditPwd->mapToGlobal(QPoint(m_pEditPwd->width() / 2, 15));
            BubbleTipWidget::showTextTipsWithShadowColor(strTip, pos, BubbleTipWidget::Top, QColor(170, 0, 0), this);
            break;
        }

        buttonLogin->setEnabled(false);
        RequestTask task;
        task.reqeustId = (quint64)(this);
        task.bodyObj.insert("account", userName);
        task.bodyObj.insert("password", password);
        task.apiIndex = API::login;
        WebHandler::instance()->Post(task);
        QSettings userSetting("user.ini", QSettings::IniFormat);
        userSetting.setValue(USER_NAME, userName);
    } while (0);
}

void UiLogin::onRequestCallback(const ResponData& data)
{
    buttonLogin->setEnabled(true);
    if (data.task.reqeustId == (quint64)(this)) {
        qInfo("data respond=%s", data.dataReturned.constData());
        QJsonDocument document = QJsonDocument::fromJson(data.dataReturned);
        QJsonObject dataObj = document.object();
        if (dataObj.isEmpty())
            return;
        if (dataObj.contains("code")) {
            int code = dataObj.value("code").toInt();
            if (code != 0) {
                DialogMsg::question(this, tr("warning"), dataObj.value("msg").toString(), QMessageBox::Ok);
                return;
            }

            UserData userSession;
            userSession.uid = dataObj.value("id").toInt();
            userSession.token = dataObj.value("token").toString();
            userSession.status = dataObj.value("status").toInt();
            UserSession::instance().setUserData(userSession);

            this->accept();
        }
    }
}

int UiLogin::fadeIn()
{
    this->show();

    /*开启事件循环，同步等待动画完成再启动*/
    QEventLoop eventLoop;
    QPropertyAnimation* animation = new QPropertyAnimation(this, "windowOpacity", this);
    connect(animation, &QPropertyAnimation::finished, &eventLoop, &QEventLoop::quit);
    animation->setDuration(200);
    animation->setStartValue(0.0);
    animation->setEndValue(1.0);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
    eventLoop.exec();

    return this->exec();
}

void UiLogin::mouseMoveEvent(QMouseEvent* event)
{
    QDialog::mouseMoveEvent(event);
    if (event->buttons().testFlag(Qt::LeftButton) && m_bCanMove) {
        QPoint posDes = event->globalPos() - m_dragPoint;
        this->move(posDes);
    }
}

void UiLogin::mousePressEvent(QMouseEvent* event)
{
    QDialog::mousePressEvent(event);
    if (QRect(0, 0, this->width(), 44).contains(event->pos())) {
        m_bCanMove = true;
        m_dragPoint = event->globalPos() - frameGeometry().topLeft();
    } else
        m_bCanMove = false;
}

void UiLogin::mouseReleaseEvent(QMouseEvent* event)
{
    QDialog::mouseReleaseEvent(event);
    m_bCanMove = false;
}

void UiLogin::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
        this->verify();
    }
}
