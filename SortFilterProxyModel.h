#pragma once

#include <QSortFilterProxyModel>

class SortFilterProxyModel : public QSortFilterProxyModel
{
	Q_OBJECT

public:
	SortFilterProxyModel(QObject *parent = nullptr);
	~SortFilterProxyModel();

protected:
	virtual bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const;
};
