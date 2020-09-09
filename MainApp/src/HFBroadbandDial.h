#pragma once

#include <QWidget>
#include "ui_HFBroadbandDial.h"
#include "BaseWidget.h"

class HFBroadbandDial : public BaseWidget
{
	Q_OBJECT

public:
	HFBroadbandDial(QWidget *parent = Q_NULLPTR);
	~HFBroadbandDial();

private slots:
	void on_btn_save_clicked();

private:
	Ui::HFBroadbandDial ui;
};
