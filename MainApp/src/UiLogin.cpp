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

static const char* REGISTER = "register";
static const char* FORGET_PWD = "forget";
static const char* USER_NAME = "USER_NAME";
static const char* RICH_TEXT = "<p><a href=\"register\"><span style=\" text - decoration: underline; color:rgb(0,122,204); \"> %s</span></a> %s<a href=\"forget\"><span style=\" text - decoration: underline; color:rgb(0,122,204); \"> %s</span></a></p>";

UiLogin::UiLogin(QWidget* parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    this->setWindowOpacity(0.0);
    this->setWindowIcon(QIcon("images/app.ico"));
    this->setWindowTitle(tr("feng he network"));
    this->setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setMouseTracking(true);

    qRegisterMetaType<ResponData>("ResponData");
    qRegisterMetaType<NetworkRequestError>("NetworkRequestError");
    WebHandler::bindDataCallback(this, SLOT(onRequestCallback(const ResponData&)));
    WebHandler::bindErrorCallback(this, SLOT(onRequestError(const ResponData&, NetworkRequestError)));

    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(0, 0);
    shadow->setColor(Qt::black);
    shadow->setBlurRadius(10);
    this->setGraphicsEffect(shadow);

    btn_close = UiHelper::creatPushButton(
        this, [=]() {
            this->reject();
        },
        25, 25, "", "btn_close");

    btn_back = UiHelper::creatPushButton(
        this, [=]() {
            btn_back->hide();
            ui.stackedWidget->setCurrentWidget(ui.page_login);
        },
        25, 25, "", "btn_back");
    btn_back->hide();

    QString linkText;
    linkText.sprintf(RICH_TEXT, tr("register").toUtf8().constData(), tr("or").toUtf8().constData(), tr("forget password").toUtf8().constData());
    ui.label_link->setText(linkText);
    connect(ui.label_link, &QLabel::linkActivated, this, &UiLogin::onLinkActived);

    ui.btn_login->setShortcut(QKeySequence(Qt::Key_Return));
    connect(ui.btn_login, &QPushButton::clicked, this, &UiLogin::verify);

    CDbHelper::opendatabase("db.s3db");
    this->initUser();
    ui.stackedWidget->setCurrentWidget(ui.page_login);
}

UiLogin::~UiLogin()
{
}

void UiLogin::initUser()
{
    QPixmap pixLogo("images/logo.png");
    ui.label_logo->setMinimumWidth(115);
    ui.label_logo->setPixmap(UiHelper::justPixmapByWidth(115, pixLogo));

    QSettings userSetting("user.ini", QSettings::IniFormat);
    QString userName = userSetting.value(USER_NAME, "").toString();
    ui.input_user_name->setText(userName);
    if (userName.isEmpty()) {
        ui.input_user_name->setFocus();
    } else {
        ui.input_password->setFocus();
    }
}

void UiLogin::verify()
{
    do {
        QString strTip = "";
        QString userName = ui.input_user_name->text();
        if (userName.isEmpty()) {
            strTip = tr("please input user name");
            QPoint pos = ui.input_user_name->mapToGlobal(QPoint(ui.input_user_name->width() / 2, 15));
            BubbleTipWidget::showTextTipsWithShadowColor(strTip, pos, BubbleTipWidget::Top, QColor(170, 0, 0), this);
            break;
        }
        QString password = ui.input_password->text();
        if (password.isEmpty()) {
            strTip = tr("please input password");
            QPoint pos = ui.input_password->mapToGlobal(QPoint(ui.input_password->width() / 2, 15));
            BubbleTipWidget::showTextTipsWithShadowColor(strTip, pos, BubbleTipWidget::Top, QColor(170, 0, 0), this);
            break;
        }

        ui.btn_login->setEnabled(false);
        RequestTask task;
        task.reqeustId = (quint64)(this);
        task.bodyObj.insert("account", userName);
        task.bodyObj.insert("password", password);
        task.apiIndex = API::login;
        WebHandler::instance()->Post(task);
        QSettings userSetting("user.ini", QSettings::IniFormat);
        userSetting.setValue(USER_NAME, userName);
    } while (false);
}

void UiLogin::onRequestCallback(const ResponData& data)
{
    ui.btn_login->setEnabled(true);
    if (data.task.reqeustId == (quint64)(this)) {
        qInfo("data respond=%s", data.dataReturned.constData());
        QJsonDocument document = QJsonDocument::fromJson(data.dataReturned);
        QJsonObject root = document.object();
        if (root.isEmpty())
            return;
        if (root.contains("code")) {
            int code = root.value("code").toInt();
            if (code != 0) {
                DialogMsg::question(this, tr("warning"), root.value("msg").toString(), QMessageBox::Ok);
                return;
            }

            QJsonObject dataObj = root.value("data").toObject();
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

void UiLogin::onLinkActived(const QString& link)
{
    if (link == REGISTER) {
        btn_back->show();
        ui.stackedWidget->setCurrentWidget(ui.page_register);
    } else if (link == FORGET_PWD) {
    }
}

void UiLogin::onRequestError(const ResponData& data, NetworkRequestError errorType)
{
    if (data.task.reqeustId == (quint64)this) {
        ui.btn_login->setEnabled(true);
        DialogMsg::question(this, tr("warning"), tr("network error occured"), QMessageBox::Ok);
        return;
    }
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

void UiLogin::resizeEvent(QResizeEvent* event)
{
    if (btn_close) {
        btn_close->move(this->width() - btn_close->width() - 10, 10);
        btn_close->raise();
    }

    if (btn_back) {
        btn_back->move(10, 10);
        btn_back->raise();
    }
    QDialog::resizeEvent(event);
}
