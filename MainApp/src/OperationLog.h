#pragma once

#include "BaseWidget.h"
#include "ui_OperationLog.h"
#include "CDbHelper.h"
#include <QPainter>

class QStandardItemModel;
class QPushButton;
class QLabel;

class OperationLog : public BaseWidget
{
	Q_OBJECT

	enum Header
	{
		Order,
		ProductionId,
		ProductionName,
		OperationDetail,
		OperationUser,
		OperationDateTime,
		IouId
	};

public:
	OperationLog(QWidget *parent = Q_NULLPTR);
	~OperationLog();

public:
	void refreshData();

private:
	void initData();
	void initTableview();
	void initHeader();
	void initColumn();
	void appendRow(const ModelData &info,int index);
	void setData(const QList<ModelData> &datas);

protected:
	void resizeEvent(QResizeEvent *event);

private:
	Ui::OperationLog ui;
	QStandardItemModel *m_pModel;
	QPushButton *m_pBtnRefresh;
	QLabel *m_pTip;
};
