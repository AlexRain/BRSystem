#pragma once
#include "NetworkDefine.h"
#include "network_global.h"
#include "MyThreadRunable.h"
#include <QThreadPool>
#include <QtCore>
#include <QNetworkAccessManager>
#include <QRunnable>
#include <QQueue>


class NETWORK_EXPORT MyThreadRunableProducer :public QObject
{
	Q_OBJECT
public:
	void start();
	static MyThreadRunableProducer* instance()
	{
		static MyThreadRunableProducer myThreadRunableProducer;
		return &myThreadRunableProducer;
	}

	MyThreadRunableProducer(QObject* parent = nullptr);

	~MyThreadRunableProducer();

	void addQueue(MyTask mytask, QObject* reciver);

	void doTask();
	
	static QMap<QString, QString> taskList;

	static bool showPy(const QObject* receiver, const char* method);
	static bool bindRestartTask(const QObject* receiver, const char* method);

signals:
	void printLog(QString);

private slots:
	void outQueue();

	void restartQueue();


	void addPauseQueue(const MyTask& task);

private:
	QQueue<MyTask> myTaskQueue;

	QThreadPool* pool;
	QThread thread;
	
	QQueue<MyTask> pauseQueue;

	QObject* parentReciver;

protected:
	void run();
};


