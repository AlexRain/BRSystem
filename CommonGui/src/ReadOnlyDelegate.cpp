#include "ReadOnlyDelegate.h"
#include <QPainter>
#include <QStyleOptionViewItem>

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
	QString text = index.model()->data(index, Qt::DisplayRole).toString();

	painter->save();
	QStyleOptionViewItem  view_option(option);
	view_option.displayAlignment = Qt::AlignHCenter | Qt::AlignVCenter;
	if (view_option.state & QStyle::State_Selected) {
		view_option.state = view_option.state ^ QStyle::State_Selected;
		QColor color = index.model()->data(index, Qt::BackgroundRole).value<QColor>();
		painter->fillRect(view_option.rect, QBrush(color));
	}

	QTextOption textOption;
	textOption.setAlignment(Qt::AlignCenter);
	if (painter->fontMetrics().width(text) > view_option.rect.width() - 8)
	{
		text = painter->fontMetrics().elidedText(text, Qt::ElideRight, view_option.rect.width() - 8);
	}
	painter->drawText(view_option.rect, text, textOption);
	painter->restore();
}
