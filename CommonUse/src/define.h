#ifndef DEFINE
#define DEFINE

#include <QApplication>
#include <QDate>
#include <QDir>
#include <QFile>
#include <QLabel>
#include <QPainter>
#include <QPushButton>
#include <QSize>
#include <QStyle>
#include <QWidget>
#include <QStandardPaths>

#define TOCH(str) QString::fromLocal8Bit(str)
#define CONFIG_FILE "app.ini"
#define USER_CONFIG_FILE "user.ini"
#define DAIL_SETTING "yunguo/dail_setting.ini"

static const char* PY_SERVER_EXE = "yunguo/hl-py.exe";

static const int RESTART_CODE = 1;

typedef QMap<QString, QString> ModelData;

enum class AccountStatus {
    Forbidden = 2,
    SafeMode = 1,
    Normal = 0
};

enum TableHeader {
    UniqueId = 0,
    Order,
    ProductionId,
    ProductionName,
    BorrowerName,
    BorrowDate,
    Status,
    BorrowReason,
    Remark,
    UpdateDate
};

enum TableAcocountList {
    uid = 0,
    index,
    qqNumber,
    password,
    phoneNumber,
    newPhoneNumber,
    task_status,
    bizType,
    status
   
};

enum BorrowStatus {
    Returned = 0, /*�ѻ�*/
    NotReturned, /*δ��*/
    Lost /*��ʧ*/
};
typedef std::function<void()> Func_Void;

enum StyleType {
    Dark = 0,
    White,
    Yellow
};

typedef struct _SkinStyle {
    StyleType type;
    QString styleName;
    QString cssFile;
    QColor itemColor;
} StyleStruct;

/*������Ϣ�ṹ��*/
typedef struct _BorrowInfo {
    int order;
    QString id;
    QString productionId;
    QString productionName;
    QString borrowerName;
    QDateTime borrowDate;
    QDateTime updateDate;
    BorrowStatus borrowStatus;

    QString borrowReason;
    QString remarks;
} BorrowInfo;

namespace StyleHelper {
static void loadAppStyle(const QString& qssFile)
{
    QFile file(qssFile);
    // ֻ����ʽ�򿪸��ļ�
    if (file.open(QFile::ReadOnly)) {
        // ��ȡ�ļ�ȫ�����ݣ�ʹ��tr()��������ת��ΪQString����
        qApp->setStyleSheet(file.readAll());
        file.close();
    }
}

static void initAppStyle()
{
    qApp->setStyleSheet("");
}
};

static QString GetAppDataLocation() {
    QString strPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(strPath);
    if (!dir.exists()) {
        dir.mkpath(strPath);
    }
    return strPath;
}

namespace UiHelper {
static QLabel* createSplitter(QWidget* parent)
{
    QLabel* pSp = new QLabel(parent);
    pSp->setObjectName("divider");
    pSp->setFixedHeight(1);
    return pSp;
}

/*���ݹ̶��ߴ�С����ȡ���ʵ�ͼ��*/
static QPixmap justPixmapByHeight(int height, const QPixmap& pixIn)
{
    double ratio_src = pixIn.width() / (double)pixIn.height();
    int width_src = height * ratio_src;
    return pixIn.scaled(width_src, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

static QPixmap justPixmapByWidth(int width, const QPixmap& pixIn)
{
    double ratio_src = pixIn.width() / (double)pixIn.height();
    int height_src = width * ratio_src;
    return pixIn.scaled(width, height_src, Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

/*����һ����ť�����󶨵���¼����õ���������*/
static QPushButton* creatPushButton(QWidget* parent, const Func_Void& func, QSize& size = QSize(0, 0), const QString& text = QString(), const QString& objectName = QString())
{
    QPushButton* btn = new QPushButton(parent);
    btn->setText(text);
    QObject::connect(btn, &QPushButton::clicked, func);
    btn->setCursor(Qt::PointingHandCursor);
    btn->setObjectName(objectName);
    if (size.width() > 0 && size.height() > 0)
        btn->setFixedSize(size);
    return btn;
}

/*����һ����ť�����󶨵���¼����õ���������*/
static QPushButton* creatPushButton(QWidget* parent, const Func_Void& func, int width = 0, int height = 0, const QString& text = QString(), const QString& objectName = QString())
{
    QPushButton* btn = new QPushButton(parent);
    btn->setText(text);
    QObject::connect(btn, &QPushButton::clicked, func);
    btn->setCursor(Qt::PointingHandCursor);
    btn->setObjectName(objectName);
    if (width > 0 && height > 0)
        btn->setFixedSize(width, height);
    return btn;
}

static void PaintPixmapFrame8(QPixmap& pixMap, QList<QPixmap>& listPixmap, int nL = 4, int nT = 4, int nR = 4, int nB = 4)
{
    if (listPixmap.size() == 8) {
        int nBY1 = nT; // �߿�߶�1
        int nBY2 = nB; // �߿�߶�2
        int nBX1 = nL; // �߿���1
        int nBX2 = nR; // �߿���2

        int nW = pixMap.width();
        int nH = pixMap.height();

        QPainter painter(&pixMap);

        painter.setBrush(QBrush(QColor(255, 255, 255, 0)));
        int i = 0;
        if (!listPixmap[i].isNull()) { //����
            painter.drawPixmap(QRect(0, 0, nBX1, nBY1), listPixmap[i]);
        }
        i++;
        if (!listPixmap[i].isNull()) { //����
            painter.drawPixmap(QRect(nBX1, 0, nW - nBX1 - nBX2, nBY1), listPixmap[i]);
        }
        i++;
        if (!listPixmap[i].isNull()) { //����
            painter.drawPixmap(QRect(nW - nBX2, 0, nBX2, nBY1), listPixmap[i]);
        }
        i++;
        if (!listPixmap[i].isNull()) { //����
            painter.drawPixmap(QRect(nW - nBX2, nBY1, nBX2, nH - nBY1 - nBY2), listPixmap[i]);
        }
        i++;
        if (!listPixmap[i].isNull()) { //����
            painter.drawPixmap(QRect(nW - nBX2, nH - nBY2, nBX2, nBY2), listPixmap[i]);
        }
        i++;
        if (!listPixmap[i].isNull()) { //����
            painter.drawPixmap(QRect(nBX1, nH - nBY2, nW - nBX1 - nBX2, nBY2), listPixmap[i]);
        }
        i++;
        if (!listPixmap[i].isNull()) { //����
            painter.drawPixmap(QRect(0, nH - nBY2, nBX1, nBY2), listPixmap[i]);
        }
        i++;
        if (!listPixmap[i].isNull()) { //����
            painter.drawPixmap(QRect(0, nBY1, nBX1, nH - nBY1 - nBY2), listPixmap[i]);
        }
    }
}

static void flushStyle(QWidget* widget)
{
    if (widget) {
        widget->style()->unpolish(widget);
        widget->style()->polish(widget);
    }
}
};

#endif // DEFINE
