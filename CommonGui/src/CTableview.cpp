#include "CTableview.h"
#include <QGridLayout>
#include <QStandardItemModel>

CTableview::CTableview(QWidget *parent)
	: QTableView(parent), m_pSetDataListener(nullptr), m_pModel(nullptr), m_pTip(nullptr)
{
	this->init();
}

CTableview::~CTableview()
{
}

void CTableview::setHeader(const QList<HeadStruct> &headList)
{
	m_headList.clear();
	m_headList = headList;
	this->initHeader();
}

void CTableview::setData(const QList<ModelData> &data)
{
	m_pModel->clear();
	this->initHeader();
	uint nLen = data.size();
	if (nLen <= 0) {
		m_pTip->setText(TOCH("暂无数据"));
		m_pTip->show();
		return;
	}
	m_pTip->hide();
	Q_ASSERT(m_pSetDataListener);
	for (int i = 0; i < nLen; ++i)
	{
		const ModelData &model = data[i];
		auto row = m_pSetDataListener->creatRow(model,i + 1);
		m_pModel->appendRow(row);
	}
}

void CTableview::setCreatRowCallbackListener(SetDataCallback *listener)
{
	this->m_pSetDataListener = listener;
}

void CTableview::init()
{
	m_pModel = new TableModel;
	this->setModel(m_pModel);
	CHeaderView *headerView = new CHeaderView(Qt::Horizontal,this);
	this->setHorizontalHeader(headerView);

	QGridLayout *layout = new QGridLayout(this);

	/*提示label*/
	m_pTip = new QLabel(this);
	QSizePolicy sp_retain = m_pTip->sizePolicy();
	sp_retain.setRetainSizeWhenHidden(true);
	m_pTip->setSizePolicy(sp_retain);
	m_pTip->setAlignment(Qt::AlignCenter);
	m_pTip->setText(TOCH("暂无数据"));
	layout->addWidget(m_pTip);

	//设置右键菜单
	this->setContextMenuPolicy(Qt::CustomContextMenu);
	this->verticalHeader()->setVisible(false);
	this->setShowGrid(true);
	this->setEditTriggers(QAbstractItemView::NoEditTriggers);
	this->setSelectionBehavior(QAbstractItemView::SelectRows);
	this->setSelectionMode(QAbstractItemView::SingleSelection);
	this->setAlternatingRowColors(true);
	this->setFrameShape(QFrame::NoFrame);
}

void CTableview::initHeader()
{
	QStringList list;
	for each (const HeadStruct &headInfo in m_headList)
	{
		list << headInfo.headText;
	}

	Q_ASSERT(m_pModel);
	m_pModel->setHorizontalHeaderLabels(list);
	for(int i = 0; i < m_headList.size();++i)
	{
		const HeadStruct &head = m_headList.at(i);
		if (head.isHidden){
			this->setColumnHidden(i,true);
		}
		if (head.columnWidth > 0) {
			this->setColumnWidth(i, head.columnWidth);
		}
	}
}
