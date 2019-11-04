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
#include "UserSession.h"
#include "OperationLog.h"
#include <QFileDialog>

CUiCenter::CUiCenter(QWidget *parent)
	: QWidget(parent), mLineEdit(nullptr), mTableView(nullptr), mModel(nullptr)
{
	qRegisterMetaType<BorrowInfo>("BorrowInfo");
	this->initUi();
	this->initData();
}

CUiCenter::~CUiCenter()
{
}

void CUiCenter::initUi()
{
	//������
	QGridLayout *main_layout = new QGridLayout(this);
	main_layout->setContentsMargins(9, 0, 9, 9);
	main_layout->setSpacing(9);

	/*��ѯ�����*/
	mLineEdit = new CSearchLineEdit(this);
	connect(mLineEdit, &CSearchLineEdit::query, [this](const QString &info) {
		this->FuzzyQuery(info);
	});
	mLineEdit->setPlaceholderText(TOCH("���������Ϣ"));
	mLineEdit->setFixedHeight(24);

	m_pDataCount = new QLabel(this);
	m_pDataCount->setAlignment(Qt::AlignCenter);

	this->initTableView();
	Q_ASSERT(mTableView);
	Q_ASSERT(mLineEdit);
	main_layout->addWidget(mLineEdit, 0, 0, 1, 2);
	main_layout->addWidget(m_pDataCount, 0, 2, 1, 1);

	//�½�������ť
	QPushButton *btn_add = UiHelper::creatPushButton(this, [=]() {
		CEditInfoDialog *infoDialog = new CEditInfoDialog(this, false);
		connect(infoDialog, &CEditInfoDialog::saveData, this, [=](const BorrowInfo &info) {
			CDbHelper dbHelper;
			dbHelper.open();
			if (dbHelper.isTableExist(DIC_BORROW_RETURN))
			{
				ModelData model;
				model["productionId"]   = info.productionId;
				model["productionName"] = info.productionName;
				model["borrowerName"]   = info.borrowerName;
				model["borrowReason"]   = info.borrowReason;
				model["borrowStatus"]   = QString::number((int)info.borrowStatus);
				model["remarks"]        = info.remarks;
				model["createUserId"]   = UserSession::getInstance().userData().userId;
				int rows = dbHelper.Insert(DIC_BORROW_RETURN,model);
				if (rows > 0){
					QList<ModelData> vModel;
					int rows = dbHelper.Queuey(vModel, "SELECT * FROM DIC_BORROW_RETURN ORDER BY updateDate desc");
					this->setData(vModel);
				}
			}
			else {
				dbHelper.exec(CREATE_TABLE_SQL);
			}
		}, Qt::DirectConnection);
		PopupDialogContainer::showPopupDialogFadeIn(infoDialog, CApp->getMainWidget(),TOCH("�½�����"));
	}, 25, 25, "","btn_add");
	btn_add->setToolTip(TOCH("�½�����"));

	/*�򿪲�����־*/
	QPushButton *btn_log = UiHelper::creatPushButton(this, [=]() {
		OperationLog *content = new OperationLog(this);
		PopupDialogContainer::showPopupDialogFadeIn(content, CApp->getMainWidget(), TOCH("������־"));
	}, 25, 25, "", "btn_log");
	btn_log->setToolTip(TOCH("������־"));

	/*����*/
	QPushButton *btn_export = UiHelper::creatPushButton(this, [=]() {
		QFileDialog dialog;
		dialog.setFileMode(QFileDialog::ExistingFile);
		dialog.setAcceptMode(QFileDialog::AcceptSave);
		QStringList list;
		list << TOCH("Excel������ (*.xlsx)") << TOCH("Excel97-2003������(*.xls)");
		dialog.setNameFilters(list);
		dialog.selectFile(TOCH("�����Ƽ�%1�軹��Ϣ.xlsx").arg(TOCH("")));
		dialog.exec();
	}, 25, 25, "", "btn_export");
	btn_export->setToolTip(TOCH("��������"));

	main_layout->addWidget(btn_export, 0, 9, 1, 1);
	main_layout->addWidget(btn_log, 0, 10, 1, 1);
	main_layout->addWidget(btn_add, 0, 11, 1, 1);
	main_layout->addWidget(mTableView, 1, 0, 10, 12);
}

void CUiCenter::initHeader()
{
	Q_ASSERT(mTableView);
	QList<HeadStruct> listHead;
	HeadStruct headNode = { TOCH("���"),70,false };
	listHead << headNode;

	headNode = { TOCH("��Ʒ���"),-1,false };
	listHead << headNode;

	headNode = { TOCH("��Ʒ����"),150,false };
	listHead << headNode;

	headNode = { TOCH("������"),150 ,false };
	listHead << headNode;

	headNode = { TOCH("��������"),150 ,false };
	listHead << headNode;

	headNode = { TOCH("״̬"),80 ,false };
	listHead << headNode;

	headNode = { TOCH("����ԭ��"),-1,true };
	listHead << headNode;

	headNode = { TOCH("��ע"),-1,true };
	listHead << headNode;

	headNode = { TOCH("����ʱ��"),150,false };
	listHead << headNode;

	headNode = { TOCH("ΨһID"),-1,true };
	listHead << headNode;

	mTableView->setHeader(listHead);
}

