#pragma once

#include "BaseDialog.h"
#include "ui_InputIdentityView.h"

class InputIdentityView : public BaseDialog {
    Q_OBJECT

public:
    InputIdentityView(QWidget* parent = Q_NULLPTR);
    ~InputIdentityView();

private slots:
    void on_btn_ok_clicked();

private:
    Ui::InputIdentityView ui;
};
