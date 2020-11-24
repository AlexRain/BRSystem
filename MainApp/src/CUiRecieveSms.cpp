#include "CUiRecieveSms.h"
#include <QWidget>
#include <QMenu>


CUiRecieveSms::CUiRecieveSms(CUiCenter* c,QWidget* parent )
	: QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowTitle(QString::fromLocal8Bit("¶ÌÐÅ"));
	//ÓÒ¼ü²Ëµ¥
	ui.textBrowser->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui.textBrowser, SIGNAL(customContextMenuRequested(const QPoint&)),this, SLOT(slotLogContextMenu(const QPoint&)));

	connect(this, SIGNAL(recieveSms(QString)),c, SLOT(on_recieve_sms(QString)));

	connect(c, SIGNAL(showRecieveSms(QString)),this, SLOT(showSms(QString)));
}

CUiRecieveSms::~CUiRecieveSms() {


}

void CUiRecieveSms::slotLogContextMenu(const QPoint& pos) {
	qDebug() << "solt";
	QMenu* menu = new QMenu(ui.textBrowser);
	menu->addAction(QString::fromLocal8Bit("Çå¿Õ"), [=]() {
		ui.textBrowser->setText("");
	});
	menu->exec(QCursor::pos());
}


void CUiRecieveSms::showSms(QString sms) {
	ui.textBrowser->append(sms);
}


void CUiRecieveSms::on_btn_get_sms_clicked() {
	QString phone = ui.lineEdit_phone->text();
	qDebug() << "input " << phone;
	emit recieveSms(phone);
}

