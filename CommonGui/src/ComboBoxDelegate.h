#pragma once

#include <QItemDelegate>
#include "commongui_global.h"

class COMMONGUI_EXPORT ComboBoxDelegate : public QItemDelegate
{
	Q_OBJECT

public:
	ComboBoxDelegate(QObject *parent);
	~ComboBoxDelegate();

protected:
	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private slots:
	void commitAndCloseEditor();
};
