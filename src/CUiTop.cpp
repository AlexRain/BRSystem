#include "CUiTop.h"
#include <QApplication>
#include <QStyle>
#include "define.h"
#include "UiChangeSkin.h"

CUiTop::CUiTop(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.label_logo->setPixmap(QPixmap("images/healson_logo.png").scaled(ui.label_logo->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
}

CUiTop::~CUiTop()
{
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
	exit(0);
}
