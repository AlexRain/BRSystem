/**
* @file Condition.h 
* @brief ����������
*
* detail ��������sql
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

/// ����item
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

    /// ת��Ϊ�ַ��� �磺ID = '123'
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
    int m_nType;//0 Ϊ������� 1 ��ʾm_csKeyΪ��������
    QString m_csKey;
    QString m_csCond;
    QString m_csValue;
};

/// ������
class DATABASE_EXPORT CCondition
{
public:
    CCondition(void);
    ~CCondition(void);

    void Add(ConditionItem Item);
    void Clear();

    /// ת��Ϊ�ַ��� �磺ID = '123',NAME = 'sss' csCH Ϊ�����ָ��
    QString ToQString(const QString &csCH = " and ");
    /// ׷��ת���ַ��� �磺ID = '123',NAME = 'sss' csCH Ϊ�����ָ��
    void AppendToQString(QString &csValue, const QString &csCH = " and ");

private:
    QVector<ConditionItem > m_vpItem;
};

#endif // CONDITION_H