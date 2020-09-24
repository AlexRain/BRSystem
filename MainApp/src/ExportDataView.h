#pragma once

#include "BaseDialog.h"
#include "ui_ExportDataView.h"

class ExportDataView : public BaseDialog {
    Q_OBJECT

public:
    struct ExportSetting {
        bool export_account = true;
        bool export_phone = true;
        bool export_password = true;
        bool export_role = true;
    };

public:
    ExportDataView(QWidget* parent = Q_NULLPTR);
    ~ExportDataView();

    void GetExportSetting(ExportSetting& setting);

private slots:
    void on_btn_ok_clicked();

private:
    Ui::ExportDataView ui;
};
