#include "HFChargeView.h"
#include "UserSession.h"
#include "DialogMsg.h"

static const char* html = "<p><a href=\"%s\"><span style=\" text - decoration: underline; color:rgb(0,122,204); \"> %s</span></a></p>";
static const char* link = "www.baidu.com";
HFChargeView::HFChargeView(QWidget *parent)
	: BaseWidget(parent)
{
	ui.setupUi(this);
	QString linkText;
	linkText.sprintf(html, link,tr("get charge card?").toUtf8().constData());
	ui.label_get_card->setText(linkText);
	ui.label_get_card->installEventFilter(this);
	ui.label_get_card->setOpenExternalLinks(true);
	WebHandler::bindDataCallback(this, SLOT(onRequestCallback(const ResponData&)));
}

HFChargeView::~HFChargeView()
{
}


bool HFChargeView::eventFilter(QObject *obj, QEvent *event)
{
	if (qobject_cast<QLabel*>(obj) == ui.label_get_card&& event->type() == QEvent::MouseButtonRelease)
	{
		DialogMsg::question(this, tr("tips"),QString::fromLocal8Bit("QQÈº£º102153497"), QMessageBox::Ok);

		return true;
	}

	return false;
}

void HFChargeView::onRequestCallback(const ResponData& data)
{
	if (data.task.reqeustId == 0)
		return;
	if (data.task.reqeustId == (quint64)this) {
		QJsonObject dataObj;
		DataParseResult result;
		WebHandler::ParseJsonData(data.dataReturned, dataObj, &result);
		if (result.errorCode == DataParseResult::NoError) {
			emit chargeSuccess();
			close();
		}
		else {
			DialogMsg::question(this, tr("warning"), result.message, QMessageBox::Ok);
		}
	}
}

void HFChargeView::on_btn_charge_clicked()
{
	if (ui.lineEdit_card_number->text().isEmpty()) {
		int result = DialogMsg::question(this, tr("tips"), tr("please enter card number"), QMessageBox::Ok);
		return;
	}

	if (ui.lineEdit_password->text().isEmpty()) {
		int result = DialogMsg::question(this, tr("tips"), tr("please enter card secret"), QMessageBox::Ok);
		return;
	}

	RequestTask task;
	task.reqeustId = (quint64)this;
	task.headerObj.insert("uid", UserSession::instance().userData().uid);
	task.headerObj.insert("token", UserSession::instance().userData().token);
	task.bodyObj.insert("card_no", ui.lineEdit_card_number->text());
	task.bodyObj.insert("card_secret", ui.lineEdit_password->text());
	task.apiIndex = API::charge;
	WebHandler::instance()->Post(task);
}
