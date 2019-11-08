/*
* File : CEditInfoDialog.h
*
* Author : Xiewei
*
* Date : 2019-10-12
*
* Description : 新建借条或编辑借条类头文件
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
	/*初始化*/
	void init();
	/*初始化控件*/
	void initEdit();
	/*初始化用户*/
	void initUsers();
	/*初始化数据*/
	void initData();
	/*验证输入合法性*/
	bool verifyInput();
	/*获取输入的信息*/
	void getInputData();

signals:
	/*删除借条*/
	void deleteItem(const BorrowInfo &info);
	/*保存借条*/
	void saveData(const BorrowInfo &info);
	/*更新借条信息*/
	void updateData(const BorrowInfo &info);

private slots:
	void on_btn_cancel_clicked();
	void on_btn_save_clicked();
	void on_btn_history_clicked();
	void on_btn_delete_clicked();
private:
	Ui::CEditInfoDialog ui;
	BorrowInfo mBorrowInfo; /*借条信息结构体*/
	bool mIsEditUi; /*是否是编辑界面*/
};
