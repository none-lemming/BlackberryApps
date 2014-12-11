/*
 * Device.cpp
 *
 *  Created on: 2014-5-11
 *      Author: qGG
 */

#include "Device.h"

using namespace bb::device;

Device::Device(QObject* parent)
	: QObject(parent)
{

}

Device::~Device() {
}

void Device::startVibration(int milliseconds)
{
	m_vibration.start(50, milliseconds);
}

void Device::cancelVibration()
{
	m_vibration.stop();
}

void Device::startLed(LedColor::Type ledColor, int count)
{
	qDebug()<<"led start";
	m_led.setColor(ledColor);
	m_led.flash(count);
}

void Device::cancelLed()
{
	qDebug()<<"led cancel";
	m_led.cancel();
}
