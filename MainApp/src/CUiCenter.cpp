#include "CUiCenter.h"
#include "BRSystem.h"
#include "CApplication.h"
#include "CEditInfoDialog.h"
#include "CSearchLineEdit.h"
#include "CheckBoxDelegate.h"
#include "ComboBoxDelegate.h"
#include "DialogMsg.h"
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
    TaskManager::bindTaskGoing(this, SLOT(onTaskDo(const QString&, const QString&, const QString&)));
    TaskManager::bindErrorCallback(this, SLOT(onTaskRequestError(const ResponData&, NetworkRequestError, const QString&)));
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

    //消息模板
    ui.comboBox_template->addItem(tr("chang phone"), tr("change phone message temp"));
    ui.comboBox_template->addItem(tr("safe mode"), tr("safe mode message temp"));
    ui.comboBox_template->setCurrentIndex(0);
}

void CUiCenter::initHeader()
{
    HeadStruct headNode = { "id", -1, true };
    listHead << headNode;

    //headNode = { tr("password"), -1, true };
    
    headNode = { tr("index"), 30 };
    listHead << headNode;

    headNode = { tr("qq number"), -1 };
    listHead << headNode;

    headNode = { tr("password"), -1 };
    listHead << headNode;

    headNode = { tr("phone number"), -1 };
    listHead << headNode;

    headNode = { tr("status"), -1 };
    listHead << headNode;

    headNode = { tr("task_status"), -1 };
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
    ui.tableView->setNodaTips(tr("drop files here"));
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
    doExport(setting);
}

void CUiCenter::doExport(ExportDataView::ExportSetting setting)
{
    QAbstractItemModel* model = ui.tableView->model();
    int rowCount = model->rowCount();

    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    QStringList list;
    auto defaultFileName = QString("export_") + QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
    list << "*.txt";
    dialog.setNameFilters(list);
    dialog.selectFile(defaultFileName);
    if (QDialog::Accepted != dialog.exec())
        return;
    QString fileName = dialog.selectedFiles()[0];

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        file.close();
        return;
    }
    QTextStream dataStream(&file);
    for (int row = 0; row < rowCount; ++row) {

        QString rowString;
        if (setting.export_account) {
            auto qq = model->data(model->index(row, TableAcocountList::qqNumber), Qt::DisplayRole).toString();
            rowString.append(qq);
        }
        if (setting.export_phone) {
            if (!rowString.isEmpty())
                rowString.append(separator);
            auto phone = model->data(model->index(row, TableAcocountList::phoneNumber), Qt::DisplayRole).toString();
            rowString.append(phone);
        }

        if (setting.export_password) {
            if (!rowString.isEmpty())
                rowString.append(separator);
            auto password = model->data(model->index(row, TableAcocountList::password), Qt::DisplayRole).toString();
            rowString.append(password);
        }

        if (setting.export_role) {
            if (!rowString.isEmpty())
                rowString.append(separator);
            auto status = model->data(model->index(row, TableAcocountList::status), Qt::DisplayRole).toString();
            rowString.append(status);
        }
        dataStream << rowString << "\n";
    }
    file.close();
    qobject_cast<BRSystem*>(CApp->getMainWidget())->printLog(QtInfoMsg, tr("export success"));
}

void CUiCenter::setAddvertiseLink(const QString& link)
{
}

void CUiCenter::openPhoneNumberList()
{
    OperationLog* content = new OperationLog(this);
    PopupDialogContainer::showSecondPopupFadeIn(content, CApp->getMainWidget(), tr("phone number list"));
}

void CUiCenter::PrintLog(QtMsgType type, const QString& text)
{
    qobject_cast<BRSystem*>(CApp->getMainWidget())->printLog(type, text);
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
        data["index"] = QString::number(row);
        data["qq"] = model->data(model->index(row, TableAcocountList::qqNumber), Qt::DisplayRole).toString();
        data["phone"] = model->data(model->index(row, TableAcocountList::phoneNumber), Qt::DisplayRole).toString();
        data["password"] = model->data(model->index(row, TableAcocountList::password), Qt::DisplayRole).toString();
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
          
            //修改状态为执行中
            RequestTask task;
            task.reqeustId = (quint64)this;
            task.index = data["index"].toInt();
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
    InputPwdView view(this);
    if (QDialog::Accepted == view.exec()) {
        view.GetPassword(password);
        return true;
    }
    return false;
}

bool CUiCenter::ShowInputPhone(QString& phone)
{
    InputPhoneView view(this);
    if (QDialog::Accepted == view.exec()) {
        view.GetPhoneNumber(phone);
        return true;
    }
    return false;
}

void CUiCenter::parseLocalTaskData(const QJsonObject& dataObj, int index, const QString& taskId)
{
    if (dataObj.contains("bizType")) {
        int type = dataObj.value("bizType").toInt();
        qInfo("task excute success, task type=%d, task id=%s", type, taskId.toUtf8().constData());
        if (dataObj.contains("show")) {
            QString strShow = dataObj.value("show").toString();
            setListRowData(index, TableAcocountList::status, strShow);
            auto taskType = static_cast<TaskType>(type);
            QString strType = getTaskTypeString(taskType);
            PrintLog(QtInfoMsg, QString("[%1]").arg(strType) + strShow);
        } else {
            PrintLog(QtWarningMsg, tr("parse json error"));
        }
    }
}

