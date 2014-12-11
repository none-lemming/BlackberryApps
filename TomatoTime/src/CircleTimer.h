/*
 * CircleTimer.h
 *
 *  Created on: 2014-5-8
 *      Author: qGG
 */

#ifndef CIRCLETIMER_H_
#define CIRCLETIMER_H_

#include "CircleButton.h"
#include <bb/cascades/Color>
class QTimer;

class CircleTimer : public CircleButton
{
	Q_OBJECT

	Q_PROPERTY(int value READ value WRITE setValue NOTIFY valueChanged);
	Q_PROPERTY(int duration READ duration  WRITE setDuration NOTIFY durationChanged);
	Q_PROPERTY(int penWidth READ penWidth WRITE setPenWidth);
	Q_PROPERTY(bb::cascades::Color color READ color WRITE setColor);
	Q_PROPERTY(int warpWidth READ warpWidth WRITE setWarpWidth);
	Q_PROPERTY(bb::cascades::Color warpColor READ warpColor WRITE setWarpColor);

Q_SIGNALS:
	void valueChanged(int value);
	void durationChanged();
	void finished();
	void started();

protected Q_SLOTS:
	void onTimeout();
	virtual void onButtonTouched(bb::cascades::TouchEvent *touchEvent);

public:
	CircleTimer(bb::cascades::Container *parent = 0);
	virtual ~CircleTimer();

	Q_INVOKABLE void start();
	Q_INVOKABLE void stop();
	Q_INVOKABLE void reset(int value=0);

private:
	void updateImage();

	//getter & setter
	int value();
	void setValue(int value);
	int duration();
	void setDuration(int duration);
	int penWidth();
	void setPenWidth(int width);
	bb::cascades::Color color();
	void setColor(const bb::cascades::Color& color);
	int warpWidth();
	void setWarpWidth(int width);
	bb::cascades::Color warpColor();
	void setWarpColor(const bb::cascades::Color& color);

	//bb::cascades::Image m_image;
	QTimer m_timer;
	int m_value;
	int m_duration;
	int m_penWidth;
	int m_wrapWidth;
	QColor m_color;
	QColor m_wrapColor;
};

#endif /* CIRCLETIMER_H_ */
