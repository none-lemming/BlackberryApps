/*
 * CircleButton.cpp
 *
 *  Created on: 2014-5-7
 *      Author: qGG
 */

#include "CircleButton.h"
#include <bb/cascades/AbsoluteLayout>
#include <bb/cascades/Container>
#include <bb/cascades/DockLayout>
#include <bb/cascades/ImageView>
#include <bb/cascades/TouchEvent>

using namespace bb::cascades;

CircleButton::CircleButton(Container *parent)
	: 	CustomControl(parent)
	,	m_content(NULL)
	,	m_width(150)
	,	m_height(150)
	,	m_innerScaleW(0.5)
	,	m_innerScaleH(0.5)
	,	m_outerScaleW(1)
	,	m_outerScaleH(1)
	,	m_innerScaleD(1)
	,	m_outerScaleD(1)
	, 	m_status(Up)

{
	//qDebug()<<"CircleButton creating...";
	m_rootContainer = new Container();
	m_rootContainer->setLayout(new AbsoluteLayout());

	m_circle = new ImageView();
	//m_circle->setImage(Image(QUrl("asset:///images/slider_track.png")));
	//m_circle = ImageView::create().image(QUrl("asset:///images/slider_track.png"));

	m_handleContainer = Container::create().layout(new DockLayout());

	m_rootContainer->add(m_circle);
	m_rootContainer->add(m_handleContainer);

	setRoot(m_rootContainer);

	bool ok = connect(this, SIGNAL(preferredHeightChanged(float)), this, SLOT(onHeightChanged(float)));
	Q_ASSERT(ok);
	ok = connect(this, SIGNAL(preferredWidthChanged(float)), this, SLOT(onWidthChanged(float)));
	Q_ASSERT(ok);

	m_width = 150;
	m_height = 150;
	setPreferredSize(m_width, m_height);
	//qDebug()<<m_width<<" "<<m_height;

	ok = connect(m_rootContainer, SIGNAL(touch(bb::cascades::TouchEvent*)), this, SLOT(onButtonTouched(bb::cascades::TouchEvent*)));
	Q_ASSERT(ok);
}

CircleButton::~CircleButton()
{
}

void CircleButton::onWidthChanged(float width)
{
    m_width = width;
    onSizeChanged();
}

void CircleButton::onHeightChanged(float height)
{
    m_height = height;
    onSizeChanged();
}

void CircleButton::onSizeChanged()
{
	//qDebug()<<"CircleButton onSizeChanged";
	m_rootContainer->setPreferredSize(m_width, m_height);
	onInnerSizeChanged();
	onOuterSizeChanged();
}

void CircleButton::onOuterSizeChanged()
{
	//qDebug()<<"CircleButton onOuterSizeChanged";
	m_circle->setPreferredSize(m_width * m_outerScaleW, m_height * m_outerScaleH);
	//让元素居中
	m_circle->setTranslation((m_width - m_outerScaleW * m_width) / 2, (m_height - m_outerScaleH * m_height) / 2);
}

void CircleButton::onInnerSizeChanged()
{
	//qDebug()<<"CircleButton onInnerSizeChanged";
	m_handleContainer->setPreferredSize(m_width * m_innerScaleW, m_height * m_innerScaleH);
	m_handleContainer->setTranslation((m_width - m_innerScaleW * m_width) / 2, (m_height - m_innerScaleH * m_height) / 2);
}

void CircleButton::onButtonTouched(bb::cascades::TouchEvent *touchEvent)
{
	//qDebug()<<"CircleButton onButtonTouched";
	if (touchEvent->isDown())
	{
		if (!m_outerImageD.isNull())
			m_circle->setImage(m_outerImageD);
	    m_circle->setScale(m_outerScaleD,m_outerScaleD);
	    m_handleContainer->setScale(m_innerScaleD,m_innerScaleD);
	    setStatus(Down);
	}
	else if (touchEvent->isCancel() || touchEvent->isUp())
	{
		m_circle->setImage(m_outerImageU);
		m_circle->resetScale();
		m_handleContainer->resetScale();
		setStatus(Up);
		if (touchEvent->isUp())
			emit clicked();
	}
}

//////////////////////////////////////////////////////////

CircleButton::Status CircleButton::status()
{
	return m_status;
}

void CircleButton::setStatus(Status status)
{
	if (m_status == status)
		return;
	m_status = status;
	emit statusChanged(m_status);
}

QString CircleButton::outerSourceU()
{
	return m_outerSourceU;
}

void CircleButton::setOuterSourceU(QString source)
{
	m_outerSourceU = source;
	m_outerImageU = Image(QUrl(m_outerSourceU));
	if (m_status == Up)
		m_circle->setImage(m_outerImageU);
}

QString CircleButton::outerSourceD()
{
	return m_outerSourceD;
}

void CircleButton::setOuterSourceD(QString source)
{
	m_outerSourceD = source;
	m_outerImageD = Image(QUrl(m_outerSourceD));
	if (m_status == Down)
		m_circle->setImage(m_outerImageD);
}

bb::cascades::Control* CircleButton::content()
{
	return m_content;
}

void CircleButton::setContent(bb::cascades::Control* content)
{
	if (m_content == content)
		return;
	m_content = content;
	//only one control
	m_handleContainer->removeAll();
	m_handleContainer->add(content);
}

float CircleButton::width()
{
	return m_width;
}
void CircleButton::setWidth(float width)
{
	if (m_width == width)
		return;
	m_width = width;
	setPreferredSize(m_width, m_height);
}

float CircleButton::height()
{
	return m_height;
}
void CircleButton::setHeight(float height)
{
	if (m_height == height)
		return;
	m_height = height;
	setPreferredSize(m_width, m_height);
}

float CircleButton::innerScaleW()
{
	return m_innerScaleW;
}
void CircleButton::setInnerScaleW(float scale)
{
	if (m_innerScaleW == scale)
		return;
	m_innerScaleW = scale;
	onInnerSizeChanged();
}

float CircleButton::innerScaleH()
{
	return m_innerScaleH;
}
void CircleButton::setInnerScaleH(float scale)
{
	if (m_innerScaleH == scale)
		return;
	m_innerScaleH = scale;
	onInnerSizeChanged();
}

float CircleButton::outerScaleW()
{
	return m_outerScaleW;
}

void CircleButton::setOuterScaleW(float scale)
{
	if (m_outerScaleW == scale)
		return;
	m_outerScaleW = scale;
	onOuterSizeChanged();
}

float CircleButton::outerScaleH()
{
	return m_outerScaleH;
}
void CircleButton::setOuterScaleH(float scale)
{
	if (m_outerScaleH == scale)
		return;
	m_outerScaleH = scale;
	onOuterSizeChanged();
}

float CircleButton::innerScaleD()
{
	return m_innerScaleD;
}
void CircleButton::setInnerScaleD(float scale)
{
	if (m_innerScaleD == scale)
		return;
	m_innerScaleD = scale;
}

float CircleButton::outerScaleD()
{
	return m_outerScaleD;
}

void CircleButton::setOuterScaleD(float scale)
{
	if (m_outerScaleD == scale)
		return;
	m_outerScaleD = scale;
}
