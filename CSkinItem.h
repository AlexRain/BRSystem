#pragma once

#include <QLabel>
#include <QVariant>
#include "define.h"

class CSkinItem : public QLabel
{
	Q_OBJECT

public:
	CSkinItem(QWidget *parent);
	~CSkinItem();

	void setChecked(bool checked);
	bool isChecked();
	void setData(const StyleStruct &data);
	const StyleStruct& getData();

	void setContentText(const QString &text);
	void setBorderColor(const QColor &borderColor);
	void setContentColor(const QColor &contentColor);

protected:
	void paintEvent(QPaintEvent *event);
	void enterEvent(QEvent *event);
	void leaveEvent(QEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
signals:
	void clicked();
	void checked(bool checked);

private:
	bool mIsEnter;
	bool mIsChecked;
	QColor borderColor;
	QColor contentColor;
	QString contentText;
	StyleStruct mData;
};
