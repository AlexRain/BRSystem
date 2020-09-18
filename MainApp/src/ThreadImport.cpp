#include "ThreadImport.h"
#include <QFile>
#include <QTextStream>
#include <QThread>
#include <QTimer>

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
        if (listData.size() > 0) {
            data.qq = listData.at(0);
            if (listData.size() > 1) {
                data.password = listData.at(1);
            }
            if (listData.size() > 2) {
                data.phoneNumber = listData.at(2);
            }
            if (listData.size() > 3) {
                data.newPhoneNumber = listData.at(3);
            }
            emit addRow(data);
            line = in.readLine();
            QThread::msleep(30);
        }
    }

    startNextTask();
}

void ThreadImport::excuteTask(const QString& fileName)
{
    if (taskImportQueue.isEmpty())
        QTimer::singleShot(0, this, SLOT(startNextTask()));
    taskImportQueue.enqueue(fileName);
}
