#include "MsgTaskManager.h"
#include <QTimer>

MsgTaskManager::MsgTaskManager(QObject* parent)
    : QObject(parent)
{
    qRegisterMetaType<MsgTask>("MsgTask");
}

MsgTaskManager::~MsgTaskManager()
{
}

void MsgTaskManager::AppendTask(MsgTask task, TaskChannel channel)
{
    if (!taskThread.isRunning()) {
        this->moveToThread(&taskThread);
        taskThread.start();
    }

    QMetaObject::invokeMethod(this, "excuteTask", Qt::QueuedConnection, Q_ARG(MsgTask, task), Q_ARG(TaskChannel, channel));
}

void MsgTaskManager::excuteTask(MsgTask task, TaskChannel channel)
{
    switch (channel) {
    case TaskChannel::HttpChannel: {
        if (taskHttpService == nullptr) {
            taskHttpService = new HttpTaskService;
        }
        taskHttpService->excuteTask(task);
        break;
    }
    case TaskChannel::TcpSocketChannel: {
        if (taskTcpService == nullptr) {
            taskTcpService = new TcpTaskService;
        }
        taskTcpService->excuteTask(task);
        break;
    }
    default:
        break;
    }

    if (taskQueue.isEmpty())
        QTimer::singleShot(0, this, SLOT(startNextTask()));

    taskQueue.enqueue(task);
}

void MsgTaskManager::startNextTask()
{
    MsgTask task = taskQueue.dequeue();
}
