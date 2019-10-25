#pragma once

#include <QWidget>
#include "commongui_global.h"

class COMMONGUI_EXPORT StyledWidget : public QWidget
{
	Q_OBJECT
		Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor)

public:
	StyledWidget(QWidget *parent = nullptr);
	~StyledWidget();

public:
	static StyledWidget& getInstance()
	{
		static StyledWidget obj;
		return obj;
	}

public:
	void setBackgroundColor(const QColor &color);
	const QColor &backgroundColor() const;

private:
	QColor _backgroundColor;
};
