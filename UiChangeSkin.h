#pragma once

#include <QDialog>
#include "define.h"

class CSkinItem;

class UiChangeSkin : public QDialog
{
	Q_OBJECT

public:
	UiChangeSkin(QWidget *parent = Q_NULLPTR);
	~UiChangeSkin();

private slots:
	void itemChecked(bool checked);
private:
	QMap<StyleType, CSkinItem*> mapItems;

};
