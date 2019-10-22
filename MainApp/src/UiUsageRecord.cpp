#include "UiUsageRecord.h"

UiUsageRecord::UiUsageRecord(QWidget *parent)
	: BaseWidget(parent)
{
	ui.setupUi(this);
	this->resize(570, 360);
}

UiUsageRecord::~UiUsageRecord()
{
}
