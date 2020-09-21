#include "BRSystem.h"
#include "AboutView.h"
#include "BubbleTipWidget.h"
#include "CUiCenter.h"
#include "CUiTop.h"
#include "DialogMsg.h"
#include "HFBroadbandDial.h"
#include "HFChangePhoneView.h"
#include "HFChargeView.h"
#include "PopupDialogContainer.h"
#include "UiChangeSkin.h"
#include "UiFrostedLayer.h"
#include "UpgradeHelper.h"
#include "Windows.h"
#include "tlhelp32.h"
#include <QBitmap>
#include <QDebug>
#include <QDesktopServices>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>
#include <QVBoxLayout>

int KillProcess(const wchar_t* processName)
{
    PROCESSENTRY32 pe;
    DWORD id = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    pe.dwSize = sizeof(PROCESSENTRY32);
    if (!Process32First(hSnapshot, &pe)) {
        return 0;
    }

    while (1) {
        pe.dwSize = sizeof(PROCESSENTRY32);
        if (Process32Next(hSnapshot, &pe) == FALSE) {
            break;
        }
        //find processName
        if (wcsicmp(pe.szExeFile, processName) == 0) {
            id = pe.th32ProcessID;
            break;
        }
    }
    CloseHandle(hSnapshot);
    //if(id == 0)
    //  return ;

    //Kill The Process
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, id);
    if (hProcess != NULL) {
        TerminateProcess(hProcess, 0);
        CloseHandle(hProcess);
    }
    return 1;
}

BRSystem::BRSystem(QWidget* parent)
    : QWidget(parent)
    , m_pContentLayout(nullptr)
    , m_pCurrentWidget(nullptr)
{
    qRegisterMetaType<ResponData>("ResponData");
    this->init();
    this->setWindowTitle(tr("HelloGame"));
    pLayer = new UiFrostedLayer(this);
    pLayer->hide();
    startLocalPyServer();
}

BRSystem::~BRSystem()
{
    KillProcess(L"hl-py.exe");
}

void BRSystem::showCoverWidget(BaseWidget* content)
{
    qobject_cast<BRSystem*>(CApp->getMainWidget())->addwidget(content);
}

void BRSystem::addwidget(BaseWidget* content)
{
    Q_ASSERT(content);
    connect(content, &BaseWidget::closed, [this](BaseWidget* obj) {
        m_pContentLayout->removeWidget(obj);
        auto it = mListWidgets.begin();
        while (it != mListWidgets.end()) {
            if (*it == obj) {
                mListWidgets.erase(it);
                break;
            }
            it++;
        }
    });

    if (m_pCurrentWidget && m_pCurrentWidget != content) {
        m_pContentLayout->removeWidget(m_pCurrentWidget);
        m_pCurrentWidget->hide();
    }
    content->setParent(this);
    m_pContentLayout->addWidget(content);
    content->show();
    m_pCurrentWidget = content;
    mListWidgets << content;
}

void BRSystem::showMainPage()
{
    this->addwidget(mListWidgets.at(0));
}

void BRSystem::outputMessage(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    QString message;
    switch (type) {
    case QtDebugMsg:
        message = QString("[Debug]");
        break;

    case QtWarningMsg:
        message = QString("[Warning]");
        break;

    case QtCriticalMsg:
        message = QString("[Critical]");
        break;

    case QtFatalMsg:
        message = QString("[Fatal]");
        break;

    case QtInfoMsg:
        message = QString("[Info]");
        break;
    }
    if (logOutput)
        logOutput->append(message.append(msg));
}

void BRSystem::printLog(QtMsgType type, const QString& msg)
{
    QString message;
    switch (type) {
    case QtDebugMsg:
        message = QString("[Debug]%1");
        break;

    case QtWarningMsg:
        message = QString("<span style='color:red'>[Warning]%1</span>");
        break;

    case QtCriticalMsg:
        message = QString("<span style='color:red'>[Critical]%1</span>");
        break;

    case QtFatalMsg:
        message = QString("[Fatal]%1");
        break;

    case QtInfoMsg:
        message = QString("[Info]%1");
        break;
    }
    if (logOutput)
        logOutput->append(message.arg(msg));
}

