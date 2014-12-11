/*
 * MusicPlayer.cpp
 *
 *  Created on: 2014-5-31
 *      Author: qGG
 */

#include "MusicPlayer.h"
#include "Mp3Parser.h"
#include "SongsDao.h"
#include <bb/cascades/Image>
#include <bb/utility/ImageConverter>
#include "ImageHandler.h"
#include <iostream>
#include "Setting.h"

using namespace bb::cascades;

MusicPlayer::MusicPlayer(QObject* parent)
	:	QObject(parent)
	, 	m_model(new GroupDataModel(QStringList()<<"singer_name", this))
	,	m_lrc(new ArrayDataModel())
	,	m_playMode(0)
{
	qDebug()<<connect(&m_nao, SIGNAL(lrcGot(const QByteArray&, const QVariant&)),
			this, SLOT(onLrcGot(const QByteArray&, const QVariant&)));
	qDebug()<<connect(&m_nao, SIGNAL(imageGot(const QByteArray&, const QVariant&)),
				this, SLOT(onImageGot(const QByteArray&, const QVariant&)));
	qDebug()<<connect(&m_nao, SIGNAL(noLrc(const QVariant&)),
				this, SLOT(onNoLrc(const QVariant&)));
	qDebug()<<connect(&m_nao, SIGNAL(noImage(const QVariant&)),
					this, SLOT(onNoImage(const QVariant&)));
	qDebug()<<connect(&m_nao, SIGNAL(networkError(const QString&, const QVariant&)),
				this, SLOT(onNetworkError(const QString&, const QVariant&)));

	QTime time;
	time= QTime::currentTime();
	qsrand(time.msec()+time.second()*1000);

	//m_musicDir = QDir::currentPath() + "/shared/misc/android/netease/cloudmusic/Music";
	m_model->setGrouping(bb::cascades::ItemGrouping::ByFullValue);

	init();
}

void MusicPlayer::init()
{
	setEmptySong();
	m_musicDir = Setting::getSongsPath();
	if (m_musicDir.isEmpty())
	{
		emit emptyList();
		return;
	}

	initDataModel();
	m_playMode = Setting::getPlayMode();
	QVariantList indexPath = Setting::getIndexPath();
	if (indexPath.isEmpty())
		setSong(m_model->first());
	else
	{
		//读出来的是QString List, after 函数只接受 int List
		QVariantList list;
		list.append(indexPath[0].toInt());
		list.append(indexPath[1].toInt());
		setSong(list);
	}
}

void MusicPlayer::initDataModel()
{
	SongsDao dao;
	m_model->clear();
	m_model->insertList(dao.songsList());
	qDebug()<<m_model->size();
}

void MusicPlayer::setEmptySong()
{
	m_songInfo.clear();
	m_songInfo["lrc"] = QVariant::fromValue(m_lrc);
	m_songInfo["filePath"] = "";
	m_songInfo["singer"] = tr("Import local songs at setting page");
	m_songInfo["name"] = tr("Empty data");
	m_songInfo["imagePath"] = "asset:///icon/cover.jpg";
	m_songInfo["darkImagePath"] = "asset:///icon/darkCover.jpg";
}

int MusicPlayer::importSongs(const QString& dirPath)
{
	//记录路径
	Setting::setSongsPath(dirPath);
	m_musicDir = dirPath;
	//创建相应文件夹
	QDir dir;
	dir.mkpath(dirPath + "/lrc");
	dir.mkpath(dirPath + "/image");
	dir.mkpath(dirPath + "/image_dark");
	QFile hideFile(dirPath + "/image/.nomedia");
	hideFile.open(QIODevice::WriteOnly);
	hideFile.close();
	hideFile.copy(dirPath + "/image_dark/.nomedia");
	//导入数据库
	SongsDao dao;
	dao.dropSongsTable();
	dao.createSongsTable();
	//dao.clearSongsTable();
	dao.importSongs(dirPath);
	//处理播放列表
	initDataModel();
	setSong(m_model->first());
	return m_model->size();
}

