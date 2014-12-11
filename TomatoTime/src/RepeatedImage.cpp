/*
 * RepeatedImage.cpp
 *
 *  Created on: 2014-5-11
 *      Author: qGG
 */

#include "RepeatedImage.h"
#include <bb/cascades/AbsoluteLayout>
#include <bb/cascades/StackLayout>
#include <bb/cascades/Container>
#include <bb/cascades/ImageView>

using namespace bb::cascades;

RepeatedImage::RepeatedImage(Container *parent)
	: 	CustomControl(parent)
	,	m_count(0)
	,	m_imageWidth(0)
	,	m_imageHeight(0)
{
	StackLayout *pStackLayout = new StackLayout();
	pStackLayout->setOrientation( LayoutOrientation::LeftToRight );
	m_rootContainer = Container::create().layout(pStackLayout);

	setRoot(m_rootContainer);
}

RepeatedImage::~RepeatedImage() {
}

int RepeatedImage::count()
{
	return m_count;
}

void RepeatedImage::setCount(int count)
{
	qDebug()<<count;
	if (m_count == count || count<0)
		return;
	for (; m_count < count; m_count++)
	{
		increaseImage();
	}
	for (; m_count > count; m_count--)
	{
		decreaseImage();
	}
	setPreferredSize(m_count*m_imageWidth, m_imageHeight);
}

QString RepeatedImage::imageSource()
{
	return m_imageSource;
}

void RepeatedImage::setImageSource(QString source)
{
	if (m_imageSource == source)
		return;
	m_imageSource = source;
	for (int i=0; i<m_rootContainer->count(); i++)
	{
		m_rootContainer->at(i)->setProperty("imageSource", source);
	}
}

int RepeatedImage::imageWidth()
{
	return m_imageWidth;
}

void RepeatedImage::setImageWidth(int width)
{
	if (m_imageWidth == width)
		return;
	qDebug()<<width;
	m_imageWidth = width;
	m_rootContainer->setPreferredWidth(m_imageWidth*m_count);
	for (int i=0; i<m_rootContainer->count(); i++)
	{
		m_rootContainer->at(i)->setPreferredWidth(m_imageWidth);
	}
}

int RepeatedImage::imageHeight()
{
	return m_imageHeight;
}

void RepeatedImage::setImageHeight(int height)
{
	if (m_imageHeight == height)
		return;
	qDebug()<<height;
	m_imageHeight = height;
	m_rootContainer->setPreferredHeight(m_imageHeight);
	for (int i=0; i<m_rootContainer->count(); i++)
	{
		m_rootContainer->at(i)->setPreferredHeight(m_imageHeight);
	}
}

void RepeatedImage::increaseImage()
{
	ImageView* tmp = new ImageView();
	tmp->setPreferredSize(m_imageWidth, m_imageHeight);
	tmp->setImageSource(m_imageSource);
	m_rootContainer->add(tmp);
}

void RepeatedImage::decreaseImage()
{
	if (m_rootContainer->count() > 0)
		m_rootContainer->remove(m_rootContainer->at(0));
}
