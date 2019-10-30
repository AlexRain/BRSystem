#include "ReadOnlyDelegate.h"
#include <QPainter>
#include "define.h"
#include "CStyleManager.h"

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
	if (TableHeader::Status == index.column())
	{
		BorrowStatus status = (BorrowStatus)index.model()->data(index, Qt::UserRole).toInt();
		if (BorrowStatus::Returned == status) {
			painter->setPen(Qt::green);
			text = TOCH("�ѻ�");
		}
		else if (BorrowStatus::NotReturned == status) {
			painter->setPen(Qt::gray);
			QFont font = painter->font();
			font.setItalic(true);
			painter->setFont(font);
			text = TOCH("δ��");
		}
		else {
			painter->setPen(Qt::red);
			text = TOCH("��ʧ");
		}
	}else if (TableHeader::BorrowDate == index.column()){
		QDateTime date = index.model()->data(index, Qt::UserRole).toDateTime();
		text = date.toString("yyyy-MM-dd");
	}
	else if (TableHeader::UpdateDate == index.column()) {
		QDateTime date = index.model()->data(index, Qt::UserRole).toDateTime();
		text = date.toString("yyyy-MM-dd hh:mm");
	}

	QStyleOptionViewItem  view_option(option);
	view_option.displayAlignment = Qt::AlignHCenter | Qt::AlignVCenter;
	if (view_option.state & QStyle::State_Selected) {
		view_option.state = view_option.state ^ QStyle::State_Selected;
		QColor color = index.model()->data(index, Qt::BackgroundRole).value<QColor>();
		painter->fillRect(view_option.rect, color);
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
