#pragma once

#include "BaseWidget.h"
#include "ui_AboutView.h"

class AboutView : public BaseWidget
{
	Q_OBJECT

public:
	AboutView(QWidget *parent = Q_NULLPTR);
	~AboutView();

private:
	Ui::AboutView ui;
};
