#pragma once

#include "BaseDialog.h"
#include "ui_UiTaskPrice.h"
#include "webHandler.h"


class UiTaskPrice : public BaseDialog {
    Q_OBJECT

public:
	UiTaskPrice(QWidget* parent = Q_NULLPTR);
	~UiTaskPrice();
	void getConfig();

signals:
	void checkFinished();

private slots:
	void onRequestCallback(const ResponData & data);

private:
    Ui::UiTaskPrice ui;
};
