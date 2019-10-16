#include "UiChangeSkin.h"
#include <QGridLayout>
#include <QSettings>
#include <QIcon>
#include "CStyleManager.h"
#include "CSkinItem.h"

UiChangeSkin::UiChangeSkin(QWidget *parent)
	: BaseWidget(parent)
{
	this->resize(396, 224);
	QGridLayout *layout = new QGridLayout(this);

	for (int i = 0; i < CStyleManager::getInstance().mStyles.size(); i++)
	{
		StyleStruct style = CStyleManager::getInstance().mStyles.at(i).second;
		CSkinItem *item = new CSkinItem(this);
		connect(item, SIGNAL(checked(bool)), this, SLOT(itemChecked(bool)));
		item->setData(style);
		item->setContentText(style.styleName);
		item->setContentColor(style.itemColor);
		layout->addWidget(item, 0, i, 1, 1);
		mapItems[style.type] = item;
	}

	/*ÉèÖÃÄ¬ÈÏÑ¡Ôñ*/
	mapItems[CStyleManager::getInstance().getCurrentStyleType()]->setChecked(true);
}

UiChangeSkin::~UiChangeSkin()
{
}

void UiChangeSkin::itemChecked(bool checked)
{
	CSkinItem *item = qobject_cast<CSkinItem*>(sender());
	Q_ASSERT(item);
	StyleHelper::loadAppStyle(item->getData().cssFile);
	qApp->processEvents();
	this->update();
	if (checked) {
		for each (CSkinItem *p in mapItems) {
			p->setChecked(false);
		}
		item->setChecked(true);
		CStyleManager::getInstance().setCurrentStyle(item->getData().type);
	}
}
