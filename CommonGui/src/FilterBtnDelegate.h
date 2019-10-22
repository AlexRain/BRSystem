#pragma once

#include <QItemDelegate>
#include "commongui_global.h"

class COMMONGUI_EXPORT FilterBtnDelegate : public QItemDelegate
{
	Q_OBJECT

public:
	FilterBtnDelegate(QObject *parent);
	~FilterBtnDelegate();

protected:
	void paint(QPainter *painter,
		const QStyleOptionViewItem &option,
		const QModelIndex &index) const override;
};
