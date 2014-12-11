/*
 * DropboxClient.h
 *
 *  Created on: 2014-5-17
 *      Author: qGG
 */

#ifndef DROPBOXCLIENT_H_
#define DROPBOXCLIENT_H_

#include <QObject>
class QNetworkReply;
class QNetworkAccessManager;
typedef QList< QPair<QString,QString> > QDict;

class DropboxClient : public QObject
{
	Q_OBJECT
public:
	DropboxClient(const QByteArray& token, const QByteArray& root, const QByteArray& local, QObject* parent=0);

	//实现的功能,结果附带在响应的信号中
	void accountInfo();
	void getFile(const QString& fromPath, int id=-1, int rev=-1);	//fromPaht前缀要有/
	//fullPath,包括文件名	parent_rev,版次,会忽略overwrite,如果该版次是服务器中的最近版次,文件被重写,否则重命名
	void putFile(const QString& fullPath, QIODevice* fileObj, int id=-1, bool overwrite=false, int parent_rev=-1);
	void createCopyRef(const QString& fromPath);
	void addCopyRef(const QString& copyRef, const QString& toPath);
	void fileCopy(const QString& fromPath, const QString& toPath);
	void fileMove(const QString& fromPath, const QString& toPath);
	void fileCreateFolder(const QString& path);
	void fileDelete(const QString& path);
	void thumbnail(const QString& fromPath, int id=-1, const QString& size="m", const QString& format="JPEG");
	void media(const QString& path);

	//功能函数调用完成的信号,附带有结果,json数据参考官方说明来解析
signals:
	void requestError(const QString& msg);
	void accountInfoFinished(const QVariantMap& info);
	void getFileFinished(const QByteArray& buff, const QVariantMap& metadata, int id);
	void putFileFinished(const QVariantMap& metadata);
	void createCopyRefFinished(const QVariantMap& ref);
	void addCopyRefFinished(const QVariantMap& metadata);
	void fileCopyFinished(const QVariantMap& metadata);
	void fileMoveFinished(const QVariantMap& metadata);
	void fileCreateFolderFinished(const QVariantMap& metadata);
	void fileDeleteFinished(const QVariantMap& metadata);
	void thumbnailFinished(const QByteArray& buff, const QVariantMap& metadata, int id);
	void mediaFinished(const QVariantMap& url);

protected slots:
	void onAccountInfoReply();
	void onGetFileReply();
	void onPutFileReply();
	void onCreateCopyRefReply();
	void onAddCopyRefReply();
	void onFileCopyReply();
	void onFileMoveReply();
	void onFileCreateFolderReply();
	void onFileDeleteReply();
	void onThumbnailReply();
	void onMediaReply();

protected:
	QByteArray processParams(const QDict& params);
	QNetworkReply* GET(QString url, const QDict& params);
	QNetworkReply* PUT(QString url, const QDict& params, QIODevice* fileObj);
	QNetworkReply* POST(QString url, const QDict& params);

	static const QString API_VERSION;
	static const QString WEB_HOST;
	static const QString API_HOST;
	static const QString API_CONTENT_HOST;
	static const QString API_PRE;
	static const QString API_CONTENT_PRE;

	QByteArray m_token;
	QByteArray m_root;
	QByteArray m_local;
	QNetworkAccessManager* m_networkManager;
};

#endif /* DROPBOXCLIENT_H_ */
