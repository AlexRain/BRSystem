#include "NetworkDefine.h"
#include "define.h"
#include "UiTaskPrice.h"
#include <QEventLoop>

UiTaskPrice::UiTaskPrice(QWidget* parent)
	: BaseDialog(parent)
{
	ui.setupUi(this->getContent());
	WebHandler::bindDataCallback(this, SLOT(onRequestCallback(const ResponData&)));
	this->setWindowTitle(QString::fromLocal8Bit("»ý·Ö"));
	this->getConfig();
}

void UiTaskPrice::getConfig() {
	QEventLoop eventLoop;
	connect(this, &UiTaskPrice::checkFinished, &eventLoop, &QEventLoop::quit);
	RequestTask task;
	task.reqeustId = (quint64)this;
	task.apiIndex = API::getTaskConfig;
	WebHandler::instance()->Get(task);
	eventLoop.exec();
}

void UiTaskPrice::onRequestCallback(const ResponData& data)
{
	QVariantList  dataList;
	DataParseResult result;
	WebHandler::ParseJsonArrayData(data.dataReturned, dataList, &result);
	if (result.errorCode == DataParseResult::NoError) {
		if (dataList.count() > 0) {
			for (int i = 0; i < dataList.count(); i++)
			{
				QString rowString;
				QVariantMap map = dataList[i].toMap();
				QLabel* label = new QLabel();
				label->setText(map["name"].toString()+":"+map["value"].toString()+QString::fromLocal8Bit("·Ö"));
				ui.verticalLayout->addWidget(label);
			}

		}
	}
	else {
		qWarning("network busy");
	}
	emit checkFinished();

}

UiTaskPrice::~UiTaskPrice()
{
}


