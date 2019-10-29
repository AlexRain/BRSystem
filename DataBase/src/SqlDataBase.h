#pragma once

#include "database_global.h"
#include <QSqlDatabase>
#include <QObject>

class DATABASE_EXPORT SqlDataBase:public QObject
{
public:
	SqlDataBase();
	~SqlDataBase();

	void initDataBase(const QString &);

private:
	QSqlDatabase dataBase;

};
