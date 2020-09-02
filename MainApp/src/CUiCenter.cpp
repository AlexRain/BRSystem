#include "CUiCenter.h"
#include "CApplication.h"
#include "CEditInfoDialog.h"
#include "CSearchLineEdit.h"
#include "CheckBoxDelegate.h"
#include "ComboBoxDelegate.h"
#include "DialogMsg.h"
#include "FilterBtnDelegate.h"
#include "Odbexcel.h"
#include "OperationLog.h"
#include "PopupDialogContainer.h"
#include "ReadOnlyDelegate.h"
#include "SortFilterProxyModel.h"
#include "StyledDelegate.h"
#include "UserSession.h"
#include "WebHandler.h"
#include <QCursor>
#include <QDesktopServices>
#include <QFileDialog>
#include <QGridLayout>
#include <QHeaderView>
#include <QMenu>
#include <QMessageBox>
#include <QSizeGrip>
#include <QSortFilterProxyModel>
#include <QSplitter>
#include <QStandardItem>
#include <QTableView>

QList<HeadStruct> listHead;

CUiCenter::CUiCenter(QWidget* parent)
    : BaseWidget(parent)
{
    ui.setupUi(this);
    WebHandler::bindDataCallback(this, SLOT(onRequestCallback(const ResponData&)));
    qRegisterMetaType<BorrowInfo>("BorrowInfo");
    this->initUi();
    this->initData();
    QMetaObject::connectSlotsByName(this);
}

CUiCenter::~CUiCenter()
{
}

void CUiCenter::initUi()
{
    this->initTableView();
    ui.label_ads->setText(TOCH("<p><a href=\"register\"><span style=\" text - decoration: underline; color:rgb(0,122,204); \">广告位招商，详询qq:123456</span></a></p>"));

    {
        QSplitter* splitter = new QSplitter(Qt::Horizontal, ui.frameTool);
        ui.horizontalLayout_3->addWidget(splitter);
        splitter->addWidget(ui.groupBox);
        splitter->addWidget(ui.groupBox_btn);
        splitter->setStretchFactor(0, 5);
        splitter->setStretchFactor(1, 2);
        splitter->setHandleWidth(5);
        splitter->setCollapsible(0, false);
        splitter->setCollapsible(1, false);
    }

    QPushButton* btn_add = UiHelper::creatPushButton(
        this, [=]() {
            CEditInfoDialog* infoDialog = new CEditInfoDialog(this, false);
            connect(
                infoDialog, &CEditInfoDialog::saveData, this, [=](const BorrowInfo& info) {
                    CDbHelper dbHelper;
                    dbHelper.open();
                    if (dbHelper.isTableExist(DIC_BORROW_RETURN)) {
                        ModelData model;
                        model["productionId"] = info.productionId;
                        model["productionName"] = info.productionName;
                        model["borrowerName"] = info.borrowerName;
                        model["borrowReason"] = info.borrowReason;
                        model["borrowStatus"] = QString::number((int)info.borrowStatus);
                        model["remarks"] = info.remarks;
                        model["createUserId"] = UserSession::instance().userData().userId;
                        int rows = dbHelper.Insert(DIC_BORROW_RETURN, model);
                        if (rows > 0) {
                            this->FuzzyQuery();
                        }
                    } else {
                        dbHelper.exec(CREATE_TABLE_SQL);
                    }
                },
                Qt::DirectConnection);
            PopupDialogContainer::showSecondPopupFadeIn(infoDialog, CApp->getMainWidget(), tr("detail"));
        },
        -1, -1, "", "btn_add");
    btn_add->setToolTip(tr("add"));

    QPushButton* btn_log = UiHelper::creatPushButton(
        this, [=]() {
            OperationLog* content = new OperationLog(this);
            PopupDialogContainer::showSecondPopupFadeIn(content, CApp->getMainWidget(), tr("log"));
        },
        -1, -1, "", "btn_log");
    btn_log->setToolTip(tr("log"));

    QPushButton* btn_export = UiHelper::creatPushButton(
        this, [=]() {
            this->sqlQueryExport();
        },
        -1, -1, "", "btn_export");
    btn_export->setToolTip(tr("export"));

    QPushButton* btn_phone_manage = UiHelper::creatPushButton(
        this, [=]() {
            this->openPhoneNumberList();
        },
        -1, -1, "", "btn_phone_manage");
    btn_phone_manage->setToolTip(tr("phone manage"));

    QPushButton* btn_sync = UiHelper::creatPushButton(
        this, [=]() {
        },
        -1, -1, "", "btn_sync_phone");
    btn_sync->setToolTip(tr("phone sync"));

    ui.layout_left_button->addWidget(btn_phone_manage);
    ui.layout_left_button->addWidget(btn_sync);

    ui.layout_buttons->addWidget(btn_export);
    ui.layout_buttons->addWidget(btn_log);
    ui.layout_buttons->addWidget(btn_add);
}

