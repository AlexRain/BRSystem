#include "HFBroadbandDial.h"
#include "DialogMsg.h"
#include "define.h"

HFBroadbandDial::HFBroadbandDial(QWidget* parent)
    : BaseWidget(parent)
    , mSetting(DAIL_SETTING, QSettings::IniFormat)
{
    ui.setupUi(this);
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
    mSetting.setValue("username", ui.lineEdit_account->text());
    mSetting.setValue("password", ui.lineEdit_password->text());
    mSetting.setValue("redail", ui.spinBox_tasks->value());
    mSetting.setValue("switch", ui.comboBox_switch->currentIndex());
    int result = DialogMsg::question(this, tr("tips"), tr("Setting will be apply after restart, restart now?"), QMessageBox::Ok | QMessageBox::Cancel);
    if (result == QMessageBox::Ok) {
        qApp->exit(RESTART_CODE);
    }
}
