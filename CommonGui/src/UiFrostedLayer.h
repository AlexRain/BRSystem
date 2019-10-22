#pragma once

#include <QWidget>
#include <QPixmap>
#include "commongui_global.h"


class COMMONGUI_EXPORT UiFrostedLayer : public QWidget
{
	Q_OBJECT 

public:
	UiFrostedLayer(QWidget *parent = nullptr);
	~UiFrostedLayer();

protected:
	void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent *event);

private:
	QPixmap mPixmap;
};
