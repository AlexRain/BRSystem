#pragma once

#include "BaseWidget.h"
#include "ui_UiUsageRecord.h"

class UiUsageRecord : public BaseWidget
{
	Q_OBJECT

public:
	UiUsageRecord(QWidget *parent = Q_NULLPTR);
	~UiUsageRecord();

private:
	Ui::UiUsageRecord ui;
};
