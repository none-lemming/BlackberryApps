/*
 * ImageCache.h
 *
 *  Created on: 2014年8月31日
 *      Author: qGG
 */

#ifndef IMAGECACHE_H_
#define IMAGECACHE_H_

#include <QObject>
#include <bb/cascades/Image>
#include "Nao.h"

//高级功能,设置缓存大小,删除规则,保存及恢复缓存,
class ImageCache: public QObject
{
    Q_OBJECT
public:
    ImageCache(QObject* parent=0);

public slots:
    QVariant retrieve(QString url, QVariant addition=QVariant());
    void downloadImage(QString url, QVariant addition);

signals:
    void retrieveImageOk(bb::cascades::Image image, QVariant addition);
    void retrieveImageError(QString error, QVariant addition);

private slots:
    void onDownloadImage();

private:
    QVariantMap m_cache;
    QVariant m_defaultImage;
};

#endif /* IMAGECACHE_H_ */
