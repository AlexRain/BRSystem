#include "CommonGui.h"
#include <QCoreApplication>
#include <QTranslator>

CommonGui::CommonGui()
{
}

void CommonGui::installTranslator()
{
    QTranslator translator;
    translator.load(":/translation/commongui_zh.qm");
    QCoreApplication::installTranslator(&translator);
}
