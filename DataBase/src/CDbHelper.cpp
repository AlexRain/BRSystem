#include "CDbHelper.h"
#include <QVariant>
#include <qDebug>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QSqlDatabase>
#include <QThread>
#include <QSqlError>
#include "qfile.h"

QSqlDatabase g_db;
QMap<QString, QSqlDatabase> g_mapDb;

bool b_dbInit = false;

CDbHelper::CDbHelper(QObject *parent)
    : QObject(parent)
{
    m_pQuery = NULL;
    m_pDb    = NULL;
}

CDbHelper::~CDbHelper()
{
    if (m_pQuery != NULL)
    {
        delete m_pQuery;
        m_pQuery = NULL;
    }
}

// 打开数据库
void CDbHelper::opendatabase(const QString &qsDbName)
{
    QSqlDatabase &db = g_mapDb[qsDbName];
    if(QSqlDatabase::contains(qsDbName))
    {
        db = QSqlDatabase::database(qsDbName);
    }
    else
    {
        db = QSqlDatabase::addDatabase("QSQLITE", qsDbName);
    }

    db.setDatabaseName(qsDbName);

    b_dbInit = true;
}


bool CDbHelper::open(const QString &qsDbName)
{
    if (!b_dbInit)
    {//等待初始化完成
        while (!b_dbInit)
        {
            QThread::msleep(10);
        }
    }

    m_pDb = &g_mapDb[qsDbName];

    if (m_pDb == NULL)
    {
        return false;
    }

    if (m_pDb->isOpen())
    {
        return true;
    }
    return m_pDb->open();
}

void CDbHelper::close(const QString &qsDbName)
{
    m_pDb = &g_mapDb[qsDbName];

    if (m_pDb != NULL)
    {
        m_pDb->close();
    }
}

void CDbHelper::closeAll()
{
    QMap<QString, QSqlDatabase>::iterator iter = g_mapDb.begin();
    while (iter != g_mapDb.end())
    {
        iter.value().close();
        iter ++;
    }
}

bool CDbHelper::isTableExist( const QString &qsTableName )
{
    QList<ModelData> listModel;
    int nRv = Queuey(listModel, "SELECT count(*)  FROM sqlite_master WHERE type='table' AND name='" + qsTableName + "'");
    if (nRv != -1)
    {
        if (listModel.size() > 0)
        {
            QString bExist = listModel[0]["count(*)"];
            if (bExist != "0")
            {
                return true;
            }
        }
    }
    return false;
}

QString CDbHelper::lastErrorText() const
{
	Q_ASSERT(m_pDb);
	return m_pDb->lastError().text();
}

// 转换成insert的sql语句
void CDbHelper::ModelToInsertString(const QString &qsTableName, const ModelData &model, QString &qsSql, const QString &qsAction /*= "INSERT"*/)
{
    QString csKstr;
    QString csVstr;

    ModelData::const_iterator iter = model.begin();
    for ( ; iter != model.end(); iter++)
    {
        csKstr += QString("%1,").arg(iter.key());
        csVstr += QString("'%1',").arg(iter.value());
    }
    csKstr = csKstr.mid(0, csKstr.lastIndexOf(","));
    csVstr = csVstr.mid(0, csVstr.lastIndexOf(","));

    qsSql = QString("%1 INTO %2 (%3) VALUES (%4)").arg(qsAction).arg(qsTableName).arg(csKstr).arg(csVstr);
}

int CDbHelper::Replace( const QString &table, const ModelData &model )
{
    if (model.size() <= 0)
    {
        return -1;
    }

    QString qsSql;

    ModelToInsertString(table, model, qsSql, "REPLACE");

    return exec(qsSql);
}

// 插入
int CDbHelper::Insert(const QString &table, const ModelData &model)
{
    if (model.size() <= 0)
    {
        return -1;
    }

    QString qsSql;

    ModelToInsertString(table, model, qsSql);

    return exec(qsSql);
}

// 转换成update的sql语句
void CDbHelper::ModelToUpdateString(const QString &csTableName, const ModelData &model, QString &strSql)
{
    strSql = QString("UPDATE %1 SET ").arg(csTableName);

    CCondition condition;
    ModelData::const_iterator iter = model.begin();
    for (; iter != model.end(); iter++)
    {
        condition.Add(ConditionItem(iter.key(), iter.value()));
    }
    condition.AppendToQString(strSql, ",");
}

// 修改
int CDbHelper::Update( const QString &qsTableName, const ModelData &model, CCondition &condition )
{
    if (qsTableName.isEmpty())
    {
        return -1;
    }
    QString qsWhere = " WHERE ";

    condition.AppendToQString(qsWhere);

    return Update(qsTableName, model, qsWhere);
}

