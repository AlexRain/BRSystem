#include "CUiCenter.h"
#include "BRSystem.h"
#include "CApplication.h"
#include "CEditInfoDialog.h"
#include "CSearchLineEdit.h"
#include "ComboBoxDelegate.h"
#include "DialogMsg.h"
#include "FilterBtnDelegate.h"
#include "DownloadManager.h"
#include "InputPhoneView.h"
#include "InputPwdView.h"
#include "InputIdentityView.h"
#include "Odbexcel.h"
#include "OperationLog.h"
#include "PopupDialogContainer.h"
#include "ReadOnlyDelegate.h"
#include "SortFilterProxyModel.h"
#include "StyledDelegate.h"
#include "UserSession.h"
#include "WebHandler.h"
#include "MyThreadRunableProducer.h"
#include "MyThreadRunable.h"
#include "MyThreadPool.h"
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
#include <QButtonGroup>
#include "WaitWindow.h"

static const char* LAST_FILE_PATH = "last_file_path";
static const char* ThreadNumber = "thread_number";
QList<HeadStruct> listHead;
#define ENABLE_DRAG_FILE

bool hasWork = false;
bool canOp = true;
bool importFinished = true;
bool btnEnabled = true;
bool isPause = false;
int queueSize = 0;


QString getRole(QString type) {
	switch (type.toInt())
	{
	case 0:return QString::fromLocal8Bit("鬼剑士(男)");
	case 1: return QString::fromLocal8Bit("格斗家(女)");
	case 2: return QString::fromLocal8Bit("神枪手(男)");
	case 3: return QString::fromLocal8Bit("魔法师(女)");
	case 4: return QString::fromLocal8Bit("圣职者");
	case 5: return QString::fromLocal8Bit("神枪手(女)");
	case 6: return QString::fromLocal8Bit("暗夜使者");
	case 7: return QString::fromLocal8Bit("格斗家(男)");
	case 8: return QString::fromLocal8Bit("魔法师(男)");
	case 9: return QString::fromLocal8Bit("暗夜武士");
	case 10: return QString::fromLocal8Bit("缔造者");
	case 11: return QString::fromLocal8Bit("鬼剑士(女)");
	case 12: return QString::fromLocal8Bit("守护者");
	case 13: return QString::fromLocal8Bit("魔枪士");
	case 14: return QString::fromLocal8Bit("圣职者(女)");
	case 15: return QString::fromLocal8Bit("枪剑士");
	default:
		break;
	}
	return "";
}

CUiCenter::CUiCenter(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	qRegisterMetaType<ImportData>("ImportData");
	qRegisterMetaType<ResponData>("ResponData");

	connect(parent, SIGNAL(doImportLastFile()), this, SLOT(importLastFile()));
	connect(parent, SIGNAL(doExportFile()), this, SLOT(exportList()));


	WebHandler::bindDataCallback(this, SLOT(onRequestCallback(const ResponData&)));

	TaskManager::bindDataCallback(this, SLOT(onTaskRequestCallback(const ResponData&, const QString&)));

	TaskManager::bindTaskGoing(this, SLOT(onTaskDo(const int, const QString, const int)));

	TaskManager::bindErrorCallback(this, SLOT(onTaskRequestError(const ResponData&, NetworkRequestError, const QString&)));

	TaskManager::bindQuqueSize(this, SLOT(onQueueSize(const int)));

	TaskManager::bindTaskPause(this, SIGNAL(onTaskPause(const bool&)));

	MyThreadRunable::bindPauseTask(this, SIGNAL(onThreadPause(const bool&)));

	MyThreadRunable::bindDoRequest(this, SIGNAL(doImTask(MyTask)));

	MyThreadRunableProducer::bindRestartTask(this, SIGNAL(onRestartTask()));

	wsClient = new WebSocketClientManager();
	connect(wsClient, SIGNAL(showMsg(const int, const QString, const QString, const int)), this, SLOT(showPyMsg(const int, const QString, const QString, const int)));
	connect(wsClient, SIGNAL(loseConn()), this, SLOT(loseConnection()));
	wsClient->startConnect();

	this->getUserWallet();
	this->initUi();
	this->initData();
}

void CUiCenter::showPoolMsg(QString msg) {
	PrintLog(QtInfoMsg, msg);
}

CUiCenter::~CUiCenter()
{
	threadImport.quit();
	threadImport.wait();
}


void CUiCenter::showLoading() {
	WaitWindow* waitWindow = new WaitWindow(this);
	waitWindow->setColor(Qt::white);
	waitWindow->startAnimation();
}

