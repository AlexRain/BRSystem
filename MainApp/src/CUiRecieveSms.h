#pragma once

#include "NetworkDefine.h"
#include "CUiCenter.h"
#include "ui_CUiRecieveSms.h"

class CUiRecieveSms : public QWidget {
	Q_OBJECT

public:
	CUiRecieveSms(CUiCenter* c = Q_NULLPTR,QWidget* parent = Q_NULLPTR);
	~CUiRecieveSms();


private slots:
	void on_btn_get_sms_clicked();
	void showSms(QString sms);
	void slotLogContextMenu(const QPoint & pos);

signals:
	void recieveSms(QString sms);

private:
	Ui::CUiRecieveSms ui;

};