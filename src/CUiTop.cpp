#include "CUiTop.h"
#include <QApplication>
#include <QStyle>
#include "define.h"
#include "UiChangeSkin.h"

CUiTop::CUiTop(QWidget *parent)
	: QWidget(parent), mStates(Qt::WindowNoState)
{
	ui.setupUi(this);
	ui.label_logo->setPixmap(QPixmap("images/healson_logo.png").scaled(ui.label_logo->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
	this->windowStateChanged(Qt::WindowNoState);
}

CUiTop::~CUiTop()
{
}

void CUiTop::windowStateChanged(Qt::WindowStates states)
{
	if (states == Qt::WindowMaximized || states == Qt::WindowNoState) {
		ui.btn_max->setProperty("isMaximized",(states == Qt::WindowMaximized));
		ui.btn_max->style()->unpolish(ui.btn_max);
		ui.btn_max->style()->polish(ui.btn_max);
		mStates = states;
	}
}

void CUiTop::on_btn_changeSkin_clicked()
{
	emit showChangeSkinDlg();
}

void CUiTop::on_btn_min_clicked()
{
	emit aboutToChangeWindowState(StateMin);
}

void CUiTop::on_btn_max_clicked()
{
	emit aboutToChangeWindowState(StateNormalOrMax);
}

void CUiTop::on_btn_close_clicked()
{
	emit appAboutToExit();
}