//初始化ui
void CUiCenter::initUi()
{

	AppSetting = new QSettings("app.ini", QSettings::IniFormat);

	this->initTableView();

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

	ui.xiancheng_groupBox->setTitle(QString::fromLocal8Bit("线程设置"));

	ui.label_xiancheng->setText(QString::fromLocal8Bit("线程数量(1-10)"));

	ui.btn_query_ban->setText(QString::fromLocal8Bit("查询封号/制裁"));

	ui.btn_query_point->setText(QString::fromLocal8Bit("查询点券"));

	ui.btn_modify_pwd->setText(QString::fromLocal8Bit("修改密码"));

	ui.btn_update_identity->setText(QString::fromLocal8Bit("修改认证"));

	ui.btn_buy_goods->setText(QString::fromLocal8Bit("买道具"));

	ui.btn_query_quick_role->setText(QString::fromLocal8Bit("查询角色（快速）"));

	ui.btn_xiancheng_save->setText(QString::fromLocal8Bit("保存"));
	int xc_number = AppSetting->value(ThreadNumber).toInt();
	if (xc_number <= 0) {
		xc_number = 1;
	}
	ui.spinBox_xiancheng->setValue(xc_number);
	ui.spinBox_xiancheng->setMaximum(10);


	ui.groupBox_xy->setTitle(QString::fromLocal8Bit("执行方式"));
	//打码方式
	QButtonGroup *m_group_1 = new QButtonGroup(this);
	m_group_1->addButton(ui.radioButton_api, 1);
	m_group_1->addButton(ui.radioButton_driver, 2);

	//无头模式
	QButtonGroup *m_group_2 = new QButtonGroup(this);
	m_group_2->addButton(ui.radioButton_hd_on, 1);
	m_group_2->addButton(ui.radioButton_hd_off, 2);

	//短信模式
	QButtonGroup *m_group_3 = new QButtonGroup(this);
	m_group_3->addButton(ui.radioButton_sms_auto, 1);
	m_group_3->addButton(ui.radioButton_sms, 2);

	ui.radioButton_api->setText("API");
	int api_type = AppSetting->value("api_type", 2).toInt();
	if (api_type == 1) {
		ui.radioButton_api->setChecked(true);
	}
	else {
		ui.radioButton_driver->setChecked(true);
	}

	ui.radioButton_driver->setText(QString::fromLocal8Bit("网页模拟"));

	int headless = AppSetting->value("headless", 2).toInt();
	if (headless == 1) {
		ui.radioButton_hd_on->setChecked(true);
	}
	else {
		ui.radioButton_hd_off->setChecked(true);
	}

	int sms_mode = AppSetting->value("sms_mode", 1).toInt();
	if (sms_mode == 1) {
		ui.radioButton_sms_auto->setChecked(true);
	}
	else {
		ui.radioButton_sms->setChecked(true);
	}

	//打码方式
	connect(m_group_1, SIGNAL(buttonClicked(int)), this, SLOT(on_radioButton_api_click(int)));
	connect(m_group_2, SIGNAL(buttonClicked(int)), this, SLOT(on_radioButton_hd_click(int)));
	connect(m_group_3, SIGNAL(buttonClicked(int)), this, SLOT(on_radioButton_sms_click(int)));

}


//初始化表头
void CUiCenter::initHeader()
{
	HeadStruct headNode = { "id", -1, true };
	listHead << headNode;

	headNode = { QString::fromLocal8Bit("选择"), 30 };
	listHead << headNode;

	headNode = { tr("index"), 60 };
	listHead << headNode;

	headNode = { tr("qq number"), 120 };
	listHead << headNode;

	headNode = { tr("password"), 120 };
	listHead << headNode;

	headNode = { QString::fromLocal8Bit("手机号/大区/姓名"), 120 };
	listHead << headNode;

	headNode = { QString::fromLocal8Bit("新手机号/身份证号"), 120 };
	listHead << headNode;

	headNode = { tr("task_status"), 60 ,true };
	listHead << headNode;


	headNode = { QString::fromLocal8Bit("任务状态"), 60 };
	listHead << headNode;

	headNode = { tr("bizType"), 60 ,true };
	listHead << headNode;

	headNode = { QString::fromLocal8Bit("类型"), 60 };
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

	{
		auto itemStatus = new QStandardItem;
		itemStatus->setData(false, Qt::CheckStateRole);
		item.append(itemStatus);
	}

	item.append(new QStandardItem(QString::number(index + 1)));

	item.append(new QStandardItem(model["qq"]));

	item.append(new QStandardItem(model["password"]));

	//{
	//	auto itemPassword = new QStandardItem;
	//	itemPassword->setData(model["password"], Qt::UserRole;
	//	item.append(itemPassword);
	//}

	item.append(new QStandardItem(model["phone"]));

	item.append(new QStandardItem(model["new_phone"]));


	auto taskStatus = (TaskStatus)model["task_status"].toInt();
	{
		auto itemStatus = new QStandardItem;
		itemStatus->setData(taskStatus, Qt::UserRole);
		itemStatus->setData(getTaskStatusString(taskStatus), Qt::ToolTipRole);
		item.append(itemStatus);
	}

	item.append(new QStandardItem(getTaskStatusString(taskStatus)));

	auto bizType = (TaskType)model["bizType"].toInt();
	{
		auto itemStatus = new QStandardItem;
		itemStatus->setData(bizType, Qt::UserRole);
		itemStatus->setData(getTaskTypeString(bizType), Qt::ToolTipRole);
		item.append(itemStatus);
	}
	item.append(new QStandardItem(getTaskTypeString(bizType)));
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
	item.at(8)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

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

	ui.tableView->setItemDelegate(new CheckBoxDelegateBRTable(this));
	//ui.tableView->setItemDelegate(new ReadOnlyDelegateBRTable(this));
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
	//btnEnabled = (selectedCount > 0);
	//if (btnEnabled) {
	//    ModeDataList dataList;
	//    GetCurrentSelectData(dataList);
	//}
	//ui.groupBox_btn->setEnabled(btnEnabled);
}


