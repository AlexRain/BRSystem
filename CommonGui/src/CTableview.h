#pragma once

#include "CApplication.h"
#include "commongui_global.h"
#include "define.h"
#include <QCheckBox>
#include <QDebug>
#include <QHeaderView>
#include <QMouseEvent>
#include <QStandardItemModel>
#include <QTableView>

class QLabel;

typedef struct _HeadStruct {
    QString headText;
    int columnWidth = -1;
    bool isHidden = false;
    QString key;
} HeadStruct;

class TableModel : public QStandardItemModel {
    Q_OBJECT

public:
    explicit TableModel(QObject* parent = nullptr) {};
    ~TableModel() {};

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

class CHeaderView : public QHeaderView {

    Q_OBJECT

public:
    explicit CHeaderView(Qt::Orientation orientation, QWidget* parent = nullptr)
        : QHeaderView(orientation, parent)
        , m_bIsChecked(false)
    {
    }
    virtual ~CHeaderView() {};

    void setSectionCheckBoxEnable(int logicalIndex, bool enable)
    {
        this->checkBoxEnable[logicalIndex] = enable;
    }

protected:
    void paintSection(QPainter* painter, const QRect& rect, int logicalIndex) const
    {
        if (!rect.isValid())
            return;
        if (checkBoxEnable.contains(logicalIndex) && checkBoxEnable[logicalIndex]) {
            // get the state of the section
            painter->save();
            QStyleOptionHeader opt;
            initStyleOption(&opt);
            style()->drawControl(QStyle::CE_Header, &opt, painter, this);
            painter->restore();

            bool showCheckBox = this->model()->headerData(logicalIndex, this->orientation(), Qt::UserRole).toBool();
            QString text = showCheckBox ? TOCH("ȫѡ") : this->model()->headerData(logicalIndex, this->orientation(), Qt::DisplayRole).toString();
            if (showCheckBox) {
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
            } else {
                painter->save();
                painter->setBrush(opt.palette.foreground());
                QTextOption textOption;
                textOption.setAlignment(Qt::AlignCenter);
                painter->drawText(rect, text, textOption);
                painter->restore();
            }
        } else {
            QHeaderView::paintSection(painter, rect, logicalIndex);
        }
    }

    void mousePressEvent(QMouseEvent* event)
    {
        if (event->button() == Qt::LeftButton) {
            int index = logicalIndexAt(event->pos());
            bool showCheckBox = this->model()->headerData(index, this->orientation(), Qt::UserRole).toBool();
            if (checkBoxEnable.contains(index) && checkBoxEnable[index] && showCheckBox) {
                m_bIsChecked = !m_bIsChecked;
                emit checkStateChanged(m_bIsChecked);
            }
        }
        this->update();
        QHeaderView::mousePressEvent(event);
    }

signals:
    void checkStateChanged(bool state);

private:
    bool m_bIsChecked;
    QMap<int, bool> checkBoxEnable;
};

class COMMONGUI_EXPORT CTableview : public QTableView {
    Q_OBJECT

public:
    explicit CTableview(QWidget* parent = nullptr);
    ~CTableview();

public:
    typedef struct _SetDataCallback {
        explicit _SetDataCallback() = default;
        ~_SetDataCallback() { }

        virtual QList<QStandardItem*> creatRow(const ModelData& model, int index) = 0;
    } SetDataCallback;

public:
    void setHeader(const QList<HeadStruct>& head);
    void setData(const QList<ModelData>& data);
    void addData(const ModelData& data);
    void setCreatRowCallbackListener(SetDataCallback* listener);
    void setHeaderCheckBoxEnable(int logicalIndex, bool enable);
    void showMutipleSelectionMode();
    void exitMutipleSelectionMode();
    void setItemDelegateForColumn(int column, QAbstractItemDelegate* delegate);
    void setNodaTips(const QString &tips);

protected:
    virtual void currentChanged(const QModelIndex& current, const QModelIndex& previous) override;
    virtual void dragEnterEvent(QDragEnterEvent* event) override;
    virtual void dragMoveEvent(QDragMoveEvent* event) override;
    virtual void dragLeaveEvent(QDragLeaveEvent* event) override;
    virtual void dropEvent(QDropEvent* event) override;

private:
    void init();
    void initHeader();

signals:
    void selectionRowChanged(int);
    void currentIndexChanged(const QModelIndex& current, const QModelIndex& previous);
    void dropFiles(const QList<QUrl>& listFiles);

private:
    TableModel* m_pModel;
    CHeaderView* m_pHeaderView;
    QList<HeadStruct> m_headList;
    QLabel* m_pTip;
    SetDataCallback* m_pSetDataListener;
    QMap<int, bool> checkBoxColumn;
};
