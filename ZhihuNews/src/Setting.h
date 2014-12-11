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
	Q_PROPERTY(bool autoOffline READ autoOffline WRITE setAutoOffline NOTIFY autoOfflineChanged);
	Q_PROPERTY(QString theme READ theme WRITE setTheme NOTIFY themeChanged);
public:
	Setting(QObject* parent=0);

public slots:
	static void set(QString key, QVariant value);
	static QVariant get(QString key, QVariant defaultValue=QVariant());

    bool autoOffline();
    void setAutoOffline(bool autoOffline);
    QString theme();
    void setTheme(QString theme);

signals:
    void autoOfflineChanged();
    void themeChanged();

private:
	bool m_autoOffline;
	QString m_theme;
};

#endif /* SETTING_H_ */
