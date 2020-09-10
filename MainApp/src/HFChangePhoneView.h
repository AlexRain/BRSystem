#pragma once

#include <QWidget>
#include "ui_HFChangePhoneView.h"
#include "BaseWidget.h"

class HFChangePhoneView : public BaseWidget
{
	Q_OBJECT

public:
	HFChangePhoneView(QWidget *parent = Q_NULLPTR);
	~HFChangePhoneView();

	void SetOldPhoneNumber(const QString &number);

private:
	Ui::HFChangePhoneView ui;
	QString oldNumber;
};