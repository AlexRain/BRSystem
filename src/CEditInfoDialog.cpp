#include "CEditInfoDialog.h"
#include "DialogMsg.h"
#include <QMessageBox>

CEditInfoDialog::CEditInfoDialog(QWidget *parent,bool isEditUi)
	: BaseWidget(parent), mIsEditUi(isEditUi)
{
	ui.setupUi(this);
	this->init();
}

CEditInfoDialog::~CEditInfoDialog()
{
}

void CEditInfoDialog::setData(const BorrowInfo &info)
{
	this->mBorrowInfo = info;
	this->initData();
}

void CEditInfoDialog::init()
{
	this->setWindowTitle(mIsEditUi ? TOCH("编辑借条"):TOCH("新建借条"));
	ui.radioBtn_notReturned->setChecked(true);
	this->initEdit();

	if (!mIsEditUi){
		ui.btn_delete->hide();
		ui.btn_history->hide();
	}
}

void CEditInfoDialog::initEdit()
{
	ui.lineEdit_productId->setFocus();
	ui.lineEdit_productId->setMaxLength(20);
	ui.lineEdit_productName->setMaxLength(30);
	ui.lineEdit_browerName->setMaxLength(20);
	ui.lineEdit_reason->setMaxLength(150); 
	ui.textEdit_mark->setMaximumBlockCount(500);

	ui.lineEdit_productId->setClearButtonEnabled(true);
	ui.lineEdit_productName->setClearButtonEnabled(true);
	ui.lineEdit_browerName->setClearButtonEnabled(true);
	ui.lineEdit_reason->setClearButtonEnabled(true);
}

void CEditInfoDialog::initData()
{
	ui.lineEdit_productId->setText(mBorrowInfo.productionId);
	ui.lineEdit_productName->setText(mBorrowInfo.productionName);
	ui.lineEdit_browerName->setText(mBorrowInfo.borrowerName);
	ui.lineEdit_reason->setText(mBorrowInfo.borrowReason);
	ui.textEdit_mark->appendPlainText(mBorrowInfo.remarks);

	switch (mBorrowInfo.borrowStatus)
	{
	case Returned:
		ui.radioBtn_returned->setChecked(true);
		break;
	case NotReturned:
		ui.radioBtn_notReturned->setChecked(true);
		break;
	case Lost:
		ui.radioBtn_lost->setChecked(true);
		break;
	default:
		break;
	}
}

bool CEditInfoDialog::verifyInput()
{
	if (ui.lineEdit_productId->text().isEmpty()) {
		ui.lineEdit_productId->setFocus();
		return false;
	}

	if (ui.lineEdit_productName->text().isEmpty()) {
		ui.lineEdit_productName->setFocus();
		return false;
	}

	if (ui.lineEdit_browerName->text().isEmpty()) {
		ui.lineEdit_browerName->setFocus();
		return false;
	}

	if (ui.lineEdit_reason->text().isEmpty()) {
		ui.lineEdit_reason->setFocus();
		return false;
	}

	return true;
}

void CEditInfoDialog::getInputData()
{
	this->mBorrowInfo.productionId   = ui.lineEdit_productId->text();
	this->mBorrowInfo.productionName = ui.lineEdit_productName->text();
	this->mBorrowInfo.borrowerName   = ui.lineEdit_browerName->text();
	this->mBorrowInfo.borrowReason   = ui.lineEdit_reason->text();
	this->mBorrowInfo.remarks        = ui.textEdit_mark->toPlainText();

	if(!mIsEditUi)
		this->mBorrowInfo.borrowDate = QDateTime::currentDateTime();
	this->mBorrowInfo.updateDate     = QDateTime::currentDateTime();

	if (ui.radioBtn_returned->isChecked())
		this->mBorrowInfo.borrowStatus = Returned;
	else if (ui.radioBtn_notReturned->isChecked())
		this->mBorrowInfo.borrowStatus = NotReturned;
	else if (ui.radioBtn_lost->isChecked())
		this->mBorrowInfo.borrowStatus = Lost;
}

void CEditInfoDialog::on_btn_cancel_clicked()
{
	this->close();
}

void CEditInfoDialog::on_btn_save_clicked()
{
	if (!verifyInput()) {
		DialogMsg::question(this, TOCH("警告"), TOCH("请填写完整信息！"), QMessageBox::Ok);

		return;
	}
	
	this->getInputData();
	if (mIsEditUi)
		emit updateData(this->mBorrowInfo);
	else
		emit saveData(this->mBorrowInfo);
	this->close();
}

void CEditInfoDialog::on_btn_history_clicked()
{

}

void CEditInfoDialog::on_btn_delete_clicked()
{
	QMessageBox::StandardButton ok = DialogMsg::question(this, TOCH("提示"),
		TOCH("确定要删除这条记录吗？"), QMessageBox::Ok | QMessageBox::Cancel);
	if (QMessageBox::Ok == ok)
	{
		emit deleteItem(mBorrowInfo);
		this->close();
	}
}
