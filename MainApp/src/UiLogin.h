#pragma once

#include "NetworkDefine.h"
#include "ui_UiLogin.h"
#include <QComboBox>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QMouseEvent>
#include <QSettings>

class UiLogin : public QDialog {
    Q_OBJECT

    enum class UiType { UiLogin,
        UiRegister };

public:
    UiLogin(QWidget* parent = Q_NULLPTR);
    ~UiLogin();

public:
    void getUserInfo();
    void initUser();
    void verify();
	int fadeIn();

private slots:
    void onRequestCallback(const ResponData& data);
    void onLinkActived(const QString& link);
    void onRequestError(const ResponData& data, NetworkRequestError errorType);
	void acceptDown();

protected:
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void keyPressEvent(QKeyEvent* event);
    virtual void resizeEvent(QResizeEvent* event) override;
    void on_btn_register_clicked();

	void AutoUpgrade(QString url,QString fileName);

	void ShowProgressBar();

private:
    Ui::UiLogin ui;

    QPushButton* btn_close = nullptr;
    QPushButton* btn_back = nullptr;

    UiType uiType { UiType::UiLogin };

    QPoint m_dragPoint;
    bool m_bCanMove = false;
};
