#include "BRSystem.h"
#include <QtWidgets/QApplication>
#include <QFile>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QFile file(":/qss/qss/global.qss");
	// ֻ����ʽ�򿪸��ļ�
	if (file.open(QFile::ReadOnly)) {
		// ��ȡ�ļ�ȫ�����ݣ�ʹ��tr()��������ת��ΪQString����
		QString styleSheet = QObject::tr(file.readAll());
		// ΪQApplication������ʽ��
		a.setStyleSheet(styleSheet);
		file.close();
	}

	BRSystem w;
	w.show();
	return a.exec();
}
