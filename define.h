#ifndef DEFINE
#define DEFINE

#include <QFile>
#include <QApplication>
#include <QDate>
#include <QLabel>
#include <QWidget>
#include <QSize>
#include <QPushButton>

#define TOCH(str) QString::fromLocal8Bit(str)
#define CONFIG_FILE "app.ini"

enum TableHeader
{
	Order = 0,
	ProductionId,
	ProductionName,
	BorrowerName,
	BorrowDate,
	Status,
	BorrowReason,
	Remark,
	UpdateDate
};

enum BorrowStatus
{
	Returned = 0,/*�ѻ�*/
	NotReturned, /*δ��*/
	Lost /*��ʧ*/
};
typedef std::function<void()> Func_Void;

enum StyleType
{
	Dark = 0,
	White,
	Yellow
};

typedef struct _SkinStyle
{
	StyleType type;
	QString styleName;
	QString cssFile;
	QColor itemColor;
}StyleStruct;

/*������Ϣ�ṹ��*/
typedef struct _BorrowInfo
{
	int	order;
	QString productionId;
	QString productionName;
	QString borrowerName;
	QDateTime borrowDate;
	QDateTime updateDate;
	BorrowStatus borrowStatus;

	QString borrowReason;
	QString remarks;
}BorrowInfo;

namespace StyleHelper {
	static void loadAppStyle(const QString &qssFile) {
		QFile file(qssFile);
		// ֻ����ʽ�򿪸��ļ�
		if (file.open(QFile::ReadOnly)) {
			// ��ȡ�ļ�ȫ�����ݣ�ʹ��tr()��������ת��ΪQString����
			qApp->setStyleSheet(file.readAll());
			file.close();
		}
	}

	static void initAppStyle() {
		qApp->setStyleSheet("");
	}
};

namespace UiHelper {
	static QLabel *createSplitter(QWidget *parent) {
		QLabel *pSp = new QLabel(parent);
		pSp->setObjectName("divider");
		pSp->setFixedHeight(1);
		return pSp;
	}

	/*����һ����ť�����󶨵���¼����õ���������*/
	static QPushButton *creatPushButton(QWidget *parent,const Func_Void &func, QSize &size = QSize(0,0), const QString &text = QString(), const QString &objectName = QString()) {
		QPushButton *btn = new QPushButton(parent);
		btn->setText(text);
		QObject::connect(btn, &QPushButton::clicked, func);
		btn->setCursor(Qt::PointingHandCursor);
		btn->setObjectName(objectName);
		if (size.width() > 0 &&size.height() > 0)
			btn->setFixedSize(size);
		return btn;
	}

	/*����һ����ť�����󶨵���¼����õ���������*/
	static QPushButton *creatPushButton(QWidget *parent, const Func_Void & func,int width = 0,int height = 0, const QString &text = QString(),const QString &objectName = QString()) {
		QPushButton *btn = new QPushButton(parent);
		btn->setText(text);
		QObject::connect(btn, &QPushButton::clicked, func);
		btn->setCursor(Qt::PointingHandCursor);
		btn->setObjectName(objectName);
		if (width > 0 && height > 0)
			btn->setFixedSize(width,height);
		return btn;
	}
};

#endif // DEFINE

