#pragma once

#include <QWidget>

class CSearchLineEdit;
class QTableView;

class CUiCenter : public QWidget
{
	Q_OBJECT

public:
	CUiCenter(QWidget *parent = Q_NULLPTR);
	~CUiCenter();

private:
	CSearchLineEdit *mLineEdit;
	QTableView *mTableView;
};
