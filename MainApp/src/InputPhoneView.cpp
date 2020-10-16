#include "InputPhoneView.h"

InputPhoneView::InputPhoneView(QWidget* parent)
    : BaseDialog(parent)
{
    ui.setupUi(this->getContent());
    this->SetTitle(tr("modify phone"));
	connect(ui.lineEdit_phone, &QLineEdit::textChanged, this, [=](const QString& text) {
		ui.btn_ok->setEnabled(!text.isEmpty());
	});
	QMetaObject::connectSlotsByName(this);
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
