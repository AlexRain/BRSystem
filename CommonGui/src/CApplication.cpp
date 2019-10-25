#include "CApplication.h"

CApplication::CApplication(int argc, char ** argv)
	: QApplication(argc,argv)
{
}

CApplication::~CApplication()
{
}

void CApplication::setMainWidget(QWidget*widget)
{
	this->mainWidget = widget;
}

QWidget* CApplication::getMainWidget()
{
	return this->mainWidget;
}

CApplication* CApplication::GetApplication()
{
	CApplication* cApp = static_cast<CApplication*>(qApp);
	return cApp;
}

const StyledWidget & CApplication::getStyledWidget() const
{
	return this->_styedWidget;
}