void CUiCenter::initHeader()
{
    HeadStruct headNode = { "id", -1, true };
    listHead << headNode;

    headNode = { tr("index"), 70 };
    listHead << headNode;

    headNode = { tr("qq number"), -1 };
    listHead << headNode;

    headNode = { tr("phone number"), -1 };
    listHead << headNode;

    headNode = { tr("status"), 70 };
    listHead << headNode;

    ui.tableView->setHeader(listHead);
}

void CUiCenter::initData()
{
    RequestTask task;
    task.reqeustId = (quint64)ui.tableView;
    task.headerObj.insert("uid", UserSession::instance().userData().uid);
    task.headerObj.insert("token", UserSession::instance().userData().token);
    task.apiIndex = API::accountList;
    WebHandler::instance()->Post(task);
}

void CUiCenter::initTableView()
{
    this->initHeader();
    ui.tableView->setCreatRowCallbackListener(this);

    ui.tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui.tableView, SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(slotContextMenu(const QPoint&)));
    connect(ui.tableView, SIGNAL(doubleClicked(const QModelIndex&)),
        this, SLOT(slotTableViewDoubleClicked(const QModelIndex&)));
    connect(ui.tableView, &CTableview::selectionRowChanged, [=](int selectedCount) {
        ui.tableView->setProperty("selectedCount", selectedCount);
        this->updateCountLabel();
    });

    connect(ui.tableView, &CTableview::currentIndexChanged, [=](const QModelIndex& current, const QModelIndex& previous) {
        qInfo("current index changed");
    });

    ui.tableView->setItemDelegate(new ReadOnlyDelegateBRTable(this));
}

void CUiCenter::sqlQuery(const QString& sql)
{
    CDbHelper dbHelper;
    dbHelper.open();
    QList<ModelData> vModel;
    int rows = dbHelper.Queuey(vModel, sql);
    this->setData(vModel);
}

void CUiCenter::sqlUpdate(const ModelData& model, const QString& id)
{
    CDbHelper dbHelper;
    dbHelper.open();
    ModelData modelTemp = const_cast<ModelData&>(model);
    modelTemp["updateUserId"] = UserSession::instance().userData().userId;
    int rows = dbHelper.Update(DIC_BORROW_RETURN, modelTemp, QString(" WHERE ID = %1").arg(id));
}

void CUiCenter::sqlDelete(const BorrowInfo& info)
{
    ModelData model;
    model["deleteFlag"] = "1";
    model["updateUserId"] = UserSession::instance().userData().userId;
    CDbHelper dbHelper;
    dbHelper.open();
    int rows = dbHelper.Update(DIC_BORROW_RETURN, model, QString(" WHERE ID = %1").arg(info.id));
}

void CUiCenter::sqlDelete(const QList<BorrowInfo>& infos)
{
    CDbHelper dbHelper;
    dbHelper.open();
    QString ids = "";
    for (BorrowInfo info : infos) {
        if (ids.isEmpty()) {
            ids = "ID = " + info.id;
        } else
            ids += QString(" OR ") + "ID = " + info.id;
    }
    ModelData model;
    model["deleteFlag"] = "1";
    model["updateUserId"] = UserSession::instance().userData().userId;
    int rows = dbHelper.Update(DIC_BORROW_RETURN, model, QString(" WHERE %1").arg(ids));
}

