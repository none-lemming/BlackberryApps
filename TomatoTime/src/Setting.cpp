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

const QString Setting::m_author = "none.lemming";
const QString Setting::m_appName = "TomatoTime";

const QString Setting::m_workTimeKey = "WorkTime";
const QString Setting::m_RestTimeKey = "RestTime";
const QString Setting::m_longRestTimeKey = "LongRestTime";
const QString Setting::m_longRestIntervalKey = "LongRestInterval";
const QString Setting::m_ledKey = "LedNeeded";
const QString Setting::m_ledColorKey = "LedColor";
const QString Setting::m_vibrationKey = "VibrationNeeded";
const QString Setting::m_vibrationTimeKey = "VibrationTime";
const QString Setting::m_themePathKey = "ThemePath";
const QString Setting::m_screenUnlockKey = "ScreenUnlockNeeded";
const QString Setting::m_restImageDir= "RestImageDir";

Setting::Setting(QObject* parent)
	: QObject(parent)
	, m_setting(m_author, m_appName)
{
	load();
}

Setting::~Setting()
{
}

void Setting::load()
{
	//TODO 处理颜色
	m_settingMap[m_workTimeKey] = m_setting.value(m_workTimeKey, 1500).toInt();
	m_settingMap[m_RestTimeKey] = m_setting.value(m_RestTimeKey, 300).toInt();
	m_settingMap[m_longRestTimeKey] = m_setting.value(m_longRestTimeKey, 900).toInt();
	m_settingMap[m_longRestIntervalKey] = m_setting.value(m_longRestIntervalKey, 4).toInt();
	m_settingMap[m_ledKey] = m_setting.value(m_ledKey, true).toBool();
	//m_settingMap[m_ledColorKey] = m_setting.value(m_ledColorKey, QVariant(bb::cascades::Color::Blue));
	m_settingMap[m_vibrationKey] = m_setting.value(m_vibrationKey, true).toBool();
	m_settingMap[m_vibrationTimeKey] = m_setting.value(m_vibrationTimeKey, 1000).toInt();
	m_settingMap[m_themePathKey] = m_setting.value(m_themePathKey, "").toString();
	m_settingMap[m_screenUnlockKey] = m_setting.value(m_screenUnlockKey, true).toBool();
	m_settingMap[m_restImageDir] = m_setting.value(m_restImageDir, "").toString();
}

void Setting::set(QString key, QVariant value)
{
	if (m_settingMap[key] == value)
		return;
	m_settingMap[key] = value;
	m_setting.setValue(key, value);
	emit mapChanged();
}

QVariantMap Setting::map()
{
	return m_settingMap;
}

QString Setting::getRestImage()
{
	//qDebug()<<QDir::currentPath();
	//"/accounts/1000/appdata/com.lemming.PomodoroTime.testDev_omodoroTime7e8f29a8"
	//return "file:///accounts/1000/removable/sdcard/test.jpg";
	QStringList filters;
	filters<<"*.jpg"<<"*.png";

	QDir dir(m_settingMap[m_restImageDir].toString());
	//dir.setNameFilters(filters);		设置后在SD卡有问题,只能手动过滤
	//dir.setFilter(QDir::Files);
	QFileInfoList list = dir.entryInfoList();
	qDebug()<<list.size();
	for (int i=0; i<list.size(); i++)
	{
		qDebug()<<list.at(i).fileName();
		if (!list.at(i).fileName().endsWith(".jpg", Qt::CaseInsensitive) || !list.at(i).fileName().endsWith(".png", Qt::CaseInsensitive))
		{
			list.removeAt(i);
			i--;
		}
	}
	qDebug()<<list.size();
	if (list.size() == 0)
		return "asset:///image/RestingImage.png";
	QTime time;
	time= QTime::currentTime();
	qsrand(time.msec()+time.second()*1000);
	int pos = qrand() % list.size();
	qDebug()<<list.at(pos).absoluteFilePath();
	return "file://" + list.at(pos).absoluteFilePath();
}

bool Setting::isExist(QString path)
{
	return QFileInfo(QDir::currentPath() + "/shared" + path).isDir();
}
