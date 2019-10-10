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

#endif // DEFINE

