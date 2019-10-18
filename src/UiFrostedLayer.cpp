#include "UiFrostedLayer.h"
#include <QGraphicsBlurEffect >
#include <QPainter>

UiFrostedLayer::UiFrostedLayer(QWidget *parent)
	: QWidget(parent)
{
	//this->setAttribute(Qt::WA_TranslucentBackground);
	this->setWindowFlags(windowFlags() | Qt::FramelessWindowHint);

	mPixmap.load("pic.jpg");
}

UiFrostedLayer::~UiFrostedLayer()
{
}

void UiFrostedLayer::paintEvent(QPaintEvent *event)
{
	QPainter dc(this);
	dc.drawPixmap(this->rect(),mPixmap);
	QWidget::paintEvent(event);
}

void UiFrostedLayer::resizeEvent(QResizeEvent *event)
{
	QWidget::resizeEvent(event);
}
