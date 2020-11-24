#include "MyThreadRunableProducer.h"
#include <windows.h>

MyThreadRunableProducer::MyThreadRunableProducer(QObject* parent)
	: QObject(parent) {
}

MyThreadRunableProducer::~MyThreadRunableProducer() {
}


//��ͣ�����
bool MyThreadRunableProducer::showPy(const QObject* receiver, const char* method) {
	return QObject::connect(
		MyThreadRunableProducer::instance(),
		SIGNAL(printLog(QString)),
		receiver,
		method,
		Qt::QueuedConnection);
}

bool MyThreadRunableProducer::bindRestartTask(const QObject* receiver, const char* method)
{
	return QObject::connect(
		receiver,
		method,
		MyThreadRunableProducer::instance(),
		SLOT(restartQueue()),
		Qt::QueuedConnection);
}


//ֹͣ����������ֹͣ����
void MyThreadRunableProducer::addPauseQueue(const MyTask& task) {
	qDebug() << "recieve task is " << task.taskName;
	pauseQueue.append(task);
}

//�������
void MyThreadRunableProducer::addQueue(MyTask mytask,QObject* reciver) {
	parentReciver = reciver;
	MyThreadRunable::restartTask();
	if (!pauseQueue.isEmpty()) {
		pauseQueue.clear();
	}
	if (myTaskQueue.isEmpty()) {
		pool = new QThreadPool();
		QTimer::singleShot(0, this, SLOT(outQueue()));
	}
	qDebug() << "producer add myTask" << mytask.taskName;
	myTaskQueue.append(mytask);
}

//����ִ������
void MyThreadRunableProducer::restartQueue() {
	MyThreadRunable::restartTask();
	emit printLog(QString("is destroyed %s").arg((pool == nullptr)));
	if (pool == nullptr) {
		emit printLog(QString("create pool %s"));
		pool = new QThreadPool();
		pool->setMaxThreadCount(3);
	}
	else {
		emit printLog(QString("clear pool %s"));
		pool->clear();
	}
	qDebug() << "restart task queue size is "<<pauseQueue.size();
	while (!pauseQueue.isEmpty()) {
		MyTask task = pauseQueue.dequeue();
		qDebug() << "restart consumer  myTask" << task.taskName;
		MyThreadRunable* myThreadRunable = new MyThreadRunable(task);
		qRegisterMetaType<MyTask>("MyTask");
		MyThreadRunable::bindAddPauseQueueTask(this, SLOT(addPauseQueue(const MyTask&)), myThreadRunable);

		MyThreadRunable::bindDataCallback(parentReciver, SLOT(onTaskRequestCallback(const ResponData&, const QString&)), myThreadRunable);
		MyThreadRunable::bindTaskStatus(parentReciver, SLOT(onTaskDo(const int, const QString, const int)), myThreadRunable);
		MyThreadRunable::bindErrorCallback(parentReciver, SLOT(onTaskRequestError(const ResponData&, NetworkRequestError, const QString&)), myThreadRunable);

		pool->start(myThreadRunable);

	}
}

void MyThreadRunableProducer::start() {
	if (!thread.isRunning()) {
		this->moveToThread(&thread);
		thread.start();
	}
}

void MyThreadRunableProducer::run() {
	outQueue();
}

//ִ������
void MyThreadRunableProducer::outQueue() {
	emit printLog(QString("is destroyed %1").arg((pool == nullptr)));
	if (pool == nullptr) {
		emit printLog(QString("create pool"));
		pool = new QThreadPool();
	}
	else {
		emit printLog(QString("clear pool"));
		pool->clear();
	}
	QSettings setting("app.ini", QSettings::IniFormat);
	int size = setting.value("thread_number").toInt();
	qDebug() << "current thread size is " << size;
	pool->setMaxThreadCount(size);
	while (!myTaskQueue.isEmpty()) {
		MyTask task = myTaskQueue.dequeue();
		qDebug() << "start consumer  myTask" << task.taskName;
		MyThreadRunable* myThreadRunable = new MyThreadRunable(task);
		qRegisterMetaType<MyTask>("MyTask");
		MyThreadRunable::bindAddPauseQueueTask(this, SLOT(addPauseQueue(const MyTask&)), myThreadRunable);
		MyThreadRunable::bindDataCallback(parentReciver, SLOT(onTaskRequestCallback(const ResponData&, const QString&)), myThreadRunable);
		MyThreadRunable::bindTaskStatus(parentReciver, SLOT(onTaskDo(const int, const QString, const int)), myThreadRunable);
		MyThreadRunable::bindErrorCallback(parentReciver, SLOT(onTaskRequestError(const ResponData&, NetworkRequestError, const QString&)), myThreadRunable);
		pool->start(myThreadRunable);
	}
}

void MyThreadRunableProducer::doTask() {

}