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
#include <QStyleFactory>
#include <QTranslator>
#include <QtWidgets/QApplication>

using namespace std;

int main(int argc, char* argv[])
{
    CApplication a(argc, argv);

    a.setWindowIcon(QIcon("images/app.ico"));
    a.setApplicationName(QObject::tr("feng he network"));
    a.setAttribute(Qt::AA_EnableHighDpiScaling);

    QTranslator translator_zh_CN;
    translator_zh_CN.load(":/translation/lang_zh.qm");
    a.installTranslator(&translator_zh_CN);

    StyleStruct style = CStyleManager::getInstance().getCurrentStyleStruct();
    StyleHelper::loadAppStyle(style.cssFile);

    /*登录界面*/
    UiLogin login;
    if (QDialog::Accepted != login.fadeIn())
        return 0;

    /*主界面*/
    BRSystem w;
    a.setMainWidget(&w);
    PopupDialogContainer::showMainWidgetFadeIn(&w);
    return a.exec();
}
