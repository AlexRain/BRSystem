#include "OperationLog.h"
#include "DialogMsg.h"
#include "ReadOnlyDelegate.h"
#include "UserSession.h"
#include "WebHandler.h"
#include "define.h"
#include <QAbstractItemView>
#include <QGraphicsDropShadowEffect>
#include <QHeaderView>
#include <QLabel>
#include <QPushButton>
#include <QStandardItemModel>

OperationLog::OperationLog(QWidget* parent)
    : BaseWidget(parent)
    , m_pTableView(nullptr)
{
    QMetaObject::connectSlotsByName(this);
    this->resize(698, 435);
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(9, 44, 9, 9);

    QHBoxLayout* layout_btn = new QHBoxLayout;
    layout_btn->setContentsMargins(10, 0, 10, 0);
    layout_btn->setSpacing(10);

    QPushButton* btn_import = new QPushButton(this);
    btn_import->setCursor(Qt::PointingHandCursor);
    btn_import->setObjectName("btn_import_phone");

    QPushButton* btn_export = new QPushButton(this);
    btn_export->setCursor(Qt::PointingHandCursor);
    btn_export->setObjectName("btn_export_phone");

    QPushButton* btn_add = new QPushButton(this);
    btn_add->setCursor(Qt::PointingHandCursor);
    btn_add->setObjectName("btn_add_phone");

    layout_btn->addStretch();
    layout_btn->addWidget(btn_import);
    layout_btn->addWidget(btn_export);
    layout_btn->addWidget(btn_add);

    WebHandler::bindDataCallback(this, SLOT(onRequestCallback(const ResponData&)));
    m_pTableView = new CTableview(this);
    this->initHeader();
    m_pTableView->setItemDelegate(new ReadOnlyDelegate(this));
    m_pTableView->setCreatRowCallbackListener(this);

    layout->addLayout(layout_btn);
    layout->addWidget(m_pTableView);
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
    HeadStruct headNode = { tr("index"), 70, false };
    listHead << headNode;

    headNode = { tr("phone number"), -1, false };
    listHead << headNode;
    m_pTableView->setHeader(listHead);
}

void OperationLog::initData()
{
    RequestTask task;
    task.reqeustId = (quint64)m_pTableView;
    task.headerObj.insert("uid", UserSession::instance().userData().uid);
    task.headerObj.insert("token", UserSession::instance().userData().token);
    task.apiIndex = API::getPhoneList;
    WebHandler::instance()->Get(task);
}

void OperationLog::setData(const QList<ModelData>& datas)
{
    m_pTableView->setData(datas);
}

void OperationLog::resizeEvent(QResizeEvent* event)
{
    BaseWidget::resizeEvent(event);
}

QList<QStandardItem*> OperationLog::creatRow(const ModelData& model, int index)
{
    QList<QStandardItem*> item;
    item.append(new QStandardItem(QString::number(index)));
    item.append(new QStandardItem(model["phone"]));

    //设置列数的对齐方式
    item.at(0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    item.at(1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    return item;
}

void OperationLog::onRequestCallback(const ResponData& data)
{
    if (data.task.reqeustId == 0)
        return;
    if (data.task.reqeustId == (quint64)m_pTableView) {
        QJsonObject dataObj;
        DataParseResult result;
        WebHandler::ParseJsonData(data.dataReturned, dataObj, &result);
        if (result.errorCode == DataParseResult::NoError) {
            QJsonArray array = dataObj.value("list").toArray();
            auto iter = array.constBegin();
            QList<ModelData> listData;
            while (iter != array.constEnd()) {
                QJsonObject obj = (*iter).toObject();
                ModelData itemData;
                itemData["phone"] = obj.value("phone").toString();
                listData.append(itemData);
                ++iter;
            }
            this->setData(listData);
        }
    }
}