void CUiCenter::exportList()
{
	ExportSettingView viewExport;
	if (QDialog::Accepted != viewExport.exec())
		return;
	viewExport.GetExportSetting(exportSetting);
	qDebug() << "setting is " << exportSetting.export_role;
	doExport(exportSetting);
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
bool CUiCenter::GetCurrentSelectData(QList<ModelData>& selectedRows)
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
		ModelData data = getRowData(model, row);
		qDebug() << "select model data is " << data;
		if (data["task_status"].toInt() <= TaskStatus::Stop) {
			isCanOp = false;
		}
		bool isCheck = model->data(model->index(row, TableAcocountList::checkBox), Qt::CheckStateRole).toBool();
		selectedRows.append(data);
	}
	canOp = isCanOp;
	return true;
}

bool CUiCenter::GetCurrentData(QList<ModelData>& selectedRows)
{
	QAbstractItemModel* tableModel = ui.tableView->model();
	int rowCount = tableModel->rowCount();
	for (int row = 0; row < rowCount; row++) {
		bool isCheck = tableModel->data(tableModel->index(row, TableAcocountList::checkBox), Qt::CheckStateRole).toBool();
		if (isCheck) {
			ModelData data = getRowData(tableModel, row);
			qDebug() << "model data is " << data;
			selectedRows.append(data);
		}
	}
	if (selectedRows.size() > 0) {
		return true;
	}
	DialogMsg::question(this, tr("warning"), tr("please select at least one row"), QMessageBox::Ok);
	return false;
}

//获取单条记录
ModelData CUiCenter::getRowData(QAbstractItemModel* model, int row) {
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

	data["task_status_txt"] = model->data(model->index(row, TableAcocountList::task_status_txt), Qt::DisplayRole).toString();

	data["bizType_txt"] = model->data(model->index(row, TableAcocountList::bizType_txt), Qt::DisplayRole).toString();

	data["show"] = model->data(model->index(row, TableAcocountList::show), Qt::DisplayRole).toString();

	return data;
}

//添加任务进队
void CUiCenter::excuteSingleTasks(ModelData data)
{
	qDebug() << "excutor data is " << data;
	//禁用按钮
	ui.groupBox_btn->setEnabled(false);

	RequestTask task;
	task.reqeustId = (quint64)this;
	task.bizType = data["bizType"].toInt();
	task.index = data["index"].toInt();
	task.headerObj.insert("uid", UserSession::instance().userData().uid);
	task.headerObj.insert("token", UserSession::instance().userData().token);
	task.bodyObj.insert("bizType", task.bizType);

	QJsonObject objParam;
	objParam.insert("new_password", data["new_password"]);
	objParam.insert("new_phone", data["new_phone"]);
	objParam.insert("password", data["password"]);
	objParam.insert("phone", data["phone"]);
	objParam.insert("qq", data["qq"]);
	task.bodyObj.insert("params", objParam);
	task.apiIndex = API::addTask;

	//修改状态为等待执行中
	MyTask myTask;
	myTask.taskName = data["qq"];
	myTask.task = task;
	MyThreadRunableProducer::instance()->addQueue(myTask, this);

	//修改状态为等待执行中
	onTaskDo(task.index, QString::fromLocal8Bit("等待执行"), TaskStatus::Wait, task.bizType);
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
			bool inputPwd = ShowInputPwdView(password);
			if (!inputPwd) {
				return;
			}
		}

		qInfo("=========start excute tasks,total count is %d, task type is %d=========", dataList.size(), bizType);
		for (const auto& data : dataList) {
			doTask(data, password, bizType);
		}
	}
}

