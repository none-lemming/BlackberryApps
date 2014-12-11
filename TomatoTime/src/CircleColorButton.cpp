/*
 * CircleColorButton.cpp
 *
 *  Created on: 2014-5-9
 *      Author: qGG
 */

#include "CircleColorButton.h"
#include <bb/cascades/Container>
#include <bb/cascades/ImageView>
using namespace bb::cascades;

CircleColorButton::CircleColorButton(Container *parent)
	: CircleButton(parent)
	, m_widthO(0)
{
}

CircleColorButton::~CircleColorButton()
{
}

Image CircleColorButton::createColorCircle(const Color& color)
{
	QImage qimage(m_width, m_height, QImage::Format_ARGB32);
	qimage.fill(Qt::transparent);

	QPainter painter(&qimage);
	painter.setRenderHint(QPainter::Antialiasing, true);
	//painter.setPen(Qt::NoPen);

	painter.setPen(QPen(QColor::fromRgbF(m_colorO.red(),m_colorO.green(),m_colorO.blue(),m_colorO.alpha()), m_widthO, Qt::SolidLine, Qt::FlatCap));
	painter.setBrush(QBrush(QColor::fromRgbF(color.red(),color.green(),color.blue(),color.alpha()), Qt::SolidPattern));
	painter.drawEllipse(m_widthO/2+5, m_widthO/2+5, m_width-m_widthO-5, m_height-m_widthO-5);
	//painter.drawArc(m_penWidth/2+10, m_penWidth/2+10, m_width-m_penWidth-10, m_height-m_penWidth-10, 90*16, -16*360*m_value/m_duration);
	//painter.setPen(Qt::black);
	//painter.setBrush(Qt::NoBrush);
	//painter.drawRect(0, 0, m_width, m_height);

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
	return Image(imageData);
}

Color CircleColorButton::colorU()
{
	return m_colorU;
}

void CircleColorButton::setColorU(const Color& color)
{
	qDebug()<<"setColorU";
	if (color == m_colorU)
		return;
	m_colorU = color;
	m_outerImageU = createColorCircle(m_colorU);
	if (m_status == Up)
	{
		qDebug()<<"setImage m_outerImageU";
		m_circle->setImage(m_outerImageU);
	}
}

Color CircleColorButton::colorD()
{
	return m_colorD;
}

void CircleColorButton::setColorD(const Color& color)
{
	if (color == m_colorD)
		return;
	m_colorD = color;
	m_outerImageD = createColorCircle(m_colorD);
	if (m_status == Down)
		m_circle->setImage(m_outerImageD);
}

Color CircleColorButton::colorO()
{
	return m_colorO;
}

void CircleColorButton::setColorO(const Color& color)
{
	if (color == m_colorO)
		return;
	m_colorO = color;
	m_outerImageD = createColorCircle(m_colorD);
	m_outerImageU = createColorCircle(m_colorU);
	if (m_status == Down)
		m_circle->setImage(m_outerImageD);
	else
		m_circle->setImage(m_outerImageU);
}

int CircleColorButton::widthO()
{
	return m_widthO;
}

void CircleColorButton::setWidthO(int width)
{
	if (m_widthO == width)
		return;
	m_widthO = width;
	m_outerImageD = createColorCircle(m_colorD);
	m_outerImageU = createColorCircle(m_colorU);
	if (m_status == Down)
		m_circle->setImage(m_outerImageD);
	else
		m_circle->setImage(m_outerImageU);
}
