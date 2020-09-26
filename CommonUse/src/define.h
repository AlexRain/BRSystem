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
    Returned = 0, /*已还*/
    NotReturned, /*未还*/
    Lost /*丢失*/
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

/*借条信息结构体*/
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
    // 只读方式打开该文件
    if (file.open(QFile::ReadOnly)) {
        // 读取文件全部内容，使用tr()函数将其转换为QString类型
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

/*根据固定边大小，获取合适的图像*/
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

/*构建一个按钮，并绑定点击事件调用的匿名函数*/
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

/*构建一个按钮，并绑定点击事件调用的匿名函数*/
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
        int nBY1 = nT; // 边框高度1
        int nBY2 = nB; // 边框高度2
        int nBX1 = nL; // 边框宽度1
        int nBX2 = nR; // 边框宽度2

        int nW = pixMap.width();
        int nH = pixMap.height();

        QPainter painter(&pixMap);

        painter.setBrush(QBrush(QColor(255, 255, 255, 0)));
        int i = 0;
        if (!listPixmap[i].isNull()) { //左上
            painter.drawPixmap(QRect(0, 0, nBX1, nBY1), listPixmap[i]);
        }
        i++;
        if (!listPixmap[i].isNull()) { //上中
            painter.drawPixmap(QRect(nBX1, 0, nW - nBX1 - nBX2, nBY1), listPixmap[i]);
        }
        i++;
        if (!listPixmap[i].isNull()) { //右上
            painter.drawPixmap(QRect(nW - nBX2, 0, nBX2, nBY1), listPixmap[i]);
        }
        i++;
        if (!listPixmap[i].isNull()) { //右中
            painter.drawPixmap(QRect(nW - nBX2, nBY1, nBX2, nH - nBY1 - nBY2), listPixmap[i]);
        }
        i++;
        if (!listPixmap[i].isNull()) { //右下
            painter.drawPixmap(QRect(nW - nBX2, nH - nBY2, nBX2, nBY2), listPixmap[i]);
        }
        i++;
        if (!listPixmap[i].isNull()) { //下中
            painter.drawPixmap(QRect(nBX1, nH - nBY2, nW - nBX1 - nBX2, nBY2), listPixmap[i]);
        }
        i++;
        if (!listPixmap[i].isNull()) { //左下
            painter.drawPixmap(QRect(0, nH - nBY2, nBX1, nBY2), listPixmap[i]);
        }
        i++;
        if (!listPixmap[i].isNull()) { //左中
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
