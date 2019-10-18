#pragma once

#include <QDialog>
#include <QMessageBox>
#include "BaseWidget.h"
#include "ui_DialogMsg.h"

class DialogMsg : public QDialog
{
	Q_OBJECT

public:
	DialogMsg(QWidget *parent, const QString &title, const QString &text,
		QMessageBox::StandardButtons buttons, const QStringList &btnText);
	~DialogMsg();

public:
	static QMessageBox::StandardButton question(QWidget *parent,const QString &title,const QString &text,
		QMessageBox::StandardButtons buttons = QMessageBox::StandardButtons(QMessageBox::Yes | QMessageBox::No),const QStringList &btnText = QStringList());

private slots:
	void slotButtonClicked();
	void on_btn_close_clicked();

protected:
	void paintEvent(QPaintEvent *event);

private:
	Ui::DialogMsg ui;
	QMessageBox::StandardButtons mButtonClicked;
	QStringList mButtonsText;
	QList<QPushButton*> mButtonsPointer;
	QString mTitle;
	QString mContent;
	QMessageBox::StandardButtons mButtons;
};
