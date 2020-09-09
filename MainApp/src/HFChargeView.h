#pragma once

#include <QWidget>
#include "BaseWidget.h"
#include "ui_HFChargeView.h"
#include "WebHandler.h"

class HFChargeView : public BaseWidget
{
	Q_OBJECT

public:
	HFChargeView(QWidget *parent = Q_NULLPTR);
	~HFChargeView();

private slots:
	 void on_btn_charge_clicked();
	 void onRequestCallback(const ResponData&);

signals:
	void chargeSuccess();

private:
	Ui::HFChargeView ui;
};
