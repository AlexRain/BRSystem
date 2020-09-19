#include "AboutView.h"
#include "NetworkDefine.h"
#include "define.h"

AboutView::AboutView(QWidget* parent)
    : BaseDialog(parent)
{
    ui.setupUi(this->getContent());
    QMetaObject::connectSlotsByName(this);
    ui.label_desc->setOpenExternalLinks(true);
    QString showText;
    showText.append(tr("current version:") + app_version + "<br>");
    showText.append("<p><a href=\"www.HL019.com\"><span style=\" text - decoration: underline; color:rgb(0,122,204); \">www.HL019.com</span></a></p>");
    ui.label_desc->setText(showText);

    QPixmap pixLogo("images/logo.png");
    ui.label_logo->setMinimumWidth(115);
    ui.label_logo->setPixmap(UiHelper::justPixmapByWidth(115, pixLogo));
}

AboutView::~AboutView()
{
}

void AboutView::on_btn_ok_clicked()
{
    this->accept();
}
