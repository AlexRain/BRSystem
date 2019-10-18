#include "BRSystem.h"
#include <QtWidgets/QApplication>
#include <QFile>
#include <QStyleFactory>
#include "define.h"
#include "UiLogin.h"
#include "CStyleManager.h"
#include "UiFrostedLayer.h"
#include "PopupDialogContainer.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	a.setWindowIcon(QIcon("images/app.ico"));
	a.setApplicationName(TOCH("汇声科技生产专用借还系统"));

	StyleStruct style = CStyleManager::getInstance().getCurrentStyleStruct();
	StyleHelper::loadAppStyle(style.cssFile);

	/*登录界面*/
	//UiLogin login;
	//if (QDialog::Accepted != login.exec()) return 0;

	/*主界面*/
	/*BRSystem w;
	PopupDialogContainer::showPopupDialog(&w, nullptr, false,false);*/
	UiFrostedLayer w;
	w.show();
	return a.exec();
}
