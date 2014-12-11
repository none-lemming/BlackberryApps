/*
 * PocketNao.cpp
 *
 *  Created on: 2014年8月29日
 *      Author: qGG
 */

#include <src/PocketNao.h>
#include <bb/data/JsonDataAccess>
#include <bb/system/Clipboard>
#include <bb/utility/ImageConverter>

using namespace bb::cascades;
using namespace bb::data;

const QString consumer_key = "31559-b80a1734a8834db84c7efe9d";
const int page_size = 15;

PocketNao::PocketNao(QObject* parent)
    :   QObject(parent)
{

}

QString PocketNao::token()
{
    return m_token;
}

void PocketNao::setToken(QString token)
{
    m_token = token;
    if (m_token.isEmpty())
        emit pocketAuthRequire();
}

void PocketNao::addUrl(QString url, QString title, QString tags)
{
    QString addUrl("https://getpocket.com/v3/add");
    QDict dict;
    dict.append(QPair<QString,QString>("consumer_key", consumer_key));
    dict.append(QPair<QString,QString>("access_token", m_token));
    dict.append(QPair<QString,QString>("url", url));
    dict.append(QPair<QString,QString>("title", title));
    dict.append(QPair<QString,QString>("tags", tags));

    ReplyWrap* wrap = Nao::POST(addUrl, dict);
    connect(wrap, SIGNAL(finished()), this, SLOT(onAddUrl()));
}

void PocketNao::onAddUrl()
{
    ReplyWrap* wrap = static_cast<ReplyWrap*>(QObject::sender());
    QNetworkReply* reply = wrap->reply();
    if (!reply->error())
    {
        qDebug()<<"!error";
        JsonDataAccess jda;
        QVariant response = jda.loadFromBuffer(reply->readAll());
        QVariantMap map = response.toMap();
        if (map["status"].toInt() == 1)
        {
            emit addUrlOk(map["item"]);
        }
        else
        {
            emit addUrlError("eror: status="+map["status"].toString());
        }
    }
    else
    {
        qDebug()<<reply->errorString();
        emit addUrlError(reply->errorString());
    }
    wrap->deleteLater();
    reply->deleteLater();
}

void PocketNao::retrieveItems(QDict params, QVariant addition)
{
    QString getUrl("https://getpocket.com/v3/get");
    params.append(QPair<QString,QString>("consumer_key", consumer_key));
    params.append(QPair<QString,QString>("access_token", m_token));
    params.append(QPair<QString,QString>("detailType", "complete"));
    params.append(QPair<QString,QString>("sort", "newest"));
    params.append(QPair<QString,QString>("count", QString::number(page_size)));
    ReplyWrap* wrap = Nao::POST(getUrl, params);
    wrap->setAddition(addition);
    connect(wrap, SIGNAL(finished()), this, SLOT(onRetrieveItems()));
    qDebug()<<"retrieveItems";
}

void PocketNao::onRetrieveItems()
{
    qDebug()<<"onRetrieveItems";
    ReplyWrap* wrap = static_cast<ReplyWrap*>(QObject::sender());
    QNetworkReply* reply = wrap->reply();
    if (!reply->error())
    {
        qDebug()<<"!error";
        JsonDataAccess jda;
        QString buff = reply->readAll();
        qDebug()<<buff;
        //paste(buff);
        QVariant response = jda.loadFromBuffer(buff);
        QVariantMap map = response.toMap();
        int status = map["status"].toInt();
        if (status == 1)
        {
            qDebug()<<map["list"].toMap().size();
            emit retrieveItemsOk(map["list"].toMap().values(), wrap->addition());
        }
        else if (status == 2)
        {
            emit retrieveItemsComplete(wrap->addition());
        }
        else
        {
            emit retrieveItemsError("eror: status="+map["status"].toString(), wrap->addition());
        }
    }
    else
    {
        qDebug()<<reply->errorString();
        emit retrieveItemsError(reply->errorString(), wrap->addition());
    }
    wrap->deleteLater();
    reply->deleteLater();
}

void PocketNao::retrieveAllItems(QVariant addition, int offset)
{
    QDict dict;
    dict.append(QPair<QString,QString>("state", "all"));
    dict.append(QPair<QString,QString>("offset", QString::number(offset)));
    retrieveItems(dict, addition);
}

void PocketNao::retrieveUnreadItems(QVariant addition, int offset)
{
    QDict dict;
    dict.append(QPair<QString,QString>("state", "unread"));
    dict.append(QPair<QString,QString>("offset", QString::number(offset)));
    retrieveItems(dict, addition);
}

void PocketNao::retrieveFavotiteItems(QVariant addition, int offset)
{
    QDict dict;
    dict.append(QPair<QString,QString>("favorite", "1"));
    dict.append(QPair<QString,QString>("offset", QString::number(offset)));
    retrieveItems(dict, addition);
}

void PocketNao::retrieveArchiveItems(QVariant addition, int offset)
{
    QDict dict;
    dict.append(QPair<QString,QString>("state", "archive"));
    dict.append(QPair<QString,QString>("offset", QString::number(offset)));
    retrieveItems(dict, addition);
}

void PocketNao::modifyItems(QVariant actions, QVariant addition)
{
    QString modifyUrl("https://getpocket.com/v3/send");
    QString jsonBuffer;
    JsonDataAccess jda;
    jda.saveToBuffer(actions, &jsonBuffer);
    if (actions.type() == QVariant::Map)
    {
        jsonBuffer.prepend('[');
        jsonBuffer.append(']');
    }
    qDebug()<<jsonBuffer;

    QDict dict;
    dict.append(QPair<QString,QString>("consumer_key", consumer_key));
    dict.append(QPair<QString,QString>("access_token", m_token));
    dict.append(QPair<QString,QString>("actions", jsonBuffer));
    ReplyWrap* wrap = Nao::POST(modifyUrl, dict);
    wrap->setAddition(addition);
    connect(wrap, SIGNAL(finished()), this, SLOT(onModifyItems()));
}

void PocketNao::onModifyItems()
{
    ReplyWrap* wrap = static_cast<ReplyWrap*>(QObject::sender());
    QNetworkReply* reply = wrap->reply();
    if (!reply->error())
    {
        qDebug()<<"!error";
        JsonDataAccess jda;
        QString buff = reply->readAll();
        qDebug()<<buff;
        QVariant response = jda.loadFromBuffer(buff);
        QVariantMap map = response.toMap();
    }
    else
    {
        qDebug()<<reply->errorString();
    }
    wrap->deleteLater();
    reply->deleteLater();
}
