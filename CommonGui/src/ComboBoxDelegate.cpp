#include "ComboBoxDelegate.h"
#include "define.h"
#include "CStyleManager.h"
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
	editor->addItem(QString::fromLocal8Bit("未还"), 0);
	editor->addItem(QString::fromLocal8Bit("已还"), 1);
	editor->setCurrentIndex(0);
	connect(editor, SIGNAL(editingFinished()), SLOT(commitAndCloseEditor()));
	return editor;
}

void ComboBoxDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
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
		QColor colorItemCheck(0,0,0,80);
		switch (CStyleManager::getInstance().getCurrentStyleType())
		{
		case Dark:colorItemCheck.setRgb(44, 46, 50); break;
		case White:colorItemCheck.setRgb(223, 202, 136); break;
		default:
			break;
		}
		painter->fillRect(view_option.rect, colorItemCheck);
	}

	QTextOption textOption;
	textOption.setAlignment(Qt::AlignCenter);
	painter->drawText(view_option.rect, text, textOption);
	painter->restore();
}

void ComboBoxDelegate::commitAndCloseEditor()
{
	QComboBox *editor = qobject_cast<QComboBox*>(QObject::sender());
	emit commitData(editor);
	emit closeEditor(editor);
}
