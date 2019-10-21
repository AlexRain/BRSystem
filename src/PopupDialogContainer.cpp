#include "PopupDialogContainer.h"
#include "define.h"
#include "UiFrostedLayer.h"
#include <QGraphicsDropShadowEffect>
#include <QStyleOption>
#include <QPainter>
#include <QDebug>
#include <qmath.h>

const int borderWidth = 8;
const int titleHeight = 34;

PopupDialogContainer::PopupDialogContainer(QWidget *parent,const QString &title,bool isModal, bool showClose)
	: QWidget(parent), _pCenterWidget(nullptr), mLabelTitle(nullptr),mTitleText(title), closeFlag(false),
	mLayout(nullptr), m_bCanMove(false),mIsModal(isModal), mParentWidget(parent), mPLayer(nullptr)
{
	mLayout = new QVBoxLayout(this);
	mLayout->setSpacing(0);
	mLayout->setContentsMargins(borderWidth, borderWidth, borderWidth, borderWidth);
	mCloseBtn = UiHelper::creatPushButton(this, [=](){
		delete mPLayer;
		this->closeFlag = true;
		this->close();
	},20,20,"","btn_close");
	mCloseBtn->setVisible(showClose);
	mCloseBtn->setShortcut(Qt::Key_Escape);

	mLabelTitle = new QLabel(this);
	mLabelTitle->setAlignment(Qt::AlignCenter);
	mLabelTitle->setIndent(10);
	mLabelTitle->setObjectName("label_title");
	mLabelTitle->setText(title);
	mLabelTitle->resize(this->width() - borderWidth * 2 - 2,titleHeight);
	mLabelTitle->setVisible(!title.isEmpty());

	this->setMouseTracking(true);
	this->setAttribute(Qt::WA_TranslucentBackground);
	this->setWindowState(Qt::WindowNoState);
	this->setAttribute(Qt::WA_DeleteOnClose, true);
	mLastState = Qt::WindowNoState;

	/*是否需要显示为模态窗口*/
	if (mIsModal) { 
		this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
		this->setWindowModality(Qt::WindowModal); 
	}
	else{
		this->setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
	}

	mPLayer = new UiFrostedLayer(parent);
	mPLayer->hide();
}

PopupDialogContainer::~PopupDialogContainer()
{
	qDebug() << "PopupDialogContainer";
}

void PopupDialogContainer::addWidget(BaseWidget *widget)
{
	Q_ASSERT(widget);
	_pCenterWidget = widget;
	widget->setObjectName("contentWidget");
	widget->setParent(this);
	mLayout->addWidget(widget);

	/*使用graphicsDropShadowEffect添加阴影效果*/
	QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(widget);
	shadow->setOffset(0, 0);
	shadow->setColor(Qt::black);
	shadow->setBlurRadius(10);
	widget->setGraphicsEffect(shadow);

	connect(widget, &BaseWidget::closed, this, [=]() {
		delete mPLayer;
		this->closeFlag = true;
		this->close();
	});
	int marginSpace = mLayout->margin() * 2;
	this->resize(widget->width() + marginSpace,widget->height() + marginSpace);
}

void PopupDialogContainer::showMaxorNormal()
{
	if (this->windowState() == Qt::WindowMaximized)
	{
		mLayout->setContentsMargins(borderWidth, borderWidth, borderWidth, borderWidth);
		this->setWindowState(Qt::WindowNoState);
	}
	else {
		mLayout->setContentsMargins(0, 0, 0, 0);
		this->showMaximized();
	}
}

void PopupDialogContainer::showMin()
{
	mLastState = Qt::WindowMinimized;
	this->showMinimized();
}

void PopupDialogContainer::showLayer()
{
	if (mPLayer && mParentWidget)
	{
		mPLayer->show();
	}
}

void PopupDialogContainer::hideLayer()
{
	if (mPLayer && mParentWidget)
	{
		mPLayer->hide();
	}
}

void PopupDialogContainer::showPopupDialog(BaseWidget *widget,QWidget *parent,const QString &title, bool isModal, bool showCloseBtn)
{
	PopupDialogContainer *container = new PopupDialogContainer(parent, title, isModal,showCloseBtn);
	container->showLayer();
	container->addWidget(widget);
	container->show();
}

void PopupDialogContainer::mouseMoveEvent(QMouseEvent *event)
{
	QWidget::mouseMoveEvent(event);
	if (event->buttons().testFlag(Qt::LeftButton) && m_bCanMove){
		QPoint posDes = event->globalPos() - m_dragPoint;
		this->move(posDes);
	}
}

void PopupDialogContainer::mousePressEvent(QMouseEvent *event)
{
	QWidget::mousePressEvent(event);
	if (QRect(0, 0, this->width(), 40).contains(event->pos())){
		m_bCanMove = true;
		m_dragPoint = event->globalPos() - frameGeometry().topLeft();
	}
	else
		m_bCanMove = false;
}

void PopupDialogContainer::mouseReleaseEvent(QMouseEvent *event)
{
	QWidget::mouseReleaseEvent(event);
	m_bCanMove = false;
}

void PopupDialogContainer::resizeEvent(QResizeEvent *event)
{
	QWidget::resizeEvent(event);
	mCloseBtn->move(this->width() - mCloseBtn->width() - mLayout->margin() - 5,mLayout->margin() + 5);
	mLabelTitle->resize(this->width() -  2 * borderWidth - 2, titleHeight);
	mLabelTitle->move(borderWidth + 1, borderWidth + 1);
	mLabelTitle->raise();
	mCloseBtn->raise();

	if (mPLayer && mParentWidget)
	{
		mPLayer->resize(mParentWidget->size());
		mPLayer->move(0, 0);
	}
}

void PopupDialogContainer::changeEvent(QEvent *event)
{
	/*监听窗口状态变化*/
	if (event->type() == QEvent::WindowStateChange)
	{
		if (this->windowState() == Qt::WindowMaximized) {
			mLayout->setContentsMargins(0, 0, 0, 0);
		}
		else {
			mLayout->setContentsMargins(borderWidth, borderWidth, borderWidth, borderWidth);
		}
		_pCenterWidget->windowStateChanged(this->windowState());
	}

	QWidget::changeEvent(event);
}

void PopupDialogContainer::closeEvent(QCloseEvent *event)
{
	if (closeFlag)
	{
		event->accept();
	}
	else {
		event->ignore();
	}
}
