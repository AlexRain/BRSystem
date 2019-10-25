#include "CUiCenter.h"
#include <QGridLayout>
#include "CSearchLineEdit.h"
#include <QTableView>
#include <QStandardItem>
#include <QMenu>
#include <QCursor>
#include <QHeaderView>
#include <QMessageBox>
#include <QSortFilterProxyModel>
#include "ReadOnlyDelegate.h"
#include "ComboBoxDelegate.h"
#include "StyledDelegate.h"
#include "CheckBoxDelegate.h"
#include "FilterBtnDelegate.h"
#include "SortFilterProxyModel.h"
#include "CEditInfoDialog.h"
#include "PopupDialogContainer.h"
#include "CApplication.h"
#include "DialogMsg.h"

CUiCenter::CUiCenter(QWidget *parent)
	: QWidget(parent), mLineEdit(nullptr), mTableView(nullptr), mModel(nullptr)
{
	qRegisterMetaType<BorrowInfo>("BorrowInfo");
	initUi();
}

CUiCenter::~CUiCenter()
{
}

void CUiCenter::initUi()
{
	//������
	QGridLayout *main_layout = new QGridLayout(this);
	mLineEdit = new CSearchLineEdit(this);
	mLineEdit->setPlaceholderText(TOCH("���������Ϣ"));
	mLineEdit->setFixedHeight(24);
	main_layout->setContentsMargins(9, 0, 9, 9);
	main_layout->setSpacing(9);
	this->initTableView();
	Q_ASSERT(mTableView);
	Q_ASSERT(mLineEdit);
	main_layout->addWidget(mLineEdit, 0, 0, 1, 2);

	//�½�������ť
	//auto func = std::bind(&CUiCenter::test,this);
	QPushButton *btn_add = UiHelper::creatPushButton(this, [=]() {
		CEditInfoDialog *infoDialog = new CEditInfoDialog(this, false);
		connect(infoDialog, &CEditInfoDialog::saveData, this, [=](const BorrowInfo &info) {
			this->appendRow(info);
		}, Qt::DirectConnection);
		PopupDialogContainer::showPopupDialogFadeIn(infoDialog, CApp->getMainWidget(),TOCH("�½�����"));
	}, 25, 25, "","btn_add");
	btn_add->setToolTip(TOCH("�½�����"));

	main_layout->addWidget(btn_add, 0, 11, 1, 1);
	main_layout->addWidget(mTableView, 1, 0, 10, 12);
}

void CUiCenter::initTableView()
{
	mModel = new CustomTableModel;
	mModel->setHorizontalHeaderItem(TableHeader::Order, new QStandardItem(TOCH("���")));
	mModel->setHorizontalHeaderItem(TableHeader::ProductionId, new QStandardItem(TOCH("��Ʒ���")));
	mModel->setHorizontalHeaderItem(TableHeader::ProductionName, new QStandardItem(TOCH("��Ʒ����")));
	mModel->setHorizontalHeaderItem(TableHeader::BorrowerName, new QStandardItem(TOCH("������")));
	mModel->setHorizontalHeaderItem(TableHeader::BorrowDate, new QStandardItem(TOCH("����ʱ��")));
	mModel->setHorizontalHeaderItem(TableHeader::Status, new QStandardItem(TOCH("״̬")));
	mModel->setHorizontalHeaderItem(TableHeader::BorrowReason, new QStandardItem(TOCH("����ԭ��")));
	mModel->setHorizontalHeaderItem(TableHeader::Remark, new QStandardItem(TOCH("��ע")));
	mModel->setHorizontalHeaderItem(TableHeader::UpdateDate, new QStandardItem(TOCH("����ʱ��")));

	pProxyModel = new SortFilterProxyModel(this);
	pProxyModel->setSourceModel(mModel);

	mTableView = new QTableView(this);
	mTableView->setModel(pProxyModel);
	// ���ÿ�����
	mTableView->setSortingEnabled(true);
	// ���ð����ļ�����������
	mTableView->sortByColumn(TableHeader::Status, Qt::AscendingOrder);
	mTableView->sortByColumn(TableHeader::BorrowDate, Qt::AscendingOrder);

	mTableView->verticalHeader()->setVisible(false);
	mTableView->horizontalHeader()->setSectionResizeMode(TableHeader::ProductionName, QHeaderView::Stretch);
	mTableView->horizontalHeader()->setHighlightSections(false);

	/*������*/
	mTableView->setColumnHidden(TableHeader::BorrowReason, true);
	mTableView->setColumnHidden(TableHeader::Remark, true);
	mTableView->setColumnHidden(TableHeader::UpdateDate, true);

	mTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
	mTableView->setSelectionMode(QAbstractItemView::SingleSelection);
	mTableView->setAlternatingRowColors(true);
	mTableView->setFrameShape(QFrame::NoFrame);

	mTableView->setColumnWidth(TableHeader::Status,     80);
	mTableView->setColumnWidth(TableHeader::Order,      70);
	mTableView->setColumnWidth(TableHeader::BorrowDate, 150);

	mTableView->setShowGrid(true);
	mTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	mTableView->setItemDelegate(new ReadOnlyDelegate(this));
	mTableView->setItemDelegateForColumn(TableHeader::Order, new CheckBoxDelegate(this));

	//�����Ҽ��˵�
	mTableView->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(mTableView, SIGNAL(customContextMenuRequested(const QPoint &)),
		this, SLOT(slotContextMenu(const QPoint &)));
	connect(mTableView, SIGNAL(doubleClicked(const QModelIndex &)),
		this, SLOT(slotTableViewDoubleClicked(const QModelIndex &)));
}

