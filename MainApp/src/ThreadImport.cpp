#include "ThreadImport.h"
#include <QFile>
#include <QTextStream>
#include <QThread>
#include <QTimer>

static const char* separator = "----";

ThreadImport::ThreadImport(QObject* parent)
    : QObject(parent)
{
}

ThreadImport::~ThreadImport()
{
}

void ThreadImport::AddTask(const QString& fileName)
{
    QMetaObject::invokeMethod(this, "excuteTask", Q_ARG(const QString&, fileName));
}

void ThreadImport::startNextTask()
{
    if (taskImportQueue.isEmpty()) {
        emit taskFinished();
        return;
    }

    QString currentTask = taskImportQueue.dequeue();
    QFile file(currentTask);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        file.close();
        startNextTask();
        return;
    }

    QTextStream in(&file);
    QString line = in.readLine();
    while (!line.isNull()) {
        ImportData data;
        QStringList listData = line.split(separator);
        if (3 == listData.size()) {
            data.qq = listData.at(0);
            data.password = listData.at(1);
            data.phoneNumber = listData.at(2);
        } else if (1 == listData.size()) {
            data.qq = listData.at(0);
        }
        emit addRow(data);
        line = in.readLine();
        QThread::msleep(30);
    }

    startNextTask();
}

void ThreadImport::excuteTask(const QString& fileName)
{
    if (taskImportQueue.isEmpty())
        QTimer::singleShot(0, this, SLOT(startNextTask()));
    taskImportQueue.enqueue(fileName);
}
