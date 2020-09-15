#pragma once

#include "BaseDialog.h"
#include "ui_AboutView.h"

class AboutView : public BaseDialog {
    Q_OBJECT

public:
    AboutView(QWidget* parent = Q_NULLPTR);
    ~AboutView();

private slots:
    void on_btn_ok_clicked();

private:
    Ui::AboutView ui;
};
