#include "BaseWidget.h"
#include <QIcon>
#include <QStyleOption>
#include <QPainter>
#include "define.h"

BaseWidget::BaseWidget(QWidget *parent)
	: QWidget(parent)
{
	this->setAttribute(Qt::WA_Mapped);
}

BaseWidget::~BaseWidget()
{
}

void BaseWidget::paintEvent(QPaintEvent *event)
{
	/*子类化控件使用样式表*/
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
	QWidget::paintEvent(event);
}

void BaseWidget::closeEvent(QCloseEvent *e)
{
	e->ignore();
	emit closed();
}