void CUiCenter::initData()
{
	CDbHelper dbHelper;
	dbHelper.open();
	QList<ModelData> vModel;
	if (dbHelper.isTableExist(DIC_BORROW_RETURN))
	{
		int rows = dbHelper.Queuey(vModel, "SELECT * FROM DIC_BORROW_RETURN ORDER BY updateDate desc");
	}
	else {
		dbHelper.exec(CREATE_TABLE_SQL);
	}
	this->setData(vModel);
}

void CUiCenter::initTableView()
{
	mModel = new CustomTableModel;
	pProxyModel = new SortFilterProxyModel(this);
	pProxyModel->setSourceModel(mModel);

	mTableView = new CTableview(this);
	mTableView->setModel(pProxyModel);
	this->initHeader();
	mTableView->setCreatRowCallbackListener(this);

	// ���ÿ�����
	//mTableView->setSortingEnabled(true);

	//�����Ҽ��˵�
	mTableView->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(mTableView, SIGNAL(customContextMenuRequested(const QPoint &)),
		this, SLOT(slotContextMenu(const QPoint &)));
	connect(mTableView, SIGNAL(doubleClicked(const QModelIndex &)),
		this, SLOT(slotTableViewDoubleClicked(const QModelIndex &)));

	// ���ð����ļ�����������
	mTableView->sortByColumn(TableHeader::Status, Qt::AscendingOrder);
	mTableView->sortByColumn(TableHeader::BorrowDate, Qt::AscendingOrder);
	mTableView->sortByColumn(TableHeader::UpdateDate, Qt::AscendingOrder);

	mTableView->setItemDelegate(new ReadOnlyDelegateBRTable(this));
	mTableView->setItemDelegateForColumn(TableHeader::Order, new CheckBoxDelegate(this));
}

void CUiCenter::sqlQuery(const QString &sql)
{
	CDbHelper dbHelper;
	dbHelper.open();
	QList<ModelData> vModel;
	int rows = dbHelper.Queuey(vModel, sql);
	this->setData(vModel);
}

void CUiCenter::sqlUpdate(const ModelData &model,const QString &id)
{
	CDbHelper dbHelper;
	dbHelper.open();
	int rows = dbHelper.Update(DIC_BORROW_RETURN, model,QString("WHERE ID = %1").arg(id));
}

void CUiCenter::sqlDelete(const BorrowInfo &info)
{
	CDbHelper dbHelper;
	dbHelper.open();
	int rows = dbHelper.Delete(DIC_BORROW_RETURN, QString("ID = %1").arg(info.id));
}

void CUiCenter::FuzzyQuery(const QString &key)
{
	QString sql = "";
	if (key.isEmpty()) {
		sql = QString("SELECT * \
		FROM \
		DIC_BORROW_RETURN ORDER BY updateDate desc");
	}
	else {
		sql = QString("SELECT * \
		FROM \
		DIC_BORROW_RETURN \
		WHERE \
		productionId LIKE '%%1%' \
		OR productionName LIKE '%%1%' \
		OR borrowerName LIKE '%%1%' \
		OR borrowReason LIKE '%%1%' \
		OR remarks LIKE '%%1%' ORDER BY updateDate desc;").arg(key);
	}

	this->sqlQuery(sql);
}

void CUiCenter::setData(const QList<ModelData> &vModel)
{
	uint nLen = vModel.size();
	m_pDataCount->setText(TOCH("��<span style='color:rgb(0,122,204)'>%1</span>������").arg(nLen));
	mTableView->setData(vModel);
}

void CUiCenter::showDetailUi(const QModelIndex & index)
{
	QModelIndex indexFirst = pProxyModel->index(0, (int)TableHeader::Order);
	if (pProxyModel->data(indexFirst, Qt::UserRole).toBool()) return;

	CEditInfoDialog *infoDialog = new CEditInfoDialog(this);
	connect(infoDialog, &CEditInfoDialog::deleteItem, this, [=](const BorrowInfo &info) {
		this->sqlDelete(info);
		this->FuzzyQuery();
	}, Qt::DirectConnection);

	connect(infoDialog, &CEditInfoDialog::updateData, this, [=](const BorrowInfo &info) {
		ModelData model;
		model["productionId"] = info.productionId;
		model["productionName"] = info.productionName;
		model["borrowerName"] = info.borrowerName;
		model["borrowStatus"] = QString::number((int)info.borrowStatus);
		model["borrowReason"] = info.borrowReason;
		model["remarks"] = info.remarks;
		this->sqlUpdate(model, info.id);
		this->FuzzyQuery();
	}, Qt::DirectConnection);
	BorrowInfo info;
	this->getBorrowData(info, index.row());
	infoDialog->setData(info);
	PopupDialogContainer::showPopupDialogFadeIn(infoDialog, CApp->getMainWidget(), TOCH("�༭����"));
}

