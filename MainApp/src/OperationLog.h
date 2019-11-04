#pragma once

#include "BaseWidget.h"
#include "CDbHelper.h"
#include "CTableview.h"
#include <QPainter>

class QStandardItemModel;
class QPushButton;
class QLabel;
class CTableview;

class OperationLog : public BaseWidget,CTableview::SetDataCallback
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
	void initHeader();
	void initData();
	void initColumn();
	void setData(const QList<ModelData> &datas);

protected:
	void resizeEvent(QResizeEvent *event);
	QList<QStandardItem*> creatRow(const ModelData &model,int index) override;

private:
	CTableview *m_pTableView;
	QPushButton *m_pBtnRefresh;	
	QLabel *m_pLabelCount;
};
