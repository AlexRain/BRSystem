#include "WaitWindow.h"
//#include <QMovie>
//
//WaitWindow::WaitWindow(QWidget* parent)
//	: BaseDialog(parent) {
//
//	ui.setupUi(this);
//
//	setWindowFlags(Qt::FramelessWindowHint);//ÎÞ±ß¿ò
//	setAttribute(Qt::WA_TranslucentBackground);//±³¾°Í¸Ã÷
//	setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool | Qt::X11BypassWindowManagerHint);
//	this->setWindowOpacity(1);    //ÉèÖÃ´°¿ÚÍ¸Ã÷¶È
//
//	////ÆÁÄ»¾ÓÖÐÏÔÊ¾
//	//int frmX = width();
//	//int frmY = height();
//
//	//int deskWidth = parent->width();
//	//int deskHeight = parent->height();
//
//	//QPoint movePoint(deskWidth / 2 - frmX / 2, deskHeight / 2 - frmY / 2);
//	//move(movePoint);
//
//	//¼ÓÔØgifÍ¼Æ¬
//	QLabel* lbl_gif = new QLabel();
//	QMovie *movie = new QMovie("images/loading.gif");
//	lbl_gif->setMovie(movie);
//	movie->start();
//}
//WaitWindow::~WaitWindow()
//{
//}

#include "WaitWindow.h"

#include <QPainter>

WaitWindow::WaitWindow(QWidget* parent)
	: QWidget(parent),
	m_angle(0),
	m_timerId(-1),
	m_delay(40),
	m_displayedWhenStopped(false),
	m_color(Qt::black)
{
	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	setFocusPolicy(Qt::NoFocus);
}

bool WaitWindow::isAnimated() const
{
	return (m_timerId != -1);
}

void WaitWindow::setDisplayedWhenStopped(bool state)
{
	m_displayedWhenStopped = state;

	update();
}

bool WaitWindow::isDisplayedWhenStopped() const
{
	return m_displayedWhenStopped;
}

void WaitWindow::startAnimation()
{
	m_angle = 0;

	if (m_timerId == -1)
		m_timerId = startTimer(m_delay);
}

void WaitWindow::stopAnimation()
{
	if (m_timerId != -1)
		killTimer(m_timerId);

	m_timerId = -1;

	update();
}

void WaitWindow::setAnimationDelay(int delay)
{
	if (m_timerId != -1)
		killTimer(m_timerId);

	m_delay = delay;

	if (m_timerId != -1)
		m_timerId = startTimer(m_delay);
}

void WaitWindow::setColor(const QColor & color)
{
	m_color = color;

	update();
}

QSize WaitWindow::sizeHint() const
{
	return QSize(20, 20);
}

int WaitWindow::heightForWidth(int w) const
{
	return w;
}

void WaitWindow::timerEvent(QTimerEvent * /*event*/)
{
	m_angle = (m_angle + 30) % 360;

	update();
}

void WaitWindow::paintEvent(QPaintEvent * /*event*/)
{
	if (!m_displayedWhenStopped && !isAnimated())
		return;

	int width = qMin(this->width(), this->height());

	QPainter p(this);
	p.setRenderHint(QPainter::Antialiasing);

	int outerRadius = (width - 1)*0.5;
	int innerRadius = (width - 1)*0.5*0.38;

	int capsuleHeight = outerRadius - innerRadius;
	int capsuleWidth = (width > 32) ? capsuleHeight * .23 : capsuleHeight * .35;
	int capsuleRadius = capsuleWidth / 2;

	for (int i = 0; i < 12; i++)
	{
		QColor color = m_color;
		color.setAlphaF(1.0f - (i / 12.0f));
		p.setPen(Qt::NoPen);
		p.setBrush(color);
		p.save();
		p.translate(rect().center());
		p.rotate(m_angle - i * 30.0f);
		p.drawRoundedRect(-capsuleWidth * 0.5, -(innerRadius + capsuleHeight), capsuleWidth, capsuleHeight, capsuleRadius, capsuleRadius);
		p.restore();
	}
}