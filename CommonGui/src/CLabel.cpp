#include "CLabel.h"
#include <QStyleOption>
#include <QPainter>

CLabel::CLabel(QWidget *parent)
	: QLabel(parent)
{
}

CLabel::~CLabel()
{
}

void CLabel::paintEvent(QPaintEvent *event)
{
	/*子类化控件使用样式表*/
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
	QLabel::paintEvent(event);
}

void CLabel::mouseReleaseEvent(QMouseEvent *event)
{
	emit clicked();
	QLabel::mouseReleaseEvent(event);
}

void CLabel::mousePressEvent(QMouseEvent *event)
{
	emit pressed();
	QLabel::mousePressEvent(event);
}

void CLabel::mouseDoubleClickEvent(QMouseEvent *event)
{
	emit doubleClicked();
	QLabel::mouseDoubleClickEvent(event);
}
