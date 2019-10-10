#pragma once

#include <QWidget>

class CSearchLineEdit;
class QTableView;
class QStandardItemModel;

class CUiCenter : public QWidget
{
	Q_OBJECT

public:
	CUiCenter(QWidget *parent = Q_NULLPTR);
	~CUiCenter();

private:
	void initData();

private:
	CSearchLineEdit *mLineEdit;
	QTableView *mTableView;
	QStandardItemModel *mModel;
};
