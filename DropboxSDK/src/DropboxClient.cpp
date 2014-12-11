/*
 * DropboxClient.cpp
 *
 *  Created on: 2014-5-17
 *      Author: qGG
 */

#include "DropboxClient.h"
#include <bb/cascades/QmlDocument>
#include <bb/data/JsonDataAccess>
#include "ReplyWrap.h"

using namespace bb::cascades;
using namespace bb::data;

const QString DropboxClient::API_VERSION = "1";
const QString DropboxClient::WEB_HOST = "www.dropbox.com";
const QString DropboxClient::API_HOST = "api.dropbox.com";
const QString DropboxClient::API_CONTENT_HOST = "api-content.dropbox.com";
const QString DropboxClient::API_PRE = "https://api.dropbox.com/1";
const QString DropboxClient::API_CONTENT_PRE = "https://api-content.dropbox.com/1";

DropboxClient::DropboxClient(const QByteArray& token, const QByteArray& root, const QByteArray& local, QObject* parent)
	:	QObject(parent)
	,	m_token(token)
	,	m_root(root)
	,	m_local(local)
	,	m_networkManager(QmlDocument::defaultDeclarativeEngine()->networkAccessManager())
{
}

QByteArray DropboxClient::processParams(const QDict& params)
{
	QByteArray str;
	if (params.size() > 0)
		str.append(params[0].first + "=" + params[0].second);
	for (int i=1; i<params.size(); i++)
		str.append("&" + params[i].first + "=" + params[i].second);
	return str;
}

QNetworkReply* DropboxClient::GET(QString url, const QDict& params)
{
	//先处理参数,附加到url
	QString str = processParams(params);
	if (!str.isEmpty())
		url.append("?"+str);
	qDebug()<<url;
	QNetworkRequest request;
	request.setUrl(QUrl(url));
	request.setRawHeader("Authorization", "Bearer " + m_token);
	return m_networkManager->get(request);
}

QNetworkReply* DropboxClient::PUT(QString url, const QDict& params, QIODevice* fileObj)
{
	QString str = processParams(params);
	if (!str.isEmpty())
		url.append("?"+str);
	qDebug()<<url;
	if (!fileObj->isOpen())
		fileObj->open(QIODevice::ReadOnly);

	QNetworkRequest request;
	request.setUrl(QUrl(url));
	request.setRawHeader("Authorization", "Bearer " + m_token);
	return m_networkManager->put(request,fileObj);
}

QNetworkReply* DropboxClient::POST(QString url, const QDict& params)
{
	QNetworkRequest request;
	request.setUrl(QUrl(url));
	request.setRawHeader("Authorization", "Bearer " + m_token);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
	return m_networkManager->post(request, processParams(params));
}

/////////////////////////////// 功能函数
void DropboxClient::accountInfo()
{
	QString url(API_PRE+"/account/info");
	QNetworkReply* reply = GET(url,QDict());
	bool ok = connect(reply, SIGNAL(finished()), this,	SLOT(onAccountInfoReply()));
	Q_ASSERT(ok);
}

void DropboxClient::onAccountInfoReply()
{
	qDebug()<<"onAccountInfoReply";
	QNetworkReply* reply = static_cast<QNetworkReply*>(QObject::sender());
	if (!reply->error())
	{
		qDebug()<<"!error";
		JsonDataAccess jda;
		QVariant response = jda.load(reply);
		emit accountInfoFinished(response.toMap());
	}
	else
		emit requestError(reply->errorString());
	reply->deleteLater();
}

void DropboxClient::getFile(const QString& fromPath, int id, int rev)
{
	QString url(API_CONTENT_PRE+"/files/"+m_root+fromPath);
	QDict dict;
	if (rev > 0)
		dict.append(QPair<QString,QString>("rev", QString(rev)));
	qDebug()<<url;
	QNetworkReply* reply = GET(url, dict);
	ReplyWrap* wrap = new ReplyWrap();
	wrap->setId(id);
	wrap->setReply(reply);
	bool ok = connect(wrap, SIGNAL(finished()), this, SLOT(onGetFileReply()));
	Q_ASSERT(ok);
}

void DropboxClient::onGetFileReply()
{
	qDebug()<<"onGetFileReply";
	ReplyWrap* wrap = static_cast<ReplyWrap*>(QObject::sender());
	QNetworkReply* reply = wrap->reply();
	if (!reply->error())
	{
		qDebug()<<"!error";
		QByteArray metaData = reply->rawHeader("x-dropbox-metadata");
		JsonDataAccess jda;
		QVariant response = jda.loadFromBuffer(metaData);
		QVariantMap metadata = response.toMap();
		emit getFileFinished(reply->readAll(), metadata, wrap->id());
	}
	else
		emit requestError(reply->errorString());
	wrap->deleteLater();
	reply->deleteLater();
}

