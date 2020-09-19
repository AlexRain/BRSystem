#include "UpgradeHelper.h"
#include "UserSession.h"
#include "webHandler.h"
#include <QEventLoop>

UpgradeHelper::UpgradeHelper(QObject* parent)
    : QObject(parent)
{
    qRegisterMetaType<ResponData>("ResponData");
    WebHandler::bindDataCallback(this, SLOT(onRequestCallback(const ResponData&)));
}

UpgradeHelper::~UpgradeHelper()
{
}

void UpgradeHelper::CheckUpgrade()
{
    QEventLoop eventLoop;
    connect(this, &UpgradeHelper::checkFinished, &eventLoop, &QEventLoop::quit);
    RequestTask task;
    task.reqeustId = (quint64)this;
    task.queryObj.insert("version", QString::fromUtf8(app_version));
    task.apiIndex = API::checkUpdate;
    WebHandler::instance()->Get(task);
    eventLoop.exec();
}

void UpgradeHelper::onRequestCallback(const ResponData& data)
{
    if (data.task.reqeustId != 0 && data.task.reqeustId == (PointerValue)this) {
        needUpdate = false;
        UpgradeResult checkResult;
        QJsonObject dataObj;
        DataParseResult result;
        WebHandler::ParseJsonData(data.dataReturned, dataObj, &result);
        if (result.errorCode == DataParseResult::NoError) {
            if (dataObj.contains("current_version")) {
                QString currentVersion = dataObj.value("current_version").toString();
                QString localVersion(app_version);
                checkResult.currentVersion = currentVersion;
                checkResult.needUpdate = (1 == dataObj.value("is_update").toInt());
                checkResult.force_update = (1 == dataObj.value("is_force").toInt());
                checkResult.desc = dataObj.value("desc").toString();
                checkResult.download_url = dataObj.value("download_url").toString();
            } else {
                qWarning("check upgrade failed");
            }
        } else {
            qWarning("check upgrade failed");
        }
        updateResult = checkResult;
        emit checkFinished();
    }
}

UpgradeHelper::UpgradeResult UpgradeHelper::GetCheckResult()
{
    return updateResult;
}
