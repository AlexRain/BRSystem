#include "MyThreadRunable.h"
#include "WebHandler.h"
#include <windows.h>
static const char* PAUSE_TASK = "pause";
MyThreadRunable::MyThreadRunable(QObject* parent)
	: QObject(parent), QRunnable(){
}

MyThreadRunable::MyThreadRunable(MyTask mtask,QObject* parent)
	: QObject(parent) {
	task = mtask;
}

MyThreadRunable::~MyThreadRunable() {
}



bool MyThreadRunable::bindDataCallback(const QObject* receiver, const char* method, const QObject* excutor)
{
	return QObject::connect(excutor,
		SIGNAL(requestFinishedCallback(const ResponData&, const QString&)),
		receiver,
		method,
		Qt::QueuedConnection);
}

bool MyThreadRunable::bindErrorCallback(const QObject* receiver, const char* method, const QObject* excutor)
{
	return QObject::connect(
		excutor,
		SIGNAL(requestWithError(const ResponData&, NetworkRequestError, const QString&)),
		receiver,
		method,
		Qt::QueuedConnection);
}



bool MyThreadRunable::bindTaskStatus(const QObject* receiver, const char* method, const QObject* excutor)
{
	return QObject::connect(excutor,
		SIGNAL(showTaskStatus(const int, const QString, const int, const int)),
		receiver,
		method,
		Qt::QueuedConnection);
}


bool MyThreadRunable::bindDoRequest(const QObject* receiver, const char* method)
{
	return QObject::connect(
		receiver,
		method,
		MyThreadRunable::instance(),
		SLOT(doRequest(MyTask)),
		Qt::QueuedConnection);
}




//失败任务信号
bool MyThreadRunable::bindAddPauseQueueTask(const QObject* receiver, const char* method, const QObject* excutor)
{
	return QObject::connect(excutor,
		SIGNAL(pasueAddQueue(const MyTask&)),
		receiver,
		method,
		Qt::QueuedConnection);
}


//暂停任务槽
bool MyThreadRunable::bindPauseTask(const QObject* receiver, const char* method)
{
	return QObject::connect(
		receiver,
		method,
		MyThreadRunable::instance(),
		SLOT(pasueTask(const bool&)),
		Qt::QueuedConnection);
}


//暂停任务
void MyThreadRunable::pasueTask(const bool& p) {
	qDebug() << "task is pause" << p;
	QSettings setting("app.ini", QSettings::IniFormat);
	setting.setValue(PAUSE_TASK, p);
}


void MyThreadRunable::restartTask() {
	QSettings setting("app.ini", QSettings::IniFormat);
	setting.setValue(PAUSE_TASK, false);
}


bool MyThreadRunable::getPause() {
	QSettings setting("app.ini", QSettings::IniFormat);
	bool pause = setting.value(PAUSE_TASK,false).toBool();
	return pause;
}

//执行任务
void MyThreadRunable::run() {
	emit showTaskStatus(task.task.index, QString::fromLocal8Bit("任务执行run"), TaskStatus::Doing, 0);

	QSettings setting("app.ini", QSettings::IniFormat);
	bool pause = setting.value(PAUSE_TASK,false).toBool();
	if (!pause) {
		emit showTaskStatus(task.task.index, QString::fromLocal8Bit("任务执行"), TaskStatus::Doing, 0);
		doRequest(task);
	}
	else {
		emit pasueAddQueue(task);
		emit showTaskStatus(task.task.index, QString::fromLocal8Bit("任务暂停"), TaskStatus::Stop,0);
	}
	return;

}

void MyThreadRunable::doRequest(MyTask currentTask) {
	if (nullptr == manager) {
		manager = new QNetworkAccessManager;
	}
	QNetworkRequest requstTask;
	ResponData dataCallback;
	dataCallback.task = currentTask.task;
	std::string strUrl = std::string(localServer) + getApi(API::createTask);
	QUrl url = QUrl::fromEncoded(QByteArray::fromStdString(strUrl));
	requstTask.setUrl(url);
	requstTask.setHeader(QNetworkRequest::ContentTypeHeader,
		"application/json");
	requstTask.setRawHeader("uid", QString::number(dataCallback.task.headerObj.value("uid").toInt()).toUtf8());
	requstTask.setRawHeader("token", dataCallback.task.headerObj.value("token").toString().toUtf8());
	requstTask.setRawHeader("position", QString::number(currentTask.task.index).toUtf8());

	// create a local python http request to excute task by task id.
	QTimer requestTimeOut;
	requestTimeOut.setSingleShot(true);
	QEventLoop loop;
	QNetworkReply* reply = nullptr;

	QByteArray bodyParam;
	WebHandler::setRequestBodyByParam(bodyParam, currentTask.task.bodyObj);
	emit showTaskStatus(task.task.index,QString::fromLocal8Bit("发送请求"), TaskStatus::Doing, 0);

	reply = manager->post(requstTask, bodyParam);

	connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
	connect(&requestTimeOut, &QTimer::timeout, &loop, &QEventLoop::quit);
	requestTimeOut.start(300 * 1000);
	loop.exec();

	if (requestTimeOut.isActive()) {
		requestTimeOut.stop();
		//currentRequest 判断是否成功
		int taskIndex = currentTask.task.index;
		qDebug() << "table index is " << taskIndex;
		if (reply->error()) {
			// request failed
			qDebug() << "network error:" << reply->error();
			qDebug("http request url=%s failed, error info=%s", reply->url().toEncoded().constData(), qPrintable(reply->errorString()));
			emit showTaskStatus(taskIndex, QString::fromLocal8Bit("服务器连接失败，请稍后..."), TaskStatus::Error,0);
			emit requestWithError(dataCallback, NetworkRequestError::Status_Error, reply->errorString());
		}
		else {
			int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
			if (statusCode == 200) {
				QByteArray data = reply->readAll();
				dataCallback.dataReturned = data;
				qDebug() << "get response data" << data.constData();
				emit requestFinishedCallback(dataCallback,"");
			}
			else {
				qDebug() << "ERROR IS " << 500;
				emit showTaskStatus(taskIndex, QString::fromLocal8Bit("服务器错误"), TaskStatus::Error,0);
				emit requestWithError(dataCallback, NetworkRequestError::Status_Error, "服务器错误");
			}
		}
		reply->deleteLater();
	}
	else {
		// request time out.
		qDebug() << "task time out";
		emit showTaskStatus(currentTask.task.index, QString::fromLocal8Bit("服务器超时1111"), TaskStatus::Error, 0);
		emit requestWithError(dataCallback, NetworkRequestError::Status_Error, "服务器超时");
		disconnect(reply,
			&QNetworkReply::finished,
			&loop,
			&QEventLoop::quit);
		qDebug() << "emit success";
		reply->abort();
		reply->close();
		reply->deleteLater();
	}
}