void CUiCenter::doTask(ModelData data, QString newPassword = "", int bizType = 0) {

	QString phoneNumber = data["phone"];
	if (phoneNumber.contains(QRegExp("[\\x4e00-\\x9fa5]+")) && (bizType != buy_goods && bizType != query_update_identity)) {
		phoneNumber = "";
	}
	if (bizType == recieve_sms) {
		phoneNumber = Reciever;
	}
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
	objParam.insert("phone", phoneNumber);
	objParam.insert("qq", data["qq"]);

	objParam.insert("goods", goodsReq);

	task.bodyObj.insert("params", objParam);
	task.apiIndex = API::createTask;

	MyTask myTask;
	myTask.taskName = data["qq"];
	myTask.task = task;
	if (bizType == send_short_message) {
		emit doImTask(myTask);
	}
	else {
		MyThreadRunableProducer::instance()->addQueue(myTask, this);
		onTaskDo(task.index, QString::fromLocal8Bit("等待执行"), TaskStatus::Wait, task.bizType);
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

bool CUiCenter::ShowInputIdentityView()
{
	InputIdentityView view(this);
	if (QDialog::Accepted == view.exec()) {
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
			qDebug() << "show is " << strShow;
			onTaskDo(index, strShow, TaskStatus::Success);
			auto taskType = static_cast<TaskType>(type);
			QString strType = getTaskTypeString(taskType);
			//打印日志
			PrintLog(QtInfoMsg, "task:" + QString("[%1]").arg(strType) + strShow);
		}
		else {
			onTaskDo(index, tr("parse json error"), TaskStatus::Error);
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
		//GetCurrentSelectData(dataList);

		menu = new QMenu(ui.tableView);
		QAbstractItemModel* model = ui.tableView->model();
		QModelIndex curentIndex = ui.tableView->currentIndex();

		if (!hasWork) {
			addTableMenuActionSolt(menu, QString::fromLocal8Bit("全选"), SLOT(selectTableAll()), false);
			addTableMenuActionSolt(menu, QString::fromLocal8Bit("取消全选"), SLOT(cancelSelectTableAll()), false);
			addTableMenuActionSolt(menu, QString::fromLocal8Bit("选中"), SLOT(selectOrder()), false);
			addTableMenuActionSolt(menu, QString::fromLocal8Bit("取消选中"), SLOT(selectDimiss()), false);
			addTableMenuActionSolt(menu, QString::fromLocal8Bit("重做"), SLOT(resetTask()), false);
			addTableMenuActionSolt(menu, QString::fromLocal8Bit("失败重做"), SLOT(resetAllTask()), false);
		}

		addTableMenuActionSolt(menu, QString::fromLocal8Bit("停止"), SLOT(pauseTask()), false);
		addTableMenuActionSolt(menu, QString::fromLocal8Bit("继续"), SLOT(restartTask()), false);
		addTableMenuActionSolt(menu, tr("modify_pwd"), SLOT(on_btn_modify_pwd_clicked()), !canOp);
		addTableMenuActionSolt(menu, tr("release_safe_mode"), SLOT(on_btn_release_safe_model_clicked()), !canOp);
		addTableMenuActionSolt(menu, tr("query_ban"), SLOT(on_btn_query_ban_clicked()), !canOp);
		addTableMenuActionSolt(menu, tr("query_role"), SLOT(on_btn_role_clicked()), !canOp);
		addTableMenuActionSolt(menu, QString::fromLocal8Bit("快速查询角色"), SLOT(on_btn_query_quick_role_clicked()), !canOp);
		addTableMenuActionSolt(menu, tr("query_score"), SLOT(on_btn_query_score_clicked()), !canOp);
		addTableMenuActionSolt(menu, tr("query_identity"), SLOT(on_btn_query_identity_clicked()), !canOp);
		addTableMenuActionSolt(menu, QString::fromLocal8Bit("查询点券"), SLOT(on_btn_query_point_clicked()), !canOp);
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
void  CUiCenter::addTableMenuActionSolt(QMenu *menu, QString text, const char* method, bool disabled) {
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
	if (data.task.apiIndex == API::exportAccount) {
		qDebug() << "exportAccount is callback";
		QVariantList  dataList;
		DataParseResult result;
		WebHandler::ParseJsonArrayData(data.dataReturned, dataList, &result);
		if (result.errorCode == DataParseResult::NoError) {
			exportFile(dataList);
		}
		else {
			DialogMsg::question(this, tr("warning"), QString::fromLocal8Bit("导出失败"), QMessageBox::Ok);
			return;
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
			onTaskDo(data.task.index, result.message, TaskStatus::Success);
		}
		else {
			onTaskDo(data.task.index, result.message, TaskStatus::Error);
		}
	}
	else if (data.task.reqeustId == (quint64)this) {
		if (data.task.apiIndex == API::SyncPhone) {
			PrintLog(QtInfoMsg, tr("sync phone numbers success!"));
			initData();
		}
	}
}

//全选
void CUiCenter::selectTableAll() {
	QAbstractItemModel* tableModel = ui.tableView->model();
	int rowCount = tableModel->rowCount();
	for (int row = 0; row < rowCount; row++) {
		bool isCheck = tableModel->data(tableModel->index(row, TableAcocountList::checkBox), Qt::CheckStateRole).toBool();
		if (!isCheck) {
			tableModel->setData(tableModel->index(row, TableAcocountList::checkBox), true, Qt::CheckStateRole);
		}
	}
}

//取消全选
void CUiCenter::cancelSelectTableAll() {
	QAbstractItemModel* tableModel = ui.tableView->model();
	int rowCount = tableModel->rowCount();
	for (int row = 0; row < rowCount; row++) {
		bool isCheck = tableModel->data(tableModel->index(row, TableAcocountList::checkBox), Qt::CheckStateRole).toBool();
		if (isCheck) {
			tableModel->setData(tableModel->index(row, TableAcocountList::checkBox), false, Qt::CheckStateRole);
		}
	}
}

//选中
void CUiCenter::selectOrder() {
	auto selectRows = ui.tableView->selectionModel()->selectedRows();
	int count = selectRows.size();
	if (selectRows.isEmpty()) {
		return;
	}
	QAbstractItemModel* model = ui.tableView->model();
	for (const auto& rowData : selectRows) {
		int row = rowData.row();
		QModelIndex index = model->index(row, TableAcocountList::checkBox);
		bool isCheck = model->data(index, Qt::CheckStateRole).toBool();
		if (!isCheck) {
			model->setData(index, true, Qt::CheckStateRole);
		}
	}
}

//取消选中
void CUiCenter::selectDimiss() {
	auto selectRows = ui.tableView->selectionModel()->selectedRows();
	int count = selectRows.size();
	if (selectRows.isEmpty()) {
		return;
	}
	QAbstractItemModel* model = ui.tableView->model();
	for (const auto& rowData : selectRows) {
		int row = rowData.row();
		QModelIndex index = model->index(row, TableAcocountList::checkBox);
		bool isCheck = model->data(index, Qt::CheckStateRole).toBool();
		if (isCheck) {
			model->setData(index, false, Qt::CheckStateRole);
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
		qDebug() << "reset data is " << data;
		qDebug() << "bizType is " << taskType;
		qDebug() << "task_status is " << task_status;
		if (taskType > 0 && task_status == TaskStatus::Error) {
			emit onTaskDo(data["index"].toInt(), QString::fromLocal8Bit("失败重做"), TaskStatus::Wait);
			doTask(data, data["new_password"], taskType);
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
		qDebug() << "reset data is " << data;
		qDebug() << "bizType is " << taskType;
		qDebug() << "task_status is " << task_status;
		if (taskType > 0 && task_status == TaskStatus::Error) {
			emit onTaskDo(data["index"].toInt(), QString::fromLocal8Bit("失败重做"), TaskStatus::Wait);
			doTask(data, data["new_password"], taskType);
		}
	}
}

//暂停任务
void CUiCenter::pauseTask() {
	emit  onThreadPause(true);

	if (queueSize <= 0) {
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
		if (taskStatus == TaskStatus::Wait) {
			onTaskDo(row, QString::fromLocal8Bit("停止"), TaskStatus::Stop);
		}
	}
}

//恢复任务
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
	data.qq = ui.lineEdit_single_qq->text();
	if (data.qq.isEmpty()) {
		return;
	}
	data.bizType = ui.comboBox_single->currentIndex();
	if (data.bizType <= 0) {
		return;
	}

	AddSignleRow(data);
}

//线程设置
void CUiCenter::on_btn_xiancheng_save_clicked()
{
	int xiancheng_number = ui.spinBox_xiancheng->value();
	if (xiancheng_number <= 0 || xiancheng_number > 10) {
		DialogMsg::question(this, tr("warning"), QString::fromLocal8Bit("线程数在1-10之间"), QMessageBox::Ok);
		return;
	}
	QSettings setting("app.ini", QSettings::IniFormat);
	setting.setValue(ThreadNumber, xiancheng_number);
	int result = DialogMsg::question(this, tr("tips"), QString::fromLocal8Bit("重启生效，立即重启？"), QMessageBox::Ok | QMessageBox::Cancel);
	if (result == QMessageBox::Ok) {
		qApp->exit(RESTART_CODE);
	}
}

/*
修改密码
*/
void CUiCenter::on_btn_modify_pwd_clicked()
{
	excuteTasks(change_password);
}

//解除安全模式
void CUiCenter::on_btn_release_safe_model_clicked()
{
	excuteTasks(unpack_safe_mode);
}

//查询封号
void CUiCenter::on_btn_query_ban_clicked()
{
	excuteTasks(query_ban);
}

//查询角色
void CUiCenter::on_btn_role_clicked()
{
	excuteTasks(query_role);
}

//快速查询角色
void CUiCenter::on_btn_query_quick_role_clicked()
{
	qDebug() << "query_quick_role";
	excuteTasks(query_quick_role);
}

//查询信用分
void CUiCenter::on_btn_query_score_clicked()
{
	excuteTasks(query_credit_score);
}

//查询认证
void CUiCenter::on_btn_query_identity_clicked()
{
	excuteTasks(query_identity);
}

//查询点券
void CUiCenter::on_btn_query_point_clicked()
{
	excuteTasks(query_point);
}

//修改认证
void CUiCenter::on_btn_update_identity_clicked()
{
	//if (ShowInputIdentityView()) {
	excuteTasks(query_update_identity);
	//}
	//return;
}


//买道具
void CUiCenter::on_btn_buy_goods_clicked()
{
	SelectGoodsView goodsView;
	if (QDialog::Accepted == goodsView.exec()) {
		goodsReq = goodsView.goodsRequest;
		excuteTasks(buy_goods);
	}
	return;
}


//收取短信
void CUiCenter::on_recieve_sms(QString phone)
{
	Reciever = phone;
	qDebug() << "Reciever " << Reciever;

	if (Reciever.isEmpty()) {
		DialogMsg::question(this, tr("tips"), QString::fromLocal8Bit("请输入手机号"), QMessageBox::Ok | QMessageBox::Cancel);
		return;
	}

	RequestTask task;
	task.reqeustId = (quint64)this;
	task.headerObj.insert("uid", UserSession::instance().userData().uid);
	task.headerObj.insert("token", UserSession::instance().userData().token);
	int bizType = static_cast<int>(recieve_sms);
	task.bodyObj.insert("bizType", bizType);
	QJsonObject objParam;
	objParam.insert("phone", Reciever);
	task.bodyObj.insert("params", objParam);
	task.apiIndex = API::createTask;

	MyTask myTask;
	myTask.taskName = Reciever;
	myTask.task = task;
	MyThreadRunableProducer::instance()->addQueue(myTask, this);
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
	QString toPhone = ui.comboBox_to_phone->currentData().toString();
	if (toPhone.isEmpty()) {
		toPhone = "1069070069";
	}

	if (ui.lineEdit_phone->text().isEmpty() && toPhone.isEmpty()) {
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
	task.apiIndex = API::createTask;

	MyTask myTask;
	myTask.taskName = fromPhone;
	myTask.task = task;
	MyThreadRunableProducer::instance()->addQueue(myTask, this);
	//修改状态为等待执行中
	//onTaskDo(task.index, QString::fromLocal8Bit("等待执行"), TaskStatus::Wait, task.bizType);
	//TaskManager::instance()->Post(task);

}



//绑定手机
void CUiCenter::on_btn_bind_phone_clicked()
{
	excuteTasks(bind_mobile);
}


//协议方式
void CUiCenter::on_radioButton_api_click(int id)
{
	qDebug() << "api "<< id;
	AppSetting->setValue("api_type", id);
}


void CUiCenter::on_radioButton_hd_click(int id)
{
	AppSetting->setValue("headless",id);
}

void CUiCenter::on_radioButton_sms_click(int id)
{

	AppSetting->setValue("sms_mode", id);
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
	//if (data.phoneNumber.contains(QRegExp("[\\x4e00-\\x9fa5]+"))) {
	//	data.phoneNumber = "";
	//}
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

	model["checkBox"] = QString::number(0);

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

	model["bizType"] = QString::number(data.bizType);

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


	bool isModifyPassword = (change_password == data.bizType);
	bool isBindPhone = (bind_mobile == data.bizType);
	qDebug() << "isBindPhone " << isBindPhone;
	QString password;
	QString phoneNumber;
	if (isModifyPassword) {
		bool inputPwd = ShowInputPwdView(password);
		qDebug() << "inputPwd is " << inputPwd;
		if (!inputPwd) {
			return;
		}
		model["new_password"] = password;
	}
	if (isBindPhone) {
		if (!ShowInputPhone(phoneNumber)) {
			return;
		}
		model["new_phone"] = phoneNumber;
	}

	ui.tableView->addData(model);
	qDebug() << "singble mode is " << model;
	QAbstractItemModel* tableModel = ui.tableView->model();
	int rowCount = tableModel->rowCount();
	model["index"] = QString::number(rowCount - 1);

	setListRowData(rowCount - 1, TableAcocountList::newPhoneNumber, model["new_phone"]);

	qDebug() << "single index is " << rowCount - 1;

	ui.tableView->selectRow(rowCount - 1);

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
void CUiCenter::onTaskDo(const int index, const QString msg, const int status, const int  bizType)
{
	if (status >= TaskStatus::None) {
		canOp = true;
		ui.groupBox_btn->setEnabled(true);
	}
	setListRowData(index, TableAcocountList::show, msg);
	setListRowData(index, TableAcocountList::task_status_txt, getTaskStatusString((TaskStatus)status));
	setListRowData(index, TableAcocountList::task_status, QString::number(status));
	if (bizType <= 0) {
		return;
	}
	setListRowData(index, TableAcocountList::bizType, QString::number(bizType));
	setListRowData(index, TableAcocountList::bizType_txt, getTaskTypeString((TaskType)bizType));

}

//修改任务状态
void CUiCenter::loseConnection() {
	//qDebug() << "recieve lose connection";
	QAbstractItemModel* model = ui.tableView->model();
	int rowCount = model->rowCount();
	if (rowCount <= 0) {
		return;
	}
	canOp = true;
	bool isCanOp = true;
	for (int row = 0; row < rowCount; row++) {
		ModelData data = getRowData(model, row);
		//qDebug() << "lose data is " << data;
		int taskStatus = data["task_status"].toInt();
		if (taskStatus == TaskStatus::Take) {
			onTaskDo(row, QString::fromLocal8Bit("与服务器断开连接"), TaskStatus::Error);
		}
	}
}

void CUiCenter::showPyMsg(const int index, const QString recQQ, const QString msg, const int status)
{
	QAbstractItemModel* tableModel = ui.tableView->model();
	QString qq = tableModel->data(tableModel->index(index, TableAcocountList::qqNumber), Qt::DisplayRole).toString();
	qDebug() << "index is " << index;
	qDebug() << "MSG is " << msg;
	qDebug() << "recQQ is " << recQQ;
	qDebug() << "qq is " << qq;
	qDebug() << "status" << status;
	if (qq == recQQ) {
		setListRowData(index, TableAcocountList::show, msg);
		setListRowData(index, TableAcocountList::task_status, status);
	}

	if (status == 3) {
		QSettings smsSetting("sms.ini", QSettings::IniFormat);
		smsSetting.setValue(qq, 0);
		int result = DialogMsg::question(this, tr("tips"), msg, QMessageBox::Ok | QMessageBox::Cancel);
		if (result == QMessageBox::Ok) {
			smsSetting.setValue(qq, 1);
		}
		else {
			smsSetting.setValue(qq, 2);
		}
	}

	if (status == 4) {
		emit showRecieveSms(msg);
	}
	PrintLog(QtInfoMsg, msg);

}

//任务执行成功
void CUiCenter::onTaskRequestCallback(const ResponData& data, const QString& taskId)
{
	canOp = true;
	qDebug() << "data callback is " << data.dataReturned.constData();
	ui.groupBox_btn->setEnabled(true);

	if (data.task.reqeustId == 0)
		return;
	if (data.task.reqeustId == (quint64)this) {
		QJsonObject dataObj;
		DataParseResult result;
		WebHandler::ParseJsonData(data.dataReturned, dataObj, &result);
		if (result.errorCode == DataParseResult::NoError) {
			if (change_password == data.task.bizType) {
				//dataObj.value("bizType")
				setListRowData(index, TableAcocountList::password, data.task.bodyObj.value("new_passowrd"));
			}
			parseLocalTaskData(dataObj, data.task.index, taskId);
			getUserWallet();
		}
		else {
			onTaskDo(data.task.index, result.message, TaskStatus::Error);
			PrintLog(QtWarningMsg, result.message);
		}
	}
}

//任务执行错误
void CUiCenter::onTaskRequestError(const ResponData& data, NetworkRequestError errorType, const QString& errorString)
{
	canOp = true;
	ui.groupBox_btn->setEnabled(true);
	if (data.task.reqeustId == 0)
		return;
	if (data.task.reqeustId == (quint64)this) {
		QString msg = errorString.isEmpty() ? tr("Network erorr, error code = %1").arg((int)errorType) : errorString;
		qDebug() << "error is " << msg;
		if (msg.contains(QString("refused"))) {
			msg = QString::fromLocal8Bit("网络连接失败");
		}
		onTaskDo(data.task.index, msg, TaskStatus::Error);
		PrintLog(QtWarningMsg, msg);
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
	qDebug() << "data is " << dataList;
	if (dataList.size() > 0) {
		for (const auto& data : dataList) {
			RequestTask task;
			task.reqeustId = (quint64)this;
			task.headerObj.insert("uid", UserSession::instance().userData().uid);
			task.headerObj.insert("token", UserSession::instance().userData().token);
			QJsonArray arrayParam;
			if (!data["qq"].isEmpty() || !data["password"].isEmpty()) {
				QJsonObject obj;
				obj.insert("password", data["password"]);
				obj.insert("qq", data["qq"]);
				QString phoneNumber = data["phone"];
				if (phoneNumber.contains(QRegExp("[\\x4e00-\\x9fa5]+"))) {
					phoneNumber = "";
				}
				obj.insert("phone", phoneNumber);
				arrayParam.append(obj);
				task.bodyObj.insert("list", arrayParam);
				task.apiIndex = API::bindPlatform;
				WebHandler::instance()->Post(task);
			}
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
				QString phoneNumber = data["phone"];
				if (phoneNumber.contains(QRegExp("[\\x4e00-\\x9fa5]+"))) {
					phoneNumber = "";
				}
				obj.insert("phone", phoneNumber);
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
	case query_point:
		taskTypeString = QString::fromLocal8Bit("查询点券");
		break;
	case buy_goods:
		taskTypeString = QString::fromLocal8Bit("道聚城");
		break;
	case query_quick_role:
		taskTypeString = QString::fromLocal8Bit("快速查询角色");
		break;
	case query_update_identity:
		taskTypeString = QString::fromLocal8Bit("修改认证");
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
		return QString::fromLocal8Bit("等待执行");
	case Doing:
		return QString::fromLocal8Bit("执行中");
	case Stop:
		return QString::fromLocal8Bit("暂停");
	case Error:
		return QString::fromLocal8Bit("失败");
	case Success:
		return QString::fromLocal8Bit("成功");
		break;
	default:
		return "";
	}
}


//导出
void CUiCenter::doExport(ExportSettingView::ExportSetting setting)
{
	QJsonArray accountList;
	ModeDataList dataList;
	GetCurrentData(dataList);
	if (dataList.size() > 0) {
		for (const auto& data : dataList) {
			qDebug() << "data is " << data;
			qDebug() << "qq is " << data["qq"];
			qDebug() << "status is " << data["status"];
			accountList.append(data["qq"]);
			exportModeDataStatusMap[data["qq"]] = data["bizType_txt"]+data["task_status_txt"];
		}
		qDebug() << "accountList is " << accountList;
		if (accountList.size() > 0) {
			getExportList(accountList, setting);
		}
	}
	return;
}

//获取导出信息
void CUiCenter::exportFile(QVariantList & list)
{
	qDebug() << "export data is" << list;
	if (list.count() > 0) {
		QFileDialog dialog;
		dialog.setFileMode(QFileDialog::ExistingFile);
		dialog.setAcceptMode(QFileDialog::AcceptSave);
		QStringList strList;
		auto defaultFileName = QString("export_") + QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
		strList << "*.txt";
		dialog.setNameFilters(strList);
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
		for (int i = 0; i < list.count(); i++)
		{
			QString rowString;

			QVariantMap map = list[i].toMap();
			QString qq = map["QQ"].toString();
			qDebug() << "export qq is " << qq;
			rowString.append(qq);

			QString Password = map["Password"].toString();
			qDebug() << "export Password is " << Password;
			if (exportSetting.export_password) {
				if (!rowString.isEmpty())
					rowString.append(separator);
				rowString.append(Password);
			}

			QString Phone = map["Phone"].toString();
			qDebug() << "export Phone is " << Phone;
			if (exportSetting.export_phone) {
				if (!rowString.isEmpty())
					rowString.append(separator);
				rowString.append(Phone);
			}

			int identity = map["IdentityStatus"].toInt();
			qDebug() << "export IdentityStatus is " << identity;
			QString identityInfo = map["IdentityInfo"].toString();
			qDebug() << "export IdentityInfo is " << identity;
			if (exportSetting.export_identity) {
				if (!rowString.isEmpty())
					rowString.append(separator);
				rowString.append(QString::fromLocal8Bit("认证次数：") + QString::number(identity) + QString::fromLocal8Bit("次") + "|" + identityInfo);
			}

			int score = map["Score"].toInt();
			qDebug() << "export Score is " << score;
			if (exportSetting.export_score) {
				if (!rowString.isEmpty())
					rowString.append(separator);
				rowString.append(QString::fromLocal8Bit("信用分：") + QString::number(score));
			}

			int Point = map["Point"].toInt();
			qDebug() << "export Point is " << score;
			if (exportSetting.export_point) {
				if (!rowString.isEmpty())
					rowString.append(separator);
				rowString.append(QString::fromLocal8Bit("点券：") + QString::number(Point));
			}

			QString banInfo = map["BanInfo"].toString();
			if (!rowString.isEmpty())
				rowString.append(separator);
			rowString.append(banInfo);

			QString roles = map["Roles"].toString();
			qDebug() << "export roles is " << roles;
			QString rolesStr = "";

			qInfo() << map["name"].toString();
			qInfo() << map["age"].toString();
			if (exportSetting.export_level > 0 || exportSetting.export_role || exportSetting.export_roleName, exportSetting.export_roleCount) {
				if (!roles.isEmpty()) {
					QJsonParseError error;
					QJsonDocument document = QJsonDocument::fromJson(roles.toUtf8(), &error);
					if (QJsonParseError::NoError == error.error)
					{
						QJsonObject root = document.object();
						int count = root.value("count").toInt();
						if (count > 0) {
							//数组
							QList<QVariant> list = root.value("list").toVariant().toList();
							int rolesCount = 0;
							foreach(QVariant item, list)
							{
								QVariantMap map = item.toMap();
								QString area = map["area"].toString();
								QString roleName = map["name"].toString();
								QString role = map["type"].toString();
								QString level = map["level"].toString();
								if (exportSetting.export_level > 0) {
									if (level.toInt() < exportSetting.export_level) {
										continue;
									}
								}
								rolesStr += area;
								if (exportSetting.export_roleName) {
									rolesStr += "-" + roleName;
								}
								if (exportSetting.export_role) {
									rolesStr += "-" + getRole(role);
								}
								rolesStr += "-" + level + "|";
								rolesCount++;
							}
							rolesStr = QString::fromLocal8Bit("数量：") + QString::number(rolesCount) + "|" + rolesStr;
						}
					}
				}
			}

			if (!rowString.isEmpty())
				rowString.append(separator);
			rowString.append(rolesStr);

			if (!rowString.isEmpty())
				rowString.append(separator);
			rowString.append(QString::fromLocal8Bit("状态") + exportModeDataStatusMap[qq]);
			dataStream << rowString << "\n";
		}
		file.close();
		qobject_cast<BRSystem*>(CApp->getMainWidget())->printLog(QtInfoMsg, tr("export success"));
	}
}

//获取导出信息
void CUiCenter::getExportList(QJsonArray accounts, ExportSettingView::ExportSetting exportSetting)
{
	RequestTask task;
	task.reqeustId = (quint64)this;
	task.headerObj.insert("uid", UserSession::instance().userData().uid);
	task.headerObj.insert("token", UserSession::instance().userData().token);
	QJsonArray arrayParam;
	task.bodyObj.insert("accounts", accounts);
	task.bodyObj.insert("has_ban", exportSetting.export_ban);
	task.bodyObj.insert("has_zhicai", exportSetting.export_zhicai);

	task.apiIndex = API::exportAccount;
	WebHandler::instance()->Post(task);
}