#include "CTableview.h"
#include <QGridLayout>
#include <QStandardItemModel>

CTableview::CTableview(QWidget* parent)
    : QTableView(parent)
    , m_pSetDataListener(nullptr)
    , m_pModel(nullptr)
    , m_pTip(nullptr)
    , m_pHeaderView(nullptr)
{
    this->init();
}

CTableview::~CTableview()
{
}

void CTableview::setHeader(const QList<HeadStruct>& headList)
{
    m_headList.clear();
    m_headList = headList;
    this->initHeader();
}

void CTableview::setData(const QList<ModelData>& data)
{
    m_pModel->clear();
    this->initHeader();
    uint nLen = data.size();
    if (nLen <= 0) {
        m_pTip->setText(tr("no data"));
        m_pTip->show();
        return;
    }
    m_pTip->hide();
    Q_ASSERT(m_pSetDataListener);
    for (int i = 0; i < nLen; ++i) {
        const ModelData& model = data[i];
        auto row = m_pSetDataListener->creatRow(model, i + 1);
        m_pModel->appendRow(row);
    }
}

void CTableview::setCreatRowCallbackListener(SetDataCallback* listener)
{
    this->m_pSetDataListener = listener;
}

void CTableview::setHeaderCheckBoxEnable(int logicalIndex, bool enable)
{
    Q_ASSERT(m_pHeaderView);
    m_pHeaderView->setSectionCheckBoxEnable(logicalIndex, enable);
}

void CTableview::showMutipleSelectionMode()
{
    for (int row = 0; row < m_pModel->rowCount(); ++row) {
        for (int column = 0; column < m_pModel->columnCount(); ++column) {
            if (checkBoxColumn.contains(column) && checkBoxColumn[column]) {
                QModelIndex checkBox = m_pModel->index(row, column);
                m_pModel->setData(checkBox, true, Qt::UserRole);
                m_pModel->setHeaderData(column, Qt::Horizontal, true, Qt::UserRole);
            }
        }
    }
}

void CTableview::exitMutipleSelectionMode()
{
    for (int row = 0; row < m_pModel->rowCount(); ++row) {
        for (int column = 0; column < m_pModel->columnCount(); ++column) {
            if (checkBoxColumn.contains(column) && checkBoxColumn[column]) {
                QModelIndex checkBox = m_pModel->index(row, column);
                m_pModel->setData(checkBox, false, Qt::UserRole);
                m_pModel->setData(checkBox, false, Qt::CheckStateRole);
                m_pModel->setHeaderData(column, Qt::Horizontal, false, Qt::UserRole);
            }
        }
    }
}

void CTableview::setItemDelegateForColumn(int column, QAbstractItemDelegate* delegate)
{
    if (delegate->inherits("CheckBoxDelegate")) {
        this->checkBoxColumn[column] = true;
    }
    QTableView::setItemDelegateForColumn(column, delegate);
}

void CTableview::init()
{
    m_pModel = new TableModel;
    this->setModel(m_pModel);
    m_pHeaderView = new CHeaderView(Qt::Horizontal, this);
    connect(m_pHeaderView, &CHeaderView::checkStateChanged, this, [=](bool checked) {
        if (checked) {
            this->selectAll();
        } else {
            this->clearSelection();
        }
    });
    this->setHorizontalHeader(m_pHeaderView);
    this->horizontalHeader()->setHighlightSections(false);

    QGridLayout* layout = new QGridLayout(this);

    /*提示label*/
    m_pTip = new QLabel(this);
    QSizePolicy sp_retain = m_pTip->sizePolicy();
    sp_retain.setRetainSizeWhenHidden(true);
    m_pTip->setSizePolicy(sp_retain);
    m_pTip->setAlignment(Qt::AlignCenter);
    m_pTip->setText(tr("no data"));
    layout->addWidget(m_pTip, 0, 0);
    layout->setAlignment(m_pTip, Qt::AlignCenter);

    //设置右键菜单
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    this->verticalHeader()->setVisible(false);
    this->setShowGrid(true);
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setSelectionMode(QAbstractItemView::ExtendedSelection);
    this->setAlternatingRowColors(true);
    this->setFrameShape(QFrame::NoFrame);

    connect(selectionModel(), &QItemSelectionModel::selectionChanged, [=]() {
        emit selectionRowChanged(selectionModel()->selectedRows().count());
    });
}

void CTableview::initHeader()
{
    QStringList list;
    for (const HeadStruct& headInfo : m_headList) {
        list << headInfo.headText;
    }

    Q_ASSERT(m_pModel);
    m_pModel->setHorizontalHeaderLabels(list);
    for (int i = 0; i < m_headList.size(); ++i) {
        const HeadStruct& head = m_headList.at(i);
        if (head.isHidden) {
            this->setColumnHidden(i, true);
        }
        if (head.columnWidth > 0) {
            this->setColumnWidth(i, head.columnWidth);
        } else {
            this->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
        }
    }
}
