#pragma once

#include <QWidget>
#include "ui_CUiTop.h"

class CUiTop : public QWidget
{
	Q_OBJECT

public:
	enum WindowState
	{
		StateMin,
		StateNormalOrMax,
	};

public:
	CUiTop(QWidget *parent = Q_NULLPTR);
	~CUiTop();

public:
	void windowStateChanged(Qt::WindowStates states);

private slots:
	void on_btn_changeSkin_clicked();
	void on_btn_min_clicked();
	void on_btn_max_clicked();
	void on_btn_close_clicked();

signals:
	void showChangeSkinDlg();
	void aboutToChangeWindowState(CUiTop::WindowState state);
	void appAboutToExit();

private:
	Ui::CUiTop ui;
	Qt::WindowStates mStates;
};
