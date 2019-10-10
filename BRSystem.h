#pragma once

#include <QWidget>

class CUiTop;
class CUiCenter;
class QMenuBar;
class QToolBar;

class BRSystem : public QWidget
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
