#include "BRSystem.h"
#include <QtWidgets/QApplication>
#include <QFile>
#include <QStyleFactory>
#include "define.h"
#include "UiLogin.h"
#include "CStyleManager.h"
#include "UiFrostedLayer.h"
#include "PopupDialogContainer.h"
#include "CApplication.h"

int main(int argc, char *argv[])
{
	CApplication a(argc, argv);

	a.setWindowIcon(QIcon("images/app.ico"));
	a.setApplicationName(TOCH("�����Ƽ�����ר�ý軹ϵͳ"));

	StyleStruct style = CStyleManager::getInstance().getCurrentStyleStruct();
	StyleHelper::loadAppStyle(style.cssFile);

	/*��¼����*/
	//UiLogin login;
	//if (QDialog::Accepted != login.fadeIn()) return 0;

	/*������*/
	BRSystem w;
	a.setMainWidget(&w);
	PopupDialogContainer::showPopupDialogFadeIn(&w, nullptr, "",false,false);
	return a.exec();
}
