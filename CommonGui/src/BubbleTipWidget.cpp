#include "BubbleTipWidget.h"
#include <QPainter>
#include <QBitmap>
#include <QStyleOption>
#include <QPropertyAnimation>
#include "CApplication.h"

const int borderWidth = 8;
const int arrowHeight = 10;
const int arrowWidth = 20;
const int contentMargins = 9;

BubbleTipWidget::BubbleTipWidget(ArrowDirection arowDirection,QWidget *conent, QWidget *parent)
	: QWidget(parent,Qt::Popup), _pContent(conent), mLayout(nullptr)
{
	this->setWindowOpacity(0.0);
	this->setAttribute(Qt::WA_TranslucentBackground);
	this->setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
	this->setWindowFlags(windowFlags() | Qt::NoDropShadowWindowHint);

	this->resize(conent->width() + borderWidth * 2,
		conent->height() + borderWidth * 2);

	_pBackground = new BubbleContentWidget(arowDirection,this);
	QHBoxLayout *layout_main = new QHBoxLayout(this);
	layout_main->setContentsMargins(borderWidth, borderWidth, borderWidth, borderWidth);
	layout_main->addWidget(_pBackground);

	QHBoxLayout *layout_content = new QHBoxLayout(_pBackground);
	switch (arowDirection)
	{
	case BubbleTipWidget::None:
		layout_content->setContentsMargins(contentMargins, contentMargins,
			contentMargins, contentMargins);
		break;
	case BubbleTipWidget::Right:
		layout_content->setContentsMargins(contentMargins + arrowHeight, contentMargins,
			contentMargins, contentMargins);
		break;
	case BubbleTipWidget::Bottom:
		layout_content->setContentsMargins(contentMargins, arrowHeight + contentMargins,
			contentMargins, contentMargins);
		break;
	case BubbleTipWidget::Left:
		layout_content->setContentsMargins(contentMargins, contentMargins, 
			contentMargins + arrowHeight, contentMargins);
		break;
	case BubbleTipWidget::Top:
		layout_content->setContentsMargins(contentMargins, contentMargins,
			contentMargins, contentMargins + arrowHeight);
		break;
	default:
		break;
	}
	if (conent){
		conent->setParent(this);
		layout_content->addWidget(conent);
	}
	
	_shadow = new QGraphicsDropShadowEffect(_pBackground);
	_shadow->setOffset(0, 0);
	_shadow->setColor(CApp->getStyledWidget().shadowColor());
	_shadow->setBlurRadius(10);
	_pBackground->setGraphicsEffect(_shadow);

	this->setFixedSize(conent->width() + borderWidth * 2,
		conent->height() + borderWidth * 2);
}

BubbleTipWidget::~BubbleTipWidget()
{
}

void BubbleTipWidget::showBubbleWidget(QWidget *content, const QPoint &globalPos,
	ArrowDirection arrowDirection /*= Top*/, QWidget *parent /*= nullptr*/)
{
	BubbleTipWidget *tips = new BubbleTipWidget(arrowDirection,content, parent);
	QPoint pos(globalPos.x() - tips->width() / 2, globalPos.y());
	switch (arrowDirection)
	{
	case BubbleTipWidget::None:
		break;
	case BubbleTipWidget::Left:
		pos.setX(globalPos.x() - tips->width());
		pos.setY(globalPos.y() - tips->height() / 2 - borderWidth);
		break;
	case BubbleTipWidget::Bottom:
		break;
	case BubbleTipWidget::Right:
		pos.setX(globalPos.x());
		pos.setY(globalPos.y() - tips->height() / 2 - borderWidth);
		break;
	case BubbleTipWidget::Top:
		pos.setY(globalPos.y() - tips->height() - borderWidth);
		break;
	default:
		break;
	}
	tips->move(pos);
	tips->fadeIn();
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
	if (_shadow && _pBackground) {
		_shadow->setColor(_pBackground->shadowColor());
	}
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
	QWidget::paintEvent(event);
}

/*内部widget实现（气泡弹窗的真正实现）*/

