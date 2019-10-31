#pragma once

#include <QWidget>
#include <QCloseEvent>
#include "commongui_global.h"

class COMMONGUI_EXPORT BaseWidget : public QWidget
{ 
	Q_OBJECT

public:
	BaseWidget(QWidget *parent = nullptr);
	virtual ~BaseWidget();

public:
	virtual void windowStateChanged(Qt::WindowStates states) {};

protected:
	void paintEvent(QPaintEvent *event);
	void closeEvent(QCloseEvent *e);

signals:
	void closed();
};
