#include "DialogMsg.h"
#include "PopupDialogContainer.h"
#include "define.h"
#include <QStyleOption>
#include <QGraphicsDropShadowEffect>

DialogMsg::DialogMsg(QWidget *parent, const QString &title, const QString &text,QMessageBox::StandardButtons buttons, const QStringList &buttonsText)
	: QDialog(parent), mTitle(title), mContent(text),mButtons(buttons), mButtonClicked(QMessageBox::Cancel)
{
	ui.setupUi(this);
	this->setFixedSize(360, 216);
	this->setAttribute(Qt::WA_TranslucentBackground);
	this->setWindowFlags(windowFlags() | Qt::FramelessWindowHint);

	ui.label_title->setText(title);
	ui.label_content->setWordWrap(true);
	ui.label_content->setText(text);
	ui.btn_close->setCursor(Qt::PointingHandCursor);

	QList<int> listId;
	if (buttons&QMessageBox::Ok
		&& buttons&QMessageBox::Cancel)
	{
		listId << QMessageBox::Ok << QMessageBox::Cancel;
		mButtonsText << TOCH("确 定") << TOCH("取 消");
	}
	else if (buttons&QMessageBox::Ok)
	{
		listId << QMessageBox::Ok;
		mButtonsText << TOCH("确 定");
	}
	else if (buttons&QMessageBox::Cancel)
	{
		listId << QMessageBox::Cancel;
		mButtonsText << TOCH("取 消");
	}
	else
	{
		listId << QMessageBox::Ok;
	}

	if (buttonsText.isEmpty()){
		ui.layout_btn->addStretch();
		for(int i = 0;i < mButtonsText.size();++i)
		{
			QPushButton *btn = new QPushButton(mButtonsText.at(i), this);
			btn->setFixedSize(72, 30);
			connect(btn,&QPushButton::clicked,this,&DialogMsg::slotButtonClicked);
			btn->setProperty("buttonId",listId.at(i));
			ui.layout_btn->addWidget(btn);
		}
		ui.layout_btn->addStretch();
	}
	else {
		ui.layout_btn->addStretch();
		for (int i = 0; i < buttonsText.size(); ++i)
		{
			QPushButton *btn = new QPushButton(mButtonsText.at(i), this);
			btn->setFixedSize(72, 30);
			connect(btn, &QPushButton::clicked, this, &DialogMsg::slotButtonClicked);
			btn->setProperty("buttonId", listId.at(i));
			ui.layout_btn->addWidget(btn);
		}
		ui.layout_btn->addStretch();
	}

	QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(ui.contentWidget);
	shadow->setOffset(0, 0);
	shadow->setColor(Qt::black);
	shadow->setBlurRadius(10);
	ui.contentWidget->setGraphicsEffect(shadow);
}

DialogMsg::~DialogMsg()
{
}

QMessageBox::StandardButton DialogMsg::question(QWidget *parent, const QString &title, const QString &text,
	QMessageBox::StandardButtons buttons , const QStringList &buttonsText)
{
	QMessageBox::StandardButton ok = QMessageBox::Cancel;
	DialogMsg dlgMsg(parent,title,text,buttons, buttonsText);
	int result = dlgMsg.exec();
	if (result == QDialog::Accepted)
		ok = QMessageBox::Ok;
	return ok;
}

void DialogMsg::slotButtonClicked()
{
	QPushButton *btn = qobject_cast<QPushButton*>(sender());
	if (btn) {
		switch (btn->property("buttonId").toInt())
		{
		case QMessageBox::Ok:this->accept(); break;
		case QMessageBox::Cancel: this->reject(); break;
		default:
			this->reject();
			break;
		}
	}
}

void DialogMsg::on_btn_close_clicked()
{
	this->reject();
}

void DialogMsg::paintEvent(QPaintEvent *event)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
	QDialog::paintEvent(event);
}
