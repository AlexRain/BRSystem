#include "InputIdentityView.h"
#include "DialogMsg.h"
#include <QTime>
#include <QSettings>
#include <QTextCodec>



InputIdentityView::InputIdentityView(QWidget* parent)
    : BaseDialog(parent)
{
    ui.setupUi(this->getContent());
    this->SetTitle(QString::fromLocal8Bit("实名信息"));
    connect(ui.lineEdit_userName, &QLineEdit::textChanged, this, [=](const QString& text) {
        ui.btn_ok->setEnabled(!text.isEmpty());
    });
    QMetaObject::connectSlotsByName(this);
	QSettings setting("app.ini", QSettings::IniFormat);
	ui.label_userName->setText(QString::fromLocal8Bit("姓名"));
	ui.label_certId->setText(QString::fromLocal8Bit("身份证"));
	ui.lineEdit_userName->setText(setting.value("identity/userName", "").toString());
	ui.lineEdit_certId->setText(setting.value("identity/certId", "").toString());
}

InputIdentityView::~InputIdentityView()
{
}



void InputIdentityView::on_btn_ok_clicked()
{
    if (ui.lineEdit_userName->text().isEmpty()|| ui.lineEdit_certId->text().isEmpty()) {
        DialogMsg::question(this, tr("tips"),QString::fromLocal8Bit("请完善姓名或者身份证"), QMessageBox::Ok);
        return;
    }
	QSettings setting("app.ini", QSettings::IniFormat);
	setting.setIniCodec(QTextCodec::codecForName("GBK"));
	setting.setValue("identity/userName", QString(ui.lineEdit_userName->text()));
	setting.setValue("identity/certId", QString(ui.lineEdit_certId->text()));
    this->accept();
}
