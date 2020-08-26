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
#include <QStandardItem>
#include <QTableView>

QList<HeadStruct> listHead;

CUiCenter::CUiCenter(QWidget* parent)
    : BaseWidget(parent)
    , mLineEdit(nullptr)
    , mTableView(nullptr)
{
    WebHandler::bindDataCallback(this, SLOT(onRequestCallback(const ResponData&)));
    qRegisterMetaType<BorrowInfo>("BorrowInfo");
    this->initUi();
    this->initData();
}

CUiCenter::~CUiCenter()
{
}

void CUiCenter::initUi()
{
    QGridLayout* main_layout = new QGridLayout(this);
    main_layout->setContentsMargins(9, 9, 9, 9);
    main_layout->setSpacing(9);

    mLineEdit = new CSearchLineEdit(this);
    connect(mLineEdit, &CSearchLineEdit::query, [this](const QString& info) {
        this->FuzzyQuery(info);
    });
    mLineEdit->setPlaceholderText(TOCH("search"));
    mLineEdit->setFixedHeight(24);

    m_pDataCount = new QLabel(this);
    m_pDataCount->setAlignment(Qt::AlignCenter);

    this->initTableView();
    Q_ASSERT(mTableView);
    Q_ASSERT(mLineEdit);
    main_layout->addWidget(mLineEdit, 0, 0, 1, 2);
    main_layout->addWidget(m_pDataCount, 0, 2, 1, 1);

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
        25, 25, "", "btn_add");
    btn_add->setToolTip(tr("add"));

    QPushButton* btn_log = UiHelper::creatPushButton(
        this, [=]() {
            OperationLog* content = new OperationLog(this);
            PopupDialogContainer::showSecondPopupFadeIn(content, CApp->getMainWidget(), tr("log"));
        },
        25, 25, "", "btn_log");
    btn_log->setToolTip(tr("log"));

    QPushButton* btn_export = UiHelper::creatPushButton(
        this, [=]() {
            this->sqlQueryExport();
        },
        25, 25, "", "btn_export");
    btn_export->setToolTip(TOCH("export"));

    main_layout->addWidget(btn_export, 0, 9, 1, 1);
    main_layout->addWidget(btn_log, 0, 10, 1, 1);
    main_layout->addWidget(btn_add, 0, 11, 1, 1);
    main_layout->addWidget(mTableView, 1, 0, 10, 12);
}

void CUiCenter::initHeader()
{
    Q_ASSERT(mTableView);

    HeadStruct headNode = { "id", -1, true };
    listHead << headNode;

    headNode = { tr("index"), 70, false };
    listHead << headNode;

    headNode = { tr("qq number"), -1, false };
    listHead << headNode;

    headNode = { tr("phone number"), -1, false };
    listHead << headNode;

    headNode = { tr("status"), -1, false };
    listHead << headNode;

    mTableView->setHeader(listHead);
}

void CUiCenter::initData()
{
    /*CDbHelper dbHelper;
    dbHelper.open();
    QList<ModelData> vModel;
    if (dbHelper.isTableExist(DIC_BORROW_RETURN)) {
        int rows = dbHelper.Queuey(vModel, "SELECT * FROM DIC_BORROW_RETURN  WHERE deleteFlag = '0' ORDER BY updateDate desc");
    } else {
        dbHelper.exec(CREATE_TABLE_SQL);
    }
    this->setData(vModel);*/
    RequestTask task;
    task.reqeustId = (quint64)mTableView;
    task.headerObj.insert("uid", UserSession::instance().userData().uid);
    task.headerObj.insert("token", UserSession::instance().userData().token);
    task.apiIndex = API::accountList;
    WebHandler::instance()->Post(task);
}

void CUiCenter::initTableView()
{
    mTableView = new CTableview(this);
    this->initHeader();
    mTableView->setCreatRowCallbackListener(this);

    mTableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(mTableView, SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(slotContextMenu(const QPoint&)));
    connect(mTableView, SIGNAL(doubleClicked(const QModelIndex&)),
        this, SLOT(slotTableViewDoubleClicked(const QModelIndex&)));
    connect(mTableView, &CTableview::selectionRowChanged, [=](int selectedCount) {
        m_pDataCount->setProperty("selectedCount", selectedCount);
        this->updateCountLabel();
    });

    mTableView->setItemDelegate(new ReadOnlyDelegateBRTable(this));
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
    QString sql = "";
    if (key.isEmpty()) {
        sql = QString("SELECT * \
		FROM \
		DIC_BORROW_RETURN WHERE deleteFlag = '0' ORDER BY updateDate desc");
    } else {
        sql = QString("SELECT * \
		FROM \
		DIC_BORROW_RETURN \
		WHERE \
		(productionId LIKE '%%1%' \
		OR productionName LIKE '%%1%' \
		OR borrowerName LIKE '%%1%' \
		OR borrowReason LIKE '%%1%' \
		OR remarks LIKE '%%1%') AND deleteFlag = '0' ORDER BY updateDate desc;")
                  .arg(key);
    }

    this->sqlQuery(sql);
}