void DropboxClient::putFile(const QString& fullPath, QIODevice* fileObj, int id, bool overwrite, int parent_rev)
{
	QString url(API_CONTENT_PRE+"/files_put/"+m_root+fullPath);
	QDict dict;
	//dict.append(QPair<QString,QString>("overwrite", QString("false")));//TODO , bool到QString的转换
	if (parent_rev > 0)
		dict.append(QPair<QString,QString>("parent_rev", QString(parent_rev)));

	QNetworkReply* reply = PUT(url, dict, fileObj);
	ReplyWrap* wrap = new ReplyWrap();
	wrap->setId(id);
	wrap->setReply(reply);
	bool ok = connect(wrap, SIGNAL(finished()), this, SLOT(onPutFileReply()));
	Q_ASSERT(ok);
}

void DropboxClient::onPutFileReply()
{
	qDebug()<<"onPutFileReply";
	ReplyWrap* wrap = static_cast<ReplyWrap*>(QObject::sender());
	QNetworkReply* reply = wrap->reply();
	if (!reply->error())
	{
		qDebug()<<"!error";
		JsonDataAccess jda;
		QVariant response = jda.load(reply);
		emit putFileFinished(response.toMap());
	}
	else
	{
		emit requestError(reply->errorString());
		qDebug()<<reply->errorString();
	}
	wrap->deleteLater();
	reply->deleteLater();
}

void DropboxClient::createCopyRef(const QString& fromPath)
{
	QString url(API_PRE+"/copy_ref/"+m_root+fromPath);
	QNetworkReply* reply = GET(url, QDict());
	bool ok = connect(reply, SIGNAL(finished()), this,	SLOT(onCreateCopyRefReply()));
	Q_ASSERT(ok);
}

void DropboxClient::onCreateCopyRefReply()
{
	qDebug()<<"onCreateCopyRefReply";
	QNetworkReply* reply = static_cast<QNetworkReply*>(QObject::sender());
	if (!reply->error())
	{
		qDebug()<<"!error";
		JsonDataAccess jda;
		QVariant response = jda.load(reply);
		emit createCopyRefFinished(response.toMap());
	}
	else
		emit requestError(reply->errorString());
	reply->deleteLater();
}

void DropboxClient::addCopyRef(const QString& copyRef, const QString& toPath)
{
	QString url(API_PRE+"/fileops/copy");
	QDict dict;
	dict.append(QPair<QString,QString>("from_copy_ref", copyRef));
	dict.append(QPair<QString,QString>("to_path", toPath));
	dict.append(QPair<QString,QString>("root", m_root));

	QNetworkReply* reply = POST(url, dict);
	bool ok = connect(reply, SIGNAL(finished()), this,	SLOT(onAddCopyRefReply()));
	Q_ASSERT(ok);
}

void DropboxClient::onAddCopyRefReply()
{
	qDebug()<<"onAddCopyRefReply";
	QNetworkReply* reply = static_cast<QNetworkReply*>(QObject::sender());
	if (!reply->error())
	{
		qDebug()<<"!error";
		JsonDataAccess jda;
		QVariant response = jda.load(reply);
		emit addCopyRefFinished(response.toMap());
	}
	else
		emit requestError(reply->errorString());
	reply->deleteLater();
}

void DropboxClient::fileCopy(const QString& fromPath, const QString& toPath)
{
	QString url(API_PRE+"/fileops/copy");
	QDict dict;
	dict.append(QPair<QString,QString>("from_path", fromPath));
	dict.append(QPair<QString,QString>("to_path", toPath));
	dict.append(QPair<QString,QString>("root", m_root));

	QNetworkReply* reply = POST(url, dict);
	bool ok = connect(reply, SIGNAL(finished()), this,	SLOT(onFileCopyReply()));
	Q_ASSERT(ok);
}

void DropboxClient::onFileCopyReply()
{
	qDebug()<<"onFileCopyReply";
	QNetworkReply* reply = static_cast<QNetworkReply*>(QObject::sender());
	if (!reply->error())
	{
		qDebug()<<"!error";
		JsonDataAccess jda;
		QVariant response = jda.load(reply);
		emit fileCopyFinished(response.toMap());
	}
	else
		emit requestError(reply->errorString());
	reply->deleteLater();
}

void DropboxClient::fileMove(const QString& fromPath, const QString& toPath)
{
	QString url(API_PRE+"/fileops/move");
	QDict dict;
	dict.append(QPair<QString,QString>("from_path", fromPath));
	dict.append(QPair<QString,QString>("to_path", toPath));
	dict.append(QPair<QString,QString>("root", m_root));

	QNetworkReply* reply = POST(url, dict);
	bool ok = connect(reply, SIGNAL(finished()), this,	SLOT(onFileMoveReply()));
	Q_ASSERT(ok);
}

