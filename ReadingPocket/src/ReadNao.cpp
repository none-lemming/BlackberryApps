/*
 * ReadNao.cpp
 *
 *  Created on: 2014年8月31日
 *      Author: qGG
 */

#include <src/ReadNao.h>
#include <bb/data/JsonDataAccess>
#include <bb/system/Clipboard>
#include <bb/utility/ImageConverter>

using namespace bb::cascades;
using namespace bb::data;

ReadNao::ReadNao(QString token, QObject* parent)
    :   QObject(parent)
    ,   m_token(token)
{
}

void paste(QString content)
{
    bb::system::Clipboard clipboard;
    clipboard.clear();
    clipboard.insert("text/plain", content.toLocal8Bit());
}

QString ReadNao::token()
{
    return m_token;
}

void ReadNao::setToken(QString token)
{
    m_token = token;
}

QVariant ReadNao::parseUrl(QString url, QVariant addition)
{
    if (m_cache.contains(url))
    {
        //qDebug()<<"retrun article cache";
        return m_cache[url];
    }

    QString getUrl("http://www.readability.com/api/content/v1/parser");
    QDict dict;
    dict.append(QPair<QString,QString>("token", m_token));
    dict.append(QPair<QString,QString>("url", url));

    QVariantMap map;
    map["addition"] = addition;
    map["url"] = url;
    ReplyWrap* wrap = Nao::GET(getUrl, dict);
    wrap->setAddition(map);
    connect(wrap, SIGNAL(finished()), this, SLOT(onParseUrl()));
    return QVariant();
}

void ReadNao::onParseUrl()
{
    qDebug()<<"onParseUrl";
    ReplyWrap* wrap = static_cast<ReplyWrap*>(QObject::sender());
    QNetworkReply* reply = wrap->reply();
    if (!reply->error())
    {
        JsonDataAccess jda;
        QByteArray buff = reply->readAll();
        //qDebug()<<buff;
        QVariant response = jda.loadFromBuffer(buff);
        //paste(buff);
        QVariantMap map = wrap->addition().toMap();
        qDebug()<<"paste:"+map["url"].toString();
        m_cache[map["url"].toString()] = response;
        emit parseUrlOk(response, map["addition"]);
    }
    else
    {
        qDebug()<<reply->errorString();
        emit networkError(reply->errorString());
    }
    wrap->deleteLater();
    reply->deleteLater();
}
