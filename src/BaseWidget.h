#pragma once

#include <QWidget>
#include <QCloseEvent>

class BaseWidget : public QWidget
{
	Q_OBJECT

public:
	BaseWidget(QWidget *parent = nullptr);
	~BaseWidget();

public:
	virtual void windowStateChanged(Qt::WindowStates states) {};

protected:
	void paintEvent(QPaintEvent *event);
	void closeEvent(QCloseEvent *e);

signals:
	void closed();
};
