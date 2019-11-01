#include "OperationLog.h"
#include "ReadOnlyDelegate.h"
#include <QAbstractItemView>
#include <QStandardItemModel>
#include <QPushButton>
#include <QLabel>
#include "define.h"
#include "DialogMsg.h"

OperationLog::OperationLog(QWidget *parent)
	: BaseWidget(parent)
{
	ui.setupUi(this);
	m_pModel = new QStandardItemModel;
	auto fun = std::bind(&OperationLog::refreshData, this);
	m_pBtnRefresh = UiHelper::creatPushButton(this, fun, 25, 25, "", "btn_refresh");
	m_pBtnRefresh->setWindowOpacity(0.5);
	m_pBtnRefresh->setToolTip(TOCH("刷新"));
	ui.label_count->setAlignment(Qt::AlignRight);

	/*提示label*/
	m_pTip = new QLabel(this);
	QSizePolicy sp_retain = m_pTip->sizePolicy();
	sp_retain.setRetainSizeWhenHidden(true);
	m_pTip->setSizePolicy(sp_retain);
	m_pTip->setAlignment(Qt::AlignCenter);
	m_pTip->setText(TOCH("暂无数据"));

	ui.gridLayout->addWidget(m_pTip,0,0);
	ui.gridLayout->setAlignment(m_pTip, Qt::AlignCenter);
	this->initHeader();
	this->initTableview();
	this->initColumn();
	this->initData();
}

OperationLog::~OperationLog()
{
}

void OperationLog::refreshData()
{
	this->initData();
}

void OperationLog::initData()
{
	m_pTip->setText(TOCH("正在加载..."));
	m_pTip->show();
	CDbHelper dbHelper;
	dbHelper.open();
	QList<ModelData> vModel;
	if (dbHelper.isTableExist(DIC_OPERATION_LOG))
	{
		/*联表查*/
		int rows = dbHelper.Queuey(vModel, "SELECT log.productionId,log.productionName,log.operateItem,log.operateTime,log.operateUserId,log.iouId,user.userName \
FROM DIC_OPERATION_LOG AS log INNER JOIN DIC_USER AS user ON log.operateUserId = user.userId ORDER BY operateTime desc");
	}
	else {
		DialogMsg::question(this, TOCH("错误"), TOCH("暂无相关数据库表,请联系管理员"), QMessageBox::Ok);
	}
	this->setData(vModel);
}

void OperationLog::initTableview()
{
	Q_ASSERT(m_pModel);
	ui.tableView->verticalHeader()->setVisible(false);

	ui.tableView->setShowGrid(true);
	ui.tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.tableView->setItemDelegate(new ReadOnlyDelegate(this));

	ui.tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui.tableView->setSelectionMode(QAbstractItemView::SingleSelection);
	ui.tableView->setAlternatingRowColors(true);
	ui.tableView->setFrameShape(QFrame::NoFrame);
	ui.tableView->setModel(m_pModel);
}

void OperationLog::initHeader()
{
	Q_ASSERT(m_pModel);
	m_pModel->setHorizontalHeaderItem(Order, new QStandardItem(TOCH("序号")));
	m_pModel->setHorizontalHeaderItem(ProductionId, new QStandardItem(TOCH("物品编号")));
	m_pModel->setHorizontalHeaderItem(ProductionName, new QStandardItem(TOCH("物品名称")));
	m_pModel->setHorizontalHeaderItem(OperationDetail, new QStandardItem(TOCH("操作事项")));
	m_pModel->setHorizontalHeaderItem(OperationUser, new QStandardItem(TOCH("操作人")));
	m_pModel->setHorizontalHeaderItem(OperationDateTime, new QStandardItem(TOCH("操作时间")));
	m_pModel->setHorizontalHeaderItem(IouId, new QStandardItem(TOCH("凭据ID")));
}

void OperationLog::initColumn()
{
	ui.tableView->horizontalHeader()->setSectionResizeMode(ProductionName, QHeaderView::Stretch);
	ui.tableView->horizontalHeader()->setHighlightSections(false);
	/*隐藏列*/
	ui.tableView->setColumnHidden(IouId, true);

	ui.tableView->setColumnWidth(Order, 70);
	ui.tableView->setColumnWidth(OperationDetail, 150);
	ui.tableView->setColumnWidth(OperationDateTime, 150);
}

void OperationLog::appendRow(const ModelData &model,int index)
{
	QList<QStandardItem*> item;
	item.append(new QStandardItem(QString::number(index)));
	item.append(new QStandardItem(model["productionId"]));
	item.append(new QStandardItem(model["productionName"]));
	item.append(new QStandardItem(model["operateItem"]));
	item.append(new QStandardItem(model["userName"]));
	item.append(new QStandardItem(model["operateTime"]));
	item.append(new QStandardItem(model["iouId"]));

	//设置列数的对齐方式
	item.at(0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	item.at(1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	item.at(2)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	item.at(3)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	item.at(4)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	item.at(5)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	item.at(6)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	m_pModel->appendRow(item);
}

void OperationLog::setData(const QList<ModelData> &datas)
{
	m_pModel->clear();
	this->initHeader();
	this->initColumn();
	uint nLen = datas.size();
	ui.label_count->setText(TOCH("共查询到<span style='color:rgb(0,122,204)'>%1</span>条记录").arg(nLen));
	if (nLen <= 0){
		m_pTip->setText(TOCH("暂无数据"));
		m_pTip->show();
		return;
	}

	m_pTip->hide();
	for (int i = 0; i < nLen; ++i)
	{
		const ModelData &model = datas[i];
		this->appendRow(model,i + 1);
	}
}

void OperationLog::resizeEvent(QResizeEvent *event)
{
	QRect rect = ui.tableView->rect();
	m_pBtnRefresh->move(rect.right() - 20 - m_pBtnRefresh->width(),rect.bottom() - 15);
	BaseWidget::resizeEvent(event);
}
