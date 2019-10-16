#include "PopupDialogContainer.h"
#include "define.h"
#include <QGraphicsDropShadowEffect>
#include <QStyleOption>
#include <QPainter>
#include <QDebug>

PopupDialogContainer::PopupDialogContainer(QWidget *parent,bool isModal)
	: QWidget(parent), mLayout(nullptr), m_bCanMove(false),mIsModal(isModal)
{
	mLayout = new QHBoxLayout(this);
	mCloseBtn = UiHelper::creatPushButton(this, [=](){
		this->close();
	},20,20,"","btn_close");

	this->setMouseTracking(true);
	this->setAttribute(Qt::WA_TranslucentBackground);
	if (mIsModal) { 
		this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
		this->setWindowModality(Qt::WindowModal); 
	}
	else
	{
		this->setWindowFlags(Qt::FramelessWindowHint);
	}

	QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
	shadow->setOffset(0, 0);
	shadow->setColor(Qt::black);
	shadow->setBlurRadius(10);
	this->setGraphicsEffect(shadow);
}

PopupDialogContainer::~PopupDialogContainer()
{
}

void PopupDialogContainer::addWidget(BaseWidget *widget)
{
	Q_ASSERT(widget);
	widget->setObjectName("contentWidget");
	mLayout->addWidget(widget);
	connect(widget, &BaseWidget::closed, this,&PopupDialogContainer::close);
	int marginSpace = mLayout->margin() * 2;
	this->resize(widget->width() + marginSpace,widget->height() + marginSpace);
}

void PopupDialogContainer::showPopupDialog(BaseWidget *widget,QWidget *parent, bool isModal)
{
	PopupDialogContainer *container = new PopupDialogContainer(parent, isModal);
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
