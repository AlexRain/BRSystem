#pragma once

#include <QStyledItemDelegate>
#include "commongui_global.h"
#include <QColor>

class COMMONGUI_EXPORT CheckBoxDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	/*初始化代理
	buddyColorIndex:表示使用列表哪一列的背景颜色
	*/
	CheckBoxDelegate(QObject *parent);
	~CheckBoxDelegate();

protected:
	void paint(QPainter *painter,
		const QStyleOptionViewItem &option,
		const QModelIndex &index) const override;

	bool editorEvent(QEvent *event, QAbstractItemModel *model,
		const QStyleOptionViewItem &option, const QModelIndex &index) override;
};
