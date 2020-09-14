#include "ExportDataView.h"

ExportDataView::ExportDataView(QWidget* parent)
    : BaseDialog(parent)
{
    ui.setupUi(this->getContent());
    this->SetTitle(tr("export setting"));
}

ExportDataView::~ExportDataView()
{
}

void ExportDataView::GetExportSetting(ExportSetting& setting)
{
    setting.export_account = ui.checkBox_account->isChecked();
    setting.export_phone = ui.checkBox_phone->isChecked();
    setting.export_account = ui.checkBox_pwd->isChecked();
    setting.export_account = ui.checkBox_role->isChecked();
}

void ExportDataView::on_btn_ok_clicked()
{
    this->accept();
}
