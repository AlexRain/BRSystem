#pragma once

#include <QObject>
#include <QLineEdit>
#include "commongui_global.h"

class COMMONGUI_EXPORT CSearchLineEdit : public QLineEdit
{
	Q_OBJECT

public:
	CSearchLineEdit(QWidget *parent);
	~CSearchLineEdit();

signals:
	void query(const QString &);
};
