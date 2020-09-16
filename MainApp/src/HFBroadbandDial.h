#pragma once

#include "BaseWidget.h"
#include "ui_HFBroadbandDial.h"
#include <QSettings>
#include <QWidget>

class HFBroadbandDial : public BaseWidget {
    Q_OBJECT

public:
    HFBroadbandDial(QWidget* parent = Q_NULLPTR);
    ~HFBroadbandDial();

private slots:
    void on_btn_save_clicked();

private:
    Ui::HFBroadbandDial ui;
    QSettings mSetting;
};
