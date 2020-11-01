
#include "ExportSettingView.h"
#include <QSettings>
const char* EXPORT_FILE_PWD = "export/pwd";
const char* EXPORT_FILE_PHONE = "export/phone";
const char* EXPORT_FILE_ROLE = "export/role";
const char* EXPORT_FILE_ROLE_NAME = "export/roleName";
const char* EXPORT_FILE_ROLE_COUNT = "export/roleCount";
const char* EXPORT_FILE_BAN = "export/ban";
const char* EXPORT_FILE_ZHICAI = "export/zhicai";
const char* EXPORT_FILE_IDENTITY = "export/identity";
const char* EXPORT_FILE_LEVEL = "export/level";
const char* EXPORT_FILE_POINT = "export/point";
const char* EXPORT_FILE_SCORE = "export/score";

ExportSettingView::ExportSettingView(QWidget* parent)
	: BaseDialog(parent)
{
	QSettings saveSetting("app.ini", QSettings::IniFormat);
	ui.setupUi(this->getContent());
	ui.checkBox_account->setText(QString::fromLocal8Bit("账号"));
	ui.checkBox_account->setChecked(true);
	ui.checkBox_account->setDisabled(true);

	ui.checkBox_pwd->setText(QString::fromLocal8Bit("密码"));
	ui.checkBox_pwd->setChecked(saveSetting.value(EXPORT_FILE_PWD, true).toBool());

	ui.checkBox_phone->setText(QString::fromLocal8Bit("手机号"));
	ui.checkBox_phone->setChecked(saveSetting.value(EXPORT_FILE_PHONE, true).toBool());

	ui.checkBox_role->setText(QString::fromLocal8Bit("角色职业"));
	ui.checkBox_role->setChecked(saveSetting.value(EXPORT_FILE_ROLE, true).toBool());

	ui.checkBox_roleName->setText(QString::fromLocal8Bit("角色名称"));
	ui.checkBox_roleName->setChecked(saveSetting.value(EXPORT_FILE_ROLE_NAME, true).toBool());

	ui.checkBox_roleCount->setText(QString::fromLocal8Bit("角色数量"));
	ui.checkBox_roleCount->setChecked(saveSetting.value(EXPORT_FILE_ROLE_COUNT, true).toBool());

	ui.checkBox_ban->setText(QString::fromLocal8Bit("封号"));
	ui.checkBox_ban->setChecked(saveSetting.value(EXPORT_FILE_BAN, false).toBool());

	ui.checkBox_zhicai->setText(QString::fromLocal8Bit("制裁"));
	ui.checkBox_zhicai->setChecked(saveSetting.value(EXPORT_FILE_ZHICAI, false).toBool());

	ui.checkBox_identity->setText(QString::fromLocal8Bit("认证"));
	ui.checkBox_identity->setChecked(saveSetting.value(EXPORT_FILE_IDENTITY, true).toBool());

	ui.checkBox_score->setText(QString::fromLocal8Bit("信用分"));
	ui.checkBox_score->setChecked(saveSetting.value(EXPORT_FILE_SCORE, true).toBool());

	ui.checkBox_point->setText(QString::fromLocal8Bit("点券"));
	ui.checkBox_point->setChecked(saveSetting.value(EXPORT_FILE_POINT, true).toBool());

	ui.spinBox_level->setValue(saveSetting.value(EXPORT_FILE_LEVEL, 0).toInt());

	ui.label_level1->setText(QString::fromLocal8Bit("导出大于等于"));

	ui.label_level2->setText(QString::fromLocal8Bit("级的角色"));
	ui.btn_ok->setText(QString::fromLocal8Bit("确认"));
	this->SetTitle(QString::fromLocal8Bit("导出设置"));
	QMetaObject::connectSlotsByName(this);
}

ExportSettingView::~ExportSettingView()
{
}

void ExportSettingView::GetExportSetting(ExportSetting& setting)
{
	QSettings saveSetting("app.ini", QSettings::IniFormat);
	setting.export_phone = ui.checkBox_phone->isChecked();
	saveSetting.setValue(EXPORT_FILE_PHONE, ui.checkBox_phone->isChecked());

	setting.export_password = ui.checkBox_pwd->isChecked();
	saveSetting.setValue(EXPORT_FILE_PWD, ui.checkBox_pwd->isChecked());

	setting.export_role = ui.checkBox_role->isChecked();
	saveSetting.setValue(EXPORT_FILE_ROLE, ui.checkBox_role->isChecked());

	setting.export_roleName = ui.checkBox_roleName->isChecked();
	saveSetting.setValue(EXPORT_FILE_ROLE_NAME, ui.checkBox_roleName->isChecked());

	setting.export_roleCount = ui.checkBox_roleCount->isChecked();
	saveSetting.setValue(EXPORT_FILE_ROLE_COUNT, ui.checkBox_roleCount->isChecked());

	setting.export_ban = ui.checkBox_ban->isChecked();
	saveSetting.setValue(EXPORT_FILE_BAN, ui.checkBox_ban->isChecked());

	setting.export_identity = ui.checkBox_identity->isChecked();
	saveSetting.setValue(EXPORT_FILE_IDENTITY, ui.checkBox_identity->isChecked());

	setting.export_zhicai = ui.checkBox_zhicai->isChecked();
	saveSetting.setValue(EXPORT_FILE_ZHICAI, ui.checkBox_zhicai->isChecked());

	setting.export_point = ui.checkBox_point->isChecked();
	saveSetting.setValue(EXPORT_FILE_POINT, ui.checkBox_point->isChecked());

	setting.export_score = ui.checkBox_score->isChecked();
	saveSetting.setValue(EXPORT_FILE_SCORE, ui.checkBox_score->isChecked());

	setting.export_level = ui.spinBox_level->value();
	saveSetting.setValue(EXPORT_FILE_LEVEL, ui.spinBox_level->value());
}

void ExportSettingView::on_btn_ok_clicked()
{
	this->accept();
}
