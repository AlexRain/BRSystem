#pragma once

#include "BaseWidget.h"
#include "CApplication.h"
#include "CDbHelper.h"
#include "CTableview.h"
#include "ExportDataView.h"
#include "NetworkDefine.h"
#include "ReadOnlyDelegate.h"
#include "TaskManager.h"
#include "ThreadImport.h"
#include "define.h"
#include "ui_CUiCenter.h"
#include <QModelIndex>
#include <QPainter>
#include <QStandardItemModel>
#include <QWidget>

class CSearchLineEdit;
class QTableView;
class SortFilterProxyModel;

class ReadOnlyDelegateBRTable : public ReadOnlyDelegate {
    Q_OBJECT

public:
    ReadOnlyDelegateBRTable(QObject* parent)
        : ReadOnlyDelegate(parent)
    {
    }
    ~ReadOnlyDelegateBRTable() { }

protected:
    virtual void paint(QPainter* painter,
        const QStyleOptionViewItem& option,
        const QModelIndex& index) const override
    {
        QString text = index.model()->data(index, Qt::DisplayRole).toString();

        painter->save();
        /*if (TableHeader::Status == index.column()) {
            BorrowStatus status = (BorrowStatus)index.model()->data(index, Qt::UserRole).toInt();
            if (BorrowStatus::Returned == status) {
                painter->setPen(Qt::green);
                text = TOCH("�ѻ�");
            } else if (BorrowStatus::NotReturned == status) {
                painter->setPen(Qt::gray);
                QFont font = painter->font();
                font.setItalic(true);
                painter->setFont(font);
                text = TOCH("δ��");
            } else {
                painter->setPen(Qt::red);
                text = TOCH("��ʧ");
            }
        } else if (TableHeader::BorrowDate == index.column()) {
            QDateTime date = index.model()->data(index, Qt::UserRole).toDateTime();
            text = date.toString("yyyy-MM-dd");
        } else if (TableHeader::UpdateDate == index.column()) {
            QDateTime date = index.model()->data(index, Qt::UserRole).toDateTime();
            text = date.toString("yyyy-MM-dd hh:mm");
        }*/

        QStyleOptionViewItem view_option(option);
        view_option.displayAlignment = Qt::AlignHCenter | Qt::AlignVCenter;
        if (view_option.state & QStyle::State_Selected) {
            view_option.state = view_option.state ^ QStyle::State_Selected;
            QColor color = index.model()->data(index, Qt::BackgroundRole).value<QColor>();
            painter->fillRect(view_option.rect, QBrush(color));
        }

        QTextOption textOption;
        textOption.setAlignment(Qt::AlignCenter);
        if (painter->fontMetrics().width(text) > view_option.rect.width() - 8) {
            text = painter->fontMetrics().elidedText(text, Qt::ElideRight, view_option.rect.width() - 8);
        }
        painter->drawText(view_option.rect, text, textOption);
        painter->restore();
    }
};

class CustomTableModel : public QStandardItemModel {
    Q_OBJECT

public:
    explicit CustomTableModel(QObject* parent = nullptr) { }
    ~CustomTableModel() {};

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const
    {
        if (!index.isValid())
            return QVariant();

        switch (role) {
        case Qt::BackgroundRole: {
            return CApp->getStyledWidget().backgroundColor();
        }
        }
        return QStandardItemModel::data(index, role);
    }
};

class CUiCenter : public BaseWidget, CTableview::SetDataCallback {
    Q_OBJECT

public:
    CUiCenter(QWidget* parent = Q_NULLPTR);
    ~CUiCenter();

    using ModeDataList = QList<ModelData>;

private:
    friend class CUiTop;
    void initUi();
    void initHeader();
    void initData();
    void initTableView();
    void setData(const QList<ModelData>& model);
    void doExport(QList<ModelData>& datas);
    void updateButtonState(int selectedCount);
    void exportList();
    void doExport(ExportDataView::ExportSetting setting);

    void setAddvertiseLink(const QString& link);
    void openPhoneNumberList();
    void PrintLog(QtMsgType type, const QString& text);
    bool GetCurrentData(QList<ModelData>& selectedRows);
    void excuteTasks(TaskType type);
    bool ShowInputPwdView(QString& password);
    bool ShowInputPhone(QString& phone);
    void parseLocalTaskData(const QJsonObject& dataObj, int index, const QString& taskId);
    void setListRowData(int rowIndex, int column, const QVariant& data);
    QString getTaskTypeString(TaskType type);

protected:
    virtual QList<QStandardItem*> creatRow(const ModelData& model, int index);

private slots:
    void slotContextMenu(const QPoint&);
    void slotTableViewDoubleClicked(const QModelIndex& index);
    void onRequestCallback(const ResponData&);
    void on_bt_modify_pwd_clicked();
    void on_btn_unsecure_clicked();
    void on_btn_account_status_clicked();
    void on_btn_role_clicked();
    void remove();
    void on_btn_send_msg_clicked();
    void on_btn_sync_phone_clicked();
    void on_btn_bind_phone_clicked();
    void OnDropFiles(const QList<QUrl>& listFiles);
    void OnAddRow(ImportData data);
    void OnImportFinished();
    void onTaskDo(const QString& index, const QString& msg, const QString& status);
    void onTaskRequestCallback(const ResponData&, const QString& taskId);
    void onTaskRequestError(const ResponData& data, NetworkRequestError errorType, const QString& errorString);


private:
    Ui::CUiCenter ui;
    ThreadImport taskImport;
    QThread threadImport;
    int importCount = 0;
    ModeDataList listImport;
};
