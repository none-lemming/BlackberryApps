/*
 * ImageCache.cpp
 *
 *  Created on: 2014年8月31日
 *      Author: qGG
 */

#include <src/ImageCache.h>
#include <bb/utility/ImageConverter>

using namespace bb::cascades;

ImageCache::ImageCache(QObject* parent)
    :   QObject(parent)
{
}

QVariant ImageCache::retrieve(QString url, QVariant addition)
{
    if (m_cache.contains(url))
    {
        qDebug()<<"return image cache";
        return m_cache[url];
    }
    qDebug()<<"retrieve: " + url;
    QVariantMap map;
    map["addition"] = addition;
    map["url"] = url;
    downloadImage(url, map);
    return m_defaultImage;
}

void ImageCache::downloadImage(QString url, QVariant addition)
{
    //qDebug()<<"image: "<<url;
    ReplyWrap* wrap = Nao::GET(url);
    wrap->setAddition(addition);
    connect(wrap, SIGNAL(finished()), this, SLOT(onDownloadImage()));
}

void ImageCache::onDownloadImage()
{
    ReplyWrap* wrap = static_cast<ReplyWrap*>(QObject::sender());
    QNetworkReply* reply = wrap->reply();
    if (!reply->error())
    {
        QByteArray data = reply->readAll();
        Image image = Image(bb::utility::ImageConverter::decode(data));
        QVariantMap map = wrap->addition().toMap();
        m_cache[map["url"].toString()] = QVariant::fromValue(image);
        emit retrieveImageOk(image, map["addition"]);
    }
    else
    {
        qDebug()<<reply->errorString();
        emit retrieveImageError(reply->errorString(), wrap->addition());
    }
    wrap->deleteLater();
    reply->deleteLater();
}