int CDbHelper::Update(const QString &qsTableName, const ModelData &model, const QString &qsWhere)
{
    if (qsTableName.isEmpty())
    {
        return -1;
    }

    QString qsSql;

    ModelToUpdateString(qsTableName, model, qsSql);

    qsSql.append(qsWhere);

    return exec(qsSql);
}

// 删除
int CDbHelper::Delete(const QString &qsTableName, CCondition &condition)
{
    if (qsTableName.isEmpty())
    {
        return -1;
    }
    
    QString qsWhere;

    condition.AppendToQString(qsWhere);

    return Delete(qsTableName, qsWhere);
}

int CDbHelper::Delete(const QString &qsTableName, const QString &qsWhere)
{
    if (qsTableName.isEmpty())
    {
        return -1;
    }

    if (qsWhere.isEmpty())
    {
        return -1;
    }

    QString qsSql = "DELETE FROM ";
    qsSql.append(qsTableName);
    qsSql.append(" WHERE ");
    qsSql.append(qsWhere);

    return exec(qsSql);
}

int CDbHelper::Queuey(QList<ModelData> &listModel, const QString &qsTableName, const QString &qsColunm, CCondition &condition)
{
    QString qsWhere;

    condition.AppendToQString(qsWhere);

    QString qsSql = "SELECT ";
    qsSql.append(qsColunm);
    qsSql.append(" FROM ");
    qsSql.append(qsTableName);
    if (!qsWhere.isEmpty())
    {
        qsSql.append(" WHERE ");
        qsSql.append(qsWhere);
    }

    return Queuey(listModel, qsSql);
}


int CDbHelper::Queuey(QList<ModelData> &listModel, const QString &qsTableName, const QString &qsColunmKey, const QString &qsColunmValue, CCondition &condition)
{
    QString qsWhere;

    condition.AppendToQString(qsWhere);

    QString qsSql = "SELECT ";
    qsSql.append(qsColunmKey);
    qsSql.append(","+qsColunmValue);
    qsSql.append(" FROM ");
    qsSql.append(qsTableName);
    if (!qsWhere.isEmpty())
    {
        qsSql.append(" WHERE ");
        qsSql.append(qsWhere);
    }

    int nRv = -1;

    nRv = exec(qsSql);

    if (nRv != -1)
    {
        while (m_pQuery->next())
        {
            ModelData model;
            QSqlRecord &record = m_pQuery->record();
            int nLen = record.count();
            if (nLen == 2)
            {
                model.insert(record.value(0).toString(),record.value(1).toString());
                listModel.push_back(model);
            }  
        }

        nRv = listModel.size();
    }

    return nRv;
}

// 查询
int CDbHelper::Queuey(QList<ModelData> &listModel, const QString &qsSql )
{
    int nRv = -1;

    nRv = exec(qsSql);

    if (nRv != -1)
    {
        while (m_pQuery->next())
        {
            ModelData model;
            QSqlRecord &record = m_pQuery->record();
            int nLen = record.count();
            for (int i = 0; i < nLen ; i++)
            {
                model.insert(record.fieldName(i), record.value(i).toString());
            }
            listModel.push_back(model);
        }
        nRv = listModel.size();
    }
    qWarning()<<"exec SQL: "<<qsSql;
    qWarning()<<"exec SQL numRowsAffected: "<<nRv;

    return nRv;
}

int CDbHelper::exec( const QString &qsSql )
{
    int nRv = -1;

    if (m_pQuery == NULL){
        m_pQuery = new QSqlQuery(*m_pDb);
    }
    else{
        m_pQuery->clear();
    }

    if (m_pQuery != NULL){
        if (!m_pQuery->exec(qsSql)){
            qWarning()<<"exec SQL: "<<qsSql;
            qWarning()<<m_pQuery->lastError();
        }
        else{
            nRv = m_pQuery->numRowsAffected();
            qDebug()<<"exec SQL: "<<qsSql;
            qDebug()<<"exec SQL numRowsAffected: "<<nRv;
        }
    }
    return nRv;

}

bool CDbHelper::transaction()
{
    bool b = false;
    if (m_pDb != NULL)
    {
        b = m_pDb->transaction();
    }
    return b;
}

bool CDbHelper::commit()
{
    bool bRe = false;
    if (m_pDb != NULL){
        bRe = m_pDb->commit();
        if (!bRe)
        {
            if (m_pDb->rollback())
            {
                if (!m_pDb->commit())
                {
                    qDebug() << QString::fromLocal8Bit("提交失败！");
                    m_pDb->rollback();
                }
                else
                {
                    bRe = true;
                }
            }
        }

    }
    return bRe;
}

bool CDbHelper::rollback()
{
    bool b = false;
    if (m_pDb != NULL)
    {
        b = m_pDb->rollback();
    }
    return b;
}
