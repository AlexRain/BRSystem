#include "SortFilterProxyModel.h"
#include "define.h"

SortFilterProxyModel::SortFilterProxyModel(QObject *parent)
	: QSortFilterProxyModel(parent)
{
}

SortFilterProxyModel::~SortFilterProxyModel()
{
}

bool SortFilterProxyModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{
	if (!source_left.isValid() || !source_right.isValid())
		return false;

	if ((source_left.column() == TableHeader::BorrowDate) && (source_right.column() == TableHeader::BorrowDate)){
		QVariant leftData = sourceModel()->data(source_left, Qt::UserRole);
		QVariant rightData = sourceModel()->data(source_right, Qt::UserRole);

		if (leftData.canConvert<QDate>() && rightData.canConvert<QDate>())
		{
			return leftData.toDate() < rightData.toDate();
		}
	}
	else if ((source_left.column() == TableHeader::Status) && (source_right.column() == TableHeader::Status)) {
		QVariant leftData = sourceModel()->data(source_left, Qt::UserRole);
		QVariant rightData = sourceModel()->data(source_right, Qt::UserRole);

		if (leftData.canConvert<int>() && rightData.canConvert<int>())
		{
			return leftData.toInt() < rightData.toInt();
		}
	}

	return QSortFilterProxyModel::lessThan(source_left, source_right);
}
