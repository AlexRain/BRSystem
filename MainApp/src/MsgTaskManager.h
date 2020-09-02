#pragma once

#include "TaskService.h"
#include <QMutex>
#include <QObject>
#include <QQueue>
#include <QThread>

enum class TaskChannel { HttpChannel,
    TcpSocketChannel };
Q_ENUMS(TaskChannel)

class MsgTaskManager : public QObject {
    Q_OBJECT

public:
    MsgTaskManager(QObject* parent);
    ~MsgTaskManager();

    void AppendTask(MsgTask task, TaskChannel channel);

private slots:
    void startNextTask();
    void excuteTask(MsgTask task, TaskChannel channel);

private:
    QQueue<MsgTask> taskQueue;
    QMutex taskMutex;
    QThread taskThread;
    TaskService* taskHttpService = nullptr;
    TaskService* taskTcpService = nullptr;
};
