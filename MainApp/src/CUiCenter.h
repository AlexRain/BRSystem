#pragma once

#include <QWidget>
#include "define.h"

class CSearchLineEdit;
class QTableView;
class QStandardItemModel;
class SortFilterProxyModel;

class CUiCenter : public QWidget
{
	Q_OBJECT

public:
	CUiCenter(QWidget *parent = Q_NULLPTR);
	~CUiCenter();

private:
	void initUi();
	//void initData();
	void initTableView();

	void appendRow(const BorrowInfo &info);
	void getBorrowData(BorrowInfo &info, int row);
	void setBorrowData(const BorrowInfo &info,int row);

private slots:
	void slotContextMenu(const QPoint &);
	void slotTableViewDoubleClicked(const QModelIndex &index);

private:
	CSearchLineEdit *mLineEdit;
	QTableView *mTableView;
	QStandardItemModel *mModel;
	SortFilterProxyModel *pProxyModel;
	QList<BorrowInfo> mListData;
};
