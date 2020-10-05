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
#include <WebSocketClientManager.h>

static const char* LAST_FILE_PATH = "last_file_path";
QList<HeadStruct> listHead;
#define ENABLE_DRAG_FILE

bool hasWork = false;
bool canOp = true;
bool importFinished = true;
bool btnEnabled = true;
bool isPause = false;
int queueSize = 0;

CUiCenter::CUiCenter(QWidget* parent)
    : QWidget(parent)
{

    ui.setupUi(this);
    qRegisterMetaType<ImportData>("ImportData");
    qRegisterMetaType<ResponData>("ResponData");

    connect(parent, SIGNAL(doImportLastFile()), this, SLOT(importLastFile()));
    WebHandler::bindDataCallback(this, SLOT(onRequestCallback(const ResponData&)));
    TaskManager::bindDataCallback(this, SLOT(onTaskRequestCallback(const ResponData&, const QString&)));
    TaskManager::bindTaskGoing(this, SLOT(onTaskDo(const int, const QString, const int, const int)));
    TaskManager::bindErrorCallback(this, SLOT(onTaskRequestError(const ResponData&, NetworkRequestError, const QString&)));
    TaskManager::bindQuqueSize(this, SLOT(onQueueSize(const int)));
    TaskManager::bindTaskPause(this, SIGNAL(onTaskPause(const bool&)));
    TaskManager::bindTaskReStart(this, SIGNAL(onRestartTask()));
    
    wsClient = new WebSocketClientManager();
    connect(wsClient, SIGNAL(showMsg(const int, const QString, const QString)), this, SLOT(showPyMsg(const int, const QString, const QString)));
    wsClient->startConnect();

    this->getUserWallet();
    this->initUi();
    this->initData();

}



CUiCenter::~CUiCenter()
{
    threadImport.quit();
    threadImport.wait();
}


//初始化ui
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

    //单个处理
    QMetaEnum metaEnum = QMetaEnum::fromType<CUiCenter::TaskType2>();

    ui.comboBox_single->addItem(QString::fromLocal8Bit("请选择操作"), 0);
    //qDebug()<<"enum count is "<< metaEnum.keyCount();
    for (int i = 0; i < metaEnum.keyCount(); i++) {
        //qDebug() << "enum value is " << metaEnum.value(i);
        auto taskTypeInt = metaEnum.value(i);
        QString taskString = getTaskTypeString((TaskType)taskTypeInt);
        ui.comboBox_single->addItem(taskString, taskTypeInt);
    }

    ui.btn_single_button->setText(QString::fromLocal8Bit("单个处理"));

    ui.label_single_qq->setText(QString::fromLocal8Bit("QQ"));
    ui.groupBox_2->setTitle(QString::fromLocal8Bit("单个处理"));
}


//初始化表头
void CUiCenter::initHeader()
{
    HeadStruct headNode = { "id", -1, true };
    listHead << headNode;

    headNode = { tr("index"), 30 };
    listHead << headNode;

    headNode = { tr("qq number"), 120 };
    listHead << headNode;

    headNode = { tr("password"), 120, true };
    listHead << headNode;

    headNode = { tr("phone number"), 120 };
    listHead << headNode;

    headNode = { tr("new phone number"), 120 };
    listHead << headNode;

    headNode = { tr("task_status"), 60,true };
    listHead << headNode;

    headNode = { tr("bizType"), 60,true };
    listHead << headNode;
    
    headNode = { tr("status") };
    listHead << headNode;



    ui.tableView->setHeader(listHead);
}

