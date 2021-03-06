#include "CStyleManager.h"

CStyleManager::CStyleManager()
	:mSetting(CONFIG_FILE, QSettings::IniFormat)
{
	StyleStruct styleDark = {
		Dark,
		TOCH("�ſ��"),
		QString("qss/global_dark.qss"),
		QColor(41, 44, 50)
	};

	StyleStruct styleWhite = {
		White,
		TOCH("������"),
		QString("qss/global_white.qss"),
		QColor(221,214,193)
	};

	StyleStruct styleYellow = {
		Yellow,
		TOCH("�Ķ���"),
		QString("qss/global_orange.qss"),
		QColor("#ffaa00")
	};

	mCurrentStyle = (StyleType)mSetting.value("CONFIG/skinType", (int)StyleType::Dark).toInt();
	mStyles << std::make_pair(styleDark.type, styleDark) 
		<< std::make_pair(styleWhite.type, styleWhite) << std::make_pair(styleYellow.type, styleYellow);

	mapStyles[Dark] = styleDark;
	mapStyles[White] = styleWhite;
	mapStyles[Yellow] = styleYellow;
	
}

CStyleManager& CStyleManager::getInstance()
{
	static CStyleManager obj;
	return obj;
}

StyleStruct CStyleManager::getDefaultStyle()
{
	return StyleStruct {
		White,
		TOCH("������"),
		QString("qss/global_white.qss")
	};
}

void CStyleManager::setCurrentStyle(const StyleType &style)
{
	mCurrentStyle = style;
	mSetting.setValue("CONFIG/skinType", (int)style);
}

const StyleType & CStyleManager::getCurrentStyleType()
{
	return mCurrentStyle;
}

StyleStruct CStyleManager::getCurrentStyleStruct()
{
	if (!mapStyles.contains(mCurrentStyle))
	{
		return getDefaultStyle();
	}
	return mapStyles[mCurrentStyle];
}

const StyleStruct & CStyleManager::getStyleStruct(const StyleType &type)
{
	return mapStyles[type];
}

CStyleManager::~CStyleManager()
{
}
