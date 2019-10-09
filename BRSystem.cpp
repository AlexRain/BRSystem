#include "BRSystem.h"
#include "CUiTop.h"
#include "CUiCenter.h"
#include <QMenuBar>
#include <QToolBar>

BRSystem::BRSystem(QWidget *parent)
	: QWidget(parent), mTopWidget(nullptr), mCenterWidget(nullptr),
	mToolbar(nullptr)
{
	init();
	this->setWindowIcon(QIcon("images/app.ico"));
}

BRSystem::~BRSystem()
{
}

void BRSystem::init()
{
	mTopWidget = new CUiTop(this);
	mCenterWidget = new CUiCenter(this);
	mMenuBar = new QMenuBar(this);
	mToolbar = new QToolBar(this);
	mMenuBar->setFixedHeight(25);
	{
		QMenu* file = mMenuBar->addMenu(QString::fromLocal8Bit("文件(&F)"));
		file->setMinimumWidth(120);
		file->addAction(QIcon(QPixmap(":images/images/new.png")), QString::fromLocal8Bit("新建文件"));
		file->addAction(QIcon(QPixmap(":images/images/open.png")), QString::fromLocal8Bit("打开"));
		file->addAction(QIcon(QPixmap(":images/images/open.png")), QString::fromLocal8Bit("关闭"));
		
		mToolbar->addAction(QIcon(QPixmap(":images/images/new.png")), QString::fromLocal8Bit("新建文件"));
		mToolbar->addAction(QIcon(QPixmap(":images/images/open.png")), QString::fromLocal8Bit("打开"));
		mToolbar->addAction(QIcon(QPixmap(":images/images/open.png")), QString::fromLocal8Bit("关闭"));
	}

	{
		QMenu* edit = mMenuBar->addMenu("Edit(&E)");
		edit->setMinimumWidth(120);
		edit->addAction(QIcon(QPixmap(":images/images/new.png")), "New File");
		edit->addAction(QIcon(QPixmap(":images/images/open.png")), "OPen");
		edit->addAction(QIcon(QPixmap(":images/images/open.png")),"Close");
	}

	QLabel *pSp = new QLabel(this);
	pSp->setObjectName("divider");
	pSp->setFixedHeight(1);

	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(6);
	layout->addWidget(mTopWidget,0);
	layout->addWidget(mMenuBar,  1);
	layout->addWidget(mToolbar, 2);
	layout->addWidget(pSp,       3);
	layout->addWidget(mCenterWidget, 4);

	this->resize(958, 596);
}
