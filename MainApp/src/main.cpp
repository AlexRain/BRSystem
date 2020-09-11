#include "BRSystem.h"
#include "CApplication.h"
#include "CStyleManager.h"
#include "PopupDialogContainer.h"
#include "UiFrostedLayer.h"
#include "UiLogin.h"
#include "define.h"
#include <QDebug>
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

int main(int argc, char* argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    CApplication a(argc, argv);

    a.setWindowIcon(QIcon("images/app.ico"));
    a.setApplicationName(QObject::tr("feng he network"));

    QTranslator translator_zh_CN;
    translator_zh_CN.load(":/translation/lang_zh.qm");
    a.installTranslator(&translator_zh_CN);

    StyleStruct style = CStyleManager::getInstance().getCurrentStyleStruct();
    StyleHelper::loadAppStyle(style.cssFile);

    /*主界面*/
    int result = 0;
    do {
        /*登录界面*/
        UiLogin login;
        if (QDialog::Accepted != login.fadeIn())
            return 0;

        BRSystem w;
        logPrinter = &w;
        //qInstallMessageHandler(outputMessage);
        a.setMainWidget(&w);
        w.show();
        result = a.exec();
    } while (result == RESTART_CODE);

    return result;
}
