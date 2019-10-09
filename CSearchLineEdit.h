#pragma once

#include <QObject>
#include <QLineEdit>

class CSearchLineEdit : public QLineEdit
{
	Q_OBJECT

public:
	CSearchLineEdit(QWidget *parent);
	~CSearchLineEdit();
};
