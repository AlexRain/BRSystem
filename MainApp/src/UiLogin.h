#pragma once

#include <QDialog>
#include <QMouseEvent>
#include <QLineEdit>

class UiLogin : public QDialog
{
	Q_OBJECT

public:
	UiLogin(QWidget *parent = Q_NULLPTR);
	~UiLogin();

public:
	void verify();
	int fadeIn();

protected:
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);

private:
	QLineEdit *_pEditName;
	QLineEdit *_pEditPwd;
	QPoint m_dragPoint;
	bool m_bCanMove;
	
};
