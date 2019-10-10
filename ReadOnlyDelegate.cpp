#include "ReadOnlyDelegate.h"
#include <QPainter>
#include "define.h"

ReadOnlyDelegate::ReadOnlyDelegate(QObject *parent)
	: QItemDelegate(parent)
{
}

ReadOnlyDelegate::~ReadOnlyDelegate()
{
}

QWidget * ReadOnlyDelegate::createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	Q_UNUSED(parent)
	Q_UNUSED(option)
	Q_UNUSED(index)
	return nullptr;
}

void ReadOnlyDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	if (TableHeader::Status != index.column())
	{
		QString text = index.model()->data(index, Qt::DisplayRole).toString();

		QStyleOptionViewItem  view_option(option);
		view_option.displayAlignment = Qt::AlignHCenter | Qt::AlignVCenter;
		if (view_option.state & QStyle::State_Selected) {
			view_option.state = view_option.state ^ QStyle::State_Selected;
			painter->fillRect(view_option.rect, QColor(192, 192, 192));
		}

		//drawDisplay(painter, view_option, view_option.rect, text);
		QTextOption textOption;
		textOption.setAlignment(Qt::AlignCenter);
		painter->drawText(view_option.rect, text, textOption);
		painter->restore();
	}
}
