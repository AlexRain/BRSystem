#pragma once
#include "NetworkDefine.h"
#include "network_global.h"
#include "MyThreadRunable.h"
#include <QThread>
#include <QtCore>
#include <QNetworkAccessManager>


class NETWORK_EXPORT MyThreadPool :public QObject
{
	Q_OBJECT


public:
	MyThreadPool(QObject* parent = nullptr);

	~MyThreadPool();

	void startTask();

	void addQueue(MyTask mytask);

signals:

protected:
	virtual void run();

private:

	QThread thread;

	QQueue<MyTask> myQueue;
};


