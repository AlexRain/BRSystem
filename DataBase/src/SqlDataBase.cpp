#include "SqlDataBase.h"

SqlDataBase::SqlDataBase()
{
}

SqlDataBase::~SqlDataBase()
{

}

void SqlDataBase::initDataBase(const QString &name)
{
	dataBase = QSqlDatabase::addDatabase("QPSQL");
}
