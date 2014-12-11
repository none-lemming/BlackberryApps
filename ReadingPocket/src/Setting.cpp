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

QVariantMap Setting::theme(QString themeName)
{
    QVariantMap map;
    if (themeName == "bright")
    {
        map["theme_name"] = "bright";
        map["title_color"] = "#000000";
        map["info_color"] = "#999999";
        map["background_color"] = "#f9f9f9";
    }
    else if (themeName == "dark")
    {
        map["theme_name"] = "dark";
        map["title_color"] = "#cccccc";
        map["info_color"] = "#b9b9b9";
        map["background_color"] = "#252525";
    }
    else
    {
        map["theme_name"] = "custom";
        map["title_color"] = "#5b4636";
        map["info_color"] = "#9c846d";
        map["background_color"] = "#fbf0d9";
    }
    return map;
}
