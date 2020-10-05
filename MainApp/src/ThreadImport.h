#pragma once

#include <QObject>
#include <QQueue>

struct ImportData {
    QString qq;
    QString password;
    QString phoneNumber;
    QString newPhoneNumber;
    int bizType;
};

static const char* separator = "----";

Q_DECLARE_METATYPE(ImportData)

class ThreadImport : public QObject {
    Q_OBJECT

public:
    ThreadImport(QObject* parent = nullptr);
    ~ThreadImport();

    void AddTask(const QString& fileName);

private slots:
    void startNextTask();
    void excuteTask(const QString& fileName);

signals:
    void taskFinished();
    void addRow(ImportData data);

private:
    QQueue<QString> taskImportQueue;
};