void CUiCenter::appendRow(const BorrowInfo &info)
{
	QList<QStandardItem*> item;
	item.append(new QStandardItem(QString::number(info.order)));
	item.append(new QStandardItem(info.productionId));
	item.append(new QStandardItem(info.productionName));
	item.append(new QStandardItem(info.borrowerName));
	QStandardItem *itemDate = new QStandardItem;
	itemDate->setData(info.borrowDate, Qt::UserRole);
	item.append(itemDate);
	QStandardItem *itemStatus = new QStandardItem;
	itemStatus->setData((int)info.borrowStatus, Qt::UserRole);
	item.append(itemStatus);

	item.append(new QStandardItem(info.borrowReason));
	item.append(new QStandardItem(info.remarks));

	QStandardItem *itemUpdateDate = new QStandardItem;
	itemUpdateDate->setData(info.updateDate, Qt::UserRole);
	item.append(itemUpdateDate);

	//���������Ķ��뷽ʽ
	item.at(0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	item.at(1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	item.at(2)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	item.at(3)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	item.at(4)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	item.at(5)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	mModel->appendRow(item);
}

void CUiCenter::getBorrowData(BorrowInfo &info,int row)
{
	info.productionId = pProxyModel->data(pProxyModel->index(row, TableHeader::ProductionId), Qt::DisplayRole).toString();
	info.productionName = pProxyModel->data(pProxyModel->index(row, TableHeader::ProductionName), Qt::DisplayRole).toString();
	info.borrowerName = pProxyModel->data(pProxyModel->index(row, TableHeader::BorrowerName), Qt::DisplayRole).toString();
	info.borrowDate = pProxyModel->data(pProxyModel->index(row, TableHeader::BorrowDate), Qt::UserRole).toDateTime();
	info.borrowStatus = (BorrowStatus)pProxyModel->data(pProxyModel->index(row, TableHeader::Status), Qt::UserRole).toInt();
	info.borrowReason = pProxyModel->data(pProxyModel->index(row, TableHeader::BorrowReason), Qt::DisplayRole).toString();
	info.remarks = pProxyModel->data(pProxyModel->index(row, TableHeader::Remark), Qt::DisplayRole).toString();
	info.updateDate = pProxyModel->data(pProxyModel->index(row, TableHeader::UpdateDate), Qt::UserRole).toDateTime();
}

void CUiCenter::setBorrowData(const BorrowInfo &info, int row)
{
	pProxyModel->setData(pProxyModel->index(row, TableHeader::ProductionId), info.productionId, Qt::DisplayRole);
	pProxyModel->setData(pProxyModel->index(row, TableHeader::ProductionName), info.productionName, Qt::DisplayRole);
	pProxyModel->setData(pProxyModel->index(row, TableHeader::BorrowerName), info.borrowerName, Qt::DisplayRole);
	pProxyModel->setData(pProxyModel->index(row, TableHeader::BorrowDate), info.borrowDate, Qt::UserRole);
	pProxyModel->setData(pProxyModel->index(row, TableHeader::Status), (int)info.borrowStatus, Qt::UserRole);
	pProxyModel->setData(pProxyModel->index(row, TableHeader::BorrowReason), info.borrowReason, Qt::DisplayRole);
	pProxyModel->setData(pProxyModel->index(row, TableHeader::Remark), info.remarks, Qt::DisplayRole);
}

/*��ʾ�Ҽ��˵�*/
void CUiCenter::slotContextMenu(const QPoint &pos)
{
	QModelIndex index = mTableView->indexAt(pos);
	if (index.isValid())
	{
		QMenu *menu = new QMenu(mTableView);

		QModelIndex indexFirst = pProxyModel->index(0, (int)TableHeader::Order);
		if (!pProxyModel->data(indexFirst,Qt::UserRole).toBool())
		{
			menu->addAction(TOCH("��ѡ"), [=] {
				for (int row = 0; row < pProxyModel->rowCount(); ++row)
				{
					QModelIndex checkBox = pProxyModel->index(row, (int)TableHeader::Order);
					pProxyModel->setData(checkBox, true, Qt::UserRole);
				}
			});
		}
		else{
			menu->addAction(TOCH("ȡ����ѡ"), [=] {
				for (int row = 0; row < pProxyModel->rowCount(); ++row)
				{
					QModelIndex checkBox = pProxyModel->index(row, (int)TableHeader::Order);
					pProxyModel->setData(checkBox, false, Qt::UserRole);
					pProxyModel->setData(checkBox, false, Qt::CheckStateRole);
				}
			});
		}

		menu->addAction(TOCH("ɾ��"), [=] {
			QMessageBox::StandardButton ok = DialogMsg::question(CApp->getMainWidget(), TOCH("��ʾ"),
				TOCH("ȷ��Ҫɾ��������¼��"), QMessageBox::Ok | QMessageBox::Cancel);
			if (ok == QMessageBox::Ok)
				pProxyModel->removeRow(index.row());
		});

		QModelIndex indexLast = pProxyModel->index(index.row(), (int)TableHeader::Status);
		BorrowStatus status = (BorrowStatus)pProxyModel->data(indexLast, Qt::UserRole).toInt();
		switch (status)
		{
		case Returned:
			menu->addAction(TOCH("���Ϊδ��"), [=] {
				pProxyModel->setData(indexLast, BorrowStatus::NotReturned, Qt::UserRole);
			});

			menu->addAction(TOCH("���Ϊ��ʧ"), [=] {
				pProxyModel->setData(indexLast, BorrowStatus::Lost, Qt::UserRole);
			});
			break;
		case NotReturned:
			menu->addAction(TOCH("���Ϊ�ѻ�"), [=] {
				pProxyModel->setData(indexLast, BorrowStatus::Returned, Qt::UserRole);
			});

			menu->addAction(TOCH("���Ϊ��ʧ"), [=] {
				pProxyModel->setData(indexLast, BorrowStatus::Lost, Qt::UserRole);
			});
			break;
		case Lost:
			menu->addAction(TOCH("���Ϊ�ѻ�"), [=] {
				pProxyModel->setData(indexLast, BorrowStatus::Returned, Qt::UserRole);
			});

			menu->addAction(TOCH("���Ϊδ��"), [=] {
				pProxyModel->setData(indexLast, BorrowStatus::NotReturned, Qt::UserRole);
			});
			break;
		default:
			break;
		}
		menu->exec(QCursor::pos());
	}
}

void CUiCenter::slotTableViewDoubleClicked(const QModelIndex &index)
{
	QModelIndex indexFirst = pProxyModel->index(0, (int)TableHeader::Order);
	if (pProxyModel->data(indexFirst, Qt::UserRole).toBool()) return;

	CEditInfoDialog *infoDialog = new CEditInfoDialog(this);
	connect(infoDialog, &CEditInfoDialog::deleteItem,this, [=](const BorrowInfo &info) {
		pProxyModel->removeRow(index.row());
	},Qt::DirectConnection);

	connect(infoDialog, &CEditInfoDialog::updateData, this, [=](const BorrowInfo &info) {
		this->setBorrowData(info,index.row());
	},Qt::DirectConnection);
	BorrowInfo info;
	this->getBorrowData(info, index.row());
	infoDialog->setData(info);
	PopupDialogContainer::showPopupDialogFadeIn(infoDialog, CApp->getMainWidget(),TOCH("�༭����"));
}
