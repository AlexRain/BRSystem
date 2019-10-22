#pragma once

/* 
* File : CirclePixmap.h
*
* Author : Xiewei
*
* Date : 2019-10-22
*
* Description : Ô²ÐÎÍ·Ïñ¿Ø¼þ
*
*/

#include <QWidget>
#include "commongui_global.h"

class COMMONGUI_EXPORT CirclePixmap : public QWidget
{
	Q_OBJECT

public:
	CirclePixmap(QWidget *parent = nullptr, int borderWidth = 0,const QColor &borderColor = Qt::gray);
	CirclePixmap(const QPixmap &pixmap, QWidget *parent = nullptr, int borderWidth = 0, const QColor &borderColor = Qt::gray);
	~CirclePixmap();

public:
	void setPixmap(const QPixmap &pix);
	void setBorderWidth(int borderWidth);
	void setBorderColor(const QColor &color);

protected:
	void paintEvent(QPaintEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void resizeEvent(QResizeEvent *event);

signals:
	void clicked();

private:
	QPixmap mPixmap;
	QPixmap mPixmapOrigin;
	int mBorderWidth;
	QColor mColorBorder;

};
