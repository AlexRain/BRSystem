#include "CStyleManager.h"

CStyleManager::CStyleManager()
{
    StyleStruct styleDark = {
        Dark,
        QObject::tr("black"),
        QString("qss/global_dark.css"),
        QColor(41, 44, 50)
    };

    StyleStruct styleWhite = {
        White,
        QObject::tr("white"),
        QString("qss/global_white.css"),
        QColor(221, 214, 193)
    };

    /*StyleStruct styleYellow = {
        Yellow,
        QObject::tr("orange"),
        QString("qss/global_orange.css"),
        QColor("#ffaa00")
    };*/
    QSettings setting(GetAppDataLocation() + QDir::separator() +  CONFIG_FILE, QSettings::IniFormat);
    mCurrentStyle = (StyleType)setting.value("CONFIG/skinType", (int)StyleType::White).toInt();
    mStyles << std::make_pair(styleDark.type, styleDark)
            << std::make_pair(styleWhite.type, styleWhite) /*<< std::make_pair(styleYellow.type, styleYellow)*/;

    mapStyles[Dark] = styleDark;
    mapStyles[White] = styleWhite;
    //mapStyles[Yellow] = styleYellow;
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
        QObject::tr("white"),
        QString("qss/global_white.css")
    };
}

void CStyleManager::setCurrentStyle(const StyleType& style)
{
    mCurrentStyle = style;
    QSettings setting(GetAppDataLocation() + QDir::separator() + CONFIG_FILE, QSettings::IniFormat);
    setting.setValue("CONFIG/skinType", (int)style);
}

const StyleType& CStyleManager::getCurrentStyleType()
{
    return mCurrentStyle;
}

StyleStruct CStyleManager::getCurrentStyleStruct()
{
    if (!mapStyles.contains(mCurrentStyle)) {
        return getDefaultStyle();
    }
    return mapStyles[mCurrentStyle];
}

const StyleStruct& CStyleManager::getStyleStruct(const StyleType& type)
{
    return mapStyles[type];
}

CStyleManager::~CStyleManager()
{
}
