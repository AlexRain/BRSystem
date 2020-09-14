﻿#include "CUiCenter.h"
#include "BRSystem.h"
#include "CApplication.h"
#include "CEditInfoDialog.h"
#include "CSearchLineEdit.h"
#include "CheckBoxDelegate.h"
#include "ComboBoxDelegate.h"
#include "DialogMsg.h"
#include "ExportDataView.h"
#include "FilterBtnDelegate.h"
#include "InputPhoneView.h"
#include "InputPwdView.h"
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
#define ENABLE_DRAG_FILE

CUiCenter::CUiCenter(QWidget* parent)
    : BaseWidget(parent)
{
    ui.setupUi(this);
    WebHandler::bindDataCallback(this, SLOT(onRequestCallback(const ResponData&)));
    TaskManager::bindDataCallback(this, SLOT(onTaskRequestCallback(const ResponData&)));
    TaskManager::bindErrorCallback(this, SLOT(onTaskRequestError(const ResponData&, NetworkRequestError, const QString&)));
    qRegisterMetaType<BorrowInfo>("BorrowInfo");
    qRegisterMetaType<ImportData>("ImportData");
    this->initUi();
    this->initData();
}

CUiCenter::~CUiCenter()
{
    threadImport.quit();
    threadImport.wait();
}

void CUiCenter::initUi()
{
    this->initTableView();

    QPushButton* btn_export = UiHelper::creatPushButton(
        this, [=]() {
            this->exportList();
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

    ui.comboBox_template->addItem(tr("chang phone"), tr("change phone message temp"));
    ui.comboBox_template->addItem(tr("safe mode"), tr("safe mode message temp"));
    ui.comboBox_template->setCurrentIndex(0);
}

void CUiCenter::initHeader()
{
    HeadStruct headNode = { "id", -1, true };
    listHead << headNode;

    headNode = { tr("password"), -1, true };
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
#ifdef ENABLE_DRAG_FILE
    QList<ModelData> listData;
    this->setData(listData);
#else
    RequestTask task;
    task.reqeustId = (quint64)ui.tableView;
    task.headerObj.insert("uid", UserSession::instance().userData().uid);
    task.headerObj.insert("token", UserSession::instance().userData().token);
    task.apiIndex = API::accountList;
    WebHandler::instance()->Post(task);
#endif
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
        this->updateButtonState(selectedCount);
    });

    connect(ui.tableView, &CTableview::dropFiles, this, &CUiCenter::OnDropFiles);

    connect(ui.tableView, &CTableview::currentIndexChanged, [=](const QModelIndex& current, const QModelIndex& previous) {

    });

    connect(&taskImport, &ThreadImport::addRow, this, &CUiCenter::OnAddRow, Qt::QueuedConnection);
    connect(&taskImport, &ThreadImport::taskFinished, this, &CUiCenter::OnImportFinished, Qt::QueuedConnection);

    ui.tableView->setItemDelegate(new ReadOnlyDelegateBRTable(this));
    ui.tableView->setNodaTips(tr("no data"));
    updateButtonState(0);
}

void CUiCenter::setData(const QList<ModelData>& vModel)
{
    uint nLen = vModel.size();
    ui.tableView->setData(vModel);
}

void CUiCenter::doExport(QList<ModelData>& vModel)
{
}

void CUiCenter::updateButtonState(int selectedCount)
{
    bool enabled = (selectedCount > 0);
    ui.groupBox_btn->setEnabled(enabled);
}

void CUiCenter::exportList()
{
    ExportDataView viewExport;
    ExportDataView::ExportSetting setting;
    if (QDialog::Accepted != viewExport.exec())
        return;
    viewExport.GetExportSetting(setting);
    //TODO EXPORT
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
}

void CUiCenter::openPhoneNumberList()
{
    OperationLog* content = new OperationLog(this);
    PopupDialogContainer::showSecondPopupFadeIn(content, CApp->getMainWidget(), tr("phone number list"));
}

