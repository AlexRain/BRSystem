#include "OperationLog.h"
#include "ReadOnlyDelegate.h"
#include <QAbstractItemView>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QPushButton>
#include <QLabel>
#include "define.h"
#include "DialogMsg.h"

OperationLog::OperationLog(QWidget *parent)
	: BaseWidget(parent), m_pTableView(nullptr)
{
	this->resize(698,435);
	QGridLayout *layout = new QGridLayout(this);
	layout->setContentsMargins(9, 44, 9, 9);

	m_pTableView = new CTableview(this);
	this->initHeader();
	m_pTableView->setItemDelegate(new ReadOnlyDelegate(this));
	m_pTableView->setCreatRowCallbackListener(this);

	m_pLabelCount = new QLabel(this);
	m_pLabelCount->setAlignment(Qt::AlignRight);

	layout->addWidget(m_pTableView);
	layout->addWidget(m_pLabelCount);

	auto fun = std::bind(&OperationLog::refreshData, this);
	m_pBtnRefresh = UiHelper::creatPushButton(this, fun, 25, 25, "", "btn_refresh");
	m_pBtnRefresh->setWindowOpacity(0.5);
	m_pBtnRefresh->setToolTip(TOCH("刷新"));

	this->initData();
}

OperationLog::~OperationLog()
{
}

void OperationLog::refreshData()
{
	this->initData();
}

void OperationLog::initHeader()
{
	QList<HeadStruct> listHead;
	HeadStruct headNode = { TOCH("序号"),70,false };
	listHead << headNode;

	headNode = { TOCH("物品编号"),-1,false };
	listHead << headNode;

	headNode = { TOCH("物品名称"),150,false };
	listHead << headNode;

	headNode = { TOCH("操作事项"),150 ,false};
	listHead << headNode;

	headNode = { TOCH("操作人"),-1 ,false};
	listHead << headNode;

	headNode = { TOCH("操作时间"),150 ,false};
	listHead << headNode;

	headNode = { TOCH("凭据ID"),-1,true };
	listHead << headNode;
	m_pTableView->setHeader(listHead);
}

void OperationLog::initData()
{
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

void OperationLog::setData(const QList<ModelData> &datas)
{
	uint nLen = datas.size();
	m_pLabelCount->setText(TOCH("共查询到<span style='color:rgb(0,122,204)'>%1</span>条记录").arg(nLen));
	m_pTableView->setData(datas);
}

void OperationLog::resizeEvent(QResizeEvent *event)
{
	QRect rect = m_pTableView->rect();
	m_pBtnRefresh->move(rect.right() - 20 - m_pBtnRefresh->width(),rect.bottom() - 15);
	BaseWidget::resizeEvent(event);
}

QList<QStandardItem*> OperationLog::creatRow(const ModelData &model,int index)
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

	return item;
}
