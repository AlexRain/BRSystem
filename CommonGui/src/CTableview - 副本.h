#pragma once

#include "CApplication.h"
#include "define.h"
#include <QTableView>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QAbstractItemModel>
#include <QCheckBox>
#include <QVariant>
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
			QStyleOptionHeader opt;
			initStyleOption(&opt);
			// setup the style options structure
			QVariant textAlignment = this->model()->headerData(logicalIndex, this->orientation(),
				Qt::TextAlignmentRole);
			opt.rect = rect;
			opt.section = logicalIndex;
			opt.state = m_bIsChecked ? QStyle::State_On : QStyle::State_Off;
			opt.state |= QStyle::State_Enabled;
			opt.textAlignment = Qt::Alignment(textAlignment.isValid()
				? Qt::Alignment(textAlignment.toInt())
				: this->defaultAlignment());

			opt.iconAlignment = Qt::AlignVCenter;
			opt.text = this->model()->headerData(logicalIndex, this->orientation(),
				Qt::DisplayRole).toString();

			int margin = 2 * style()->pixelMetric(QStyle::PM_HeaderMargin, 0, this);

			const Qt::Alignment headerArrowAlignment = static_cast<Qt::Alignment>(style()->styleHint(QStyle::SH_Header_ArrowAlignment, 0, this));
			const bool isHeaderArrowOnTheSide = headerArrowAlignment & Qt::AlignVCenter;
			if (isSortIndicatorShown() && sortIndicatorSection() == logicalIndex && isHeaderArrowOnTheSide)
				margin += style()->pixelMetric(QStyle::PM_HeaderMarkSize, 0, this);

			const QVariant variant = this->model()->headerData(logicalIndex, this->orientation(),
				Qt::DecorationRole);
			opt.icon = qvariant_cast<QIcon>(variant);
			if (opt.icon.isNull())
				opt.icon = qvariant_cast<QPixmap>(variant);
			if (!opt.icon.isNull()) // see CT_HeaderSection
				margin += style()->pixelMetric(QStyle::PM_SmallIconSize, 0, this) +
				style()->pixelMetric(QStyle::PM_HeaderMargin, 0, this);

			if (this->textElideMode() != Qt::ElideNone)
				opt.text = opt.fontMetrics.elidedText(opt.text, this->textElideMode(), rect.width() - margin);

			QVariant foregroundBrush = this->model()->headerData(logicalIndex, this->orientation(),
				Qt::ForegroundRole);
			if (foregroundBrush.canConvert<QBrush>())
				opt.palette.setBrush(QPalette::ButtonText, qvariant_cast<QBrush>(foregroundBrush));

			QPointF oldBO = painter->brushOrigin();
			QVariant backgroundBrush = this->model()->headerData(logicalIndex, this->orientation(),
				Qt::BackgroundRole);
			if (backgroundBrush.canConvert<QBrush>()) {
				opt.palette.setBrush(QPalette::Button, qvariant_cast<QBrush>(backgroundBrush));
				opt.palette.setBrush(QPalette::Window, qvariant_cast<QBrush>(backgroundBrush));
				painter->setBrushOrigin(opt.rect.topLeft());
			}

			// draw the section
			style()->drawControl(QStyle::CE_Header, &opt, painter, this);

			painter->setBrushOrigin(oldBO);
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
