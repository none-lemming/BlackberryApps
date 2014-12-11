/*
 * SongsDao.cpp
 *
 *  Created on: 2014-6-1
 *      Author: qGG
 */

#include "SongsDao.h"
#include <QtSql/QtSql>
#include "Mp3Parser.h"

using namespace bb::data;

const QString DB_PATH = "./data/MusicInfo.db";

SongsDao::SongsDao(QObject* parent)
	:	QObject(parent)
	,	m_sqlda(DB_PATH)
{
	initDatabase();
}

void SongsDao::initDatabase()
{
	//加载驱动
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(DB_PATH);

    if (database.isOpen())
    	return;

    if (database.open() == false)
        qDebug() << database.lastError().text();
}

void SongsDao::createSongsTable()
{
	const QString createSQL = "CREATE TABLE music_info "
							  "(file_name VARCHAR PRIMARY KEY, "
							  "music_name VARCHAR, "
							  "singer_name VARCHAR, "
							  "album_name VARCHAR, "
							  //"lrc_name VARCHAR, "
							  //"image_name VARCHAR, "
							  "duration INTEGER);";
	m_sqlda.execute(createSQL);
	if(!m_sqlda.hasError())
		qDebug() << "Table created";
	else
		qDebug() << m_sqlda.error().errorMessage();
}

void SongsDao::dropSongsTable()
{
	m_sqlda.execute("DROP TABLE IF EXISTS music_info");
	if(!m_sqlda.hasError())
		qDebug() << "Table drop";
}

void SongsDao::clearSongsTable()
{
	m_sqlda.execute("DELETE FROM music_info");
	if(!m_sqlda.hasError())
		qDebug() << "Table clear";
}

void SongsDao::addRecord(const QVariantMap& record)
{
	m_sqlda.execute("INSERT INTO music_info"
	              "(file_name, music_name, singer_name, album_name) "
	              "VALUES (:file_name, :music_name, :singer_name, :album_name)", record);
	if(m_sqlda.hasError())
		qDebug() << m_sqlda.error().errorMessage();
}

void SongsDao::setLrcName(const QString& fileName, const QString& lrcName)
{
	const QString sqlCommand = "UPDATE music_info "
							   "SET lrc_name = :lrc_name "
							   "WHERE file_name = :file_name";
	QVariantMap bindings;
	bindings["file_name"] = fileName;
	bindings["lrc_name"] = lrcName;

	m_sqlda.execute(sqlCommand, bindings);
	if(!m_sqlda.hasError())
		qDebug() << "lrc updated";
	else
		qDebug() << m_sqlda.error().errorMessage();
}

void SongsDao::setImageName(const QString& fileName, const QString& imageName)
{
	const QString sqlCommand = "UPDATE music_info "
							   "SET image_name = :image_name "
							   "WHERE file_name = :file_name";
	QVariantMap bindings;
	bindings["file_name"] = fileName;
	bindings["image_name"] = imageName;

	m_sqlda.execute(sqlCommand, bindings);
	if(!m_sqlda.hasError())
		qDebug() << "image updated";
	else
		qDebug() << m_sqlda.error().errorMessage();
}

void SongsDao::importSongs(const QString& dirPath)
{
	QStringList filters;
	filters<<"*.mp3";

	QDir dir(dirPath);
	QFileInfoList list = dir.entryInfoList(filters,QDir::Files);

	QVariantMap record;
	Mp3Parser parser;
	QString musicName;
	QString fileName;
	qDebug()<<list.size();
	for (int i=0; i<list.size(); i++)
	{
		//qDebug()<<list.at(i).absoluteFilePath();
		parser.setSource(list.at(i).absoluteFilePath());
		fileName = list.at(i).fileName();
		record["file_name"] = fileName;
		musicName = parser.getTitle();
		if (musicName.isEmpty())
		{
			musicName = fileName.left(fileName.lastIndexOf('.'));
		}
		record["music_name"] = musicName;
		record["singer_name"] = parser.getArtist();
		record["album_name"] = parser.getAlbum();
		addRecord(record);
	}
}

QVariantList SongsDao::songsList()
{
	QVariant list = m_sqlda.execute("select * from music_info");
	return list.value<QVariantList>();
}