void BRSystem::init()
{
    centerWidget = new CUiCenter(this);
    centerWidget->setObjectName("centerWidget");
    m_pContentLayout = new QVBoxLayout;
    m_pContentLayout->setContentsMargins(10, 0, 10, 0);
    m_pContentLayout->setSpacing(6);

    QMenuBar* menuBar = new QMenuBar(this);
    createMenus(menuBar);

    // add log output
    auto groupLog = new QGroupBox(this);
    groupLog->setTitle(tr("log"));

    auto layoutLog = new QVBoxLayout(groupLog);
    layoutLog->setSpacing(0);
    logOutput = new QTextBrowser(groupLog);
    layoutLog->addWidget(logOutput);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 10, 0, 10);
    layout->setSpacing(6);

    QHBoxLayout* layoutMenu = new QHBoxLayout;
    layoutMenu->setContentsMargins(10, 0, 0, 0);
    layoutMenu->setSpacing(0);
    layoutMenu->addWidget(menuBar);

    layout->addLayout(layoutMenu);
    layout->addWidget(UiHelper::createSplitter(this));
    layout->addItem(m_pContentLayout);
    m_pContentLayout->addWidget(centerWidget);
    m_pContentLayout->addWidget(groupLog);
    m_pContentLayout->setStretchFactor(centerWidget, 8);
    m_pContentLayout->setStretchFactor(groupLog, 3);

    // adds area bottom
    {
        labelAdds = new QLabel(this);
        labelAdds->setOpenExternalLinks(true);
        labelAdds->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        labelAdds->setText(TOCH("<p><a href=\"www.HL019.com\"><span style=\" text - decoration: underline; color:rgb(0,122,204); \">哈喽游戏交易平台.WWW.HL019.COM&nbsp;专业回收DNF&nbsp;魔兽&nbsp;金币&nbsp;材料&nbsp;秒出货秒打款，官方出货群：692812379</span></a></p>"));
        auto widgetBottom = new QFrame(this);
        widgetBottom->setObjectName("frameBottom");
        auto laytoutBottom = new QHBoxLayout(widgetBottom);
        laytoutBottom->setContentsMargins(0, 0, 0, 0);
        laytoutBottom->addStretch();
        laytoutBottom->addWidget(labelAdds);
        m_pContentLayout->addWidget(widgetBottom);
    }

    this->resize(958, 596);
}

void BRSystem::createMenus(QMenuBar* menuBar)
{
    //account
    {
        auto accountMenu = menuBar->addMenu(tr("account"));
        accountMenu->addAction(tr("charge"), [=]() {
            HFChargeView* chargeView = new HFChargeView(this);
            connect(chargeView, &HFChargeView::chargeSuccess, [=]() {});
            chargeView->resize(400, 300);
            PopupDialogContainer::showPopupDialogFadeIn(chargeView, CApp->getMainWidget(), tr("charge"));
        });
        accountMenu->addAction(tr("change phone"), [=]() {
            HFChangePhoneView* changePhoneView = new HFChangePhoneView(this);
            PopupDialogContainer::showPopupDialogFadeIn(changePhoneView, CApp->getMainWidget(), tr("change phone"));
        });
        accountMenu->addAction(tr("register"), [=]() {});
        accountMenu->addAction(tr("logout"), [=]() {
            qApp->exit(RESTART_CODE);
        });
        accountMenu->addSeparator();
        accountMenu->addAction(tr("exit"), [=]() {
            qApp->exit(0);
        });
    }

    //import
    {
        auto accountMenu = menuBar->addMenu(tr("import export bind"));
        accountMenu->addAction(tr("import"), [=]() {});
    }

    //setting
    {
        auto accountMenu = menuBar->addMenu(tr("settings"));
        accountMenu->addAction(tr("change ip"), [=]() {});
        accountMenu->addAction(tr("pppoe"), [=]() {
            HFBroadbandDial* pppoeView = new HFBroadbandDial(this);
            PopupDialogContainer::showPopupDialogFadeIn(pppoeView, CApp->getMainWidget(), tr("pppoe"));
        });
        accountMenu->addSeparator();
        accountMenu->addAction(tr("change style"), [=]() {
            UiChangeSkin* dialog = new UiChangeSkin(this);
            dialog->resize(350, 192);
            PopupDialogContainer::showPopupDialogFadeIn(dialog, CApp->getMainWidget(), tr("change style"));
        });
        accountMenu->addSeparator();
        accountMenu->addAction(tr("up to date"), [=]() { CheckUpgrade(); });
        accountMenu->addAction(tr("about"), [=]() {
            AboutView about(this);
            about.SetTitle(tr("about"));
            about.exec();
        });
    }
}

void BRSystem::CheckUpgrade()
{
    UpgradeHelper checkHelper;
    auto result = checkHelper.GetCheckResult();
    if (result.needUpdate) {
        if (result.force_update) {
            int code = DialogMsg::question(this, QObject::tr("question"), QObject::tr("find a new version, please up to date."), QMessageBox::Ok);
            if (code == QMessageBox::Ok) {
                QDesktopServices::openUrl(QUrl::fromEncoded(result.download_url.toUtf8()));
            }
            qApp->exit(0);
        }
    } else {
        DialogMsg::question(this, QObject::tr("tips"), QObject::tr("your application is up to date"), QMessageBox::Ok);
    }
}

void BRSystem::startLocalPyServer()
{
    if (nullptr == process) {
        process = new QProcess(this);
        connect(process, SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(onStartPyServerError(QProcess::ProcessError)));
        process->start(PY_SERVER_EXE);
    }
}

void BRSystem::resizeEvent(QResizeEvent* event)
{
    pLayer->resize(this->size());
    pLayer->move(0, 0);
    QWidget::resizeEvent(event);
}

void BRSystem::onStartPyServerError(QProcess::ProcessError error)
{
    printLog(QtWarningMsg, tr("start py server failed").append("error code=%1").arg(error));
}
//void BRSystem::closeEvent(QCloseEvent* event)
//{
//    int result = DialogMsg::question(this, tr("tips"), tr("are you sure you want to exit?"), QMessageBox::Ok | QMessageBox::Cancel);
//    if (result != QMessageBox::Ok) {
//        event->ignore();
//        return;
//    }
//    /*QWidget::closeEvent(event);*/
//}
