#include "InputPhoneView.h"

InputPhoneView::InputPhoneView(QWidget* parent)
    : BaseDialog(parent)
{
    ui.setupUi(this->getContent());
    this->SetTitle(tr("modify phone"));
}

InputPhoneView::~InputPhoneView()
{
}

void InputPhoneView::GetPhoneNumber(QString& phone)
{
    phone = ui.lineEdit_phone->text();
}

void InputPhoneView::on_btn_ok_clicked()
{
    this->accept();
}
