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

#include <QDialog>
#include "ui_CEditInfoDialog.h"
#include "define.h"



class CEditInfoDialog : public QDialog
{
	Q_OBJECT

public:
	CEditInfoDialog(QWidget *parent = Q_NULLPTR,bool isEditUi = true);
	~CEditInfoDialog();

public:
	void setData(const BorrowInfo &info);
	void setDeleteFlag(bool del);

private:
	/*��ʼ��*/
	void init();
	/*��ʼ���ؼ�*/
	void initEdit();
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
	bool mIsDelete;  /*�Ƿ��ѱ�ɾ�����ⲿ�޸Ĵ�ֵ*/
	bool mIsEditUi; /*�Ƿ��Ǳ༭����*/
};
