#pragma once

#include <QStyledItemDelegate>

class StyledDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	StyledDelegate(QObject *parent);
	~StyledDelegate();

protected:
	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};
