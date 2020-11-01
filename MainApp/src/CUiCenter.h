#pragma once

#include "BaseWidget.h"
#include "CApplication.h"
#include "CDbHelper.h"
#include "CTableview.h"
#include "ExportSettingView.h"
#include "SelectGoodsView.h"
#include "NetworkDefine.h"
#include "ReadOnlyDelegate.h"
#include "CheckBoxDelegate.h"
#include "TaskManager.h"
#include "ThreadImport.h"
#include "define.h"
#include <QModelIndex>
#include <QPainter>
#include <QStandardItemModel>
#include <QWidget>
#include "WebSocketClientManager.h"
#include "ui_CUiCenter.h"
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


static QRect CheckBoxRect(const QStyleOptionViewItem &viewItemStyleOptions)/*const*/
{
	//绘制按钮所需要的参数
	QStyleOptionButton checkBoxStyleOption;
	//按照给定的风格参数 返回元素子区域
	QRect checkBoxRect = QApplication::style()->subElementRect(QStyle::SE_CheckBoxIndicator, &checkBoxStyleOption);
	//返回QCheckBox坐标
	QPoint checkBoxPoint(viewItemStyleOptions.rect.x() + viewItemStyleOptions.rect.width() / 2 - checkBoxRect.width() / 2,
		viewItemStyleOptions.rect.y() + viewItemStyleOptions.rect.height() / 2 - checkBoxRect.height() / 2);
	//返回QCheckBox几何形状
	return QRect(checkBoxPoint, checkBoxRect.size());
}

class CheckBoxDelegateBRTable : public CheckBoxDelegate {
    Q_OBJECT

public:
	CheckBoxDelegateBRTable(QObject* parent)
        : CheckBoxDelegate(parent)
    {
    }
    ~CheckBoxDelegateBRTable() { }


protected:
    virtual 	void paint(QPainter *painter,
		const QStyleOptionViewItem &option,
		const QModelIndex &index) const override
    {
		QStyleOptionViewItem  view_option(option);
		view_option.displayAlignment = Qt::AlignHCenter | Qt::AlignVCenter;
		if (view_option.state & QStyle::State_Selected) {
			view_option.state = view_option.state ^ QStyle::State_Selected;
			QColor color = index.model()->data(index, Qt::BackgroundRole).value<QColor>();
			painter->fillRect(view_option.rect, color);
		}

		if (index.column() == TableAcocountList::checkBox) {
			bool data = index.model()->data(index, Qt::CheckStateRole).toBool();
			//bool showCheckBox = index.model()->data(index, Qt::UserRole).toBool();
			bool showCheckBox = true;
			QString text = QString::number(index.row() + 1);
			/*qDebug() << "is showCheckBox index" << index;
			qDebug() << "is showCheckBox" << showCheckBox;
			qDebug() << "is showCheckBox data " << data;*/
			if (showCheckBox)
			{
				QStyleOptionButton checkBoxStyle;
				checkBoxStyle.state = data ? QStyle::State_On : QStyle::State_Off;
				checkBoxStyle.state |= QStyle::State_Enabled;
				checkBoxStyle.iconSize = QSize(30, 30);
				//QRect rectBox = QRect(option.rect.x(), option.rect.y() + option.rect.height() / 2 - 10,option.rect.width() / 2, 20);
				QRect rectBox = CheckBoxRect(option);
				checkBoxStyle.rect = rectBox;

				QCheckBox checkBox;
				QApplication::style()->drawPrimitive(QStyle::PE_IndicatorCheckBox, &checkBoxStyle, painter, &checkBox);
			}

			//QTextOption textOption;
			//QRect rectText = QRect(option.rect.x() + option.rect.width() / 2, option.rect.y(),
			//	option.rect.width() / 2, option.rect.height());
			//textOption.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
			//painter->drawText(rectText, text, textOption);
		}
		else {
			QStyledItemDelegate::paint(painter, option, index);
		}
    }

