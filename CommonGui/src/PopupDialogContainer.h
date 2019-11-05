#pragma once

#include <QDialog>
#include <QPushButton>
#include <QGridLayout>
#include <QMouseEvent>
#include <QPointer>
#include <QLabel>
#include "BaseWidget.h"
#include "commongui_global.h"

class UiFrostedLayer;

class COMMONGUI_EXPORT PopupDialogContainer : public QWidget
{
	Q_OBJECT

private:
	enum ItemIndex
	{
		IndexTitle = 0,
		IndexContent = 1
	};

public:
	explicit PopupDialogContainer(QWidget *parent = Q_NULLPTR,const QString &title = QString(),bool isModal = true, bool showClose = true,bool resieGrip = false);
	~PopupDialogContainer();

public:
	static void showMainWidget(BaseWidget *widget);
	static void showMainWidgetFadeIn(BaseWidget *widget);
	static void showPopupDialog(BaseWidget *widget, QWidget *parent = nullptr, const QString &title = QString(), bool isModal = true, bool showCloseBtn = true,bool resizeGrip = false);
	static void showPopupDialogFadeIn(BaseWidget *widget, QWidget *parent = nullptr, const QString &title = QString(), bool isModal = true, bool showCloseBtn = true,bool resieGrip = false);
	void addWidget(BaseWidget *widget);
	void showMaxorNormal();
	void showMin();
	void showLayer();
	void hideLayer();
	void fadeIn();
	void fadeOut();
	void show();

protected:
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);
	void resizeEvent(QResizeEvent *event);
	void changeEvent(QEvent *event);
	void closeEvent(QCloseEvent *event);

private:
	QPointer<BaseWidget> _pCenterWidget;
	QPointer<QWidget> mParentWidget;
	UiFrostedLayer *mPLayer;
	QPushButton *mCloseBtn;
	QGridLayout *mLayout;
	QLabel *mLabelTitle;
	bool closeFlag;

	QString mTitleText;
	QPoint m_dragPoint;
	bool m_bCanMove;
	bool mIsModal;
	bool mUseFadeIn;
	bool mResizeGrip;
	Qt::WindowStates mLastState;
};
