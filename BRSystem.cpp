#include "BRSystem.h"
#include "CUiTop.h"
#include "CUiCenter.h"
#include "UiChangeSkin.h"
#include <QMenuBar>
#include <QToolBar>

BRSystem::BRSystem(QWidget *parent)
	: QWidget(parent), mTopWidget(nullptr), mCenterWidget(nullptr),
	mToolbar(nullptr)
{
	init();
	this->setWindowIcon(QIcon("images/app.ico"));
	this->setWindowTitle(TOCH("汇声科技生产专用借还系统"));
}

BRSystem::~BRSystem()
{
}

void BRSystem::init()
{
	mTopWidget = new CUiTop(this);
	connect(mTopWidget, &CUiTop::showChangeSkinDlg, [=]() {
		UiChangeSkin *dialog = new UiChangeSkin(this);
		dialog->exec();
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
