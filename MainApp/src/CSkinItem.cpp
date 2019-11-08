#include "CSkinItem.h"
#include <QPainter>
static const int Margin = 5;

CSkinItem::CSkinItem(QWidget *parent)
	: QLabel(parent), mIsEnter(false),
	mIsChecked(false),borderColor(Qt::red),
	contentColor(Qt::red)
{
	this->setFixedSize(94 + Margin * 2,94 + Margin * 2);
	this->setMouseTracking(true);
	this->setCursor(Qt::PointingHandCursor);
}

CSkinItem::~CSkinItem()
{
}

void CSkinItem::setChecked(bool checked)
{
	this->mIsChecked = checked;
	this->update();
}

bool CSkinItem::isChecked()
{
	return this->mIsChecked;
	this->update();
}

void CSkinItem::setData(const StyleStruct &data)
{
	this->mData = data;
}

const StyleStruct& CSkinItem::getData()
{
	return this->mData;
}

void CSkinItem::setContentText(const QString &text)
{
	this->contentText = text;
	this->update();
}

void CSkinItem::setBorderColor(const QColor &borderColor)
{
	this->borderColor = borderColor;
	this->update();
}

void CSkinItem::setContentColor(const QColor &contentColor)
{
	this->contentColor = contentColor;
	this->borderColor = contentColor;
	this->update();
}

void CSkinItem::paintEvent(QPaintEvent *event)
{
	QPainter dc(this);
	dc.setRenderHint(QPainter::SmoothPixmapTransform);
	dc.fillRect(this->rect(), Qt::transparent);
	dc.save();
	dc.setPen(borderColor);
	QRect rect(Margin, Margin, this->rect().width() - Margin * 2, this->rect().height() - Margin * 2);
	dc.fillRect(rect,contentColor);
	if (mIsEnter)
		dc.drawRect(rect.adjusted(-2,-2,1,1));

	dc.restore();

	/*»­ÎÄ×Ö*/
	dc.save();
	QFont font;
	font.setPixelSize(12);
	dc.setFont(font);
	dc.setPen(Qt::white);
	int textHeight = dc.fontMetrics().height() + 5;
	dc.translate(rect.x(), rect.y() +  (rect.height() - textHeight));

	QRect rectText(0, 0, rect.width(), textHeight);
	dc.fillRect(rectText,QColor(0,0,0,80));
	QTextOption option;
	option.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
	dc.drawText(rectText.adjusted(5,0,0,0), contentText, option);
	dc.restore();

	if (mIsChecked)
	{
		dc.drawPixmap(this->width() - 20, this->height() - 20, 20, 20, QPixmap(":/BRSystem/images/select.png"));
	}

	rect = this->rect();
	rect.adjust(15, 15, -15, -15);
	dc.drawPixmap(rect, QPixmap(":/BRSystem/images/logo.png").scaled(rect.size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));

	QLabel::paintEvent(event);
}

void CSkinItem::enterEvent(QEvent *event)
{
	this->mIsEnter = true;
	this->update();
	QLabel::enterEvent(event);
}

void CSkinItem::leaveEvent(QEvent *event)
{
	this->mIsEnter = false;
	this->update();
	QLabel::leaveEvent(event);
}

void CSkinItem::mouseReleaseEvent(QMouseEvent *event)
{
	if (!mIsChecked)
		mIsChecked = true;
	emit clicked();
	emit checked(mIsChecked);
	this->update();
	QLabel::mouseReleaseEvent(event);
}