BubbleContentWidget::BubbleContentWidget(BubbleTipWidget::ArrowDirection arrowDirection,QWidget *parent )
	:QWidget(parent),_borderColor(QColor(35, 38, 44)),
	_backgroundColor(QColor("#16181c")),_arrowDirection(arrowDirection)
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

	painter->setPen(QPen(this->borderColor(), 1.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	painter->setBrush(this->backgroundColor());
	this->drawArrow(painter);
	painter->restore();
}

void BubbleContentWidget::drawArrow(QPainter *painter)
{

	/*绘制外框*/
	QVector<QLineF> lineLayer;
	QPainterPath painterPath; 

	/*绘制箭头*/
	const int arrowWidth = 20;
	QPainterPath pathArrow;
	QPolygonF arrowPolygon;
	QPolygonF layerPolygon;

	bool needRevise = false;

	switch (_arrowDirection)
	{
	case BubbleTipWidget::None:
		painterPath.addRoundRect(0, 0,
			this->width(),
			this->height(), 3.0, 3.0);
		break;
	case BubbleTipWidget::Right:
		painterPath.addRoundRect(arrowHeight, 0,
			this->width() - arrowHeight,
			this->height(), 3.0, 3.0);

		arrowPolygon << QPointF(arrowHeight,this->height() / 2.0 - arrowWidth / 2.0)
			<< QPointF(0.0, this->height() / 2.0)
			<< QPointF(arrowHeight, this->height() / 2.0 + arrowWidth / 2.0);

		lineLayer << QLineF(QPointF(arrowHeight, this->height() / 2.0 - arrowWidth / 2.0)
			, QPointF(arrowHeight, this->height() / 2.0 + arrowWidth / 2.0));
		lineLayer << QLineF(lineLayer[0].p1() + QPointF(0.1, 0), lineLayer[0].p2() + QPointF(0.1, 0));
		break;
	case BubbleTipWidget::Bottom:
		painterPath.addRoundRect(0, arrowHeight,
			this->width(),
			this->height() - arrowHeight, 3.0, 3.0);

		arrowPolygon << QPointF(this->width() / 2.0 - arrowWidth / 2.0, arrowHeight + 0.2)
			<< QPointF(this->width() / 2.0, 0.0)
			<< QPointF(this->width() / 2.0 + arrowWidth / 2.0, arrowHeight + 0.2);

		lineLayer << QLineF(QPointF(this->width() / 2.0 - arrowWidth / 2.0 + 0.2, arrowHeight + 0.2)
			, QPointF(this->width() / 2.0 + arrowWidth / 2.0 - 0.2, arrowHeight + 0.2));
		lineLayer << QLineF(lineLayer[0].p1() + QPointF(0,0.1),lineLayer[0].p2() + QPointF(0,0.1));
		needRevise = true;
		break;
	case BubbleTipWidget::Left:
		painterPath.addRoundRect(0, 0,
			this->width() - arrowHeight,
			this->height(), 3.0, 3.0);

		arrowPolygon << QPointF(this->width() - arrowHeight, this->height() / 2.0 - arrowWidth / 2.0)
			<< QPointF(this->width(), this->height() / 2.0)
			<< QPointF(this->width() - arrowHeight, this->height() / 2.0 + arrowWidth / 2.0);

		lineLayer << QLineF(QPointF(this->width() - arrowHeight, this->height() / 2.0 - arrowWidth / 2.0)
			, QPointF(this->width() - arrowHeight, this->height() / 2.0 + arrowWidth / 2.0));
		lineLayer << QLineF(lineLayer[0].p1() - QPointF(0.1, 0), lineLayer[0].p2() - QPointF(0.1, 0));
		break;
	case BubbleTipWidget::Top:

		painterPath.addRoundRect(0, 0,
			this->width(),
			this->height() - arrowHeight, 3.0, 3.0);
		arrowPolygon << QPointF(this->width() / 2.0 - arrowWidth / 2.0, this->height() - arrowHeight - 0.2)
			<< QPointF(this->width() / 2.0, this->height())
			<< QPointF(this->width() / 2.0 + arrowWidth / 2.0, this->height() - arrowHeight - 0.2);

		lineLayer <<QLineF(QPointF(this->width() / 2.0 - arrowWidth / 2.0, this->height() - arrowHeight - 0.2)
			, QPointF(this->width() / 2.0 + arrowWidth / 2.0, this->height() - arrowHeight - 0.2));
		lineLayer << QLineF(lineLayer[0].p1() - QPointF(0, 0.1), lineLayer[0].p2() - QPointF(0, 0.1));
		needRevise = true;
		break;
	default:
		break;
	}

	painter->drawPath(painterPath);
	
	/*画箭头*/
	if (!arrowPolygon.empty()) {
		pathArrow.addPolygon(arrowPolygon);
		painter->drawPath(pathArrow);
	}

	/*画遮罩层*/
	if (!lineLayer.empty()) {
		painter->save();
		painter->setPen(_backgroundColor);
		painter->drawLines(lineLayer);
		painter->restore();
	}

	if (!needRevise)
		return;

	painter->setPen(_backgroundColor);
	painter->drawLine(QPointF(this->width() / 2.0 - arrowWidth / 2.0 + 0.2, arrowHeight + 0.2), 
		QPointF(this->width() / 2.0 + arrowWidth / 2.0 - 0.2, arrowHeight + 0.2));
	painter->drawLine(QPointF(this->width() / 2.0 - arrowWidth / 2.0 + 0.2, arrowHeight + 0.3), 
		QPointF(this->width() / 2.0 + arrowWidth / 2.0 - 0.2, arrowHeight + 0.3));
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
