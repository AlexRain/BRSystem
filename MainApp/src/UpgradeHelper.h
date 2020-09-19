#pragma once

#include "NetworkDefine.h"
#include <QObject>

class UpgradeHelper : public QObject {
    Q_OBJECT

public:
    struct UpgradeResult {
        bool needUpdate = false;
        bool force_update = true;
        QString desc;
        QString currentVersion;
        QString nextVersion;
        QString download_url;
    };

public:
    UpgradeHelper(QObject* parent = nullptr);
    ~UpgradeHelper();

    void CheckUpgrade();
    UpgradeHelper::UpgradeResult GetCheckResult();

private slots:
    void onRequestCallback(const ResponData&);

signals:
    void checkFinished();

private:
    bool needUpdate = false;
    UpgradeResult updateResult;
};
