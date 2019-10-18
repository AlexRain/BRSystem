#pragma once

#include <QItemDelegate>

class ReadOnlyDelegate : public QItemDelegate
{
	Q_OBJECT

public:
	ReadOnlyDelegate(QObject *parent);
	~ReadOnlyDelegate();

protected:
	QWidget *createEditor(QWidget *parent,
		const QStyleOptionViewItem &option,
		const QModelIndex &index) const override;

	void paint(QPainter *painter,
		const QStyleOptionViewItem &option,
		const QModelIndex &index) const override;
};
