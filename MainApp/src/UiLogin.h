#pragma once

#include "NetworkDefine.h"
#include <QComboBox>
#include <QDialog>
#include <QLineEdit>
#include <QMouseEvent>
#include <QSettings>

class UiLogin : public QDialog {
    Q_OBJECT

public:
    UiLogin(QWidget* parent = Q_NULLPTR);
    ~UiLogin();

public:
    void initUser();
    void verify();
    int fadeIn();

private slots:
    void onRequestCallback(const ResponData& data);

protected:
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void keyPressEvent(QKeyEvent* event);

private:
    QLineEdit* m_pEditName = nullptr;
    QLineEdit* m_pEditPwd = nullptr;
    QPushButton* buttonLogin = nullptr;
    QPoint m_dragPoint;
    bool m_bCanMove = false;
};
