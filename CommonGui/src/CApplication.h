#pragma once

#include <QApplication>
#include "commongui_global.h"
#include "StyledWidget.h"

#define CApp CApplication::GetApplication()

class COMMONGUI_EXPORT CApplication : public QApplication
{
	Q_OBJECT

public:
	CApplication(int argc, char ** argv);
	~CApplication();

public:
	void setMainWidget(QWidget*);
	QWidget* getMainWidget();
	static CApplication* GetApplication();
	const StyledWidget &getStyledWidget() const;

private:
	QWidget *mainWidget;
	StyledWidget _styedWidget;
};