void MusicPlayer::clearData()
{
	SongsDao dao;
	dao.dropSongsTable();
	m_musicDir = "";
	Setting::setSongsPath("");
	m_model->clear();
	m_songInfo.clear();
	m_lrc->clear();
	setEmptySong();
	emit songChanged();
}

QVariantList MusicPlayer::nextSong()
{
	qDebug()<<m_playMode;
	qDebug()<<m_songInfo["indexPath"].toList();
	QVariantList indexPath;
	if (m_model->size() > 0)
		switch (m_playMode)
		{
			case 0: //随机播放
				return posToIndexPath(qrand() % m_model->size());
			case 1:   //顺序播放
			case 2: //单曲循环
				indexPath = m_model->after(m_songInfo["indexPath"].toList());
				qDebug()<<indexPath;
				return indexPath;
			default:
				return m_model->after(m_songInfo["indexPath"].toList());
		}
	qDebug()<<"here";
	return QVariantList();
}

QVariantList MusicPlayer::posToIndexPath(int pos)
{
	QVariantList indexPath = m_model->first();
	for (int i=0; i<pos; i++)
	{
		indexPath = m_model->after(indexPath);
	}
	return indexPath;
}

////////////////////
QString MusicPlayer::getSongFilePath(const QVariantList& indexPath)
{
	//必须有	"file://"
	return m_musicDir + "/" + m_model->data(indexPath).toMap()["file_name"].toString();
}

QString MusicPlayer::getLrcPath(const QVariantList& indexPath)
{
	return m_musicDir + "/lrc/" + getSongName(indexPath) + ".lrc";
}

QString MusicPlayer::getImagePath(const QVariantList& indexPath)
{
	return m_musicDir + "/image/" + getSongName(indexPath) + ".jpg";
}

QString MusicPlayer::getDarkImagePath(const QVariantList& indexPath)
{
	return m_musicDir + "/image_dark/" + getSongName(indexPath) + ".jpg";
}

QString MusicPlayer::getSongName(const QVariantList& indexPath)
{
	return m_model->data(indexPath).toMap()["music_name"].toString();
}
QString MusicPlayer::getSongSinger(const QVariantList& indexPath)
{
	return m_model->data(indexPath).toMap()["singer_name"].toString();
}
QString MusicPlayer::getSongAlbum(const QVariantList& indexPath)
{
	return m_model->data(indexPath).toMap()["album_name"].toString();
}


///////////////////////////////////////////

int MusicPlayer::playMode()
{
	return m_playMode;
}

void MusicPlayer::setPlayMode(int mode)
{
	if (mode == m_playMode)
		return;
	m_playMode = mode;
	emit playModeChanged();
}

GroupDataModel* MusicPlayer::model()
{
	return m_model;
}

QVariantMap MusicPlayer::song()
{
	return m_songInfo;
}

void MusicPlayer::setSong(const QVariantList& indexPath)
{
	qDebug()<<"setSong indexPath: "<<indexPath;
	if (indexPath.size() == 0)
	{
		//TODO something
		return;
	}
	if (m_songInfo["indexPath"].toList() == indexPath)
		return;
	m_songInfo["indexPath"] = indexPath;
	m_songInfo["filePath"] = "file://" + getSongFilePath(indexPath);
	m_songInfo["name"] = getSongName(indexPath);
	m_songInfo["singer"] = getSongSinger(indexPath);
	m_songInfo["album"] = getSongAlbum(indexPath);

	//lrc
	m_lrc->clear();
	QString lrcPath = getLrcPath(indexPath);
	if (QFile::exists(lrcPath))
	{
		m_lrc->append(processLrc(lrcPath));
	}
	else
	{
		lrcMsg(tr("downloading lyrics"));
		m_nao.getLrc(getSongName(indexPath), getSongSinger(indexPath), QVariant(indexPath));
	}


	//image
	QString imagePath = getImagePath(indexPath);
	if (!QFile::exists(imagePath))
	{
		m_songInfo["imagePath"] = "asset:///icon/cover.jpg";
		m_songInfo["darkImagePath"] = "asset:///icon/darkCover.jpg";
		m_nao.getImage(getSongName(indexPath), getSongSinger(indexPath), QVariant(indexPath));
	}
	else
	{
		QString darkPath = getDarkImagePath(indexPath);
		if (!QFile::exists(darkPath))
		{
			QImage orignal(imagePath);
			QImage dark = ImageHandler::GaussianBlur(orignal, 30, 0.7);
			dark.save(darkPath);
		}
		m_songInfo["darkImagePath"] = "file://" + getDarkImagePath(indexPath);
		m_songInfo["imagePath"] = "file://" + imagePath;
	}

	emit songChanged();
}

