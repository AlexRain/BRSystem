#include "CheckBoxDelegate.h"
#include <QCheckBox>
#include <QApplication>
#include <QMouseEvent>
#include <QTextOption>
#include <QPainter>
#include "define.h"
#include "CStyleManager.h"

CheckBoxDelegate::CheckBoxDelegate(QObject *parent)
	: QStyledItemDelegate(parent)
{
}

CheckBoxDelegate::~CheckBoxDelegate()
{
}

void CheckBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QStyleOptionViewItem  view_option(option);
	view_option.displayAlignment = Qt::AlignHCenter | Qt::AlignVCenter;
	if (view_option.state & QStyle::State_Selected) {
		view_option.state = view_option.state ^ QStyle::State_Selected;
		QColor colorItemCheck(0, 0, 0, 80);
		switch (CStyleManager::getInstance().getCurrentStyleType())
		{
		case Dark:colorItemCheck.setRgb(44, 46, 50); break;
		case White:colorItemCheck.setRgb(223, 202, 136); break;
		default:
			break;
		}
		painter->fillRect(view_option.rect, colorItemCheck);
	}

	if (index.column() == TableHeader::Order)
	{
		bool data = index.model()->data(index, Qt::CheckStateRole).toBool();
		bool showCheckBox = index.model()->data(index, Qt::UserRole).toBool();
		QString text = QString::number(index.row() + 1);

		if (showCheckBox)
		{
			QStyleOptionButton checkBoxStyle;
			checkBoxStyle.state = data ? QStyle::State_On : QStyle::State_Off;
			checkBoxStyle.state |= QStyle::State_Enabled;
			checkBoxStyle.iconSize = QSize(20, 20);
			QRect rectBox = QRect(option.rect.x(), option.rect.y() + option.rect.height() / 2 - 10,
				option.rect.width() / 2, 20);
			checkBoxStyle.rect = rectBox;

			QCheckBox checkBox;
			QApplication::style()->drawPrimitive(QStyle::PE_IndicatorCheckBox, &checkBoxStyle, painter, &checkBox);
		}

		QTextOption textOption;
		QRect rectText = QRect(option.rect.x() + option.rect.width() / 2, option.rect.y(),
			option.rect.width() / 2, option.rect.height());
		textOption.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
		painter->drawText(rectText, text, textOption);
	}
}

bool CheckBoxDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
	QRect decorationRect = option.rect;

	QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
	if (event->type() == QEvent::MouseButtonPress && decorationRect.contains(mouseEvent->pos())
		&&Qt::LeftButton == mouseEvent->button())
	{
		bool showCheckBox = index.model()->data(index, Qt::UserRole).toBool();
		if (index.column() == TableHeader::Order && showCheckBox)
		{
			bool data = model->data(index, Qt::CheckStateRole).toBool();
			model->setData(index, !data, Qt::CheckStateRole);
		}
	}

	return QStyledItemDelegate::editorEvent(event, model, option, index);
}
