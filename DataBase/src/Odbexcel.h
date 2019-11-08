#pragma once

#include <QObject>
#include <QTableView>
#include <QString>
#include <QSqlError>
#include <QStringList>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "database_global.h"


class DATABASE_EXPORT Odbexcel : public QObject
{
	Q_OBJECT

public:
	Odbexcel(QObject *parent);
	~Odbexcel();
	/*将数据保存为excel*/
	bool static save(QString filePath, QStringList headers, QList<QStringList> data, QString comment = "");
	/*将tableview数据导出为excel*/
	bool static saveFromTable(QString filePath, QTableView *tableView, QString comment = "");
	/*获取错误信息*/
	QString static getError() { return error; }
private:
	void static printError(QSqlError error);
	bool static insert(QSqlQuery& query, QString sheetName, QStringList slist);
	static QString error;
};