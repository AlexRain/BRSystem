#include "CSearchLineEdit.h"
#include <QPushButton>
#include <QHBoxLayout>

CSearchLineEdit::CSearchLineEdit(QWidget *parent)
	: QLineEdit(parent)
{
	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->setAlignment(Qt::AlignCenter);
	layout->setContentsMargins(0, 0, 5, 0);
	layout->setSpacing(0);
	QPushButton *btn_search = new QPushButton(this);
	btn_search->setObjectName("btn_search");
	//ÉèÖÃ¿ì½Ý¼ü
	btn_search->setShortcut(Qt::Key_Enter);
	btn_search->setCursor(Qt::PointingHandCursor);
	btn_search->setFixedSize(17, 17);
	layout->addStretch();
	layout->addWidget(btn_search);
}

CSearchLineEdit::~CSearchLineEdit()
{
}
