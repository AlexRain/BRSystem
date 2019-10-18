#include "BRSystem.h"
#include "CUiTop.h"
#include "CUiCenter.h"
#include "UiChangeSkin.h"
#include "PopupDialogContainer.h"
#include <QMenuBar>
#include <QToolBar>

BRSystem::BRSystem(QWidget *parent)
	: BaseWidget(parent), mTopWidget(nullptr), mCenterWidget(nullptr),
	mToolbar(nullptr)
{
	init();
	this->setWindowTitle(TOCH("汇声科技生产专用借还系统"));
}

BRSystem::~BRSystem()
{
}

void BRSystem::init()
{
	mTopWidget = new CUiTop(this);
	connect(mTopWidget, &CUiTop::showChangeSkinDlg,this, [=]() {
		UiChangeSkin *dialog = new UiChangeSkin(this);
		PopupDialogContainer::showPopupDialog(dialog, this);
	});

	connect(mTopWidget, &CUiTop::aboutToChangeWindowState, this, [=](CUiTop::WindowState stateAboutToChanged) {

		QWidget *parent = this;
		while (parent->parentWidget())
		{
			parent = parent->parentWidget();
		}

		Qt::WindowStates state = parent->windowState();
		if (stateAboutToChanged == CUiTop::StateNormalOrMax)
		{
			switch (state)
			{
			case Qt::WindowActive:
			case Qt::WindowNoState:
				parent->showMaximized(); break;
			case Qt::WindowMinimized:
				parent->showNormal(); break;
			case Qt::WindowMaximized:
				parent->showNormal(); break;
			default:
				break;
			}
		}else
			parent->showMinimized();
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
