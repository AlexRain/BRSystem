/**
* @file CDbHelper.h 
* @brief CDbHelper ���ݿ������
*
* detail Ŀǰ�����sqlite
*
* @version 1.0
* @author sxl
* @data [2014-12-9]
*/
#ifndef CDBHELPER_H
#define CDBHELPER_H

#include <QObject>

#include <QMap>
#include "Condition.h"
#include "database_global.h"
#include "define.h"

#define DIC_BORROW_RETURN      "DIC_BORROW_RETURN"   /// ��Ʒ�軹��
#define DIC_DEPARTMENT         "DIC_DEPARTMENT"      /// ���ű�
#define DIC_OPERATION_LOG      "DIC_OPERATION_LOG"   /// ������־��
#define DIC_USER               "DIC_USER"            /// �û���

#define CREATE_TABLE_SQL "CREATE TABLE DIC_BORROW_RETURN \
                (                                       \
                  id      smallint,     \
                  productionId      varchar(10)  NOT NULL ,     \
                  productionName    varchar(50)  NOT NULL ,     \
                  borrowerName varchar(50)  NULL ,         \
                  borrowDate    timestamp  NULL ,         \
                  updateDate   timestamp   NULL ,         \
                  borrowStatus     smallint ,         \
                  borrowReason varchar(150)  NULL ,         \
                  remarks varchar(150)  NULL         \
                );"

class QSqlDatabase;
class QSqlQuery;

/// ���ݿ������
class DATABASE_EXPORT CDbHelper : public QObject
{
public:
    CDbHelper(QObject *parent = 0);
    ~CDbHelper();

    /// �����ݿ�
    static void opendatabase(const QString &table);

    bool open(const QString &qsDbName = "db.s3db");
    void close(const QString &qsDbName = "db.s3db");
    static void closeAll();

    /// �жϱ��Ƿ����
    bool isTableExist(const QString &qsTableName);
    /// ����
    int Replace(const QString &table, const ModelData &model);
    int Insert(const QString &table, const ModelData &model);
    /// �޸�
    int Update(const QString &qsTableName, const ModelData &model, CCondition &condition);
    int Update(const QString &qsTableName, const ModelData &model, const QString &qsWhere);
    /// ɾ��
    int Delete(const QString &qsTableName, CCondition &condition);
    int Delete(const QString &qsTableName, const QString &qsWhere);
    /// ��ѯ
    int Queuey(QList<ModelData> &listModel, const QString &qsTableName, const QString &qsColunm, CCondition &condition);
    int Queuey(QList<ModelData> &listModel, const QString &qsSql );
    int Queuey(QList<ModelData> &listModel, const QString &qsTableName, const QString &qsColunm1, const QString &qsColunm2, CCondition &condition);
    /// ִ��sql
    int exec(const QString &qsSql);

    bool transaction();
    bool commit();
    bool rollback();

protected:
    /// ת����insert��sql���
    void ModelToInsertString(const QString &qsTableName, const ModelData &model, QString &qsSql, const QString &qsAction = "INSERT");
    /// ת����update��sql���
    void ModelToUpdateString(const QString &csTableName, const ModelData &model, QString &strSql);

    
private:
    QSqlQuery *m_pQuery;
    QSqlDatabase *m_pDb;
};

#endif // CDBHELPER_H
