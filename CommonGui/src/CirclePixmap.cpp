#include "CirclePixmap.h"
#include "define.h"
#include <QPainter>
#include <QMouseEvent>

CirclePixmap::CirclePixmap(QWidget *parent, int borderWidth, const QColor &borderColor)
	: QWidget(parent),mBorderWidth(borderWidth),mColorBorder(borderColor)
{
	this->mPixmap.load("images/defaulthead.png");
}

CirclePixmap::CirclePixmap(const QPixmap &pixmap, QWidget *parent,
	int borderWidth , const QColor &borderColor )
	: QWidget(parent), mPixmap(pixmap),mBorderWidth(borderWidth), mColorBorder(borderColor)
{
	this->mPixmap.load("images/defaulthead.png");
}

CirclePixmap::~CirclePixmap()
{
}

void CirclePixmap::setPixmap(const QPixmap &pix)
{
	this->mPixmap = pix;
	this->mPixmapOrigin = pix.scaled(this->size(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
	this->update();
}

void CirclePixmap::setBorderWidth(int borderWidth)
{
	this->mBorderWidth = borderWidth;
	this->update();
}

void CirclePixmap::setBorderColor(const QColor &color)
{
	this->mColorBorder = color;
	this->update();
}

void CirclePixmap::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.fillRect(rect(), Qt::transparent);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

	if (mBorderWidth > 0)
		painter.setPen(QPen(QBrush(mColorBorder),mBorderWidth));
	else
		painter.setPen(Qt::NoPen);

	QPainterPath circlePath;
	circlePath.addEllipse(0, 0, this->width(), this->height());

	painter.setClipPath(circlePath); // [1.2] 指定 clip 为圆的 path
	painter.drawPixmap(circlePath.boundingRect().toRect(), mPixmap); // [1.3] 绘制出来的 pixmap 只会显示在圆中的部分
	painter.drawPath(circlePath);

	QWidget::paintEvent(event);
}

void CirclePixmap::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		emit clicked();
	}
	QWidget::mouseReleaseEvent(event);
}

void CirclePixmap::resizeEvent(QResizeEvent *event)
{
	this->mPixmapOrigin = mPixmap.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	QWidget::resizeEvent(event);
}
