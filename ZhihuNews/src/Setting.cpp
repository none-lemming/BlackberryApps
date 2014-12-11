/*
 * Setting.cpp
 *
 *  Created on: 2014-5-10
 *      Author: qGG
 */

#include "Setting.h"
#include <QSettings>

const QString Author = "none.lemming";
const QString AppName = "ReadingPocket";

Setting::Setting(QObject* parent)
	:   QObject(parent)
    ,   m_autoOffline(get("auto_offline", "true").toBool())
    ,   m_theme(get("theme", "bright").toString())
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

bool Setting::autoOffline()
{
    return m_autoOffline;
}

void Setting::setAutoOffline(bool autoOffline)
{
    if (autoOffline == m_autoOffline)
        return;
    m_autoOffline = autoOffline;
    set("auto_offline", autoOffline);
    emit autoOfflineChanged();
}

QString Setting::theme()
{
    return m_theme;
}

void Setting::setTheme(QString theme)
{
    if (theme == m_theme)
        return;
    m_theme = theme;
    set("theme", theme);
    emit themeChanged();
}
