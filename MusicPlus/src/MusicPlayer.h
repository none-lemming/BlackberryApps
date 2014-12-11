/*
 * MusicPlayer.h
 *
 *  Created on: 2014-5-31
 *      Author: qGG
 */

#ifndef MUSICPLAYER_H_
#define MUSICPLAYER_H_

#include <qobject.h>
#include <bb/cascades/GroupDataModel>
#include <bb/cascades/ArrayDataModel>
#include "Nao.h"

class MusicPlayer: public QObject
{
	Q_OBJECT
	Q_PROPERTY(bb::cascades::GroupDataModel* model READ model CONSTANT);
	Q_PROPERTY(QVariantMap song READ song NOTIFY songChanged);
	Q_PROPERTY(int playMode READ playMode WRITE setPlayMode NOTIFY playModeChanged);
public:
	MusicPlayer(QObject* parent=0);

public slots:
	int importSongs(const QString& dirPath);
	void clearData();
	QVariantList nextSong();
	void setSong(const QVariantList& indexPath);
	void saveData();

	QString getSongFilePath(const QVariantList& indexPath);
	QString getLrcPath(const QVariantList& indexPath);
	QString getImagePath(const QVariantList& indexPath);
	QString getDarkImagePath(const QVariantList& indexPath);
	QString getSongName(const QVariantList& indexPath);
	QString getSongSinger(const QVariantList& indexPath);
	QString getSongAlbum(const QVariantList& indexPath);

signals:
	void songChanged();
	void currSongChanged();
	void playModeChanged();
	void emptyList();

private slots:
	void onLrcGot(const QByteArray& buff, const QVariant& addition);
	void onNoLrc(const QVariant& addition);
	void onNetworkError(const QString& msg, const QVariant& addition);
	void onImageGot(const QByteArray& buff, const QVariant& addition);
	void onNoImage(const QVariant& addition);

private:
	void init();
	void initDataModel();
	void setEmptySong();
	QVariantList processLrc(const QString& lrcPath);
	void lrcMsg(const QString& msg);
	QVariantList posToIndexPath(int pos);

	bb::cascades::GroupDataModel* model();
	QVariantMap song();
	int playMode();
	void setPlayMode(int mode);

	bb::cascades::GroupDataModel* m_model;
	bb::cascades::ArrayDataModel* m_lrc;
	QString m_musicDir;
	QVariantMap m_songInfo;
	Nao m_nao;
	int m_playMode;
};

#endif /* MUSICPLAYER_H_ */
