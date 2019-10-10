#pragma once

#include <QWidget>
#include "ui_CUiTop.h"

class CUiTop : public QWidget
{
	Q_OBJECT

public:
	CUiTop(QWidget *parent = Q_NULLPTR);
	~CUiTop();
private slots:
	void on_btn_changeSkin_clicked();
private:
	Ui::CUiTop ui;
};
