#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include "commongui_global.h"

class BubbleContentWidget;

class COMMONGUI_EXPORT BubbleTipWidget : public QWidget
{
	Q_OBJECT

public:
	BubbleTipWidget(QWidget *content = nullptr, QWidget *parent = Q_NULLPTR);
	~BubbleTipWidget();

	enum ArrowDirection { None = 0, Left, Top, Right, Bottom };

public:
	void fadeIn();

protected:
	void paintEvent(QPaintEvent *event);

private:
	QPixmap mPixmapMask;
	QWidget *_pContent;
	QHBoxLayout *mLayout;
	BubbleContentWidget *_pBackground;
};


/* 
* File : BubbleTipWidget.h
*
* Author : Xiewei
*
* Date : 2019-10-24
*
* Description : 
*
*/

class BubbleContentWidget : public QWidget
{
	Q_OBJECT

	Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor)
		Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor)
		Q_PROPERTY(QColor shadowColor READ shadowColor WRITE setShadowColor)

public:
	BubbleContentWidget( QWidget *parent = Q_NULLPTR);
	~BubbleContentWidget();

	void setBackgroundColor(const QColor &color);
	const QColor &backgroundColor() const;

	void setBorderColor(const QColor &color);
	const QColor &borderColor() const;

	void setShadowColor(const QColor &color);
	const QColor &shadowColor() const;

private:
	void drawMask(QPainter *painter);

protected:
	void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent *event);

private:
	QColor _borderColor;
	QColor _backgroundColor;
	QColor _shadowColor;
};
