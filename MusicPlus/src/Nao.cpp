/*
 * Nao.cpp
 *
 *  Created on: 2014-6-22
 *      Author: qGG
 */

#include "Nao.h"
#include <bb/cascades/QmlDocument>
#include <bb/data/JsonDataAccess>
#include <QRegExp>
#include "ReplyWrap.h"

using namespace bb::cascades;
using namespace bb::data;

const QString Nao::BDSongPattern = "http://box.zhangmen.baidu.com/x?op=12&count=1&title=%1$$%2$$$$";
const QString Nao::BDLrcPattern = "http://box.zhangmen.baidu.com/bdlrc/%1/%2.lrc";
const QString Nao::DBSongPattern = "https://api.douban.com/v2/music/search?q=%1&count=1";
const QString Nao::DBImagePattern = "http://img3.douban.com/lpic/%1.jpg";

Nao::Nao(QObject* parent)
	:	QObject(parent)
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

QNetworkReply* Nao::GET(QString url, const QDict& params)
{
	//先处理参数,附加到url
	QString str = processParams(params);
	if (!str.isEmpty())
		url.append("?"+str);
	qDebug()<<url;
	QNetworkRequest request;
	request.setUrl(QUrl(url));
	return QmlDocument::defaultDeclarativeEngine()->networkAccessManager()->get(request);
}

void Nao::getLrc(const QString& songName, const QString& singer, const QVariant& addition)
{
	if (songName.isEmpty() && singer.isEmpty())
		return;
	QString url(BDSongPattern.arg(songName).arg(singer));
	QNetworkReply* reply = GET(url);

	ReplyWrap* wrap = new ReplyWrap();
	wrap->setAddition(addition);
	wrap->setReply(reply);
	bool ok = connect(wrap, SIGNAL(finished()), this,	SLOT(onBDSongReply()));
	Q_ASSERT(ok);
}

void Nao::onBDSongReply()
{
	qDebug()<<"onBDSongReply";
	ReplyWrap* wrap = static_cast<ReplyWrap*>(QObject::sender());
	QNetworkReply* reply = wrap->reply();
	if (!reply->error())
	{
		QString songInfo = reply->readAll();
		QRegExp rx("<lrcid>(\\d+)</lrcid>");
		if (songInfo.indexOf(rx) >= 0)// 匹配出lrcID且不为0
		{
			int lrcId = rx.cap(1).toInt();
			if (lrcId > 0)
			{
				QString url(BDLrcPattern.arg(lrcId/100).arg(lrcId));
				QNetworkReply* reply = GET(url);
				ReplyWrap* newWrap = new ReplyWrap();
				newWrap->setAddition(wrap->addition());
				newWrap->setReply(reply);
				bool ok = connect(newWrap, SIGNAL(finished()), this, SLOT(onBDLrcReply()));
				Q_ASSERT(ok);
			}
			else
			{
				qDebug()<<"-------------- no lrc";
				emit noLrc(wrap->addition());
			}
		}
		else
		{
			qDebug()<<"-------------- song not found";
			emit noLrc(wrap->addition());
		}
	}
	else
	{
		emit networkError(reply->errorString(), wrap->addition());
	}
	wrap->deleteLater();
	reply->deleteLater();
}

void Nao::onBDLrcReply()
{
	qDebug()<<"onBDLrcReply";
	ReplyWrap* wrap = static_cast<ReplyWrap*>(QObject::sender());
	QNetworkReply* reply = wrap->reply();
	if (!reply->error())
	{
		QString lrcBuff = QTextCodec::codecForName("GB2312")->toUnicode(reply->readAll());
		qDebug()<<lrcBuff;
		emit lrcGot(lrcBuff.toUtf8(), wrap->addition());
	}
	else
	{
		emit networkError(reply->errorString(), wrap->addition());
	}
	wrap->deleteLater();
	reply->deleteLater();
}

void Nao::getImage(const QString& songName, const QString& singer, const QVariant& addition)
{
	if (songName.isEmpty() && singer.isEmpty())
		return;
	QString url(DBSongPattern.arg(songName + " " + singer));
	QNetworkReply* reply = GET(url);

	ReplyWrap* wrap = new ReplyWrap();
	wrap->setAddition(addition);
	wrap->setReply(reply);
	bool ok = connect(wrap, SIGNAL(finished()), this,	SLOT(onDBSongReply()));
	Q_ASSERT(ok);
}

void Nao::onDBSongReply()
{
	qDebug()<<"onDBSongReply";
	ReplyWrap* wrap = static_cast<ReplyWrap*>(QObject::sender());
	QNetworkReply* reply = wrap->reply();
	if (!reply->error())
	{
		JsonDataAccess jda;
		QVariantMap response = jda.load(reply).toMap();
		qDebug()<<response;
		if (response["count"] != 0)
		{
			QVariantList musics = response["musics"].toList();
			QVariantMap music = musics.at(0).toMap();
			QString url = music["image"].toString().replace("spic", "lpic");

			QNetworkReply* reply = GET(url);
			ReplyWrap* newWrap = new ReplyWrap();
			newWrap->setAddition(wrap->addition());
			newWrap->setReply(reply);
			bool ok = connect(newWrap, SIGNAL(finished()), this, SLOT(onDBImageReply()));
			Q_ASSERT(ok);
		}
		else
		{
			qDebug()<<"song not found on douban.com";
			emit noImage(wrap->addition());
		}
	}
	else
	{
		qDebug()<<reply->errorString();
	}
	wrap->deleteLater();
	reply->deleteLater();
}

void Nao::onDBImageReply()
{
	qDebug()<<"onDBImageReply";
	ReplyWrap* wrap = static_cast<ReplyWrap*>(QObject::sender());
	QNetworkReply* reply = wrap->reply();
	if (!reply->error())
	{
		emit imageGot(reply->readAll(), wrap->addition());
	}
	else
	{
		qDebug()<<reply->errorString();
	}
	wrap->deleteLater();
	reply->deleteLater();
}
