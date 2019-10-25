#pragma once

#include <QWidget>
#include <QModelIndex>
#include <QStandardItemModel>
#include "define.h"
#include "CApplication.h"

class CSearchLineEdit;
class QTableView;
class SortFilterProxyModel;

class CustomTableModel :public QStandardItemModel {
	Q_OBJECT

public:
	explicit CustomTableModel(QObject *parent = nullptr) {}
	~CustomTableModel() {};

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const {
		if (!index.isValid())
			return QVariant();

		switch (role)
		{
		case Qt::BackgroundRole:
		{
			if (index.column() == TableHeader::Order)
				return CApp->getStyledWidget().backgroundColor();
		}
		}
		return QStandardItemModel::data(index,role);
	}
};

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
	CustomTableModel *mModel;
	SortFilterProxyModel *pProxyModel;
	QList<BorrowInfo> mListData;
};
