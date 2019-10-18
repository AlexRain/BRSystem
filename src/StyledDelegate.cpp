#include "StyledDelegate.h"

StyledDelegate::StyledDelegate(QObject *parent)
	: QStyledItemDelegate(parent)
{
}

StyledDelegate::~StyledDelegate()
{
}

void StyledDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const 
{
	QStyleOptionViewItem  view_option(option);
	if (view_option.state & QStyle::State_HasFocus) {
		view_option.state = view_option.state ^ QStyle::State_HasFocus;
	}

	QStyledItemDelegate::paint(painter, view_option, index);
}
