#include "CUiTop.h"

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
	
}
