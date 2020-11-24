#include "HFBroadbandDial.h"
#include "DialogMsg.h"
#include "define.h"
#include <qdebug.h>

HFBroadbandDial::HFBroadbandDial(QWidget* parent)
    : BaseWidget(parent)
{
    ui.setupUi(this);
    QSettings *settings = new QSettings(DAIL_SETTING, QSettings::IniFormat);
    QString username = settings->value("dial/username").toString();
    QString password = settings->value("dial/password").toString();
    int redail = settings->value("dial/redail").toInt();
    int dialSwitch = settings->value("dial/switch").toInt();
    int dialWait = settings->value("dial/wait",5).toInt();
    qDebug() << "username is " << username;
    qDebug() << "password is " << password;
    qDebug() << "redail is " << redail;
    qDebug() << "dialSwitch is " << dialSwitch;
    ui.lineEdit_account->setText(username);
    ui.lineEdit_password->setText(password);
    ui.spinBox_tasks->setValue(redail);
    ui.spinBox_wait->setValue(dialWait);
    ui.comboBox_switch->setCurrentIndex(dialSwitch);
    qDebug() << "heart msg ==> " << username;
}

HFBroadbandDial::~HFBroadbandDial()
{
}

void HFBroadbandDial::on_btn_save_clicked()
{
    if (ui.lineEdit_account->text().isEmpty()) {
        int result = DialogMsg::question(this, tr("tips"), tr("please enter account"), QMessageBox::Ok);
        return;
    }

    if (ui.lineEdit_password->text().isEmpty()) {
        int result = DialogMsg::question(this, tr("tips"), tr("please enter password"), QMessageBox::Ok);
        return;
    }

    if (ui.spinBox_tasks->value() == 0) {
        int result = DialogMsg::question(this, tr("tips"), tr("time can not be zero"), QMessageBox::Ok);
        return;
    }
    //QSettings setting(GetAppDataLocation() + QDir::separator() + DAIL_SETTING, QSettings::IniFormat);
    QSettings setting(DAIL_SETTING, QSettings::IniFormat);
    setting.setValue("dial/username", ui.lineEdit_account->text());
    setting.setValue("dial/password", ui.lineEdit_password->text());
    setting.setValue("dial/redail", ui.spinBox_tasks->value());
    setting.setValue("dial/switch", ui.comboBox_switch->currentIndex());
    setting.setValue("dial/wait", ui.spinBox_wait->value());
    //int result = DialogMsg::question(this, tr("tips"), tr("Setting will be apply after restart, restart now?"), QMessageBox::Ok | QMessageBox::Cancel);
    //if (result == QMessageBox::Ok) {
    //    qApp->exit(RESTART_CODE);
    //}
    //else {
    this->close();
    //}
}
