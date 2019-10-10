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
	QItemDelegate::paint(painter, option, index);

	QString text = index.model()->data(index, Qt::DisplayRole).toString();

	painter->save();
	if (TableHeader::Status == index.column())
	{
		if (TOCH("ÒÑ»¹") == text)
			painter->setPen(Qt::green);
		else 
			painter->setPen(Qt::red);
	}

	QStyleOptionViewItem  view_option(option);
	view_option.displayAlignment = Qt::AlignHCenter | Qt::AlignVCenter;
	if (view_option.state & QStyle::State_Selected) {
		view_option.state = view_option.state ^ QStyle::State_Selected;
		painter->fillRect(view_option.rect, QColor(44,46,50));
	}

	QTextOption textOption;
	textOption.setAlignment(Qt::AlignCenter);
	painter->drawText(view_option.rect, text, textOption);
	painter->restore();
}