void CUiCenter::UpdateStatusText(const QString& text)
{
}

bool CUiCenter::GetCurrentData(QList<ModelData>& selectedRows)
{
    auto selectRows = ui.tableView->selectionModel()->selectedRows();
    int count = selectRows.size();
    if (selectRows.isEmpty()) {
        DialogMsg::question(this, tr("warning"), tr("please select at least one row"), QMessageBox::Ok);
        return false;
    }
    QAbstractItemModel* model = ui.tableView->model();
    for (const auto& rowData : selectRows) {
        ModelData data;
        int row = rowData.row();
        data["qq"] = model->data(model->index(row, TableAcocountList::qqNumber), Qt::DisplayRole).toString();
        data["phone"] = model->data(model->index(row, TableAcocountList::phoneNumber), Qt::DisplayRole).toString();
        data["password"] = model->data(model->index(row, TableAcocountList::password), Qt::UserRole).toString();
        selectedRows.append(data);
    }
    return true;
}

void CUiCenter::excuteTasks(TaskType type)
{
    ModeDataList dataList;
    if (GetCurrentData(dataList)) {
        int bizType = static_cast<int>(type);
        bool isModifyPassword = (change_password == type);
        bool isBindPhone = (bind_mobile == type);
        QString password;
        QString phoneNumber;
        if (isModifyPassword) {
            ShowInputPwdView(password);
        } else if (isBindPhone) {
            ShowInputPhone(phoneNumber);
        }

        qInfo("=========start excute tasks,total count is %d, task type is %d=========", dataList.size(), bizType);
        for (const auto& data : dataList) {
            RequestTask task;
            task.reqeustId = (quint64)this;
            task.headerObj.insert("uid", UserSession::instance().userData().uid);
            task.headerObj.insert("token", UserSession::instance().userData().token);
            task.bodyObj.insert("bizType", bizType);
            QJsonObject objParam;
            objParam.insert("new_password", password);
            objParam.insert("new_phone", phoneNumber);
            objParam.insert("password", data["password"]);
            objParam.insert("phone", data["phone"]);
            objParam.insert("qq", data["qq"]);
            task.bodyObj.insert("params", objParam);
            task.apiIndex = API::addTask;
            TaskManager::instance()->Post(task);
        }
    }
}

bool CUiCenter::ShowInputPwdView(QString& password)
{
    InputPwdView view;
    if (QDialog::Accepted == view.exec()) {
        view.GetPassword(password);
        return true;
    }
    return false;
}

bool CUiCenter::ShowInputPhone(QString& phone)
{
    InputPhoneView view;
    if (QDialog::Accepted == view.exec()) {
        view.GetPhoneNumber(phone);
        return true;
    }
    return false;
}

