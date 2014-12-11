/*
 * Setting.h
 *
 *  Created on: 2014-5-10
 *      Author: qGG
 */

#ifndef SETTING_H_
#define SETTING_H_

#include <QObject>
#include <QSettings>

class QSettings;

class Setting : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QVariantMap map READ map NOTIFY mapChanged);
public:
	Setting(QObject* parent=0);
	virtual ~Setting();
	void load();

	Q_INVOKABLE void set(QString key, QVariant value);
	Q_INVOKABLE QString getRestImage();
	Q_INVOKABLE bool isExist(QString path);

signals:
	void mapChanged();

private:

	QVariantMap map();

    static const QString m_author;
    static const QString m_appName;

    static const QString m_workTimeKey;
    static const QString m_RestTimeKey;
    static const QString m_longRestTimeKey;
    static const QString m_longRestIntervalKey;
    static const QString m_ledKey;
    static const QString m_ledColorKey;
    static const QString m_vibrationKey;
    static const QString m_vibrationTimeKey;
    static const QString m_themePathKey;
    static const QString m_screenUnlockKey;
    static const QString m_restImageDir;

	QVariantMap m_settingMap;
	QSettings m_setting;
};

#endif /* SETTING_H_ */
