#include "BubbleTipWidget.h"
#include <QPainter>
#include <QBitmap>
#include <QGraphicsDropShadowEffect>
#include <QStyleOption>
#include <QPropertyAnimation>

const int borderWidth = 8;
const int arrowHeight = 11;
const int arrowWidth = 22;

BubbleTipWidget::BubbleTipWidget(QWidget *parent)
	: QWidget(parent,Qt::Popup)
{
	ui.setupUi(this);
	this->setWindowOpacity(0.0);
	this->setAttribute(Qt::WA_TranslucentBackground);
	this->setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
	this->setWindowFlags(windowFlags() | Qt::NoDropShadowWindowHint);

	QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(ui.contentWidget);
	shadow->setOffset(0, 0);
	shadow->setColor(QColor(0,0,0,80));
	shadow->setBlurRadius(10);
	ui.contentWidget->setGraphicsEffect(shadow);
}

BubbleTipWidget::~BubbleTipWidget()
{
}

void BubbleTipWidget::fadeIn()
{
	this->show();
	QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity", this);
	animation->setDuration(150);
	animation->setStartValue(0.0);
	animation->setEndValue(1.0);
	animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void BubbleTipWidget::paintEvent(QPaintEvent *event)
{
	/*QPainter painter(this);
	this->drawMask(&painter);*/
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
	QWidget::paintEvent(event);
}

void BubbleTipWidget::resizeEvent(QResizeEvent *event)
{
	QBitmap bitmap(this->width(), this->height());
	QPainter painter(&bitmap);
	painter.fillRect(this->rect(), QColor(255, 255, 255));
	painter.save();
	painter.setPen(QColor(0, 0, 0));
	this->drawMask(&painter);
	this->setMask(bitmap);
	painter.restore();
	QWidget::resizeEvent(event);
}

void BubbleTipWidget::drawMask(QPainter *painter)
{
	painter->save();
	painter->setRenderHint(QPainter::Antialiasing);

	painter->setPen(QPen(QColor("#23262c"), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	painter->setBrush(QColor("#16181c"));
	QPainterPath painterPath;
	painterPath.addRoundedRect(borderWidth, arrowHeight + borderWidth,
		this->width() - borderWidth * 2,
		this->height() - borderWidth * 2 - arrowHeight, 3.0, 3.0);

	painter->drawPath(painterPath);
	painter->restore();
}
