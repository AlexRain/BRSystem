#pragma once

#include <QWidget>
#include "ui_BubbleTipWidget.h"
#include "commongui_global.h"

class COMMONGUI_EXPORT BubbleTipWidget : public QWidget
{
	Q_OBJECT

public:
	BubbleTipWidget(QWidget *parent = Q_NULLPTR);
	~BubbleTipWidget();

public:
	void fadeIn();

protected:
	void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent *event);

private:
	void drawMask(QPainter *painter);

private:
	QPixmap mPixmapMask;

private:
	Ui::BubbleTipWidget ui;
};
