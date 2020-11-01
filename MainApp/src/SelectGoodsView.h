#pragma once
#include "BaseDialog.h"
#include"ui_SelectGoodsView.h"
#include <QJsonObject>
class SelectGoodsView : public BaseDialog {
	Q_OBJECT

public:
	typedef QMap<QString, int> Goods;
	QJsonObject goodsRequest;

	struct GoodsSetting {
		int goods_yj = 0;
		bool goods_yj_checked = false;
		int goods_mc_1 = 1;
		bool goods_mc_1_checked = true;
		int goods_mc_5 = 0;
		bool goods_mc_5_checked = false;
		int goods_mc_10 = 2;
		bool goods_mc_10_checked = true;
		int goods_lb_5 = 1;
		bool goods_lb_5_checked = false;
		int goods_lb_10 = 1;
		bool goods_lb_10_checked = false;
	};
public:
	SelectGoodsView(QWidget* parent = Q_NULLPTR);
	bool GetGoods();
	~SelectGoodsView();


private slots:
	void on_btn_ok_clicked();

private:
	Ui::SelectGoodsView ui;
	QButtonGroup* buttonGroup;
	GoodsSetting goodsSetting;
};
