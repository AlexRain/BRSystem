#include "CUiTop.h"
#include <QApplication>
#include <QStyle>
#include "define.h"
#include "UiChangeSkin.h"
#include "CirclePixmap.h"

CUiTop::CUiTop(QWidget *parent)
	: QWidget(parent), mStates(Qt::WindowNoState), headPixmap(nullptr)
{
	ui.setupUi(this);
	this->windowStateChanged(Qt::WindowNoState);
	headPixmap = new CirclePixmap(this);
	headPixmap->setFixedSize(30, 30);
	headPixmap->setPixmap(QPixmap("images/testhead01.jpg"));
	headPixmap->setCursor(Qt::PointingHandCursor);
	connect(headPixmap, &CirclePixmap::clicked, this, &CUiTop::clickProfile);
	ui.horizontalLayout->replaceWidget(ui.label_head, headPixmap);
	ui.label_head->hide();

	ui.label_logo->setMinimumHeight(33);
	QPixmap logo("images/logo.png");
	ui.label_logo->setPixmap(UiHelper::justPixmapByHeight(33,logo));
}

CUiTop::~CUiTop()
{
}

void CUiTop::setLogo(const QPixmap &logo)
{
	this->m_pixmapLogo = logo;
}

const QPixmap & CUiTop::logo() const
{
	return this->m_pixmapLogo;
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
