#pragma once
#include "NetworkDefine.h"
#include "network_global.h"
#include <QThreadPool>
#include <QtCore>
#include <QNetworkAccessManager>
#include <QRunnable>
#include <QQueue>
#include <QtNetwork>

class NETWORK_EXPORT MyThreadRunable:public QObject, public QRunnable
{
	Q_OBJECT
public:
	static MyThreadRunable* instance()
	{
		static MyThreadRunable myThreadRunable;
		return &myThreadRunable;
	}

	MyThreadRunable(QObject* parent = nullptr);

	MyThreadRunable(MyTask task,QObject* parent = nullptr);
	
	~MyThreadRunable();
	bool pause;

	static bool bindDataCallback(const QObject* receiver, const char* method, const QObject* excutor);

	static bool bindErrorCallback(const QObject* receiver, const char* method, const QObject* excutor);

	static bool bindTaskStatus(const QObject* receiver, const char* method, const QObject* excutor);

	static bool bindPauseTask(const QObject* receiver, const char* method);

	static bool bindAddPauseQueueTask(const QObject* receiver, const char* method, const QObject* excutor);

	static void restartTask();

	static bool getPause();

signals:
	void pasueAddQueue(const MyTask& task);

	void showTaskStatus(const int, const QString, const int, const int);

	void requestFinishedCallback(const ResponData& data, const QString&);

	void requestWithError(const ResponData& data, NetworkRequestError errorType, const QString& errorString);

private slots:
	void pasueTask(const bool&);

private:
	void doRequest(MyTask task);
private:
	QQueue<MyTask> myTaskQueue;
	QThreadPool* pool;
	MyTask task;
	QNetworkAccessManager* manager = nullptr;
protected:
	void run();
};