void CUiCenter::setData(const QList<ModelData>& vModel)
{
    uint nLen = vModel.size();
    m_pDataCount->setProperty("resultCount", nLen);
    this->updateCountLabel();
    mTableView->setData(vModel);
}

void CUiCenter::showDetailUi(const QModelIndex& index)
{
    QModelIndex indexFirst = mTableView->model()->index(0, (int)TableHeader::Order);
    if (mTableView->model()->data(indexFirst, Qt::UserRole).toBool())
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
        int status = model["borrowStatus"].toInt();
        if (BorrowStatus::Returned == status) {
            model["borrowStatus"] = TOCH("�ѻ�");
        } else if (BorrowStatus::NotReturned == status) {
            model["borrowStatus"] = TOCH("δ��");
        } else {
            model["borrowStatus"] = TOCH("��ʧ");
        }
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
    list << TOCH("Excel97-2003������(*.xls)");
    dialog.setNameFilters(list);
    dialog.selectFile(TOCH("�����Ƽ�%1�軹��Ϣ.xls").arg(TOCH("")));
    if (QDialog::Accepted != dialog.exec())
        return;

    QString fileName = dialog.selectedFiles()[0];
    if (Odbexcel::save(fileName, header, datas, "")) {
        QStringList button;
        button << TOCH("������") << TOCH("ȡ��");
        int res = DialogMsg::question(this, TOCH("��ʾ"), TOCH("����ɹ���<br>�ѱ��浽��<span style='color:rgb(0,122,204)'>%1</span>").arg(fileName), QMessageBox::Ok | QMessageBox::Cancel, button);
        if (res == QMessageBox::Ok) {
            QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));
        }
    } else {
        QString msgError = TOCH("����ʧ�ܣ�") + Odbexcel::getError();
        DialogMsg::question(this, TOCH("����"), msgError, QMessageBox::Ok);
    }
}

void CUiCenter::updateCountLabel()
{
    int resultCount = m_pDataCount->property("resultCount").toInt();
    int selectedCount = m_pDataCount->property("selectedCount").toInt();
    QString tip;
    tip.sprintf("%s<span style='color:rgb(0,122,204)'>%d</span>%s", tr("searched"), resultCount, tr("records"));
    m_pDataCount->setText(tip);
}

