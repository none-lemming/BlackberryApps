/*
 * RepeatedImage.h
 *
 *  Created on: 2014-5-11
 *      Author: qGG
 */

#ifndef REPEATEDIMAGE_H_
#define REPEATEDIMAGE_H_

#include <bb/cascades/CustomControl>

namespace bb {
namespace cascades {
class Container;
class ImageView;
}
}

class RepeatedImage : public bb::cascades::CustomControl
{
	Q_OBJECT
	Q_PROPERTY(QString imageSource READ imageSource WRITE setImageSource);
	Q_PROPERTY(int count READ count WRITE setCount);
	Q_PROPERTY(int imageWidth READ imageWidth WRITE setImageWidth);
	Q_PROPERTY(int imageHeight READ imageHeight WRITE setImageHeight);

public:
	RepeatedImage(bb::cascades::Container *parent = 0);
	virtual ~RepeatedImage();

signals:
	void created();

protected:
	int count();
	void setCount(int count);
	QString imageSource();
	void setImageSource(QString source);
	int imageWidth();
	void setImageWidth(int width);
	int imageHeight();
	void setImageHeight(int height);

	void increaseImage();
	void decreaseImage();

	bb::cascades::Container *m_rootContainer;
	QString m_imageSource;
	int m_count;
	int m_imageWidth;
	int m_imageHeight;
};

#endif /* REPEATEDIMAGE_H_ */
