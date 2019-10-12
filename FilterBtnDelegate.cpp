#include "FilterBtnDelegate.h"
#include "define.h"
#include <QPainter>
#include <QPushButton>

FilterBtnDelegate::FilterBtnDelegate(QObject *parent)
	: QItemDelegate(parent)
{
}

FilterBtnDelegate::~FilterBtnDelegate()
{
}

void FilterBtnDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QStyleOptionViewItem  view_option(option);
	view_option.displayAlignment = Qt::AlignHCenter | Qt::AlignVCenter;
	if (view_option.state & QStyle::State_Selected) {
		view_option.state = view_option.state ^ QStyle::State_Selected;
	}

	bool data = index.model()->data(index, Qt::CheckStateRole).toBool();
	QString text = index.model()->data(index, Qt::DisplayRole).toString();

	QStyleOptionButton buttonStyle;
	buttonStyle.state = data ? QStyle::State_On : QStyle::State_Off;
	buttonStyle.state |= QStyle::State_Enabled;
	buttonStyle.iconSize = QSize(20, 20);
	QRect rectBtn = QRect(option.rect.x(), option.rect.y() + option.rect.height() / 2 - 10,
		option.rect.width() / 2, 20);
	buttonStyle.rect = rectBtn;

	QPushButton filterButton;
	filterButton.setObjectName("filterBtn");
	QApplication::style()->drawPrimitive(QStyle::PE_IndicatorButtonDropDown, &buttonStyle, painter, &filterButton);

	QTextOption textOption;
	QRect rectText = QRect(option.rect.x() + option.rect.width() / 2, option.rect.y(),
		option.rect.width() / 2, option.rect.height());
	textOption.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
	painter->drawText(rectText, text, textOption);
}
