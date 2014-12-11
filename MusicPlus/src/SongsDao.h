/*
 * SongsDao.h
 *
 *  Created on: 2014-6-1
 *      Author: qGG
 */

#ifndef SONGSDAO_H_
#define SONGSDAO_H_

#include <qobject.h>
#include <bb/data/SqlDataAccess>

class SongsDao: public QObject
{
	Q_OBJECT
public:
	SongsDao(QObject* parent=0);

public slots:
	void createSongsTable();
	void dropSongsTable();
	void clearSongsTable();
	void addRecord(const QVariantMap& record);
	void setLrcName(const QString& fileName, const QString& lrcName);
	void setImageName(const QString& fileName, const QString& imageName);
	void importSongs(const QString& dirPath);
	QVariantList songsList();

private:
	void initDatabase();
	bb::data::SqlDataAccess m_sqlda;
};

#endif /* SONGSDAO_H_ */
