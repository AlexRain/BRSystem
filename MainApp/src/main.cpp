#include "BRSystem.h"
#include "CApplication.h"
#include "CStyleManager.h"
#include "DialogMsg.h"
#include "PopupDialogContainer.h"
#include "UiFrostedLayer.h"
#include "UiLogin.h"
#include "UpgradeHelper.h"
#include "define.h"
#include <QDebug>
#include <QDesktopServices>
#include <QFile>
#include <QObject>
#include <QPointer>
#include <QStyleFactory>
#include <QTranslator>
#include <QtWidgets/QApplication>

QPointer<BRSystem> logPrinter = nullptr;

void outputMessage(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    /*if (logPrinter)
        logPrinter->outputMessage(type, context, msg);*/
}

using namespace std;

void relogin(void);

QString gstrFilePath = "";

int main(int argc, char* argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    CApplication a(argc, argv);
    gstrFilePath = QCoreApplication::applicationFilePath();
    a.setWindowIcon(QIcon("images/app.ico"));
    a.setApplicationName(QObject::tr("feng he network"));

    QTranslator translator_zh_CN;
    translator_zh_CN.load(":/translation/lang_zh.qm");
    a.installTranslator(&translator_zh_CN);

    StyleStruct style = CStyleManager::getInstance().getCurrentStyleStruct();
    StyleHelper::loadAppStyle(style.cssFile);

#ifndef _DEBUG
    //check new version
    {
        UpgradeHelper checkHelper;
        checkHelper.CheckUpgrade();
        auto result = checkHelper.GetCheckResult();
        if (result.needUpdate && result.force_update) {
            int code = DialogMsg::question(nullptr, QObject::tr("question"), QObject::tr("find a new version, please up to date."), QMessageBox::Ok);
            if (code == QMessageBox::Ok) {
                QDesktopServices::openUrl(QUrl::fromEncoded(result.download_url.toUtf8()));
            }
            return 0;
        }
    }
#endif // _DEBUG

    /*主界面*/
    int result = 0;
    do {
        /*登录界面*/
        UiLogin login;

        if (QDialog::Accepted != login.fadeIn())
            return 0;

        BRSystem w;
        logPrinter = &w;
        a.setMainWidget(&w);
        w.show();
        result = a.exec();
        if (result == 2) {
            atexit(relogin);
        }
    } while (result == RESTART_CODE);

    return result;
}

//这里启用重新启动一个新的示例
void relogin()
{
    QProcess process;
    process.startDetached(gstrFilePath);
}
