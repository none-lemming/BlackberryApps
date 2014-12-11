/*
 * PocketAuth.h
 *
 *  Created on: 2014年8月28日
 *      Author: qGG
 */

#ifndef POCKETAUTH_H_
#define POCKETAUTH_H_

#include <QObject>
typedef QList< QPair<QString,QString> > QDict;

class PocketAuth: public QObject
{
    Q_OBJECT
    //Q_PROPERTY(int status READ status WRITE setStatus NOTIFY statusChanged);
public:
    PocketAuth(QObject* parent=0);
    int status();
    void setStatus(int status);

public slots:
    void getCode();
    QString getAuthUrl();
    void getToken();

signals:
    void codeGot(QString code);
    void tokenGot(QString token, QString userName);
    void authError(QString error);
    void statusChanged(int status);

private slots:
    void onGetCode();
    void onGetToken();

private:
    QString m_code;
    int m_status;
};

#endif /* POCKETAUTH_H_ */
