#include "HFChangePhoneView.h"

HFChangePhoneView::HFChangePhoneView(QWidget *parent)
	: BaseWidget(parent)
{
	ui.setupUi(this);
}

HFChangePhoneView::~HFChangePhoneView()
{
}

void HFChangePhoneView::SetOldPhoneNumber(const QString& number)
{
	oldNumber = number;
	QString displayNumber = number.left(3) + "****" + number.right(4);
	ui.label_title->setText(tr("get %1 code").arg(displayNumber));
}
