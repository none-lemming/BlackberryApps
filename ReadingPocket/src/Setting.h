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

public slots:
    QVariantMap theme(QString themeName);

public slots:
	static void set(QString key, QVariant value);
	static QVariant get(QString key, QVariant defaultValue=QVariant());
};

#endif /* SETTING_H_ */
