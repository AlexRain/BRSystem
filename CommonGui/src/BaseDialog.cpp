#include "BaseDialog.h"
#include "define.h"
#include <QGraphicsDropShadowEffect>
#include <QMouseEvent>

BaseDialog::BaseDialog(QWidget* parent)
    : QDialog(parent)
    , m_bCanMove(false)
    , btn_close(nullptr)
{
    ui.setupUi(this);

    this->setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setMouseTracking(true);

    btn_close = UiHelper::creatPushButton(
        this, [=]() { this->reject(); },
        20, 20, "", "btn_close");
    auto layout = new QHBoxLayout(ui.label_title);
    layout->setContentsMargins(0, 0, 10, 0);
    layout->addStretch();
    layout->addWidget(btn_close);
    layout->setAlignment(btn_close, Qt::AlignVCenter);

    /*使用graphicsDropShadowEffect添加阴影效果*/
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(0, 0);
    shadow->setColor(QColor(0, 0, 0, 80));
    shadow->setBlurRadius(10);
    this->setGraphicsEffect(shadow);
}

BaseDialog::~BaseDialog()
{
}

void BaseDialog::SetTitle(const QString& title)
{
    ui.label_title->setText(title);
    setWindowTitle(title);
}

QWidget* BaseDialog::getContent()
{
    return ui.content;
}

void BaseDialog::mouseMoveEvent(QMouseEvent* event)
{
    QDialog::mouseMoveEvent(event);
    if (event->buttons().testFlag(Qt::LeftButton) && m_bCanMove) {
        QPoint posDes = event->globalPos() - m_dragPoint;
        this->move(posDes);
    }
}

void BaseDialog::mousePressEvent(QMouseEvent* event)
{
    QWidget::mousePressEvent(event);
    if (QRect(0, 0, this->width(), ui.frame_title->height()).contains(event->pos())) {
        m_bCanMove = true;
        m_dragPoint = event->globalPos() - frameGeometry().topLeft();
    } else
        m_bCanMove = false;
}

void BaseDialog::mouseReleaseEvent(QMouseEvent* event)
{
    QWidget::mouseReleaseEvent(event);
    m_bCanMove = false;
}
