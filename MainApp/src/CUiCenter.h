#pragma once

#include "BaseWidget.h"
#include "CApplication.h"
#include "CDbHelper.h"
#include "CTableview.h"
#include "NetworkDefine.h"
#include "ReadOnlyDelegate.h"
#include "define.h"
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
        if (TableHeader::Status == index.column()) {
            BorrowStatus status = (BorrowStatus)index.model()->data(index, Qt::UserRole).toInt();
            if (BorrowStatus::Returned == status) {
                painter->setPen(Qt::green);
                text = TOCH("ÒÑ»¹");
            } else if (BorrowStatus::NotReturned == status) {
                painter->setPen(Qt::gray);
                QFont font = painter->font();
                font.setItalic(true);
                painter->setFont(font);
                text = TOCH("Î´»¹");
            } else {
                painter->setPen(Qt::red);
                text = TOCH("¶ªÊ§");
            }
        } else if (TableHeader::BorrowDate == index.column()) {
            QDateTime date = index.model()->data(index, Qt::UserRole).toDateTime();
            text = date.toString("yyyy-MM-dd");
        } else if (TableHeader::UpdateDate == index.column()) {
            QDateTime date = index.model()->data(index, Qt::UserRole).toDateTime();
            text = date.toString("yyyy-MM-dd hh:mm");
        }

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

private:
    void initUi();
    void initHeader();
    void initData();
    void initTableView();
    void sqlQuery(const QString& sql);
    void sqlUpdate(const ModelData& model, const QString& id);
    void sqlDelete(const BorrowInfo& info);
    void sqlDelete(const QList<BorrowInfo>& infos);
    void FuzzyQuery(const QString& info = QString());
    void setData(const QList<ModelData>& model);
    void showDetailUi(const QModelIndex& index);
    void sqlQueryExport();
    void doExport(QList<ModelData>& datas);
    void updateCountLabel();

    void getBorrowData(BorrowInfo& info, int row);
    void getBorrowData(ModelData& info, int row);
    void setBorrowData(const BorrowInfo& info, int row);

protected:
    virtual QList<QStandardItem*> creatRow(const ModelData& model, int index);

private slots:
    void slotContextMenu(const QPoint&);
    void slotTableViewDoubleClicked(const QModelIndex& index);
    void onRequestCallback(const ResponData&);

private:
    CSearchLineEdit* mLineEdit;
    CTableview* mTableView;
    QLabel* m_pDataCount;
};
