#pragma once
#include "BaseDialog.h"
#include"ui_ExportSettingView.h"
class ExportSettingView : public BaseDialog {
	Q_OBJECT

public:
	struct ExportSetting {
		bool export_account = true;
		bool export_phone = true;
		bool export_password = true;
		bool export_role = true;
		bool export_roleName = true;
		bool export_roleCount = true;
		bool export_identity = true;
		bool export_zhicai = true;
		bool export_ban = true;
		bool export_point = true;
		bool export_score = true;
		int export_level = 0;
	};

public:
	ExportSettingView(QWidget* parent = Q_NULLPTR);
	~ExportSettingView();

	void GetExportSetting(ExportSetting& setting);

private slots:
	void on_btn_ok_clicked();

private:
	Ui::ExportSettingView ui;
};
