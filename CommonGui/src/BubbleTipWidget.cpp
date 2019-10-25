#include "BubbleTipWidget.h"
#include <QPainter>
#include <QBitmap>
#include <QGraphicsDropShadowEffect>
#include <QStyleOption>
#include <QPropertyAnimation>

const int borderWidth = 8;
const int arrowHeight = 10;
const int arrowWidth = 20;

BubbleTipWidget::BubbleTipWidget(QWidget *conent, QWidget *parent)
	: QWidget(parent,Qt::Popup), _pContent(conent), mLayout(nullptr)
{
	this->setWindowOpacity(0.0);
	this->resize(275, 365);
	this->setAttribute(Qt::WA_TranslucentBackground);
	this->setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
	this->setWindowFlags(windowFlags() | Qt::NoDropShadowWindowHint);

	_pBackground = new BubbleContentWidget();
	QHBoxLayout *layout_main = new QHBoxLayout(this);
	layout_main->setContentsMargins(borderWidth, borderWidth, borderWidth, borderWidth);
	layout_main->addWidget(_pBackground);

	QHBoxLayout *layout_content = new QHBoxLayout(_pBackground);
	layout_content->setContentsMargins(1, arrowHeight + 30, 1, 30);
	if (conent){
		conent->setParent(this);
		conent->setObjectName("contentWidgetNoRadius");
		layout_content->addWidget(conent);
	}
	
	QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(_pBackground);
	shadow->setOffset(0, 0);
	shadow->setColor(QColor(0,0,0,80));
	shadow->setBlurRadius(10);
	_pBackground->setGraphicsEffect(shadow);
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
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
	QWidget::paintEvent(event);
}


/*内部widget实现（气泡弹窗的真正实现）*/

BubbleContentWidget::BubbleContentWidget(QWidget *parent )
	:QWidget(parent),_borderColor(QColor(35, 38, 44)),
	_backgroundColor(QColor("#16181c"))
{

}

BubbleContentWidget::~BubbleContentWidget()
{

}

void BubbleContentWidget::setBackgroundColor(const QColor &color)
{
	this->_backgroundColor = color;
	this->update();
}

const QColor & BubbleContentWidget::backgroundColor() const
{
	return this->_backgroundColor;
}

void BubbleContentWidget::setBorderColor(const QColor &color)
{
	this->_borderColor = color;
	this->update();
}

const QColor & BubbleContentWidget::borderColor() const
{
	return this->_borderColor;
}

void BubbleContentWidget::setShadowColor(const QColor &color)
{
	this->_shadowColor = color;
}

const QColor & BubbleContentWidget::shadowColor() const
{
	return this->_shadowColor;
}

void BubbleContentWidget::drawMask(QPainter *painter)
{
	painter->save();
	painter->setRenderHint(QPainter::Antialiasing);

	painter->setPen(QPen(this->borderColor(), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	painter->setBrush(this->backgroundColor());
	QPainterPath painterPath;
	painterPath.addRoundedRect(0, arrowHeight,
		this->width(),
		this->height() - arrowHeight, 3.0, 3.0);

	painter->drawPath(painterPath);

	/*绘制箭头*/
	const int arrowWidth = 20;
	QPainterPath pathArrow;
	QPolygonF arrowPolygon;
	arrowPolygon << QPointF(this->width() / 2.0 - arrowWidth / 2.0, arrowHeight + 1)
		<< QPointF(this->width() / 2.0, 1) << QPointF(this->width() / 2.0 + arrowWidth / 2.0, arrowHeight + 1);
	pathArrow.addPolygon(arrowPolygon);
	painter->drawPath(pathArrow);
	painter->restore();
}

void BubbleContentWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	this->drawMask(&painter);
}

void BubbleContentWidget::resizeEvent(QResizeEvent *event)
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
