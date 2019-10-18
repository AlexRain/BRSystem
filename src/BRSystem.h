#pragma once

#include <QWidget>
#include "BaseWidget.h"

class CUiTop;
class CUiCenter;
class QMenuBar;
class QToolBar;
class UiFrostedLayer;

class BRSystem : public BaseWidget
{
	Q_OBJECT

public:
	BRSystem(QWidget *parent = nullptr);
	~BRSystem();

private:
	void init();

protected:
	virtual void windowStateChanged(Qt::WindowStates states) override;
	void resizeEvent(QResizeEvent *event);

private:
	CUiTop *mTopWidget;
	CUiCenter *mCenterWidget;
	QMenuBar *mMenuBar;
	QToolBar *mToolbar;
	UiFrostedLayer *pLayer;
};
