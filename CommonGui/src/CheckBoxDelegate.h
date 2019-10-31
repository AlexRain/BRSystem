#pragma once

#include <QStyledItemDelegate>
#include "commongui_global.h"
#include <QColor>

class COMMONGUI_EXPORT CheckBoxDelegate : public QStyledItemDelegate
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
