
#include "SelectGoodsView.h"
#include <QSettings>
#include <QCheckBox>
#include <QRadioButton>
#include <QFile>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>
#include <QButtonGroup>
#include "DialogMsg.h"

const char* goods_yj = "goods/yj";
const char* goods_yj_checked = "goods/goods_yj_checked";
const char* goods_mc_1 = "goods/goods_mc_1";
const char* goods_mc_1_checked = "goods/goods_mc_1_checked";
const char* goods_mc_5= "goods/goods_mc_5";
const char* goods_mc_5_checked = "goods/goods_mc_5_checked";
const char* goods_mc_10 = "goods/goods_mc_10";
const char* goods_mc_10_checked = "goods/goods_mc_10_checked";
const char* goods_lb_5= "goods/goods_lb_5";
const char* goods_lb_5_checked = "goods/goods_lb_5_checked";
const char* goods_lb_10 = "goods/goods_lb_10";
const char* goods_lb_10_checked = "goods/goods_lb_10_checked";


SelectGoodsView::SelectGoodsView(QWidget* parent)
	: BaseDialog(parent)
{
	QSettings saveSetting("app.ini", QSettings::IniFormat);
	ui.setupUi(this->getContent());
	//幸运魔锤1个
	ui.spinBox_mc_1->setValue(saveSetting.value(goods_mc_1, 1).toInt());
	ui.checkBox_mc_1->setChecked(saveSetting.value(goods_mc_1_checked,true).toBool());
	//幸运魔锤10个
	ui.checkBox_mc_10->setChecked(saveSetting.value(goods_mc_10_checked,true).toBool());
	ui.spinBox_mc_10->setValue(saveSetting.value(goods_mc_10, 2).toInt());
	//幸运魔锤5个
	ui.checkBox_mc_5->setChecked(saveSetting.value(goods_mc_5_checked, false).toBool());
	ui.spinBox_mc_5->setValue(saveSetting.value(goods_mc_5, 1).toInt());
	//清除卷
	ui.checkBox_2->setChecked(saveSetting.value(goods_yj_checked, false).toBool());
	ui.spinBox_yj->setValue(saveSetting.value(goods_yj, 1).toInt());
	//喇叭10个
	ui.checkBox_lb_10->setChecked(saveSetting.value(goods_lb_10_checked, false).toBool());
	ui.spinBox_lb_10->setValue(saveSetting.value(goods_lb_10, 1).toInt());
	//喇叭5个
	ui.checkBox_lb_5->setChecked(saveSetting.value(goods_lb_5_checked, false).toBool());
	ui.spinBox_lb_5->setValue(saveSetting.value(goods_lb_5, 1).toInt());
	this->SetTitle(QString::fromLocal8Bit("买道具"));
	this->resize(332, 199);
	QMetaObject::connectSlotsByName(this);
	//getGoods();
	//ui.checkBox_mc_1->setText(QString::fromLocal8Bit("频道喇叭（1个）"));
}