/////////////////////////// slots

void MusicPlayer::onImageGot(const QByteArray& buff, const QVariant& addition)
{
	QDateTime start = QDateTime::currentDateTime();
	qDebug()<<start.toString("mm-ss-zzz");

	//保存image文件
	QVariantList indexPath = addition.toList();
	QString imagePath = getImagePath(indexPath);
	QImage image = QImage::fromData(buff);
	qDebug()<<imagePath;
	qDebug()<<image.save(imagePath);
	QImage dark = ImageHandler::GaussianBlur(image, 30, 0.7);
	qDebug()<<dark.save(getDarkImagePath(indexPath));

	if (indexPath != m_songInfo["indexPath"].toList())
		return;

	//更新m_songInfo
	m_songInfo["darkImagePath"] = "file://" + getDarkImagePath(indexPath);
	m_songInfo["imagePath"] = "file://" + imagePath;
	emit songChanged();
	QDateTime finish = QDateTime::currentDateTime();
	qDebug()<<finish.toString("mm-ss-zzz");
	qDebug()<<finish.msecsTo(start);
}

void MusicPlayer::onNoImage(const QVariant& addition)
{
	Mp3Parser parser;
	parser.setSource(getSongFilePath(addition.toList()));
	onImageGot(parser.getImage(), addition);
}

void MusicPlayer::onLrcGot(const QByteArray& buff, const QVariant& addition)
{
	//保存lrc文件
	QVariantList indexPath = addition.toList();
	QString lrcPath = getLrcPath(indexPath);
	QFile file(lrcPath);
	//qDebug()<<lrcPath;
	file.open(QIODevice::WriteOnly);
	file.write(buff);
	file.close();

	if (indexPath != m_songInfo["indexPath"].toList())
		return;

	//更新m_songInfo
	m_lrc->clear();
	m_lrc->append(processLrc(lrcPath));
	m_songInfo["lrc"] = QVariant::fromValue(m_lrc);
	emit songChanged();
}

QVariantList MusicPlayer::processLrc(const QString& lrcPath)
{
	QFile file(lrcPath);
	file.open(QIODevice::ReadOnly);
	QRegExp rx("\\[(\\d+):(\\d+).(\\d+)\\](.*)");
	QVariantList list;

	while (!file.atEnd())
	{
		QByteArray buff = file.readLine();
		QString line = QString::fromUtf8(buff);
		if (line.indexOf(rx) >= 0)
		{
			QString lrc = rx.cap(4);
			lrc = lrc.trimmed();
			if (lrc.isEmpty())
				continue;
			QVariantMap map;
			map["offset"] = rx.cap(1).toInt()*60000 + rx.cap(2).toInt()*1000 + rx.cap(3).toInt()*10;
			map["lrc"] = lrc;
			list << map;
		}
	}
	//qDebug()<<list;
	return list;
}

void MusicPlayer::lrcMsg(const QString& msg)
{
	QVariantMap map;
	map["offset"] = 0;
	map["lrc"] = msg;
	m_lrc->clear();
	m_lrc->append(map);
	emit songChanged();
}

void MusicPlayer::onNoLrc(const QVariant& addition)
{
	if (addition.toList() != m_songInfo["indexPath"].toList())
		return;
	lrcMsg(tr("lyrics not found"));
}

void MusicPlayer::onNetworkError(const QString& msg, const QVariant& addition)
{
	qDebug()<<msg;
	if (addition.toList() != m_songInfo["indexPath"].toList())
		return;
	lrcMsg(tr("network error"));
}

void MusicPlayer::saveData()
{
	qDebug()<<"onQuit";
	Setting::setPlayMode(m_playMode);
	Setting::setIndexPath(m_songInfo["indexPath"].toList());
}
