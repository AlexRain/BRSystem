#include "BRSystem.h"
#include "BubbleTipWidget.h"
#include "CUiCenter.h"
#include "CUiTop.h"
#include "DialogMsg.h"
#include "PopupDialogContainer.h"
#include "UiChangeSkin.h"
#include "UiFrostedLayer.h"
#include "HFChargeView.h"
#include "HFBroadbandDial.h"
#include "HFChangePhoneView.h"
#include <QBitmap>
#include <QDebug>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>
#include <QVBoxLayout>

BRSystem::BRSystem(QWidget* parent)
    : QWidget(parent)
    , m_pContentLayout(nullptr)
    , mToolbar(nullptr)
    , m_pCurrentWidget(nullptr)
{
    this->init();
    this->setWindowTitle(tr("feng he network"));
    pLayer = new UiFrostedLayer(this);
    pLayer->hide();
}

BRSystem::~BRSystem()
{
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

void BRSystem::init()
{
    CUiCenter* centerWidget = new CUiCenter(this);
    centerWidget->setObjectName("centerWidget");
    m_pContentLayout = new QVBoxLayout;
    m_pContentLayout->setContentsMargins(0, 0, 0, 0);
    m_pContentLayout->setSpacing(0);

    QMenuBar* menuBar = new QMenuBar(this);
    createMenus(menuBar);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(6);
    layout->addWidget(menuBar);
    //layout->addWidget(mTopWidget);
    layout->addWidget(UiHelper::createSplitter(this));
    layout->addItem(m_pContentLayout);

    this->resize(958, 596);
    this->addwidget(centerWidget);
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
        accountMenu->addAction(tr("login"), [=]() {});
        accountMenu->addSeparator();
        accountMenu->addAction(tr("exit"), [=]() {
            exit(0);
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
        accountMenu->addAction(tr("up to date"), [=]() {});
        accountMenu->addAction(tr("about"), [=]() {});
    }
}

void BRSystem::createStatusBar()
{
}

void BRSystem::resizeEvent(QResizeEvent* event)
{
    pLayer->resize(this->size());
    pLayer->move(0, 0);
}

void BRSystem::closeEvent(QCloseEvent* event)
{
    int result = DialogMsg::question(this, tr("tips"), tr("are you sure you want to exit?"), QMessageBox::Ok | QMessageBox::Cancel);
    if (result != QMessageBox::Ok) {
        event->ignore();
        return;
    }
    QWidget::closeEvent(event);
}
