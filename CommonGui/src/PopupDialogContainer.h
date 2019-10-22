#pragma once

#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
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
	explicit PopupDialogContainer(QWidget *parent = Q_NULLPTR,const QString &title = QString(),bool isModal = true, bool showClose = true);
	~PopupDialogContainer();

public:
	static void showPopupDialog(BaseWidget *widget, QWidget *parent = nullptr, const QString &title = QString(), bool isModal = true,bool showCloseBtn = true);
	void addWidget(BaseWidget *widget);
	void showMaxorNormal();
	void showMin();
	void showLayer();
	void hideLayer();

protected:
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void resizeEvent(QResizeEvent *event);
	void changeEvent(QEvent *event);
	void closeEvent(QCloseEvent *event);

private:
	QPointer<BaseWidget> _pCenterWidget;
	QPointer<QWidget> mParentWidget;
	UiFrostedLayer *mPLayer;
	QPushButton *mCloseBtn;
	QVBoxLayout *mLayout;
	QLabel *mLabelTitle;
	bool closeFlag;

	QString mTitleText;
	QPoint m_dragPoint;
	bool m_bCanMove;
	bool mIsModal;
	Qt::WindowStates mLastState;
};
