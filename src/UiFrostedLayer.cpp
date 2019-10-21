#include "UiFrostedLayer.h"
#include <QGraphicsBlurEffect >
#include <QPainter>

UiFrostedLayer::UiFrostedLayer(QWidget *parent)
	: QWidget(parent)
{
	this->setAttribute(Qt::WA_TranslucentBackground);
	this->setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
}

UiFrostedLayer::~UiFrostedLayer()
{
}

void UiFrostedLayer::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setPen(Qt::transparent);
	painter.setBrush(QColor(1, 1, 1, 175));
	painter.drawRoundedRect(rect(), 3, 3);
	QWidget::paintEvent(event);
}

void UiFrostedLayer::resizeEvent(QResizeEvent *event)
{
	QWidget::resizeEvent(event);
}
