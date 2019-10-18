
#ifndef CSTYLEMANAGER_H
#define CSTYLEMANAGER_H

#include <QList>
#include <QSettings>
#include "define.h"


/* 
* File : CStyleManager.h
*
* Author : Xiewei
*
* Date : 2019-10-15
*
* Description : Ƥ����ʽ�����࣬����ʵ��
*
*/

class CStyleManager
{

public:
	~CStyleManager();
private:
	CStyleManager();

public:
	static CStyleManager& getInstance();
	StyleStruct getDefaultStyle();
	void setCurrentStyle(const StyleType &style);
	const StyleType &getCurrentStyleType();
	StyleStruct getCurrentStyleStruct();
	const StyleStruct &getStyleStruct(const StyleType &type);

public:
	QList<std::pair<StyleType,StyleStruct>> mStyles;
private:
	QMap<StyleType, StyleStruct> mapStyles;
	StyleType mCurrentStyle;
	QSettings mSetting;

};

#endif //CSTYLEMANAGER_H
