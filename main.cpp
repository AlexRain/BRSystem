#include "BRSystem.h"
#include <QtWidgets/QApplication>
#include <QFile>
#include <QStyleFactory>
#include "define.h"
#include "UiLogin.h"
#include "CStyleManager.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	StyleStruct style = CStyleManager::getInstance().getCurrentStyleStruct();
	StyleHelper::loadAppStyle(style.cssFile);

	/*��¼����*/
	UiLogin login;
	if (QDialog::Accepted != login.exec()) return 0;

	/*������*/
	BRSystem w;
	w.show();
	return a.exec();
}