void CUiCenter::setListRowData(int rowIndex, int column, const QVariant& data)
{
    QAbstractItemModel* model = ui.tableView->model();
    auto index = model->index(rowIndex, column);
    if (model->index(rowIndex, column).isValid()) {
        model->setData(index, data, Qt::DisplayRole);
        model->setData(index, data, Qt::ToolTipRole);
    }
}

QString CUiCenter::getTaskTypeString(TaskType taskType)
{
    QString taskTypeString;
    switch (taskType) {
    case change_password:
        taskTypeString = tr("modify password");
        break;
    case unpack_safe_mode:
        taskTypeString = tr("unpack safe mode");
        break;
    case bind_mobile:
        taskTypeString = tr("bind phone");
        break;
    case query_role:
        taskTypeString = tr("search role");
        break;
    case query_identity:
        taskTypeString = tr("query_identity");
        break;
    case query_ban:
        taskTypeString = tr("query_ban");
        break;
    case query_credit_score:
        taskTypeString = tr("query_credit_score");
        break;
    default:
        break;
    }
    return taskTypeString;
}

QList<QStandardItem*> CUiCenter::creatRow(const ModelData& model, int index)
{
    QList<QStandardItem*> item;
    item.append(new QStandardItem(model["id"]));


    item.append(new QStandardItem(QString::number(index+1)));
    item.append(new QStandardItem(model["qq"]));
    {
        auto itemPassword = new QStandardItem;
        itemPassword->setData(model["password"], Qt::DisplayRole);
        item.append(itemPassword);
    }
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

    {
        auto itemPassword = new QStandardItem;
        itemPassword->setData(strStatus, Qt::DisplayRole);
        itemPassword->setData(strStatus, Qt::ToolTipRole);
        item.append(itemPassword);
    }

    item.at(0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    item.at(1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    item.at(2)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    item.at(3)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    item.at(4)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    item.at(5)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    return item;
}

//右键菜单
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

        menu->addAction(tr("remove"), [=] {
            remove();
        });

        menu->addAction(tr("remove all"), [=] {
            QAbstractItemModel* model = ui.tableView->model();
            model->removeRows(0, model->rowCount());
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
        PrintLog(QtInfoMsg, tr("fetch list data success!"));
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
            PrintLog(QtInfoMsg, tr("sync phone numbers success!"));
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

void CUiCenter::remove()
{   
    QAbstractItemModel* model = ui.tableView->model();
    QItemSelectionModel* selections = ui.tableView->selectionModel();
    QModelIndexList selected = selections->selectedIndexes();
    QMap<int, int> rows;
    foreach(QModelIndex index, selected)
        rows.insert(index.row(), 0);
    QMapIterator<int, int> r(rows);
    r.toBack();
    while (r.hasPrevious()) {
        r.previous();
        model->removeRow(r.key());
    }
    // 对行号进行重新排序
     int rowCount = model->rowCount();
     for (int i = 0; i < rowCount; i++)
     {
         model->setData(model->index(i,TableAcocountList::index), i+1);
     }
}

//发送短信
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

//绑定手机
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
    // 判重

    QAbstractItemModel* tableModel = ui.tableView->model();
    int rowCount = tableModel->rowCount();
    bool isReply = false;
    for (int row = 0;row < rowCount;row++) {
        QString qq = tableModel->data(tableModel->index(row, TableAcocountList::qqNumber), Qt::DisplayRole).toString();
        if (qq == model["qq"]) {
            isReply = true;
            break;
        }
    }
    if (!isReply){
        listImport.append(model);
        ui.tableView->addData(model);
    }


    /*if (rowCount == 0) {

    }
    else {
        for (int row = 0;row < rowCount;row++) {
            auto qq = tableModel->data(tableModel->index(row, TableAcocountList::qqNumber), Qt::DisplayRole).toString();
            if (qq != data.qq) {
                listImport.append(model);
                ui.tableView->addData(model);
            }
        }
    }*/
    
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


void CUiCenter::onTaskDo(const QString& index,const QString& msg,const QString& status)
{
    qDebug() << "index :" << index;
    qDebug() << "msg :" << msg;
    qDebug() << "status :" << status;
    QAbstractItemModel* model = ui.tableView->selectionModel()->model();
    model->setData(model->index(index.toInt(), TableAcocountList::status), msg);
    model->setData(model->index(index.toInt(), TableAcocountList::task_status),status);
}


void CUiCenter::onTaskRequestCallback(const ResponData& data, const QString& taskId)
{
    if (data.task.reqeustId == 0)
        return;
    if (data.task.reqeustId == (quint64)ui.tableView) {
        QJsonObject dataObj;
        DataParseResult result;
        WebHandler::ParseJsonData(data.dataReturned, dataObj, &result);
        if (result.errorCode == DataParseResult::NoError) {
            parseLocalTaskData(dataObj, data.task.index, taskId);
        }
    }
}

void CUiCenter::onTaskRequestError(const ResponData& data, NetworkRequestError errorType, const QString& errorString)
{
    if (data.task.reqeustId == 0)
        return;
    if (data.task.reqeustId == (quint64)this) {
        PrintLog(QtWarningMsg, errorString.isEmpty() ? tr("Network erorr, error code = %1").arg((int)errorType) : errorString);
    }
}