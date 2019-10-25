#pragma once

#include <QGraphicsEffect>
#include "commongui_global.h"

class GlowEffectPrivate;

/// @brief 发光效果的QGraphicsEffect
/// @par 示例:
/// @code
/// GlowEffect *effect = new GlowEffect(this);
/// effect->setRadius(5);
/// effect->setGlowColor(Qt::blue);
/// lbl->setGraphicsEffect(effect);
/// @endcode
/// @see setLogFile
class COMMONGUI_EXPORT GlowEffect : public QGraphicsEffect
{
	Q_OBJECT
public:
	GlowEffect(QObject *parent = 0);
	virtual ~GlowEffect();

	/// @brief set radius
	/// @param [in] int radius
	void setRadius(int radius);
	/// @brief set glow color
	/// @param [in] QColor  color
	void setGlowColor(const QColor &color);
	virtual QRectF boundingRectFor(const QRectF &sourceRect) const;
protected:
	virtual void draw(QPainter *painter);
	virtual void sourceChanged(ChangeFlags flags);

private:
	Q_DISABLE_COPY(GlowEffect)
	Q_DECLARE_PRIVATE(GlowEffect)
	GlowEffectPrivate* const d_ptr;
};