void CUiCenter::FuzzyQuery(const QString& key)
{
    initData();
}

void CUiCenter::setData(const QList<ModelData>& vModel)
{
    uint nLen = vModel.size();
    this->updateCountLabel();
    ui.tableView->setData(vModel);
}

void CUiCenter::showDetailUi(const QModelIndex& index)
{
    QModelIndex indexFirst = ui.tableView->model()->index(0, (int)TableHeader::Order);
    if (ui.tableView->model()->data(indexFirst, Qt::UserRole).toBool())
        return;

    CEditInfoDialog* infoDialog = new CEditInfoDialog(this);
    connect(
        infoDialog, &CEditInfoDialog::deleteItem, this, [=](const BorrowInfo& info) {
            ModelData model;
            model["deleteFlag"] = "1";
            this->sqlUpdate(model, info.id);
            this->FuzzyQuery();
        },
        Qt::DirectConnection);

    connect(
        infoDialog, &CEditInfoDialog::updateData, this, [=](const BorrowInfo& info) {
            ModelData model;
            model["productionId"] = info.productionId;
            model["productionName"] = info.productionName;
            model["borrowerName"] = info.borrowerName;
            model["borrowStatus"] = QString::number((int)info.borrowStatus);
            model["borrowReason"] = info.borrowReason;
            model["remarks"] = info.remarks;
            this->sqlUpdate(model, info.id);
            this->FuzzyQuery();
        },
        Qt::DirectConnection);
    BorrowInfo info;
    this->getBorrowData(info, index.row());
    infoDialog->setData(info);
    PopupDialogContainer::showSecondPopupFadeIn(infoDialog, CApp->getMainWidget(), tr(""));
}

void CUiCenter::sqlQueryExport()
{
    CDbHelper dbHelper;
    dbHelper.open();
    QList<ModelData> vModel;
    int rows = dbHelper.Queuey(vModel, "SELECT * FROM DIC_BORROW_RETURN  WHERE deleteFlag = '0' ORDER BY updateDate desc");
    if (rows <= 0) {
        DialogMsg::question(this, tr("question"), tr(""), QMessageBox::Ok);
        return;
    }

    this->doExport(vModel);
}

void CUiCenter::doExport(QList<ModelData>& vModel)
{
    QStringList header;
    for (const HeadStruct& info : listHead) {
        if (!info.key.isEmpty()) {
            header << info.headText;
        }
    }

    QList<QStringList> datas;
    for (ModelData& model : vModel) {
        QStringList list;
        for (const HeadStruct& info : listHead) {
            if (!info.key.isEmpty()) {
                list << model[info.key];
            }
        }
        datas << list;
    }

    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    QStringList list;
    list << TOCH("Excel97-2003工作簿(*.xls)");
    dialog.setNameFilters(list);
    dialog.selectFile(tr("借还信息.xls").arg(TOCH("")));
    if (QDialog::Accepted != dialog.exec())
        return;

    QString fileName = dialog.selectedFiles()[0];
    if (Odbexcel::save(fileName, header, datas, "")) {
        QStringList button;
        button << TOCH("立即打开") << TOCH("取消");
        int res = DialogMsg::question(this, TOCH("提示"), TOCH("保存成功！<br>已保存到：<span style='color:rgb(0,122,204)'>%1</span>").arg(fileName), QMessageBox::Ok | QMessageBox::Cancel, button);
        if (res == QMessageBox::Ok) {
            QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));
        }
    } else {
        QString msgError = TOCH("保存失败，") + Odbexcel::getError();
        DialogMsg::question(this, TOCH("警告"), msgError, QMessageBox::Ok);
    }
}

void CUiCenter::updateCountLabel()
{
}

