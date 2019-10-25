#include "GlowEffect.h"
#include <QPainter>
#include <QPixmap>

// in src/qtbase/src/widgets/effects/qpixmapfilter.cpp
extern Q_DECL_IMPORT void qt_blurImage(QImage &blurImage, qreal radius,
	bool quality, int transposed = 0);

class GlowEffectPrivate
{
	Q_DECLARE_PUBLIC(GlowEffect)
public:
	explicit GlowEffectPrivate(GlowEffect *qptr);
	~GlowEffectPrivate();
	GlowEffect* const q_ptr;

	int radius;
	QColor color;
};

GlowEffectPrivate::GlowEffectPrivate(GlowEffect *qptr) : q_ptr(qptr)
{
}

GlowEffectPrivate::~GlowEffectPrivate()
{
}

GlowEffect::GlowEffect(QObject *parent)
	: QGraphicsEffect(parent), d_ptr(new GlowEffectPrivate(this))
{
	d_ptr->radius = 0;
	d_ptr->color = QColor(255, 255, 255, 255);
}

GlowEffect::~GlowEffect()
{
	delete d_ptr;
}

void GlowEffect::setRadius(int radius)
{
	Q_D(GlowEffect);
	d->radius = radius;
	updateBoundingRect();
	update();
}

void GlowEffect::setGlowColor(const QColor &color)
{
	Q_D(GlowEffect);
	d->color = color;
	update();
}

QRectF GlowEffect::boundingRectFor(const QRectF &sourceRect) const
{
	Q_D(const GlowEffect);
	QRectF tmp(sourceRect);
	tmp.setBottomRight(tmp.bottomRight()
		+ QPointF(d->radius * 2, d->radius * 2));
	return tmp;
}

void GlowEffect::draw(QPainter *painter)
{
	Q_D(GlowEffect);
	if (!d->radius)
	{
		drawSource(painter);
		return;
	}
	QPixmap source = sourcePixmap();
	QImage sourceBlured(source.size() + QSize(d->radius * 2, d->radius),
		QImage::Format_ARGB32_Premultiplied);
	sourceBlured.fill(0);
	QPainter tmpPainter(&sourceBlured);
	tmpPainter.drawPixmap(d->radius, 0, source.width(), source.height(), source);
	tmpPainter.setCompositionMode(QPainter::CompositionMode_SourceIn);
	tmpPainter.fillRect(sourceBlured.rect(), d->color);
	tmpPainter.end();
	qt_blurImage(sourceBlured, d->radius, false);
	painter->drawImage(0, 0, sourceBlured);
	painter->drawImage(0, 0, sourceBlured);
	painter->drawPixmap(d->radius, 0, source.width(), source.height(), source);
}

void GlowEffect::sourceChanged(ChangeFlags flags)
{
	Q_UNUSED(flags)
		updateBoundingRect();
	update();
}
