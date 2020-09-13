#pragma once

#include "commongui_global.h"
#include "ui_BaseDialog.h"
#include <QDialog>
#include <QPushButton>

class COMMONGUI_EXPORT BaseDialog : public QDialog {
    Q_OBJECT

public:
    QWidget* getContent();

public:
    BaseDialog(QWidget* parent = Q_NULLPTR);
    ~BaseDialog();

    void SetTitle(const QString& title);

protected:
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

private:
    Ui::BaseDialog ui;
    bool m_bCanMove;
    QString mTitleText;
    QPoint m_dragPoint;
    QPushButton* btn_close;
};
