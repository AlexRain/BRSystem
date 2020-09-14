#pragma once

#include "BaseDialog.h"
#include "ui_InputPhoneView.h"

class InputPhoneView : public BaseDialog {
    Q_OBJECT

public:
    InputPhoneView(QWidget* parent = Q_NULLPTR);
    ~InputPhoneView();

    void GetPhoneNumber(QString& phone);

private slots:
    void on_btn_ok_clicked();

private:
    Ui::InputPhoneView ui;
};