QList<QStandardItem*> CUiCenter::creatRow(const ModelData& model, int index)
{
    QList<QStandardItem*> item;
    item.append(new QStandardItem(model["id"]));
    auto itemPassword = new QStandardItem;
    itemPassword->setData(model["password"], Qt::UserRole);
    item.append(itemPassword);
    item.append(new QStandardItem(QString::number(index)));
    item.append(new QStandardItem(model["qq"]));
    item.append(new QStandardItem(model["phone"]));

    QString strStatus;
    auto status = (AccountStatus)model["status"].toInt();
    switch (status) {
    case AccountStatus::Normal:
        strStatus = tr("normal");
        break;
    case AccountStatus::SafeMode:
        strStatus = tr("safe mode");
        break;
    case AccountStatus::Forbidden:
        strStatus = tr("account forbidden");
        break;
    default:
        break;
    }
    item.append(new QStandardItem(strStatus));

    item.at(0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    item.at(1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    item.at(2)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    item.at(3)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    item.at(4)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    item.at(5)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

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
        menu->exec(QCursor::pos());
    }
}

void CUiCenter::slotTableViewDoubleClicked(const QModelIndex& index)
{
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
    excuteTasks(change_password);
}

void CUiCenter::on_btn_unsecure_clicked()
{
    excuteTasks(unpack_safe_mode);
}

void CUiCenter::on_btn_account_status_clicked()
{
    excuteTasks(query_ban);
}

void CUiCenter::on_btn_role_clicked()
{
    excuteTasks(query_role);
}

void CUiCenter::on_btn_send_msg_clicked()
{
    if (ui.lineEdit_phone->text().isEmpty() && ui.lineEdit_qq_number->text().isEmpty()) {
        DialogMsg::question(this, tr("tips"), tr("please enter qq account or phone number"), QMessageBox::Ok | QMessageBox::Cancel);
        return;
    }

    RequestTask task;
    task.reqeustId = (quint64)this;
    task.headerObj.insert("uid", UserSession::instance().userData().uid);
    task.headerObj.insert("token", UserSession::instance().userData().token);
    int bizType = static_cast<int>(send_short_message);
    task.bodyObj.insert("bizType", bizType);
    QJsonObject objParam;
    objParam.insert("phone", ui.lineEdit_phone->text());
    objParam.insert("qq", ui.lineEdit_qq_number->text());
    objParam.insert("content", ui.comboBox_template->currentData().toString());
    task.bodyObj.insert("params", objParam);
    task.apiIndex = API::addTask;
    TaskManager::instance()->Post(task);
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
    excuteTasks(bind_mobile);
}

void CUiCenter::OnDropFiles(const QList<QUrl>& listFiles)
{
    importCount = 0;
    listImport.clear();
    if (!threadImport.isRunning()) {
        taskImport.moveToThread(&threadImport);
        threadImport.start();
    }
    for (auto url : listFiles) {
        taskImport.AddTask(url.toLocalFile());
    }
}

void CUiCenter::OnAddRow(ImportData data)
{
    importCount++;
    ModelData model;
    model["qq"] = data.qq;
    model["phone"] = data.phoneNumber;
    model["password"] = data.password;
    model["status"] = QString::number((int)AccountStatus::Normal);
    listImport.append(model);
    ui.tableView->addData(model);
}

void CUiCenter::OnImportFinished()
{
    if (listImport.isEmpty())
        return;
    int result = DialogMsg::question(this, tr("question"), tr("you have imported %1 pieces of data, bind platform for them?").arg(listImport.size()), QMessageBox::Ok | QMessageBox::Cancel);
    if (result == QMessageBox::Ok) {
        RequestTask task;
        task.reqeustId = (quint64)this;
        task.headerObj.insert("uid", UserSession::instance().userData().uid);
        task.headerObj.insert("token", UserSession::instance().userData().token);
        QJsonArray arrayParam;
        for (const auto& data : listImport) {
            if (!data.value("qq").isEmpty() || !data.value("password").isEmpty()) {
                QJsonObject obj;
                obj.insert("password", data.value("password"));
                obj.insert("qq", data.value("qq"));
                obj.insert("phone", data.value("phone"));
                arrayParam.append(obj);
            }
        }
        task.bodyObj.insert("list", arrayParam);
        task.apiIndex = API::bindPlatform;
        WebHandler::instance()->Post(task);
    }
}

void CUiCenter::onTaskRequestCallback(const ResponData& data)
{
    if (data.task.reqeustId == 0)
        return;
    if (data.task.reqeustId == (quint64)ui.tableView) {
    }
}

void CUiCenter::onTaskRequestError(const ResponData& data, NetworkRequestError errorType, const QString& errorString)
{
    if (data.task.reqeustId == 0)
        return;
    if (data.task.reqeustId == (quint64)this) {
        qobject_cast<BRSystem*>(CApp->getMainWidget())->printLog(QtWarningMsg, errorString.isEmpty() ? tr("Network erorr, error code = %1").arg((int)errorType) : errorString);
    }
}
