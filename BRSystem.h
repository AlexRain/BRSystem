#pragma once

#include <QWidget>
#include "BaseWidget.h"

class CUiTop;
class CUiCenter;
class QMenuBar;
class QToolBar;

class BRSystem : public BaseWidget
{
	Q_OBJECT

public:
	BRSystem(QWidget *parent = nullptr);
	~BRSystem();

private:
	void init();

signals:
	void updateStyle();

private:
	CUiTop *mTopWidget;
	CUiCenter *mCenterWidget;
	QMenuBar *mMenuBar;
	QToolBar *mToolbar;
};
