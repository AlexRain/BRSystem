#include "CUiCenter.h"
#include <QGridLayout>
#include "CSearchLineEdit.h"
#include <QTableView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QHeaderView>
#include "ReadOnlyDelegate.h"
#include "ComboBoxDelegate.h"
#include "define.h"
#include "StyledDelegate.h"

CUiCenter::CUiCenter(QWidget *parent)
	: QWidget(parent), mLineEdit(nullptr), mTableView(nullptr), mModel(nullptr)
{
	QGridLayout *main_layout = new QGridLayout(this);
	mLineEdit = new CSearchLineEdit(this);
	mLineEdit->setPlaceholderText(TOCH("���������Ϣ"));
	mLineEdit->setFixedHeight(24);
	main_layout->setContentsMargins(9, 0, 9, 9);
	main_layout->setSpacing(6);

	mModel = new QStandardItemModel;
	mModel->setHorizontalHeaderItem(TableHeader::Oder, new QStandardItem(TOCH("���")));
	mModel->setHorizontalHeaderItem(TableHeader::ProductionId, new QStandardItem(TOCH("��Ʒ���")));
	mModel->setHorizontalHeaderItem(TableHeader::ProductionName, new QStandardItem(TOCH("��Ʒ����")));
	mModel->setHorizontalHeaderItem(TableHeader::BorrowerName, new QStandardItem(TOCH("����������")));
	mModel->setHorizontalHeaderItem(TableHeader::BorrowDate, new QStandardItem(TOCH("����ʱ��")));
	mModel->setHorizontalHeaderItem(TableHeader::Status, new QStandardItem(TOCH("״̬")));

	mTableView = new QTableView(this);
	mTableView->setModel(mModel);
	mTableView->verticalHeader()->setVisible(false);
	//mTableView->horizontalHeader()->setStretchLastSection(true);
	mTableView->horizontalHeader()->setSectionResizeMode(TableHeader::ProductionName, QHeaderView::Stretch);
	//mTableView->horizontalHeader()->sectionResizeMode(QHeaderView::Stretch);
	mTableView->horizontalHeader()->setHighlightSections(false);
	mTableView->setItemDelegate(new StyledDelegate(this));
	mTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
	mTableView->setSelectionMode(QAbstractItemView::SingleSelection);
	mTableView->setAlternatingRowColors(true);
	mTableView->setFrameShape(QFrame::NoFrame);
	mTableView->setColumnWidth(TableHeader::Status,80);

	mTableView->setItemDelegateForColumn(TableHeader::Oder, new ReadOnlyDelegate(this));
	mTableView->setItemDelegateForColumn(TableHeader::ProductionId, new ReadOnlyDelegate(this));
	mTableView->setItemDelegateForColumn(TableHeader::ProductionName, new ReadOnlyDelegate(this));
	mTableView->setItemDelegateForColumn(TableHeader::BorrowerName, new ReadOnlyDelegate(this));
	mTableView->setItemDelegateForColumn(TableHeader::BorrowDate, new ReadOnlyDelegate(this));
	mTableView->setItemDelegateForColumn(TableHeader::Status,new ComboBoxDelegate(this));

	main_layout->addWidget(mLineEdit,0,0,1,2);
	main_layout->addWidget(mTableView, 1, 0, 10, 12);
	initData();
}

CUiCenter::~CUiCenter()
{
}

void CUiCenter::initData()
{
	QList<QStandardItem*> item;
	item.append(new QStandardItem("1"));
	item.append(new QStandardItem("HS1001"));
	item.append(new QStandardItem(TOCH("C30����̽ͷ")));
	item.append(new QStandardItem(TOCH("���")));
	item.append(new QStandardItem(TOCH("2018.06.18")));
	item.append(new QStandardItem(TOCH("δ��")));
	//���������Ķ��뷽ʽ
	item.at(0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	item.at(1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	item.at(2)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	item.at(3)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	item.at(4)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	item.at(5)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	mModel->insertRow(mModel->rowCount(), item);
	mModel->insertRow(mModel->rowCount(), item);
	mModel->insertRow(mModel->rowCount(), item);
	mModel->insertRow(mModel->rowCount(), item);
	mModel->insertRow(mModel->rowCount(), item);
	mModel->insertRow(mModel->rowCount(), item);
}
