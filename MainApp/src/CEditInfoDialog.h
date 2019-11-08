/*
* File : CEditInfoDialog.h
*
* Author : Xiewei
*
* Date : 2019-10-12
*
* Description : �½�������༭������ͷ�ļ�
*
*/

#pragma once


#include "ui_CEditInfoDialog.h"
#include "define.h"
#include "BaseWidget.h"



class CEditInfoDialog : public BaseWidget
{
	Q_OBJECT

public:
	CEditInfoDialog(QWidget *parent = Q_NULLPTR,bool isEditUi = true);
	~CEditInfoDialog();

public:
	void setData(const BorrowInfo &info);

private:
	/*��ʼ��*/
	void init();
	/*��ʼ���ؼ�*/
	void initEdit();
	/*��ʼ���û�*/
	void initUsers();
	/*��ʼ������*/
	void initData();
	/*��֤����Ϸ���*/
	bool verifyInput();
	/*��ȡ�������Ϣ*/
	void getInputData();

signals:
	/*ɾ������*/
	void deleteItem(const BorrowInfo &info);
	/*�������*/
	void saveData(const BorrowInfo &info);
	/*���½�����Ϣ*/
	void updateData(const BorrowInfo &info);

private slots:
	void on_btn_cancel_clicked();
	void on_btn_save_clicked();
	void on_btn_history_clicked();
	void on_btn_delete_clicked();
private:
	Ui::CEditInfoDialog ui;
	BorrowInfo mBorrowInfo; /*������Ϣ�ṹ��*/
	bool mIsEditUi; /*�Ƿ��Ǳ༭����*/
};
