#pragma once

#include <QSortFilterProxyModel>
#include "commongui_global.h"

class COMMONGUI_EXPORT SortFilterProxyModel : public QSortFilterProxyModel
{
	Q_OBJECT

public:
	SortFilterProxyModel(QObject *parent = nullptr);
	~SortFilterProxyModel();

protected:
	virtual bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const;
};
