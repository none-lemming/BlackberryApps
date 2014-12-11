/*
 * Device.h
 *
 *  Created on: 2014-5-11
 *      Author: qGG
 */

#ifndef DEVICE_H_
#define DEVICE_H_

#include <QObject>
#include <bb/device/VibrationController>
#include <bb/device/Led>
#include <bb/device/LedColor>

class Device : public QObject
{
	Q_OBJECT

public:
	Device(QObject* parent=0);
	virtual ~Device();

	Q_INVOKABLE void startVibration(int milliseconds);
	Q_INVOKABLE void cancelVibration();
	Q_INVOKABLE void startLed(bb::device::LedColor::Type ledColor=bb::device::LedColor::Red, int count=-1);
	Q_INVOKABLE void cancelLed();


	bb::device::VibrationController m_vibration;
	bb::device::Led m_led;
};

#endif /* DEVICE_H_ */
