#include "PopupDialogContainer.h"
#include "define.h"
#include <QGraphicsDropShadowEffect>
#include <QStyleOption>
#include <QPainter>
#include <QDebug>
#include <qmath.h>

const int borderWidth = 8;

PopupDialogContainer::PopupDialogContainer(QWidget *parent,bool isModal,bool showClose)
	: QWidget(parent), _pCenterWidget(nullptr),
	mLayout(nullptr), m_bCanMove(false),mIsModal(isModal)
{
	mLayout = new QHBoxLayout(this);
	mLayout->setContentsMargins(borderWidth, borderWidth, borderWidth, borderWidth);
	mCloseBtn = UiHelper::creatPushButton(this, [=](){
		this->close();
	},20,20,"","btn_close");
	mCloseBtn->setVisible(showClose);

	this->setMouseTracking(true);
	this->setAttribute(Qt::WA_TranslucentBackground);
	this->setWindowState(Qt::WindowNoState);
	mLastState = Qt::WindowNoState;

	if (mIsModal) { 
		this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
		this->setWindowModality(Qt::WindowModal); 
	}
	else{
		this->setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
	}
}

PopupDialogContainer::~PopupDialogContainer()
{
	if (_pCenterWidget)
	{
		delete _pCenterWidget;
	}
}

void PopupDialogContainer::addWidget(BaseWidget *widget)
{
	Q_ASSERT(widget);
	_pCenterWidget = widget;

	widget->setObjectName("contentWidget");
	mLayout->addWidget(widget);
	QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(widget);
	shadow->setOffset(0, 0);
	shadow->setColor(Qt::black);
	shadow->setBlurRadius(10);
	widget->setGraphicsEffect(shadow);

	widget->setParent(this);
	connect(widget, &BaseWidget::closed, this, &PopupDialogContainer::close);
	int marginSpace = mLayout->margin() * 2;
	this->resize(widget->width() + marginSpace,widget->height() + marginSpace);
}

void PopupDialogContainer::showPopupDialog(BaseWidget *widget,QWidget *parent, bool isModal,bool showCloseBtn)
{
	PopupDialogContainer *container = new PopupDialogContainer(parent, isModal, showCloseBtn);
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
	mCloseBtn->raise();
}

void PopupDialogContainer::changeEvent(QEvent *event)
{
	/*¼àÌý´°¿Ú×´Ì¬±ä»¯*/
	if (event->type() == QEvent::WindowStateChange)
	{
		if (this->windowState() == Qt::WindowMaximized){
			mLayout->setContentsMargins(0, 0, 0, 0);
		}
		else{
			mLayout->setContentsMargins(borderWidth, borderWidth, borderWidth, borderWidth);
			this->setWindowState(Qt::WindowNoState);
		}
	}

	QWidget::changeEvent(event);
}