void CUiCenter::getBorrowData(BorrowInfo& info, int row)
{
    QAbstractItemModel* model = mTableView->model();
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
    QAbstractItemModel* model = mTableView->model();
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
    QAbstractItemModel* model = mTableView->model();
    model->setData(model->index(row, TableHeader::UniqueId), info.id, Qt::DisplayRole);
    model->setData(model->index(row, TableHeader::ProductionId), info.productionId, Qt::DisplayRole);
    model->setData(model->index(row, TableHeader::ProductionName), info.productionName, Qt::DisplayRole);
    model->setData(model->index(row, TableHeader::BorrowerName), info.borrowerName, Qt::DisplayRole);
    model->setData(model->index(row, TableHeader::BorrowDate), info.borrowDate, Qt::UserRole);
    model->setData(model->index(row, TableHeader::Status), (int)info.borrowStatus, Qt::UserRole);
    model->setData(model->index(row, TableHeader::BorrowReason), info.borrowReason, Qt::DisplayRole);
    model->setData(model->index(row, TableHeader::Remark), info.remarks, Qt::DisplayRole);
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
    QModelIndex index = mTableView->indexAt(pos);
    if (index.isValid()) {
        QMenu* menu = new QMenu(mTableView);

        QAbstractItemModel* model = mTableView->model();
        int nCount = mTableView->selectionModel()->selectedRows().count();
        if (nCount > 1) {
            menu->addAction(tr("export"), [=] {
                QList<ModelData> vModel;
                QModelIndexList list = mTableView->selectionModel()->selectedRows();
                for (const QModelIndex& index : list) {
                    ModelData info;
                    this->getBorrowData(info, index.row());
                    vModel.append(info);
                }
                this->doExport(vModel);
            });

            menu->addAction(tr("delete"), [=] {
                QMessageBox::StandardButton ok = DialogMsg::question(CApp->getMainWidget(), tr("tips"),
                    TOCH("ȷ��Ҫɾ����<span style='color:rgb(0,122,204)'>%1</span>����¼��").arg(nCount), QMessageBox::Ok | QMessageBox::Cancel);
                if (ok == QMessageBox::Ok) {
                    QList<BorrowInfo> listInfo;
                    QModelIndexList list = mTableView->selectionModel()->selectedRows();
                    for (QModelIndex index : list) {
                        BorrowInfo info;
                        this->getBorrowData(info, index.row());
                        listInfo << info;
                    }
                    this->sqlDelete(listInfo);
                    this->FuzzyQuery();
                }
            });
        } else {
            QModelIndex indexFirst = mTableView->model()->index(0, (int)TableHeader::Order);
            menu->addAction(TOCH("�鿴����"), [=] {
                this->showDetailUi(index);
            });

            menu->addAction(TOCH("ɾ��"), [=] {
                QMessageBox::StandardButton ok = DialogMsg::question(CApp->getMainWidget(), TOCH("��ʾ"),
                    TOCH("ȷ��Ҫɾ��������¼��"), QMessageBox::Ok | QMessageBox::Cancel);
                if (ok == QMessageBox::Ok) {
                    BorrowInfo info;
                    this->getBorrowData(info, index.row());
                    this->sqlDelete(info);
                    this->FuzzyQuery();
                }
            });

            QModelIndex indexLast = model->index(index.row(), (int)TableHeader::Status);
            BorrowStatus status = (BorrowStatus)model->data(indexLast, Qt::UserRole).toInt();
            QString id = model->data(model->index(index.row(), (int)TableHeader::UniqueId)).toString();
            switch (status) {
            case Returned:
                menu->addAction(TOCH("���Ϊδ��"), [=] {
                    ModelData data;
                    data["borrowStatus"] = QString::number((int)BorrowStatus::NotReturned);
                    this->sqlUpdate(data, id);
                    model->setData(indexLast, BorrowStatus::NotReturned, Qt::UserRole);
                });

                menu->addAction(TOCH("���Ϊ��ʧ"), [=] {
                    ModelData data;
                    data["borrowStatus"] = QString::number((int)BorrowStatus::Lost);
                    this->sqlUpdate(data, id);
                    model->setData(indexLast, BorrowStatus::Lost, Qt::UserRole);
                });
                break;
            case NotReturned:
                menu->addAction(TOCH("���Ϊ�ѻ�"), [=] {
                    ModelData data;
                    data["borrowStatus"] = QString::number((int)BorrowStatus::Returned);
                    this->sqlUpdate(data, id);
                    model->setData(indexLast, BorrowStatus::Returned, Qt::UserRole);
                });

                menu->addAction(TOCH("���Ϊ��ʧ"), [=] {
                    ModelData data;
                    data["borrowStatus"] = QString::number((int)BorrowStatus::Lost);
                    this->sqlUpdate(data, id);
                    model->setData(indexLast, BorrowStatus::Lost, Qt::UserRole);
                });
                break;
            case Lost:
                menu->addAction(TOCH("���Ϊ�ѻ�"), [=] {
                    ModelData data;
                    data["borrowStatus"] = QString::number((int)BorrowStatus::Returned);
                    this->sqlUpdate(data, id);
                    model->setData(indexLast, BorrowStatus::Returned, Qt::UserRole);
                });

                menu->addAction(TOCH("���Ϊδ��"), [=] {
                    ModelData data;
                    data["borrowStatus"] = QString::number((int)BorrowStatus::NotReturned);
                    this->sqlUpdate(data, id);
                    model->setData(indexLast, BorrowStatus::NotReturned, Qt::UserRole);
                });
                break;
            default:
                break;
            }
        }
        menu->exec(QCursor::pos());
    }
}

void CUiCenter::slotTableViewDoubleClicked(const QModelIndex& index)
{
    this->showDetailUi(index);
}

void CUiCenter::onRequestCallback(const ResponData& data)
{
    if (data.task.reqeustId == 0)
        return;
    if (data.task.reqeustId == (quint64)mTableView) {
        QJsonObject dataObj;
        DataParseResult result;
        WebHandler::ParseJsonData(data.dataReturned, dataObj, &result);
        if (result.errorCode == DataParseResult::NoError) {
            QJsonArray array = dataObj.value("List").toArray();
            auto iter = array.constBegin();
            QList<ModelData> listData;
            while (iter != array.constEnd()) {
                QJsonObject obj = (*iter).toObject();
                ModelData itemData;
                itemData["id"] = obj.value("id").toString();
                itemData["qq"] = obj.value("qq").toString();
                itemData["phone"] = obj.value("phone").toString();
                itemData["status"] = obj.value("status").toString();
                listData.append(itemData);
                ++iter;
            }
            this->setData(listData);
        }
    }
}