void CUiCenter::getBorrowData(BorrowInfo& info, int row)
{
    QAbstractItemModel* model = ui.tableView->model();
    info.id = model->data(model->index(row, TableHeader::UniqueId), Qt::DisplayRole).toString();
    info.productionId = model->data(model->index(row, TableHeader::ProductionId), Qt::DisplayRole).toString();
    info.productionName = model->data(model->index(row, TableHeader::ProductionName), Qt::DisplayRole).toString();
    info.borrowerName = model->data(model->index(row, TableHeader::BorrowerName), Qt::DisplayRole).toString();
    info.borrowDate = model->data(model->index(row, TableHeader::BorrowDate), Qt::UserRole).toDateTime();
    info.borrowStatus = (BorrowStatus)model->data(model->index(row, TableHeader::Status), Qt::UserRole).toInt();
    info.borrowReason = model->data(model->index(row, TableHeader::BorrowReason), Qt::DisplayRole).toString();
    info.remarks = model->data(model->index(row, TableHeader::Remark), Qt::DisplayRole).toString();
    info.updateDate = model->data(model->index(row, TableHeader::UpdateDate), Qt::UserRole).toDateTime();
}

void CUiCenter::getBorrowData(ModelData& info, int row)
{
    QAbstractItemModel* model = ui.tableView->model();
    info["id"] = model->data(model->index(row, TableHeader::UniqueId), Qt::DisplayRole).toString();
    info["productionId"] = model->data(model->index(row, TableHeader::ProductionId), Qt::DisplayRole).toString();
    info["productionName"] = model->data(model->index(row, TableHeader::ProductionName), Qt::DisplayRole).toString();
    info["borrowerName"] = model->data(model->index(row, TableHeader::BorrowerName), Qt::DisplayRole).toString();
    info["borrowDate"] = model->data(model->index(row, TableHeader::BorrowDate), Qt::UserRole).toDateTime().toString("yyyy-MM-dd hh:mm");
    info["borrowStatus"] = (BorrowStatus)model->data(model->index(row, TableHeader::Status), Qt::UserRole).toInt();
    info["borrowReason"] = model->data(model->index(row, TableHeader::BorrowReason), Qt::DisplayRole).toString();
    info["remarks"] = model->data(model->index(row, TableHeader::Remark), Qt::DisplayRole).toString();
    info["updateDate"] = model->data(model->index(row, TableHeader::UpdateDate), Qt::UserRole).toDateTime().toString("yyyy-MM-dd hh:mm");
}

void CUiCenter::setBorrowData(const BorrowInfo& info, int row)
{
    QAbstractItemModel* model = ui.tableView->model();
    model->setData(model->index(row, TableHeader::UniqueId), info.id, Qt::DisplayRole);
    model->setData(model->index(row, TableHeader::ProductionId), info.productionId, Qt::DisplayRole);
    model->setData(model->index(row, TableHeader::ProductionName), info.productionName, Qt::DisplayRole);
    model->setData(model->index(row, TableHeader::BorrowerName), info.borrowerName, Qt::DisplayRole);
    model->setData(model->index(row, TableHeader::BorrowDate), info.borrowDate, Qt::UserRole);
    model->setData(model->index(row, TableHeader::Status), (int)info.borrowStatus, Qt::UserRole);
    model->setData(model->index(row, TableHeader::BorrowReason), info.borrowReason, Qt::DisplayRole);
    model->setData(model->index(row, TableHeader::Remark), info.remarks, Qt::DisplayRole);
}

void CUiCenter::setAddvertiseLink(const QString& link)
{
    ui.label_ads->setText(link);
}

void CUiCenter::openPhoneNumberList()
{
    OperationLog* content = new OperationLog(this);
    PopupDialogContainer::showSecondPopupFadeIn(content, CApp->getMainWidget(), tr("phone number list"));
}

void CUiCenter::UpdateStatusText(const QString& text)
{
    ui.label_status->setText(text);
}

bool CUiCenter::GetCurrentData(ModelData& data)
{
    QModelIndex currentIndex = ui.tableView->currentIndex();
    if (!currentIndex.isValid()) {
        DialogMsg::question(this, tr("warning"), tr("network error occured"), QMessageBox::Ok);
        return false;
    }
    QAbstractItemModel* model = ui.tableView->model();
    int row = currentIndex.row();
    data["qq"] = model->data(model->index(row, TableAcocountList::qqNumber), Qt::DisplayRole).toString();
    data["phone"] = model->data(model->index(row, TableAcocountList::phoneNumber), Qt::DisplayRole).toString();
    data["status"] = model->data(model->index(row, TableAcocountList::status), Qt::DisplayRole).toString();
    return true;
}

