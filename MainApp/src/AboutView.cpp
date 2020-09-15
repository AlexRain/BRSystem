#include "AboutView.h"

AboutView::AboutView(QWidget* parent)
    : BaseDialog(parent)
{
    ui.setupUi(this->getContent());
    QMetaObject::connectSlotsByName(this);
}

AboutView::~AboutView()
{
}

void AboutView::on_btn_ok_clicked()
{
    this->accept();
}
