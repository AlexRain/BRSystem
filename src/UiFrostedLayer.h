#pragma once

#include <QWidget>
#include <QPixmap>

class UiFrostedLayer : public QWidget
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
