#ifndef CSHADOWUICONTAINER_H
#define CSHADOWUICONTAINER_H


/* 
* File : CShadowUiContainer.h
*
* Author : Xiewei
*
* Date : 2019-10-15
*
* Description : 阴影效果窗体
*
*/

#include <QWidget>

class CShadowUiContainer : public QWidget
{
	Q_OBJECT

public:
	CShadowUiContainer(QWidget *parent);
	~CShadowUiContainer();
};

#endif // !CSHADOWUICONTAINER_H