//void SelectGoodsView::getGoods()
//{
//	QFile loadFile("goods.json");
//
//	if (!loadFile.open(QIODevice::ReadOnly))
//	{
//		qDebug() << "could't open projects json";
//		return;
//	}
//
//	QByteArray allData = loadFile.readAll();
//	loadFile.close();
//
//	QJsonParseError error;
//
//	QJsonDocument document = QJsonDocument::fromJson(allData, &error);
//
//
//	if (QJsonParseError::NoError != error.error){
//		qDebug() << "json error!";
//		return;
//	}
//	if (document.isArray())
//	{
//		QJsonArray array = document.array();  // 转数组 
//		int nSize = array.size();
//		buttonGroup = new QButtonGroup(this);
//		for (int i = 0; i < nSize; ++i)
//		{
//			int id = array.at(i).toObject().value("id").toString().toInt();
//			QString name = array.at(i).toObject().value("name").toString();
//			QString price = array.at(i).toObject().value("price").toString();
//			//
//
//			QRadioButton* radioButton = new QRadioButton(this);
//			radioButton->setObjectName(QString::fromUtf8("radioButton"));
//			radioButton->setText(name+"("+price+")");
//			buttonGroup->addButton(radioButton, id);
//			ui.gridLayout_goods_list->addWidget(radioButton, i/3, i%3, 1, 1);
//		}
//	}
//}
bool SelectGoodsView::GetGoods() {

	int price = 0;
	QJsonObject gd;
	QJsonArray goodsList;
	QSettings saveSetting("app.ini", QSettings::IniFormat);
	//幸运魔锤1个
	int mc_1 = ui.spinBox_mc_1->value();
	if (mc_1 > 0) {
		saveSetting.setValue(goods_mc_1, mc_1);
	}
	bool mc_1_checked = ui.checkBox_mc_1->isChecked();
	saveSetting.setValue(goods_mc_1_checked, mc_1_checked);
	if (mc_1_checked && mc_1 >0) {
		QJsonObject goods;

		QSettings deugSetting("setting.ini", QSettings::IniFormat);
		bool debugMode = deugSetting.value("System/debug", false).toBool();
		if (debugMode) {
			goods.insert("id", 9803);
			goods.insert("price", 80);
			goods.insert("number", mc_1);
			goodsList.append(goods);
			price += 80 * mc_1;
		}
		else {
			goods.insert("id", 9720);
			goods.insert("price", 300);
			goods.insert("number", mc_1);
			goodsList.append(goods);
			price += 300 * mc_1;
		}

	}

	//幸运魔锤5个
	int mc_5 = ui.spinBox_mc_5->value();
	if (mc_5 > 0) {
		saveSetting.setValue(goods_mc_5, mc_5);
	}
	bool mc_5_checked = ui.checkBox_mc_5->isChecked();
	saveSetting.setValue(goods_mc_5_checked, mc_5_checked);
	if (mc_5_checked && mc_5 > 0) {
		QJsonObject goods;
		goods.insert("id", 9721);
		goods.insert("price", 1200);
		goods.insert("number", mc_5);
		goodsList.append(goods);
		price += 1200 * mc_5;
	}

	//幸运魔锤10个
	int mc_10 = ui.spinBox_mc_10->value();
	if (mc_10 > 0) {
		saveSetting.setValue(goods_mc_10, mc_10);
	}
	bool mc_10_checked = ui.checkBox_mc_10->isChecked();
	saveSetting.setValue(goods_mc_10_checked, mc_10_checked);
	if (mc_10_checked && mc_10 > 0) {
		QJsonObject goods;
		goods.insert("id", 9722);
		goods.insert("price", 2380);
		goods.insert("number", mc_10);
		goodsList.append(goods);
		price += 2380 * mc_10;
	}


	//喇叭10个
	int lb_10 = ui.spinBox_lb_10->value();
	if (lb_10 > 0) {
		saveSetting.setValue(goods_lb_10, lb_10);
	}
	bool lb_10_checked = ui.checkBox_lb_10->isChecked();
	saveSetting.setValue(goods_lb_10_checked, lb_10_checked);
	if (lb_10_checked && lb_10 > 0) {
		QJsonObject goods;
		goods.insert("id", 9765);
		goods.insert("price", 1950);
		goods.insert("number", lb_10);
		goodsList.append(goods);
		price += 1950 * lb_10;
	}

	//喇叭5个
	int lb_5 = ui.spinBox_lb_5->value();
	if (lb_5 > 0) {
		saveSetting.setValue(goods_lb_5, lb_5);
	}
	bool lb_5_checked = ui.checkBox_lb_5->isChecked();
	saveSetting.setValue(goods_lb_5_checked, lb_5_checked);
	if (lb_5_checked && lb_5 > 0) {
		QJsonObject goods;
		goods.insert("id", 9766);
		goods.insert("price", 1050);
		goods.insert("number", lb_5);
		goodsList.append(goods);
		price += 1050 * lb_5;
	}


	//异界1个
	int yj = ui.spinBox_yj->value();
	if (yj > 0) {
		saveSetting.setValue(goods_yj, yj);
	}
	bool yj_checked = ui.checkBox_2->isChecked();
	saveSetting.setValue(goods_yj_checked, yj_checked);
	if (yj_checked && yj > 0) {
		QJsonObject goods;
		goods.insert("id", 9810);
		goods.insert("price", 2000);
		goods.insert("number", yj);
		goodsList.append(goods);
		price += 2000 * yj;
	}
	if (goodsList.size() > 0) {
		gd.insert("list", goodsList);
		gd.insert("total", price);
		goodsRequest = gd;
		qDebug() << "accpet";
		return true;
	}
	return false;
}

void SelectGoodsView::on_btn_ok_clicked()
{
	if (GetGoods()) {
		this->accept();
	}
	else {
		DialogMsg::question(this, tr("warning"), QString::fromLocal8Bit("请选择一个道具"), QMessageBox::Ok);
	}
}

SelectGoodsView::~SelectGoodsView()
{
}
