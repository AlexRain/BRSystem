#include "BRSystem.h"
#include <QtWidgets/QApplication>
#include <QFile>
#include <QStyleFactory>
#include "define.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	StyleHelper::setAppStyle("qss/global_white.qss");
	BRSystem w;
	w.show();
	return a.exec();
}
