#pragma once

#include <QWidget>
#include "ui_CUiTop.h"

class CirclePixmap;

class CUiTop : public QWidget
{
	Q_OBJECT

		Q_PROPERTY(QPixmap logo READ logo WRITE setLogo)

public:
	enum WindowState
	{
		StateMin,
		StateNormalOrMax,
	};

public:
	CUiTop(QWidget *parent = Q_NULLPTR);
	~CUiTop();

	void setLogo(const QPixmap &logo);
	const QPixmap &logo() const;

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
	void clickProfile();

private:
	Ui::CUiTop ui;
	Qt::WindowStates mStates;
	CirclePixmap *headPixmap;
	QPixmap m_pixmapLogo;
};
