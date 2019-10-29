/**
* @file Condition.h 
* @brief 条件生成类
*
* detail 用于生成sql
*
* @version 1.0
* @author sxl
* @data [2014-12-9]
*/
#ifndef CONDITION_H
#define CONDITION_H

#include <QObject>
#include <QVector>
#include "database_global.h"
using namespace std;

/// 条件item
class DATABASE_EXPORT ConditionItem
{
public:
    
    ConditionItem(QString csKey, QString csCond, QString csValue)
    {
        m_nType = 0;
        m_csKey = csKey;
        m_csCond = csCond;
        m_csValue = csValue;
    };

    ConditionItem(QString csKey, QString csValue)
    {
        m_nType = 0;
        m_csKey = csKey;
        m_csCond = " = ";
        m_csValue = csValue;
    };

    ConditionItem(QString csKey)
    {
        m_nType = 1;
        m_csKey = csKey;
    };

    ConditionItem()
    {
        m_nType = 1;
        m_csKey = "";
    };

    ~ConditionItem(){};

    /// 转换为字符串 如：ID = '123'
    QString ToQString(QString csCH = "'")
    {
        if (0 == m_nType)
        {
            return m_csKey + m_csCond + csCH + m_csValue + csCH ;
        }
        else if (1 == m_nType)
        {
            return m_csKey;
        }
        return "";
    }
private:
    int m_nType;//0 为组合条件 1 表示m_csKey为单个条件
    QString m_csKey;
    QString m_csCond;
    QString m_csValue;
};

/// 条件类
class DATABASE_EXPORT CCondition
{
public:
    CCondition(void);
    ~CCondition(void);

    void Add(ConditionItem Item);
    void Clear();

    /// 转换为字符串 如：ID = '123',NAME = 'sss' csCH 为条件分割符
    QString ToQString(const QString &csCH = " and ");
    /// 追加转换字符串 如：ID = '123',NAME = 'sss' csCH 为条件分割符
    void AppendToQString(QString &csValue, const QString &csCH = " and ");

private:
    QVector<ConditionItem > m_vpItem;
};

#endif // CONDITION_H