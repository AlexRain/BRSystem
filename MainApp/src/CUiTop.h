#pragma once

#include "ui_CUiTop.h"
#include <QWidget>

class CirclePixmap;

class CUiTop : public QWidget {
    Q_OBJECT

    Q_PROPERTY(QPixmap logo READ logo WRITE setLogo)

public:
    enum WindowState {
        StateMin,
        StateNormalOrMax,
    };

    friend class CUiCenter;

public:
    CUiTop(QWidget* parent = Q_NULLPTR);
    ~CUiTop();

    void setLogo(const QPixmap& logo);
    const QPixmap& logo() const;

public:
    void windowStateChanged(Qt::WindowStates states);

private slots:
    void on_btn_changeSkin_clicked();
    void on_btn_min_clicked();
    void on_btn_max_clicked();
    void on_btn_close_clicked();
    void on_btn_setting_clicked();
    void on_label_userName_clicked();
    void on_label_logo_clicked();

signals:
    void showChangeSkinDlg(const QPoint& point);
    void aboutToChangeWindowState(CUiTop::WindowState state);
    void appAboutToExit();
    void clickProfile(const QPoint& point);
    void showSettingUi();
    void showMainPage();

private:
    Ui::CUiTop ui;
    Qt::WindowStates mStates;
    CirclePixmap* headPixmap;
    QPixmap m_pixmapLogo;
};
