/*
 * CircleButton.h
 *
 *  Created on: 2014-5-7
 *      Author: qGG
 */

#ifndef CIRCLEBUTTON_H_
#define CIRCLEBUTTON_H_

#include <bb/cascades/CustomControl>
#include <bb/cascades/Image>

namespace bb {
namespace cascades {
class Container;
class ImageView;
class TouchEvent;
}
}

class CircleButton : public bb::cascades::CustomControl
{
	Q_OBJECT
	Q_PROPERTY(Status status READ status WRITE setStatus NOTIFY statusChanged);
	Q_PROPERTY(QString outerSourceU READ outerSourceU WRITE setOuterSourceU);
	Q_PROPERTY(QString outerSourceD READ outerSourceD WRITE setOuterSourceD);
	Q_PROPERTY(bb::cascades::Control* content READ content WRITE setContent);
	Q_PROPERTY(float width READ width WRITE setWidth);
	Q_PROPERTY(float height READ height WRITE setHeight);
	Q_PROPERTY(float innerScaleW READ innerScaleW WRITE setInnerScaleW);
	Q_PROPERTY(float innerScaleH READ innerScaleH WRITE setInnerScaleH);
	Q_PROPERTY(float outerScaleW READ outerScaleW WRITE setOuterScaleW);
	Q_PROPERTY(float outerScaleH READ outerScaleH WRITE setOuterScaleH);
	Q_PROPERTY(float innerScaleD READ innerScaleD WRITE setInnerScaleD);
	Q_PROPERTY(float outerScaleD READ outerScaleD WRITE setOuterScaleD);

	Q_ENUMS(Status);

public:
	CircleButton(bb::cascades::Container *parent = 0);
	virtual ~CircleButton();

	enum Status{
		Up,
		Down,
		Move
	};

signals:
	void statusChanged(Status status);
	void clicked();

protected Q_SLOTS:
	virtual void onButtonTouched(bb::cascades::TouchEvent *touchEvent);
    void onWidthChanged(float width);
    void onHeightChanged(float height);

protected:
    virtual void onSizeChanged();
    virtual void onInnerSizeChanged();
    virtual void onOuterSizeChanged();

    //property getter & setter
    Status status();
    void setStatus(Status status);
    QString outerSourceU();
    void setOuterSourceU(QString source);
    QString outerSourceD();
    void setOuterSourceD(QString source);
    bb::cascades::Control* content();
    void setContent(bb::cascades::Control* content);
    float width();
    void setWidth(float width);
    float height();
    void setHeight(float height);
    float innerScaleW();
    void setInnerScaleW(float scale);
    float innerScaleH();
    void setInnerScaleH(float scale);
    float outerScaleW();
    void setOuterScaleW(float scale);
    float outerScaleH();
    void setOuterScaleH(float scale);
    float innerScaleD();
    void setInnerScaleD(float scale);
    float outerScaleD();
    void setOuterScaleD(float scale);

    //container
    bb::cascades::Container *m_rootContainer;//一个圈(图片),加一个容器(居中)
    bb::cascades::Container *m_handleContainer;
    bb::cascades::ImageView* m_circle;

    //source
    QString m_outerSourceU;
    QString m_outerSourceD;
    bb::cascades::Image m_outerImageU;
    bb::cascades::Image m_outerImageD;
    bb::cascades::Control* m_content;

    //appearance
    float m_width;
    float m_height;
    float m_innerScaleW;
    float m_innerScaleH;
    float m_outerScaleW;
    float m_outerScaleH;
    float m_innerScaleD;
    float m_outerScaleD;



    Status m_status;
};

#endif /* CIRCLEBUTTON_H_ */
