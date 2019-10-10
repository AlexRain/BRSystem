#include "ComboBoxDelegate.h"
#include "define.h"
#include <QComboBox>
#include <QPainter>

ComboBoxDelegate::ComboBoxDelegate(QObject *parent)
	: QItemDelegate(parent)
{
}

ComboBoxDelegate::~ComboBoxDelegate()
{
}

QWidget * ComboBoxDelegate::createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	QComboBox *editor = new QComboBox(parent);
	if (index.column() == 5) {
		editor->addItem(QString::fromLocal8Bit("未还"),0);
		editor->addItem(QString::fromLocal8Bit("已还"),1);
		editor->setCurrentIndex(0);

		connect(editor, SIGNAL(editingFinished()), SLOT(commitAndCloseEditor()));
		return editor;
	}
	else
	{
		return QItemDelegate::createEditor(parent, option, index);
	}
}

void ComboBoxDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	if (index.column() == 5)
	{
		QString text = index.model()->data(index, Qt::DisplayRole).toString();

		painter->save();
		if (TOCH("已还") == text)
		{
			painter->setPen(Qt::green);
		}
		else {
			painter->setPen(Qt::red);
		}

		QStyleOptionViewItem  view_option(option);
		view_option.displayAlignment = Qt::AlignHCenter | Qt::AlignVCenter;
		if (view_option.state & QStyle::State_Selected) {
			view_option.state = view_option.state ^ QStyle::State_Selected;
			painter->fillRect(view_option.rect,QColor(192,192,192));
		}

		//drawDisplay(painter, view_option, view_option.rect, text);
		QTextOption textOption;
		textOption.setAlignment(Qt::AlignCenter);
		painter->drawText(view_option.rect, text, textOption);
		painter->restore();
		//drawFocus(painter, view_option, view_option.rect);

	}
	else
	{
		QItemDelegate::paint(painter, option, index);
	}
}

void ComboBoxDelegate::commitAndCloseEditor()
{
	QComboBox *editor = qobject_cast<QComboBox*>(QObject::sender());
	emit commitData(editor);
	emit closeEditor(editor);
}
