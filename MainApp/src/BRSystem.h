#pragma once

#include "BaseWidget.h"
#include <QLabel>
#include <QMap>
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

private:
    void init();
    void createMenus(QMenuBar* menuBar);
    void createStatusBar();

protected:
    virtual void resizeEvent(QResizeEvent* event) override;
    virtual void closeEvent(QCloseEvent* event) override;

private:
    //CUiTop* mTopWidget;
    QMenuBar* mMenuBar;
    QToolBar* mToolbar;
    UiFrostedLayer* pLayer;
    QList<BaseWidget*> mListWidgets;
    QMap<int, QWidget*> m_mapWidget;
    QVBoxLayout* m_pContentLayout;
    BaseWidget* m_pCurrentWidget;
    QTextBrowser* logOutput = nullptr;
    QLabel* labelAdds = nullptr;
};
