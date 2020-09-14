#pragma once

#include "BaseDialog.h"
#include "ui_ExportDataView.h"

class ExportDataView : public BaseDialog {
    Q_OBJECT

public:
    struct ExportSetting {
        bool export_account = false;
        bool export_phone = false;
        bool export_password = false;
        bool export_role = false;
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
