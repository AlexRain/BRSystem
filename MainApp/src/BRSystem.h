#pragma once

#include "BaseWidget.h"
#include <QLabel>
#include <QMap>
#include <QProcess>
#include <QTextBrowser>
#include <QWidget>

class CUiTop;
class CUiCenter;
class QMenuBar;
class QToolBar;
class UiFrostedLayer;
class QVBoxLayout;

class BRSystem : public QWidget {
    Q_OBJECT

public:
    BRSystem(QWidget* parent = nullptr);
    ~BRSystem();

public:
    static void showCoverWidget(BaseWidget* content);
    void addwidget(BaseWidget* content);
    void showMainPage();
    void outputMessage(QtMsgType type, const QMessageLogContext& context, const QString& msg);
    void printLog(QtMsgType type, const QString& msg);

private:
    void init();
    void createMenus(QMenuBar* menuBar);
    void CheckUpgrade();
    void startLocalPyServer();

protected:
    virtual void resizeEvent(QResizeEvent* event) override;
    virtual void closeEvent(QCloseEvent* event) override;

private:
    UiFrostedLayer* pLayer;
    QList<BaseWidget*> mListWidgets;
    QMap<int, QWidget*> m_mapWidget;
    QVBoxLayout* m_pContentLayout;
    BaseWidget* m_pCurrentWidget;
    QTextBrowser* logOutput = nullptr;
    QLabel* labelAdds = nullptr;
    QProcess* process = nullptr;
};