	bool CheckBoxDelegateBRTable::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
	{
		QRect decorationRect = option.rect;

		QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
		if (event->type() == QEvent::MouseButtonPress && decorationRect.contains(mouseEvent->pos())
			&& Qt::LeftButton == mouseEvent->button())
		{
			if (index.column() == TableAcocountList::checkBox)
			{
				bool data = model->data(index, Qt::CheckStateRole).toBool();
				model->setData(index, !data, Qt::CheckStateRole);
			}
		}

		return QStyledItemDelegate::editorEvent(event, model, option, index);
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

class CUiCenter : public QWidget, CTableview::SetDataCallback {
    Q_OBJECT

public:
    CUiCenter(QWidget* parent = Q_NULLPTR);
    virtual ~CUiCenter();


    void PrintLog(QtMsgType type, const QString& text);

    using ModeDataList = QList<ModelData>;


    enum TaskType2 {
        change_password2 = 1,
        unpack_safe_mode2,
        bind_mobile2,
        query_role2,
        query_identity2,
        query_ban2,
        query_credit_score2,
        send_short_message2
    };

    Q_ENUM(TaskType2);

private:
    friend class CUiTop;
    void initUi();
    void initHeader();
    void initData();
    void initTableView();
    void setData(const QList<ModelData>& model);
    void doExport(QList<ModelData>& datas);
    void updateButtonState(int selectedCount);
    void doExport(ExportSettingView::ExportSetting setting);
	void getExportList(QJsonArray accounts, ExportSettingView::ExportSetting exportSetting);
    void setAddvertiseLink(const QString& link);
    void openPhoneNumberList();
	bool GetCurrentSelectData(QList<ModelData>& selectedRows);
	bool GetCurrentData(QList<ModelData>& selectedRows);
    ModelData getRowData(QAbstractItemModel* model, int row);
    void excuteSingleTasks(ModelData data);
    void excuteTasks(TaskType type);
    void doTask(ModelData data,QString newPassword, int bizType);
    bool ShowInputPwdView(QString& password);
    bool ShowInputPhone(QString& phone);
    bool ShowInputIdentityView();
    void parseLocalTaskData(const QJsonObject& dataObj, int index, const QString& taskId);
    void setListRowData(int rowIndex, int column, const QVariant& data);
    QString getTaskTypeString(TaskType type);

    QString getTaskStatusString(TaskStatus taskStatus);

    void addTableMenuActionSolt(QMenu* menu, QString text, const char* method, bool disabled=false);
    void AddSignleRow(ImportData data);

	ExportSettingView::ExportSetting exportSetting;
protected:
    virtual QList<QStandardItem*> creatRow(const ModelData& model, int index);

signals:
    void onTaskPause(const bool&);
    void onThreadPause(const bool&);
    void onRestartTask();

private slots:
	void exportFile(QVariantList& list);
	void exportList();
    void loseConnection();
    void pauseTask();
    void onQueueSize(const int);
    void slotContextMenu(const QPoint&);
    void slotTableViewDoubleClicked(const QModelIndex& index);
    void onRequestCallback(const ResponData&);
	void selectTableAll();
	void cancelSelectTableAll();
	void selectOrder();
    void restartTask();
    void resetTask();
	void selectDimiss();
	void resetAllTask();
    void on_btn_xiancheng_save_clicked();
    void on_btn_single_button_clicked();
    void on_btn_modify_pwd_clicked();
    void on_btn_release_safe_model_clicked();
    void on_btn_query_ban_clicked();
    void on_btn_role_clicked();
	void on_btn_query_quick_role_clicked();
    void on_btn_query_score_clicked();
    void on_btn_query_identity_clicked();
	void on_btn_query_point_clicked();
	void on_btn_update_identity_clicked();
	void on_btn_buy_goods_clicked();
	void removeAll();
    void remove();
    void on_btn_send_msg_clicked();
    void on_btn_sync_phone_clicked();
    void on_btn_bind_phone_clicked();
    //export file
    void OnDropFiles(const QList<QUrl>& listFiles);
    void OnAddRow(ImportData data);
    void bindPlatformAll();
    void bindPlatform();
    void OnImportFinished();
    void onTaskDo(const int index, const QString msg, const int status, const int bizType=0);
    void showPyMsg(const int index, const QString qq, const QString val,const int status = 0);
    void onTaskRequestCallback(const ResponData&, const QString& taskId);
    void onTaskRequestError(const ResponData& data, NetworkRequestError errorType, const QString& errorString);
    void importLastFile();
    void getUserWallet();

private:
    Ui::CUiCenter ui;
    ThreadImport taskImport;
    QThread threadImport;
    int importCount = 0;
    ModeDataList listImport;
    QSet<QString> accountSet;
    QMap<QString,QString> exportModeDataStatusMap;
    QMenu* menu;
    WebSocketClientManager* wsClient;
	QJsonObject goodsReq;
};
