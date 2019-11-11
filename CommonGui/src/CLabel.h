#pragma once

#include <QLabel>
#include "commongui_global.h"

class CLabel : public QLabel
{
	Q_OBJECT

public:
	explicit COMMONGUI_EXPORT CLabel(QWidget *parent = nullptr);
	~CLabel();

protected:
	void paintEvent(QPaintEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);

signals:
	void clicked();
	void pressed();
	void doubleClicked();
};
