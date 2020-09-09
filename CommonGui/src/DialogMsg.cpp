#include "DialogMsg.h"
#include "PopupDialogContainer.h"
#include "UiFrostedLayer.h"
#include "define.h"
#include <QGraphicsDropShadowEffect>
#include <QStyleOption>

const int borderWidth = 8;
const int titleHeight = 34;

DialogMsg::DialogMsg(QWidget* parent, const QString& title,
    const QString& text, QMessageBox::StandardButtons buttons,
    const QStringList& buttonsText)
    : QDialog(parent)
    , mParentWidget(parent)
    , mTitle(title)
    , _pTitle(nullptr)
    , _pBtnClose(nullptr)
    , mContent(text)
    , mButtons(buttons)
    , mButtonClicked(QMessageBox::Cancel)
{
    ui.setupUi(this);
    ui.verticalLayout->setContentsMargins(borderWidth, borderWidth, borderWidth, borderWidth);
    this->setFixedSize(360, 216);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowFlags(windowFlags() | Qt::FramelessWindowHint);

    _pTitle = new QLabel(this);
    _pTitle->setObjectName("label_title");
    _pTitle->setText(title);
    _pTitle->setAlignment(Qt::AlignCenter);

    ui.label_content->setWordWrap(true);
    ui.label_content->setText(text);

    _pBtnClose = new QPushButton(this);
    connect(_pBtnClose, &QPushButton::clicked, this, &DialogMsg::slotClose);
    _pBtnClose->setFixedSize(20, 20);
    _pBtnClose->setObjectName("btn_close");
    _pBtnClose->setCursor(Qt::PointingHandCursor);

    QList<int> listId;
    if (buttons & QMessageBox::Ok
        && buttons & QMessageBox::Cancel) {
        listId << QMessageBox::Ok << QMessageBox::Cancel;
        mButtonsText << TOCH("确 定") << TOCH("取 消");
    } else if (buttons & QMessageBox::Ok) {
        listId << QMessageBox::Ok;
        mButtonsText << TOCH("确 定");
    } else if (buttons & QMessageBox::Cancel) {
        listId << QMessageBox::Cancel;
        mButtonsText << TOCH("取 消");
    } else {
        listId << QMessageBox::Ok;
    }

    if (buttonsText.isEmpty()) {
        ui.layout_btn->addStretch();
        for (int i = 0; i < mButtonsText.size(); ++i) {
            QPushButton* btn = new QPushButton(mButtonsText.at(i), this);
            btn->setFixedSize(72, 30);
            connect(btn, &QPushButton::clicked, this, &DialogMsg::slotButtonClicked);
            btn->setProperty("buttonId", listId.at(i));
            ui.layout_btn->addWidget(btn);
        }
        ui.layout_btn->addStretch();
    } else {
        ui.layout_btn->addStretch();
        for (int i = 0; i < buttonsText.size(); ++i) {
            QPushButton* btn = new QPushButton(buttonsText.at(i), this);
            btn->setFixedSize(72, 30);
            connect(btn, &QPushButton::clicked, this, &DialogMsg::slotButtonClicked);
            btn->setProperty("buttonId", listId.at(i));
            ui.layout_btn->addWidget(btn);
        }
        ui.layout_btn->addStretch();
    }

    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(ui.contentWidget);
    shadow->setOffset(0, 0);
    shadow->setColor(QColor(0, 0, 0, 80));
    shadow->setBlurRadius(10);
    ui.contentWidget->setGraphicsEffect(shadow);

    while (nullptr != mParentWidget && nullptr != mParentWidget->parentWidget()) {
        mParentWidget = mParentWidget->parentWidget();
        if (mParentWidget->inherits("PopupDialogContainer"))
            break;
    }
    mPLayer = new UiFrostedLayer(mParentWidget);
    mPLayer->hide();
}

DialogMsg::~DialogMsg()
{
    if (mPLayer)
        delete mPLayer;
}

QMessageBox::StandardButton DialogMsg::question(QWidget* parent, const QString& title, const QString& text,
    QMessageBox::StandardButtons buttons, const QStringList& buttonsText)
{
    QMessageBox::StandardButton ok = QMessageBox::Cancel;
    DialogMsg dlgMsg(parent, title, text, buttons, buttonsText);
    dlgMsg.showLayer();
    int result = dlgMsg.exec();
    if (result == QDialog::Accepted)
        ok = QMessageBox::Ok;
    return ok;
}

void DialogMsg::showLayer()
{
    if (mPLayer && mParentWidget) {
        mPLayer->show();
        mPLayer->raise();
    }
}

void DialogMsg::hideLayer()
{
    if (mPLayer && mParentWidget) {
        mPLayer->hide();
    }
}

void DialogMsg::slotButtonClicked()
{
    mPLayer->deleteLater();
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (btn) {
        switch (btn->property("buttonId").toInt()) {
        case QMessageBox::Ok:
            this->accept();
            break;
        case QMessageBox::Cancel:
            this->reject();
            break;
        default:
            this->reject();
            break;
        }
    }
}

void DialogMsg::slotClose()
{
    mPLayer->deleteLater();
    this->reject();
}

void DialogMsg::paintEvent(QPaintEvent* event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QDialog::paintEvent(event);
}

void DialogMsg::resizeEvent(QResizeEvent* event)
{
    _pTitle->resize(this->width() - borderWidth * 2 - 2, titleHeight);
    _pTitle->move(borderWidth + 1, borderWidth + 1);
    _pBtnClose->move(this->width() - _pBtnClose->width() - borderWidth - 5, borderWidth + 5);
    _pTitle->raise();
    _pBtnClose->raise();

    if (mPLayer && mParentWidget) {
        QSize size = mParentWidget->size();
        mPLayer->resize(size);
        mPLayer->move(0, 0);
        mPLayer->raise();
    }

    QDialog::resizeEvent(event);
}
