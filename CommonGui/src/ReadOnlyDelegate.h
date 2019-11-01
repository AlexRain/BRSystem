#pragma once

#include <QItemDelegate>
#include "commongui_global.h"

class COMMONGUI_EXPORT ReadOnlyDelegate : public QItemDelegate
{
	Q_OBJECT

public:
	ReadOnlyDelegate(QObject *parent);
	virtual ~ReadOnlyDelegate();

protected:
	virtual QWidget *createEditor(QWidget *parent,
		const QStyleOptionViewItem &option,
		const QModelIndex &index) const override;

	virtual void paint(QPainter *painter,
		const QStyleOptionViewItem &option,
		const QModelIndex &index) const override;
};
