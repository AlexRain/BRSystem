#ifndef DEFINE
#define DEFINE

#include <QFile>
#include <QApplication>

#define TOCH(str) QString::fromLocal8Bit(str)

enum TableHeader
{
	Order = 0,
	ProductionId,
	ProductionName,
	BorrowerName,
	BorrowDate,
	Status
};

namespace StyleHelper {
	static void setAppStyle(const QString &qssFile) {
		QFile file(qssFile);
		// 只读方式打开该文件
		if (file.open(QFile::ReadOnly)) {
			// 读取文件全部内容，使用tr()函数将其转换为QString类型
			qApp->setStyleSheet(file.readAll());
			file.close();
		}
	}

	static void initAppStyle() {
		qApp->setStyleSheet("");
	}
};

#endif // DEFINE

