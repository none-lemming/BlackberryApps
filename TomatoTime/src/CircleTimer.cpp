/*
 * CircleTimer.cpp
 *
 *  Created on: 2014-5-8
 *      Author: qGG
 */

#include "CircleTimer.h"
#include <bb/cascades/ImageView>
#include <bb/cascades/Container>
#include <Qt/QImage.h>
#include <Qt/QPainter.h>
#include <bb/ImageData>
#include <QTimer>
//#include <QtCore/qmath.h>
using namespace bb::cascades;

CircleTimer::CircleTimer(Container *parent)
	:	CircleButton(parent)
	,	m_value(0)
	,	m_duration(0)
	,	m_penWidth(0)
	, 	m_wrapWidth(0)
	,	m_color(Qt::transparent)
	,	m_wrapColor(Qt::transparent)
{
	m_timer.setInterval(1000);
	reset();
	bool ok = connect(&m_timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
	qDebug()<<ok;
}

CircleTimer::~CircleTimer()
{
}

void CircleTimer::onTimeout()
{
	//qDebug()<<"onTimeout";
	if (m_value+1 == m_duration)
	{
		m_timer.stop();
		setValue(m_value + 1);
		emit finished();
	}
	else if(m_value+1 > m_duration)
	{
		m_timer.stop();
		void finished();
	}
	else
	{
		setValue(m_value + 1);
	}
}

void CircleTimer::updateImage()
{
	QImage qimage(m_width, m_height, QImage::Format_ARGB32);
	qimage.fill(Qt::transparent);

	QPainter painter(&qimage);
	painter.setRenderHint(QPainter::Antialiasing, true);
	//绘图区域,需要考虑画笔的宽度
	QRect rect(m_penWidth/2+5, m_penWidth/2+5, m_width-m_penWidth-5, m_width-m_penWidth-5);

	//进度
	painter.setPen(QPen(m_color, m_penWidth-m_wrapWidth*2, Qt::SolidLine, Qt::FlatCap));
	painter.drawArc(rect, 90*16, -16*360*m_value/m_duration);

	//周圈
	painter.setPen(QPen(m_wrapColor, m_penWidth-4, Qt::SolidLine, Qt::FlatCap));
	painter.drawEllipse(rect);

	//dot
	//painter.setPen(Qt::NoPen);
	//painter.setBrush(QBrush(Qt::black, Qt::SolidPattern));
	//qreal r = (m_width-m_penWidth/2*3-10)/2;
	//qDebug()<<r;
	//qreal beta = -360*m_value/m_duration+90;
	//qreal beta = 2*3.14*m_value/m_duration - 3.14/4;
	//qDebug()<<beta;
	//painter.drawEllipse(QPointF(m_width/2 + r*qCos(beta), m_height/2 + r*qSin(beta)), m_penWidth,m_penWidth);

	bb::ImageData imageData = bb::ImageData::fromPixels(qimage.bits(), bb::PixelFormat::RGBA_Premultiplied,
				 qimage.width(), qimage.height(), qimage.bytesPerLine());
	//格式问题,不知道还有没有更加高效的方法
	unsigned char *dstLine = imageData.pixels();
	for (int y = 0; y < imageData.height(); y++) {
		unsigned char * dst = dstLine;
	    for (int x = 0; x < imageData.width(); x++) {
	    	QRgb srcPixel = qimage.pixel(x, y);
	        *dst++ = qRed(srcPixel);
	        *dst++ = qGreen(srcPixel);
	        *dst++ = qBlue(srcPixel);
	        *dst++ = qAlpha(srcPixel);
	    }
	    dstLine += imageData.bytesPerLine();
	}
	m_circle->setImage(Image(imageData));
}

void CircleTimer::onButtonTouched(bb::cascades::TouchEvent *touchEvent)
{
	if (touchEvent->isDown())
	{
	    m_circle->setScale(m_outerScaleD,m_outerScaleD);
	    m_handleContainer->setScale(m_innerScaleD,m_innerScaleD);
	    setStatus(Down);
	}
	else if (touchEvent->isUp())
	{
		m_circle->resetScale();
		m_handleContainer->resetScale();
		setStatus(Up);
		emit clicked();
	}
}

void CircleTimer::start()
{
	if (m_duration>0 && m_value<m_duration)
	{
		m_timer.start();
		emit started();
		qDebug()<<"start";
	}
}

void CircleTimer::stop()
{
	m_timer.stop();
}

void CircleTimer::reset(int value)
{
	m_timer.stop();
	setValue(value);
}

////////////////////////////////////////
int CircleTimer::value()
{
	return m_value;
}

void CircleTimer::setValue(int value)
{
	if (m_value == value)
		return;
	m_value = value;
	qDebug()<<m_value;
	emit valueChanged(m_value);
	updateImage();
}

int CircleTimer::duration()
{
	return m_duration;
}
void CircleTimer::setDuration(int duration)
{
	if (m_duration == duration)
		return;
	m_duration = duration;
	updateImage();
	emit durationChanged();
}

int CircleTimer::penWidth()
{
	return m_penWidth;
}

void CircleTimer::setPenWidth(int width)
{
	if (m_penWidth == width)
		return;
	m_penWidth = width;
	//updateImage();
}

Color CircleTimer::color()
{
	return Color::fromRGBA(m_color.redF(),m_color.greenF(),m_color.blueF(),m_color.alphaF());
}

void CircleTimer::setColor(const Color& color)
{
	QColor c = QColor::fromRgbF(color.red(),color.green(),color.blue(),color.alpha());
	if (m_color == c)
		return;
	m_color = c;
	updateImage();
}

int CircleTimer::warpWidth()
{
	return m_wrapWidth;
}

void CircleTimer::setWarpWidth(int width)
{
	if (m_wrapWidth == width)
		return;
	m_wrapWidth = width;
	//updateImage();
}

Color CircleTimer::warpColor()
{
	return Color::fromRGBA(m_wrapColor.redF(),m_wrapColor.greenF(),m_wrapColor.blueF(),m_wrapColor.alphaF());
}

void CircleTimer::setWarpColor(const Color& color)
{
	QColor c = QColor::fromRgbF(color.red(),color.green(),color.blue(),color.alpha());
	if (m_wrapColor == c)
		return;
	m_wrapColor = c;
	//updateImage();
}
