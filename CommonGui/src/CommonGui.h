#pragma once

#include "commongui_global.h"
#include <QObject>

class COMMONGUI_EXPORT CommonGui : public QObject {
    Q_OBJECT
public:
    CommonGui();
    void installTranslator();
};
