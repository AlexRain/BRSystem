#pragma once

#include <QWidget>
#include <QMap>
#include "BaseWidget.h"

class CUiTop;
class CUiCenter;
class QMenuBar;
class QToolBar;
class UiFrostedLayer;
class QVBoxLayout;

class BRSystem : public BaseWidget
{
	Q_OBJECT

public:
	BRSystem(QWidget *parent = nullptr);
	~BRSystem();

public:
	static void showCoverWidget(BaseWidget *content);
	void addwidget(BaseWidget *content);
	inline void showMainPage();

private:
	void init();

protected:
	virtual void windowStateChanged(Qt::WindowStates states) override;
	void resizeEvent(QResizeEvent *event);

private:
	CUiTop *mTopWidget;
	QMenuBar *mMenuBar;
	QToolBar *mToolbar;
	UiFrostedLayer *pLayer;
	QList<BaseWidget *> mListWidgets;
	QMap<int, QWidget*> m_mapWidget;
	QVBoxLayout *m_pContentLayout;
	BaseWidget *m_pCurrentWidget;
};
