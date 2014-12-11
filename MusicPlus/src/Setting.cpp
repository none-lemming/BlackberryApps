/*
 * Setting.cpp
 *
 *  Created on: 2014-5-10
 *      Author: qGG
 */

#include "Setting.h"
#include <bb/cascades/Color>
#include <QTime>
#include <iostream>
#include <QDir>
#include <QSettings>

const QString Setting::Author = "none.lemming";
const QString Setting::AppName = "MusicPlus";

const QString Setting::SongsPath = "SongsPath";
const QString Setting::PlayMode = "PlayMode";
const QString Setting::IndexPath = "IndexPath";

Setting::Setting(QObject* parent)
	: QObject(parent)
{
}

void Setting::set(QString key, QVariant value)
{
	QSettings set(Author, AppName);
	set.setValue(key, value);
}

QVariant Setting::get(QString key, QVariant defaultValue)
{
	QSettings set(Author, AppName);
	return set.value(key, defaultValue);
}

QString Setting::getSongsPath()
{
	return get(SongsPath, "").toString();
}

void Setting::setSongsPath(QString path)
{
	set(SongsPath, path);
}

int Setting::getPlayMode()
{
	return get(PlayMode, 0).toInt();
}

void Setting::setPlayMode(int mode)
{
	set(PlayMode, mode);
}

QVariantList Setting::getIndexPath()
{
	return get(IndexPath).toList();
}

void Setting::setIndexPath(QVariantList indexPath)
{
	set(IndexPath, indexPath);
}
