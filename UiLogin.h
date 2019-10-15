#pragma once

#include <QDialog>
#include "ui_UiLogin.h"

class UiLogin : public QDialog
{
	Q_OBJECT

public:
	UiLogin(QWidget *parent = Q_NULLPTR);
	~UiLogin();

public:
	void verify();

private:
	Ui::UiLogin ui;
	QLineEdit *_pEditName;
	QLineEdit *_pEditPwd;
};
