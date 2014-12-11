/*
 * Setting.h
 *
 *  Created on: 2014-5-10
 *      Author: qGG
 */

#ifndef SETTING_H_
#define SETTING_H_

#include <QObject>

class QSettings;

class Setting : public QObject
{
	Q_OBJECT
public:
	Setting(QObject* parent=0);

	Q_INVOKABLE static void set(QString key, QVariant value);
	Q_INVOKABLE static QVariant get(QString key, QVariant defaultValue=QVariant());
	Q_INVOKABLE static QString getSongsPath();
	Q_INVOKABLE static void setSongsPath(QString path);
	Q_INVOKABLE static int getPlayMode();
	Q_INVOKABLE static void setPlayMode(int mode);
	Q_INVOKABLE static QVariantList getIndexPath();
	Q_INVOKABLE static void setIndexPath(QVariantList indexPath);

private:

    static const QString Author;
    static const QString AppName;

    static const QString SongsPath;
    static const QString PlayMode;
    static const QString IndexPath;
};

#endif /* SETTING_H_ */
