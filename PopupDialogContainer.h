#pragma once

#include <QDialog>
#include <QPushButton>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QPointer>
#include "BaseWidget.h"

class PopupDialogContainer : public QWidget
{
	Q_OBJECT

public:
	PopupDialogContainer(QWidget *parent = Q_NULLPTR,bool isModal = true);
	~PopupDialogContainer();

public:
	static void showPopupDialog(BaseWidget *widget, QWidget *parent = nullptr, bool isModal = true);
	void addWidget(BaseWidget *widget);

protected:
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void resizeEvent(QResizeEvent *event);

private:
	QPushButton *mCloseBtn;
	QHBoxLayout *mLayout;
	QPoint m_dragPoint;
	bool m_bCanMove;
	bool mIsModal;
};
