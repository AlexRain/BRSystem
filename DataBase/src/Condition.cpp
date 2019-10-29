#include "Condition.h"


CCondition::CCondition(void)
{
}


CCondition::~CCondition(void)
{
    Clear();
}

void CCondition::Add(ConditionItem pItem)
{
    m_vpItem.push_back(pItem);
}
void CCondition::Clear()
{
    m_vpItem.clear();
}

QString CCondition::ToQString(const QString &csCH/* = " and "*/)
{
    QString csStr;
    int t = 0;
    int n = m_vpItem.size();
    for(;t < n; t ++)
    {
        csStr += m_vpItem[t].ToQString() + csCH;
    }

    return csStr.mid(0, csStr.lastIndexOf(csCH));
}

void CCondition::AppendToQString( QString &csValue, const QString &csCH /*= " and "*/ )
{
    int t = 0;
    int n = m_vpItem.size();
    for(;t < n; t ++)
    {
        csValue += m_vpItem[t].ToQString() + csCH;
    }

    csValue = csValue.mid(0, csValue.lastIndexOf(csCH));
}