void DropboxClient::onFileMoveReply()
{
	qDebug()<<"onFileMoveReply";
	QNetworkReply* reply = static_cast<QNetworkReply*>(QObject::sender());
	if (!reply->error())
	{
		qDebug()<<"!error";
		JsonDataAccess jda;
		QVariant response = jda.load(reply);
		emit fileMoveFinished(response.toMap());
	}
	else
		emit requestError(reply->errorString());
	reply->deleteLater();
}

void DropboxClient::fileCreateFolder(const QString& path)
{
	QString url(API_PRE+"/fileops/create_folder");
	QDict dict;
	dict.append(QPair<QString,QString>("path", path));
	dict.append(QPair<QString,QString>("root", m_root));

	QNetworkReply* reply = POST(url, dict);
	bool ok = connect(reply, SIGNAL(finished()), this,	SLOT(onFileCreateFolderReply()));
	Q_ASSERT(ok);
}

void DropboxClient::onFileCreateFolderReply()
{
	qDebug()<<"onFileCreateFolderReply";
	QNetworkReply* reply = static_cast<QNetworkReply*>(QObject::sender());
	if (!reply->error())
	{
		qDebug()<<"!error";
		JsonDataAccess jda;
		QVariant response = jda.load(reply);
		emit fileCreateFolderFinished(response.toMap());
	}
	else
		emit requestError(reply->errorString());
	reply->deleteLater();
}

void DropboxClient::fileDelete(const QString& path)
{
	QString url(API_PRE+"/fileops/delete");
	QDict dict;
	dict.append(QPair<QString,QString>("path", path));
	dict.append(QPair<QString,QString>("root", m_root));

	QNetworkReply* reply = POST(url, dict);
	bool ok = connect(reply, SIGNAL(finished()), this,	SLOT(onFileDeleteReply()));
	Q_ASSERT(ok);
}

void DropboxClient::onFileDeleteReply()
{
	qDebug()<<"onFileDeleteReply";
	QNetworkReply* reply = static_cast<QNetworkReply*>(QObject::sender());
	if (!reply->error())
	{
		qDebug()<<"!error";
		JsonDataAccess jda;
		QVariant response = jda.load(reply);
		emit fileDeleteFinished(response.toMap());
	}
	else
		emit requestError(reply->errorString());
	reply->deleteLater();
}

void DropboxClient::thumbnail(const QString& fromPath, int id, const QString& size, const QString& format)
{
	QString url(API_CONTENT_PRE+"/thumbnails/"+m_root+fromPath);
	QDict dict;
	dict.append(QPair<QString,QString>("size", size));
	dict.append(QPair<QString,QString>("format", format));

	qDebug()<<url;
	QNetworkReply* reply = GET(url, dict);
	ReplyWrap* wrap = new ReplyWrap();
	wrap->setId(id);
	wrap->setReply(reply);
	bool ok = connect(wrap, SIGNAL(finished()), this, SLOT(onThumbnailReply()));
	Q_ASSERT(ok);
}

void DropboxClient::onThumbnailReply()
{
	qDebug()<<"onThumbnailReply";
	ReplyWrap* wrap = static_cast<ReplyWrap*>(QObject::sender());
	QNetworkReply* reply = wrap->reply();
	if (!reply->error())
	{
		qDebug()<<"!error";
		QByteArray metaData = reply->rawHeader("x-dropbox-metadata");
		JsonDataAccess jda;
		QVariant response = jda.loadFromBuffer(metaData);
		QVariantMap metadata = response.toMap();
		emit thumbnailFinished(reply->readAll(), metadata, wrap->id());
	}
	else
		emit requestError(reply->errorString());
	wrap->deleteLater();
	reply->deleteLater();
}

void DropboxClient::media(const QString& path)
{
	QString url(API_PRE+"/media/"+m_root+path);
	QNetworkReply* reply = POST(url,QDict());
	bool ok = connect(reply, SIGNAL(finished()), this,	SLOT(onMediaReply()));
	Q_ASSERT(ok);
}

void DropboxClient::onMediaReply()
{
	qDebug()<<"onMediaReply";
	QNetworkReply* reply = static_cast<QNetworkReply*>(QObject::sender());
	if (!reply->error())
	{
		qDebug()<<"!error";
		JsonDataAccess jda;
		QVariant response = jda.load(reply);
		emit mediaFinished(response.toMap());
	}
	else
		emit requestError(reply->errorString());
	reply->deleteLater();
}
