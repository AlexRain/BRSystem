#pragma once

#include <QApplication>
#define CApp CApplication::GetApplication()

class CApplication : public QApplication
{
	Q_OBJECT

public:
	CApplication(int argc, char ** argv);
	~CApplication();

public:
	void setMainWidget(QWidget*);
	QWidget* getMainWidget();
	static CApplication* GetApplication();

private:
	QWidget *mainWidget;
};
