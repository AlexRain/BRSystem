#include "ExportDataView.h"

ExportDataView::ExportDataView(QWidget* parent)
    : BaseDialog(parent)
{
    ui.setupUi(this->getContent());
	ui.checkBox_role->setText(QString::fromLocal8Bit("״̬"));
    this->SetTitle(tr("export setting"));
    QMetaObject::connectSlotsByName(this);
}

ExportDataView::~ExportDataView()
{
}

void ExportDataView::GetExportSetting(ExportSetting& setting)
{
    setting.export_account = ui.checkBox_account->isChecked();
    setting.export_phone = ui.checkBox_phone->isChecked();
    setting.export_password = ui.checkBox_pwd->isChecked();
    setting.export_role = ui.checkBox_role->isChecked();
}

void ExportDataView::on_btn_ok_clicked()
{
    this->accept();
}
