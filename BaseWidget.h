#pragma once

#include <QDialog>
#include <QCloseEvent>

class BaseWidget : public QWidget
{
	Q_OBJECT

public:
	BaseWidget(QWidget *parent = nullptr);
	~BaseWidget();

	void paintEvent(QPaintEvent *event);

signals:
	void closed();

protected:
	void closeEvent(QCloseEvent *e);
};
