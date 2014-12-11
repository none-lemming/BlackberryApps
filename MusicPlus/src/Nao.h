/*
 * Nao.h
 *
 *  Created on: 2014-6-22
 *      Author: qGG
 */

#ifndef NAO_H_
#define NAO_H_

#include <QObject>

class QNetworkReply;
typedef QList< QPair<QString,QString> > QDict;

class Nao : public QObject
{
	Q_OBJECT
public:
	Nao(QObject* parent=0);

	void getLrc(const QString& songName, const QString& singer, const QVariant& addition=QVariant());
	void getImage(const QString& songName, const QString& singer, const QVariant& addition=QVariant());

signals:
	void lrcGot(const QByteArray& buff, const QVariant& addition);
	void noLrc(const QVariant& addition);
	void networkError(const QString& msg, const QVariant& addition);
	void imageGot(const QByteArray& buff, const QVariant& addition);
	void noImage(const QVariant& addition);

private slots:
	void onBDSongReply();
	void onBDLrcReply();
	void onDBSongReply();
	void onDBImageReply();

private:
	QByteArray processParams(const QDict& params);
	QNetworkReply* GET(QString url, const QDict& params=QDict());

	static const QString BDSongPattern;
	static const QString BDLrcPattern;
	static const QString DBSongPattern;
	static const QString DBImagePattern;
};

#endif /* NAO_H_ */
