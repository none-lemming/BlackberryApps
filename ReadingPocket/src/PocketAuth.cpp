/*
 * PocketAuth.cpp
 *
 *  Created on: 2014年8月28日
 *      Author: qGG
 */

#include "PocketAuth.h"
#include "Nao.h"
#include <bb/data/JsonDataAccess>

using namespace bb::cascades;
using namespace bb::data;

const QString consumer_key = "31559-b80a1734a8834db84c7efe9d";
const QString redirect_uri = "lemming:pocket/pocketAuthCallback";

PocketAuth::PocketAuth(QObject* parent)
    :   QObject(parent)
    ,   m_status(-1)
{
    setStatus(0);
}

void PocketAuth::getCode()
{
    setStatus(1);
    QString getCodeUrl("https://getpocket.com/v3/oauth/request");
    QDict dict;
    dict.append(QPair<QString,QString>("consumer_key", consumer_key));
    dict.append(QPair<QString,QString>("redirect_uri", redirect_uri));
    ReplyWrap* wrap = Nao::POST(getCodeUrl, dict);
    connect(wrap, SIGNAL(finished()), this, SLOT(onGetCode()));
}

QString PocketAuth::getAuthUrl()
{
    setStatus(2);
    return QString("https://getpocket.com/auth/authorize?request_token=%1&redirect_uri=%2").arg(m_code).arg(redirect_uri);
}

void PocketAuth::getToken()
{
    setStatus(3);
    QString getCodeUrl("https://getpocket.com/v3/oauth/authorize");
    QDict dict;
    dict.append(QPair<QString,QString>("consumer_key", consumer_key));
    dict.append(QPair<QString,QString>("code", m_code));
    ReplyWrap* wrap = Nao::POST(getCodeUrl, dict);
    connect(wrap, SIGNAL(finished()), this, SLOT(onGetToken()));
}

void PocketAuth::onGetCode()
{
    qDebug()<<"onGetCode";
    ReplyWrap* wrap = static_cast<ReplyWrap*>(QObject::sender());
    QNetworkReply* reply = wrap->reply();
    if (!reply->error())
    {
        qDebug()<<"!error";
        JsonDataAccess jda;
        QVariant response = jda.loadFromBuffer(reply->readAll());
        QVariantMap map = response.toMap();
        qDebug()<<map;
        m_code = map["code"].toString();
        emit codeGot(m_code);
    }
    else
    {
        qDebug()<<reply->errorString();
        setStatus(5);
        emit authError(reply->errorString());
    }
    wrap->deleteLater();
    reply->deleteLater();
}

void PocketAuth::onGetToken()
{
    qDebug()<<"onGetToken";
    ReplyWrap* wrap = static_cast<ReplyWrap*>(QObject::sender());
    QNetworkReply* reply = wrap->reply();
    if (!reply->error())
    {
        qDebug()<<"!error";
        JsonDataAccess jda;
        QVariant response = jda.loadFromBuffer(reply->readAll());
        QVariantMap map = response.toMap();
        qDebug()<<map;
        setStatus(4);
        emit tokenGot(map["access_token"].toString(), map["username"].toString());
    }
    else
    {
        qDebug()<<reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        setStatus(5);
        emit authError(reply->errorString());
    }
    wrap->deleteLater();
    reply->deleteLater();
}

int PocketAuth::status()
{
    return m_status;
}

void PocketAuth::setStatus(int status)
{
    if (status == m_status)
        return;
    m_status = status;
    emit statusChanged(status);
}
