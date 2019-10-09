#include "CUiCenter.h"
#include <QGridLayout>
#include "CSearchLineEdit.h"
#include <QTableView>

CUiCenter::CUiCenter(QWidget *parent)
	: QWidget(parent), mLineEdit(nullptr), mTableView(nullptr)
{
	QGridLayout *main_layout = new QGridLayout(this);
	mLineEdit = new CSearchLineEdit(this);
	mLineEdit->setPlaceholderText(QString::fromLocal8Bit("ÊäÈë¼ìË÷ÐÅÏ¢"));
	mLineEdit->setFixedHeight(24);
	main_layout->setContentsMargins(9, 0, 9, 9);
	main_layout->setSpacing(6);

	mTableView = new QTableView(this);
	main_layout->addWidget(mLineEdit,0,0,1,2);
	main_layout->addWidget(mTableView, 1, 0, 10, 12);

}

CUiCenter::~CUiCenter()
{
}
