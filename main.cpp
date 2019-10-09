#include "BRSystem.h"
#include <QtWidgets/QApplication>
#include <QFile>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QFile file(":/qss/qss/global.qss");
	// 只读方式打开该文件
	if (file.open(QFile::ReadOnly)) {
		// 读取文件全部内容，使用tr()函数将其转换为QString类型
		QString styleSheet = QObject::tr(file.readAll());
		// 为QApplication设置样式表
		a.setStyleSheet(styleSheet);
		file.close();
	}

	BRSystem w;
	w.show();
	return a.exec();
}
