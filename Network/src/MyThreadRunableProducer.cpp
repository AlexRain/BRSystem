#include "MyThreadRunableProducer.h"
#include <windows.h>

MyThreadRunableProducer::MyThreadRunableProducer(QObject* parent)
	: QObject(parent) {
}

MyThreadRunableProducer::~MyThreadRunableProducer() {
}


//暂停任务槽
bool MyThreadRunableProducer::bindRestartTask(const QObject* receiver, const char* method)
{
	return QObject::connect(
		receiver,
		method,
		MyThreadRunableProducer::instance(),
		SLOT(restartQueue()),
		Qt::QueuedConnection);
}


//停止后的任务进入停止队列
void MyThreadRunableProducer::addPauseQueue(const MyTask& task) {
	qDebug() << "recieve task is " << task.taskName;
	pauseQueue.append(task);
}

//任务入队
void MyThreadRunableProducer::addQueue(MyTask mytask,QObject* reciver) {
	parentReciver = reciver;
	MyThreadRunable::restartTask();
	if (pauseQueue.isEmpty()) {
		pauseQueue.clear();
	}
	if (myTaskQueue.isEmpty())
		QTimer::singleShot(0, this, SLOT(outQueue()));
	qDebug() << "producer add myTask" << mytask.taskName;
	myTaskQueue.append(mytask);
}

//重新执行任务
void MyThreadRunableProducer::restartQueue() {
	MyThreadRunable::restartTask();
	if (pool == nullptr) {
		pool = new QThreadPool();
		pool->setMaxThreadCount(3);
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

//执行任务
void MyThreadRunableProducer::outQueue() {
	if (pool == nullptr) {
		pool = new QThreadPool();
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