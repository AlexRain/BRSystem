#pragma once

#include <QDialog>
#include <QMessageBox>
#include <QPointer>
#include "BaseWidget.h"
#include "ui_DialogMsg.h"
#include "commongui_global.h"


class UiFrostedLayer;

class COMMONGUI_EXPORT DialogMsg : public QDialog
{
	Q_OBJECT

public:
	DialogMsg(QWidget *parent, const QString &title, const QString &text,
		QMessageBox::StandardButtons buttons, const QStringList &btnText);
	~DialogMsg();

public:
	static QMessageBox::StandardButton question(QWidget *parent,const QString &title,const QString &text,
		QMessageBox::StandardButtons buttons = QMessageBox::StandardButtons(QMessageBox::Yes | QMessageBox::No),const QStringList &btnText = QStringList());

public:
	void showLayer();
	void hideLayer();

private slots:
	void slotButtonClicked();
	void slotClose();

protected:
	void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent *event);

private:
	Ui::DialogMsg ui;
	QMessageBox::StandardButtons mButtonClicked;
	QStringList mButtonsText;
	QList<QPushButton*> mButtonsPointer;
	QLabel *_pTitle;
	QPushButton *_pBtnClose;
	QString mTitle;
	QString mContent;
	QMessageBox::StandardButtons mButtons;



	QPointer<QWidget> mParentWidget;
	UiFrostedLayer *mPLayer;
};
