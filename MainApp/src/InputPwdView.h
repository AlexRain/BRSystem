#pragma once

#include "BaseDialog.h"
#include "ui_InputPwdView.h"

class InputPwdView : public BaseDialog {
    Q_OBJECT

public:
    InputPwdView(QWidget* parent = Q_NULLPTR);
    ~InputPwdView();

    void GetPassword(QString& password);

private slots:
    void on_btn_ok_clicked();
    void on_btn_autoCreate_clicked();

private:
    void CreatePassword(QString& password);

private:
    Ui::InputPwdView ui;
};
