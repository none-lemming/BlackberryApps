/*
 * Nao.cpp
 *
 *  Created on: 2014年8月28日
 *      Author: qGG
 */

#include "Nao.h"
#include <bb/cascades/QmlDocument>
#include <bb/device/WiFiDirect>
#include <bps/bps.h>
#include <bps/netstatus.h>

using namespace bb::cascades;
using namespace bb::device;

Nao::Nao(QObject* parent)
    :   QObject(parent)
{
}

QByteArray Nao::processParams(const QDict& params)
{
    QByteArray str;
    if (params.size() > 0)
        str.append(params[0].first + "=" + params[0].second);
    for (int i=1; i<params.size(); i++)
        str.append("&" + params[i].first + "=" + params[i].second);
    return str;
}

ReplyWrap* Nao::GET(QString url, const QDict& params)
{
    QString str = processParams(params);
    if (!str.isEmpty())
        url.append("?"+str);

    QNetworkRequest request;
    request.setUrl(QUrl(url));
    QNetworkReply* reply = QmlDocument::defaultDeclarativeEngine()->networkAccessManager()->get(request);

    ReplyWrap* wrap = new ReplyWrap();
    wrap->setReply(reply);
    return wrap;
}

ReplyWrap* Nao::POST(QString url, const QDict& params)
{
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    //request.setRawHeader("Authorization", "Bearer " + m_token);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded; charset=UTF-8");
    //request.setRawHeader("X-Accept", "application/json");

    QNetworkReply* reply = QmlDocument::defaultDeclarativeEngine()->networkAccessManager()->post(request, processParams(params));

    ReplyWrap* wrap = new ReplyWrap();
    wrap->setReply(reply);
    return wrap;
}

bool Nao::isNetworkEnable()
{
    bool is_available;

    bps_initialize();
    netstatus_get_availability(&is_available);
    return is_available;
}

bool Nao::isWifiConnected()
{
    WiFiDirect wifi;
    qDebug()<<wifi.connectionState();
    return wifi.connectionState()==WiFiDirectConnectionState::Connected;
}
