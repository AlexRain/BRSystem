#include "BRSystem.h"
#include "CUiTop.h"
#include "CUiCenter.h"
#include "UiChangeSkin.h"
#include "PopupDialogContainer.h"
#include "DialogMsg.h"
#include "UiFrostedLayer.h"
#include "BubbleTipWidget.h"
#include <QMenuBar>
#include <QToolBar>
#include <QDebug>
#include <QBitmap>

BRSystem::BRSystem(QWidget *parent)
	: BaseWidget(parent), mTopWidget(nullptr), mCenterWidget(nullptr),
	mToolbar(nullptr)
{
	this->init();
	this->setWindowTitle(TOCH("汇声科技生产专用借还系统"));
	this->setAttribute(Qt::WA_DeleteOnClose);
	pLayer = new UiFrostedLayer(this);
	pLayer->hide();
}

BRSystem::~BRSystem()
{
	qDebug() << "BRSystem";
}

void BRSystem::init()
{
	mTopWidget = new CUiTop(this);
	connect(mTopWidget, &CUiTop::showChangeSkinDlg,this, [=]() {
		UiChangeSkin *dialog = new UiChangeSkin(this);
		PopupDialogContainer::showPopupDialogFadeIn(dialog, this,TOCH("更换皮肤"));
	});

	connect(mTopWidget, &CUiTop::aboutToChangeWindowState, this, [=](CUiTop::WindowState stateAboutToChanged) {

		QWidget *parent = this;
		while (parent->parentWidget()) {
			parent = parent->parentWidget();
		}

		if (parent->inherits("PopupDialogContainer")) {
			PopupDialogContainer *pWidget = qobject_cast<PopupDialogContainer*>(parent);
			if (pWidget) {
				if (stateAboutToChanged == CUiTop::StateMin)
					parent->setWindowState(Qt::WindowMinimized);
				else
					pWidget->showMaxorNormal();
			}
		}
	});

	connect(mTopWidget, &CUiTop::appAboutToExit, this, [=]() {
		int result = DialogMsg::question(this, TOCH("提示"),TOCH("确定要关闭？"),QMessageBox::Ok | QMessageBox::Cancel);
		if (result == QMessageBox::Ok) exit(0);
	});

	connect(mTopWidget, &CUiTop::clickProfile, [this]() {
		QWidget *p = new QWidget();
		BubbleTipWidget *tips = new BubbleTipWidget(p,this);
		QPoint pos = this->mapToGlobal(QPoint(0, mTopWidget->height() + 8));
		tips->move(QCursor::pos().x() - tips->width() / 2, pos.y() - 22 + 5);
		tips->fadeIn();
	});

	mCenterWidget = new CUiCenter(this);
	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(6);
	layout->addWidget(mTopWidget);
	layout->addWidget(UiHelper::createSplitter(this));
	layout->addWidget(mCenterWidget);
	this->resize(958, 596);
}

void BRSystem::windowStateChanged(Qt::WindowStates states)
{
	mTopWidget->windowStateChanged(states);
	BaseWidget::windowStateChanged(states);
}

void BRSystem::resizeEvent(QResizeEvent *event)
{
	pLayer->resize(this->size());
	pLayer->move(0, 0);
}