QList<QStandardItem*> CUiCenter::creatRow(const ModelData& model, int index)
{
    QList<QStandardItem*> item;
    item.append(new QStandardItem(model["id"]));
    item.append(new QStandardItem(QString::number(index)));
    item.append(new QStandardItem(model["qq"]));
    item.append(new QStandardItem(model["phone"]));
    item.append(new QStandardItem(model["status"]));

    item.at(0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    item.at(1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    item.at(2)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    item.at(3)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    item.at(4)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    return item;
}

void CUiCenter::slotContextMenu(const QPoint& pos)
{
    QModelIndex index = ui.tableView->indexAt(pos);
    if (index.isValid()) {
        QMenu* menu = new QMenu(ui.tableView);

        QAbstractItemModel* model = ui.tableView->model();
        QModelIndex curentIndex = ui.tableView->currentIndex();
        menu->addAction(tr("modify password"), [=] {
            on_bt_modify_pwd_clicked();
        });

        menu->addAction(tr("unsecure mode"), [=] {
            on_btn_unsecure_clicked();
        });

        menu->addAction(tr("search account status"), [=] {
            on_btn_account_status_clicked();
        });

        menu->addAction(tr("search role"), [=] {
            on_btn_role_clicked();
        });

        menu->addAction(tr("add account"), [=] {
            on_btn_add_account_clicked();
        });
        menu->exec(QCursor::pos());
    }
}

void CUiCenter::slotTableViewDoubleClicked(const QModelIndex& index)
{
    return;
    this->showDetailUi(index);
}

void CUiCenter::onRequestCallback(const ResponData& data)
{
    if (data.task.reqeustId == 0)
        return;
    if (data.task.reqeustId == (quint64)ui.tableView) {
        this->UpdateStatusText(tr("fetch list data success!"));
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
                itemData["id"] = QString::number(obj.value("id").toInt());
                itemData["qq"] = obj.value("qq").toString();
                itemData["phone"] = obj.value("phone").toString();
                itemData["status"] = QString::number(obj.value("status").toInt());
                listData.append(itemData);
                ++iter;
            }
            this->setData(listData);
        }
    } else if (data.task.reqeustId == (quint64)this) {
        if (data.task.apiIndex == API::SyncPhone) {
            this->UpdateStatusText(tr("sync phone numbers success!"));
            initData();
        }
    }
}

void CUiCenter::on_bt_modify_pwd_clicked()
{
    ModelData data;
    if (GetCurrentData(data)) {
    }
}

void CUiCenter::on_btn_unsecure_clicked()
{
    ModelData data;
    if (GetCurrentData(data)) {
    }
}

void CUiCenter::on_btn_account_status_clicked()
{
    ModelData data;
    if (GetCurrentData(data)) {
    }
}

void CUiCenter::on_btn_role_clicked()
{
    ModelData data;
    if (GetCurrentData(data)) {
    }
}

void CUiCenter::on_btn_add_account_clicked()
{
    ModelData data;
    if (GetCurrentData(data)) {
    }
}

void CUiCenter::on_btn_send_msg_clicked()
{
    ModelData data;
    if (GetCurrentData(data)) {
    }
}

void CUiCenter::on_btn_sync_phone_clicked()
{
    RequestTask task;
    task.reqeustId = (quint64)this;
    task.headerObj.insert("uid", UserSession::instance().userData().uid);
    task.headerObj.insert("token", UserSession::instance().userData().token);
    task.apiIndex = API::SyncPhone;
    WebHandler::instance()->Get(task);
}

void CUiCenter::on_btn_bind_phone_clicked()
{
    ModelData data;
    if (GetCurrentData(data)) {
        RequestTask task;
        task.reqeustId = (quint64)this;
        task.headerObj.insert("uid", UserSession::instance().userData().uid);
        task.headerObj.insert("token", UserSession::instance().userData().token);
        task.apiIndex = API::bindPhone;
        WebHandler::instance()->Post(task);
    }
}
