#include "StyledWidget.h"

StyledWidget::StyledWidget(QWidget *parent)
	: QWidget(parent)
{
}

StyledWidget::~StyledWidget()
{
}

void StyledWidget::setBackgroundColor(const QColor &color)
{
	this->_backgroundColor = color;
}

const QColor & StyledWidget::backgroundColor() const
{
	return this->_backgroundColor;
}

void StyledWidget::setShadowColor(const QColor &color)
{
	this->_shadowColor = color;
}

const QColor & StyledWidget::shadowColor() const
{
	return this->_shadowColor;
}

