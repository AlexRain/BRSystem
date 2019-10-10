#pragma once

#include <QStyledItemDelegate>

class CheckBoxDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	CheckBoxDelegate(QObject *parent);
	~CheckBoxDelegate();

protected:
	void paint(QPainter *painter,
		const QStyleOptionViewItem &option,
		const QModelIndex &index) const override;

	bool editorEvent(QEvent *event, QAbstractItemModel *model,
		const QStyleOptionViewItem &option, const QModelIndex &index) override;
};
