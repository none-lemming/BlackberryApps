/*
 * CircleColorButton.h
 *
 *  Created on: 2014-5-9
 *      Author: qGG
 */

#ifndef CIRCLECOLORBUTTON_H_
#define CIRCLECOLORBUTTON_H_

#include "CircleButton.h"
#include <bb/cascades/Color>

class CircleColorButton  : public CircleButton
{
	Q_OBJECT

	Q_PROPERTY(bb::cascades::Color colorU READ colorU WRITE setColorU);
	Q_PROPERTY(bb::cascades::Color colorD READ colorD WRITE setColorD);
	Q_PROPERTY(bb::cascades::Color colorO READ colorO WRITE setColorO);
	Q_PROPERTY(int widthO READ widthO WRITE setWidthO);

public:
	CircleColorButton(bb::cascades::Container *parent = 0);
	virtual ~CircleColorButton();

private:
	bb::cascades::Image createColorCircle(const bb::cascades::Color& color);

	bb::cascades::Color colorU();
	void setColorU(const bb::cascades::Color& color);
	bb::cascades::Color colorD();
	void setColorD(const bb::cascades::Color& color);
	bb::cascades::Color colorO();
	void setColorO(const bb::cascades::Color& color);
	int widthO();
	void setWidthO(int width);

	bb::cascades::Color m_colorU;
	bb::cascades::Color	m_colorD;
	bb::cascades::Color m_colorO;
	int m_widthO;
};

#endif /* CIRCLECOLORBUTTON_H_ */
