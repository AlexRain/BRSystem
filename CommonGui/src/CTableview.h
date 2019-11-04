#pragma once

#include "CApplication.h"
#include "define.h"
#include <QTableView>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QCheckBox>
#include <QDebug>
#include "commongui_global.h"

class QLabel;

typedef struct _HeadStruct{
	QString headText;
	int columnWidth;
	bool isHidden;
}HeadStruct;

class TableModel :public QStandardItemModel {
	Q_OBJECT

public:
	explicit TableModel(QObject *parent = nullptr) {};
	~TableModel() {};

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const {
		if (!index.isValid())
			return QVariant();

		switch (role)
		{
		case Qt::BackgroundRole:
		{
			return CApp->getStyledWidget().backgroundColor();
		}
		}
		return QStandardItemModel::data(index, role);
	}
};

class CHeaderView : public QHeaderView {

public:
	explicit CHeaderView(Qt::Orientation orientation, QWidget *parent = nullptr):QHeaderView(orientation,parent),m_bIsChecked(false) {}
	virtual ~CHeaderView() {};

protected:
	void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const {
		if (!rect.isValid())
			return;
		if (logicalIndex == 0){
			// get the state of the section
			painter->save();
			QStyleOptionHeader opt;
			initStyleOption(&opt);
			style()->drawControl(QStyle::CE_Header, &opt, painter, this);
			painter->restore();

			bool showCheckBox = this->model()->headerData(logicalIndex,this->orientation(), Qt::UserRole).toBool();
			QString text = showCheckBox?TOCH("ȫѡ") : this->model()->headerData(logicalIndex, this->orientation(),
				Qt::DisplayRole).toString();
			if (showCheckBox){
				QStyleOptionButton checkBoxStyle;
				checkBoxStyle.state = m_bIsChecked ? QStyle::State_On : QStyle::State_Off;
				checkBoxStyle.state |= QStyle::State_Enabled;
				checkBoxStyle.iconSize = QSize(20, 20);
				QRect rectBox = QRect(rect.x(), rect.y() + rect.height() / 2 - 10,
					rect.width() / 2, 20);
				checkBoxStyle.rect = rectBox;

				painter->save();
				QCheckBox checkBox;
				style()->drawPrimitive(QStyle::PE_IndicatorCheckBox, &checkBoxStyle, painter, &checkBox);
				painter->restore();

				painter->save();
				painter->setBrush(opt.palette.foreground());
				QTextOption textOption;
				QRect rectText = QRect(rect.x() + rect.width() / 2, rect.y(),
					rect.width() / 2, rect.height());
				textOption.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
				painter->drawText(rectText, text, textOption);
				painter->restore();
			}else {
				painter->save();
				painter->setBrush(opt.palette.foreground());
				QTextOption textOption;
				QRect rectText = QRect(rect.x() + rect.width() / 2, rect.y(),
					rect.width() / 2, rect.height());
				textOption.setAlignment(Qt::AlignCenter);
				painter->drawText(rectText, text, textOption);
				painter->restore();
			}
		}
		else {
			QHeaderView::paintSection(painter, rect, logicalIndex);
		}
	}
private:
	bool m_bIsChecked;
};

class COMMONGUI_EXPORT CTableview : public QTableView
{
	Q_OBJECT

public:
	explicit CTableview(QWidget *parent = nullptr);
	~CTableview();

public:
	typedef struct _SetDataCallback {
		explicit _SetDataCallback() = default;
		~_SetDataCallback() {}

		virtual QList<QStandardItem*> creatRow(const ModelData &model,int index) = 0;
	}SetDataCallback;

public:
	void setHeader(const QList<HeadStruct> &head);
	void setData(const QList<ModelData> &data);
	void setCreatRowCallbackListener(SetDataCallback *listener);

private:
	void init();
	void initHeader();

private:
	TableModel *m_pModel;
	QList<HeadStruct> m_headList;
	QLabel *m_pTip;
	SetDataCallback *m_pSetDataListener;
};
