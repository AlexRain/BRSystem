#pragma once

#include <QDialog>
#include <QMouseEvent>
#include <QLineEdit>
#include <QComboBox>

class UiLogin : public QDialog
{
	Q_OBJECT

public:
	UiLogin(QWidget *parent = Q_NULLPTR);
	~UiLogin();

public:
	void initUser();
	void verify();
	int fadeIn();

protected:
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);

private:
	QComboBox *m_pEditName;
	QLineEdit *m_pEditPwd;
	QPoint m_dragPoint;
	bool m_bCanMove;
	
};
