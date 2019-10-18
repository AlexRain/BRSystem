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
	a.setApplicationName(TOCH("�����Ƽ�����ר�ý軹ϵͳ"));

	StyleStruct style = CStyleManager::getInstance().getCurrentStyleStruct();
	StyleHelper::loadAppStyle(style.cssFile);

	/*��¼����*/
	//UiLogin login;
	//if (QDialog::Accepted != login.exec()) return 0;

	/*������*/
	/*BRSystem w;
	PopupDialogContainer::showPopupDialog(&w, nullptr, false,false);*/
	UiFrostedLayer w;
	w.show();
	return a.exec();
}