//渲染表格
QList<QStandardItem*> CUiCenter::creatRow(const ModelData& model, int index)
{
    QList<QStandardItem*> item;
    item.append(new QStandardItem(model["id"]));

    item.append(new QStandardItem(QString::number(index + 1)));
    item.append(new QStandardItem(model["qq"]));

    {
        auto itemPassword = new QStandardItem;
        itemPassword->setData(model["password"], Qt::UserRole);
        item.append(itemPassword);
    }

    item.append(new QStandardItem(model["phone"]));

    item.append(new QStandardItem(model["new_phone"]));


    auto taskStatus = (TaskStatus)model["task_status"].toInt();
    {
        auto itemStatus = new QStandardItem;
        itemStatus->setData(taskStatus, Qt::DisplayRole);
        itemStatus->setData(getTaskStatusString(taskStatus), Qt::ToolTipRole);
        item.append(itemStatus);
    }


    auto bizType = (TaskType)model["bizType"].toInt();
    {
        auto itemStatus = new QStandardItem;
        itemStatus->setData(bizType, Qt::DisplayRole);
        itemStatus->setData(getTaskTypeString(bizType), Qt::ToolTipRole);
        item.append(itemStatus);
    }

    {
        auto itemStatus = new QStandardItem;
        itemStatus->setData(model["status"], Qt::DisplayRole);
        itemStatus->setData(model["status"], Qt::ToolTipRole);
        item.append(itemStatus);
    }

    item.at(0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    item.at(1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    item.at(2)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    item.at(3)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    item.at(4)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    item.at(5)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    item.at(6)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    item.at(7)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    return item;
}

//初始化数据
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

//表格初始化
void CUiCenter::initTableView()
{
    this->initHeader();

    ui.tableView->setCreatRowCallbackListener(this);

    ui.tableView->setContextMenuPolicy(Qt::CustomContextMenu);

    //右键菜单
    connect(ui.tableView, SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(slotContextMenu(const QPoint&)));

    //双击
    connect(ui.tableView, SIGNAL(doubleClicked(const QModelIndex&)),
        this, SLOT(slotTableViewDoubleClicked(const QModelIndex&)));

    //选中事件
    connect(ui.tableView, &CTableview::selectionRowChanged, [=](int selectedCount) {
        this->updateButtonState(selectedCount);
    });

    //拖拽文件
    connect(ui.tableView, &CTableview::dropFiles, this, &CUiCenter::OnDropFiles);

    //
    connect(ui.tableView, &CTableview::currentIndexChanged, [=](const QModelIndex& current, const QModelIndex& previous) {});

    //添加纪录
    connect(&taskImport, &ThreadImport::addRow, this, &CUiCenter::OnAddRow, Qt::QueuedConnection);
    
    //导入任务完成
    connect(&taskImport, &ThreadImport::taskFinished, this, &CUiCenter::OnImportFinished, Qt::QueuedConnection);

    ui.tableView->setItemDelegate(new ReadOnlyDelegateBRTable(this));
    ui.tableView->setNodaTips(tr("drop files here"));
    updateButtonState(0);

    // get last file
}

//设置表格数据
void CUiCenter::setData(const QList<ModelData>& vModel)
{
    uint nLen = vModel.size();
    ui.tableView->setData(vModel);
}

void CUiCenter::doExport(QList<ModelData>& vModel)
{
}

//操作按钮状态
void CUiCenter::updateButtonState(int selectedCount)
{
    btnEnabled = (selectedCount > 0);
    //判断是否有进行中的任务
    if (btnEnabled) {
        ModeDataList dataList;
        GetCurrentData(dataList);
    }
    ui.groupBox_btn->setEnabled(btnEnabled);
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

//导出
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
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
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

        if (setting.export_password) {
            if (!rowString.isEmpty())
                rowString.append(separator);
            auto password = model->data(model->index(row, TableAcocountList::password), Qt::UserRole).toString();
            rowString.append(password);
        }

        if (setting.export_phone) {
            if (!rowString.isEmpty())
                rowString.append(separator);
            auto phone = model->data(model->index(row, TableAcocountList::phoneNumber), Qt::DisplayRole).toString();
            rowString.append(phone);
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

//日志
void CUiCenter::PrintLog(QtMsgType type, const QString& text)
{
    qobject_cast<BRSystem*>(CApp->getMainWidget())->printLog(type, text);
}

//当前选中的数据列表
bool CUiCenter::GetCurrentData(QList<ModelData>& selectedRows)
{
    auto selectRows = ui.tableView->selectionModel()->selectedRows();
    int count = selectRows.size();
    if (selectRows.isEmpty()) {
        DialogMsg::question(this, tr("warning"), tr("please select at least one row"), QMessageBox::Ok);
        return false;
    }
    QAbstractItemModel* model = ui.tableView->model();

    canOp = true;
    bool isCanOp = true;
    for (const auto& rowData : selectRows) {
        int row = rowData.row();
        ModelData data=getRowData(model, row);
        qDebug() << "model data is " << data;
        if (data["task_status"].toInt() <= TaskStatus::Stop) {
            isCanOp = false;
        }
        selectedRows.append(data);
    }
    canOp = isCanOp;
    return true;
}

//获取单条记录
ModelData CUiCenter::getRowData(QAbstractItemModel* model,int row) {
    ModelData data;

    data["index"] = QString::number(row);

    int bizType = model->data(model->index(row, TableAcocountList::bizType), Qt::DisplayRole).toInt();
    data["bizType"] = QString::number(bizType);

    data["qq"] = model->data(model->index(row, TableAcocountList::qqNumber), Qt::DisplayRole).toString();

    //选中的行有任务在执行等待或者暂停，不能执行其他任务
    int taskStatus = model->data(model->index(row, TableAcocountList::task_status), Qt::DisplayRole).toInt();
    data["task_status"] = QString::number(taskStatus);

    data["phone"] = model->data(model->index(row, TableAcocountList::phoneNumber), Qt::DisplayRole).toString();

    data["password"] = model->data(model->index(row, TableAcocountList::password), Qt::UserRole).toString();

    data["new_phone"] = model->data(model->index(row, TableAcocountList::newPhoneNumber), Qt::DisplayRole).toString();
    
    return data;
}

//添加任务进队
void CUiCenter::excuteSingleTasks(ModelData data)
{
   
        int bizType = QString(data["bizType"]).toInt();
        bool isModifyPassword = (change_password == bizType);
        bool isBindPhone = (bind_mobile == bizType);
        QString password;
        QString phoneNumber;
        if (isModifyPassword) {
            ShowInputPwdView(password);
        }
        //else if (isBindPhone) {
        //    ShowInputPhone(phoneNumber);
        //}

        //禁用按钮
        ui.groupBox_btn->setEnabled(false);

		RequestTask task;
		task.reqeustId = (quint64)this;
		task.bizType = bizType;
		task.index = data["index"].toInt();
		task.headerObj.insert("uid", UserSession::instance().userData().uid);
		task.headerObj.insert("token", UserSession::instance().userData().token);
		task.bodyObj.insert("bizType", bizType);

		QJsonObject objParam;
		objParam.insert("new_password", password);
		objParam.insert("new_phone", data["new_phone"]);
		objParam.insert("password", data["password"]);
		objParam.insert("phone", data["phone"]);
		objParam.insert("qq", data["qq"]);
		task.bodyObj.insert("params", objParam);
		task.apiIndex = API::addTask;

		//修改状态为等待执行中
		onTaskDo(task.index, QString::fromLocal8Bit("等待执行"), TaskStatus::Wait, task.bizType);
		TaskManager::instance()->Post(task);
}

//添加任务进队
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
        }

        //禁用按钮
        ui.groupBox_btn->setEnabled(false);

        qInfo("=========start excute tasks,total count is %d, task type is %d=========", dataList.size(), bizType);
        for (const auto& data : dataList) {
            doTask(data, password, bizType);
        }
    }
}

void CUiCenter::doTask(ModelData data,QString newPassword="",int bizType=0) {
    RequestTask task;
    task.reqeustId = (quint64)this;
    task.bizType = bizType;
    task.index = data["index"].toInt();
    task.headerObj.insert("uid", UserSession::instance().userData().uid);
    task.headerObj.insert("token", UserSession::instance().userData().token);
    task.bodyObj.insert("bizType", bizType);

    QJsonObject objParam;
    objParam.insert("new_password", newPassword);
    objParam.insert("new_phone", data["new_phone"]);
    objParam.insert("password", data["password"]);
    objParam.insert("phone", data["phone"]);
    objParam.insert("qq", data["qq"]);
    task.bodyObj.insert("params", objParam);
    task.apiIndex = API::addTask;

    //修改状态为等待执行中
    onTaskDo(task.index, QString::fromLocal8Bit("等待执行"), TaskStatus::Wait, task.bizType);
    TaskManager::instance()->Post(task);
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

//返回数据回显
void CUiCenter::parseLocalTaskData(const QJsonObject& dataObj, int index, const QString& taskId)
{
    if (dataObj.contains("bizType")) {
        int type = dataObj.value("bizType").toInt();
        qInfo("task excute success, task type=%d, task id=%s", type, taskId.toUtf8().constData());
        if (dataObj.contains("show")) {
            QString strShow = dataObj.value("show").toString();
            //任务执行成功回显
            onTaskDo(index, strShow, TaskStatus::Success, type);
            auto taskType = static_cast<TaskType>(type);
            QString strType = getTaskTypeString(taskType);
            //打印日志
            PrintLog(QtInfoMsg, "task:" + QString("[%1]").arg(strType) + strShow);
        } else {
            onTaskDo(index, tr("parse json error"), TaskStatus::Error, type);
            PrintLog(QtWarningMsg, tr("parse json error"));
        }
    }
}

//修改表格数据
void CUiCenter::setListRowData(int rowIndex, int column, const QVariant& data)
{
    QAbstractItemModel* model = ui.tableView->model();
    auto index = model->index(rowIndex, column);
    if (model->index(rowIndex, column).isValid()) {
        model->setData(index, data, Qt::DisplayRole);
        model->setData(index, data, Qt::ToolTipRole);
    }
}

//右键菜单
void CUiCenter::slotContextMenu(const QPoint& pos)
{
    QModelIndex index = ui.tableView->indexAt(pos);
    if (index.isValid()) {
        ModeDataList dataList;
        GetCurrentData(dataList);

         menu = new QMenu(ui.tableView);
        QAbstractItemModel* model = ui.tableView->model();
        QModelIndex curentIndex = ui.tableView->currentIndex();

        if (!hasWork) {
            addTableMenuActionSolt(menu, QString::fromLocal8Bit("重做"), SLOT(resetTask()), false);
            addTableMenuActionSolt(menu, QString::fromLocal8Bit("失败重做"), SLOT(resetAllTask()), false);
        }

        addTableMenuActionSolt(menu, QString::fromLocal8Bit("停止"), SLOT(pauseTask()), false);
        addTableMenuActionSolt(menu, QString::fromLocal8Bit("继续"), SLOT(restartTask()), false);
        addTableMenuActionSolt(menu, tr("modify_pwd"), SLOT(on_bt_modify_pwd_clicked()), !canOp);
        addTableMenuActionSolt(menu, tr("release_safe_mode"), SLOT(on_btn_release_safe_model_clicked()), !canOp);
        addTableMenuActionSolt(menu, tr("query_ban"), SLOT(on_btn_query_ban_clicked()), !canOp);
        addTableMenuActionSolt(menu, tr("query_role"), SLOT(on_btn_role_clicked()), !canOp);
        addTableMenuActionSolt(menu, tr("query_score"), SLOT(on_btn_query_score_clicked()), !canOp);
        addTableMenuActionSolt(menu, tr("query_identity"), SLOT(on_btn_query_identity_clicked()), !canOp);
        addTableMenuActionSolt(menu, tr("bind"), SLOT(bindPlatform()));
        addTableMenuActionSolt(menu, tr("bindAll"), SLOT(bindPlatformAll()));

        bool canRemove = !hasWork;
        if (!importFinished) {
            canRemove = false;
        }
        addTableMenuActionSolt(menu, tr("remove"), SLOT(remove()), !canRemove);
        addTableMenuActionSolt(menu, tr("removeAll"), SLOT(removeAll()), !canRemove);
        menu->exec(QCursor::pos());
    }

}

//表格右键菜单
void  CUiCenter::addTableMenuActionSolt(QMenu *menu, QString text,const char* method,bool disabled) {
    QAction* menuAction = new QAction(ui.tableView);
    connect(menuAction, SIGNAL(triggered()), this, method);
    menuAction->setText(text);
    menuAction->setDisabled(disabled);
    menu->addAction(menuAction);
}

void CUiCenter::slotTableViewDoubleClicked(const QModelIndex& index)
{
}

//任务成功返回
void CUiCenter::onRequestCallback(const ResponData& data)
{
    ui.groupBox_btn->setEnabled(true);
    if (data.task.reqeustId == 0)
        return;
    if (data.task.apiIndex == API::getWalllet) {
        QJsonObject dataObj;
        DataParseResult result;
        WebHandler::ParseJsonData(data.dataReturned, dataObj, &result);
        if (result.errorCode == DataParseResult::NoError) {
            ui.wallet_label->setText(tr("balance : %1").arg(dataObj.value("balance").toDouble()));
        } 
        return;
    }
    if (data.task.reqeustId == (quint64)ui.tableView) {
        hasWork = false;
        PrintLog(QtInfoMsg, tr("fetch list data success!"));
        QJsonObject dataObj;
        DataParseResult result;
        WebHandler::ParseJsonData(data.dataReturned, dataObj, &result);
        if (result.errorCode == DataParseResult::NoError) {
   /*         QJsonArray array = dataObj.value("list").toArray();
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
            this->setData(listData);*/
        } else {
            onTaskDo(data.task.index, result.message, TaskStatus::Error,data.task.bizType);
        }
    } else if (data.task.reqeustId == (quint64)this) {
        if (data.task.apiIndex == API::SyncPhone) {
            PrintLog(QtInfoMsg, tr("sync phone numbers success!"));
            initData();
        }
    }
}

//重做全部任务
void CUiCenter::resetAllTask() {
    QAbstractItemModel* tableModel = ui.tableView->model();
    int rowCount = tableModel->rowCount();
    for (int row = 0; row < rowCount; row++) {
        ModelData data = getRowData(tableModel, row);
        //如果任务成功跳过，如果失败重做
        int task_status = data["task_status"].toInt();
        int taskType = data["bizType"].toInt();
        if (taskType > 0 && task_status == TaskStatus::Error) {
            emit onTaskDo(data["index"].toInt(), QString::fromLocal8Bit("失败重做"), TaskStatus::Wait, taskType);
            doTask(data);
        }
    }
}


//重做任务
void CUiCenter::resetTask() {
    ModeDataList dataList;
    GetCurrentData(dataList);
    for (const auto& data : dataList) {
        //如果任务成功跳过，如果失败重做
        int task_status = data["task_status"].toInt();
        int taskType = data["bizType"].toInt();
        if (taskType > 0 && task_status == TaskStatus::Error) {
            emit onTaskDo(data["index"].toInt(), QString::fromLocal8Bit("失败重做"), TaskStatus::Wait, taskType);
            doTask(data);
        }
    }
}

//暂停任务
void CUiCenter::pauseTask() {
    if (queueSize <=0) {
        return;
    }
    qDebug() << "pause task";
    isPause = true;
    emit  onTaskPause(true);
    //所有任务停止
    QAbstractItemModel* tableModel = ui.tableView->model();
    int rowCount = tableModel->rowCount();
    bool isReply = false;
    for (int row = 0; row < rowCount; row++) {
        int taskStatus = tableModel->data(tableModel->index(row, TableAcocountList::task_status), Qt::DisplayRole).toInt();
        if (taskStatus==TaskStatus::Wait) {
            onTaskDo(row, QString::fromLocal8Bit("停止"), TaskStatus::Stop,-1);
        }
    }
}


void CUiCenter::restartTask() {
    isPause = false;
    emit onRestartTask();
}

//任务管理
void CUiCenter::onQueueSize(const int size) {
    //qDebug() << "queue size is " << size;
    queueSize = size;
    hasWork = (size > 0);
}

//单个处理
void CUiCenter::on_btn_single_button_clicked() {
    // 添加一行row
    ImportData data;
    data.qq= ui.lineEdit_single_qq->text();
    if (data.qq.isEmpty()) {
        return;
    }
    data.bizType= ui.comboBox_single->currentIndex();
    if (data.bizType <= 0) {
        return;
    }

    AddSignleRow(data);
}

void CUiCenter::on_bt_modify_pwd_clicked()
{
    excuteTasks(change_password);
}

void CUiCenter::on_btn_release_safe_model_clicked()
{
    excuteTasks(unpack_safe_mode);
}

void CUiCenter::on_btn_query_ban_clicked()
{
    excuteTasks(query_ban);
}

void CUiCenter::on_btn_role_clicked()
{
    excuteTasks(query_role);
}

void CUiCenter::on_btn_query_score_clicked()
{
    excuteTasks(query_credit_score);
}

void CUiCenter::on_btn_query_identity_clicked()
{
    excuteTasks(query_identity);
}

//移除
void CUiCenter::removeAll() {
    QAbstractItemModel* model = ui.tableView->model();
    model->removeRows(0, model->rowCount());
}

//移除
void CUiCenter::remove()
{
    QAbstractItemModel* model = ui.tableView->model();
    QItemSelectionModel* selections = ui.tableView->selectionModel();
    QModelIndexList selected = selections->selectedIndexes();
    QMap<int, int> rows;
    foreach (QModelIndex index, selected)
        rows.insert(index.row(), 0);
    QMapIterator<int, int> r(rows);
    r.toBack();
    while (r.hasPrevious()) {
        r.previous();
        model->removeRow(r.key());
    }
    // 对行号进行重新排序
    int rowCount = model->rowCount();
    for (int i = 0; i < rowCount; i++) {
        model->setData(model->index(i, TableAcocountList::index), i + 1);
    }
}

//发送短信
void CUiCenter::on_btn_send_msg_clicked()
{
    QString fromPhone = ui.lineEdit_phone->text();
    if (fromPhone.isEmpty()) {
        DialogMsg::question(this, tr("tips"), tr("please enter qq account or phone number"), QMessageBox::Ok | QMessageBox::Cancel);
        return;
    }
    QString toPhone = ui.lineEdit_qq_number->text();
    if (toPhone.isEmpty()) {
        toPhone = ui.lineEdit_qq_number->placeholderText();
    }

    if (ui.lineEdit_phone->text().isEmpty() && ui.lineEdit_qq_number->text().isEmpty()) {
        DialogMsg::question(this, tr("tips"), tr("please enter qq account or phone number"), QMessageBox::Ok | QMessageBox::Cancel);
        return;
    }
    QString content;
    content = ui.comboBox_template->currentData().toString();
    QString sms = ui.lineEdit_sms->text();
    if (!sms.isEmpty()) {
        content = sms;
    }

    RequestTask task;
    task.reqeustId = (quint64)this;
    task.headerObj.insert("uid", UserSession::instance().userData().uid);
    task.headerObj.insert("token", UserSession::instance().userData().token);
    int bizType = static_cast<int>(send_short_message);
    task.bodyObj.insert("bizType", bizType);
    QJsonObject objParam;
    objParam.insert("phone", fromPhone);
    objParam.insert("new_phone", toPhone);
    objParam.insert("content", content);
    task.bodyObj.insert("params", objParam);
    task.apiIndex = API::addTask;
    TaskManager::instance()->Post(task);
    getUserWallet();
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

//导入上次的文件
void CUiCenter::importLastFile() {
    //QSettings setting(GetAppDataLocation() + QDir::separator() + CONFIG_FILE, QSettings::IniFormat);
    QSettings setting(CONFIG_FILE, QSettings::IniFormat);
    QString lastExportFile = setting.value(LAST_FILE_PATH).toString();
    if (lastExportFile != "") {
        QUrl u(lastExportFile);
        QList<QUrl> list;
        list.append(u);
        this->OnDropFiles(list);
    }
}

//拖拽槽
void CUiCenter::OnDropFiles(const QList<QUrl>& listFiles)
{

    listImport.clear();
    if (!threadImport.isRunning()) {
        taskImport.moveToThread(&threadImport);
        threadImport.start();
    }
    //QSettings setting(GetAppDataLocation() + QDir::separator() + CONFIG_FILE, QSettings::IniFormat);
    QSettings setting(CONFIG_FILE, QSettings::IniFormat);
    for (auto url : listFiles) {
        setting.setValue(LAST_FILE_PATH, url.toDisplayString());
        taskImport.AddTask(url.toLocalFile());
        importFinished = false;
    }
}

//添加数据
void CUiCenter::OnAddRow(ImportData data)
{
    importCount++;
    ModelData model;
    model["qq"] = data.qq;
    if (data.phoneNumber.contains(QRegExp("[\\x4e00-\\x9fa5]+"))) {
        data.phoneNumber = "";
    }
    model["phone"] = data.phoneNumber;

    if (data.password.contains(QRegExp("[\\x4e00-\\x9fa5]+"))) {
        data.password = "";
    }

    model["password"] = data.password;

    if (data.newPhoneNumber.contains(QRegExp("[\\x4e00-\\x9fa5]+"))) {
        data.newPhoneNumber = "";
    }
    model["new_phone"] = data.newPhoneNumber;

    model["status"] = "";

    model["task_status"] = QString::number(TaskStatus::None);

    // 判重
    QAbstractItemModel* tableModel = ui.tableView->model();
    int rowCount = tableModel->rowCount();
    bool isReply = false;
    for (int row = 0; row < rowCount; row++) {
        QString qq = tableModel->data(tableModel->index(row, TableAcocountList::qqNumber), Qt::DisplayRole).toString();
        if (qq == model["qq"]) {
            isReply = true;
            break;
        }
    }
    if (!isReply) {
        listImport.append(model);
        ui.tableView->addData(model);
    } 
}

//添加单个数据
void CUiCenter::AddSignleRow(ImportData data)
{
    ModelData model; 
    model["qq"] = data.qq;

    model["bizType"] =QString::number(data.bizType);

    //qDebug()<<"bizType is "<<data.bizType;
    if (data.phoneNumber.contains(QRegExp("[\\x4e00-\\x9fa5]+"))) {
        data.phoneNumber = "";
    }
    model["phone"] = data.phoneNumber;

    if (data.password.contains(QRegExp("[\\x4e00-\\x9fa5]+"))) {
        data.password = "";
    }

    model["password"] = data.password;

    if (data.newPhoneNumber.contains(QRegExp("[\\x4e00-\\x9fa5]+"))) {
        data.newPhoneNumber = "";
    }
    model["new_phone"] = data.newPhoneNumber;

    model["status"] = "";

    model["task_status"] = QString::number(TaskStatus::None);

    ui.tableView->addData(model);
    //qDebug() << "mode is " << model;
    QAbstractItemModel* tableModel = ui.tableView->model();
    int rowCount = tableModel->rowCount();
    model["index"] = rowCount + 1;
    excuteSingleTasks(model);
}

//导入完成
void CUiCenter::OnImportFinished()
{
    importFinished = true;
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

//修改任务状态
void CUiCenter::onTaskDo(const int index, const QString msg, const int status,const int  bizType=-1)
{
    setListRowData(index, TableAcocountList::status, msg);
    setListRowData(index, TableAcocountList::task_status, status);
    if (bizType == -1) {
        return;
    }
    setListRowData(index, TableAcocountList::bizType, bizType);
}

//修改任务状态
void CUiCenter::showPyMsg(const int index, const QString recQQ, const QString val)
{
    if (index > 0) {
        QAbstractItemModel* tableModel = ui.tableView->model();
        QString qq = tableModel->data(tableModel->index(index, TableAcocountList::qqNumber), Qt::DisplayRole).toString();
        if (qq == recQQ) {
            setListRowData(index, TableAcocountList::status, val);
        }
    }
    PrintLog(QtInfoMsg, val);
}

//任务执行成功
void CUiCenter::onTaskRequestCallback(const ResponData& data, const QString& taskId)
{

    ui.groupBox_btn->setEnabled(true);

    if (data.task.reqeustId == 0)
        return;
    if (data.task.reqeustId == (quint64)this) {
        QJsonObject dataObj;
        DataParseResult result;
        WebHandler::ParseJsonData(data.dataReturned, dataObj, &result);
        if (result.errorCode == DataParseResult::NoError) {
            parseLocalTaskData(dataObj, data.task.index, taskId);
        } else {
            onTaskDo(data.task.index, result.message, TaskStatus::Error, data.task.bizType);
        }
    }

}

//任务执行错误
void CUiCenter::onTaskRequestError(const ResponData& data, NetworkRequestError errorType, const QString& errorString)
{
    ui.groupBox_btn->setEnabled(true);
    if (data.task.reqeustId == 0)
        return;
    if (data.task.reqeustId == (quint64)this) {
        QString msg = errorString.isEmpty() ? tr("Network erorr, error code = %1").arg((int)errorType) : errorString;
        qDebug() << "error is " << msg;
        if (msg.contains(QString("refused"))) {
            msg = QString::fromLocal8Bit("网络连接失败");
        }
        onTaskDo(data.task.index, msg, TaskStatus::Error, data.task.bizType);
        PrintLog(QtWarningMsg,msg);
    }
}

//获取余额
void CUiCenter::getUserWallet()
{
    RequestTask task;
    task.reqeustId = (quint64)this;
    task.headerObj.insert("uid", UserSession::instance().userData().uid);
    task.headerObj.insert("token", UserSession::instance().userData().token);
    task.apiIndex = API::getWalllet;
    WebHandler::instance()->Get(task);
}

// bind platform
void CUiCenter::bindPlatform()
{

    ModeDataList dataList;
    GetCurrentData(dataList);
    if (dataList.size()>0) {
        for (ModelData data : dataList) {
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
}

//绑定平台
void CUiCenter::bindPlatformAll()
{
    QAbstractItemModel* model = ui.tableView->model();
    int rowCounts = model->rowCount();
    for (int row = 0; row < rowCounts; row++) {
        ModelData data;
        data["qq"] = model->data(model->index(row, TableAcocountList::qqNumber), Qt::DisplayRole).toString();
        data["phone"] = model->data(model->index(row, TableAcocountList::phoneNumber), Qt::DisplayRole).toString();
        data["password"] = model->data(model->index(row, TableAcocountList::password), Qt::UserRole).toString();
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

//任务类型
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

//任务状态
QString CUiCenter::getTaskStatusString(TaskStatus taskStatus)
{
    switch (taskStatus) {
    case Wait:
        return tr("task Wait");
    case Doing:
        return tr("task Doing");
    case Stop:
        return tr("task Stop");
    case Error:
        return tr("task Error");
    case Success:
        return tr("task Success");
        break;
    default:
        return "";
    }
}
