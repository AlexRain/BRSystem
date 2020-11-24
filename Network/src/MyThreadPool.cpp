#include "MyThreadPool.h"
#include <QThreadPool>

MyThreadPool::MyThreadPool(QObject* parent): QObject(parent) {

}

MyThreadPool::~MyThreadPool() {
}


void MyThreadPool::startTask() {
	if (!thread.isRunning()) {
		this->moveToThread(&thread);
		thread.start();
	}
}
//任务入队
void MyThreadPool::addQueue(MyTask mytask) {
	qDebug() << "producer add myTask" << mytask.taskName;
	myQueue.append(mytask);
}


void MyThreadPool::run() {
	QThreadPool tp;
	tp.setMaxThreadCount(20);
	while (true)
	{
		if (!myQueue.empty())
		{
			MyTask task = myQueue.dequeue();
			qDebug() << "start consumer  myTask" << task.taskName;
			MyThreadRunable* myThreadRunable = new MyThreadRunable(task);
			qRegisterMetaType<MyTask>("MyTask");
			tp.start(myThreadRunable);
		}
		else
		{
			qDebug() << "wait pool";
			QThread::msleep(10);
		}
	}
}