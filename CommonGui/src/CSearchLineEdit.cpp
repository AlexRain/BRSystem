#include "CSearchLineEdit.h"
#include "DialogMsg.h"
#include "define.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QDebug>

CSearchLineEdit::CSearchLineEdit(QWidget *parent)
	: QLineEdit(parent)
{
	this->setClearButtonEnabled(true);
	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->setAlignment(Qt::AlignCenter);
	layout->setContentsMargins(5, 0, 0, 0);
	layout->setSpacing(0);
	QPushButton *btn_search = new QPushButton(this);
	connect(btn_search, &QPushButton::clicked, this, [=]() {
		emit query(text());
	});
	btn_search->setObjectName("btn_search");
	//ÉèÖÃ¿ì½Ý¼ü
	btn_search->setShortcut(Qt::Key_Return);
	btn_search->setCursor(Qt::PointingHandCursor);
	btn_search->setFixedSize(17, 17);
	layout->addWidget(btn_search);
	layout->addStretch();
	connect(this, &QLineEdit::editingFinished, [=]() {
		qDebug() << "editingFinished";
		emit query(text());
	});
	connect(this, &QLineEdit::textChanged, [=](const QString & text) {
		qDebug() << "textChanged";
		emit query(text);
	});
}

CSearchLineEdit::~CSearchLineEdit()
{
}
