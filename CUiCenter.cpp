#include "CUiCenter.h"
#include <QGridLayout>
#include "CSearchLineEdit.h"
#include <QTableView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QMenu>
#include <QCursor>
#include <QHeaderView>
#include <QMessageBox>
#include "ReadOnlyDelegate.h"
#include "ComboBoxDelegate.h"
#include "define.h"
#include "StyledDelegate.h"
#include "CheckBoxDelegate.h"

CUiCenter::CUiCenter(QWidget *parent)
	: QWidget(parent), mLineEdit(nullptr), mTableView(nullptr), mModel(nullptr)
{
	QGridLayout *main_layout = new QGridLayout(this);
	mLineEdit = new CSearchLineEdit(this);
	mLineEdit->setPlaceholderText(TOCH("输入检索信息"));
	mLineEdit->setFixedHeight(24);
	main_layout->setContentsMargins(9, 0, 9, 9);
	main_layout->setSpacing(6);

	mModel = new QStandardItemModel;
	mModel->setHorizontalHeaderItem(TableHeader::Order, new QStandardItem(TOCH("序号")));
	mModel->setHorizontalHeaderItem(TableHeader::ProductionId, new QStandardItem(TOCH("产品编号")));
	mModel->setHorizontalHeaderItem(TableHeader::ProductionName, new QStandardItem(TOCH("产品名称")));
	mModel->setHorizontalHeaderItem(TableHeader::BorrowerName, new QStandardItem(TOCH("借用人姓名")));
	mModel->setHorizontalHeaderItem(TableHeader::BorrowDate, new QStandardItem(TOCH("借用时间")));
	mModel->setHorizontalHeaderItem(TableHeader::Status, new QStandardItem(TOCH("状态")));

	mTableView = new QTableView(this);
	mTableView->setModel(mModel);
	mTableView->verticalHeader()->setVisible(false);
	//mTableView->horizontalHeader()->setStretchLastSection(true);
	mTableView->horizontalHeader()->setSectionResizeMode(TableHeader::ProductionName, QHeaderView::Stretch);
	//mTableView->horizontalHeader()->sectionResizeMode(QHeaderView::Stretch);
	mTableView->horizontalHeader()->setHighlightSections(false);
	mTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
	mTableView->setSelectionMode(QAbstractItemView::SingleSelection);
	mTableView->setAlternatingRowColors(true);
	mTableView->setFrameShape(QFrame::NoFrame);
	mTableView->setColumnWidth(TableHeader::Status, 80);
	mTableView->setColumnWidth(TableHeader::Order, 70);
	mTableView->setShowGrid(true);
	mTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	mTableView->setItemDelegate(new ReadOnlyDelegate(this));
	mTableView->setItemDelegateForColumn(TableHeader::Order, new CheckBoxDelegate(this));

	//设置右键菜单
	mTableView->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(mTableView, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(slotContextMenu(const QPoint &)));

	main_layout->addWidget(mLineEdit,0,0,1,2);
	main_layout->addWidget(mTableView, 1, 0, 10, 12);
	initData();
}

CUiCenter::~CUiCenter()
{
}

void CUiCenter::initData()
{
	QList<QStandardItem*> item1;
	item1.append(new QStandardItem("1"));
	item1.append(new QStandardItem("HS1001"));
	item1.append(new QStandardItem(TOCH("C30线阵探头")));
	item1.append(new QStandardItem(TOCH("李白")));
	item1.append(new QStandardItem(TOCH("2018.06.18")));
	item1.append(new QStandardItem(TOCH("未还")));
	//设置列数的对齐方式
	item1.at(0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	item1.at(1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	item1.at(2)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	item1.at(3)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	item1.at(4)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	item1.at(5)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

	QList<QStandardItem*> item2;
	item2.append(new QStandardItem("2"));
	item2.append(new QStandardItem("HS1001"));
	item2.append(new QStandardItem(TOCH("C30线阵探头")));
	item2.append(new QStandardItem(TOCH("李白")));
	item2.append(new QStandardItem(TOCH("2018.06.18")));
	item2.append(new QStandardItem(TOCH("未还")));
	//设置列数的对齐方式
	item2.at(0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	item2.at(1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	item2.at(2)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	item2.at(3)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	item2.at(4)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	item2.at(5)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

	QList<QStandardItem*> item3;
	item3.append(new QStandardItem("3"));
	item3.append(new QStandardItem("HS1001"));
	item3.append(new QStandardItem(TOCH("C30线阵探头")));
	item3.append(new QStandardItem(TOCH("李白")));
	item3.append(new QStandardItem(TOCH("2018.06.18")));
	item3.append(new QStandardItem(TOCH("未还")));
	//设置列数的对齐方式
	item3.at(0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	item3.at(1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	item3.at(2)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	item3.at(3)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	item3.at(4)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	item3.at(5)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

	QList<QStandardItem*> item4;
	item4.append(new QStandardItem("4"));
	item4.append(new QStandardItem("HS1001"));
	item4.append(new QStandardItem(TOCH("C30线阵探头")));
	item4.append(new QStandardItem(TOCH("李白")));
	item4.append(new QStandardItem(TOCH("2018.06.18")));
	item4.append(new QStandardItem(TOCH("未还")));
	//设置列数的对齐方式
	item4.at(0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	item4.at(1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	item4.at(2)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	item4.at(3)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	item4.at(4)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	item4.at(5)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

	mModel->insertRow(mModel->rowCount(), item1);
	mModel->insertRow(mModel->rowCount(), item2);
	mModel->insertRow(mModel->rowCount(), item3);
	mModel->insertRow(mModel->rowCount(), item4);
}

void CUiCenter::slotContextMenu(const QPoint &pos)
{
	QModelIndex index = mTableView->indexAt(pos);
	if (index.isValid())
	{
		QMenu *menu = new QMenu(mTableView);
		menu->addAction(TOCH("删除"), [=] {
			QMessageBox::StandardButton ok = QMessageBox::question(this, TOCH("提示"), TOCH("确定要删除这条记录吗？"), QMessageBox::Ok | QMessageBox::Cancel);
			if (ok == QMessageBox::Ok)
			{
				mModel->removeRow(index.row());
			}
		});
		QModelIndex indexLast = mModel->index(index.row(), (int)TableHeader::Status);
		QString text = mModel->data(indexLast, Qt::DisplayRole).toString();
		bool returned = text == TOCH("已还");
		menu->addAction(returned ? TOCH("标记为未还") : TOCH("标记为已还"), [=] {
			if (returned)
				mModel->setData(indexLast, TOCH("未还"), Qt::DisplayRole);
			else
				mModel->setData(indexLast, TOCH("已还"), Qt::DisplayRole);

		});
		menu->exec(QCursor::pos());
	}
}