void CUiCenter::getBorrowData(BorrowInfo &info,int row)
{
	info.id = pProxyModel->data(pProxyModel->index(row, TableHeader::UniqueId), Qt::DisplayRole).toString();
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
	pProxyModel->setData(pProxyModel->index(row, TableHeader::UniqueId), info.id, Qt::DisplayRole);
	pProxyModel->setData(pProxyModel->index(row, TableHeader::ProductionId), info.productionId, Qt::DisplayRole);
	pProxyModel->setData(pProxyModel->index(row, TableHeader::ProductionName), info.productionName, Qt::DisplayRole);
	pProxyModel->setData(pProxyModel->index(row, TableHeader::BorrowerName), info.borrowerName, Qt::DisplayRole);
	pProxyModel->setData(pProxyModel->index(row, TableHeader::BorrowDate), info.borrowDate, Qt::UserRole);
	pProxyModel->setData(pProxyModel->index(row, TableHeader::Status), (int)info.borrowStatus, Qt::UserRole);
	pProxyModel->setData(pProxyModel->index(row, TableHeader::BorrowReason), info.borrowReason, Qt::DisplayRole);
	pProxyModel->setData(pProxyModel->index(row, TableHeader::Remark), info.remarks, Qt::DisplayRole);
}

QList<QStandardItem*> CUiCenter::creatRow(const ModelData &model, int index)
{
	QList<QStandardItem*> item;
	item.append(new QStandardItem(model["id"]));
	item.append(new QStandardItem(QString::number(index)));
	item.append(new QStandardItem(model["productionId"]));
	item.append(new QStandardItem(model["productionName"]));
	item.append(new QStandardItem(model["borrowerName"]));

	QStandardItem *itemDate = new QStandardItem;
	itemDate->setData(QDateTime::fromString(model["borrowDate"].left(16), "yyyy-MM-dd hh:mm"), Qt::UserRole);
	item.append(itemDate);

	QStandardItem *itemStatus = new QStandardItem;
	itemStatus->setData((BorrowStatus)model["borrowStatus"].toInt(), Qt::UserRole);
	item.append(itemStatus);

	item.append(new QStandardItem(model["borrowReason"]));
	item.append(new QStandardItem(model["remarks"]));

	QStandardItem *itemUpdateDate = new QStandardItem;
	itemUpdateDate->setData(QDateTime::fromString(model["updateDate"].left(16), "yyyy-MM-dd hh:mm"), Qt::UserRole);
	item.append(itemUpdateDate);

	//���������Ķ��뷽ʽ
	item.at(0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	item.at(1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	item.at(2)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	item.at(3)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	item.at(4)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	item.at(5)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

	return item;
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
			menu->addAction(TOCH("�鿴����"), [=] {
				this->showDetailUi(index);
			});

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
			if (ok == QMessageBox::Ok) {
				BorrowInfo info;
				this->getBorrowData(info, index.row());
				this->sqlDelete(info);
				this->FuzzyQuery();
			}
		});

		QModelIndex indexLast = pProxyModel->index(index.row(), (int)TableHeader::Status);
		BorrowStatus status = (BorrowStatus)pProxyModel->data(indexLast, Qt::UserRole).toInt();
		QString id = pProxyModel->data(pProxyModel->index(index.row(), (int)TableHeader::UniqueId)).toString();
		switch (status)
		{
		case Returned:
			menu->addAction(TOCH("���Ϊδ��"), [=] {
				ModelData model;
				model["borrowStatus"] = QString::number((int)BorrowStatus::NotReturned);
				this->sqlUpdate(model, id);
				pProxyModel->setData(indexLast, BorrowStatus::NotReturned, Qt::UserRole);
			});

			menu->addAction(TOCH("���Ϊ��ʧ"), [=] {
				ModelData model;
				model["borrowStatus"] = QString::number((int)BorrowStatus::Lost);
				this->sqlUpdate(model, id);
				pProxyModel->setData(indexLast, BorrowStatus::Lost, Qt::UserRole);
			});
			break;
		case NotReturned:
			menu->addAction(TOCH("���Ϊ�ѻ�"), [=] {
				ModelData model;
				model["borrowStatus"] = QString::number((int)BorrowStatus::Returned);
				this->sqlUpdate(model, id);
				pProxyModel->setData(indexLast, BorrowStatus::Returned, Qt::UserRole);
			});

			menu->addAction(TOCH("���Ϊ��ʧ"), [=] {
				ModelData model;
				model["borrowStatus"] = QString::number((int)BorrowStatus::Lost);
				this->sqlUpdate(model, id);
				pProxyModel->setData(indexLast, BorrowStatus::Lost, Qt::UserRole);
			});
			break;
		case Lost:
			menu->addAction(TOCH("���Ϊ�ѻ�"), [=] {
				ModelData model;
				model["borrowStatus"] = QString::number((int)BorrowStatus::Returned);
				this->sqlUpdate(model, id);
				pProxyModel->setData(indexLast, BorrowStatus::Returned, Qt::UserRole);
			});

			menu->addAction(TOCH("���Ϊδ��"), [=] {
				ModelData model;
				model["borrowStatus"] = QString::number((int)BorrowStatus::NotReturned);
				this->sqlUpdate(model, id);
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
	this->showDetailUi(index);
}
